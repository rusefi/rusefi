package com.rusefi.sensor_logs;

import com.opensr5.ini.IniFileModel;
import com.opensr5.ini.field.EnumIniField;
import com.opensr5.ini.field.IniField;
import com.opensr5.ini.field.ScalarIniField;
import com.rusefi.binaryprotocol.BinaryProtocol;
import com.rusefi.config.FieldType;
import com.rusefi.core.SensorCentral;
import com.rusefi.ui.UIContext;
import org.junit.jupiter.api.Test;
import org.junit.jupiter.api.io.TempDir;

import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.nio.file.Files;
import java.nio.file.Path;
import java.util.Collections;
import java.util.LinkedHashMap;
import java.util.Map;

import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertFalse;
import static org.junit.jupiter.api.Assertions.assertTrue;
import static org.mockito.Mockito.mock;
import static org.mockito.Mockito.when;

class SensorLoggerTest {
    @TempDir
    Path tempDir;

    @Test
    void startsAndStopsExplicitly() throws Exception {
        UIContext context = mock(UIContext.class);
        BinaryProtocol protocol = mock(BinaryProtocol.class);
        IniFileModel ini = mock(IniFileModel.class);
        ScalarIniField rpm = new ScalarIniField("rpm", 0, "RPM", FieldType.UINT16, 1, "0", 0);
        ScalarIniField notGauge = new ScalarIniField("notGauge", 2, "V", FieldType.UINT8, 0.1, "1", 0);
        EnumIniField flag = new EnumIniField("flag", 3, FieldType.UINT8,
                new EnumIniField.EnumKeyValueMap(Collections.emptyMap()), 1, 0);
        Map<String, IniField> outputChannels = new LinkedHashMap<>();
        outputChannels.put("rpm", rpm);
        outputChannels.put("notGauge", notGauge);
        outputChannels.put("flag", flag);

        when(context.getBinaryProtocol()).thenReturn(protocol);
        when(protocol.getIniFileNullable()).thenReturn(ini);
        when(ini.getAllOutputChannels()).thenReturn(outputChannels);
        when(ini.getGauges()).thenReturn(Collections.emptyMap());
        when(ini.getOutputChannel("rpm")).thenReturn(rpm);
        when(ini.getOutputChannel("notGauge")).thenReturn(notGauge);
        when(ini.getOutputChannel("flag")).thenReturn(flag);

        SensorLogger logger = new SensorLogger(context);
        Path file = tempDir.resolve("selected.mlg");
        assertFalse(logger.isLogging());
        try {
            assertTrue(logger.start(file.toFile()));
            assertTrue(logger.isLogging());
            assertTrue(logger.start(file.toFile()));
            SensorCentral.getInstance().grabSensorValues(new byte[]{0, 42, 0, 7, 2}, ini, null);
        } finally {
            logger.stop();
        }
        assertFalse(logger.isLogging());
        assertTrue(Files.exists(file));

        ByteBuffer data = ByteBuffer.wrap(Files.readAllBytes(file)).order(ByteOrder.BIG_ENDIAN);
        assertEquals(3, data.getShort(22));
        data.position(data.getInt(16) + 4);
        assertEquals(42, data.getShort());
        assertEquals(7, data.get());
        assertEquals(1, data.get());
    }
}
