package com.rusefi.pinout;

import org.junit.jupiter.api.Test;
import org.junit.jupiter.api.io.TempDir;

import java.io.IOException;
import java.io.Writer;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.attribute.FileTime;

import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertNotEquals;

class FileSystemBoardInputsImplTest {
    private static final String CONTENT = "generated output\n";

    @TempDir
    Path tempDir;

    @Test
    void identicalOutputDoesNotRewriteGeneratedFiles() throws IOException {
        Path connectors = Files.createDirectories(tempDir.resolve("board").resolve("connectors"));
        FileSystemBoardInputsImpl inputs = new FileSystemBoardInputsImpl(tempDir.resolve("board").toString());

        assertUnchangedOutputIsSkippedAndChangesAreWritten(
                connectors.resolve("generated_ts_name_by_pin.cpp"), inputs::getBoardNamesWriter);
        assertUnchangedOutputIsSkippedAndChangesAreWritten(
                connectors.resolve("generated_outputs.h"), inputs::getOutputsWriter);
        assertUnchangedOutputIsSkippedAndChangesAreWritten(
                connectors.resolve("generated_board_pin_names.h"), inputs::getBoardPinNamesWriter);
    }

    private static void assertUnchangedOutputIsSkippedAndChangesAreWritten(
            Path output, WriterFactory writerFactory) throws IOException {
        try (Writer writer = writerFactory.open()) {
            writer.write(CONTENT);
        }

        Files.setLastModifiedTime(output, FileTime.fromMillis(946684800000L));
        FileTime timestampBeforeRewrite = Files.getLastModifiedTime(output);

        try (Writer writer = writerFactory.open()) {
            writer.write(CONTENT);
        }

        assertEquals(timestampBeforeRewrite, Files.getLastModifiedTime(output), output.toString());
        assertEquals(CONTENT, Files.readString(output));

        Files.setLastModifiedTime(output, FileTime.fromMillis(946684800000L));
        FileTime timestampBeforeChange = Files.getLastModifiedTime(output);
        String changedContent = CONTENT + "changed\n";
        try (Writer writer = writerFactory.open()) {
            writer.write(changedContent);
        }
        assertEquals(changedContent, Files.readString(output));
        assertNotEquals(timestampBeforeChange, Files.getLastModifiedTime(output), output.toString());
    }

    @FunctionalInterface
    private interface WriterFactory {
        Writer open() throws IOException;
    }
}
