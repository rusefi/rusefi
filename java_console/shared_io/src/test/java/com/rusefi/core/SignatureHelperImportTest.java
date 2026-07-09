package com.rusefi.core;

import org.junit.jupiter.api.Test;
import org.junit.jupiter.api.io.TempDir;

import java.io.File;
import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Path;

import static org.junit.jupiter.api.Assertions.*;

/**
 * Covers the manual "select .ini" cache import added for issue #9774: a picked .ini must land in the
 * cache keyed by the signature hash so subsequent connects resolve it without any download/prompt.
 */
public class SignatureHelperImportTest {
    private static final String SIGNATURE = "rusEFI master.2026.07.02.uaefi.2367417284";

    @Test
    public void importsPickedIniKeyedByHash(@TempDir Path cacheDir, @TempDir Path srcDir) throws IOException {
        File source = srcDir.resolve("picked.ini").toFile();
        Files.write(source.toPath(), "pageSize = 1234\n".getBytes());

        String cached = SignatureHelper.importIntoCache(SIGNATURE, source, cacheDir.toString());

        // hash is the last signature token; cached file is <hash>.ini
        assertEquals(cacheDir.resolve("2367417284.ini").toString(), cached);
        assertTrue(new File(cached).exists());
        assertArrayEquals(Files.readAllBytes(source.toPath()), Files.readAllBytes(Path.of(cached)));
    }

    @Test
    public void returnsNullForUnparseableSignature(@TempDir Path cacheDir, @TempDir Path srcDir) throws IOException {
        File source = srcDir.resolve("picked.ini").toFile();
        Files.write(source.toPath(), "x".getBytes());

        assertNull(SignatureHelper.importIntoCache("not a signature", source, cacheDir.toString()));
        assertEquals(0, cacheDir.toFile().list().length);
    }
}
