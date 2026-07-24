package com.rusefi.autoupdate;

import org.junit.jupiter.api.Test;
import org.junit.jupiter.api.io.TempDir;

import java.io.IOException;
import java.nio.charset.StandardCharsets;
import java.nio.file.Files;
import java.nio.file.Path;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;
import java.util.zip.ZipEntry;
import java.util.zip.ZipOutputStream;

import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertFalse;
import static org.junit.jupiter.api.Assertions.assertTrue;

class AutoupdateFirmwareDownloadTest {
    private static final String SHA = "cae1680e40511aa8d798edf60f4cb0c95ce3d841";

    @TempDir
    Path tempDir;

    @Test
    void fallsBackToObfuscatedZipAndExtractsOnlyFirmware() {
        FirmwareRollbackResolver.Build build = build();
        List<Boolean> attempts = new ArrayList<>();

        Autoupdate.FirmwareArtifacts artifacts = Autoupdate.downloadFirmware(
            build, ignored -> {}, ignored -> {}, tempDir,
            (ignoredBuild, destination, obfuscated, progress, logger) -> {
                attempts.add(obfuscated);
                if (!obfuscated) {
                    return null;
                }
                return createFirmwareZip(destination.resolve("firmware.zip"));
            });

        assertEquals(Arrays.asList(false, true), attempts);
        Path expectedDirectory = tempDir.resolve("uaefi-121/lts-26-test/" + SHA).toAbsolutePath();
        assertEquals(expectedDirectory.resolve("firmware.srec").toString(), artifacts.getSrecPath().orElse(null));
        assertEquals(expectedDirectory.resolve("rusefi.bin").toString(), artifacts.getBinPath().orElse(null));
        assertTrue(Files.exists(expectedDirectory.resolve("firmware.srec")));
        assertTrue(Files.exists(expectedDirectory.resolve("rusefi.bin")));
        assertTrue(Files.exists(expectedDirectory.resolve("openblt.bin")));
        assertFalse(Files.exists(expectedDirectory.resolve("console/rusefi_console.jar")));
        assertFalse(Files.exists(expectedDirectory.resolve("console/release.txt")));
    }

    @Test
    void returnsEmptyArtifactsWhenBothZipNamesAreUnavailable() {
        List<Boolean> attempts = new ArrayList<>();

        Autoupdate.FirmwareArtifacts artifacts = Autoupdate.downloadFirmware(
            build(), ignored -> {}, ignored -> {}, tempDir,
            (ignoredBuild, destination, obfuscated, progress, logger) -> {
                attempts.add(obfuscated);
                return null;
            });

        assertEquals(Arrays.asList(false, true), attempts);
        assertFalse(artifacts.getSrecPath().isPresent());
        assertFalse(artifacts.getBinPath().isPresent());
    }

    private static FirmwareRollbackResolver.Build build() {
        String html = "<a href=\"" + SHA + "/\">build</a> 2026-07-23 19:26 -";
        return FirmwareRollbackResolver.parseIndex(html, "uaefi-121", "lts-26-test",
            "https://example.test/uaefi-121/lts-26-test/").get(0);
    }

    private static Path createFirmwareZip(Path zip) throws IOException {
        try (ZipOutputStream output = new ZipOutputStream(Files.newOutputStream(zip))) {
            add(output, "firmware.srec");
            add(output, "rusefi.bin");
            add(output, "openblt.bin");
            add(output, "console/rusefi_console.jar");
            add(output, "console/release.txt");
        }
        return zip;
    }

    private static void add(ZipOutputStream output, String name) throws IOException {
        output.putNextEntry(new ZipEntry(name));
        output.write(name.getBytes(StandardCharsets.UTF_8));
        output.closeEntry();
    }
}
