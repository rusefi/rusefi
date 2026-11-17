package com.rusefi.logview;

import java.io.BufferedInputStream;
import java.io.File;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.nio.charset.StandardCharsets;
import java.nio.file.Files;
import java.util.Locale;

/** Reads a data log in either supported format, chosen by file extension. */
public final class LogFiles {
    /** Extensions the log viewer opens, without the dot. */
    public static final String[] EXTENSIONS = {"mlg", "msl"};

    private LogFiles() {
    }

    public static boolean isSupported(String fileName) {
        String lower = fileName.toLowerCase(Locale.ROOT);
        for (String extension : EXTENSIONS) {
            if (lower.endsWith("." + extension)) {
                return true;
            }
        }
        return false;
    }

    /** Binary MLG for {@code .mlg}, tab-separated text (MSL) otherwise. */
    public static CsvLogReader read(InputStream stream, String fileName) throws IOException {
        return fileName.toLowerCase(Locale.ROOT).endsWith(".mlg")
                ? BinaryLogReader.read(stream)
                : CsvLogReader.read(new InputStreamReader(stream, StandardCharsets.ISO_8859_1));
    }

    public static CsvLogReader read(File file) throws IOException {
        try (InputStream stream = new BufferedInputStream(Files.newInputStream(file.toPath()))) {
            return read(stream, file.getName());
        }
    }
}
