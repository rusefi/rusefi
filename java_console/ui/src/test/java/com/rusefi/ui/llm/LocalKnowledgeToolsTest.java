package com.rusefi.ui.llm;

import org.json.simple.JSONArray;
import org.json.simple.JSONObject;
import org.junit.jupiter.api.Test;
import org.junit.jupiter.api.io.TempDir;

import java.io.IOException;
import java.io.RandomAccessFile;
import java.nio.charset.StandardCharsets;
import java.nio.file.Files;
import java.nio.file.Path;
import java.util.concurrent.CancellationException;
import java.util.concurrent.atomic.AtomicInteger;

import static com.rusefi.ui.llm.ChatGptClient.object;
import static org.junit.jupiter.api.Assertions.*;
import static org.junit.jupiter.api.Assumptions.assumeTrue;

class LocalKnowledgeToolsTest {
    @TempDir Path root;

    @Test void literalAndKeywordSearchReturnScopedCitationsAndUnknownProvenance() throws Exception {
        write("firmware/controllers/start.cpp", "void getCrankingFuel() {}\n");
        write("rusefi_documentation/Start.md", "Intro\r\nCheck cranking voltage and RPM.\r\n![Wiring](Images/wiring.png)\r\n");
        JSONObject literal = search(object("query", "getCrankingFuel"));
        JSONObject hit = (JSONObject) ((JSONArray) literal.get("matches")).get(0);
        assertEquals("firmware/controllers/start.cpp:L1", hit.get("citation"));
        assertEquals("void getCrankingFuel() {}", hit.get("text"));
        assertTrue(((String) hit.get("sha256")).matches("[0-9a-f]{64}"));
        JSONObject provenance = (JSONObject) literal.get("provenance");
        assertEquals("unknown", provenance.get("source_revision"));
        assertEquals("unverified", provenance.get("ecu_match"));
        JSONObject keywords = search(object("query", "VOLTAGE cranking", "mode", "keywords", "path_prefix", "rusefi_documentation"));
        hit = (JSONObject) ((JSONArray) keywords.get("matches")).get(0);
        assertEquals("rusefi_documentation/Start.md:L2", hit.get("citation"));
        assertEquals("https://github.com/rusefi/rusefi_documentation/blob/master/Start.md", hit.get("upstream_url"));
        assertTrue(((JSONArray) search(object("query", "getCrankingFuel", "path_prefix", "rusefi_documentation")).get("matches")).isEmpty());
    }

    @Test void readsLineRangesAndPreservesDiagramReferencesAndUtf8() throws Exception {
        write("rusefi_documentation/Start here.md", "Intro\r\nCheck voltage λ.\r\n![Wiring](Images/wiring.png)\r\n");
        JSONObject result = read(object("path", "rusefi_documentation/Start here.md", "start_line", 2L, "max_lines", 1L));
        JSONArray lines = (JSONArray) result.get("lines");
        assertEquals(1, lines.size());
        assertEquals("Check voltage λ.", ((JSONObject) lines.get(0)).get("text"));
        assertEquals("rusefi_documentation/Start here.md:L2-L2", result.get("citation"));
        assertEquals(3, result.get("next_line"));
        assertEquals(Boolean.TRUE, result.get("truncated"));
        assertTrue(((String) result.get("upstream_url")).contains("Start%20here.md"));
        JSONObject next = read(object("path", "rusefi_documentation/Start here.md", "start_line", 3));
        assertEquals(result.get("sha256"), next.get("sha256"));
        assertEquals("![Wiring](Images/wiring.png)", ((JSONObject) ((JSONArray) next.get("lines")).get(0)).get("text"));
        assertEquals(Boolean.FALSE, next.get("truncated"));
    }

    @Test void rejectsTraversalAbsolutePathsHiddenFilesCredentialsAndUnrelatedRoots() throws Exception {
        write("firmware/start.cpp", "safe");
        write("llm-access/accounts.json", "secret-token");
        write("firmware/accounts.json", "secret-token");
        write("firmware/.private/config.txt", "secret-token");
        write("unrelated.txt", "secret-token");
        for (String path : new String[]{"../llm-access/accounts.json", "firmware/../../secret.txt", "firmware/../start.cpp",
                "/etc/passwd", "C:/private.txt", "firmware\\start.cpp", "llm-access/accounts.json",
                "firmware/accounts.json", "firmware/.private/config.txt", "unrelated.txt"}) {
            assertEquals(Boolean.FALSE, read(object("path", path)).get("success"), path);
        }
        assertTrue(((JSONArray) search(object("query", "secret-token")).get("matches")).isEmpty());
    }

    @Test void refusesFileDirectoryAndCacheSymlinks() throws Exception {
        Path outside = Files.createDirectory(root.resolve("private"));
        Files.write(outside.resolve("secret.txt"), "external-secret".getBytes(StandardCharsets.UTF_8));
        Files.createDirectories(root.resolve("firmware"));
        try {
            Files.createSymbolicLink(root.resolve("firmware/linked.txt"), outside.resolve("secret.txt"));
            Files.createSymbolicLink(root.resolve("firmware/linked-dir"), outside);
            Files.createSymbolicLink(root.resolve("cache-alias"), root);
        } catch (IOException | UnsupportedOperationException | SecurityException e) {
            assumeTrue(false, "Symlinks unavailable on this test host");
        }
        assertEquals(Boolean.FALSE, read(object("path", "firmware/linked.txt")).get("success"));
        assertEquals(Boolean.FALSE, read(object("path", "firmware/linked-dir/secret.txt")).get("success"));
        assertTrue(((JSONArray) search(object("query", "external-secret")).get("matches")).isEmpty());
        assertEquals(Boolean.FALSE, new LocalKnowledgeTools(root.resolve("cache-alias"))
                .execute("read_knowledge", object("path", "firmware/linked.txt"), () -> {}).get("success"));
    }

    @Test void zipLinkTextIsNeverFollowed() throws Exception {
        write("firmware/link.txt", "../llm-access/accounts.json");
        write("llm-access/accounts.json", "private-token");
        JSONObject result = read(object("path", "firmware/link.txt"));
        assertEquals("../llm-access/accounts.json", ((JSONObject) ((JSONArray) result.get("lines")).get(0)).get("text"));
        assertFalse(result.toJSONString().contains("private-token"));
    }

    @Test void rejectsMalformedArgumentsAndMissingFilesWithoutHostPathDisclosure() {
        for (JSONObject args : new JSONObject[]{object(), object("query", " "), object("query", 12), object("query", "x", "mode", "regex"),
                object("query", "x", "max_results", 21), object("query", "x", "extra", true), object("query", "x", "max_results", 1.5)}) {
            assertEquals(Boolean.FALSE, search(args).get("success"));
        }
        for (JSONObject args : new JSONObject[]{object(), object("path", "firmware/no.txt"), object("path", "firmware/no.txt", "start_line", 0),
                object("path", "firmware/no.txt", "max_lines", 121), object("path", "firmware/no.txt", "start_line", "1")}) {
            JSONObject result = read(args);
            assertEquals(Boolean.FALSE, result.get("success"));
            assertFalse(result.toJSONString().contains(root.toString()));
        }
    }

    @Test void boundsResultsLongLinesAndSkipsOversizedOrBinaryFiles() throws Exception {
        write("firmware/many.txt", repeat("needle with context\n", 150));
        write("firmware/long.txt", repeat("needle ", 5000));
        Files.write(root.resolve("firmware/binary.txt"), new byte[]{0, 1, 2});
        Files.write(root.resolve("firmware/invalid.md"), new byte[]{(byte) 0xc3, 0x28});
        try (RandomAccessFile file = new RandomAccessFile(root.resolve("firmware/large.txt").toFile(), "rw")) {
            file.setLength(LocalKnowledgeTools.MAX_FILE_BYTES + 1L);
        }
        JSONObject search = search(object("query", "needle", "max_results", 2));
        assertEquals(2, ((JSONArray) search.get("matches")).size());
        assertEquals(Boolean.TRUE, search.get("truncated"));
        assertTrue(search.toJSONString().length() < ChatGptAgent.MAX_RESULT);
        JSONObject read = read(object("path", "firmware/long.txt"));
        assertEquals(Boolean.TRUE, read.get("truncated"));
        assertTrue(((String) ((JSONObject) ((JSONArray) read.get("lines")).get(0)).get("text")).length() <= 2000);
        assertTrue(read.toJSONString().length() < ChatGptAgent.MAX_RESULT);
        assertEquals(Boolean.FALSE, read(object("path", "firmware/large.txt")).get("success"));
        assertEquals(Boolean.FALSE, read(object("path", "firmware/binary.txt")).get("success"));
        assertEquals(Boolean.FALSE, read(object("path", "firmware/invalid.md")).get("success"));
        JSONObject missing = search(object("query", "absent"));
        assertTrue(((Number) missing.get("files_skipped")).intValue() >= 3);
        assertEquals(Boolean.TRUE, missing.get("truncated"));
    }

    @Test void cancellationPropagatesWhileScanningAndReading() throws Exception {
        write("firmware/example.txt", repeat("example\n", 300));
        for (String name : new String[]{"search_knowledge", "read_knowledge"}) {
            AtomicInteger checks = new AtomicInteger();
            assertThrows(CancellationException.class, () -> new LocalKnowledgeTools(root).execute(name,
                    name.startsWith("search") ? object("query", "absent") : object("path", "firmware/example.txt"),
                    () -> { if (checks.incrementAndGet() >= 3) { throw new CancellationException(); } }));
        }
    }

    @Test void rereadsChangedFilesAndReportsTheirNewHash() throws Exception {
        write("firmware/example.txt", "old source");
        JSONObject first = read(object("path", "firmware/example.txt"));
        write("firmware/example.txt", "new source");
        JSONObject second = read(object("path", "firmware/example.txt"));
        assertNotEquals(first.get("sha256"), second.get("sha256"));
        assertEquals("new source", ((JSONObject) ((JSONArray) second.get("lines")).get(0)).get("text"));
    }

    private JSONObject search(JSONObject args) { return new LocalKnowledgeTools(root).execute("search_knowledge", args, () -> {}); }
    private JSONObject read(JSONObject args) { return new LocalKnowledgeTools(root).execute("read_knowledge", args, () -> {}); }
    private void write(String path, String content) throws IOException {
        Files.createDirectories(root.resolve(path).getParent());
        Files.write(root.resolve(path), content.getBytes(StandardCharsets.UTF_8));
    }
    private static String repeat(String text, int count) {
        StringBuilder value = new StringBuilder();
        for (int i = 0; i < count; i++) { value.append(text); }
        return value.toString();
    }
}
