package com.rusefi.core;

import org.junit.jupiter.api.Test;
import org.junit.jupiter.api.io.TempDir;

import java.io.File;
import java.io.IOException;
import java.nio.charset.StandardCharsets;
import java.nio.file.Files;
import java.nio.file.Path;
import java.util.stream.Collectors;
import java.util.stream.IntStream;

import static org.junit.jupiter.api.Assertions.*;

/**
 * Covers the stale-cache drop for the local ini_database: a cached .ini is keyed by the config
 * hash, so a hit implies the hash matches - but a rebuild on a later day regenerates the
 * signature date, and the cached .ini with yesterday's date must not be served to the console.
 */
public class SignatureHelperCacheStalenessTest {
    private static final String SIGNATURE = "rusEFI maccan-tx-fix.2026.08.18.m74_9.319381849";
    private static final String NEXT_DAY_SIGNATURE = "rusEFI maccan-tx-fix.2026.08.21.m74_9.319381849";

    /** Builds a >10 KB .ini whose first lines carry the given embedded signature. */
    private static File writeCachedIni(Path cacheDir, String fileName, String embeddedSignature) throws IOException {
        File iniFile = cacheDir.resolve(fileName).toFile();
        StringBuilder sb = new StringBuilder();
        sb.append("; test ini\n");
        sb.append("signature= \"").append(embeddedSignature).append("\" ; comment\n");
        // pad past the 10 KB size guard used by downloadIfNotAvailable
        sb.append(IntStream.range(0, 11000).mapToObj(i -> "x").collect(Collectors.joining()));
        Files.write(iniFile.toPath(), sb.toString().getBytes(StandardCharsets.UTF_8));
        return iniFile;
    }

    @Test
    public void readIniSignatureExtractsEmbeddedSignature(@TempDir Path dir) throws IOException {
        File ini = writeCachedIni(dir, "319381849.ini", SIGNATURE);
        assertEquals(SIGNATURE, SignatureHelper.readIniSignature(ini));
    }

    @Test
    public void divergedDateIsStale(@TempDir Path dir) throws IOException {
        File ini = writeCachedIni(dir, "319381849.ini", SIGNATURE);
        assertTrue(SignatureHelper.isStaleCachedIni(ini, NEXT_DAY_SIGNATURE));
    }

    /** A different embedded hash is a deliberate manual-picker import - keep it, never drop. */
    @Test
    public void differentHashIsNotStale(@TempDir Path dir) throws IOException {
        File ini = writeCachedIni(dir, "319381849.ini", SIGNATURE);
        String differentHashSignature = "rusEFI maccan-tx-fix.2026.08.18.m74_9.9999999999";
        assertFalse(SignatureHelper.isStaleCachedIni(ini, differentHashSignature));
    }

    @Test
    public void matchingSignatureIsNotStale(@TempDir Path dir) throws IOException {
        File ini = writeCachedIni(dir, "319381849.ini", SIGNATURE);
        assertFalse(SignatureHelper.isStaleCachedIni(ini, SIGNATURE));
    }

    @Test
    public void missingSignatureLineIsStale(@TempDir Path dir) throws IOException {
        File ini = dir.resolve("319381849.ini").toFile();
        Files.write(ini.toPath(), IntStream.range(0, 11000)
            .mapToObj(i -> "y").collect(Collectors.joining()).getBytes(StandardCharsets.UTF_8));
        assertTrue(SignatureHelper.isStaleCachedIni(ini, SIGNATURE));
    }

    @Test
    public void unreadableFileIsStale(@TempDir Path dir) {
        File ini = new File(dir.toFile(), "missing.ini");
        assertTrue(SignatureHelper.isStaleCachedIni(ini, SIGNATURE));
    }
}
