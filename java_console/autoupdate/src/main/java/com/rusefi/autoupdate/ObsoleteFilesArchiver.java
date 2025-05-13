package com.rusefi.autoupdate;

import com.devexperts.logging.Logging;

import java.io.IOException;
import java.nio.file.*;
import java.util.Collections;
import java.util.List;
import java.util.stream.Collectors;
import java.util.stream.Stream;

import static java.nio.file.StandardCopyOption.REPLACE_EXISTING;

public enum ObsoleteFilesArchiver {
    INSTANCE;

    private final static Logging log = Logging.getLogging(ObsoleteFilesArchiver.class);
    private static final List<Path> OBSOLETE_FILES_TO_DELETE = Stream.of(
        "rusefi.bin",
        "rusefi_update.srec",
        "rusefi-obfuscated.srec"
    ).map(Paths::get).collect(Collectors.collectingAndThen(Collectors.toList(), Collections::unmodifiableList));

    private static final List<String> OBSOLETE_FILE_MASKS_TO_ARCHIVE = Stream.of(
        "rusefi_*.bin",
        "rusefi_*.srec",
        "rusefi-obfuscated_*.srec"
    ).collect(Collectors.collectingAndThen(Collectors.toList(), Collections::unmodifiableList));

    private static final Path ARCHIVE_FOLDER_PATH = Paths.get("./archive");

    public void archiveObsoleteFiles() {
        for (final Path fileToRemove : OBSOLETE_FILES_TO_DELETE) {
            try {
                Files.deleteIfExists(fileToRemove);
            } catch (final IOException e) {
                log.error(String.format("Failed to delete `%s` file", fileToRemove), e);
            }
        }
        for (final String fileMaskToArchive : OBSOLETE_FILE_MASKS_TO_ARCHIVE) {
            try (final DirectoryStream<Path> dirStream = Files.newDirectoryStream(
                Paths.get("."),
                fileMaskToArchive
            )) {
                Files.createDirectories(ARCHIVE_FOLDER_PATH);
                dirStream.forEach(fileToArchive -> {
                    try {
                        Files.move(fileToArchive, ARCHIVE_FOLDER_PATH.resolve(fileToArchive), REPLACE_EXISTING);
                    } catch (final IOException e) {
                        log.error(String.format(
                            "Failed to move `%s` file to `%s` folder",
                            fileToArchive,
                            ARCHIVE_FOLDER_PATH
                        ), e);
                    }
                });
            } catch (final IOException e) {
                log.error(String.format("Failed to find files to archive by mask `%s`", fileMaskToArchive), e);
            }
        }
    }
}
