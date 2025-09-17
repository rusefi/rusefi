package com.rusefi.core;

import com.devexperts.logging.Logging;
import org.jetbrains.annotations.Nullable;

import java.io.File;
import java.io.IOException;
import java.nio.file.*;
import java.util.List;
import java.util.Optional;
import java.util.function.Supplier;
import java.util.stream.Collectors;
import java.util.stream.Stream;
import java.util.Arrays;

import static com.rusefi.core.FileUtil.RUSEFI_SETTINGS_FOLDER;

public class FindFileHelper {
    private static final Logging log = Logging.getLogging(FindFileHelper.class);
    public static String INPUT_FILES_PATH = System.getProperty("input_files_path", "..");
    /**
     * Same .bin used by primary DFU and a bit unneeded ST-LINK options
     */
    public static final String FIRMWARE_BIN_FILE = INPUT_FILES_PATH + "/" + "rusefi.bin";
    private static final String PREFIX = "";
    private static final String SUFFIX = "srec";

    @Nullable
    public static String findFile(String fileDirectory, String prefix, String suffix, AdditionalFileHandler fileMoveOperation, boolean keepOneFile) {
        File dir = new File(fileDirectory);
        log.info("Scanning " + dir.getAbsolutePath() + " for " + prefix + "/" + suffix);
        if (!dir.isDirectory()) {
            throw new IllegalStateException("Not a directory: " + fileDirectory);
        }

        log.info("Searching for " + prefix + "*" + suffix + " in " + dir.getAbsolutePath());

        return findXNumberOfFile(fileDirectory, prefix, suffix, dir.list(), fileMoveOperation, keepOneFile);
    }

    public interface AdditionalFileHandler {
        void onAdditionalFile(String fileDirectory, String fileName);
    }

    @Nullable
    public static String findXNumberOfFile(String fileDirectory, String prefix, String suffix, String[] directoryListing, AdditionalFileHandler fileMoveOperation, boolean keepOneFile) {
        Optional<String[]> fileList = Optional.ofNullable(directoryListing);

        Supplier<Stream<String>> search_file = () -> Arrays.stream(fileList.get()).filter(file -> !file.contains(" "))  // spaces not acceptable
            .filter(file -> file.startsWith(prefix) && file.endsWith(suffix));

        List<String> files = search_file.get().collect(Collectors.toList());
        if (files.isEmpty())
            return null;


        for (int index = keepOneFile ? 1 : 0; index < files.size(); index++) {
            fileMoveOperation.onAdditionalFile(fileDirectory, files.get(index));
        }
        if (!keepOneFile)
            return null;

        String file = files.get(0);

        String result = fileDirectory + File.separator + file;
        log.info("Located " + result);
        return result;
    }

    @Nullable
    public static String findSrecFile() {
        return findSrecFile(true);
    }

    @Nullable
    public static String findSrecFile(boolean keepOneFile) {
        String fileAtFirstLocation = findFile(INPUT_FILES_PATH, PREFIX, SUFFIX, new AdditionalFileHandler() {
            @Override
            public void onAdditionalFile(String fileDirectory, String fileName) {
                moveFile(fileDirectory, fileName);
            }
        }, keepOneFile);
        if (fileAtFirstLocation == null) {
            // todo: what is this second location about?!
            log.info("Second choice: current folder");
            return findFile(".", PREFIX, SUFFIX, new AdditionalFileHandler() {
                @Override
                public void onAdditionalFile(String fileDirectory, String fileName) {
                    moveFile(fileDirectory, fileName);
                }
            }, keepOneFile);
        }

        return fileAtFirstLocation;
    }

    private static void moveFile(String fileDirectory, String fileName) {
        String destinationFolder = RUSEFI_SETTINGS_FOLDER + "older-fw" + File.separator;
        boolean wasCreated = new File(destinationFolder).mkdirs();
        log.info(destinationFolder + ": created: " + wasCreated);
        String fullSourceFileName = fileDirectory + File.separator + fileName;
        Path sourcePath = Paths.get(fullSourceFileName);
        Path destinationDirectory = Paths.get(destinationFolder + fileName);
        try {
            Files.move(sourcePath, destinationDirectory);
            log.info("Moved " + sourcePath + " to " + destinationFolder);
        } catch (IOException e) {
            // this could happen when we have too many plainly named older files like rusefi.srec for many different boards
            log.warn(sourcePath + "Move failed, will try to remove: " + e);
            boolean deletedOk = new File(fullSourceFileName).delete();
            log.info(fullSourceFileName + ": deleted: " + deletedOk);
        }
    }

    public static String findFirmwareFile() {
        return FIRMWARE_BIN_FILE;
    }

    public static boolean isObfuscated() {
        String srecFile = findSrecFile();
        return srecFile != null && srecFile.contains("obfuscated");
    }
}
