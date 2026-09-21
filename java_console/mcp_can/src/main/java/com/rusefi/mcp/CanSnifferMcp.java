package com.rusefi.mcp;

import com.devexperts.logging.Logging;
import com.rusefi.io.can.PCanHelper;
import com.rusefi.io.can.slcan.SlcanClient;
import com.rusefi.util.HexBinary;
import org.json.simple.JSONArray;
import org.json.simple.JSONObject;
import org.json.simple.parser.JSONParser;
import peak.can.basic.PCANBasic;
import peak.can.basic.TPCANBaudrate;
import peak.can.basic.TPCANHandle;
import peak.can.basic.TPCANMsg;
import peak.can.basic.TPCANStatus;
import peak.can.basic.TPCANType;

import java.io.BufferedReader;
import java.io.InputStreamReader;
import java.io.IOException;
import java.io.PrintStream;
import java.nio.charset.StandardCharsets;
import java.util.ArrayDeque;
import java.util.Arrays;
import java.util.Deque;
import java.util.Locale;
import java.util.regex.Pattern;

import static com.devexperts.logging.Logging.getLogging;

/**
 * MCP (Model Context Protocol) server exposing read-only CAN bus sniffing via PCAN hardware or built-in SLCAN.
 *
 * <p>Tools provided:
 * <ul>
 *     <li><code>connect</code> — open the selected adapter and start the listener thread</li>
 *     <li><code>read_packets</code> — return buffered CAN packets since a given sequence number</li>
 *     <li><code>wait_for_packet</code> — block until a packet matching criteria arrives</li>
 *     <li><code>status</code> — report connection state and buffer statistics</li>
 * </ul>
 *
 * <p>This is a read/listen-only server — no CAN write operations are exposed.
 */
public class CanSnifferMcp {
    private static final Logging log = getLogging(CanSnifferMcp.class);

    private static final String PROTOCOL_VERSION = "2024-11-05";
    private static final String SERVER_NAME = "rusefi-can-sniffer-mcp";
    private static final String SERVER_VERSION = "0.1.0";

    private final PrintStream out;
    private final BufferedReader in;
    private final JSONParser parser = new JSONParser();
    private final Object writeLock = new Object();

    /** Ring buffer of recently received CAN packets (newest at tail). */
    private final Deque<CanMessage> messageBuffer = new ArrayDeque<>();
    private final int messageBufferCapacity = 50_000;
    private final Object messageLock = new Object();
    private long messageSeq = 0;

    private volatile PCANBasic can;
    private volatile Thread listenerThread;
    private volatile boolean running;
    private final Object connectLock = new Object();

    /** Which PCAN channel this server sniffs. Defaults to USBBUS1; override with --channel. */
    private volatile TPCANHandle channel = PCanHelper.CHANNEL;

    private String backend = "pcan";
    private String port;
    private volatile String connectedPort;
    private volatile String lastError;
    private final SlcanFactory slcanFactory;

    interface SlcanConnection extends AutoCloseable {
        String getPort();
        default boolean includesBus() { return false; }
        String readLine(int timeoutMs) throws IOException;
        void pollStatus() throws IOException;
        void close();
    }

    interface SlcanFactory {
        SlcanConnection open(String port) throws IOException;
    }

    private static SlcanConnection openSlcan(String port) throws IOException {
        SlcanClient client = port == null ? SlcanClient.findAndConnect(System.err::println)
                : SlcanClient.connect(port, System.err::println);
        if (client == null) {
            throw new IOException(port == null ? "No SLCAN port found" : "Cannot connect to SLCAN port " + port);
        }
        return new SlcanConnection() {
            public String getPort() { return client.getPort(); }
            public String readLine(int timeoutMs) throws IOException { return client.readLine(timeoutMs); }
            public void pollStatus() throws IOException { client.pollStatus(); }
            public void close() { client.close(); }
        };
    }

    public CanSnifferMcp() {
        this(new BufferedReader(new InputStreamReader(System.in, StandardCharsets.UTF_8)),
                new PrintStream(System.out, true, StandardCharsets.UTF_8));
    }

    /** Test-friendly constructor. */
    public CanSnifferMcp(BufferedReader in, PrintStream out) {
        this(in, out, CanSnifferMcp::openSlcan);
    }

    CanSnifferMcp(BufferedReader in, PrintStream out, SlcanFactory slcanFactory) {
        this.in = in;
        this.out = out;
        this.slcanFactory = slcanFactory;
    }

    /**
     * Resolve a --channel value to a TPCANHandle. Accepts a bare USB bus number
     * ("1", "2", ...) or a full enum name ("PCAN_USBBUS2").
     */
    static TPCANHandle resolveChannel(String value) {
        String v = value.trim();
        try {
            if (v.matches("\\d+")) {
                return TPCANHandle.valueOf("PCAN_USBBUS" + v);
            }
            return TPCANHandle.valueOf(v.toUpperCase());
        } catch (IllegalArgumentException e) {
            throw new IllegalArgumentException("Unknown PCAN channel '" + value
                    + "' (use a USB bus number like 1 or 2, or an enum name like PCAN_USBBUS2)");
        }
    }

    void configure(String... args) {
        boolean channelSpecified = false;
        for (int i = 0; i < args.length; i++) {
            String option = args[i];
            if (i + 1 == args.length) {
                throw new IllegalArgumentException("Missing value for " + option);
            }
            String value = args[++i];
            switch (option) {
                case "--backend": backend = value.toLowerCase(Locale.ROOT); break;
                case "--port": port = value; break;
                case "--channel": channel = resolveChannel(value); channelSpecified = true; break;
                default: throw new IllegalArgumentException("Unknown option " + option);
            }
        }
        if (!backend.equals("pcan") && !backend.equals("slcan")) {
            throw new IllegalArgumentException("Backend must be pcan or slcan");
        }
        if (port != null && (!backend.equals("slcan") || port.trim().isEmpty())) {
            throw new IllegalArgumentException("--port requires --backend slcan and a non-empty serial port");
        }
        if (channelSpecified && backend.equals("slcan")) {
            throw new IllegalArgumentException("--channel is for PCAN; SLCAN bus selection is configured in the ECU tune");
        }
    }

    public static void main(String[] args) {
        if (Arrays.asList(args).contains("--help") || Arrays.asList(args).contains("-h")) {
            System.err.println("Usage: CanSnifferMcp [--backend pcan|slcan] [--channel <n|PCAN_USBBUSn>] [--port <serialPort>]");
            System.err.println("Read-only CAN sniffing over stdio MCP. Default: PCAN USB bus 1 at 500 kbit/s.");
            System.err.println("SLCAN: optional --port (otherwise autodetect); bus and bitrate are set in the ECU tune.");
            return;
        }
        try {
            CanSnifferMcp server = new CanSnifferMcp();
            server.configure(args);
            // Serial discovery and shared IO may print diagnostics directly to System.out.
            // The server already holds the original stdout for JSON-RPC responses.
            System.setOut(System.err);
            server.run();
        } catch (Throwable t) {
            log.error("MCP server fatal", t);
            System.err.println("MCP server fatal: " + t);
            System.exit(1);
        }
    }

    /** Exposed for tests. */
    public void runForTests() throws Exception {
        run();
    }

    private void run() throws Exception {
        log.info("rusEFI CAN Sniffer MCP server starting");

        try {
            readRequests();
        } finally {
            shutdown();
        }
    }

    private void readRequests() throws IOException {
        String line;
        while ((line = in.readLine()) != null) {
            line = line.trim();
            if (line.isEmpty()) continue;
            try {
                Object parsed = parser.parse(line);
                if (parsed instanceof JSONObject) {
                    handleMessage((JSONObject) parsed);
                }
            } catch (Throwable t) {
                log.error("Failed to handle line: " + line, t);
            }
        }
        log.info("stdin closed, exiting");
    }

    private void shutdown() {
        running = false;
        Thread t = listenerThread;
        if (t != null) {
            t.interrupt();
            try {
                t.join(3000);
            } catch (InterruptedException e) {
                Thread.currentThread().interrupt();
            }
        }
        PCANBasic c = can;
        if (c != null) {
            try { c.Uninitialize(channel); } catch (Throwable ignored) {}
        }
    }

    // -----------------------------------------------------------------------------------
    // CAN listener thread
    // -----------------------------------------------------------------------------------

    private void startListenerThread() {
        running = true;
        listenerThread = new Thread(() -> {
            log.info("CAN listener thread started");
            while (running) {
                try {
                    TPCANMsg rx = new TPCANMsg(Byte.MAX_VALUE);
                    TPCANStatus status = can.Read(channel, rx, null);
                    if (status == TPCANStatus.PCAN_ERROR_OK) {
                        byte[] data = new byte[rx.getLength()];
                        System.arraycopy(rx.getData(), 0, data, 0, rx.getLength());
                        synchronized (messageLock) {
                            long seq = ++messageSeq;
                            messageBuffer.addLast(new CanMessage(seq, System.currentTimeMillis(),
                                    rx.getID(), rx.getLength(), data));
                            while (messageBuffer.size() > messageBufferCapacity)
                                messageBuffer.removeFirst();
                            messageLock.notifyAll();
                        }
                    } else if (status == TPCANStatus.PCAN_ERROR_QRCVEMPTY) {
                        Thread.sleep(1);
                    } else {
                        log.info("CAN read status: " + status);
                        Thread.sleep(10);
                    }
                } catch (InterruptedException e) {
                    break;
                } catch (Throwable t) {
                    log.error("CAN listener error", t);
                    try { Thread.sleep(100); } catch (InterruptedException ignored) { break; }
                }
            }
            log.info("CAN listener thread stopped");
        }, "CAN-sniffer-listener");
        listenerThread.setDaemon(true);
        listenerThread.start();
    }

    private void startSlcanListener(SlcanConnection connection) {
        running = true;
        listenerThread = new Thread(() -> {
            try (SlcanConnection owned = connection) {
                long nextPoll = System.nanoTime();
                while (running && !Thread.currentThread().isInterrupted()) {
                    SlcanClient.Frame frame = SlcanClient.Frame.parse(owned.readLine(200));
                    if (frame != null) {
                        synchronized (messageLock) {
                            CanMessage message = new CanMessage(++messageSeq, System.currentTimeMillis(),
                                    frame.id, frame.dlc, frame.data);
                            message.busIndex = frame.busIndex;
                            message.extended = frame.extended;
                            message.rtr = frame.rtr;
                            message.slcanTimestamp = frame.timestamp;
                            messageBuffer.addLast(message);
                            while (messageBuffer.size() > messageBufferCapacity) {
                                messageBuffer.removeFirst();
                            }
                            messageLock.notifyAll();
                        }
                    }
                    if (System.nanoTime() >= nextPoll) {
                        owned.pollStatus();
                        nextPoll = System.nanoTime() + 1_000_000_000L;
                    }
                }
            } catch (Exception e) {
                lastError = "SLCAN listener failed: " + e.getMessage();
                log.error(lastError, e);
            } finally {
                running = false;
                synchronized (messageLock) {
                    messageLock.notifyAll();
                }
            }
        }, "SLCAN-sniffer-listener");
        listenerThread.setDaemon(true);
        listenerThread.start();
    }

    // -----------------------------------------------------------------------------------
    // CAN message record
    // -----------------------------------------------------------------------------------

    static final class CanMessage {
        final long seq;
        final long timestamp;
        final int id;
        final int length;
        final byte[] data;
        Integer busIndex;
        Boolean extended;
        Boolean rtr;
        String slcanTimestamp;

        CanMessage(long seq, long timestamp, int id, int length, byte[] data) {
            this.seq = seq;
            this.timestamp = timestamp;
            this.id = id;
            this.length = length;
            this.data = data;
        }

        @SuppressWarnings("unchecked")
        JSONObject toJson() {
            JSONObject o = new JSONObject();
            o.put("seq", seq);
            o.put("timestamp", timestamp);
            o.put("id", String.format("0x%X", id));
            o.put("idDec", id);
            o.put("length", length);
            o.put("data", HexBinary.printByteArray(data));
            if (extended != null) {
                o.put("busIndex", busIndex);
                o.put("extended", extended);
                o.put("rtr", rtr);
            }
            if (slcanTimestamp != null) {
                o.put("slcanTimestamp", slcanTimestamp);
            }
            return o;
        }
    }

    // -----------------------------------------------------------------------------------
    // JSON-RPC dispatch
    // -----------------------------------------------------------------------------------

    @SuppressWarnings("unchecked")
    private void handleMessage(JSONObject msg) {
        Object id = msg.get("id");
        String method = (String) msg.get("method");
        if (method == null) return;
        JSONObject params = (JSONObject) msg.get("params");

        try {
            switch (method) {
                case "initialize":
                    respond(id, initialize());
                    break;
                case "notifications/initialized":
                case "notifications/cancelled":
                    break;
                case "tools/list":
                    respond(id, toolsList());
                    break;
                case "tools/call":
                    respond(id, toolsCall(params));
                    break;
                case "ping":
                    respond(id, new JSONObject());
                    break;
                default:
                    if (id != null) error(id, -32601, "Method not found: " + method);
            }
        } catch (Throwable t) {
            log.error("Error handling " + method, t);
            if (id != null) error(id, -32603, t.toString());
        }
    }

    @SuppressWarnings("unchecked")
    private JSONObject initialize() {
        JSONObject result = new JSONObject();
        result.put("protocolVersion", PROTOCOL_VERSION);
        JSONObject caps = new JSONObject();
        JSONObject tools = new JSONObject();
        tools.put("listChanged", false);
        caps.put("tools", tools);
        result.put("capabilities", caps);
        JSONObject info = new JSONObject();
        info.put("name", SERVER_NAME);
        info.put("version", SERVER_VERSION);
        result.put("serverInfo", info);
        return result;
    }

    // -----------------------------------------------------------------------------------
    // Tools
    // -----------------------------------------------------------------------------------

    @SuppressWarnings("unchecked")
    private JSONObject toolsList() {
        JSONArray tools = new JSONArray();
        tools.add(tool("connect",
                "Open the adapter selected by --backend (pcan or slcan) and start listening for CAN packets.",
                emptyObjectSchema()));
        tools.add(tool("read_packets",
                "Return CAN packets from the in-memory ring buffer.",
                schemaObject(new String[][]{
                        {"sinceSeq", "integer", "Only return packets with seq strictly greater than this."},
                        {"maxPackets", "integer", "Maximum number of packets to return. Default 200."},
                        {"idFilter", "string", "Optional hex CAN ID filter (e.g. '0x7E8'). Only packets with this ID are returned."}
                }, new String[]{}, false)));
        tools.add(tool("wait_for_packet",
                "Block until a CAN packet matching criteria arrives, or timeout.",
                schemaObject(new String[][]{
                        {"idFilter", "string", "Optional hex CAN ID to match (e.g. '0x7E8')."},
                        {"dataRegex", "string", "Optional Java regex matched against the hex data string."},
                        {"timeoutMs", "integer", "Wait timeout in ms. Default 10000."},
                        {"sinceSeq", "integer", "Only consider packets with seq strictly greater than this."}
                }, new String[]{}, false)));
        tools.add(tool("status",
                "Return connection state and buffer statistics.",
                emptyObjectSchema()));

        JSONObject result = new JSONObject();
        result.put("tools", tools);
        return result;
    }

    @SuppressWarnings("unchecked")
    private JSONObject toolsCall(JSONObject params) {
        String name = (String) params.get("name");
        JSONObject args = (JSONObject) params.getOrDefault("arguments", new JSONObject());
        JSONObject toolResult;
        try {
            switch (name) {
                case "connect":       toolResult = doConnect(); break;
                case "read_packets":  toolResult = doReadPackets(args); break;
                case "wait_for_packet": toolResult = doWaitForPacket(args); break;
                case "status":        toolResult = doStatus(); break;
                default:
                    return toolError("Unknown tool: " + name);
            }
        } catch (Throwable t) {
            log.error("Tool '" + name + "' failed", t);
            return toolError(t.toString());
        }
        return wrapToolResult(toolResult);
    }

    @SuppressWarnings("unchecked")
    private JSONObject doConnect() {
        synchronized (connectLock) {
            if (running) {
                JSONObject r = doStatus();
                r.put("message", "Already connected");
                return r;
            }
            if (backend.equals("slcan")) {
                // A failed reader closes its port before publishing running=false.
                if (listenerThread != null && listenerThread.isAlive()) {
                    throw new IllegalStateException("Previous SLCAN reader is still stopping; retry connect");
                }
                lastError = null;
                connectedPort = null;
                try {
                    SlcanConnection connection = slcanFactory.open(port);
                    connectedPort = connection.getPort();
                    startSlcanListener(connection);
                } catch (IOException e) {
                    lastError = e.getMessage();
                }
                return doStatus();
            }
            can = PCanHelper.create();
            TPCANStatus status = can.Initialize(channel, TPCANBaudrate.PCAN_BAUD_500K,
                    TPCANType.PCAN_TYPE_NONE, 0, (short) 0);
            if (status != TPCANStatus.PCAN_ERROR_OK) {
                JSONObject r = new JSONObject();
                r.put("connected", false);
                r.put("channel", channel.name());
                r.put("error", "PCAN init failed: " + status);
                return r;
            }
            startListenerThread();
            JSONObject r = new JSONObject();
            r.put("connected", true);
            r.put("channel", channel.name());
            return r;
        }
    }

    @SuppressWarnings("unchecked")
    private JSONObject doReadPackets(JSONObject args) {
        ensureConnected();
        long sinceSeq = asLong(args.get("sinceSeq"), -1L);
        int maxPackets = (int) asLong(args.get("maxPackets"), 200L);
        Integer idFilter = parseCanId((String) args.get("idFilter"));

        JSONArray arr = new JSONArray();
        long latestSeq;
        synchronized (messageLock) {
            latestSeq = messageSeq;
            for (CanMessage m : messageBuffer) {
                if (m.seq <= sinceSeq) continue;
                if (idFilter != null && m.id != idFilter) continue;
                arr.add(m.toJson());
                if (arr.size() >= maxPackets) break;
            }
        }
        JSONObject o = new JSONObject();
        o.put("packets", arr);
        o.put("latestSeq", latestSeq);
        return o;
    }

    @SuppressWarnings("unchecked")
    private JSONObject doWaitForPacket(JSONObject args) throws InterruptedException {
        ensureConnected();
        long timeout = asLong(args.get("timeoutMs"), 10_000L);
        long sinceSeq = asLong(args.get("sinceSeq"), -1L);
        Integer idFilter = parseCanId((String) args.get("idFilter"));
        String dataRegex = (String) args.get("dataRegex");
        Pattern dataPattern = dataRegex != null ? Pattern.compile(dataRegex) : null;

        long deadline = System.currentTimeMillis() + timeout;
        synchronized (messageLock) {
            while (true) {
                for (CanMessage m : messageBuffer) {
                    if (m.seq <= sinceSeq) continue;
                    if (idFilter != null && m.id != idFilter) continue;
                    if (dataPattern != null) {
                        String hex = HexBinary.printByteArray(m.data);
                        if (!dataPattern.matcher(hex).find()) continue;
                    }
                    JSONObject o = new JSONObject();
                    o.put("success", true);
                    o.put("match", m.toJson());
                    return o;
                }
                if (backend.equals("slcan") && !running) {
                    throw new IllegalStateException(lastError != null ? lastError : "SLCAN is disconnected");
                }
                long remaining = deadline - System.currentTimeMillis();
                if (remaining <= 0) {
                    JSONObject o = new JSONObject();
                    o.put("success", false);
                    o.put("error", "timeout");
                    o.put("latestSeq", messageSeq);
                    return o;
                }
                messageLock.wait(remaining);
            }
        }
    }

    @SuppressWarnings("unchecked")
    private JSONObject doStatus() {
        JSONObject o = new JSONObject();
        o.put("connected", running);
        o.put("backend", backend);
        if (backend.equals("slcan")) {
            o.put("port", connectedPort != null ? connectedPort : port);
            if (lastError != null) {
                o.put("success", false);
                o.put("error", lastError);
            }
        } else {
            o.put("channel", channel.name());
        }
        synchronized (messageLock) {
            o.put("bufferedPackets", messageBuffer.size());
            o.put("totalReceived", messageSeq);
        }
        return o;
    }

    private void ensureConnected() {
        if (!running) {
            doConnect();
        }
        if (backend.equals("slcan") && !running) {
            throw new IllegalStateException(lastError != null ? lastError : "SLCAN is disconnected");
        }
    }

    private static Integer parseCanId(String idStr) {
        if (idStr == null || idStr.isEmpty()) return null;
        idStr = idStr.trim();
        if (idStr.startsWith("0x") || idStr.startsWith("0X")) {
            return Integer.parseInt(idStr.substring(2), 16);
        }
        return Integer.parseInt(idStr, 16);
    }

    // -----------------------------------------------------------------------------------
    // Schema + response helpers
    // -----------------------------------------------------------------------------------

    @SuppressWarnings("unchecked")
    private static JSONObject tool(String name, String description, JSONObject inputSchema) {
        JSONObject t = new JSONObject();
        t.put("name", name);
        t.put("description", description);
        t.put("inputSchema", inputSchema);
        return t;
    }

    @SuppressWarnings("unchecked")
    private static JSONObject schemaObject(String[][] props, String[] required, boolean additional) {
        JSONObject schema = new JSONObject();
        schema.put("type", "object");
        JSONObject properties = new JSONObject();
        for (String[] p : props) {
            JSONObject ps = new JSONObject();
            ps.put("type", p[1]);
            ps.put("description", p[2]);
            properties.put(p[0], ps);
        }
        schema.put("properties", properties);
        if (required.length > 0) {
            JSONArray req = new JSONArray();
            req.addAll(Arrays.asList(required));
            schema.put("required", req);
        }
        schema.put("additionalProperties", additional);
        return schema;
    }

    private static JSONObject emptyObjectSchema() {
        return schemaObject(new String[0][], new String[0], false);
    }

    @SuppressWarnings("unchecked")
    private static JSONObject wrapToolResult(JSONObject body) {
        JSONObject envelope = new JSONObject();
        JSONArray content = new JSONArray();
        JSONObject part = new JSONObject();
        part.put("type", "text");
        part.put("text", body.toJSONString());
        content.add(part);
        envelope.put("content", content);
        envelope.put("isError", Boolean.FALSE.equals(body.get("success")) && body.containsKey("error"));
        envelope.put("structuredContent", body);
        return envelope;
    }

    @SuppressWarnings("unchecked")
    private static JSONObject toolError(String message) {
        JSONObject body = new JSONObject();
        body.put("success", false);
        body.put("error", message);
        return wrapToolResult(body);
    }

    private static long asLong(Object o, long defaultValue) {
        if (o == null) return defaultValue;
        if (o instanceof Number) return ((Number) o).longValue();
        try { return Long.parseLong(o.toString()); } catch (Exception ignored) { return defaultValue; }
    }

    @SuppressWarnings("unchecked")
    private void respond(Object id, Object result) {
        JSONObject resp = new JSONObject();
        resp.put("jsonrpc", "2.0");
        resp.put("id", id);
        resp.put("result", result);
        writeLine(resp.toJSONString());
    }

    @SuppressWarnings("unchecked")
    private void error(Object id, int code, String message) {
        JSONObject resp = new JSONObject();
        resp.put("jsonrpc", "2.0");
        resp.put("id", id);
        JSONObject err = new JSONObject();
        err.put("code", code);
        err.put("message", message);
        resp.put("error", err);
        writeLine(resp.toJSONString());
    }

    private void writeLine(String line) {
        synchronized (writeLock) {
            out.println(line);
            out.flush();
        }
    }
}
