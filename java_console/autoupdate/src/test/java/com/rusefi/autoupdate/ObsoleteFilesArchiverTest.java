package com.rusefi.autoupdate;

import com.rusefi.core.FindFileHelper;
import org.junit.jupiter.api.AfterEach;
import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.Test;
import org.junit.jupiter.api.io.TempDir;

import java.io.*;
import java.nio.charset.StandardCharsets;
import java.nio.file.Files;
import java.nio.file.Path;
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

    @TempDir
    Path bundleRoot;

    private String savedInputFilesPath;
    private Path archiveFolderPath;
    private List<Path> testArchivedObsoleteFiles;

    @BeforeEach
    void setUp() {
        savedInputFilesPath = FindFileHelper.INPUT_FILES_PATH;
        FindFileHelper.INPUT_FILES_PATH = bundleRoot.toString();
        archiveFolderPath = bundleRoot.resolve("archive");
        testArchivedObsoleteFiles = TEST_OBSOLETE_FILES_TO_ARCHIVE.stream()
            .map(archiveFolderPath::resolve)
            .collect(Collectors.toList());
        ALL_TEST_FILES.forEach(fileName -> createTestFile(bundleRoot.resolve(fileName), fileName));
    }

    @AfterEach
    void tearDown() {
        FindFileHelper.INPUT_FILES_PATH = savedInputFilesPath;
    }

    @Test
    void archiveObsoleteFilesWithMissingArchiveFolder() {
        assertFalse(Files.exists(archiveFolderPath));
        doTest();
    }

    @Test
    void archiveObsoleteFilesWithExistingArchiveFolder() throws IOException {
        Files.createDirectories(archiveFolderPath);
        assertTrue(Files.exists(archiveFolderPath));

        doTest();
    }

    @Test
    void archiveObsoleteFilesWithOverwritingAlreadyExistingFiles() throws IOException {
        Files.createDirectories(archiveFolderPath);
        assertTrue(Files.exists(archiveFolderPath));

        testArchivedObsoleteFiles.forEach(filePath -> createTestFile(filePath, filePath.getFileName().toString()));

        doTest();
    }

    private void doTest() {
        ObsoleteFilesArchiver.INSTANCE.archiveObsoleteFiles();

        ALL_TEST_FILES.forEach(fileName -> assertFalse(Files.exists(bundleRoot.resolve(fileName)), fileName));
        testArchivedObsoleteFiles.forEach(filePath -> assertTrue(Files.exists(filePath), filePath.toString()));
        TEST_OBSOLETE_FILES_TO_ARCHIVE.forEach(fileToArchiveName -> {
            final Path testArchivedFile = archiveFolderPath.resolve(fileToArchiveName);
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

    private void createTestFile(final Path testFilePath, final String content) {
        assertDoesNotThrow(() -> {
            try (final Writer fileWriter = new BufferedWriter(new OutputStreamWriter(
                Files.newOutputStream(testFilePath),
                StandardCharsets.UTF_8
            ))) {
                fileWriter.write(content);
            }
        });
        assertTrue(Files.exists(testFilePath));
        checkFileContent(testFilePath, content);
    }
}
