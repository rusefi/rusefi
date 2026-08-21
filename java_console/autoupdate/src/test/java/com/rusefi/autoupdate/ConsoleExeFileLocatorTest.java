package com.rusefi.autoupdate;

import org.junit.jupiter.api.Test;
import org.junit.jupiter.api.io.TempDir;

import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Path;

import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertFalse;
import static org.junit.jupiter.api.Assertions.assertTrue;

/**
 * #8360: the launcher lives at the bundle root while the console runs with `console` as its working
 * directory, and on Linux/macOS the launcher is a `.sh` rather than an `.exe`.
 */
class ConsoleExeFileLocatorTest {
    private static final String PREFIX = "rusefi";

    @TempDir
    Path bundleRoot;

    @Test
    void unixLauncherIsFoundAtBundleRootWhileRunningFromConsoleFolder() throws IOException {
        final Path launcher = createFile(bundleRoot.resolve("rusefi_updater.sh"));
        final Path consoleFolder = Files.createDirectories(bundleRoot.resolve("console"));

        assertEquals(
            normalized(launcher),
            ConsoleExeFileLocator.resolveDefaultLauncher(PREFIX, false, consoleFolder)
        );
    }

    @Test
    void windowsLauncherIsFoundAtBundleRootWhileRunningFromConsoleFolder() throws IOException {
        final Path launcher = createFile(bundleRoot.resolve("rusefi_updater.exe"));
        final Path consoleFolder = Files.createDirectories(bundleRoot.resolve("console"));

        assertEquals(
            normalized(launcher),
            ConsoleExeFileLocator.resolveDefaultLauncher(PREFIX, true, consoleFolder)
        );
    }

    @Test
    void launcherInTheWorkingDirectoryWins() throws IOException {
        final Path consoleFolder = Files.createDirectories(bundleRoot.resolve("console"));
        createFile(bundleRoot.resolve("rusefi_updater.sh"));
        final Path nearestLauncher = createFile(consoleFolder.resolve("rusefi_updater.sh"));

        assertEquals(
            normalized(nearestLauncher),
            ConsoleExeFileLocator.resolveDefaultLauncher(PREFIX, false, consoleFolder)
        );
    }

    @Test
    void windowsLauncherIsNotUsedOnUnix() throws IOException {
        createFile(bundleRoot.resolve("rusefi_updater.exe"));
        final Path consoleFolder = Files.createDirectories(bundleRoot.resolve("console"));

        // no rusefi_updater.sh anywhere, so we fall back to the bare name and the caller reports
        // the familiar "file to launch isn't found" error
        assertEquals(
            "rusefi_updater.sh",
            ConsoleExeFileLocator.resolveDefaultLauncher(PREFIX, false, consoleFolder)
        );
    }

    @Test
    void unixLauncherIsNotUsedOnWindows() throws IOException {
        createFile(bundleRoot.resolve("rusefi_updater.sh"));
        final Path consoleFolder = Files.createDirectories(bundleRoot.resolve("console"));

        assertEquals(
            "rusefi_updater.exe",
            ConsoleExeFileLocator.resolveDefaultLauncher(PREFIX, true, consoleFolder)
        );
    }

    @Test
    void whiteLabelPrefixIsHonored() throws IOException {
        final Path launcher = createFile(bundleRoot.resolve("purple_updater.sh"));
        final Path consoleFolder = Files.createDirectories(bundleRoot.resolve("console"));

        assertEquals(
            normalized(launcher),
            ConsoleExeFileLocator.resolveDefaultLauncher("purple", false, consoleFolder)
        );
    }

    @Test
    void aDirectoryWithTheLauncherNameIsIgnored() throws IOException {
        final Path consoleFolder = Files.createDirectories(bundleRoot.resolve("console"));
        Files.createDirectories(bundleRoot.resolve("rusefi_updater.sh"));

        assertEquals(
            "rusefi_updater.sh",
            ConsoleExeFileLocator.resolveDefaultLauncher(PREFIX, false, consoleFolder)
        );
    }

    @Test
    void packagedBundleWithoutInstallationMarkerIsExtracted() throws IOException {
        final Path consoleFolder = createConsoleFolderWithRelease();

        assertTrue(ConsoleExeFileLocator.isRunningFromUnzippedBundle(consoleFolder, true));
    }

    @Test
    void installationMarkerDistinguishesInstalledBundle() throws IOException {
        final Path consoleFolder = createConsoleFolderWithRelease();
        createFile(consoleFolder.resolve("installation.properties"));

        assertFalse(ConsoleExeFileLocator.isRunningFromUnzippedBundle(consoleFolder, true));
    }

    @Test
    void developmentAndInvalidLayoutsAreNotExtractedBundles() throws IOException {
        final Path consoleFolder = Files.createDirectories(bundleRoot.resolve("console"));

        assertFalse(ConsoleExeFileLocator.isRunningFromUnzippedBundle(consoleFolder, true));

        Files.write(consoleFolder.resolve("release.txt"), "not bundle metadata".getBytes());
        assertFalse(ConsoleExeFileLocator.isRunningFromUnzippedBundle(consoleFolder, true));

        Files.write(consoleFolder.resolve("release.txt"), "platform=universal\nrelease=development\n".getBytes());
        assertFalse(ConsoleExeFileLocator.isRunningFromUnzippedBundle(consoleFolder, false));
    }

    private Path createConsoleFolderWithRelease() throws IOException {
        final Path consoleFolder = Files.createDirectories(bundleRoot.resolve("console"));
        Files.write(consoleFolder.resolve("release.txt"), "platform=universal\nrelease=development\n".getBytes());
        return consoleFolder;
    }

    private static Path createFile(final Path path) throws IOException {
        Files.write(path, "launcher".getBytes());
        return path;
    }

    private static String normalized(final Path path) {
        return path.toAbsolutePath().normalize().toString();
    }
}
