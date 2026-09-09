package com.opensr5.ini;

import org.junit.jupiter.api.Test;
import org.junit.jupiter.api.io.TempDir;

import java.io.File;
import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Path;

import static org.junit.jupiter.api.Assertions.*;

/**
 * When rusefi.com has no .ini for a signature (custom board, local build - HTTP 404), the console
 * falls back to TunerStudio's own ecuDef cache, where every definition TS has ever loaded is stored
 * as {@code <signature without spaces>.ini}.
 */
public class PrimeTunerStudioCacheTest {
    private static final String SIGNATURE = "rusEFI master.2026.08.29.purple-gateway.1662610921";

    private static String folder(Path dir) {
        return dir.toString() + File.separator;
    }

    private static File writeIni(Path dir, String fileName, String signature) throws IOException {
        File file = dir.resolve(fileName).toFile();
        Files.write(file.toPath(), ("[MegaTune]\n   signature      = \"" + signature + "\"\n").getBytes());
        return file;
    }

    @Test
    public void findsIniNamedAfterSignatureWithoutSpaces(@TempDir Path ecuDef) throws IOException {
        File expected = writeIni(ecuDef, "rusEFImaster.2026.08.29.purple-gateway.1662610921.ini", SIGNATURE);

        assertEquals(expected.getAbsolutePath(), PrimeTunerStudioCache.findInTunerStudioCache(SIGNATURE, folder(ecuDef)));
    }

    @Test
    public void returnsNullWhenTunerStudioHasNotSeenSignature(@TempDir Path ecuDef) {
        assertNull(PrimeTunerStudioCache.findInTunerStudioCache(SIGNATURE, folder(ecuDef)));
    }

    @Test
    public void returnsNullWhenFolderDoesNotExist(@TempDir Path ecuDef) {
        assertNull(PrimeTunerStudioCache.findInTunerStudioCache(SIGNATURE, folder(ecuDef.resolve("missing"))));
    }

    /** A file with the right name but a different embedded signature must not be used. */
    @Test
    public void rejectsFileWithMismatchingSignature(@TempDir Path ecuDef) throws IOException {
        writeIni(ecuDef, "rusEFImaster.2026.08.29.purple-gateway.1662610921.ini",
            "rusEFI master.2026.08.28.purple-gateway.1111111111");

        assertNull(PrimeTunerStudioCache.findInTunerStudioCache(SIGNATURE, folder(ecuDef)));
    }

    @Test
    public void nullSignatureIsNotFound(@TempDir Path ecuDef) {
        assertNull(PrimeTunerStudioCache.findInTunerStudioCache(null, folder(ecuDef)));
    }
}
