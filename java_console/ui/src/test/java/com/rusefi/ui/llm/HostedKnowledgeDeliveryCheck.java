package com.rusefi.ui.llm;

import com.rusefi.core.net.FirmwareSourceCodeDownloader;
import com.rusefi.core.net.KnowledgeManifest;
import org.json.simple.JSONArray;
import org.json.simple.JSONObject;

import java.io.IOException;
import java.nio.charset.StandardCharsets;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.util.Arrays;

import static com.rusefi.ui.llm.ChatGptClient.*;

/** Explicit hosted acceptance check. No ECU, account store, model request or simulated ECU findings. */
public final class HostedKnowledgeDeliveryCheck {
    static final String[] REQUIRED_PASSAGES = {
            "docs/knowledge-index.md", "docs/licenses/rusefi.txt", "docs/licenses/wiki.txt",
            "docs/AI/cold_start.md", "docs/hellen-board-mapping.md"
    };

    public static void main(String[] args) throws Exception {
        if (args.length != 3 || !args[1].matches("[0-9a-f]{40}") || !args[2].matches("[0-9a-f]{64}")) {
            throw new IllegalArgumentException("Usage: <dedicated-cache-directory> <workflow-head-sha> <workflow-artifact-zip-sha256>");
        }
        Path root = new FirmwareSourceCodeDownloader(Paths.get(args[0])).downloadFresh(
                percent -> System.out.println("Published knowledge download: " + percent + "%"));
        JSONObject result = verifyPrepared(root, args[1], args[2]);
        System.out.println("Hosted knowledge delivery verified (no live ECU/ChatGPT validation): " + result.toJSONString());
    }

    @SuppressWarnings("unchecked")
    static JSONObject verifyPrepared(Path root, String expectedRevision, String expectedArchiveHash) throws Exception {
        String archiveHash = KnowledgeManifest.hashFile(root.resolve(FirmwareSourceCodeDownloader.ARCHIVE_NAME), () -> {});
        require(expectedArchiveHash.equals(archiveHash), "Published ZIP differs from the selected workflow artifact.");
        KnowledgeManifest manifest = KnowledgeManifest.read(root);
        JSONObject provenance = manifest.provenance(null);
        require("present".equals(provenance.get("manifest_status")),
                "Published ZIP has no knowledge manifest. Publish the manifest-enabled workflow before completing this gate.");
        require(expectedRevision.equals(provenance.get("source_revision")), "Manifest source revision differs from the workflow head SHA.");
        for (String repository : Arrays.asList("firmware", "libfirmware", "wiki")) {
            require(manifest.cleanRevision(repository) != null, "Hosted archive reports tracked edits in " + repository + ".");
        }
        require("unverified".equals(provenance.get("ecu_match")), "Delivery validation must not claim ECU compatibility.");

        LocalKnowledgeTools knowledge = new LocalKnowledgeTools(root);
        DiagnosticCaseStore cases = new DiagnosticCaseStore(root.resolve("delivery-check-cases"));
        JSONArray ids = new JSONArray();
        JSONArray originalResults = new JSONArray();
        for (String path : REQUIRED_PASSAGES) { retainPassage(knowledge, cases, ids, originalResults, path, 1); }

        JSONObject search = knowledge.execute("search_knowledge",
                object("query", "cranking", "path_prefix", "rusefi_documentation", "max_results", 1), () -> {});
        require(Boolean.TRUE.equals(search.get("success")) && search.get("matches") instanceof JSONArray
                && !((JSONArray) search.get("matches")).isEmpty(), "No cranking wiki passage was retrieved.");
        JSONObject match = (JSONObject) ((JSONArray) search.get("matches")).get(0);
        JSONObject wiki = retainPassage(knowledge, cases, ids, originalResults,
                (String) match.get("path"), ((Number) match.get("line")).intValue());
        require(provenance.get("wiki_revision").equals(wiki.get("upstream_revision")), "Wiki citation is not pinned to its declared revision.");

        JSONObject exported = cases.export(object("findings", "Knowledge delivery validation only; no ECU observations collected.",
                        "hypotheses", "No engine diagnosis performed.", "next_measurements", "Complete live ChatGPT and ECU acceptance.",
                        "evidence_ids", ids),
                object("ready", false, "source", "delivery_validation_without_ecu"), () -> {});
        require(Boolean.TRUE.equals(exported.get("success")), "Diagnostic case export failed.");
        Path casePath = Paths.get((String) exported.get("path"));
        byte[] bytes = Files.readAllBytes(casePath);
        require(KnowledgeManifest.sha256(bytes).equals(exported.get("sha256")), "Exported case hash differs from its bytes.");
        JSONObject report = parseObject(new String(bytes, StandardCharsets.UTF_8));
        JSONArray savedEvidence = (JSONArray) report.get("evidence");
        require(savedEvidence.size() == originalResults.size(), "Case lost referenced evidence.");
        for (int i = 0; i < originalResults.size(); i++) {
            require(originalResults.get(i).equals(((JSONObject) savedEvidence.get(i)).get("result")), "Case changed retrieved evidence/provenance.");
        }
        JSONObject savedProvenance = (JSONObject) report.get("provenance");
        require(((JSONArray) savedProvenance.get("knowledge")).contains(provenance), "Case lost manifest/revision/payload provenance.");
        require(ClientBuildProvenance.CURRENT.revision.matches("[0-9a-f]{40}")
                && ClientBuildProvenance.CURRENT.revision.equals(savedProvenance.get("client_revision")), "Case lacks the client build revision.");
        require("unverified".equals(savedProvenance.get("ecu_source_match")), "Case incorrectly claims ECU/source compatibility.");
        return object("archive_sha256", archiveHash, "provenance", provenance, "case", exported);
    }

    @SuppressWarnings("unchecked")
    private static JSONObject retainPassage(LocalKnowledgeTools knowledge, DiagnosticCaseStore cases,
                                             JSONArray ids, JSONArray originals, String path, int line) throws Exception {
        JSONObject args = object("path", path, "start_line", line, "max_lines", 5);
        JSONObject result = knowledge.execute("read_knowledge", args, () -> {});
        require(Boolean.TRUE.equals(result.get("success")) && Boolean.TRUE.equals(result.get("file_hash_matches_manifest"))
                && result.get("citation") instanceof String, "Required passage is missing or fails its manifest hash: " + path);
        originals.add(parseObject(result.toJSONString()));
        cases.retain("read_knowledge", args, result);
        require(result.get("evidence_id") instanceof String, "Required passage was not retained: " + path);
        ids.add(result.get("evidence_id"));
        return result;
    }

    private static void require(boolean condition, String message) throws IOException {
        if (!condition) { throw new IOException(message); }
    }
}
