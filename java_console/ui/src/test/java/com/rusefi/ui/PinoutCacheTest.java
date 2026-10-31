package com.rusefi.ui;

import org.junit.jupiter.api.Test;
import org.junit.jupiter.api.io.TempDir;

import java.io.IOException;
import java.nio.charset.StandardCharsets;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.StandardCopyOption;
import java.util.List;
import java.util.zip.ZipEntry;
import java.util.zip.ZipOutputStream;

import static org.junit.jupiter.api.Assertions.assertArrayEquals;
import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertFalse;
import static org.junit.jupiter.api.Assertions.assertThrows;
import static org.junit.jupiter.api.Assertions.assertTrue;

class PinoutCacheTest {
    @TempDir
    Path tempDir;

    /**
     * Regression for issue #10178: the cache decision must inspect the archive instead of
     * trusting the SHA copied into boards_meta.yaml.
     */
    @Test
    void matchingMetadataHashDoesNotHideStaleArchive() throws Exception {
        Path staleArchive = tempDir.resolve("connectors.zip");
        Files.writeString(staleArchive, "old archive without Super or Huge");

        assertFalse(PinoutCache.isCurrent(staleArchive.toFile(), "new-sha"));
    }

    @Test
    void matchingArchiveIsCurrent() throws Exception {
        Path archive = tempDir.resolve("connectors.zip");
        Files.writeString(archive, "Super and Huge connector data");

        assertTrue(PinoutCache.isCurrent(archive.toFile(), PinoutCache.sha256(archive.toFile())));
    }

    @Test
    void computesStandardSha256() throws Exception {
        Path archive = tempDir.resolve("digest-input");
        Files.writeString(archive, "abc");

        assertEquals("ba7816bf8f01cfea414140de5dae2223b00361a396177a9cb410ff61f20015ad",
            PinoutCache.sha256(archive.toFile()));
    }

    @Test
    void validDownloadAtomicallyReplacesStaleArchive() throws Exception {
        Path archive = tempDir.resolve("super-pinouts.zip");
        Files.writeString(archive, "stale uaEFI-only archive");
        Path expected = tempDir.resolve("expected.zip");
        Files.writeString(expected, "Super connector data");

        PinoutCache.downloadAndReplace(archive.toFile(), PinoutCache.sha256(expected.toFile()),
            destination -> Files.writeString(destination.toPath(), "Super connector data"));

        assertEquals("Super connector data", Files.readString(archive));
    }

    @Test
    void invalidDownloadPreservesExistingArchive() throws Exception {
        Path archive = tempDir.resolve("huge-pinouts.zip");
        Files.writeString(archive, "previous Huge connector data");

        assertThrows(IOException.class, () -> PinoutCache.downloadAndReplace(
            archive.toFile(), "expected-sha",
            destination -> Files.writeString(destination.toPath(), "truncated download")));

        assertEquals("previous Huge connector data", Files.readString(archive));
    }

    @Test
    void universalConsoleRejectsArchiveWithoutSuperOrHugeConnectors() throws Exception {
        Path uaefiOnly = tempDir.resolve("connectors.zip");
        writeZip(uaefiOnly, List.of("hellen/uaefi/connectors/uaefi-A.yaml"));
        String actualSha = PinoutCache.sha256(uaefiOnly.toFile());

        assertFalse(PinoutCache.isCurrent(uaefiOnly.toFile(), actualSha, List.of(
            "hellen/super-uaefi/connectors/26pin_4k.yaml",
            "hellen/super-uaefi/connectors/60pin.yaml",
            "hellen/super-uaefi/connectors/C-34pins-4k.yaml")));
        assertFalse(PinoutCache.isCurrent(uaefiOnly.toFile(), actualSha, List.of(
            "hellen/alphax-8chan/connectors/8chan-A-B.yaml",
            "hellen/alphax-8chan/connectors/8chan-C.yaml",
            "hellen/alphax-8chan/connectors/8chan-D.yaml")));
    }

    @Test
    void universalConsoleAcceptsArchiveWithSuperAndHugeConnectors() throws Exception {
        Path archive = tempDir.resolve("connectors.zip");
        List<String> superConnectors = List.of(
            "hellen/super-uaefi/connectors/26pin_4k.yaml",
            "hellen/super-uaefi/connectors/60pin.yaml",
            "hellen/super-uaefi/connectors/C-34pins-4k.yaml");
        List<String> hugeConnectors = List.of(
            "hellen/alphax-8chan/connectors/8chan-A-B.yaml",
            "hellen/alphax-8chan/connectors/8chan-C.yaml",
            "hellen/alphax-8chan/connectors/8chan-D.yaml");
        writeZip(archive, List.of(
            superConnectors.get(0), superConnectors.get(1), superConnectors.get(2),
            hugeConnectors.get(0), hugeConnectors.get(1), hugeConnectors.get(2)));
        String actualSha = PinoutCache.sha256(archive.toFile());

        assertTrue(PinoutCache.isCurrent(archive.toFile(), actualSha, superConnectors));
        assertTrue(PinoutCache.isCurrent(archive.toFile(), actualSha, hugeConnectors));
    }

    @Test
    void staleArchiveWithRequestedConnectorsRemainsUsableDuringUpdate() throws Exception {
        Path archive = tempDir.resolve("connectors.zip");
        List<String> superConnectors = List.of(
            "hellen/super-uaefi/connectors/26pin_4k.yaml",
            "hellen/super-uaefi/connectors/60pin.yaml",
            "hellen/super-uaefi/connectors/C-34pins-4k.yaml");
        writeZip(archive, superConnectors);

        assertFalse(PinoutCache.isCurrent(archive.toFile(), "new-archive-sha", superConnectors));
        assertTrue(PinoutCache.containsEntries(archive.toFile(), superConnectors));
    }

    @Test
    void downloadMissingRequestedBoardConnectorsPreservesExistingArchive() throws Exception {
        Path archive = tempDir.resolve("connectors.zip");
        Files.writeString(archive, "previous archive");
        byte[] previousArchive = Files.readAllBytes(archive);

        Path downloaded = tempDir.resolve("downloaded.zip");
        writeZip(downloaded, List.of("hellen/uaefi/connectors/uaefi-A.yaml"));

        assertThrows(IOException.class, () -> PinoutCache.downloadAndReplace(
            archive.toFile(), PinoutCache.sha256(downloaded.toFile()), List.of(
                "hellen/super-uaefi/connectors/60pin.yaml"),
            destination -> Files.copy(downloaded, destination.toPath(), StandardCopyOption.REPLACE_EXISTING)));

        assertArrayEquals(previousArchive, Files.readAllBytes(archive));
    }

    private static void writeZip(Path archive, List<String> entries) throws Exception {
        try (ZipOutputStream output = new ZipOutputStream(Files.newOutputStream(archive))) {
            for (String entry : entries) {
                output.putNextEntry(new ZipEntry(entry));
                output.write("pins: []\n".getBytes(StandardCharsets.UTF_8));
                output.closeEntry();
            }
        }
    }
}
