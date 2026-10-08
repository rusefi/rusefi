package com.rusefi.core.net;

import com.sun.net.httpserver.HttpServer;
import org.junit.jupiter.api.AfterEach;
import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.Test;
import org.junit.jupiter.api.io.TempDir;

import java.io.ByteArrayOutputStream;
import java.io.IOException;
import java.net.InetSocketAddress;
import java.net.URL;
import java.nio.charset.StandardCharsets;
import java.nio.file.*;
import java.nio.file.attribute.FileTime;
import java.time.Clock;
import java.time.Instant;
import java.time.ZoneOffset;
import java.util.ArrayList;
import java.util.List;
import java.util.Map;
import java.util.TreeMap;
import java.util.LinkedHashMap;
import java.util.concurrent.atomic.AtomicInteger;
import java.util.zip.CRC32;
import java.util.zip.ZipEntry;
import java.util.zip.ZipOutputStream;
import java.util.zip.ZipInputStream;
import java.io.ByteArrayInputStream;
import org.json.simple.JSONObject;

import static org.junit.jupiter.api.Assertions.*;

class FirmwareSourceCodeDownloaderTest {
    private static final Instant NOW = Instant.parse("2026-10-07T12:00:00Z");
    private static final byte[] GOOD_ZIP = archive("firmware/readme.md", true);
    @TempDir Path directory;
    private HttpServer server;
    private final AtomicInteger requests = new AtomicInteger();
    private byte[] response = GOOD_ZIP;
    private int status = 200;
    private boolean chunked;
    private boolean truncated;
    private Map<String, byte[]> edgeCache;
    private FirmwareSourceCodeDownloader downloader;

    @BeforeEach void startServer() throws Exception {
        server = HttpServer.create(new InetSocketAddress("127.0.0.1", 0), 0);
        server.createContext("/firmware-source.zip", exchange -> {
            requests.incrementAndGet();
            assertEquals("RE-Internal-Sync", exchange.getRequestHeaders().getFirst("User-Agent"));
            byte[] served = edgeCache == null ? response
                    : edgeCache.computeIfAbsent(exchange.getRequestURI().toString(), ignored -> response);
            exchange.sendResponseHeaders(status, chunked ? 0 : served.length + (truncated ? 10 : 0));
            try {
                exchange.getResponseBody().write(served);
            } finally {
                exchange.close();
            }
        });
        server.start();
        downloader = new FirmwareSourceCodeDownloader(directory,
                new URL("http://127.0.0.1:" + server.getAddress().getPort() + "/firmware-source.zip"),
                Clock.fixed(NOW, ZoneOffset.UTC));
    }

    @AfterEach void stopServer() { server.stop(0); }

    @Test void refreshDoesNotReuseCdnCachedUrlEvenWhenHeadersAreIgnored() throws Exception {
        edgeCache = new java.util.concurrent.ConcurrentHashMap<>();
        edgeCache.put("/firmware-source.zip", GOOD_ZIP);
        response = archive("firmware/new-publication.md", true);
        downloader.downloadFresh(ignored -> {});
        assertArrayEquals(response, Files.readAllBytes(zip()));
        assertTrue(Files.exists(directory.resolve("firmware/new-publication.md")));
        response = archive("firmware/next-publication.md", true);
        downloader.downloadFresh(ignored -> {});
        assertArrayEquals(response, Files.readAllBytes(zip()));
        assertTrue(Files.exists(directory.resolve("firmware/next-publication.md")));
        assertFalse(Files.exists(directory.resolve("firmware/new-publication.md")));
        assertEquals(2, requests.get());
    }

    @Test void downloadsMissingArchiveExtractsBesideItAndReportsProgress() throws Exception {
        List<Integer> progress = new ArrayList<>();
        assertEquals(directory, downloader.download(progress::add));
        assertEquals(1, requests.get());
        assertArrayEquals(GOOD_ZIP, Files.readAllBytes(zip()));
        assertEquals(NOW, Files.getLastModifiedTime(zip()).toInstant());
        assertEquals("Wiki text", new String(Files.readAllBytes(directory.resolve("rusefi_documentation/Help.md")), StandardCharsets.UTF_8));
        assertTrue(Files.exists(directory.resolve("firmware/readme.md")));
        assertEquals(Integer.valueOf(0), progress.get(0));
        assertEquals(Integer.valueOf(100), progress.get(progress.size() - 1));
        for (int i = 1; i < progress.size(); i++) {
            assertTrue(progress.get(i) > progress.get(i - 1));
        }
        assertTrue(progress.stream().anyMatch(value -> value > 0 && value < 70));
        assertTrue(progress.stream().anyMatch(value -> value > 70 && value < 100));
        assertNoTemporaryFiles();
    }

    @Test void reusesFreshCacheAndRestoresMissingExtractionWithoutNetwork() throws Exception {
        cache(GOOD_ZIP, NOW.minusSeconds(30L * 24 * 3600)); // Exactly 30 days is still valid.
        Files.createDirectories(directory.resolve("firmware"));
        Files.write(directory.resolve("firmware/removed-in-new-archive.txt"), new byte[]{1});
        Files.write(directory.resolve("unrelated.txt"), new byte[]{2});
        List<Integer> progress = new ArrayList<>();
        downloader.download(progress::add);
        assertEquals(0, requests.get());
        assertTrue(Files.exists(directory.resolve("firmware/readme.md")));
        assertFalse(Files.exists(directory.resolve("firmware/removed-in-new-archive.txt")));
        assertTrue(Files.exists(directory.resolve("unrelated.txt")));
        assertEquals(NOW.minusSeconds(30L * 24 * 3600), Files.getLastModifiedTime(zip()).toInstant());
        assertEquals(Integer.valueOf(100), progress.get(progress.size() - 1));
    }

    @Test void refreshesUndersizedAndOlderThanThirtyDaysCaches() throws Exception {
        cache(archive("firmware/small.txt", false), NOW);
        downloader.download(ignored -> {});
        assertEquals(1, requests.get());
        Files.setLastModifiedTime(zip(), FileTime.from(NOW.minusSeconds(30L * 24 * 3600 + 1)));
        downloader.download(ignored -> {});
        assertEquals(2, requests.get());
        assertEquals(NOW, Files.getLastModifiedTime(zip()).toInstant());
    }

    @Test void supportsChunkedDownloadsWithoutContentLength() throws Exception {
        chunked = true;
        List<Integer> progress = new ArrayList<>();
        downloader.download(progress::add);
        assertArrayEquals(GOOD_ZIP, Files.readAllBytes(zip()));
        assertEquals(Integer.valueOf(100), progress.get(progress.size() - 1));
    }

    @Test void cachedPreparationNeverDownloadsMissingSmallOrExpiredArchives() throws Exception {
        assertNull(downloader.prepareCached(ignored -> {}));
        cache(archive("firmware/small.txt", false), NOW);
        assertNull(downloader.prepareCached(ignored -> {}));
        cache(GOOD_ZIP, NOW.minusSeconds(31L * 24 * 3600));
        assertNull(downloader.prepareCached(ignored -> {}));
        assertEquals(0, requests.get());
    }

    @Test void cachedPreparationRestoresExtractionBeforeReturningReady() throws Exception {
        cache(GOOD_ZIP, NOW);
        assertEquals(directory, downloader.prepareCached(ignored -> {}));
        assertTrue(Files.isRegularFile(directory.resolve("firmware/readme.md")));
        assertTrue(Files.isRegularFile(directory.resolve("rusefi_documentation/Help.md")));
        assertEquals(0, requests.get());
    }

    @Test void explicitDownloadRecoversFromRecentCorruptCache() throws Exception {
        byte[] broken = GOOD_ZIP.clone();
        broken[30 + "firmware/readme.md".length()] ^= 1;
        cache(broken, NOW);
        assertThrows(IOException.class, () -> downloader.prepareCached(ignored -> {}));
        assertEquals(0, requests.get());
        assertEquals(directory, downloader.downloadFresh(ignored -> {}));
        assertEquals(1, requests.get());
        assertArrayEquals(GOOD_ZIP, Files.readAllBytes(zip()));
        assertTrue(Files.isRegularFile(directory.resolve("firmware/readme.md")));
    }

    @Test void failedOrTooSmallDownloadKeepsOldCacheAndExtractedFiles() throws Exception {
        byte[] old = archive("firmware/old.txt", false);
        cache(old, NOW);
        Files.createDirectories(directory.resolve("firmware"));
        Files.write(directory.resolve("firmware/old.txt"), new byte[]{3});
        for (int code : new int[]{500, 200}) {
            status = code;
            response = "not an archive".getBytes(StandardCharsets.UTF_8);
            List<Integer> progress = new ArrayList<>();
            assertThrows(IOException.class, () -> downloader.download(progress::add));
            assertArrayEquals(old, Files.readAllBytes(zip()));
            assertArrayEquals(new byte[]{3}, Files.readAllBytes(directory.resolve("firmware/old.txt")));
            assertFalse(progress.contains(100));
            assertNoTemporaryFiles();
        }
    }

    @Test void rejectsIncompleteDownload() {
        truncated = true;
        assertThrows(IOException.class, () -> downloader.download(ignored -> {}));
        assertFalse(Files.exists(zip()));
    }

    @Test void validatesCrcBeforeReplacingExtractedContent() throws Exception {
        response = GOOD_ZIP.clone();
        // First entry is STORED: corrupt its data while keeping the central directory intact.
        response[30 + "firmware/readme.md".length()] ^= 1;
        Files.createDirectories(directory.resolve("firmware"));
        Files.write(directory.resolve("firmware/existing.txt"), new byte[]{4});
        assertThrows(IOException.class, () -> downloader.download(ignored -> {}));
        assertFalse(Files.exists(zip()));
        assertArrayEquals(new byte[]{4}, Files.readAllBytes(directory.resolve("firmware/existing.txt")));
        assertNoTemporaryFiles();
    }

    @Test void rejectsTraversalAndReservedCachePaths() throws Exception {
        for (String name : new String[]{"../escaped.txt", "..\\escaped.txt", "/absolute.txt", "C:/escaped.txt",
                ".download.lock", "firmware-source.zip"}) {
            response = archive(name, true);
            assertThrows(IOException.class, () -> downloader.download(ignored -> {}), name);
            assertFalse(Files.exists(zip()));
            assertNoTemporaryFiles();
        }
        assertFalse(Files.exists(directory.getParent().resolve("escaped.txt")));
    }

    @Test void verifiesVersionedPayloadBeforeReplacingCacheAndClearsMetadataForLegacy() throws Exception {
        response = versionedArchive(false);
        downloader.downloadFresh(ignored -> {});
        byte[] good = Files.readAllBytes(zip());
        byte[] metadata = Files.readAllBytes(directory.resolve(KnowledgeManifest.NAME));
        assertEquals("present", KnowledgeManifest.read(directory).provenance(null).get("manifest_status"));
        response = versionedArchive(true); // Valid ZIP CRCs, but a payload hash disagrees with its manifest.
        assertThrows(IOException.class, () -> downloader.downloadFresh(ignored -> {}));
        assertArrayEquals(good, Files.readAllBytes(zip()));
        assertArrayEquals(metadata, Files.readAllBytes(directory.resolve(KnowledgeManifest.NAME)));
        assertNoTemporaryFiles();
        response = GOOD_ZIP;
        downloader.downloadFresh(ignored -> {});
        assertFalse(Files.exists(directory.resolve(KnowledgeManifest.NAME)));
        assertEquals("unknown", KnowledgeManifest.read(directory).provenance(null).get("source_revision"));
    }

    @SuppressWarnings("unchecked")
    private byte[] versionedArchive(boolean corrupt) throws Exception {
        Map<String, byte[]> contents = new LinkedHashMap<>();
        try (ZipInputStream zip = new ZipInputStream(new ByteArrayInputStream(GOOD_ZIP))) {
            ZipEntry entry;
            while ((entry = zip.getNextEntry()) != null) {
                ByteArrayOutputStream bytes = new ByteArrayOutputStream();
                byte[] buffer = new byte[8192];
                int count;
                while ((count = zip.read(buffer)) != -1) { bytes.write(buffer, 0, count); }
                contents.put(entry.getName(), bytes.toByteArray());
            }
        }
        Path fixture = directory.resolve("fixture");
        JSONObject manifest = KnowledgeManifestTest.writeFixture(fixture, false);
        JSONObject files = new JSONObject();
        StringBuilder hashes = new StringBuilder();
        for (Map.Entry<String, byte[]> file : new TreeMap<>(contents).entrySet()) {
            JSONObject metadata = new JSONObject();
            String hash = KnowledgeManifest.sha256(file.getValue());
            metadata.put("sha256", hash);
            metadata.put("size", file.getValue().length);
            files.put(file.getKey(), metadata);
            hashes.append(file.getKey()).append('\0').append(hash).append('\n');
        }
        manifest.put("files", files);
        manifest.put("payload_sha256", KnowledgeManifest.sha256(hashes.toString().getBytes(StandardCharsets.UTF_8)));
        contents.put(KnowledgeManifest.NAME, manifest.toJSONString().getBytes(StandardCharsets.UTF_8));
        if (corrupt) { contents.get("firmware/readme.md")[0] = 1; }
        ByteArrayOutputStream bytes = new ByteArrayOutputStream();
        try (ZipOutputStream zip = new ZipOutputStream(bytes)) {
            for (Map.Entry<String, byte[]> file : contents.entrySet()) {
                ZipEntry entry = new ZipEntry(file.getKey());
                CRC32 crc = new CRC32();
                crc.update(file.getValue());
                entry.setSize(file.getValue().length);
                entry.setCrc(crc.getValue());
                entry.setMethod(ZipEntry.STORED);
                zip.putNextEntry(entry);
                zip.write(file.getValue());
                zip.closeEntry();
            }
        }
        return bytes.toByteArray();
    }

    private Path zip() { return directory.resolve(FirmwareSourceCodeDownloader.ARCHIVE_NAME); }
    private void cache(byte[] contents, Instant modified) throws IOException {
        Files.write(zip(), contents);
        Files.setLastModifiedTime(zip(), FileTime.from(modified));
    }
    private void assertNoTemporaryFiles() throws IOException {
        try (DirectoryStream<Path> paths = Files.newDirectoryStream(directory, ".source-*")) {
            assertFalse(paths.iterator().hasNext());
        }
    }
    private static byte[] archive(String name, boolean large) {
        try {
            ByteArrayOutputStream bytes = new ByteArrayOutputStream();
            try (ZipOutputStream zip = new ZipOutputStream(bytes)) {
                byte[] content = large ? new byte[(int) FirmwareSourceCodeDownloader.MIN_ARCHIVE_SIZE] : new byte[]{1};
                CRC32 crc = new CRC32();
                crc.update(content);
                ZipEntry entry = new ZipEntry(name);
                entry.setMethod(ZipEntry.STORED);
                entry.setSize(content.length);
                entry.setCrc(crc.getValue());
                zip.putNextEntry(entry);
                zip.write(content);
                zip.closeEntry();
                zip.putNextEntry(new ZipEntry("rusefi_documentation/Help.md"));
                zip.write("Wiki text".getBytes(StandardCharsets.UTF_8));
                zip.closeEntry();
            }
            return bytes.toByteArray();
        } catch (IOException e) {
            throw new AssertionError(e);
        }
    }
}
