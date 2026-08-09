package com.rusefi.sensor_logs;

import com.rusefi.core.SensorCategory;
import org.junit.jupiter.api.Test;
import org.junit.jupiter.api.io.TempDir;

import java.io.DataOutputStream;
import java.io.IOException;
import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.nio.charset.StandardCharsets;
import java.nio.file.Files;
import java.nio.file.Path;
import java.util.Collections;
import java.util.concurrent.atomic.AtomicReference;

import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertTrue;

class BinarySensorLogTest {
    @TempDir
    Path tempDir;

    @Test
    void writesValidHeaderAndCurrentValues() throws IOException {
        AtomicReference<Double> value = new AtomicReference<>(12.5);
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
                entry -> value.get(), Collections.singletonList(floatEntry), () -> 123, file.toString());
        log.writeSensorLogLine();
        value.set(25.0);
        log.writeSensorLogLine();
        log.close();

        byte[] bytes = Files.readAllBytes(file);
        assertEquals("MLVLG\0", new String(bytes, 0, 6, StandardCharsets.US_ASCII));

        ByteBuffer data = ByteBuffer.wrap(bytes).order(ByteOrder.BIG_ENDIAN);
        data.position(12);
        int infoStart = data.getInt();
        int dataBegin = data.getInt();
        assertEquals(24 + 89, infoStart);
        assertTrue(dataBegin > infoStart);
        assertEquals(4, data.getShort());
        assertEquals(1, data.getShort());
        assertEquals(dataBegin + 18, bytes.length);
        assertEquals(0.5f, data.getFloat(24 + 46));

        data.position(dataBegin);
        assertEquals(0, data.get());
        assertEquals(0, data.get());
        data.getShort();
        assertEquals(12.5f, data.getFloat());
        data.get();

        assertEquals(0, data.get());
        assertEquals(1, data.get());
        data.getShort();
        assertEquals(25.0f, data.getFloat());
    }
}
