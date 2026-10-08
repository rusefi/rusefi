package com.rusefi.mcp;

import com.rusefi.binaryprotocol.BinaryProtocol;
import com.rusefi.core.SensorCentral;
import com.rusefi.io.LinkManager;
import org.json.simple.JSONArray;
import org.json.simple.JSONObject;

import java.io.IOException;
import java.util.Arrays;
import java.util.HashMap;
import java.util.HashSet;
import java.util.Locale;
import java.util.Map;
import java.util.Set;
import java.util.concurrent.TimeUnit;
import java.util.concurrent.atomic.AtomicLong;
import java.util.function.LongSupplier;

/** Read-only in-process MCP adapter, pinned to a Console-owned connection for one conversation. */
@SuppressWarnings("unchecked")
public final class ConsoleEcuSession implements AutoCloseable {
    private static final int MAX_CHANNELS = 32;
    private static final String[] DEFAULT_CHANNELS = {
            "RPMValue", "VBatt", "isCranking", "MAPValue", "TPSValue", "coolant", "intake"
    };
    private static final String[] FAULT_CHANNELS = {
            "checkEngine", "hasCriticalError", "isWarnNow", "isTriggerError", "warningCounter", "lastErrorCode",
            "recentErrorCode1", "recentErrorCode2", "recentErrorCode3", "recentErrorCode4",
            "recentErrorCode5", "recentErrorCode6", "recentErrorCode7", "recentErrorCode8"
    };
    private final LinkManager link;
    private final BinaryProtocol protocol;
    private final EcuMcpServer server;
    private final SensorCentral.FullOutputLease lease;
    private final SensorCentral.SnapshotListenerToken samples;
    private final Map<String, JSONObject> definitions = new HashMap<>();
    private final LongSupplier nanoTime;
    private final AtomicLong sampleSequence = new AtomicLong();
    private volatile Sample sample;
    private volatile boolean closed;

    public ConsoleEcuSession(LinkManager link) throws IOException {
        this(link, System::nanoTime, System::currentTimeMillis);
    }

    ConsoleEcuSession(LinkManager link, LongSupplier nanoTime, LongSupplier currentTimeMillis) throws IOException {
        this.link = link;
        this.nanoTime = nanoTime;
        protocol = link.getBinaryProtocol();
        checkConnected();
        server = new EcuMcpServer(link, protocol);
        lease = SensorCentral.getInstance().acquireFullOutput();
        samples = SensorCentral.getInstance().addSnapshotListener(snapshot -> {
            // Snapshot listeners run after SensorCentral has decoded channel values.
            if (!closed && link.getBinaryProtocol() == protocol && !protocol.isClosed()
                    && snapshot.isFull() && snapshot.getGeneration() >= lease.getGeneration()) {
                sample = new Sample(SensorCentral.getInstance().getOutputChannelMap(), sampleSequence.incrementAndGet(),
                        currentTimeMillis.getAsLong(), nanoTime.getAsLong());
            }
        });
        for (Object item : (JSONArray) server.toolsList().get("tools")) {
            JSONObject definition = (JSONObject) item;
            String name = (String) definition.get("name");
            if (Arrays.asList("ecu_info", "read_output_channel", "read_messages").contains(name)) {
                definitions.put(name, definition);
            }
        }
        JSONObject properties = object("filter", object("type", "string", "description", "Case-insensitive channel, label, description or known units substring; default empty."));
        definitions.put("list_output_channels", object("name", "list_output_channels",
                "description", "List up to 100 INI datalog channels with labels, descriptions and units. unitsStatus distinguishes known, unknown, dynamic and conflicting units; never assume units when null. Descriptions come from gauge titles or datalog labels. Narrow using filter if truncated.",
                "inputSchema", object("type", "object", "properties", properties, "required", new JSONArray(), "additionalProperties", false)));
        JSONObject names = object("type", "array", "minItems", 1, "maxItems", MAX_CHANNELS, "uniqueItems", true,
                "items", object("type", "string", "minLength", 1, "maxLength", 256),
                "description", "INI output channel names, case-insensitive. Discover names with list_output_channels.");
        definitions.put("read_live_values", object("name", "read_live_values",
                "description", "Read 1-32 channels from one recent completed full host poll, with a shared sample ID, timestamp and age. Missing/non-finite values are explicit. A host poll is not an atomic ECU measurement.",
                "inputSchema", object("type", "object", "properties", object("names", names),
                        "required", array("names"), "additionalProperties", false)));
        definitions.put("diagnostic_snapshot", object("name", "diagnostic_snapshot",
                "description", "Read selected live channels plus warning/error channels from the same recent full host poll. Optional names defaults to RPMValue, VBatt, isCranking, MAPValue, TPSValue, coolant, intake. Recent/last codes and counters are history, not proof of active faults. Missing channels do not mean healthy.",
                "inputSchema", object("type", "object", "properties", object("names", names),
                        "required", new JSONArray(), "additionalProperties", false)));
        definitions.put("read_tune_fields", object("name", "read_tune_fields",
                "description", "Read selected calibration fields freshly from ECU RAM. Supply 1-32 exact INI calibration names, case-insensitive (find names in local knowledge). Scalars, enums/bitfields and arrays of up to 64 elements are supported, with at most 512 values total. Strings/Lua and larger arrays are excluded. Sequential reads are not an atomic snapshot or proof of flash persistence; values use parsed INI scaling. Never sends the entire tune.",
                "inputSchema", object("type", "object", "properties", object("names", object("type", "array",
                        "minItems", 1, "maxItems", MAX_CHANNELS, "uniqueItems", true,
                        "items", object("type", "string", "minLength", 1, "maxLength", 256))),
                        "required", array("names"), "additionalProperties", false)));
        definitions.put("get_lua", object("name", "get_lua",
                "description", "Read a bounded line range of fresh ECU LUASCRIPT source. RAM source may differ from the running VM or flash. Cite returned lines and compare hashes across pages. Never executes or edits Lua.",
                "inputSchema", object("type", "object", "properties", object("start_line", integerSchema(1, 100000),
                        "max_lines", integerSchema(1, 120)), "additionalProperties", false)));
        definitions.put("capture_live_log", object("name", "capture_live_log",
                "description", "Collect a short in-memory log of future full polls for selected channels. At most 16 channels, 20 samples, 10 seconds, with at least 100 ms between returned samples. Not a high-rate or complete recording. Missing readings are explicit. No file paths or tune data.",
                "inputSchema", object("type", "object", "properties", object("names", object("type", "array",
                        "minItems", 1, "maxItems", 16, "uniqueItems", true, "items", object("type", "string", "maxLength", 256)),
                        "duration_ms", integerSchema(100, 10000), "max_samples", integerSchema(1, 20)),
                        "required", array("names"), "additionalProperties", false)));
        definitions.put("capture_engine_sniffer", object("name", "capture_engine_sniffer",
                "description", "Observe the next Console engine-sniffer chart alongside the UI using current firmware settings. Return at most 128 events and 32 channel summaries, without raw chart text. Does not enable, reset or configure the sniffer. Times are chart-relative; receivedAt is host time.",
                "inputSchema", object("type", "object", "properties", object("timeoutMs", integerSchema(1, 10000),
                        "max_events", integerSchema(1, 128)), "additionalProperties", false)));
    }

    private static JSONObject integerSchema(int min, int max) { return object("type", "integer", "minimum", min, "maximum", max); }

    public boolean isCurrent() {
        return !closed && protocol != null && link.getBinaryProtocol() == protocol && !protocol.isClosed();
    }

    public void checkConnected() throws IOException {
        if (!isCurrent()) {
            throw new IOException("Connect to an ECU in Console, then start a new conversation.");
        }
    }

    /** Responses function definitions, including the Console-only discovery and snapshot tools. */
    public JSONArray definitions() {
        JSONArray result = new JSONArray();
        for (String name : Arrays.asList("ecu_info", "list_output_channels", "read_output_channel",
                "read_live_values", "diagnostic_snapshot", "read_tune_fields", "get_lua", "capture_live_log",
                "capture_engine_sniffer", "read_messages")) {
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
        Runnable sessionCheck = () -> {
            checkCancellation.run();
            if (!isCurrent()) { throw new java.util.concurrent.CancellationException("Console connection changed during evidence collection."); }
        };
        if ("list_output_channels".equals(name)) {
            result = ConsoleChannelCatalog.list(protocol.getIniFile(), (String) args.getOrDefault("filter", ""), checkCancellation);
        } else if ("read_tune_fields".equals(name)) {
            result = ConsoleTuneFields.read(link, protocol, (JSONArray) args.get("names"), sessionCheck);
        } else if ("get_lua".equals(name)) {
            result = ConsoleLuaRead.read(link, protocol, intArg(args, "start_line", 1), intArg(args, "max_lines", 80), sessionCheck);
        } else if ("capture_live_log".equals(name)) {
            result = captureLiveLog((JSONArray) args.get("names"), intArg(args, "duration_ms", 2000),
                    intArg(args, "max_samples", 20), sessionCheck);
        } else if ("capture_engine_sniffer".equals(name)) {
            result = ConsoleSnifferCapture.capture(link.getEngineState(), intArg(args, "timeoutMs", 5000),
                    intArg(args, "max_events", 100), sessionCheck);
        } else if ("read_output_channel".equals(name) || "read_live_values".equals(name) || "diagnostic_snapshot".equals(name)) {
            Sample current = awaitSample(checkCancellation);
            if (current == null) {
                return error("No fresh full output-channel sample. Check the Console connection.");
            }
            if ("read_output_channel".equals(name)) {
                result = channelResult(current, (String) args.get("name"));
            } else {
                JSONArray names = (JSONArray) args.getOrDefault("names", array(DEFAULT_CHANNELS));
                result = object("success", true, "values", channelResults(current, names, checkCancellation));
                if ("diagnostic_snapshot".equals(name)) {
                    result.put("faults", channelResults(current, array(FAULT_CHANNELS), checkCancellation));
                    result.put("faultNote", "Raw warning/error channels from this poll; last/recent codes and counters can describe past events. Missing data is not evidence of no faults. Use the matching firmware documentation to interpret numeric codes.");
                }
            }
            result.put("sampleId", current.id);
            result.put("sampleTimestampMs", current.timestamp);
            result.put("sampleAgeMs", TimeUnit.NANOSECONDS.toMillis(nanoTime.getAsLong() - current.nanoTime));
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

    private static int intArg(JSONObject args, String name, int fallback) { return ((Number) args.getOrDefault(name, fallback)).intValue(); }

    private JSONObject captureLiveLog(JSONArray names, int durationMs, int maxSamples, Runnable check) throws Exception {
        long started = System.currentTimeMillis();
        long deadline = nanoTime.getAsLong() + TimeUnit.MILLISECONDS.toNanos(durationMs);
        long nextSample = nanoTime.getAsLong();
        long lastId = sampleSequence.get(); // Never replay a sample from before capture started.
        JSONArray captured = new JSONArray();
        JSONObject result = object("success", true, "samples", captured, "source", "console_full_polls",
                "startedTimestampMs", started, "requestedDurationMs", durationMs,
                "note", "Downsampled observations of future full host polls, not a high-rate or complete log. Each sample has its own ID/time; gaps and missing values must not be interpreted as zero. No tune or file data is included.");
        boolean truncated = false;
        while (nanoTime.getAsLong() - deadline < 0) {
            check.run();
            Sample current = sample;
            long now = nanoTime.getAsLong();
            if (now - deadline >= 0) { break; }
            if (current != null && current.id > lastId && now - nextSample >= 0
                    && now - current.nanoTime <= TimeUnit.SECONDS.toNanos(2)) {
                lastId = current.id;
                nextSample = now + TimeUnit.MILLISECONDS.toNanos(100);
                captured.add(object("sampleId", current.id, "sampleTimestampMs", current.timestamp,
                        "sampleAgeMs", TimeUnit.NANOSECONDS.toMillis(now - current.nanoTime),
                        "values", channelResults(current, names, check)));
                if (result.toJSONString().length() > 48000) {
                    captured.remove(captured.size() - 1);
                    truncated = true;
                    break;
                }
                if (captured.size() >= maxSamples) { truncated = true; break; }
            }
            Thread.sleep(25);
        }
        check.run();
        result.put("completedTimestampMs", System.currentTimeMillis());
        result.put("sampleCount", captured.size());
        result.put("truncated", truncated);
        if (captured.isEmpty()) {
            result.put("success", false);
            result.put("error", "No new full output polls arrived during capture.");
        }
        return result;
    }

    private Sample awaitSample(Runnable checkCancellation) throws Exception {
        long deadline = nanoTime.getAsLong() + TimeUnit.SECONDS.toNanos(3);
        while (true) {
            checkCancellation.run();
            checkConnected();
            Sample current = sample;
            long now = nanoTime.getAsLong();
            if (current != null && now - current.nanoTime <= TimeUnit.SECONDS.toNanos(2)) {
                return current;
            }
            if (now - deadline >= 0) {
                return null;
            }
            Thread.sleep(25);
        }
    }

    private static JSONObject channelResult(Sample sample, String name) {
        double value = sample.values.getOrDefault(name, Double.NaN);
        return EcuMcpServer.outputChannelResult(name, Double.isFinite(value) ? value : Double.NaN);
    }

    private static JSONArray channelResults(Sample sample, JSONArray names, Runnable checkCancellation) {
        JSONArray result = new JSONArray();
        for (Object name : names) {
            checkCancellation.run();
            result.add(channelResult(sample, (String) name));
        }
        return result;
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
            if ("integer".equals(type)) {
                long number = ((Number) value).longValue();
                Number min = (Number) property.get("minimum"), max = (Number) property.get("maximum");
                if ((min != null && number < min.longValue()) || (max != null && number > max.longValue())) {
                    return "Integer argument is outside the allowed range.";
                }
            }
            if ("array".equals(type)) {
                int max = ((Number) property.getOrDefault("maxItems", MAX_CHANNELS)).intValue();
                if (!(value instanceof JSONArray) || ((JSONArray) value).isEmpty() || ((JSONArray) value).size() > max) {
                    return "names must contain between 1 and " + max + " names.";
                }
                Set<String> unique = new HashSet<>();
                for (Object name : (JSONArray) value) {
                    if (!(name instanceof String) || ((String) name).trim().isEmpty() || ((String) name).length() > 256) {
                        return "Names must be nonblank strings of at most 256 characters.";
                    }
                    if (!unique.add(((String) name).toLowerCase(Locale.ROOT))) {
                        return "Names must be unique (case-insensitive).";
                    }
                }
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
        final long id;
        final long timestamp;
        final long nanoTime;
        final Map<String, Double> values = new java.util.TreeMap<>(String.CASE_INSENSITIVE_ORDER);

        Sample(Map<String, Double> values, long id, long timestamp, long nanoTime) {
            this.values.putAll(values);
            this.id = id;
            this.timestamp = timestamp;
            this.nanoTime = nanoTime;
        }
    }

    private static JSONArray array(String... values) {
        JSONArray result = new JSONArray();
        result.addAll(Arrays.asList(values));
        return result;
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
