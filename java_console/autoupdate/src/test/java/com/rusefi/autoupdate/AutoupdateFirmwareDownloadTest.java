package com.rusefi.autoupdate;

import com.rusefi.core.io.BundleInfo;
import org.junit.jupiter.api.Test;
import org.junit.jupiter.api.io.TempDir;

import java.io.IOException;
import java.nio.charset.StandardCharsets;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.nio.file.attribute.FileTime;
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
        assertTrue(artifacts.getSrecPath().isPresent());
        assertTrue(artifacts.getBinPath().isPresent());
        assertEquals("lts-26-test", artifacts.getBundleInfo().getBranchName());
        assertTrue(Files.exists(Paths.get(artifacts.getSrecPath().get())));
        assertTrue(Files.exists(Paths.get(artifacts.getBinPath().get())));
        Path directory = Paths.get(artifacts.getBinPath().get()).getParent();
        assertFalse(Files.exists(directory.resolve("openblt.bin")));
        assertFalse(Files.exists(directory.resolve("console/rusefi_console.jar")));
        assertFalse(Files.exists(directory.resolve("console/release.txt")));
        assertFalse(Files.exists(directory.resolve("bin/wipe/emergency_wipe.srec")));
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

    @Test
    void branchDownloadUsesIsolatedNormalizedCacheAndValidatesReleaseMetadata() throws Exception {
        Path root = tempDir.resolve("cache");
        Path active = tempDir.resolve("active");
        Files.createDirectories(active);
        Files.write(active.resolve("rusefi.bin"), new byte[]{1});
        Files.write(active.resolve("release.txt"), "console metadata".getBytes(StandardCharsets.UTF_8));
        BundleInfo wanted = new BundleInfo("master", null, "board-x");
        List<String> urls = new ArrayList<>();
        Autoupdate.FirmwareArtifacts result = Autoupdate.downloadFirmwareForBranch(wanted,
            ignored -> {}, ignored -> {}, root,
            (info, destination, obfuscated, progress, logger) -> {
                urls.add(info.getTarget() + "/" + info.getBranchName() + "/" + obfuscated);
                if (obfuscated) {
                    return null;
                }
                return branchZip(destination.resolve("firmware.zip"), "board-x", "development", "one");
            });
        assertEquals(Arrays.asList("board-x/development/false"), urls);
        assertEquals("development", result.getBundleInfo().getBranchName());
        assertTrue(result.getBinPath().get().startsWith(root.toAbsolutePath().toString()));
        assertTrue(Files.exists(active.resolve("rusefi.bin")));
        assertTrue(Files.exists(active.resolve("release.txt")));
        assertTrue(Arrays.equals(new byte[]{1}, Files.readAllBytes(active.resolve("rusefi.bin"))));
        assertEquals("console metadata", new String(Files.readAllBytes(active.resolve("release.txt")), StandardCharsets.UTF_8));
        assertFalse(Files.exists(active.resolve("firmware.srec")));
    }

    @Test
    void mismatchedMetadataReturnsEmptyWithoutExtractingArtifacts() throws Exception {
        Path root = tempDir.resolve("cache");
        Autoupdate.FirmwareArtifacts result = Autoupdate.downloadFirmwareForBranch(
            new BundleInfo("lts-26lima", null, "board-x"), ignored -> {}, ignored -> {}, root,
            (info, destination, obfuscated, progress, logger) -> obfuscated ? null
                : branchZip(destination.resolve("firmware.zip"), "board-x", "development", "one"));
        assertFalse(result.getBinPath().isPresent());
        assertFalse(Files.exists(root.resolve("board-x/lts-26lima/rusefi.bin")));
    }

    @Test
    void resolvesPackagedObfuscatedSrecWithoutPublicBin() throws Exception {
        Path root = tempDir.resolve("obfuscated-cache");
        Autoupdate.FirmwareArtifacts result = Autoupdate.downloadFirmwareForBranch(
            new BundleInfo("development", null, "board-x"), ignored -> {}, ignored -> {}, root,
            (info, destination, obfuscated, progress, logger) -> obfuscated ? null
                : obfuscatedSrecZip(destination.resolve("firmware.zip")));
        assertTrue(result.getSrecPath().isPresent());
        assertFalse(result.getBinPath().isPresent());
        assertEquals("development", result.getBundleInfo().getBranchName());
    }

    @Test
    void acceptsKansasLegacySrecForBoardProvenByReleaseMetadata() throws Exception {
        for (boolean obfuscated : Arrays.asList(false, true)) {
            Path root = tempDir.resolve("kansas-legacy-" + obfuscated);
            Autoupdate.FirmwareArtifacts result = Autoupdate.downloadFirmwareForBranch(
                new BundleInfo("lts-25kansas", null, "board-x"), ignored -> {}, ignored -> {}, root,
                (info, destination, obfuscatedRequest, progress, logger) -> obfuscatedRequest == obfuscated
                    ? legacyKansasZip(destination.resolve("firmware.zip"), obfuscated) : null);
            assertTrue(result.getSrecPath().isPresent());
            assertFalse(result.getBinPath().isPresent());
            assertEquals("board-x", result.getBundleInfo().getTarget());
        }
    }

    @Test
    void validatesNamedBinIdentityAndAllowsOnlyCanonicalLegacyBin() throws Exception {
        BundleInfo wanted = new BundleInfo("development", null, "board-x");
        for (String name : Arrays.asList(
            "rusefi_development_2026-02-03_board-x_1234_hash.bin",
            "rusefi.bin")) {
            Autoupdate.FirmwareArtifacts valid = Autoupdate.downloadFirmwareForBranch(wanted,
                ignored -> {}, ignored -> {}, tempDir.resolve("valid-bin-" + name.hashCode()),
                (info, destination, obfuscated, progress, logger) -> obfuscated ? null
                    : binOnlyZip(destination.resolve("firmware.zip"), name));
            assertTrue(valid.getBinPath().isPresent(), name);
        }
        for (String name : Arrays.asList(
            "rusefi_development_2026-02-03_other-board_1234_hash.bin",
            "rusefi_lts-25kansas_2026-02-03_board-x_1234_hash.bin",
            "rusefi_other.bin")) {
            Autoupdate.FirmwareArtifacts invalid = Autoupdate.downloadFirmwareForBranch(wanted,
                ignored -> {}, ignored -> {}, tempDir.resolve("invalid-bin-" + name.hashCode()),
                (info, destination, obfuscated, progress, logger) -> obfuscated ? null
                    : binOnlyZip(destination.resolve("firmware.zip"), name));
            assertFalse(invalid.getBinPath().isPresent(), name);
        }
    }

    @Test
    void identicalZipReuseDoesNotRewriteSelectedFileAndCorruptCacheFailsClosed() throws Exception {
        Path root = tempDir.resolve("immutable-cache");
        Files.createDirectories(root.resolve("board-x/development"));
        Path zip = branchZip(root.resolve("board-x/development/firmware.zip"), "board-x", "development", "one");
        BundleInfo wanted = new BundleInfo("development", null, "board-x");
        Autoupdate.BranchFirmwareZipDownloader downloader = (info, destination, obfuscated, progress, logger) -> obfuscated ? null : zip;
        Autoupdate.FirmwareArtifacts first = Autoupdate.downloadFirmwareForBranch(wanted, ignored -> {}, ignored -> {}, root, downloader);
        Path bin = Paths.get(first.getBinPath().get());
        byte[] selectedBytes = Files.readAllBytes(bin);
        FileTime selectedTime = Files.getLastModifiedTime(bin);
        Autoupdate.FirmwareArtifacts repeated = Autoupdate.downloadFirmwareForBranch(wanted, ignored -> {}, ignored -> {}, root, downloader);
        assertEquals(first.getBinPath(), repeated.getBinPath());
        assertEquals(selectedTime, Files.getLastModifiedTime(bin));
        assertTrue(Arrays.equals(selectedBytes, Files.readAllBytes(bin)));

        Files.write(bin, "corrupt".getBytes(StandardCharsets.UTF_8));
        byte[] corruptBytes = Files.readAllBytes(bin);
        FileTime corruptTime = Files.getLastModifiedTime(bin);
        Autoupdate.FirmwareArtifacts refused = Autoupdate.downloadFirmwareForBranch(wanted, ignored -> {}, ignored -> {}, root, downloader);
        assertFalse(refused.getBinPath().isPresent());
        assertTrue(Arrays.equals(corruptBytes, Files.readAllBytes(bin)));
        assertEquals(corruptTime, Files.getLastModifiedTime(bin));
    }

    @Test
    void identicalLargeArtifactReuseDoesNotDependOnStreamReadChunkSizes() throws Exception {
        Path root = tempDir.resolve("large-cache");
        Files.createDirectories(root);
        java.util.Random random = new java.util.Random(1234);
        StringBuilder contents = new StringBuilder();
        for (int i = 0; i < 20_000; i++) {
            contents.append((char) (32 + random.nextInt(95)));
        }
        Path zip = branchZip(root.resolve("firmware.zip"), "board-x", "development", contents.toString());
        BundleInfo wanted = new BundleInfo("development", null, "board-x");
        Autoupdate.BranchFirmwareZipDownloader downloader = (info, destination, obfuscated, progress, logger) -> zip;
        Autoupdate.FirmwareArtifacts first = Autoupdate.downloadFirmwareForBranch(
            wanted, ignored -> {}, ignored -> {}, root, downloader);
        Autoupdate.FirmwareArtifacts repeated = Autoupdate.downloadFirmwareForBranch(
            wanted, ignored -> {}, ignored -> {}, root, downloader);
        assertTrue(first.getBinPath().isPresent());
        assertEquals(first.getBinPath(), repeated.getBinPath());
    }

    @Test
    void historicalDownloadReturnsEmptyForMalformedMetadataAndInvalidZipPaths() throws Exception {
        for (boolean invalidPath : Arrays.asList(false, true)) {
            Autoupdate.FirmwareArtifacts result = Autoupdate.downloadFirmware(build(), ignored -> {}, ignored -> {},
                tempDir.resolve("historical" + invalidPath), (ignoredBuild, destination, obfuscated, progress, logger) ->
                    historicalMalformedZip(destination.resolve("firmware.zip"), invalidPath));
            assertFalse(result.getBinPath().isPresent());
            assertFalse(result.getSrecPath().isPresent());
        }
    }

    @Test
    void selectedArtifactsRemainImmutableAcrossBuildsAndFallbackStaysOnBranch() throws Exception {
        Path root = tempDir.resolve("cache");
        List<Boolean> attempts = new ArrayList<>();
        Autoupdate.BranchFirmwareZipDownloader downloader = (info, destination, obfuscated, progress, logger) -> {
            attempts.add(obfuscated);
            if (!obfuscated) {
                return null;
            }
            return branchZip(destination.resolve("firmware-" + attempts.size() + ".zip"), "board-x", "development", "build-" + attempts.size());
        };
        BundleInfo wanted = new BundleInfo("development", null, "board-x");
        Autoupdate.FirmwareArtifacts first = Autoupdate.downloadFirmwareForBranch(wanted, ignored -> {}, ignored -> {}, root, downloader);
        byte[] firstBytes = Files.readAllBytes(Paths.get(first.getBinPath().get()));
        Autoupdate.FirmwareArtifacts second = Autoupdate.downloadFirmwareForBranch(wanted, ignored -> {}, ignored -> {}, root, downloader);
        assertFalse(first.getBinPath().get().equals(second.getBinPath().get()));
        assertEquals("build-2", new String(Files.readAllBytes(Paths.get(first.getBinPath().get())), StandardCharsets.UTF_8));
        assertEquals("build-4", new String(Files.readAllBytes(Paths.get(second.getBinPath().get())), StandardCharsets.UTF_8));
        assertTrue(firstBytes.length > 0);
        assertEquals(Arrays.asList(false, true, false, true), attempts);
    }

    @Test
    void rejectsMissingOrMismatchedMetadataDuplicateCandidatesAndTraversal() throws Exception {
        Path root = tempDir.resolve("cache");
        BundleInfo wanted = new BundleInfo("development", null, "board-x");
        for (String entry : Arrays.asList("../escape.bin", "/absolute.bin", "C:/absolute.bin", "C:\\absolute.bin")) {
            Autoupdate.FirmwareArtifacts result = Autoupdate.downloadFirmwareForBranch(wanted,
                ignored -> {}, ignored -> {}, root.resolve(entry.hashCode() + ""),
                (info, destination, obfuscated, progress, logger) -> obfuscated ? null
                    : unsafeZip(destination.resolve("firmware.zip"), entry));
            assertFalse(result.getBinPath().isPresent(), entry);
        }
        Autoupdate.FirmwareArtifacts missing = Autoupdate.downloadFirmwareForBranch(wanted,
            ignored -> {}, ignored -> {}, root.resolve("missing"),
            (info, destination, obfuscated, progress, logger) -> obfuscated ? null
                : zipWithoutMetadata(destination.resolve("firmware.zip")));
        assertFalse(missing.getBinPath().isPresent());
        Autoupdate.FirmwareArtifacts wrongSrec = Autoupdate.downloadFirmwareForBranch(wanted,
            ignored -> {}, ignored -> {}, root.resolve("wrong-srec"),
            (info, destination, obfuscated, progress, logger) -> obfuscated ? null
                : branchZip(destination.resolve("firmware.zip"), "board-x", "development", "one", "other-board"));
        assertFalse(wrongSrec.getBinPath().isPresent());
        assertFalse(Files.exists(root.resolve("wrong-srec/board-x/development/rusefi.bin")));
        Autoupdate.FirmwareArtifacts duplicate = Autoupdate.downloadFirmwareForBranch(wanted,
            ignored -> {}, ignored -> {}, root.resolve("duplicates"),
            (info, destination, obfuscated, progress, logger) -> obfuscated ? null
                : duplicateZip(destination.resolve("firmware.zip")));
        assertFalse(duplicate.getBinPath().isPresent());
        Autoupdate.FirmwareArtifacts duplicateMetadata = Autoupdate.downloadFirmwareForBranch(wanted,
            ignored -> {}, ignored -> {}, root.resolve("duplicate-metadata"),
            (info, destination, obfuscated, progress, logger) -> obfuscated ? null
                : duplicateMetadataZip(destination.resolve("firmware.zip")));
        assertFalse(duplicateMetadata.getBinPath().isPresent());
    }

    private static Path branchZip(Path zip, String platform, String branch, String binContents) throws IOException {
        return branchZip(zip, platform, branch, binContents, platform);
    }

    private static Path branchZip(Path zip, String platform, String branch, String binContents, String srecPlatform) throws IOException {
        try (ZipOutputStream output = new ZipOutputStream(Files.newOutputStream(zip))) {
            add(output, "console/release.txt", "platform=" + platform + "\nrelease=" + branch + "\n");
            add(output, "rusefi_" + branch + "_2026-02-03_" + srecPlatform + "_1234_hash_update.srec");
            add(output, "rusefi.bin", binContents);
            add(output, "openblt.bin");
        }
        return zip;
    }

    private static Path duplicateZip(Path zip) throws IOException {
        try (ZipOutputStream output = new ZipOutputStream(Files.newOutputStream(zip))) {
            add(output, "console/release.txt", "platform=board-x\nrelease=development\n");
            add(output, "rusefi.bin", "one");
            add(output, "rusefi_other.bin", "two");
        }
        return zip;
    }

    private static Path duplicateMetadataZip(Path zip) throws IOException {
        try (ZipOutputStream output = new ZipOutputStream(Files.newOutputStream(zip))) {
            add(output, "console/release.txt", "platform=board-x\nrelease=development\n");
            add(output, "console/releasz.txt", "platform=board-x\nrelease=development\n");
            add(output, "rusefi.bin");
        }
        // ZipOutputStream forbids duplicate names; replace equal-length header names after writing.
        String bytes = new String(Files.readAllBytes(zip), StandardCharsets.ISO_8859_1);
        Files.write(zip, bytes.replace("console/releasz.txt", "console/release.txt")
            .getBytes(StandardCharsets.ISO_8859_1));
        return zip;
    }

    private static Path legacyKansasZip(Path zip, boolean obfuscated) throws IOException {
        try (ZipOutputStream output = new ZipOutputStream(Files.newOutputStream(zip))) {
            add(output, "console/release.txt", "platform=board-x\nrelease=lts-25kansas\n");
            add(output, obfuscated
                ? "rusefi-lts-25kansas_2026-02-03_8849742d4267db6407b1400ae917a1ed39795d32_obfuscated.srec"
                : "rusefi_lts-25kansas_2026-02-03_8849742d4267db6407b1400ae917a1ed39795d32.srec");
        }
        return zip;
    }

    private static Path binOnlyZip(Path zip, String binName) throws IOException {
        try (ZipOutputStream output = new ZipOutputStream(Files.newOutputStream(zip))) {
            add(output, "console/release.txt", "platform=board-x\nrelease=development\n");
            add(output, binName);
        }
        return zip;
    }

    private static Path historicalMalformedZip(Path zip, boolean invalidPath) throws IOException {
        try (ZipOutputStream output = new ZipOutputStream(Files.newOutputStream(zip))) {
            add(output, "console/release.txt", invalidPath
                ? "platform=uaefi-121\nrelease=lts-26-test\n"
                : "platform=uaefi-121\nrelease=lts-26-test\nkey=" + '\\' + "uZZZZ\n");
            add(output, invalidPath ? "bad\u0000name.bin" : "rusefi.bin");
        }
        return zip;
    }

    private static Path unsafeZip(Path zip, String entry) throws IOException {
        try (ZipOutputStream output = new ZipOutputStream(Files.newOutputStream(zip))) {
            add(output, "console/release.txt", "platform=board-x\nrelease=development\n");
            add(output, entry);
        }
        return zip;
    }

    private static Path zipWithoutMetadata(Path zip) throws IOException {
        try (ZipOutputStream output = new ZipOutputStream(Files.newOutputStream(zip))) {
            add(output, "rusefi.bin");
        }
        return zip;
    }

    private static Path obfuscatedSrecZip(Path zip) throws IOException {
        try (ZipOutputStream output = new ZipOutputStream(Files.newOutputStream(zip))) {
            add(output, "console/release.txt", "platform=board-x\nrelease=development\n");
            add(output, "rusefi-development_2026-02-03_board-x_1234_8849742d4267db6407b1400ae917a1ed39795d32_obfuscated.srec");
        }
        return zip;
    }

    private static FirmwareRollbackResolver.Build build() {
        String html = "<a href=\"" + SHA + "/\">build</a> 2026-07-23 19:26 -";
        return FirmwareRollbackResolver.parseIndex(html, "uaefi-121", "lts-26-test",
            "https://example.test/build_server/lts/uaefi-121/lts-26-test/").get(0);
    }

    private static Path createFirmwareZip(Path zip) throws IOException {
        try (ZipOutputStream output = new ZipOutputStream(Files.newOutputStream(zip))) {
            add(output, "rusefi_lts-26-test_2026-02-03_uaefi-121_1234_hash_update.srec");
            add(output, "rusefi.bin");
            add(output, "openblt.bin");
            add(output, "console/release.txt", "platform=uaefi-121\nrelease=lts-26-test\n");
            add(output, "console/rusefi_console.jar");
            add(output, "bin/wipe/emergency_wipe.srec");
        }
        return zip;
    }

    private static void add(ZipOutputStream output, String name) throws IOException {
        add(output, name, name);
    }

    private static void add(ZipOutputStream output, String name, String contents) throws IOException {
        output.putNextEntry(new ZipEntry(name));
        output.write(contents.getBytes(StandardCharsets.UTF_8));
        output.closeEntry();
    }
}
