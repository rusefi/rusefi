package com.rusefi.core.net;

import org.json.simple.JSONObject;
import org.junit.jupiter.api.Test;
import org.junit.jupiter.api.io.TempDir;

import java.io.IOException;
import java.nio.charset.StandardCharsets;
import java.nio.file.*;
import java.util.Arrays;
import java.util.concurrent.CancellationException;

import static org.junit.jupiter.api.Assertions.*;

@SuppressWarnings("unchecked")
class KnowledgeManifestTest {
    @TempDir Path root;

    @Test void verifiesPayloadAndRetainsUnknownEcuCompatibility() throws Exception {
        writeFixture(root, false);
        KnowledgeManifest manifest = KnowledgeManifest.read(root);
        manifest.verifyPayload(root, () -> {});
        assertEquals(revision(), manifest.cleanRevision("wiki"));
        assertTrue(manifest.contains("docs/AI/start.md", KnowledgeManifest.sha256("Check voltage".getBytes(StandardCharsets.UTF_8))));
        JSONObject provenance = manifest.provenance("rusEFI master.2026.10.08.proteus.12345");
        assertEquals(revision(), provenance.get("source_revision"));
        assertEquals(revision(), provenance.get("libfirmware_revision"));
        assertEquals(revision(), provenance.get("wiki_revision"));
        assertEquals("unverified", provenance.get("ecu_match"));
        assertEquals(64, ((String) provenance.get("manifest_sha256")).length());
    }

    @Test void dirtyRepositoryCannotSupplyPinnedRevisionAndLegacyStaysUnknown() throws Exception {
        assertEquals("unknown", KnowledgeManifest.read(root).provenance(null).get("source_revision"));
        writeFixture(root, true);
        assertNull(KnowledgeManifest.read(root).cleanRevision("wiki"));
        assertEquals(Boolean.TRUE, KnowledgeManifest.read(root).provenance(null).get("wiki_dirty"));
    }

    @Test void detectsChangedMissingAndUnindexedFiles() throws Exception {
        writeFixture(root, false);
        KnowledgeManifest manifest = KnowledgeManifest.read(root);
        Files.write(root.resolve("docs/AI/start.md"), "Wrong voltage".getBytes(StandardCharsets.UTF_8));
        assertThrows(IOException.class, () -> manifest.verifyPayload(root, () -> {}));
        Files.delete(root.resolve("docs/AI/start.md"));
        assertThrows(IOException.class, () -> manifest.verifyPayload(root, () -> {}));
        writeFixture(root, false);
        Files.write(root.resolve("extra.txt"), new byte[]{1});
        assertThrows(IOException.class, () -> manifest.verifyPayload(root, () -> {}));
    }

    @Test void rejectsMalformedMetadataAndTraversal() throws Exception {
        for (String contents : Arrays.asList("{}", "[]", "not-json")) {
            Files.write(root.resolve(KnowledgeManifest.NAME), contents.getBytes(StandardCharsets.UTF_8));
            assertThrows(IOException.class, () -> KnowledgeManifest.read(root));
        }
        JSONObject manifest = writeFixture(root, false);
        manifest.put("payload_sha256", "0000000000000000000000000000000000000000000000000000000000000000");
        write(root, manifest);
        assertThrows(IOException.class, () -> KnowledgeManifest.read(root));
        manifest = writeFixture(root, false);
        JSONObject files = (JSONObject) manifest.get("files");
        files.put("../escape", files.remove("docs/AI/start.md"));
        write(root, manifest);
        assertThrows(IOException.class, () -> KnowledgeManifest.read(root));
    }

    @Test void verificationIsCancellable() throws Exception {
        writeFixture(root, false);
        KnowledgeManifest manifest = KnowledgeManifest.read(root);
        assertThrows(CancellationException.class, () -> manifest.verifyPayload(root, () -> { throw new CancellationException(); }));
    }

    static JSONObject writeFixture(Path root, boolean dirty) throws IOException {
        Files.createDirectories(root.resolve("docs/AI"));
        byte[] content = "Check voltage".getBytes(StandardCharsets.UTF_8);
        Files.write(root.resolve("docs/AI/start.md"), content);
        String hash = KnowledgeManifest.sha256(content);
        JSONObject file = new JSONObject();
        file.put("sha256", hash);
        file.put("size", content.length);
        JSONObject files = new JSONObject();
        files.put("docs/AI/start.md", file);
        JSONObject repositories = new JSONObject();
        for (String name : Arrays.asList("firmware", "libfirmware", "wiki")) {
            JSONObject repository = new JSONObject();
            repository.put("revision", revision());
            repository.put("dirty", dirty);
            repositories.put(name, repository);
        }
        JSONObject manifest = new JSONObject();
        manifest.put("schema_version", 1);
        manifest.put("repositories", repositories);
        manifest.put("files", files);
        manifest.put("payload_sha256", KnowledgeManifest.sha256(("docs/AI/start.md\0" + hash + "\n").getBytes(StandardCharsets.UTF_8)));
        write(root, manifest);
        return manifest;
    }

    private static String revision() { return "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"; }
    private static void write(Path root, JSONObject manifest) throws IOException {
        Files.write(root.resolve(KnowledgeManifest.NAME), manifest.toJSONString().getBytes(StandardCharsets.UTF_8));
    }
}
