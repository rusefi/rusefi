package com.rusefi.core;

import org.jetbrains.annotations.Nullable;

import java.io.File;
import java.util.Objects;

public class FindFileHelper {
    private static final String BINARY_PREFIX = "rusefi";

    @Nullable
    public static String findFile(String fileDirectory, String prefix, String suffix) {
        File dir = new File(fileDirectory);
        if (!dir.isDirectory())
            return null;
        System.out.println("Searching for " + prefix + "*" + suffix + " in " + dir.getAbsolutePath());
        for (String file : Objects.requireNonNull(dir.list())) {
            if (file.contains(" "))
                continue; // spaces not acceptable
            if (file.startsWith(prefix) && file.endsWith(suffix))
                return fileDirectory + File.separator + file;
        }
        return null;
    }

    @Nullable
    public static String findSrecFile() {
        return findFile("..", BINARY_PREFIX, "srec");
    }

    public static boolean isObfuscated() {
        String srecFile = findSrecFile();
        return srecFile != null && srecFile.contains("obfuscated");
    }
}
