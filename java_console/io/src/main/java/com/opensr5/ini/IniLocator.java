package com.opensr5.ini;

import com.rusefi.core.SignatureHelper;
import com.rusefi.ini.reader.IniFileReader;
import org.jetbrains.annotations.Nullable;

import java.io.File;
import java.io.IOException;
import java.nio.file.DirectoryStream;
import java.nio.file.Files;
import java.nio.file.Path;
import java.util.ArrayList;
import java.util.List;
import java.util.function.Consumer;

/**
 * Locates a bundled {@code rusefi*.ini} on disk. This is the fallback the console uses when the
 * signature-keyed download fails — e.g. a custom board whose .ini is not on the server.
 * <p>
 * A bundle dir can legitimately contain more than one {@code rusefi*.ini}, so this never throws on
 * extra matches (the old behaviour bricked the fallback and left the board classified "Unknown").
 * When the caller knows the connected board's signature, only a matching .ini is returned — the
 * .ini analogue of the firmware brick-guard in {@link com.rusefi.core.FindFileHelper}.
 */
public class IniLocator {
    private static final int MAX_ROOT_ENTRIES_TO_CHECK = 4096;

    public enum LookupStage {
        BUNDLED_INI,
        CURRENT_DIRECTORY,
        DRIVE_ROOTS,
        CONSOLE_CACHE_DOWNLOAD,
        TUNERSTUDIO_CACHE,
        COMPLETE
    }

    /** Immutable progress snapshot for one automatic INI lookup. */
    public static final class LookupProgress {
        public final LookupStage stage;
        public final int completedRoots;
        public final int totalRoots;
        public final int activeWorkers;
        public final long sequence;

        public LookupProgress(LookupStage stage, int completedRoots, int totalRoots,
                              int activeWorkers, long sequence) {
            this.stage = stage;
            this.completedRoots = completedRoots;
            this.totalRoots = totalRoots;
            this.activeWorkers = activeWorkers;
            this.sequence = sequence;
        }

        public static LookupProgress stage(LookupStage stage) {
            return new LookupProgress(stage, 0, stage == LookupStage.DRIVE_ROOTS ? -1 : 0, 0, 0);
        }
    }

    public static final class RootSearchResult {
        public enum Status {
            UNSUPPORTED,
            FOUND,
            UNAVAILABLE,
            CANCELLED
        }

        private final Status status;
        private final byte[] ini;

        private RootSearchResult(Status status, byte[] ini) {
            this.status = status;
            this.ini = ini;
        }

        public static RootSearchResult unsupported() {
            return new RootSearchResult(Status.UNSUPPORTED, null);
        }

        public static RootSearchResult found(byte[] ini) {
            return new RootSearchResult(Status.FOUND, ini);
        }

        public static RootSearchResult unavailable() {
            return new RootSearchResult(Status.UNAVAILABLE, null);
        }

        public static RootSearchResult cancelled() {
            return new RootSearchResult(Status.CANCELLED, null);
        }

        public Status getStatus() {
            return status;
        }

        public byte[] getIni() {
            return ini;
        }
    }

    /**
     * Windows drive-root fallback used after ordinary local and cache lookups fail.
     * Returns an owned, exact-signature snapshot so callers never reopen a drive path.
     */
    public static RootSearchResult findMatchingIniOnWindows(String signature) {
        return findMatchingIniOnWindows(signature, ignored -> {});
    }

    public static RootSearchResult findMatchingIniOnWindows(
            String signature, Consumer<LookupProgress> progress) {
        if (!isWindowsRootSearchSupported()) {
            return RootSearchResult.unsupported();
        }
        return IniRootSearch.findOnWindows(signature, progress);
    }

    public static boolean isWindowsRootSearchSupported() {
        return System.getProperty("os.name", "").toLowerCase(java.util.Locale.ROOT).contains("windows");
    }

    /** Signature-agnostic lookup: first {@code rusefi*.ini} in the directory, or null. */
    public static @Nullable String findIniFile(String iniFilePath) {
        return findIniFile(iniFilePath, null);
    }

    /**
     * Find a {@code rusefi*.ini} in {@code iniFilePath}. When {@code signature} is non-null, only a file
     * with that embedded signature is returned. Never throws on multiple matches.
     */
    public static @Nullable String findIniFile(String iniFilePath, @Nullable String signature) {
        final List<File> matches = listIniFiles(iniFilePath);
        if (matches.isEmpty()) {
            return null;
        }
        if (signature == null) {
            return matches.get(0).getAbsolutePath();
        }
        for (File f : matches) {
            if (signature.equals(readIniSignature(f))) {
                return f.getAbsolutePath();
            }
        }
        return null;
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
     * Searches only immediate children of a drive root. The entry cap prevents unusually large
     * root directories from making a worker scan unbounded; the caller separately bounds waiting.
     */
    static byte[] findMatchingIniInRoot(File root, String signature) {
        int scanned = 0;
        try (DirectoryStream<Path> entries = Files.newDirectoryStream(root.toPath())) {
            for (Path path : entries) {
                if (Thread.currentThread().isInterrupted()) {
                    return null;
                }
                if (scanned++ >= MAX_ROOT_ENTRIES_TO_CHECK) {
                    return null;
                }
                String name = path.getFileName().toString();
                if (!Files.isRegularFile(path)) {
                    continue;
                }
                String lowerName = name.toLowerCase(java.util.Locale.ROOT);
                if (lowerName.endsWith(IniFileReader.INI_FILE_SUFFIX) || lowerName.endsWith(".7z")) {
                    try {
                        return SignatureHelper.readMatchingIni(signature, path.toFile());
                    } catch (IOException e) {
                        // An unreadable, malformed, or mismatched root file is not a match.
                    }
                }
            }
        } catch (IOException | SecurityException e) {
            // Removable and network roots may disappear or deny access during discovery.
        }
        return null;
    }

    /**
     * Extract the first exact INI {@code signature} key without loading the full model.
     * Returns null if the file is unreadable or has no signature key.
     */
    @Nullable
    static String readIniSignature(File iniFile) {
        try {
            return SignatureHelper.readIniSignature(iniFile);
        } catch (IOException e) {
            // unreadable candidate — treat as "no signature"
        }
        return null;
    }
}
