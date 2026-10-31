package com.rusefi.ui.llm;

import com.rusefi.core.net.FirmwareSourceCodeDownloader;
import com.rusefi.core.net.KnowledgeManifest;
import org.json.simple.JSONArray;
import org.json.simple.JSONObject;
import org.junit.jupiter.api.Test;
import org.junit.jupiter.api.io.TempDir;

import java.io.IOException;
import java.nio.charset.StandardCharsets;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.util.Arrays;
import java.util.Map;
import java.util.TreeMap;
import java.util.zip.ZipEntry;
import java.util.zip.ZipOutputStream;

import static com.rusefi.ui.llm.ChatGptClient.*;
import static org.junit.jupiter.api.Assertions.*;

@SuppressWarnings("unchecked")
class HostedKnowledgeDeliveryCheckTest {
    private static final String REVISION = "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa";
    @TempDir Path root;

    @Test void exportsLicenseAndSourceEvidenceWithPreservedProvenance() throws Exception {
        String hash = prepareFixture(false);
        JSONObject verified = HostedKnowledgeDeliveryCheck.verifyPrepared(root, REVISION, hash);
        JSONObject exported = (JSONObject) verified.get("case");
        JSONObject report = parseObject(new String(Files.readAllBytes(Paths.get((String) exported.get("path"))), StandardCharsets.UTF_8));
        assertEquals(Boolean.FALSE, ((JSONObject) report.get("ecu_identity")).get("ready"));
        assertEquals("delivery_validation_without_ecu", ((JSONObject) report.get("ecu_identity")).get("source"));
        assertEquals(6, ((JSONArray) report.get("evidence")).size());
        assertEquals(hash, verified.get("archive_sha256"));
        assertEquals(REVISION, ((JSONObject) verified.get("provenance")).get("wiki_revision"));
    }

    @Test void rejectsLegacyWrongArtifactWrongRevisionAndDirtyMetadata() throws Exception {
        String hash = prepareFixture(false);
        assertFailure("differs from the selected workflow artifact", REVISION, "bad-hash");
        assertFailure("differs from the workflow head SHA", "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb", hash);
        Files.delete(root.resolve(KnowledgeManifest.NAME));
        assertFailure("no knowledge manifest", REVISION, hash);
        hash = prepareFixture(true);
        assertFailure("tracked edits", REVISION, hash);
        assertFalse(Files.exists(root.resolve("delivery-check-cases")));
    }

    @Test void rejectsChangedOrMissingRequiredPassagesAndMissingWikiEvidence() throws Exception {
        String hash = prepareFixture(false);
        Files.write(root.resolve("docs/licenses/wiki.txt"), "Changed".getBytes(StandardCharsets.UTF_8));
        assertFailure("fails its manifest hash", REVISION, hash);
        hash = prepareFixture(false);
        Files.delete(root.resolve("docs/AI/cold_start.md"));
        assertFailure("Required passage", REVISION, hash);
        hash = prepareFixture(false);
        Files.delete(root.resolve("rusefi_documentation/Cranking.md"));
        assertFailure("No cranking wiki passage", REVISION, hash);
        assertFalse(Files.exists(root.resolve("delivery-check-cases")));
    }

    private void assertFailure(String message, String revision, String hash) {
        IOException error = assertThrows(IOException.class, () -> HostedKnowledgeDeliveryCheck.verifyPrepared(root, revision, hash));
        assertTrue(error.getMessage().contains(message), error.getMessage());
    }

    private String prepareFixture(boolean dirty) throws Exception {
        Map<String, byte[]> contents = new TreeMap<>();
        for (String path : HostedKnowledgeDeliveryCheck.REQUIRED_PASSAGES) {
            contents.put(path, ("Fixture for " + path + "\n").getBytes(StandardCharsets.UTF_8));
        }
        contents.put("rusefi_documentation/Cranking.md", "Check cranking voltage.\n".getBytes(StandardCharsets.UTF_8));
        JSONObject files = new JSONObject();
        StringBuilder payload = new StringBuilder();
        for (Map.Entry<String, byte[]> file : contents.entrySet()) {
            String hash = KnowledgeManifest.sha256(file.getValue());
            files.put(file.getKey(), object("sha256", hash, "size", file.getValue().length));
            payload.append(file.getKey()).append('\0').append(hash).append('\n');
        }
        JSONObject repositories = new JSONObject();
        for (String name : Arrays.asList("firmware", "libfirmware", "wiki")) {
            repositories.put(name, object("revision", REVISION, "dirty", dirty));
        }
        JSONObject manifest = object("schema_version", 1, "files", files, "repositories", repositories,
                "payload_sha256", KnowledgeManifest.sha256(payload.toString().getBytes(StandardCharsets.UTF_8)));
        contents.put(KnowledgeManifest.NAME, manifest.toJSONString().getBytes(StandardCharsets.UTF_8));
        Path archive = root.resolve(FirmwareSourceCodeDownloader.ARCHIVE_NAME);
        try (ZipOutputStream zip = new ZipOutputStream(Files.newOutputStream(archive))) {
            for (Map.Entry<String, byte[]> file : contents.entrySet()) {
                zip.putNextEntry(new ZipEntry(file.getKey()));
                zip.write(file.getValue());
                zip.closeEntry();
                Path target = root.resolve(file.getKey());
                Files.createDirectories(target.getParent());
                Files.write(target, file.getValue());
            }
        }
        return KnowledgeManifest.hashFile(archive, () -> {});
    }
}
