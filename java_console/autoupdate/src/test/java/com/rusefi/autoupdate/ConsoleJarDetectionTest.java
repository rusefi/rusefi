package com.rusefi.autoupdate;

import org.junit.jupiter.api.Test;
import org.junit.jupiter.api.io.TempDir;

import java.io.File;
import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Path;
import java.util.zip.ZipEntry;
import java.util.zip.ZipOutputStream;

import static org.junit.jupiter.api.Assertions.assertArrayEquals;
import static org.junit.jupiter.api.Assertions.assertFalse;
import static org.junit.jupiter.api.Assertions.assertThrows;
import static org.junit.jupiter.api.Assertions.assertTrue;

class ConsoleJarDetectionTest {
    private final String consoleJar = Autoupdate.consoleJarFileName();
    @TempDir
    Path tempDir;

    @Test
    void nullMeansClassDirectoryNotConsoleJar() {
        assertFalse(Autoupdate.isConsoleJarFileName(null));
    }

    @Test
    void consoleJarIsDetectedRegardlessOfDirectory() {
        assertTrue(Autoupdate.isConsoleJarFileName(consoleJar));
        assertTrue(Autoupdate.isConsoleJarFileName(
            new File(new File("Program Files", "console"), consoleJar).getAbsolutePath()));
    }

    @Test
    void pendingConsoleJarCountsAsConsoleJar() {
        assertTrue(Autoupdate.isConsoleJarFileName(
            new File("console", Autoupdate.PENDING_CONSOLE_JAR).getPath()));
    }

    @Test
    void updaterJarIsNotConsoleJar() {
        assertFalse(Autoupdate.isConsoleJarFileName(new File("console", "rusefi_autoupdate.jar").getPath()));
        assertFalse(Autoupdate.isConsoleJarFileName(new File("console", "some_other.jar").getPath()));
    }

    @Test
    void stagesCompleteConsoleJarOverStalePending() throws IOException {
        byte[] oldContent = {1};
        byte[] newContent = {2, 3};
        Path pendingJar = tempDir.resolve(Autoupdate.PENDING_CONSOLE_JAR);
        Files.write(pendingJar, oldContent);
        Path updateZip = createZip("console/" + consoleJar, newContent);

        Autoupdate.stageConsoleJar(updateZip, pendingJar, "console/" + consoleJar);

        assertArrayEquals(newContent, Files.readAllBytes(pendingJar));
    }

    @Test
    void missingConsoleEntryFailsWithoutReplacingStalePending() throws IOException {
        byte[] oldContent = {1};
        Path pendingJar = tempDir.resolve(Autoupdate.PENDING_CONSOLE_JAR);
        Files.write(pendingJar, oldContent);
        Path updateZip = createZip("console/not-the-console.jar", new byte[] {2});

        assertThrows(IOException.class,
            () -> Autoupdate.stageConsoleJar(updateZip, pendingJar, "console/" + consoleJar));
        assertArrayEquals(oldContent, Files.readAllBytes(pendingJar));
    }

    private Path createZip(String entryName, byte[] content) throws IOException {
        Path zip = tempDir.resolve("update-" + entryName.hashCode() + ".zip");
        try (ZipOutputStream output = new ZipOutputStream(Files.newOutputStream(zip))) {
            output.putNextEntry(new ZipEntry(entryName));
            output.write(content);
        }
        return zip;
    }
}
