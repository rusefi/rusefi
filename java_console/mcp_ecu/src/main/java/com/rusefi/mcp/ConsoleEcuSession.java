package com.rusefi.mcp;

import com.opensr5.ini.DatalogEntry;
import com.rusefi.binaryprotocol.BinaryProtocol;
import com.rusefi.core.SensorCentral;
import com.rusefi.io.LinkManager;
import org.json.simple.JSONArray;
import org.json.simple.JSONObject;

import java.io.IOException;
import java.util.Arrays;
import java.util.HashMap;
import java.util.Map;

/** Read-only in-process MCP adapter, pinned to a Console-owned connection for one conversation. */
@SuppressWarnings("unchecked")
public final class ConsoleEcuSession implements AutoCloseable {
    private final LinkManager link;
    private final BinaryProtocol protocol;
    private final EcuMcpServer server;
    private final SensorCentral.FullOutputLease lease;
    private final SensorCentral.SnapshotListenerToken samples;
    private final Map<String, JSONObject> definitions = new HashMap<>();
    private volatile Sample sample;
    private volatile boolean closed;

    public ConsoleEcuSession(LinkManager link) throws IOException {
        this.link = link;
        protocol = link.getBinaryProtocol();
        checkConnected();
        server = new EcuMcpServer(link, protocol);
        lease = SensorCentral.getInstance().acquireFullOutput();
        samples = SensorCentral.getInstance().addSnapshotListener(snapshot -> {
            // Snapshot listeners run after SensorCentral has decoded channel values.
            if (!closed && link.getBinaryProtocol() == protocol && !protocol.isClosed()
                    && snapshot.isFull() && snapshot.getGeneration() >= lease.getGeneration()) {
                sample = new Sample(SensorCentral.getInstance().getOutputChannelMap());
            }
        });
        for (Object item : (JSONArray) server.toolsList().get("tools")) {
            JSONObject definition = (JSONObject) item;
            String name = (String) definition.get("name");
            if (Arrays.asList("ecu_info", "read_output_channel", "read_messages").contains(name)) {
                definitions.put(name, definition);
            }
        }
        JSONObject properties = object("filter", object("type", "string", "description", "Case-insensitive channel or label substring; default empty."));
        definitions.put("list_output_channels", object("name", "list_output_channels",
                "description", "List up to 100 INI datalog channel names and labels. Narrow using filter if truncated.",
                "inputSchema", object("type", "object", "properties", properties, "required", new JSONArray(), "additionalProperties", false)));
    }

    public boolean isCurrent() {
        return !closed && protocol != null && link.getBinaryProtocol() == protocol && !protocol.isClosed();
    }

    public void checkConnected() throws IOException {
        if (!isCurrent()) {
            throw new IOException("Connect to an ECU in Console, then start a new conversation.");
        }
    }

    /** Responses function definitions adapted from the same schemas used by standalone MCP. */
    public JSONArray definitions() {
        JSONArray result = new JSONArray();
        for (String name : Arrays.asList("ecu_info", "list_output_channels", "read_output_channel", "read_messages")) {
            JSONObject tool = definitions.get(name);
            result.add(object("type", "function", "name", name, "description", tool.get("description"),
                    "parameters", tool.get("inputSchema"), "strict", false));
        }
        return result;
    }

    public JSONObject execute(String name, JSONObject args, Runnable checkCancellation) throws Exception {
        checkCancellation.run();
        checkConnected();
        JSONObject definition = definitions.get(name);
        if (definition == null) {
            return error("Tool is not available in the Console read-only session.");
        }
        String invalid = validate(args, (JSONObject) definition.get("inputSchema"));
        if (invalid != null) {
            return error(invalid);
        }
        if ("read_messages".equals(name)) {
            if (args.containsKey("maxLines") && (((Number) args.get("maxLines")).longValue() < 1
                    || ((Number) args.get("maxLines")).longValue() > 200)) {
                return error("maxLines must be between 1 and 200.");
            }
            if (args.containsKey("sinceSeq") && ((Number) args.get("sinceSeq")).longValue() < -1) {
                return error("sinceSeq must be at least -1.");
            }
        }
        JSONObject result;
        if ("list_output_channels".equals(name)) {
            result = listChannels((String) args.getOrDefault("filter", ""));
        } else if ("read_output_channel".equals(name)) {
            long deadline = System.nanoTime() + java.util.concurrent.TimeUnit.SECONDS.toNanos(3);
            Sample current;
            while ((current = sample) == null || System.nanoTime() - current.nanoTime > java.util.concurrent.TimeUnit.SECONDS.toNanos(2)) {
                checkCancellation.run();
                checkConnected();
                if (System.nanoTime() >= deadline) {
                    return error("No fresh full output-channel sample. Check the Console connection.");
                }
                Thread.sleep(25);
            }
            String channel = (String) args.get("name");
            double value = current.values.getOrDefault(channel, Double.NaN);
            result = EcuMcpServer.outputChannelResult(channel, Double.isFinite(value) ? value : Double.NaN);
            result.put("sampleTimestampMs", current.timestamp);
            result.put("sampleAgeMs", java.util.concurrent.TimeUnit.NANOSECONDS.toMillis(System.nanoTime() - current.nanoTime));
        } else {
            JSONObject envelope = server.toolsCall(object("name", name, "arguments", args));
            result = (JSONObject) envelope.get("structuredContent");
            if (result == null) {
                result = error("ECU read failed.");
            }
        }
        checkCancellation.run();
        checkConnected();
        result.put("signature", protocol.signature);
        return result;
    }

    private JSONObject listChannels(String filter) throws Exception {
        JSONArray channels = new JSONArray();
        int matches = 0;
        String needle = filter.toLowerCase(java.util.Locale.ROOT);
        for (DatalogEntry entry : protocol.getIniFile().getDatalogEntries()) {
            if (!(entry.getChannel() + " " + entry.getLabel()).toLowerCase(java.util.Locale.ROOT).contains(needle)) {
                continue;
            }
            matches++;
            if (channels.size() < 100) {
                channels.add(object("name", entry.getChannel(), "label", entry.getLabel()));
            }
        }
        return object("channels", channels, "matches", matches, "truncated", matches > channels.size());
    }

    private static String validate(JSONObject args, JSONObject schema) {
        JSONObject properties = (JSONObject) schema.get("properties");
        for (Object key : args.keySet()) {
            JSONObject property = properties == null ? null : (JSONObject) properties.get(key);
            if (property == null) {
                return "Unknown tool argument.";
            }
            Object value = args.get(key);
            String type = (String) property.get("type");
            if ("string".equals(type) && (!(value instanceof String) || ((String) value).length() > 256)) {
                return "String arguments must be at most 256 characters.";
            }
            if ("integer".equals(type) && !(value instanceof Long || value instanceof Integer)) {
                return "Integer argument required.";
            }
        }
        JSONArray required = (JSONArray) schema.get("required");
        if (required != null) {
            for (Object key : required) {
                if (!args.containsKey(key) || "".equals(args.get(key))) {
                    return "Missing required tool argument.";
                }
            }
        }
        return null;
    }

    private static JSONObject error(String message) {
        return object("success", false, "error", message);
    }

    private static final class Sample {
        final long timestamp = System.currentTimeMillis();
        final long nanoTime = System.nanoTime();
        final Map<String, Double> values = new java.util.TreeMap<>(String.CASE_INSENSITIVE_ORDER);

        Sample(Map<String, Double> values) {
            this.values.putAll(values);
        }
    }

    private static JSONObject object(Object... pairs) {
        JSONObject value = new JSONObject();
        for (int i = 0; i < pairs.length; i += 2) {
            value.put(pairs[i], pairs[i + 1]);
        }
        return value;
    }

    @Override public void close() {
        if (closed) {
            return;
        }
        closed = true;
        samples.remove();
        lease.close();
        server.shutdown(); // Borrows the link: closing the assistant never closes the Console port.
    }
}
