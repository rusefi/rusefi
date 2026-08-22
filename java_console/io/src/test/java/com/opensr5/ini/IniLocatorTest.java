package com.opensr5.ini;

import org.junit.jupiter.api.Test;
import org.junit.jupiter.api.io.TempDir;

import java.io.File;
import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Path;

import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertNull;
import static org.junit.jupiter.api.Assertions.assertTrue;

/**
 * Bundled-.ini fallback used when the signature-keyed download fails (custom board).
 * The old locator threw {@link IllegalStateException} on more than one {@code rusefi*.ini}, which broke
 * the fallback and left the board classified "Unknown"; these lock in that it (a) tolerates multiple
 * matches and (b) requires the signature-matching .ini when the caller knows the board signature.
 */
public class IniLocatorTest {
    private static final String SIG_A = "rusEFI master.2026.07.06.custom-fw.99386034";
    private static final String SIG_B = "rusEFI master.2026.07.06.other-board.11112222";

    private static File writeIni(Path dir, String name, String signature) throws IOException {
        File f = dir.resolve(name).toFile();
        Files.write(f.toPath(), ("[MegaTune]\n\tsignature\t= \"" + signature + "\"\n").getBytes());
        return f;
    }

    @Test
    public void singleMatchIsReturned(@TempDir Path dir) throws IOException {
        File ini = writeIni(dir, "rusefi_custom-fw.ini", SIG_A);
        assertEquals(ini.getAbsolutePath(), IniLocator.findIniFile(dir.toString()));
    }

    @Test
    public void multipleMatchesDoNotThrow(@TempDir Path dir) throws IOException {
        writeIni(dir, "rusefi_a.ini", SIG_A);
        writeIni(dir, "rusefi_b.ini", SIG_B);
        // no signature hint: must return *some* match rather than throwing
        String picked = IniLocator.findIniFile(dir.toString());
        assertTrue(picked != null && picked.endsWith(".ini"));
    }

    @Test
    public void signatureMatchWinsOverArbitraryPick(@TempDir Path dir) throws IOException {
        writeIni(dir, "rusefi_other.ini", SIG_B);
        File wanted = writeIni(dir, "rusefi_custom-fw.ini", SIG_A);
        assertEquals(wanted.getAbsolutePath(), IniLocator.findIniFile(dir.toString(), SIG_A));
    }

    @Test
    public void unmatchedSignatureReturnsNull(@TempDir Path dir) throws IOException {
        writeIni(dir, "rusefi_only.ini", SIG_B);
        assertNull(IniLocator.findIniFile(dir.toString(), SIG_A));
    }

    @Test
    public void noMatchIsNull(@TempDir Path dir) throws IOException {
        Files.write(dir.resolve("notrusefi.ini"), new byte[]{1});
        assertNull(IniLocator.findIniFile(dir.toString(), SIG_A));
    }

    @Test
    public void recursiveSearchFindsIniInSubfolder(@TempDir Path dir) throws IOException {
        Path nested = Files.createDirectories(dir.resolve("firmware/tunerstudio/generated"));
        File wanted = writeIni(nested, "rusefi_custom-fw.ini", SIG_A);
        assertEquals(wanted.getAbsolutePath(),
            IniLocator.findIniFileRecursively(dir.toString(), SIG_A, 3));
    }

    @Test
    public void recursiveSearchRespectsDepthLimit(@TempDir Path dir) throws IOException {
        Path tooDeep = Files.createDirectories(dir.resolve("a/b/c"));
        writeIni(tooDeep, "rusefi_deep.ini", SIG_A);
        assertNull(IniLocator.findIniFileRecursively(dir.toString(), SIG_A, 2));
    }

    @Test
    public void recursiveSearchSkipsHiddenFolders(@TempDir Path dir) throws IOException {
        Path hidden = Files.createDirectories(dir.resolve(".git/nested"));
        writeIni(hidden, "rusefi_hidden.ini", SIG_A);
        assertNull(IniLocator.findIniFileRecursively(dir.toString(), SIG_A, 3));
    }

    @Test
    public void findLocalIniFileFallsBackToNestedSubfolder(@TempDir Path dir) throws IOException {
        // Console launched from a folder whose ini sits one level down (an unzipped bundle dir):
        // the startup lookup must find it without a connection status rebuild.
        Path bundle = Files.createDirectories(dir.resolve("rusefi.snapshot.m74_9"));
        File wanted = writeIni(bundle, "rusefi_m74_9.ini", SIG_A);
        assertEquals(wanted.getAbsolutePath(),
            PrimeTunerStudioCache.findLocalIniFile(dir.toString(), dir.toString()));
    }

    @Test
    public void findLocalIniFilePrefersTopLevelMatch(@TempDir Path dir) throws IOException {
        File topLevel = writeIni(dir, "rusefi_top.ini", SIG_A);
        Path nested = Files.createDirectories(dir.resolve("nested/deeper"));
        writeIni(nested, "rusefi_nested.ini", SIG_B);
        assertEquals(topLevel.getAbsolutePath(),
            PrimeTunerStudioCache.findLocalIniFile(dir.toString(), dir.toString()));
    }
}
