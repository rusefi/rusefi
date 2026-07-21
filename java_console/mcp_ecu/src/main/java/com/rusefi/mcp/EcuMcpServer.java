package com.rusefi.mcp;

import com.devexperts.logging.Logging;
import com.opensr5.ConfigurationImage;
import com.opensr5.ini.IniFileModel;
import com.rusefi.binaryprotocol.BinaryProtocol;
import com.rusefi.config.generated.Integration;
import com.rusefi.core.SensorCentral;
import com.rusefi.core.MessagesCentral;
import com.rusefi.tune.xml.Msq;
import com.rusefi.tune.xml.MsqFactory;
import com.rusefi.ui.lua.LuaIncludeSyntax;
import com.rusefi.io.LinkManager;
import com.rusefi.io.lua.LuaService;
import org.json.simple.JSONArray;
import org.json.simple.JSONObject;
import org.json.simple.parser.JSONParser;

import java.io.BufferedReader;
import java.io.File;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.PrintStream;
import java.nio.charset.StandardCharsets;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.util.ArrayDeque;
import java.util.Arrays;
import java.util.Deque;
import java.util.regex.Pattern;

import static com.devexperts.logging.Logging.getLogging;

/**
 * MCP (Model Context Protocol) server exposing rusEFI Lua tooling to an LLM client
 * (Claude Desktop, JetBrains AI Assistant, etc.) over stdio JSON-RPC.
 *
 * <p>This is a minimal hand-rolled MCP implementation (no external SDK) that covers:
 * <ul>
 *     <li><code>initialize</code></li>
 *     <li><code>tools/list</code></li>
 *     <li><code>tools/call</code> for: connect, ecu_info, set_lua, get_lua, lua_reset,
 *         send_command (alias: command), read_output_channel, read_messages,
 *         wait_for_message, read_tune, reboot, reboot_to_blt — see
 *         java_console/mcp_ecu/README.md for the tool reference</li>
 *     <li><code>notifications/initialized</code></li>
 * </ul>
 *
 * <p>ECU access goes through {@link LuaService} which uses
 * {@link LinkManager#submit(Runnable)} so we never touch the wire from two threads at once.
 *
 * <p>ECU/Lua output is captured via a {@link MessagesCentral} listener — the same hook the
 * Swing {@code MessagesView} uses — into an in-memory ring buffer. Tools can pull recent
 * messages (<code>read_messages</code>) or block until a regex match
 * (<code>wait_for_message</code>) which is the typical way for an LLM to assert on
 * <code>print(...)</code> output from a candidate Lua script.
 *
 * <p>IMPORTANT: stdin/stdout are reserved for the MCP transport. All diagnostic logging
 * goes through {@link Logging} (to a file) and stderr; never to stdout.
 */
public class EcuMcpServer {
    private static final Logging log = getLogging(EcuMcpServer.class);

    private static final String PROTOCOL_VERSION = "2024-11-05";
    private static final String SERVER_NAME = "rusefi-ecu-mcp";
    private static final String SERVER_VERSION = "0.1.0";

    private final PrintStream out;
    private final BufferedReader in;
    private final JSONParser parser = new JSONParser();
    private final Object writeLock = new Object();

    /** Buffer of recent ECU messages (newest at tail). */
    private final Deque<EcuMessage> messageBuffer = new ArrayDeque<>();
    private final int messageBufferCapacity = 10_000;
    private final Object messageLock = new Object();
    /** Monotonic sequence number assigned to each message. */
    private long messageSeq = 0;

    /** Lazy-initialized link manager — created on first ECU-touching tool call. */
    private volatile LinkManager linkManager;
    private final Object connectLock = new Object();

    /** CLI: optional fixed serial port; null => autodetect on first connect. */
    private final String forcedPort;

    public EcuMcpServer(String forcedPort) {
        this(forcedPort,
                new BufferedReader(new InputStreamReader(System.in, StandardCharsets.UTF_8)),
                new PrintStream(System.out, true, StandardCharsets.UTF_8));
    }

    /** Test-friendly constructor: caller supplies the transport streams. */
    public EcuMcpServer(String forcedPort, BufferedReader in, PrintStream out) {
        this.forcedPort = forcedPort;
        this.in = in;
        this.out = out;
    }

    /** Exposed for tests so they can drive the dispatch loop. */
    public void runForTests() throws Exception {
        run();
    }

    public static void main(String[] args) {
        String port = null;
        for (int i = 0; i < args.length; i++) {
            if ("--port".equals(args[i]) && i + 1 < args.length) {
                port = args[++i];
            } else if ("--help".equals(args[i]) || "-h".equals(args[i])) {
                System.err.println("Usage: EcuMcpServer [--port <serialPort>]");
                System.err.println("Speaks MCP (JSON-RPC 2.0) over stdio.");
                return;
            }
        }
        try {
            new EcuMcpServer(port).run();
        } catch (Throwable t) {
            log.error("MCP server fatal", t);
            System.err.println("MCP server fatal: " + t);
            System.exit(1);
        }
    }

    private void run() throws Exception {
        log.info("rusEFI ECU MCP server starting. forcedPort=" + forcedPort);
        installMessagesListener();

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
        shutdown();
    }

    private void shutdown() {
        LinkManager lm = linkManager;
        if (lm != null) {
            try { lm.close(); } catch (Throwable ignored) {}
        }
    }

    // -----------------------------------------------------------------------------------
    // ECU message capture (mirrors what MessagesView does)
    // -----------------------------------------------------------------------------------

    private void installMessagesListener() {
        MessagesCentral.getInstance().addListener((clazz, message) -> {
            synchronized (messageLock) {
                long seq = ++messageSeq;
                messageBuffer.addLast(new EcuMessage(seq, System.currentTimeMillis(),
                        clazz != null ? clazz.getSimpleName() : "?", message));
                while (messageBuffer.size() > messageBufferCapacity)
                    messageBuffer.removeFirst();
                messageLock.notifyAll();
            }
        });
    }

    private static final class EcuMessage {
        final long seq;
        final long timestamp;
        final String source;
        final String message;
        EcuMessage(long seq, long timestamp, String source, String message) {
            this.seq = seq;
            this.timestamp = timestamp;
            this.source = source;
            this.message = message;
        }
        @SuppressWarnings("unchecked")
        JSONObject toJson() {
            JSONObject o = new JSONObject();
            o.put("seq", seq);
            o.put("timestamp", timestamp);
            o.put("source", source);
            o.put("message", message);
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
        if (method == null) return; // response, ignore
        JSONObject params = (JSONObject) msg.get("params");

        try {
            switch (method) {
                case "initialize":
                    respond(id, initialize(params));
                    break;
                case "notifications/initialized":
                case "notifications/cancelled":
                    // no response for notifications
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
    private JSONObject initialize(JSONObject params) {
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
                "Connect to the ECU (autodetect serial port unless --port was passed on startup). " +
                        "Most other tools call this implicitly. Returns 'connected' and the firmware 'signature'.",
                schemaObject(new String[][]{
                        {"port", "string", "Optional serial port name (e.g. /dev/ttyACM0, COM5); autodetect if omitted. " +
                                "Ignored if a connection is already active."}
                }, new String[]{}, false)));
        tools.add(tool("ecu_info",
                "Return signature + LUASCRIPT field metadata: 'luascript.offset' and 'luascript.maxSize' " +
                        "(script size budget in bytes; set_lua fails beyond it).",
                emptyObjectSchema()));
        tools.add(tool("set_lua",
                "Upload a Lua script to the ECU (write LUASCRIPT, burn flash, then luareset). " +
                        "Provide either 'script' (inline source) or 'path' (file on the MCP server host); " +
                        "'script' takes precedence if both are given. " +
                        "Returns success, bytesWritten, fieldSize, burnSucceeded.",
                schemaObject(new String[][]{
                        {"script", "string", "Inline Lua source. ASCII only — non-ASCII characters are not transmitted."},
                        {"path", "string", "Path to a .lua file on the MCP server host (not the client). " +
                                "'--include <file>' directives are expanded relative to the file's directory."},
                        {"timeoutMs", "integer", "Write + burn timeout in ms. Default 120000."}
                }, new String[]{}, false)));
        tools.add(tool("get_lua",
                "Read the currently-flashed Lua script from the cached controller image. " +
                        "Returns 'script' and 'length'.",
                emptyObjectSchema()));
        tools.add(tool("lua_reset",
                "Send 'luareset' to restart the Lua VM on the ECU. " +
                        "'queued: true' means accepted into the command queue, not that the ECU executed it.",
                emptyObjectSchema()));
        tools.add(tool("send_command",
                "Send an arbitrary text command via the standard command queue " +
                        "(e.g. 'lua 1+2', 'rpm', 'help'). Fire-and-forget: any output arrives as ECU " +
                        "messages — read it back with read_messages or wait_for_message.",
                schemaObject(new String[][]{
                        {"command", "string", "Command text."}
                }, new String[]{"command"}, false)));
        tools.add(tool("command",
                "Alias for send_command. Sends arbitrary text to the standard command queue; " +
                        "output arrives as ECU messages.",
                schemaObject(new String[][]{
                        {"command", "string", "Command text."}
                }, new String[]{"command"}, false)));
        tools.add(tool("read_output_channel",
                "Read latest output-channel value by channel name from SensorCentral. " +
                        "'found: false' means unknown channel OR no data received yet — retry before " +
                        "concluding the name is wrong.",
                schemaObject(new String[][]{
                        {"name", "string", "Output-channel (gauge) name, case-insensitive, e.g. 'RPMValue'."}
                }, new String[]{"name"}, false)));
        tools.add(tool("read_messages",
                "Return ECU messages from the in-memory ring buffer captured via MessagesCentral " +
                        "(this is the same stream the Swing MessagesView shows, including Lua print() output). " +
                        "Messages come oldest-first; when more than maxLines match, the newest are cut off — " +
                        "page forward by passing the last returned seq as sinceSeq. Also returns 'latestSeq', " +
                        "the newest captured sequence number.",
                schemaObject(new String[][]{
                        {"sinceSeq", "integer", "Only return messages with seq strictly greater than this. " +
                                "Use a previously returned seq/latestSeq to read incrementally."},
                        {"maxLines", "integer", "Maximum number of messages to return (oldest first). Default 200."},
                        {"sourceFilter", "string", "Optional substring filter on source class name (e.g. 'Lua')."}
                }, new String[]{}, false)));
        tools.add(tool("wait_for_message",
                "Block until an ECU message matches the given regex, or timeout. " +
                        "Returns the matching message (or success=false on timeout). " +
                        "Already-buffered messages are checked first, so a stale message can match — " +
                        "pass sinceSeq captured before triggering the action you are waiting on.",
                schemaObject(new String[][]{
                        {"regex", "string", "Java regex, find() semantics: succeeds if it matches anywhere " +
                                "inside the message text (source is not matched)."},
                        {"timeoutMs", "integer", "Wait timeout in ms. Default 10000."},
                        {"sinceSeq", "integer", "Only consider messages with seq strictly greater than this."}
                }, new String[]{"regex"}, false)));
        tools.add(tool("read_tune",
                "Read the complete tune (all calibration constants) from the ECU and save it as a " +
                        "TunerStudio-compatible .msq file on the MCP server host. The tune is built from " +
                        "the controller configuration image fetched over this connection plus the matching " +
                        ".ini definition. Returns 'path', 'constantCount', 'fileSize' and 'signature'. " +
                        "The file is XML and can be large — read it with file tools rather than inlining it.",
                schemaObject(new String[][]{
                        {"path", "string", "Output .msq file path on the MCP server host (not the client). " +
                                "Default: a temp file named rusefi_tune_*.msq."}
                }, new String[]{}, false)));
        tools.add(tool("reboot",
                "Reboot the ECU (send '" + Integration.CMD_REBOOT + "'). The serial link drops while the ECU " +
                        "restarts — reconnect (any ECU-touching tool reconnects implicitly) after a few seconds.",
                emptyObjectSchema()));
        tools.add(tool("reboot_to_blt",
                "Reboot the ECU into the OpenBLT bootloader (send '" + Integration.CMD_REBOOT_OPENBLT + "'). " +
                        "The firmware stops and the serial link drops; the ECU re-enumerates as an OpenBLT " +
                        "device for firmware update. Normal tools will not work until the ECU is power-cycled " +
                        "or new firmware is flashed.",
                emptyObjectSchema()));

        JSONObject result = new JSONObject();
        result.put("tools", tools);
        return result;
    }

    @SuppressWarnings("unchecked")
    private JSONObject toolsCall(JSONObject params) throws Exception {
        String name = (String) params.get("name");
        JSONObject args = (JSONObject) params.getOrDefault("arguments", new JSONObject());
        JSONObject toolResult;
        try {
            switch (name) {
                case "connect":     toolResult = doConnect(args); break;
                case "ecu_info":    toolResult = doEcuInfo(); break;
                case "set_lua":     toolResult = doSetLua(args); break;
                case "get_lua":     toolResult = doGetLua(); break;
                case "lua_reset":   toolResult = doLuaReset(); break;
                case "send_command":toolResult = doSendCommand(args); break;
                case "command":    toolResult = doSendCommand(args); break;
                case "read_output_channel": toolResult = doReadOutputChannel(args); break;
                case "read_messages": toolResult = doReadMessages(args); break;
                case "wait_for_message": toolResult = doWaitForMessage(args); break;
                case "read_tune":   toolResult = doReadTune(args); break;
                case "reboot":      toolResult = doReboot(Integration.CMD_REBOOT); break;
                case "reboot_to_blt": toolResult = doReboot(Integration.CMD_REBOOT_OPENBLT); break;
                default:
                    return toolError("Unknown tool: " + name);
            }
        } catch (Throwable t) {
            log.error("Tool '" + name + "' failed", t);
            return toolError(t.toString());
        }
        return wrapToolResult(toolResult);
    }

    private LinkManager ensureConnected(String portOrNull) throws Exception {
        LinkManager lm = linkManager;
        if (lm != null && lm.isActive()) return lm;
        synchronized (connectLock) {
            if (linkManager != null && linkManager.isActive()) return linkManager;
            String port = portOrNull != null ? portOrNull : forcedPort;
            linkManager = LuaService.connect(port, 60_000);
            return linkManager;
        }
    }

    @SuppressWarnings("unchecked")
    private JSONObject doConnect(JSONObject args) throws Exception {
        String port = (String) args.get("port");
        LinkManager lm = ensureConnected(port);
        JSONObject r = new JSONObject();
        r.put("connected", lm.isActive());
        BinaryProtocol bp = lm.getBinaryProtocol();
        if (bp != null) r.put("signature", String.valueOf(bp.signature));
        return r;
    }

    @SuppressWarnings("unchecked")
    private JSONObject doEcuInfo() throws Exception {
        LinkManager lm = ensureConnected(null);
        BinaryProtocol bp = lm.getBinaryProtocol();
        JSONObject r = new JSONObject();
        if (bp == null) {
            r.put("ready", false);
            return r;
        }
        r.put("ready", true);
        r.put("signature", String.valueOf(bp.signature));
        com.opensr5.ini.field.StringIniField f = LuaService.getLuaScriptField(bp);
        if (f != null) {
            JSONObject lua = new JSONObject();
            lua.put("offset", f.getOffset());
            lua.put("maxSize", f.getSize());
            r.put("luascript", lua);
        }
        return r;
    }

    @SuppressWarnings("unchecked")
    private JSONObject doSetLua(JSONObject args) throws Exception {
        String script = (String) args.get("script");
        String path = (String) args.get("path");
        long timeout = asLong(args.get("timeoutMs"), 120_000L);
        if (script == null && path == null)
            return errorBody("Provide 'script' or 'path'");
        if (script == null) {
            Path scriptPath = Paths.get(path);
            script = new String(Files.readAllBytes(scriptPath), StandardCharsets.US_ASCII);
            // If we have a path, we can support --include relative to that path
            script = LuaIncludeSyntax.reloadScript(script, name -> {
                try {
                    return new String(Files.readAllBytes(scriptPath.getParent().resolve(name)), StandardCharsets.US_ASCII);
                } catch (IOException e) {
                    throw new RuntimeException("Error reading included file [" + name + "]: " + e.getMessage());
                }
            });
        }

        LinkManager lm = ensureConnected(null);
        LuaService.LuaApplyResult r = LuaService.applyLuaScript(lm, script, timeout);
        JSONObject o = new JSONObject();
        o.put("success", r.success);
        if (!r.success) o.put("error", r.error);
        o.put("bytesWritten", r.bytesWritten);
        o.put("fieldSize", r.fieldSize);
        o.put("burnSucceeded", r.burnSucceeded);
        return o;
    }

    @SuppressWarnings("unchecked")
    private JSONObject doGetLua() throws Exception {
        LinkManager lm = ensureConnected(null);
        String script = LuaService.readLuaScript(lm);
        JSONObject o = new JSONObject();
        o.put("script", script);
        o.put("length", script.length());
        return o;
    }

    @SuppressWarnings("unchecked")
    private JSONObject doLuaReset() throws Exception {
        LinkManager lm = ensureConnected(null);
        LuaService.luaReset(lm);
        JSONObject o = new JSONObject();
        o.put("queued", true);
        return o;
    }

    @SuppressWarnings("unchecked")
    private JSONObject doSendCommand(JSONObject args) throws Exception {
        String cmd = (String) args.get("command");
        if (cmd == null) {
            return errorBody("'command' is required");
        }
        LinkManager lm = ensureConnected(null);
        LuaService.sendCommand(lm, cmd);
        JSONObject o = new JSONObject();
        o.put("queued", true);
        o.put("command", cmd);
        return o;
    }

    @SuppressWarnings("unchecked")
    private JSONObject doReadTune(JSONObject args) throws Exception {
        String path = (String) args.get("path");

        LinkManager lm = ensureConnected(null);
        BinaryProtocol bp = lm.getBinaryProtocol();
        if (bp == null)
            return errorBody("Binary protocol not established yet");
        ConfigurationImage image = bp.getControllerConfiguration();
        if (image == null)
            return errorBody("Controller configuration image not available yet");
        IniFileModel ini = bp.getIniFileNullable();
        if (ini == null)
            return errorBody("No .ini model for this connection");

        Msq tune = MsqFactory.valueOf(image, ini);
        File outputFile = path != null ? new File(path) : File.createTempFile("rusefi_tune_", ".msq");
        tune.writeXmlFile(outputFile.getAbsolutePath());

        JSONObject o = new JSONObject();
        o.put("success", true);
        o.put("path", outputFile.getAbsolutePath());
        o.put("constantCount", (long) tune.findPage().constant.size());
        o.put("fileSize", outputFile.length());
        o.put("signature", String.valueOf(bp.signature));
        return o;
    }

    @SuppressWarnings("unchecked")
    private JSONObject doReboot(String command) throws Exception {
        LinkManager lm = ensureConnected(null);
        LuaService.sendCommand(lm, command);
        JSONObject o = new JSONObject();
        o.put("queued", true);
        o.put("command", command);
        o.put("note", "serial link will drop while the ECU restarts");
        return o;
    }

    @SuppressWarnings("unchecked")
    private JSONObject doReadOutputChannel(JSONObject args) throws Exception {
        String name = (String) args.get("name");
        if (name == null || name.isEmpty()) {
            return errorBody("'name' is required");
        }

        ensureConnected(null);
        double value = SensorCentral.getInstance().getValue(name);

        JSONObject o = new JSONObject();
        o.put("name", name);
        o.put("found", !Double.isNaN(value));
        if (!Double.isNaN(value)) {
            o.put("value", value);
        } else {
            o.put("error", "Output channel not found or no data yet");
        }
        return o;
    }

    @SuppressWarnings("unchecked")
    private JSONObject doReadMessages(JSONObject args) {
        long sinceSeq = asLong(args.get("sinceSeq"), -1L);
        int maxLines = (int) asLong(args.get("maxLines"), 200L);
        String sourceFilter = (String) args.get("sourceFilter");

        JSONArray arr = new JSONArray();
        long latestSeq;
        synchronized (messageLock) {
            latestSeq = messageSeq;
            for (EcuMessage m : messageBuffer) {
                if (m.seq <= sinceSeq) continue;
                if (sourceFilter != null && !m.source.contains(sourceFilter)) continue;
                arr.add(m.toJson());
                if (arr.size() >= maxLines) break;
            }
        }
        JSONObject o = new JSONObject();
        o.put("messages", arr);
        o.put("latestSeq", latestSeq);
        return o;
    }

    @SuppressWarnings("unchecked")
    private JSONObject doWaitForMessage(JSONObject args) throws InterruptedException {
        String regex = (String) args.get("regex");
        if (regex == null) return errorBody("'regex' is required");
        long timeout = asLong(args.get("timeoutMs"), 10_000L);
        long sinceSeq = asLong(args.get("sinceSeq"), -1L);
        Pattern p = Pattern.compile(regex);

        long deadline = System.currentTimeMillis() + timeout;
        synchronized (messageLock) {
            while (true) {
                for (EcuMessage m : messageBuffer) {
                    if (m.seq <= sinceSeq) continue;
                    if (p.matcher(m.message).find()) {
                        JSONObject o = new JSONObject();
                        o.put("success", true);
                        o.put("match", m.toJson());
                        return o;
                    }
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
        // MCP tools/call result envelope: { content: [ { type: "text", text: "..." } ], isError: bool }
        JSONObject envelope = new JSONObject();
        JSONArray content = new JSONArray();
        JSONObject part = new JSONObject();
        part.put("type", "text");
        part.put("text", body.toJSONString());
        content.add(part);
        envelope.put("content", content);
        envelope.put("isError", Boolean.FALSE.equals(body.get("success")) && body.containsKey("error"));
        // also include the structured body for clients that prefer it
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

    @SuppressWarnings("unchecked")
    private static JSONObject errorBody(String message) {
        JSONObject o = new JSONObject();
        o.put("success", false);
        o.put("error", message);
        return o;
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
