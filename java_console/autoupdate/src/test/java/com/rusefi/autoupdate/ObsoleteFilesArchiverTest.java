package com.rusefi.autoupdate;

import org.junit.jupiter.api.AfterEach;
import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.Test;

import java.io.*;
import java.nio.charset.StandardCharsets;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.util.List;
import java.util.stream.Collectors;
import java.util.stream.Stream;

import static org.junit.jupiter.api.Assertions.*;

class ObsoleteFilesArchiverTest {
    private static final List<String> TEST_OBSOLETE_FILES_TO_DELETE = List.of(
        "rusefi.bin",
        "rusefi_update.srec",
        "rusefi-obfuscated.srec"
    );

    private static final List<String> TEST_OBSOLETE_FILES_TO_ARCHIVE = List.of(
        "rusefi_lts-25jersey_1234567890_uaefi.bin",
        "rusefi_update_lts-25jersey_1234567890_uaefi.srec",
        "rusefi-obfuscated_lts-25jersey_1234567890_uaefi.srec"
    );

    private static final List<String> ALL_TEST_FILES = Stream.concat(
        TEST_OBSOLETE_FILES_TO_DELETE.stream(),
        TEST_OBSOLETE_FILES_TO_ARCHIVE.stream()
    ).collect(Collectors.toList());

    private static final Path ARCHIVE_FOLDER_PATH = Paths.get("archive");

    private static final List<Path> TEST_ARCHIVED_OBSOLETE_FILES = TEST_OBSOLETE_FILES_TO_ARCHIVE.stream().map(
        ARCHIVE_FOLDER_PATH::resolve
    ).collect(Collectors.toList());

    @BeforeEach
    void setUp() {
        cleanUpTestFiles();
        ALL_TEST_FILES.forEach(this::createTestFile);
    }

    @AfterEach
    void tearDown() {
        cleanUpTestFiles();
    }

    @Test
    void archiveObsoleteFilesWithMissingArchiveFolder() {
        assertFalse(Files.exists(ARCHIVE_FOLDER_PATH));
        doTest();
    }

    @Test
    void archiveObsoleteFilesWithExistingArchiveFolder() throws IOException {
        Files.createDirectories(ARCHIVE_FOLDER_PATH);
        assertTrue(Files.exists(ARCHIVE_FOLDER_PATH));

        doTest();
    }

    @Test
    void archiveObsoleteFilesWithOverwritingAlreadyExistingFiles() throws IOException {
        Files.createDirectories(ARCHIVE_FOLDER_PATH);
        assertTrue(Files.exists(ARCHIVE_FOLDER_PATH));

        TEST_ARCHIVED_OBSOLETE_FILES.forEach(filePath -> createTestFile(filePath.toString()));

        doTest();
    }

    private void cleanUpTestFiles() {
        ALL_TEST_FILES.stream().map(Paths::get).forEach(this::safeDelete);
        TEST_ARCHIVED_OBSOLETE_FILES.forEach(this::safeDelete);
        safeDelete(ARCHIVE_FOLDER_PATH);
    }

    private void doTest() {
        ObsoleteFilesArchiver.INSTANCE.archiveObsoleteFiles();

        ALL_TEST_FILES.forEach(fileName -> assertFalse(Files.exists(Paths.get(fileName)), fileName));
        TEST_ARCHIVED_OBSOLETE_FILES.forEach(filePath -> assertTrue(Files.exists(filePath), filePath.toString()));
        TEST_OBSOLETE_FILES_TO_ARCHIVE.forEach(fileToArchiveName -> {
            final Path testArchivedFile = ARCHIVE_FOLDER_PATH.resolve(fileToArchiveName);
            checkFileContent(testArchivedFile, fileToArchiveName);
        });
    }

    private void checkFileContent(final Path filePath, final String expectedContent) {
        try (final FileReader fileReader = new FileReader(filePath.toString());
            final BufferedReader bufferedReader = new BufferedReader(fileReader)
        ) {
            final String content = bufferedReader.readLine();
            assertEquals(expectedContent, content);
            assertNull(
                bufferedReader.readLine(),
                String.format("File `%s` is expecting to contain the single line", filePath)
            );
        } catch (final IOException e) {
            fail(String.format("Failed to read file `%s`: %s", filePath, e.getMessage()));
        }
    }

    private void safeDelete(final Path path) {
        try {
            Files.deleteIfExists(path);
        } catch (final IOException e) {
            fail(String.format("Failed to delete `%s`: %s", path, e.getMessage()));
        }
    }

    private void createTestFile(final String testFileName){
        final Path testFilePath = Paths.get(testFileName);
        assertDoesNotThrow(() -> {
            try (final Writer fileWriter = new BufferedWriter(new OutputStreamWriter(
                Files.newOutputStream(testFilePath),
                StandardCharsets.UTF_8
            ))) {
                fileWriter.write(testFileName);
            }
        });
        assertTrue(Files.exists(testFilePath));
        checkFileContent(testFilePath, testFileName);
    }
}
