package com.rusefi.sensor_logs;

import com.rusefi.core.SensorCategory;
import org.junit.jupiter.api.Test;
import org.junit.jupiter.api.io.TempDir;

import java.io.ByteArrayOutputStream;
import java.io.DataOutputStream;
import java.io.IOException;
import java.io.OutputStream;
import java.util.concurrent.atomic.AtomicBoolean;
import java.util.concurrent.atomic.AtomicLong;
import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.nio.charset.StandardCharsets;
import java.nio.file.Files;
import java.nio.file.Path;
import java.util.Collections;
import java.util.concurrent.atomic.AtomicReference;

import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertTrue;
import static org.junit.jupiter.api.Assertions.assertThrows;

class BinarySensorLogTest {
    @TempDir
    Path tempDir;

    @Test
    void checkedWriterPropagatesOpenAndRowFailures() throws IOException {
        AtomicBoolean fail = new AtomicBoolean(true);
        OutputStream output = new OutputStream() {
            @Override
            public void write(int value) throws IOException {
                if (fail.get()) {
                    throw new IOException("disk full");
                }
            }
        };
        assertThrows(IOException.class, () -> new BinarySensorLog<>(entry -> 0.0,
                Collections.<BinaryLogEntry>emptyList(), output));
        fail.set(false);
        BinarySensorLog<BinaryLogEntry> writer = new BinarySensorLog<>(entry -> 0.0,
                Collections.emptyList(), output);
        fail.set(true);
        assertEquals("disk full", assertThrows(IOException.class, writer::writeSensorLogLineChecked).getMessage());
        writer.close();
    }

    @Test
    void writesValidHeaderAndCurrentValues() throws IOException {
        AtomicReference<Double> value = new AtomicReference<>(12.5);
        AtomicLong clock = new AtomicLong(123);
        Path file = tempDir.resolve("test.mlg");
        BinaryLogEntry floatEntry = new BinaryLogEntry() {
            @Override
            public String getName() {
                return "testValue";
            }

            @Override
            public SensorCategory getCategory() {
                return SensorCategory.OTHERS;
            }

            @Override
            public String getUnit() {
                return "unit";
            }

            @Override
            public int getByteSize() {
                return 7;
            }

            @Override
            public float getScale() {
                return 0.5f;
            }

            @Override
            public void writeToLog(DataOutputStream stream, double value) throws IOException {
                stream.writeFloat((float) value);
            }
        };

        BinarySensorLog<BinaryLogEntry> log = new BinarySensorLog<>(
                entry -> value.get(), Collections.singletonList(floatEntry), clock::get, file.toString());
        log.writeSensorLogLine();
        value.set(25.0);
        clock.set(1623);
        log.writeSensorLogLine();
        log.close();

        byte[] bytes = Files.readAllBytes(file);
        assertEquals("MLVLG\0", new String(bytes, 0, 6, StandardCharsets.US_ASCII));

        ByteBuffer data = ByteBuffer.wrap(bytes).order(ByteOrder.BIG_ENDIAN);
        data.position(12);
        int infoStart = data.getInt();
        int dataBegin = data.getInt();
        // [tag:mlg_time_field] the Time field comes first, ahead of the caller's entries
        assertEquals(24 + 2 * 89, infoStart);
        assertTrue(dataBegin > infoStart);
        assertEquals(8, data.getShort());
        assertEquals(2, data.getShort());
        assertEquals(dataBegin + 26, bytes.length);
        assertEquals(7, bytes[24], "Time is a float");
        assertEquals("Time", fieldText(bytes, 0, 1, 34));
        assertEquals("s", fieldText(bytes, 0, 35, 11));
        assertEquals(1f, data.getFloat(24 + 46));
        assertEquals("testValue", fieldText(bytes, 1, 1, 34));
        assertEquals(0.5f, data.getFloat(24 + 89 + 46));

        data.position(dataBegin);
        assertEquals(0, data.get());
        assertEquals(0, data.get());
        data.getShort();
        assertEquals(0f, data.getFloat(), "Time starts at the first row");
        assertEquals(12.5f, data.getFloat());
        data.get();

        assertEquals(0, data.get());
        assertEquals(1, data.get());
        data.getShort();
        assertEquals(1.5f, data.getFloat(), "seconds since the first row");
        assertEquals(25.0f, data.getFloat());
    }

    /** [tag:mlg_time_field] entries that bring their own Time field are written as they are */
    @Test
    void callersOwnTimeFieldIsNotDuplicated() throws IOException {
        ByteArrayOutputStream bytes = new ByteArrayOutputStream();
        BinarySensorLog<BinaryLogEntry> log = new BinarySensorLog<>(entry -> 3.0,
                Collections.singletonList(floatEntry("time")), bytes);
        log.writeSensorLogLineChecked();
        log.close();
        ByteBuffer data = ByteBuffer.wrap(bytes.toByteArray());
        assertEquals(1, data.getShort(22));
        assertEquals(4, data.getShort(20));
        assertEquals(3f, data.getFloat(data.getInt(16) + 4));
    }

    private static String fieldText(byte[] bytes, int field, int offset, int length) {
        return new String(bytes, 24 + field * 89 + offset, length, StandardCharsets.US_ASCII).trim();
    }

    private static BinaryLogEntry floatEntry(String name) {
        return new BinaryLogEntry() {
            @Override
            public String getName() {
                return name;
            }

            @Override
            public SensorCategory getCategory() {
                return SensorCategory.OTHERS;
            }

            @Override
            public String getUnit() {
                return "unit";
            }

            @Override
            public int getByteSize() {
                return 7;
            }

            @Override
            public void writeToLog(DataOutputStream stream, double value) throws IOException {
                stream.writeFloat((float) value);
            }
        };
    }
}
