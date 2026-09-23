package com.rusefi.mcp;

import com.sun.net.httpserver.HttpServer;
import org.json.simple.JSONArray;
import org.json.simple.JSONObject;
import org.junit.jupiter.api.AfterEach;
import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.Test;
import org.junit.jupiter.api.io.TempDir;

import java.io.ByteArrayOutputStream;
import java.io.IOException;
import java.net.InetSocketAddress;
import java.nio.charset.StandardCharsets;
import java.nio.file.Files;
import java.nio.file.Path;
import java.util.ArrayList;
import java.util.LinkedHashMap;
import java.util.List;
import java.util.Map;
import java.util.concurrent.ConcurrentHashMap;
import java.util.concurrent.CopyOnWriteArrayList;
import java.util.zip.ZipEntry;
import java.util.zip.ZipOutputStream;

import static org.junit.jupiter.api.Assertions.*;

class BundleDownloaderTest {
    private static final String SREC = "S3060801000001EF\n";
    @TempDir Path temporary;
    private HttpServer server;
    private BundleDownloader downloader;
    private final Map<String, byte[]> responses = new ConcurrentHashMap<>();
    private final List<String> requests = new CopyOnWriteArrayList<>();

    @BeforeEach
    void startServer() throws IOException {
        server = HttpServer.create(new InetSocketAddress("127.0.0.1", 0), 0);
        server.createContext("/", exchange -> {
            String path = exchange.getRequestURI().getPath();
            requests.add(path);
            byte[] response = responses.get(path);
            exchange.sendResponseHeaders(response == null ? 404 : 200, response == null ? -1 : response.length);
            if (response != null) {
                exchange.getResponseBody().write(response);
            }
            exchange.close();
        });
        server.start();
        downloader = new BundleDownloader("http://127.0.0.1:" + server.getAddress().getPort(), "rusefi");
    }

    @AfterEach
    void stopServer() {
        server.stop(0);
    }

    @Test
    void boardDownloadIsFreshAndReturnsUsableFirmwarePaths() throws Exception {
        responses.put("/autoupdate/rusefi_bundle_proteus_f7_autoupdate.zip",
                zip(Map.of("console/release.txt", "platform=proteus_f7", "rusefi_update.srec", SREC)));
        for (int i = 0; i < 2; i++) {
            JSONObject result = downloader.download(arguments("board", "proteus_f7", "destination", temporary.resolve("run" + i).toString()));
            assertEquals(true, result.get("success"));
            assertEquals("development", result.get("branch"));
            assertEquals(true, result.get("firmwareIncluded"));
            Path srec = Path.of((String) result.get("firmwarePath"));
            assertEquals(SREC, Files.readString(srec));
            assertEquals(List.of(srec.toString()), result.get("srecPaths"));
            assertEquals("platform=proteus_f7", Files.readString(Path.of((String) result.get("bundleDirectory")).resolve("console/release.txt")));
            JSONObject archive = (JSONObject) ((JSONArray) result.get("archives")).get(0);
            assertTrue(Files.isRegularFile(Path.of((String) archive.get("path"))));
            assertTrue(((Number) archive.get("fileSize")).longValue() > 0);
        }
        assertEquals(2, requests.size(), "Every invocation must contact the server for fresh content");
    }

    @Test
    void universalOnlyContainsNoFirmware() throws Exception {
        responses.put("/autoupdate/rusefi_bundle_universal_autoupdate.zip", zip(Map.of("console/release.txt", "platform=universal")));
        JSONObject result = downloader.download(arguments("universal", true, "destination", temporary.resolve("universal").toString()));
        assertEquals(true, result.get("success"));
        assertEquals(false, result.get("firmwareIncluded"));
        assertEquals(List.of(), result.get("srecPaths"));
        assertFalse(result.containsKey("firmwarePath"));
    }

    @Test
    void universalWithBoardAddsFirmwareWithoutOverwritingUniversalConsole() throws Exception {
        responses.put("/lts/lts-26/autoupdate/rusefi_bundle_universal_autoupdate.zip",
                zip(Map.of("console/release.txt", "platform=universal", "console/rusefi_console.jar", "universal console")));
        responses.put("/lts/lts-26/autoupdate/rusefi_bundle_uaefi_autoupdate.zip",
                zip(Map.of("console/release.txt", "platform=uaefi", "console/rusefi_console.jar", "board console", "uaefi.srec", SREC)));
        JSONObject result = downloader.download(arguments("board", "uaefi", "universal", true, "branch", "lts-26",
                "destination", temporary.resolve("combined").toString()));
        Path bundle = Path.of((String) result.get("bundleDirectory"));
        assertEquals("platform=universal", Files.readString(bundle.resolve("console/release.txt")));
        assertEquals("universal console", Files.readString(bundle.resolve("console/rusefi_console.jar")));
        assertEquals(SREC, Files.readString(bundle.resolve("uaefi.srec")));
        assertEquals(2, ((JSONArray) result.get("archives")).size());
    }

    @Test
    void missingPublicBoardBundleFallsBackToObfuscatedAndMasterMapsToDevelopment() throws Exception {
        String fallback = "/autoupdate/rusefi_bundle_uaefi_obfuscated_public_autoupdate.zip";
        responses.put(fallback, zip(Map.of("uaefi_obfuscated.srec", SREC)));
        JSONObject result = downloader.download(arguments("board", "uaefi", "branch", "master",
                "destination", temporary.resolve("obfuscated").toString()));
        assertEquals(List.of("/autoupdate/rusefi_bundle_uaefi_autoupdate.zip", fallback), requests);
        assertTrue(result.get("firmwarePath").toString().endsWith("uaefi_obfuscated.srec"));
        assertTrue(((JSONObject) ((JSONArray) result.get("archives")).get(0)).get("url").toString().endsWith(fallback));
    }

    @Test
    void multipleSrecsAreListedWithoutChoosingOneToFlash() throws Exception {
        responses.put("/autoupdate/rusefi_bundle_uaefi_autoupdate.zip", zip(Map.of("first.srec", SREC, "second.srec", SREC)));
        JSONObject result = downloader.download(arguments("board", "uaefi", "destination", temporary.resolve("multiple").toString()));
        assertEquals(2, ((JSONArray) result.get("srecPaths")).size());
        assertFalse(result.containsKey("firmwarePath"));
    }

    @Test
    void invalidInputAndExistingDestinationNeverDownloadOrOverwrite() throws Exception {
        Path existing = Files.createDirectory(temporary.resolve("existing"));
        Files.writeString(existing.resolve("keep.txt"), "keep");
        List<JSONObject> invalid = new ArrayList<>(List.of(
                arguments(), arguments("board", "../uaefi"), arguments("board", "universal"),
                arguments("universal", "true"), arguments("board", "uaefi", "branch", "../master"),
                arguments("board", "uaefi", "destination", ""), arguments("board", ""),
                arguments("board", "uaefi", "branch", 42)));
        for (JSONObject request : invalid) {
            assertThrows(IllegalArgumentException.class, () -> downloader.download(request));
        }
        assertThrows(IOException.class, () -> downloader.download(arguments("board", "uaefi", "destination", existing.toString())));
        assertTrue(requests.isEmpty());
        assertEquals("keep", Files.readString(existing.resolve("keep.txt")));
    }

    @Test
    void missingOrInvalidFirmwareAndCorruptArchivesRemovePartialOutput() throws Exception {
        for (byte[] content : List.of(zip(Map.of("console/release.txt", "no firmware")),
                zip(Map.of("bad.srec", "invalid")), "not a zip".getBytes(StandardCharsets.UTF_8))) {
            responses.put("/autoupdate/rusefi_bundle_uaefi_autoupdate.zip", content);
            Path destination = temporary.resolve("failure");
            assertThrows(IOException.class, () -> downloader.download(arguments("board", "uaefi", "destination", destination.toString())));
            assertFalse(Files.exists(destination));
        }
    }

    @Test
    void zipTraversalCannotEscapeDestination() throws Exception {
        for (String path : List.of("../escaped.srec", "../../escaped.srec", "..\\escaped.srec", "C:/escaped.srec")) {
            responses.put("/autoupdate/rusefi_bundle_uaefi_autoupdate.zip", zip(Map.of(path, SREC)));
            Path destination = temporary.resolve("traversal");
            assertThrows(IOException.class, () -> downloader.download(arguments("board", "uaefi", "destination", destination.toString())));
            assertFalse(Files.exists(destination));
            assertFalse(Files.exists(temporary.resolve("escaped.srec")));
        }
    }

    @Test
    void unavailableBoardReportsFailureAndRemovesDestination() {
        Path destination = temporary.resolve("missing");
        IOException error = assertThrows(IOException.class, () -> downloader.download(arguments("board", "missing", "destination", destination.toString())));
        assertTrue(error.getMessage().contains("Bundle not found"));
        assertFalse(Files.exists(destination));
        assertEquals(2, requests.size());
    }

    @SuppressWarnings("unchecked")
    private static JSONObject arguments(Object... pairs) {
        JSONObject result = new JSONObject();
        for (int i = 0; i < pairs.length; i += 2) {
            result.put(pairs[i], pairs[i + 1]);
        }
        return result;
    }

    private static byte[] zip(Map<String, String> entries) throws IOException {
        ByteArrayOutputStream bytes = new ByteArrayOutputStream();
        try (ZipOutputStream zip = new ZipOutputStream(bytes)) {
            for (Map.Entry<String, String> entry : new LinkedHashMap<>(entries).entrySet()) {
                zip.putNextEntry(new ZipEntry(entry.getKey()));
                zip.write(entry.getValue().getBytes(StandardCharsets.UTF_8));
                zip.closeEntry();
            }
        }
        return bytes.toByteArray();
    }
}
