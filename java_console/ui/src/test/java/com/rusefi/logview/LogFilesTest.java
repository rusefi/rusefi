package com.rusefi.logview;

import org.junit.jupiter.api.Test;
import org.junit.jupiter.api.io.TempDir;

import java.io.File;
import java.io.IOException;
import java.io.InputStream;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.StandardCopyOption;

import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertFalse;
import static org.junit.jupiter.api.Assertions.assertThrows;
import static org.junit.jupiter.api.Assertions.assertTrue;

class LogFilesTest {
    @TempDir
    Path folder;

    private File copy(String resource, String name) throws IOException {
        Path target = folder.resolve(name);
        try (InputStream stream = getClass().getResourceAsStream(resource)) {
            Files.copy(stream, target, StandardCopyOption.REPLACE_EXISTING);
        }
        return target.toFile();
    }

    @Test
    void readsBothFormatsFromDisk() throws IOException {
        assertEquals(220, LogFiles.read(copy("/logview/test-log-binary-1.mlg", "drive.mlg")).sampleCount());
        assertTrue(LogFiles.read(copy("/logview/test-log-1.msl", "drive.msl")).sampleCount() > 3000);
    }

    @Test
    void extensionPicksTheFormat() throws IOException {
        // binary content named .msl is not a text log
        File misnamed = copy("/logview/test-log-binary-1.mlg", "binary.msl");
        assertThrows(IOException.class, () -> LogFiles.read(misnamed));
    }

    @Test
    void rejectsTruncatedBinaryLog() throws IOException {
        File file = folder.resolve("short.mlg").toFile();
        Files.write(file.toPath(), new byte[]{'M', 'L', 'V', 'L', 'G'});
        assertThrows(IOException.class, () -> LogFiles.read(file));
    }

    @Test
    void supportedExtensions() {
        assertTrue(LogFiles.isSupported("a.mlg"));
        assertTrue(LogFiles.isSupported("B.MSL"));
        assertFalse(LogFiles.isSupported("tune.msq"));
        assertFalse(LogFiles.isSupported("mlg"));
    }
}
