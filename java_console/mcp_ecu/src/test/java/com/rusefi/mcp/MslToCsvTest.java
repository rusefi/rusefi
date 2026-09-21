package com.rusefi.mcp;

import com.rusefi.core.SensorCategory;
import com.rusefi.sensor_logs.BinaryLogEntry;
import com.rusefi.sensor_logs.BinarySensorLog;
import org.junit.jupiter.api.Test;
import org.junit.jupiter.api.io.TempDir;

import java.io.ByteArrayOutputStream;
import java.io.DataOutputStream;
import java.io.IOException;
import java.nio.ByteBuffer;
import java.nio.charset.StandardCharsets;
import java.nio.file.Files;
import java.nio.file.Path;
import java.util.Arrays;
import java.util.Collections;
import java.util.Locale;
import java.util.stream.Stream;

import static org.junit.jupiter.api.Assertions.*;

class MslToCsvTest {
    @TempDir Path directory;

    @Test
    void binaryDecodesAllScalarTypesAndTransformsWithUnits() throws Exception {
        Path input = directory.resolve("capture.mlg");
        Files.write(input, binaryFixture());
        Locale previous = Locale.getDefault();
        try {
            Locale.setDefault(Locale.GERMANY);
            MslToCsv.Result result = MslToCsv.convert(input, directory.resolve("capture.csv"));
            assertEquals("mlg", result.inputFormat);
            assertEquals(8, result.fieldCount);
            assertEquals(2, result.recordCount);
            String row = "255,-12,65535,-1234,4294967295,-123456,-5000000000,3.50\n";
            assertEquals("f0,f1,f2,f3,f4,f5,f6,\"AFR, \"\"wide\"\" (ratio)\"\n" + row + row,
                    new String(Files.readAllBytes(result.path), StandardCharsets.UTF_8));
        } finally {
            Locale.setDefault(previous);
        }
    }

    @Test
    void convertsProductionJavaLoggerOutput() throws Exception {
        BinaryLogEntry rpm = new BinaryLogEntry() {
            public String getName() { return "RPM"; }
            public SensorCategory getCategory() { return SensorCategory.DEBUG; }
            public String getUnit() { return "rpm"; }
            public int getByteSize() { return 2; }
            public void writeToLog(DataOutputStream output, double value) throws IOException {
                output.writeShort((int) value);
            }
        };
        ByteArrayOutputStream bytes = new ByteArrayOutputStream();
        BinarySensorLog<BinaryLogEntry> logger = new BinarySensorLog<>(entry -> 1234.0,
                Collections.singletonList(rpm), bytes);
        logger.writeSensorLogLineChecked();
        logger.close();
        Path input = directory.resolve("java.mlg");
        Files.write(input, bytes.toByteArray());
        Path output = directory.resolve("java.csv");
        MslToCsv.convert(input, output);
        assertEquals("RPM (rpm)\n1234.00\n", new String(Files.readAllBytes(output), StandardCharsets.UTF_8));
    }

    @Test
    void textPreservesEmptyCellsAndEscapesCommasAndQuotes() throws Exception {
        Path input = directory.resolve("text.msl");
        Files.write(input, ("\uFEFF\"signature\"\r\n\"Capture Date: today\"\r\n#\r\n"
                + "Time\tlabel,quoted\tempty\r\ns\t\t\r\n0.5\ta\"b\t\r\n\r\n").getBytes(StandardCharsets.UTF_8));
        Path output = MslToCsv.defaultOutput(input);
        MslToCsv.Result result = MslToCsv.convert(input, output);
        assertEquals("msl", result.inputFormat);
        assertEquals(1, result.recordCount);
        assertEquals(3, result.fieldCount);
        assertEquals("Time (s),\"label,quoted\",empty\n0.5,\"a\"\"b\",\n",
                new String(Files.readAllBytes(output), StandardCharsets.UTF_8));
    }

    @Test
    void rejectsMalformedBinaryWithoutPublishingPartialCsv() throws Exception {
        byte[] original = binaryFixture();
        int dataBegin = ByteBuffer.wrap(original).getInt(16);
        for (int offset : new int[]{5, 7, 16, 20, 24, dataBegin, original.length - 1}) {
            byte[] invalid = original.clone();
            invalid[offset] = (byte) 99;
            assertRejected(invalid);
        }
        for (int length : new int[]{5, 23, 24, dataBegin - 1, dataBegin + 1, original.length - 1}) {
            assertRejected(Arrays.copyOf(original, length));
        }
        // Failures after a valid first record must also leave no partial CSV.
        assertRejected("Time\tRPM\ns\trpm\n0\t100\n1\n".getBytes(StandardCharsets.UTF_8));
        assertRejected("garbage".getBytes(StandardCharsets.UTF_8));
    }

    @Test
    void refusesToOverwriteOutputOrInput() throws Exception {
        Path input = directory.resolve("input.mlg");
        byte[] original = binaryFixture();
        Files.write(input, original);
        assertThrows(IOException.class, () -> MslToCsv.convert(input, input));
        Path output = directory.resolve("existing.csv");
        Files.write(output, new byte[]{42});
        assertThrows(IOException.class, () -> MslToCsv.convert(input, output));
        assertArrayEquals(new byte[]{42}, Files.readAllBytes(output));
        assertArrayEquals(original, Files.readAllBytes(input));
    }

    private void assertRejected(byte[] bytes) throws Exception {
        Path input = directory.resolve("bad.mlg");
        Path output = directory.resolve("bad.csv");
        Files.write(input, bytes);
        assertThrows(IOException.class, () -> MslToCsv.convert(input, output));
        assertFalse(Files.exists(output));
        try (Stream<Path> files = Files.list(directory)) {
            assertFalse(files.anyMatch(path -> path.getFileName().toString().endsWith(".tmp")));
        }
    }

    /** Independent wire fixture: eight scalar types, info text, two records across timestamp wrap. */
    private static byte[] binaryFixture() throws IOException {
        ByteArrayOutputStream bytes = new ByteArrayOutputStream();
        DataOutputStream out = new DataOutputStream(bytes);
        out.write(new byte[]{'M', 'L', 'V', 'L', 'G', 0});
        out.writeShort(2);
        out.writeInt(0);
        out.writeInt(24 + 89 * 8);
        out.writeInt(24 + 89 * 8 + 4);
        out.writeShort(26);
        out.writeShort(8);
        for (int type = 0; type < 8; type++) {
            byte[] descriptor = new byte[89];
            descriptor[0] = (byte) type;
            byte[] name = (type == 7 ? "AFR, \"wide\"" : "f" + type).getBytes(StandardCharsets.US_ASCII);
            System.arraycopy(name, 0, descriptor, 1, name.length);
            if (type == 7) {
                System.arraycopy("ratio".getBytes(StandardCharsets.US_ASCII), 0, descriptor, 35, 5);
            }
            ByteBuffer.wrap(descriptor).putFloat(46, type == 7 ? 2 : 1).putFloat(50, type == 7 ? 0.5f : 0);
            descriptor[54] = (byte) (type == 7 ? 2 : 0);
            out.write(descriptor);
        }
        out.writeInt(0); // optional info area
        byte[] payload = ByteBuffer.allocate(26).put((byte) 255).put((byte) -12)
                .putShort((short) 65535).putShort((short) -1234).putInt(-1).putInt(-123456)
                .putLong(-5000000000L).putFloat(1.25f).array();
        int checksum = 0;
        for (byte value : payload) {
            checksum += value & 0xff;
        }
        for (int i = 0; i < 2; i++) {
            out.writeByte(0);
            out.writeByte(i);
            out.writeShort(i == 0 ? 65530 : 10);
            out.write(payload);
            out.writeByte(checksum);
        }
        return bytes.toByteArray();
    }
}
