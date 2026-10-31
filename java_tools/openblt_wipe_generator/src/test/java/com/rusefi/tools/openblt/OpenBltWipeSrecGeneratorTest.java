package com.rusefi.tools.openblt;

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

class OpenBltWipeSrecGeneratorTest {
    private static final long UAEFI_END = 0x08100000L;

    @TempDir
    Path tempDir;

    @Test
    void generatesDeterministicChecksummedUaefiEraseMapAndManifest() throws IOException {
        Path output = tempDir.resolve("uaefi_wipe.srec");
        Path manifest = tempDir.resolve("openblt_wipe.properties");
        OpenBltWipeSrecGenerator.generate(output, manifest,
            OpenBltWipeSrecGenerator.APPLICATION_START, UAEFI_END,
            "uaefi", "uaefi", "ARCH_STM32F4");
        byte[] firstOutput = Files.readAllBytes(output);

        Path secondOutput = tempDir.resolve("uaefi_wipe_2.srec");
        Path secondManifest = tempDir.resolve("openblt_wipe_2.properties");
        OpenBltWipeSrecGenerator.generate(secondOutput, secondManifest,
            OpenBltWipeSrecGenerator.APPLICATION_START, UAEFI_END,
            "uaefi", "uaefi", "ARCH_STM32F4");
        assertArrayEquals(firstOutput, Files.readAllBytes(secondOutput));

        List<String> lines = Files.readAllLines(output, StandardCharsets.US_ASCII);
        assertEquals('0', lines.get(0).charAt(1));
        assertEquals("rusEFI OpenBLT wipe", new String(parse(lines.get(0)).data, StandardCharsets.US_ASCII));
        assertEquals(240, parse(lines.get(1)).data.length);

        long expectedAddress = OpenBltWipeSrecGenerator.APPLICATION_START;
        int dataRecordCount = 0;
        for (String line : lines) {
            Record record = parse(line);
            if (record.type != '3') {
                continue;
            }
            assertEquals(expectedAddress, record.address);
            assertTrue(record.data.length > 0 && record.data.length <= OpenBltWipeSrecGenerator.RECORD_DATA_SIZE);
            for (byte value : record.data) {
                assertEquals((byte) 0xFF, value);
            }
            expectedAddress += record.data.length;
            dataRecordCount++;
        }
        assertEquals(UAEFI_END, expectedAddress);

        Record count = parse(lines.get(lines.size() - 2));
        assertEquals('5', count.type);
        assertEquals(dataRecordCount, count.address);
        assertEquals(0, count.data.length);

        Record termination = parse(lines.get(lines.size() - 1));
        assertEquals('7', termination.type);
        assertEquals(OpenBltWipeSrecGenerator.APPLICATION_START, termination.address);
        assertEquals(0, termination.data.length);

        assertEquals(
            "format=1\n"
                + "bundleTarget=uaefi\n"
                + "bootloaderTarget=uaefi\n"
                + "mcuFamily=ARCH_STM32F4\n"
                + "startAddress=0x08008000\n"
                + "endAddressExclusive=0x08100000\n"
                + "srec=uaefi_wipe.srec\n",
            new String(Files.readAllBytes(manifest), StandardCharsets.US_ASCII));
    }

    @Test
    void rejectsUnsupportedAndAmbiguousOutputs() {
        Path output = tempDir.resolve("wipe.srec");
        Path manifest = tempDir.resolve("wipe.properties");
        assertThrows(IllegalArgumentException.class, () -> OpenBltWipeSrecGenerator.generate(
            output, manifest, OpenBltWipeSrecGenerator.APPLICATION_START, 0x08200000L,
            "uaefi", "uaefi", "ARCH_STM32F4"));
        assertThrows(IllegalArgumentException.class, () -> OpenBltWipeSrecGenerator.generate(
            output, tempDir.resolve("other/wipe.properties"),
            OpenBltWipeSrecGenerator.APPLICATION_START, UAEFI_END,
            "uaefi", "uaefi", "ARCH_STM32F4"));
        assertThrows(IllegalArgumentException.class, () -> OpenBltWipeSrecGenerator.generate(
            output, output, OpenBltWipeSrecGenerator.APPLICATION_START, UAEFI_END,
            "uaefi", "uaefi", "ARCH_STM32F4"));
    }

    @Test
    void rejectsInvalidRangesAndS5Overflow() {
        Path output = tempDir.resolve("wipe.srec");
        assertThrows(IllegalArgumentException.class, () -> OpenBltWipeSrecGenerator.generateSrec(
            output, 0x08008000L, 0x08008000L));
        assertThrows(IllegalArgumentException.class, () -> OpenBltWipeSrecGenerator.generateSrec(
            output, 0, (long) OpenBltWipeSrecGenerator.RECORD_DATA_SIZE * 0x10000));
    }

    private static Record parse(String line) {
        char type = line.charAt(1);
        byte[] bytes = fromHex(line.substring(2));
        int count = bytes[0] & 0xFF;
        assertEquals(count + 1, bytes.length);
        int checksum = 0;
        for (byte value : bytes) {
            checksum += value & 0xFF;
        }
        assertEquals(0xFF, checksum & 0xFF);

        int addressSize = type == '0' || type == '5' ? 2 : 4;
        long address = 0;
        for (int i = 0; i < addressSize; i++) {
            address = address << 8 | bytes[i + 1] & 0xFF;
        }
        int dataLength = count - addressSize - 1;
        byte[] data = new byte[dataLength];
        System.arraycopy(bytes, addressSize + 1, data, 0, dataLength);
        return new Record(type, address, data);
    }

    private static byte[] fromHex(String value) {
        byte[] result = new byte[value.length() / 2];
        for (int i = 0; i < result.length; i++) {
            result[i] = (byte) Integer.parseInt(value.substring(i * 2, i * 2 + 2), 16);
        }
        return result;
    }

    private static class Record {
        final char type;
        final long address;
        final byte[] data;

        Record(char type, long address, byte[] data) {
            this.type = type;
            this.address = address;
            this.data = data;
        }
    }
}
