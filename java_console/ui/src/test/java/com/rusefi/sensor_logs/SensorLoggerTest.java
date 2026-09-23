package com.rusefi.sensor_logs;

import com.opensr5.ini.IniFileModel;
import com.opensr5.ini.field.EnumIniField;
import com.opensr5.ini.field.IniField;
import com.opensr5.ini.field.ScalarIniField;
import com.rusefi.binaryprotocol.BinaryProtocol;
import com.rusefi.config.FieldType;
import com.rusefi.core.OutputChannelSnapshot;
import com.rusefi.core.SensorCentral;
import com.rusefi.ini.reader.IniFileReaderUtil;
import com.rusefi.ui.UIContext;
import org.junit.jupiter.api.Test;
import org.junit.jupiter.api.io.TempDir;

import java.io.ByteArrayOutputStream;
import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.nio.charset.StandardCharsets;
import java.nio.file.Files;
import java.nio.file.Path;
import java.util.BitSet;
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
    void datalogLabelInBinaryHeader() throws Exception {
        String text = "[MegaTune]\nsignature = test\n[Constants]\npageSize = 0\n"
                + "pageReadCommand = R\nochBlockSize = 2\n"
                + "[OutputChannels]\nRPMValue = scalar, U16, 0, \"RPM\", 1, 0\n"
                + "[Datalog]\nentry = RPMValue, \"RPM\", int, \"%d\"\n";
        Path iniPath = tempDir.resolve("logging.ini");
        Files.writeString(iniPath, text);
        IniFileModel ini = IniFileReaderUtil.readIniFile(iniPath.toString());
        ByteArrayOutputStream output = new ByteArrayOutputStream();
        BinarySensorLog<CustomBinaryLogEntry> log = new BinarySensorLog<>(
                entry -> entry.getValue(new byte[]{0, (byte) 0xd2, 4}),
                SensorLogger.getOutputChannels(ini), output);
        log.writeSensorLogLineChecked();
        log.close();
        ByteBuffer data = ByteBuffer.wrap(output.toByteArray()).order(ByteOrder.BIG_ENDIAN);
        assertEquals(1, data.getShort(22));
        assertEquals("RPM", readFieldName(data, 0));
        assertEquals(1234, data.getShort(data.getInt(16) + 4));
    }

    @Test
    void datalogOrderKeepsLabelsAttachedToValuesAndUnlistedChannels() throws Exception {
        String text = "[MegaTune]\nsignature = test\n[Constants]\npageSize = 0\n"
                + "pageReadCommand = R\nochBlockSize = 6\n"
                + "[OutputChannels]\nMAPValue = scalar, U16, 0, \"kPa\", 0.1, 0\n"
                + "coolant = scalar, S16, 2, \"deg C\", 0.01, 0\n"
                + "isCltError = bits, U08, 4, [2:2]\n"
                + "rawClt = scalar, U08, 5, \"V\", 0.1, 0\n"
                + "computed = { coolant * 2 }\n"
                + "[Datalog]\nentry = time, \"Time\", float, \"%.3f\"\n"
                + "entry = COOLANT, \"CLT\", float, \"%.2f\"\n"
                + "entry = isCltError, \"Error: CLT\", int, \"%d\"\n"
                + "entry = computed, \"Computed\", float, \"%.2f\"\n"
                + "entry = mapvalue, \"MAP\", float, \"%.1f\"\n";
        Path iniPath = tempDir.resolve("ordered.ini");
        Files.writeString(iniPath, text);
        IniFileModel ini = IniFileReaderUtil.readIniFile(iniPath.toString());
        byte[] response = {0, (byte) 0xe8, 3, (byte) 0x85, (byte) 0xff, 4, 25};
        ByteArrayOutputStream output = new ByteArrayOutputStream();
        BinarySensorLog<CustomBinaryLogEntry> log = new BinarySensorLog<>(
                entry -> entry.getValue(response), SensorLogger.getOutputChannels(ini), output);
        log.writeSensorLogLineChecked();
        log.close();
        ByteBuffer data = ByteBuffer.wrap(output.toByteArray()).order(ByteOrder.BIG_ENDIAN);
        assertEquals(4, data.getShort(22), "No duplicate MAP alias or unsupported computed channels");
        assertEquals("CLT", readFieldName(data, 0));
        assertEquals("Error: CLT", readFieldName(data, 1));
        assertEquals("MAP", readFieldName(data, 2));
        assertEquals("rawClt", readFieldName(data, 3), "Unlisted channels retain their original case");
        assertEquals(0.01f, data.getFloat(24 + 46));
        assertEquals(0.1f, data.getFloat(24 + 2 * 89 + 46));
        data.position(data.getInt(16) + 4);
        assertEquals(-123, data.getShort());
        assertEquals(1, data.get());
        assertEquals(1000, data.getShort());
        assertEquals(25, data.get());
        assertEquals(1, data.remaining());
    }

    @Test
    void startsAndStopsExplicitly() throws Exception {
        UIContext context = mock(UIContext.class);
        BinaryProtocol protocol = mock(BinaryProtocol.class);
        IniFileModel ini = mock(IniFileModel.class);
        ScalarIniField rpm = new ScalarIniField("rpm", 0, "RPM", FieldType.UINT16, 1, "0", 0);
        ScalarIniField notGauge = new ScalarIniField("notGauge", 2, "V", FieldType.UINT8, 0.1, "1", 0);
        EnumIniField flag = new EnumIniField("flag", 3, FieldType.UINT8,
                new EnumIniField.EnumKeyValueMap(Collections.emptyMap()), 1, 0);
        ScalarIniField map = new ScalarIniField("MAPValue", 4, "kPa", FieldType.UINT16, 0.1, "1", 0);
        Map<String, IniField> outputChannels = new LinkedHashMap<>();
        outputChannels.put("rpm", rpm);
        outputChannels.put("notGauge", notGauge);
        outputChannels.put("flag", flag);
        outputChannels.put("MAPValue", map);

        when(context.getBinaryProtocol()).thenReturn(protocol);
        when(protocol.getIniFileNullable()).thenReturn(ini);
        when(ini.getAllOutputChannels()).thenReturn(outputChannels);
        when(ini.getGauges()).thenReturn(Collections.emptyMap());
        when(ini.getOutputChannel("rpm")).thenReturn(rpm);
        when(ini.getOutputChannel("notGauge")).thenReturn(notGauge);
        when(ini.getOutputChannel("flag")).thenReturn(flag);
        when(ini.getOutputChannel("MAPValue")).thenReturn(map);

        SensorLogger logger = new SensorLogger(context);
        Path file = tempDir.resolve("selected.mlg");
        assertFalse(logger.isLogging());
        try {
            assertTrue(logger.start(file.toFile()));
            assertTrue(logger.isLogging());
            assertTrue(logger.start(file.toFile()));
            SensorCentral sensorCentral = SensorCentral.getInstance();
            long generation = sensorCentral.getOutputChannelDemand().getGeneration();
            assertTrue(sensorCentral.getOutputChannelDemand().isFull());

            byte[] response = {0, 42, 0, 7, 2, -46, 4};
            BitSet valid = new BitSet(response.length - 1);
            valid.set(0, response.length - 1);
            sensorCentral.grabSensorValues(new OutputChannelSnapshot(
                response, valid, Collections.emptySet(), generation - 1, true), ini, null);
            sensorCentral.grabSensorValues(new OutputChannelSnapshot(
                response, valid, Collections.singleton("rpm"), generation, false), ini, null);
            sensorCentral.grabSensorValues(new OutputChannelSnapshot(
                response, valid, Collections.emptySet(), generation, true), ini, null);
        } finally {
            logger.stop();
        }
        assertFalse(logger.isLogging());
        assertFalse(SensorCentral.getInstance().getOutputChannelDemand().isFull());
        assertTrue(Files.exists(file));

        ByteBuffer data = ByteBuffer.wrap(Files.readAllBytes(file)).order(ByteOrder.BIG_ENDIAN);
        assertEquals(5, data.getShort(22));
        assertEquals("MAP", readFieldName(data, 4));
        data.position(data.getInt(16) + 4);
        assertEquals(42, data.getShort());
        assertEquals(7, data.get());
        assertEquals(1, data.get());
        assertEquals(1234, data.getShort());
        assertEquals(1234, data.getShort());
        assertEquals(1, data.remaining(), "One checksum byte should follow the only log row");
        data.get();
        assertFalse(data.hasRemaining(), "Only the generation-matched full snapshot should be logged");
    }

    private static String readFieldName(ByteBuffer data, int index) {
        byte[] name = new byte[34];
        data.position(24 + index * 89 + 1);
        data.get(name);
        return new String(name, StandardCharsets.US_ASCII).trim();
    }
}
