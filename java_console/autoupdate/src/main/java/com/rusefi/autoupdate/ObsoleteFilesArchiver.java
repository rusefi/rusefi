package com.rusefi.autoupdate;

import com.devexperts.logging.Logging;
import com.rusefi.core.FindFileHelper;

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

    public void archiveObsoleteFiles() {
        Path bundleRoot = Paths.get(FindFileHelper.INPUT_FILES_PATH);
        Path archiveFolder = bundleRoot.resolve("archive");
        for (final Path fileToRemove : OBSOLETE_FILES_TO_DELETE) {
            try {
                Files.deleteIfExists(bundleRoot.resolve(fileToRemove));
            } catch (final IOException e) {
                log.error(String.format("Failed to delete `%s` file", fileToRemove), e);
            }
        }
        for (final String fileMaskToArchive : OBSOLETE_FILE_MASKS_TO_ARCHIVE) {
            try (final DirectoryStream<Path> dirStream = Files.newDirectoryStream(
                bundleRoot,
                fileMaskToArchive
            )) {
                Files.createDirectories(archiveFolder);
                dirStream.forEach(fileToArchive -> {
                    try {
                        Files.move(fileToArchive, archiveFolder.resolve(fileToArchive.getFileName()), REPLACE_EXISTING);
                    } catch (final IOException e) {
                        log.error(String.format(
                            "Failed to move `%s` file to `%s` folder",
                            fileToArchive,
                            archiveFolder
                        ), e);
                    }
                });
            } catch (final IOException e) {
                log.error(String.format("Failed to find files to archive by mask `%s`", fileMaskToArchive), e);
            }
        }
    }
}
