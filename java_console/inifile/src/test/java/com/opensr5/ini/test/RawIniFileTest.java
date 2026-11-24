package com.opensr5.ini.test;

import com.opensr5.ini.RawIniFile;
import org.junit.jupiter.api.Test;
import org.junit.jupiter.api.io.TempDir;

import java.io.FileNotFoundException;
import java.nio.charset.StandardCharsets;
import java.nio.file.Files;
import java.nio.file.Path;

import static org.junit.jupiter.api.Assertions.*;

class RawIniFileTest {
    @TempDir
    Path directory;

    @Test
    void readReleasesFileForDeletion() throws Exception {
        Path file = directory.resolve("signature.ini");
        Files.write(file, "signature = \"unit test\"\n".getBytes(StandardCharsets.UTF_8));

        RawIniFile ini = RawIniFile.read(file.toString());

        assertEquals("unit test", ini.getValues("signature").get(0));
        // Windows refuses deletion while the reader still owns an open file handle.
        Files.delete(file);
        assertFalse(Files.exists(file));
    }

    @Test
    void missingFileStillThrowsFileNotFoundException() {
        assertThrows(FileNotFoundException.class,
            () -> RawIniFile.read(directory.resolve("missing.ini").toString()));
    }
}
