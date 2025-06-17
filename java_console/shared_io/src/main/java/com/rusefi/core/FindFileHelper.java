package com.rusefi.core;

import com.devexperts.logging.Logging;
import org.jetbrains.annotations.Nullable;

import java.io.File;
import java.util.Optional;
import java.util.function.Supplier;
import java.util.stream.Stream;
import java.util.Arrays;

public class FindFileHelper {
    private static final Logging log = Logging.getLogging(FindFileHelper.class);
    public static final String INPUT_FILES_PATH = System.getProperty("input_files_path", "..");
    /**
     * Same .bin used by primary DFU and a bit unneeded ST-LINK options
     */
    public static final String FIRMWARE_BIN_FILE = INPUT_FILES_PATH + "/" + "rusefi.bin";
    private static final String PREFIX = "";
    private static final String SUFFIX = "srec";

    @Nullable
    public static String findFile(String fileDirectory, String prefix, String suffix) {
        File dir = new File(fileDirectory);
        log.info("Scanning " + dir.getAbsolutePath() + " for " + prefix + "/" + suffix);
        if (!dir.isDirectory()) {
            throw new IllegalStateException("Not a directory: " + fileDirectory);
        }

        log.info("Searching for " + prefix + "*" + suffix + " in " + dir.getAbsolutePath());

        return findOneFile(fileDirectory, prefix, suffix, dir.list());
    }

    @Nullable
	public static String findOneFile(String fileDirectory, String prefix, String suffix, String[] directoryListing) {
        Optional<String[]> fileList = Optional.ofNullable(directoryListing);

        Supplier<Stream<String>> search_file = () -> Arrays.stream(fileList.get())
            .filter(file -> !file.contains(" "))  // spaces not acceptable
            .filter(file -> file.startsWith(prefix) && file.endsWith(suffix));

        long matchCount = search_file.get().count();

        if (matchCount > 1) {
            throw new IllegalStateException("More than one file match the search pattern: " + matchCount);
        }

        Optional<String> file = search_file.get().findFirst();

        if (file.isPresent()) {
            return fileDirectory + File.separator + file.get();
        }

        return null;
    }

    @Nullable
    public static String findSrecFile() {
        String fileAtFirstLocation = findFile(INPUT_FILES_PATH, PREFIX, SUFFIX);
        if (fileAtFirstLocation == null) {
            log.info("Second choice: current folder");
            return findFile(".", PREFIX, SUFFIX);
        }

        return fileAtFirstLocation;
    }

    public static String findFirmwareFile() {
        return FIRMWARE_BIN_FILE;
    }

    public static boolean isObfuscated() {
        String srecFile = findSrecFile();
        return srecFile != null && srecFile.contains("obfuscated");
    }
}
