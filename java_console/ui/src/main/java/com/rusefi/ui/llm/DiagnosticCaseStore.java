package com.rusefi.ui.llm;

import com.rusefi.UiVersion;
import org.json.simple.JSONArray;
import org.json.simple.JSONObject;

import java.io.IOException;
import java.nio.charset.StandardCharsets;
import java.nio.file.*;
import java.security.MessageDigest;
import java.security.NoSuchAlgorithmException;
import java.time.Instant;
import java.util.*;

import static com.rusefi.ui.llm.ChatGptClient.*;

/** Bounded, conversation-local evidence. Exports never read model-supplied file paths. */
@SuppressWarnings("unchecked")
final class DiagnosticCaseStore {
    static final int MAX_RECORDS = 128;
    static final int MAX_BYTES = 2 * 1024 * 1024;
    private final Path directory;
    private final String conversationId;
    private final Map<String, String> records = new LinkedHashMap<>();
    private int bytes;

    DiagnosticCaseStore(Path directory) {
        this.directory = directory.toAbsolutePath().normalize();
        conversationId = UUID.randomUUID().toString();
    }

    /** Failed turns must not add evidence to the next turn. Stored records are immutable JSON. */
    DiagnosticCaseStore(DiagnosticCaseStore previous) {
        directory = previous.directory;
        conversationId = previous.conversationId;
        records.putAll(previous.records);
        bytes = previous.bytes;
    }

    static JSONObject definition() {
        JSONObject text = object("type", "string", "minLength", 1, "maxLength", 4000);
        return object("type", "function", "name", "export_diagnostic_case", "strict", false,
                "description", "Save a local JSON diagnostic case when the user requests export. Include findings, hypotheses, next measurements and 1-64 evidence_id values returned by tools in this conversation. Copies only retained tool results, including partial/error flags and provenance. No ECU writes or arbitrary paths. Returns the saved path and SHA-256.",
                "parameters", object("type", "object", "additionalProperties", false,
                        "properties", object("findings", text, "hypotheses", text, "next_measurements", text,
                                "evidence_ids", object("type", "array", "minItems", 1, "maxItems", 64,
                                        "uniqueItems", true, "items", object("type", "string"))),
                        "required", array("findings", "hypotheses", "next_measurements", "evidence_ids")));
    }

    void retain(String tool, JSONObject arguments, JSONObject result) {
        String id = UUID.randomUUID().toString();
        String serialized = object("evidence_id", id, "tool", tool, "arguments", arguments,
                "collected_at", Instant.now().toString(), "result", result).toJSONString();
        int size = serialized.getBytes(StandardCharsets.UTF_8).length;
        // Leave room for the ID in the response limit applied by the agent.
        if (records.size() >= MAX_RECORDS || bytes + size > MAX_BYTES
                || result.toJSONString().length() > ChatGptAgent.MAX_RESULT - 256) {
            result.put("evidence_retained", false);
            result.put("export_note", "Evidence retention limit reached; start a new conversation to collect exportable evidence.");
            return;
        }
        records.put(id, serialized);
        bytes += size;
        result.put("evidence_id", id);
        result.put("evidence_retained", true);
    }

    interface Check { void run() throws IOException; }

    JSONObject export(JSONObject args, JSONObject identity, Check check) throws IOException {
        if (!args.keySet().equals(new HashSet<>(Arrays.asList("findings", "hypotheses", "next_measurements", "evidence_ids")))) {
            return error("Supply findings, hypotheses, next_measurements and evidence_ids only.");
        }
        for (String key : Arrays.asList("findings", "hypotheses", "next_measurements")) {
            Object value = args.get(key);
            if (!(value instanceof String) || ((String) value).trim().isEmpty() || ((String) value).length() > 4000) {
                return error(key + " must contain 1-4000 characters.");
            }
        }
        Object requested = args.get("evidence_ids");
        if (!(requested instanceof JSONArray) || ((JSONArray) requested).isEmpty() || ((JSONArray) requested).size() > 64) {
            return error("Select 1-64 unique evidence_ids returned by tools in this conversation.");
        }
        JSONArray evidence = new JSONArray();
        Set<String> seen = new HashSet<>();
        for (Object id : (JSONArray) requested) {
            check.run();
            if (!(id instanceof String) || !records.containsKey(id) || !seen.add((String) id)) {
                return error("Unknown or duplicate evidence_id; use retained evidence from this conversation.");
            }
            evidence.add(parseObject(records.get(id)));
        }
        String caseId = UUID.randomUUID().toString();
        JSONObject report = object("schema_version", 1, "case_id", caseId, "conversation_id", conversationId,
                "exported_at", Instant.now().toString(), "ecu_identity", identity,
                "provenance", object("console_version", UiVersion.CONSOLE_VERSION, "client_revision", "unknown",
                        "firmware_revision", "See ECU signature; not independently verified",
                        "source_revision", "unknown", "libfirmware_revision", "unknown", "wiki_revision", "unknown",
                        "ecu_source_match", "unverified"),
                "analysis", object("author", "assistant", "verified", false, "findings", args.get("findings"),
                        "hypotheses", args.get("hypotheses"), "next_measurements", args.get("next_measurements"),
                        "evidence_ids", requested),
                "evidence", evidence,
                "limitations", "Selected bounded tool results, not a full tune/log/capture or an atomic ECU snapshot. "
                        + "Original missing, error and truncation flags still apply. Analysis is model-authored. "
                        + "File hashes identify retrieved content, not firmware compatibility.");
        byte[] payload = report.toJSONString().getBytes(StandardCharsets.UTF_8);
        check.run();
        // The directory is supplied by Console, never by the model. Reject redirected destinations.
        for (Path path = directory; path != null; path = path.getParent()) {
            if (Files.isSymbolicLink(path)) { return error("Diagnostic case directory must not contain symbolic links."); }
        }
        Path temporary = null;
        Path target = directory.resolve("diagnostic-case-" + caseId + ".json");
        boolean published = false;
        boolean complete = false;
        try {
            Files.createDirectories(directory);
            temporary = Files.createTempFile(directory, ".diagnostic-case-", ".tmp");
            Files.write(temporary, payload);
            check.run();
            Files.move(temporary, target); // Same directory; never overwrite an existing case.
            published = true;
            check.run();
            JSONObject result = object("success", true, "path", target.toString(), "case_id", caseId,
                    "sha256", sha256(payload), "bytes", payload.length, "evidence_count", evidence.size());
            complete = true;
            return result;
        } catch (IOException e) {
            return error("Could not save diagnostic case. Check the Console case directory and available disk space.");
        } finally {
            if (temporary != null) { Files.deleteIfExists(temporary); }
            if (published && !complete) { Files.deleteIfExists(target); }
        }
    }

    private static String sha256(byte[] payload) {
        try {
            StringBuilder hash = new StringBuilder();
            for (byte value : MessageDigest.getInstance("SHA-256").digest(payload)) {
                hash.append(String.format(Locale.ROOT, "%02x", value & 255));
            }
            return hash.toString();
        } catch (NoSuchAlgorithmException e) { throw new AssertionError(e); }
    }

    private static JSONObject error(String message) { return object("success", false, "error", message); }
    private static JSONArray array(Object... values) {
        JSONArray result = new JSONArray();
        Collections.addAll(result, values);
        return result;
    }
}
