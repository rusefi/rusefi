package com.rusefi.maintenance;

import org.junit.jupiter.api.Test;
import org.junit.jupiter.api.io.TempDir;

import java.io.IOException;
import java.nio.charset.StandardCharsets;
import java.nio.file.Files;
import java.nio.file.Path;
import java.util.Map;
import java.util.zip.ZipEntry;
import java.util.zip.ZipOutputStream;

import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertThrows;
import static org.junit.jupiter.api.Assertions.assertTrue;

class DfuRecoveryFirmwareTest {
    private static final String TARGET = "test_board";

    @TempDir
    Path temporaryDirectory;

    @Test
    void extractsOnlyMatchingRootFullBinIntoFreshDirectory() throws Exception {
        Path zip = zip("rusefi_bundle_test_board_autoupdate.zip", Map.of(
            "rusefi_master_2026-09-15_test_board_123_abc.bin", "new firmware",
            "console/rusefi.bin", "must not extract"));

        String result = DfuRecoveryFirmware.extractDfuImage(TARGET, zip, temporaryDirectory);

        assertEquals("new firmware", Files.readString(Path.of(result)));
        assertTrue(Path.of(result).getParent().startsWith(temporaryDirectory));
    }

    @Test
    void rejectsArchiveWithoutFullBin() throws Exception {
        Path zip = zip("rusefi_bundle_test_board_autoupdate.zip", Map.of("openblt.bin", "bootloader"));

        assertNoDfuImage(() -> DfuRecoveryFirmware.extractDfuImage(TARGET, zip, temporaryDirectory));
    }

    @Test
    void rejectsObfuscatedArchiveAndImage() throws Exception {
        Path archive = zip("rusefi_bundle_test_board_obfuscated_public_autoupdate.zip", Map.of("rusefi.bin", "firmware"));
        assertNoDfuImage(() -> DfuRecoveryFirmware.extractDfuImage(TARGET, archive, temporaryDirectory));

        Path image = zip("rusefi_bundle_test_board_autoupdate.zip", Map.of("rusefi-obfuscated.bin", "firmware"));
        assertNoDfuImage(() -> DfuRecoveryFirmware.extractDfuImage(TARGET, image, temporaryDirectory));
    }

    @Test
    void doesNotReuseStaleBinFromExtractionParent() throws Exception {
        Files.writeString(temporaryDirectory.resolve("rusefi.bin"), "stale firmware");
        Path zip = zip("rusefi_bundle_test_board_autoupdate.zip", Map.of("rusefi.bin", "new firmware"));

        String result = DfuRecoveryFirmware.extractDfuImage(TARGET, zip, temporaryDirectory);

        assertEquals("new firmware", Files.readString(Path.of(result)));
        assertEquals("stale firmware", Files.readString(temporaryDirectory.resolve("rusefi.bin")));
    }

    @Test
    void rejectsAmbiguousAndMismatchedTargetImages() throws Exception {
        Path ambiguous = zip("rusefi_bundle_test_board_autoupdate.zip", Map.of(
            "rusefi.bin", "one", "rusefi_master_2026-09-15_test_board_123_abc.bin", "two"));
        assertNoDfuImage(() -> DfuRecoveryFirmware.extractDfuImage(TARGET, ambiguous, temporaryDirectory));

        Path mismatched = zip("rusefi_bundle_test_board_autoupdate.zip", Map.of(
            "rusefi_master_2026-09-15_other_board_123_abc.bin", "wrong board"));
        assertNoDfuImage(() -> DfuRecoveryFirmware.extractDfuImage(TARGET, mismatched, temporaryDirectory));
    }

    @Test
    void rejectsTargetPrefixMatchAndEmptyImage() throws Exception {
        Path prefix = zip("firmware.zip", Map.of(
            "rusefi_master_2026-09-15_test_board_pro_123_abc.bin", "different board"));
        assertNoDfuImage(() -> DfuRecoveryFirmware.extractDfuImage(TARGET, prefix, temporaryDirectory));
        Path empty = zip("empty.zip", Map.of("rusefi.bin", ""));
        assertNoDfuImage(() -> DfuRecoveryFirmware.extractDfuImage(TARGET, empty, temporaryDirectory));
    }

    private Path zip(String name, Map<String, String> entries) throws IOException {
        Path zip = temporaryDirectory.resolve(name);
        try (ZipOutputStream output = new ZipOutputStream(Files.newOutputStream(zip))) {
            for (Map.Entry<String, String> entry : entries.entrySet()) {
                output.putNextEntry(new ZipEntry(entry.getKey()));
                output.write(entry.getValue().getBytes(StandardCharsets.UTF_8));
                output.closeEntry();
            }
        }
        return zip;
    }

    private static void assertNoDfuImage(ThrowingRunnable action) {
        IOException error = assertThrows(IOException.class, action::run);
        assertTrue(error.getMessage().contains("No DFU image"));
        assertTrue(error.getMessage().contains("OpenBLT"));
    }

    @FunctionalInterface
    private interface ThrowingRunnable {
        void run() throws IOException;
    }
}
