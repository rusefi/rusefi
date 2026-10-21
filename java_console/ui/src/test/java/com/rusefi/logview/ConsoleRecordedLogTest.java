package com.rusefi.logview;

import com.rusefi.core.SensorCategory;
import com.rusefi.sensor_logs.BinaryLogEntry;
import com.rusefi.sensor_logs.BinarySensorLog;
import org.junit.jupiter.api.Test;

import java.io.ByteArrayInputStream;
import java.io.ByteArrayOutputStream;
import java.io.DataOutputStream;
import java.io.IOException;
import java.util.Arrays;
import java.util.List;

import static com.rusefi.config.generated.VariableRegistryValues.GAUGE_NAME_TIME;
import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertTrue;

/**
 * Logs recorded by the console (Binary Logging > Start, {@link BinarySensorLog}) have no Time field:
 * time is only in each data block's 16-bit timestamp, in 10 us units, as MegaLogViewer reads it.
 * ECU-written logs (firmware binary_mlg_logging.cpp) carry an explicit Time field instead.
 */
class ConsoleRecordedLogTest {
    private static BinaryLogEntry entry(String name, String unit, int mlgType) {
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
                return unit;
            }

            @Override
            public int getByteSize() {
                return mlgType;
            }

            @Override
            public void writeToLog(DataOutputStream dos, double value) throws IOException {
                if (mlgType == 7) {
                    dos.writeFloat((float) value);
                } else {
                    dos.writeShort((int) value);
                }
            }
        };
    }

    /** Three records written by the console's own recorder. */
    private static byte[] consoleLog() throws IOException {
        List<BinaryLogEntry> entries = Arrays.asList(entry("RPM", "rpm", 3), entry("MAP", "kPa", 7));
        double[] rpm = {800, 1500, 2500};
        double[] map = {35.5, 60.25, 98};
        int[] row = {0};
        ByteArrayOutputStream bytes = new ByteArrayOutputStream();
        BinarySensorLog<BinaryLogEntry> log = new BinarySensorLog<>(
                e -> e.getName().equals("RPM") ? rpm[row[0]] : map[row[0]], entries, bytes);
        for (row[0] = 0; row[0] < 3; row[0]++) {
            log.writeSensorLogLineChecked();
        }
        log.close();
        return bytes.toByteArray();
    }

    @Test
    void consoleRecordedLogOpensWithTimeFromTheBlocks() throws IOException {
        CsvLogReader log = BinaryLogReader.read(new ByteArrayInputStream(consoleLog()));
        assertEquals(3, log.sampleCount());
        assertEquals(3, log.fieldCount(), "Time added in front of RPM and MAP");
        assertEquals(GAUGE_NAME_TIME, log.name(0));
        assertEquals("s", log.unit(0));
        assertEquals("RPM", log.name(1));
        assertEquals("kPa", log.unit(2));
        assertEquals(1500, log.value(1, 1), 1e-9);
        assertEquals(98, log.value(2, 2), 1e-6);
        // the records were written back to back by the real clock
        assertEquals(0, log.value(0, 0), 1e-12);
        for (int sample = 1; sample < 3; sample++) {
            assertTrue(log.value(sample, 0) >= log.value(sample - 1, 0));
        }
        assertTrue(log.value(2, 0) < 0.6);
    }

    @Test
    void timestampStepsSurviveTheCounterWrapping() {
        assertEquals(11800, BinaryLogReader.timestampStep(14388, 26188));
        // from test recording rusEFI_outputChannels: 59188 -> 4652 is 11000 steps (110 ms) across the wrap
        assertEquals(11000, BinaryLogReader.timestampStep(59188, 4652));
        assertEquals(0, BinaryLogReader.timestampStep(500, 500));
    }

    @Test
    void ecuLogKeepsItsOwnTimeField() throws IOException {
        // the bundled log was written by the ECU and has a Time field; no column is added
        CsvLogReader log = LogFiles.read(getClass().getResourceAsStream("/logview/test-log-binary-1.mlg"), "x.mlg");
        assertEquals(917, log.fieldCount());
        assertEquals(GAUGE_NAME_TIME, log.name(0));
    }
}
