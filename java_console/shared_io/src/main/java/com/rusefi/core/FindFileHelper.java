package com.rusefi.core;

import com.devexperts.logging.Logging;
import org.jetbrains.annotations.Nullable;

import java.io.File;
import java.util.Objects;

public class FindFileHelper {
    private static final Logging log = Logging.getLogging(FindFileHelper.class);
    public static final String INPUT_FILES_PATH = System.getProperty("input_files_path", "..");
    /**
     * Same .bin used by primary DFU and a bit unneeded ST-LINK options
     */
    public static final String FIRMWARE_BIN_FILE = INPUT_FILES_PATH + "/" + "rusefi.bin";

    @Nullable
    public static String findFile(String fileDirectory, String prefix, String suffix) {
        File dir = new File(fileDirectory);
        if (!dir.isDirectory()) {
            throw new IllegalStateException("Not a directory: " + fileDirectory);
        }
        log.info("Searching for " + prefix + "*" + suffix + " in " + dir.getAbsolutePath());
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
        return findFile(INPUT_FILES_PATH, "", "srec");
    }

    public static boolean isObfuscated() {
        String srecFile = findSrecFile();
        return srecFile != null && srecFile.contains("obfuscated");
    }
}
