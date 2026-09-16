package com.rusefi.mcp;

import com.opensr5.ini.IniFileModel;
import com.opensr5.ini.field.ScalarIniField;
import com.rusefi.config.FieldType;
import com.rusefi.core.OutputChannelSnapshot;
import com.rusefi.core.SensorCentral;
import org.json.simple.JSONObject;
import org.junit.jupiter.api.Test;
import org.junit.jupiter.api.io.TempDir;

import java.nio.file.Files;
import java.nio.file.Path;
import java.util.BitSet;
import java.util.Collections;

import static org.junit.jupiter.api.Assertions.*;
import static org.mockito.Mockito.*;

class EcuDataLoggerTest {
    @TempDir
    Path tempDir;

    @Test
    void onlyRecordsCompleteCurrentSnapshotsAndDetachesOnStop() throws Exception {
        SensorCentral sensors = SensorCentral.getInstance();
        IniFileModel ini = ini();
        EcuDataLogger logger = new EcuDataLogger();
        Path file = tempDir.resolve("samples.mlg");
        try {
            logger.start(ini, file, null);
            long generation = sensors.getOutputChannelDemand().getGeneration();
            publish(ini, generation - 1, true);
            publish(ini, generation, false);
            assertEquals(0L, logger.status().get("sampleCount"));
            publish(ini, generation, true);
            assertEquals(1L, logger.status().get("sampleCount"));
            logger.stop();
            long size = Files.size(file);
            publish(ini, generation, true);
            assertEquals(1L, logger.status().get("sampleCount"));
            assertEquals(size, Files.size(file));
            assertFalse(sensors.getOutputChannelDemand().isFull());
        } finally {
            logger.stop();
        }
    }

    @Test
    void rejectsMissingDirectoryAndEmptyChannelsWithoutAcquiringLease() {
        EcuDataLogger logger = new EcuDataLogger();
        assertThrows(java.io.IOException.class,
                () -> logger.start(ini(), tempDir.resolve("missing/file.mlg"), null));
        IniFileModel empty = mock(IniFileModel.class);
        when(empty.getAllOutputChannels()).thenReturn(Collections.emptyMap());
        assertThrows(IllegalArgumentException.class, () -> logger.start(empty, tempDir.resolve("empty.mlg"), null));
        assertFalse(Files.exists(tempDir.resolve("empty.mlg")));
        assertEquals(Boolean.FALSE, logger.status().get("logging"));
        assertFalse(SensorCentral.getInstance().getOutputChannelDemand().isFull());
    }

    @Test
    void defaultPathCreatesIndependentFilesAndResetsCount() throws Exception {
        EcuDataLogger logger = new EcuDataLogger();
        Path first = null;
        Path second = null;
        try {
            first = Path.of((String) logger.start(ini(), null, null).get("path"));
            assertTrue(first.isAbsolute());
            assertTrue(Files.size(first) > 24, "header is written before start succeeds");
            logger.stop();
            second = Path.of((String) logger.start(ini(), null, null).get("path"));
            assertNotEquals(first, second);
            assertEquals(0L, logger.status().get("sampleCount"));
        } finally {
            logger.stop();
            if (first != null) {
                Files.delete(first);
            }
            if (second != null) {
                Files.delete(second);
            }
        }
    }

    @Test
    void encodingFailureStopsRecordingAndRemainsVisibleUntilNextStart() throws Exception {
        IniFileModel ini = ini();
        // Force a row-encoding failure after the header has been written successfully.
        ScalarIniField broken = mock(ScalarIniField.class);
        when(broken.getType()).thenReturn(FieldType.UINT16);
        when(broken.getUnits()).thenReturn("RPM");
        when(broken.getOffset()).thenThrow(new IllegalStateException("encoding failed"));
        when(ini.getAllOutputChannels()).thenReturn(Collections.singletonMap("rpm", broken));
        EcuDataLogger logger = new EcuDataLogger();
        try {
            logger.start(ini, tempDir.resolve("broken.mlg"), null);
            // Decode through the healthy INI so only the recorder sees the broken field.
            publish(ini(), SensorCentral.getInstance().getOutputChannelDemand().getGeneration(), true);
            JSONObject status = logger.status();
            assertEquals(Boolean.FALSE, status.get("success"));
            assertEquals(Boolean.FALSE, status.get("logging"));
            assertTrue(status.get("error").toString().contains("encoding failed"));
            assertEquals(0L, status.get("sampleCount"));
            assertEquals(status, logger.stop());
            assertFalse(SensorCentral.getInstance().getOutputChannelDemand().isFull());
            assertEquals(Boolean.TRUE, logger.start(ini(), tempDir.resolve("recovered.mlg"), null).get("success"));
        } finally {
            logger.stop();
        }
    }

    @Test
    void tuneSaveFailureDoesNotLeaveRecordingOrLogFile() throws Exception {
        EcuDataLogger logger = new EcuDataLogger();
        Path log = tempDir.resolve("failed.mlg");
        // Empty tunes cannot be serialized by the shared MSQ writer.
        com.rusefi.tune.xml.Msq invalid = com.rusefi.tune.xml.Msq.create(128, "test");
        assertThrows(IllegalStateException.class, () -> logger.start(ini(), log, invalid));
        assertEquals(Boolean.FALSE, logger.status().get("logging"));
        assertFalse(Files.exists(log));
        assertFalse(SensorCentral.getInstance().getOutputChannelDemand().isFull());
        try (java.util.stream.Stream<Path> files = Files.list(tempDir)) {
            assertEquals(0, files.count());
        }
    }

    private static IniFileModel ini() throws Exception {
        IniFileModel ini = mock(IniFileModel.class);
        ScalarIniField rpm = new ScalarIniField("rpm", 0, "RPM", FieldType.UINT16, 1, "0", 0);
        when(ini.getAllOutputChannels()).thenReturn(Collections.singletonMap("rpm", rpm));
        when(ini.getOutputChannel("rpm")).thenReturn(rpm);
        when(ini.getExpressionOutputChannels()).thenReturn(Collections.emptyMap());
        when(ini.getGauges()).thenReturn(Collections.emptyMap());
        return ini;
    }

    private static void publish(IniFileModel ini, long generation, boolean full) {
        BitSet valid = new BitSet(2);
        valid.set(0, 2);
        SensorCentral.getInstance().grabSensorValues(new OutputChannelSnapshot(
                new byte[]{0, 42, 0}, valid, Collections.emptySet(), generation, full), ini, null);
    }
}
