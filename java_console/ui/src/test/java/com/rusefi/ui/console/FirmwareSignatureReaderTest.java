package com.rusefi.ui.console;

import com.rusefi.core.RusEfiSignature;
import org.junit.jupiter.api.Test;
import org.junit.jupiter.api.io.TempDir;

import java.io.IOException;
import java.nio.charset.StandardCharsets;
import java.nio.file.Files;
import java.nio.file.Path;

import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertNull;
import static org.junit.jupiter.api.Assertions.assertThrows;

class FirmwareSignatureReaderTest {
    private static final String SIGNATURE = "rusEFI development.2026.10.26.proteus_f7.4226383888";
    @TempDir
    Path tempDir;

    @Test
    // [tag:embedded_firmware_signature] Ensure signature extraction uses image bytes, not its SREC header/name.
    void readsSignatureAcrossOutOfOrderAdjacentRecords() throws IOException {
        byte[] bytes = ("x" + SIGNATURE + "\0").getBytes(StandardCharsets.US_ASCII);
        int split = 15;
        RusEfiSignature result = read(
            record('0', 0, 2, SIGNATURE.getBytes(StandardCharsets.US_ASCII)), // S0 headers are not image data.
            record('3', 0x08001000L + split, 4, slice(bytes, split, bytes.length)),
            record('3', 0x08001000L, 4, slice(bytes, 0, split)));

        assertEquals("development", result.getBranch());
        assertEquals("2026", result.getYear());
        assertEquals("proteus_f7", result.getBundleTarget());
        assertEquals("4226383888", result.getHash());
    }

    @Test
    void doesNotJoinSignatureAcrossAddressGap() throws IOException {
        byte[] bytes = (SIGNATURE + "\0").getBytes(StandardCharsets.US_ASCII);
        assertNull(read(
            record('3', 0x08001000L, 4, slice(bytes, 0, 12)),
            record('3', 0x08001020L, 4, slice(bytes, 12, bytes.length))));
    }

    @Test
    void acceptsIdenticalDuplicatesButRejectsDistinctValidSignatures() throws IOException {
        byte[] duplicate = (SIGNATURE + "\0").getBytes(StandardCharsets.US_ASCII);
        assertEquals("4226383888", read(
            record('3', 0x08001000L, 4, duplicate),
            record('3', 0x08002000L, 4, duplicate)).getHash());

        byte[] different = "rusEFI development.2026.10.26.proteus_f7.1111111111\0".getBytes(StandardCharsets.US_ASCII);
        assertNull(read(record('3', 0x08001000L, 4, duplicate), record('3', 0x08002000L, 4, different)));
    }

    @Test
    void rejectsMissingMalformedTruncatedAndNonTerminatedSignatures() throws IOException {
        assertNull(read(record('3', 0x08001000L, 4, new byte[]{1, 2, 3})));
        assertNull(read(data("rusEFI nope\0")));
        assertNull(read(data("rusEFI development.2026.02.30.proteus_f7.4226383888\0")));
        assertNull(read(data("rusEFI .2026.10.26.proteus_f7.4226383888\0")));
        assertNull(read(data(SIGNATURE)));
        StringBuilder tooLong = new StringBuilder(SIGNATURE);
        for (int i = 0; i < 300; i++) {
            tooLong.append('x');
        }
        tooLong.append('\0');
        byte[] longBytes = tooLong.toString().getBytes(StandardCharsets.US_ASCII);
        assertNull(read(
            record('3', 0x08001000L, 4, slice(longBytes, 0, 200)),
            record('3', 0x08001000L + 200, 4, slice(longBytes, 200, longBytes.length))));
        assertNull(read(data("ciphertext rusEFI\u0001")));
    }

    @Test
    void rejectsGarbageInsideOtherwiseParseableSignatures() throws IOException {
        for (String malformed : new String[]{
            "rusEFI development.2026.10.26.proteus_f7.hash\0",
            SIGNATURE + "\u0001\0",
            SIGNATURE + " \0",
            SIGNATURE + "\n\0",
            SIGNATURE + ".\0",
            "rusEFI development.2026.1.26.proteus_f7.4226383888\0",
            "rusEFI develop ment.2026.10.26.proteus_f7.4226383888\0",
            "rusEFI develop\u0001ment.2026.10.26.proteus_f7.4226383888\0",
            "rusEFI development.2026.10.26.proteus\u0001_f7.4226383888\0"}) {
            assertNull(read(data(malformed)), malformed);
            // A malformed literal must not make a legitimate signature seem ambiguous.
            assertEquals("4226383888", read(data(malformed), record('3', 0x08002000L, 4,
                (SIGNATURE + "\0").getBytes(StandardCharsets.US_ASCII))).getHash(), malformed);
        }
        byte[] nonAscii = (SIGNATURE + "\0").getBytes(StandardCharsets.US_ASCII);
        nonAscii[10] = (byte) 0x80;
        assertNull(read(record('3', 0x08001000L, 4, nonAscii)));
    }

    @Test
    void validatesLeapDatesAndAcceptsLtsBranches() throws IOException {
        RusEfiSignature result = read(data("rusEFI lts-26lima.2024.02.29.proteus_f7.4226383888\0"));
        assertEquals("lts-26lima", result.getBranch());
        assertEquals("29", result.getDay());
        assertNull(read(data("rusEFI lts-26lima.2026.02.29.proteus_f7.4226383888\0")));
    }

    @Test
    void propagatesInvalidSrecAndIgnoresHeaderStrings() throws IOException {
        String valid = record('3', 0x08001000L, 4, "x".getBytes(StandardCharsets.US_ASCII));
        String badChecksum = valid.substring(0, valid.length() - 3) + "00\n";
        assertThrows(IOException.class, () -> FirmwareSignatureReader.read(write(badChecksum).toFile()));
        assertNull(read(record('0', 0, 2, (SIGNATURE + "\0").getBytes(StandardCharsets.US_ASCII))));
    }

    private RusEfiSignature read(String... records) throws IOException {
        return FirmwareSignatureReader.read(write(String.join("", records)).toFile());
    }

    private String data(String value) {
        return record('3', 0x08001000L, 4, value.getBytes(StandardCharsets.US_ASCII));
    }

    private Path write(String content) throws IOException {
        Path file = tempDir.resolve("image.srec");
        Files.write(file, content.getBytes(StandardCharsets.US_ASCII));
        return file;
    }

    private static byte[] slice(byte[] bytes, int start, int end) {
        byte[] result = new byte[end - start];
        System.arraycopy(bytes, start, result, 0, result.length);
        return result;
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
            sum += value & 0xff;
            result.append(String.format("%02X", value & 0xff));
        }
        return result.append(String.format("%02X%n", (~sum) & 0xff)).toString();
    }
}
