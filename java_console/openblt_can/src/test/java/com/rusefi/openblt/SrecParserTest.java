package com.rusefi.openblt;

import org.junit.jupiter.api.Test;

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

    @Test
    void parsesContiguousS3RecordsIntoOneSegment() throws IOException {
        byte[] data = pattern(40);
        List<String> lines = SrecTestUtil.image(0x08008000, data);
        Path file = write(lines);

        SrecParser.Image image = SrecParser.parse(file);

        assertEquals(1, image.segments().size());
        SrecParser.Segment seg = image.segments().get(0);
        assertEquals(0x08008000, seg.base());
        assertArrayEquals(data, seg.data());
        assertEquals(40, image.totalBytes());
    }

    @Test
    void splitsNonContiguousRecords() throws IOException {
        byte[] a = pattern(16);
        byte[] b = pattern(8);
        List<String> lines = new java.util.ArrayList<>(SrecTestUtil.image(0x08008000, a));
        lines.remove(lines.size() - 1); // drop terminator
        lines.addAll(SrecTestUtil.image(0x08010000, b));
        Path file = write(lines);

        SrecParser.Image image = SrecParser.parse(file);

        assertEquals(2, image.segments().size());
        assertEquals(0x08008000, image.segments().get(0).base());
        assertEquals(0x08010000, image.segments().get(1).base());
        assertArrayEquals(a, image.segments().get(0).data());
        assertArrayEquals(b, image.segments().get(1).data());
    }

    @Test
    void parsesS1Records() throws IOException {
        byte[] data = pattern(6);
        List<String> lines = List.of(
                SrecTestUtil.s0("test"),
                SrecTestUtil.s1(0x1234, data),
                SrecTestUtil.terminator());
        Path file = write(lines);

        SrecParser.Image image = SrecParser.parse(file);

        assertEquals(1, image.segments().size());
        assertEquals(0x1234, image.segments().get(0).base());
        assertArrayEquals(data, image.segments().get(0).data());
    }

    @Test
    void rejectsBadChecksum() throws IOException {
        List<String> lines = SrecTestUtil.image(0x08008000, pattern(8));
        // Corrupt one data byte without fixing the checksum.
        String good = lines.get(1);
        int pos = good.length() - 3; // last data nibble
        char flipped = good.charAt(pos) == '0' ? '1' : '0';
        lines.set(1, good.substring(0, pos) + flipped + good.substring(pos + 1));
        Path file = write(lines);

        assertThrows(IllegalArgumentException.class, () -> SrecParser.parse(file));
    }

    @Test
    void rejectsMissingTerminator() throws IOException {
        List<String> lines = SrecTestUtil.image(0x08008000, pattern(8));
        lines.remove(lines.size() - 1);
        Path file = write(lines);

        assertThrows(IllegalArgumentException.class, () -> SrecParser.parse(file));
    }

    @Test
    void rejectsByteCountMismatch() throws IOException {
        List<String> lines = SrecTestUtil.image(0x08008000, pattern(8));
        // Patch the count byte to a wrong value.
        String good = lines.get(1);
        lines.set(1, good.substring(0, 2) + "FF" + good.substring(4));
        Path file = write(lines);

        assertThrows(IllegalArgumentException.class, () -> SrecParser.parse(file));
    }

    @Test
    void rejectsNonSrecLine() throws IOException {
        Path file = write(List.of("hello world"));

        assertThrows(IllegalArgumentException.class, () -> SrecParser.parse(file));
    }

    @Test
    void rejectsEmptyData() throws IOException {
        Path file = write(List.of(SrecTestUtil.s0("test"), SrecTestUtil.terminator()));

        assertThrows(IllegalArgumentException.class, () -> SrecParser.parse(file));
    }

    private static byte[] pattern(int len) {
        byte[] out = new byte[len];
        for (int i = 0; i < len; i++) {
            out[i] = (byte) (i * 7 + 3);
        }
        return out;
    }

    private static Path write(List<String> lines) throws IOException {
        Path file = Files.createTempFile("openblt_test", ".srec");
        Files.write(file, lines, StandardCharsets.US_ASCII);
        file.toFile().deleteOnExit();
        return file;
    }
}
