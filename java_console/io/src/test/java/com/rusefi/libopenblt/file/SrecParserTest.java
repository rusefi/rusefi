package com.rusefi.libopenblt.file;

import org.junit.jupiter.api.Test;
import org.junit.jupiter.api.io.TempDir;

import java.io.IOException;
import java.nio.charset.StandardCharsets;
import java.nio.file.Files;
import java.nio.file.Path;
import java.util.List;

import static org.junit.jupiter.api.Assertions.assertArrayEquals;
import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertThrows;
import static org.junit.jupiter.api.Assertions.assertTrue;

class SrecParserTest {
    @TempDir
    Path tempDir;

    @Test
    void validatesAndMergesAdjacentDataRecords() throws IOException {
        Path file = write(
            record('0', 0, 2, "header".getBytes(StandardCharsets.US_ASCII)),
            record('3', 0x08008000L, 4, new byte[]{1, 2}),
            record('3', 0x08008002L, 4, new byte[]{3, 4}),
            record('7', 0x08008000L, 4, new byte[0]));

        SrecParser parser = new SrecParser();
        parser.parse(file.toFile());

        List<SrecParser.SRecord> segments = parser.getSegments();
        assertEquals(1, segments.size());
        assertEquals(0x08008000, segments.get(0).address);
        assertArrayEquals(new byte[]{1, 2, 3, 4}, segments.get(0).data);
    }

    @Test
    void rejectsBadChecksumLengthAndHex() throws IOException {
        String valid = record('3', 0x08008000L, 4, new byte[]{1, 2});
        assertThrows(IOException.class, () -> parse(valid.substring(0, valid.length() - 3) + "00\n"));
        assertThrows(IOException.class, () -> parse(valid.substring(0, 2) + "09" + valid.substring(4)));
        assertThrows(IOException.class, () -> parse(valid.substring(0, 8) + "GG" + valid.substring(10)));
    }

    @Test
    void rejectsOverlappingDataRecords() throws IOException {
        assertThrows(IOException.class, () -> parse(
            record('3', 0x08008000L, 4, new byte[]{1, 2})
                + record('3', 0x08008001L, 4, new byte[]{3, 4})));
    }

    @Test
    void reportsEmptyDataRecordsWithoutBreakingOrdinaryParsing() throws IOException {
        SrecParser parser = new SrecParser();
        parser.parse(write(record('3', 0x08008000L, 4, new byte[0])).toFile());
        assertTrue(parser.hasEmptyDataRecords());
    }

    private void parse(String content) throws IOException {
        SrecParser parser = new SrecParser();
        parser.parse(write(content).toFile());
    }

    private Path write(String... records) throws IOException {
        Path file = tempDir.resolve("test.srec");
        Files.write(file, String.join("", records).getBytes(StandardCharsets.US_ASCII));
        return file;
    }

    private static String record(char type, long address, int addressSize, byte[] data) {
        byte[] body = new byte[addressSize + data.length];
        for (int i = 0; i < addressSize; i++) {
            body[i] = (byte) (address >> (8 * (addressSize - i - 1)));
        }
        System.arraycopy(data, 0, body, addressSize, data.length);

        int count = body.length + 1;
        int sum = count;
        StringBuilder result = new StringBuilder(String.format("S%c%02X", type, count));
        for (byte value : body) {
            sum += value & 0xFF;
            result.append(String.format("%02X", value & 0xFF));
        }
        return result.append(String.format("%02X%n", (~sum) & 0xFF)).toString();
    }
}
