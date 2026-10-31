package com.rusefi.ui.llm;

import org.json.simple.JSONArray;
import org.json.simple.JSONObject;
import org.junit.jupiter.api.Test;
import org.junit.jupiter.api.io.TempDir;

import java.io.IOException;
import java.nio.charset.StandardCharsets;
import java.nio.file.*;
import java.security.MessageDigest;
import java.util.Collections;
import java.util.concurrent.CancellationException;
import java.util.concurrent.atomic.AtomicInteger;
import java.util.stream.Stream;

import static com.rusefi.ui.llm.ChatGptClient.*;
import static com.rusefi.ui.llm.ChatGptAgentTest.array;
import static org.junit.jupiter.api.Assertions.*;
import static org.junit.jupiter.api.Assumptions.assumeTrue;

@SuppressWarnings("unchecked")
class DiagnosticCaseStoreTest {
    @TempDir Path directory;

    @Test void savesOriginalEvidenceAndSeparateAnalysisWithoutReadingOtherFiles() throws Exception {
        Files.write(directory.resolve("accounts.json"), "SECRET_TOKEN".getBytes(StandardCharsets.UTF_8));
        DiagnosticCaseStore store = new DiagnosticCaseStore(directory.resolve("cases"));
        JSONObject sample = object("sampleId", 7, "sampleTimestampMs", 1234, "value", 190);
        JSONObject result = object("samples", array(sample), "partial", true, "signature", "test-ecu");
        JSONObject arguments = object("names", array("RPMValue"));
        store.retain("capture_live_log", arguments, result);
        Object id = result.get("evidence_id");
        sample.put("value", 999);
        arguments.put("names", array("different"));
        JSONObject exported = store.export(args(id), object("signature", "test-ecu"), () -> {});
        assertEquals(Boolean.TRUE, exported.get("success"));
        Path file = Paths.get((String) exported.get("path"));
        byte[] bytes = Files.readAllBytes(file);
        String serialized = new String(bytes, StandardCharsets.UTF_8);
        assertFalse(serialized.contains("SECRET_TOKEN"));
        JSONObject report = parseObject(serialized);
        assertEquals(1L, report.get("schema_version"));
        JSONObject analysis = (JSONObject) report.get("analysis");
        assertEquals("assistant", analysis.get("author"));
        assertEquals(Boolean.FALSE, analysis.get("verified"));
        JSONObject record = (JSONObject) ((JSONArray) report.get("evidence")).get(0);
        JSONObject saved = (JSONObject) record.get("result");
        assertEquals(Boolean.TRUE, saved.get("partial"));
        assertEquals(190L, ((JSONObject) ((JSONArray) saved.get("samples")).get(0)).get("value"));
        assertEquals(array("RPMValue"), ((JSONObject) record.get("arguments")).get("names"));
        assertNotNull(record.get("collected_at"));
        assertEquals("unverified", ((JSONObject) report.get("provenance")).get("ecu_source_match"));
        StringBuilder expectedHash = new StringBuilder();
        for (byte b : MessageDigest.getInstance("SHA-256").digest(bytes)) { expectedHash.append(String.format("%02x", b & 255)); }
        assertEquals(expectedHash.toString(), exported.get("sha256"));
        JSONObject second = store.export(args(id), object(), () -> {});
        assertNotEquals(exported.get("path"), second.get("path"));
        assertArrayEquals(bytes, Files.readAllBytes(file));
    }

    @Test void committedTurnsRetainEvidenceButDiscardedAndNewConversationsDoNot() throws Exception {
        DiagnosticCaseStore committed = new DiagnosticCaseStore(directory);
        Object first = retain(committed);
        DiagnosticCaseStore discarded = new DiagnosticCaseStore(committed);
        Object discardedId = retain(discarded);
        DiagnosticCaseStore next = new DiagnosticCaseStore(committed);
        assertEquals(Boolean.TRUE, next.export(args(first), object(), () -> {}).get("success"));
        assertEquals(Boolean.FALSE, next.export(args(discardedId), object(), () -> {}).get("success"));
        assertEquals(Boolean.FALSE, new DiagnosticCaseStore(directory).export(args(first), object(), () -> {}).get("success"));
    }

    @Test void rejectsInvalidReferencesArgumentsAndModelPathsWithoutCreatingFiles() throws Exception {
        DiagnosticCaseStore store = new DiagnosticCaseStore(directory.resolve("cases"));
        Object id = retain(store);
        JSONObject path = args(id);
        path.put("path", "../accounts.json");
        JSONObject blank = args(id);
        blank.put("findings", " ");
        JSONObject tooLong = args(id);
        tooLong.put("hypotheses", String.join("", Collections.nCopies(4001, "x")));
        JSONObject wrongType = args(id);
        wrongType.put("next_measurements", 1);
        for (JSONObject invalid : new JSONObject[]{path, blank, tooLong, wrongType, object(), args("invented"),
                args(id, id), args(), args(1), args(new Object[65])}) {
            assertEquals(Boolean.FALSE, store.export(invalid, object(), () -> {}).get("success"), invalid.toJSONString());
        }
        assertFalse(Files.exists(directory.resolve("cases")));
    }

    @Test void cancelsBeforeAndAfterPublishingWithoutLeavingArtifacts() throws Exception {
        for (int stopAt = 1; stopAt <= 4; stopAt++) {
            Path cases = directory.resolve("cancel-" + stopAt);
            DiagnosticCaseStore store = new DiagnosticCaseStore(cases);
            Object id = retain(store);
            AtomicInteger checks = new AtomicInteger();
            int stopping = stopAt;
            assertThrows(CancellationException.class, () -> store.export(args(id), object(), () -> {
                if (checks.incrementAndGet() == stopping) { throw new CancellationException(); }
            }));
            if (Files.exists(cases)) {
                try (Stream<Path> files = Files.list(cases)) { assertEquals(0, files.count()); }
            }
        }
    }

    @Test void reportsDestinationFailureWithoutOverwritingIt() throws Exception {
        Path file = directory.resolve("cases");
        Files.write(file, new byte[]{42});
        DiagnosticCaseStore store = new DiagnosticCaseStore(file);
        assertEquals(Boolean.FALSE, store.export(args(retain(store)), object(), () -> {}).get("success"));
        assertArrayEquals(new byte[]{42}, Files.readAllBytes(file));
    }

    @Test void connectionChangeAtPublicationRemovesTheCase() throws Exception {
        DiagnosticCaseStore store = new DiagnosticCaseStore(directory);
        Object id = retain(store);
        AtomicInteger checks = new AtomicInteger();
        JSONObject result = store.export(args(id), object(), () -> {
            if (checks.incrementAndGet() == 4) { throw new IOException("Connection replaced"); }
        });
        assertEquals(Boolean.FALSE, result.get("success"));
        try (Stream<Path> files = Files.list(directory)) { assertEquals(0, files.count()); }
    }

    @Test void rejectsSymlinkDestinationsIncludingAncestors() throws Exception {
        Path outside = Files.createDirectory(directory.resolve("outside"));
        Path link = directory.resolve("link");
        try {
            Files.createSymbolicLink(link, outside);
        } catch (IOException | UnsupportedOperationException | SecurityException e) {
            assumeTrue(false, "Host does not permit symbolic links");
        }
        for (Path destination : new Path[]{link, link.resolve("cases")}) {
            DiagnosticCaseStore store = new DiagnosticCaseStore(destination);
            assertEquals(Boolean.FALSE, store.export(args(retain(store)), object(), () -> {}).get("success"));
        }
        try (Stream<Path> files = Files.list(outside)) { assertEquals(0, files.count()); }
    }

    @Test void retentionLimitsAreExplicitAndDoNotEvictReferencedEvidence() throws Exception {
        DiagnosticCaseStore store = new DiagnosticCaseStore(directory);
        Object first = retain(store);
        for (int i = 1; i < DiagnosticCaseStore.MAX_RECORDS; i++) { assertNotNull(retain(store)); }
        assertNull(retain(store));
        assertEquals(Boolean.TRUE, store.export(args(first), object(), () -> {}).get("success"));
        DiagnosticCaseStore byteLimited = new DiagnosticCaseStore(directory);
        String large = String.join("", Collections.nCopies(60000, "x"));
        int retained = 0;
        for (int i = 0; i < DiagnosticCaseStore.MAX_RECORDS; i++) {
            JSONObject result = object("text", large);
            byteLimited.retain("read_messages", object(), result);
            if (Boolean.TRUE.equals(result.get("evidence_retained"))) { retained++; }
        }
        assertTrue(retained > 0 && retained < DiagnosticCaseStore.MAX_RECORDS);
    }

    private static Object retain(DiagnosticCaseStore store) {
        JSONObject result = object("success", true);
        store.retain("ecu_info", object(), result);
        assertEquals(result.containsKey("evidence_id"), result.get("evidence_retained"));
        return result.get("evidence_id");
    }

    private static JSONObject args(Object... ids) {
        return object("findings", "RPM observed.", "hypotheses", "Low cranking voltage is unconfirmed.",
                "next_measurements", "Measure voltage.", "evidence_ids", array(ids));
    }
}
