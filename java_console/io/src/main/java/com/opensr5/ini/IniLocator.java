package com.opensr5.ini;

import com.rusefi.ini.reader.IniFileReader;
import org.jetbrains.annotations.Nullable;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileReader;
import java.io.IOException;
import java.util.ArrayList;
import java.util.List;

/**
 * Locates a bundled {@code rusefi*.ini} on disk. This is the fallback the console uses when the
 * signature-keyed download fails — e.g. a custom board whose .ini is not on the server.
 * <p>
 * A bundle dir can legitimately contain more than one {@code rusefi*.ini}, so this never throws on
 * extra matches (the old behaviour bricked the fallback and left the board classified "Unknown").
 * When the caller knows the connected board's signature, the matching .ini is preferred over an
 * arbitrary one — the .ini analogue of the firmware brick-guard in {@link com.rusefi.core.FindFileHelper}.
 */
public class IniLocator {
    /** Signature-agnostic lookup: first {@code rusefi*.ini} in the directory, or null. */
    public static @Nullable String findIniFile(String iniFilePath) {
        return findIniFile(iniFilePath, null);
    }

    /**
     * Find a {@code rusefi*.ini} in {@code iniFilePath}. When {@code signature} is non-null and several
     * candidates exist, prefer the one whose embedded signature matches; otherwise return the first
     * match. Never throws on multiple matches.
     */
    public static @Nullable String findIniFile(String iniFilePath, @Nullable String signature) {
        final List<File> matches = listIniFiles(iniFilePath);
        if (matches.isEmpty()) {
            return null;
        }
        if (signature != null) {
            for (File f : matches) {
                if (signature.equals(readIniSignature(f))) {
                    return f.getAbsolutePath();
                }
            }
        }
        return matches.get(0).getAbsolutePath();
    }

    private static List<File> listIniFiles(String iniFilePath) {
        final List<File> result = new ArrayList<>();
        final File[] files = new File(iniFilePath).listFiles((dir, name) ->
            name.startsWith(IniFileReader.RUSEFI_INI_PREFIX)
                && name.endsWith(IniFileReader.INI_FILE_SUFFIX)
                && !name.contains(" ")); // spaces not acceptable, matching FindFileHelper
        if (files != null) {
            for (File f : files) {
                if (f.isFile()) {
                    result.add(f);
                }
            }
        }
        return result;
    }

    /**
     * Extract the {@code signature = "rusEFI ..."} value declared near the top of a TunerStudio .ini,
     * without a full parse. Returns null if the file is unreadable or has no signature line.
     */
    @Nullable
    static String readIniSignature(File iniFile) {
        try (BufferedReader reader = new BufferedReader(new FileReader(iniFile))) {
            String line;
            int scanned = 0;
            while ((line = reader.readLine()) != null && scanned++ < 200) {
                final String trimmed = line.trim();
                if (trimmed.startsWith("signature")) {
                    final int first = trimmed.indexOf('"');
                    final int last = trimmed.lastIndexOf('"');
                    if (first >= 0 && last > first) {
                        return trimmed.substring(first + 1, last);
                    }
                }
            }
        } catch (IOException e) {
            // unreadable candidate — treat as "no signature", caller falls back to first match
        }
        return null;
    }
}
