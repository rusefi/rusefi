package com.rusefi.autoupdate;

import org.junit.jupiter.api.Test;
import org.junit.jupiter.api.io.TempDir;

import java.io.File;
import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.StandardCopyOption;
import java.util.concurrent.atomic.AtomicBoolean;
import java.util.zip.ZipEntry;
import java.util.zip.ZipOutputStream;

import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertFalse;
import static org.junit.jupiter.api.Assertions.assertTrue;

class AutoupdateOutcomeTest {
    @Test
    void detectsOnlyAUsablePendingUpdate(@TempDir Path consoleDirectory) throws IOException {
        File directory = consoleDirectory.toFile();
        File pending = consoleDirectory.resolve(Autoupdate.PENDING_CONSOLE_JAR).toFile();

        assertFalse(Autoupdate.hasPendingUpdate(directory, "rusefi_console.jar"));
        assertTrue(pending.createNewFile());
        assertFalse(Autoupdate.hasPendingUpdate(directory, "rusefi_console.jar"));

        Files.write(pending.toPath(), new byte[] {1, 2, 3});
        assertFalse(Autoupdate.hasPendingUpdate(directory, "rusefi_console.jar"));

        try (ZipOutputStream zip = new ZipOutputStream(Files.newOutputStream(pending.toPath()))) {
            zip.putNextEntry(new ZipEntry("com/rusefi/Launcher.class"));
            zip.write(new byte[] {1, 2, 3});
            zip.closeEntry();
        }
        assertTrue(Autoupdate.hasPendingUpdate(directory, "rusefi_console.jar"));
        assertFalse(Autoupdate.hasPendingUpdate(directory, Autoupdate.PENDING_CONSOLE_JAR));
    }

    @Test
    void appliedPendingUpdateIsRemovedInsteadOfPromptingAgain(@TempDir Path consoleDirectory) throws IOException {
        File directory = consoleDirectory.toFile();
        Path pending = consoleDirectory.resolve(Autoupdate.PENDING_CONSOLE_JAR);
        Path installed = consoleDirectory.resolve("rusefi_console.jar");
        writeConsoleJar(pending, new byte[] {1, 2, 3});
        writeConsoleJar(installed, new byte[] {4, 5, 6});

        assertTrue(Autoupdate.hasPendingUpdate(directory, "rusefi_console.jar"),
            "a different staged JAR still waits for a restart");
        assertTrue(Files.exists(pending));

        // finalizePendingUpdate() copied the pending JAR but could not delete it (Windows).
        Files.copy(pending, installed, StandardCopyOption.REPLACE_EXISTING);

        assertFalse(Autoupdate.hasPendingUpdate(directory, "rusefi_console.jar"),
            "an identical staged JAR has already been applied");
        assertFalse(Files.exists(pending), "the applied leftover is cleaned up");
    }

    @Test
    void comparesFileContent(@TempDir Path directory) throws IOException {
        Path a = directory.resolve("a");
        Path b = directory.resolve("b");
        Files.write(a, new byte[] {1, 2, 3});
        Files.write(b, new byte[] {1, 2, 4});
        assertFalse(Autoupdate.hasSameContent(a.toFile(), b.toFile()));
        Files.write(b, new byte[] {1, 2});
        assertFalse(Autoupdate.hasSameContent(a.toFile(), b.toFile()));
        Files.write(b, new byte[] {1, 2, 3});
        assertTrue(Autoupdate.hasSameContent(a.toFile(), b.toFile()));
        assertFalse(Autoupdate.hasSameContent(a.toFile(), directory.resolve("missing").toFile()));
    }

    private static void writeConsoleJar(Path path, byte[] payload) throws IOException {
        try (ZipOutputStream zip = new ZipOutputStream(Files.newOutputStream(path))) {
            zip.putNextEntry(new ZipEntry("com/rusefi/Launcher.class"));
            zip.write(payload);
            zip.closeEntry();
        }
    }

    @Test
    void checkingAndInstallingAreSeparateOperations() {
        assertEquals(Autoupdate.UpdateOutcome.AVAILABLE,
            Autoupdate.checkForUpdate(null, ignored -> true));
        assertEquals(Autoupdate.UpdateOutcome.UP_TO_DATE,
            Autoupdate.checkForUpdate(null, ignored -> false));
        assertEquals(Autoupdate.UpdateOutcome.FAILED,
            Autoupdate.checkForUpdate(null, ignored -> {
                throw new IOException("check failed");
            }));
    }

    @Test
    void preservesNonDownloadOutcomes() {
        for (Autoupdate.UpdateOutcome outcome : new Autoupdate.UpdateOutcome[] {
            Autoupdate.UpdateOutcome.UP_TO_DATE,
            Autoupdate.UpdateOutcome.SKIPPED,
            Autoupdate.UpdateOutcome.FAILED
        }) {
            AtomicBoolean applied = new AtomicBoolean();
            assertEquals(outcome, Autoupdate.performUpdate(null,
                ignored -> Autoupdate.DownloadResult.of(outcome),
                ignored -> applied.set(true)));
            assertFalse(applied.get());
        }
    }

    @Test
    void reportsSuccessfulApply() {
        Autoupdate.DownloadedAutoupdateFileInfo file =
            new Autoupdate.DownloadedAutoupdateFileInfo("update.zip", 123);
        AtomicBoolean applied = new AtomicBoolean();

        assertEquals(Autoupdate.UpdateOutcome.UPDATED, Autoupdate.performUpdate(null,
            ignored -> Autoupdate.DownloadResult.updated(file),
            downloaded -> applied.set(downloaded == file)));
        assertTrue(applied.get());
    }

    @Test
    void reportsDownloadAndApplyFailures() {
        assertEquals(Autoupdate.UpdateOutcome.FAILED, Autoupdate.performUpdate(null,
            ignored -> {
                throw new IOException("download failed");
            },
            ignored -> {}));

        Autoupdate.DownloadedAutoupdateFileInfo file =
            new Autoupdate.DownloadedAutoupdateFileInfo("update.zip", 123);
        assertEquals(Autoupdate.UpdateOutcome.FAILED, Autoupdate.performUpdate(null,
            ignored -> Autoupdate.DownloadResult.updated(file),
            ignored -> {
                throw new IOException("apply failed");
            }));
    }
}
