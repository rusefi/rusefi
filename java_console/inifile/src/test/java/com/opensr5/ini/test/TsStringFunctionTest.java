package com.opensr5.ini.test;

import com.opensr5.ConfigurationImage;
import com.opensr5.ini.GaugeModel;
import com.opensr5.ini.IniFileModel;
import com.opensr5.ini.RawIniFile;
import com.opensr5.ini.TsStringFunction;
import com.opensr5.ini.field.EnumIniField;
import com.opensr5.ini.field.IniField;
import com.opensr5.ini.field.StringIniField;
import com.rusefi.config.FieldType;
import com.rusefi.ini.reader.IniFileReaderUtil;
import org.junit.jupiter.api.Test;

import java.io.ByteArrayInputStream;
import java.nio.charset.StandardCharsets;
import java.util.HashMap;
import java.util.Map;
import java.util.Optional;
import java.util.TreeMap;

import static com.opensr5.ini.test.IniFileReaderTest.readLines;
import static org.junit.jupiter.api.Assertions.*;
import static org.mockito.Mockito.*;

public class TsStringFunctionTest {

    // ========== containsStringFunction() ==========

    @Test
    public void testContainsStringFunction_bitStringValue() {
        assertTrue(TsStringFunction.containsStringFunction("bitStringValue(debugFieldF1List, debugMode)"));
    }

    @Test
    public void testContainsStringFunction_bitStringValue_withExtraSpaces() {
        // Real INI format has extra spaces
        assertTrue(TsStringFunction.containsStringFunction("{bitStringValue( debugFieldF1List,  debugMode )}"));
    }

    @Test
    public void testContainsStringFunction_stringValue() {
        assertTrue(TsStringFunction.containsStringFunction("{ stringValue(gpPwmNote1) }"));
    }

    @Test
    public void testContainsStringFunction_plainExpression() {
        assertFalse(TsStringFunction.containsStringFunction("{ coolant * 1.8 + 32 }"));
    }

    @Test
    public void testContainsStringFunction_null() {
        assertFalse(TsStringFunction.containsStringFunction(null));
    }

    @Test
    public void testContainsStringFunction_plainString() {
        assertFalse(TsStringFunction.containsStringFunction("RPM"));
    }

    // ========== extractBitStringValueExpression() ==========

    @Test
    public void testExtractBitStringValueExpression_simple() {
        assertEquals("debugMode", TsStringFunction.extractBitStringValueExpression(
            "bitStringValue(debugFieldF1List, debugMode)"));
    }

    @Test
    public void testExtractBitStringValueExpression_withBraces() {
        assertEquals("debugMode", TsStringFunction.extractBitStringValueExpression(
            "{ bitStringValue(debugFieldF1List, debugMode) }"));
    }

    @Test
    public void testExtractBitStringValueExpression_notBitStringValue() {
        assertNull(TsStringFunction.extractBitStringValueExpression("{ stringValue(gpPwmNote1) }"));
    }

    @Test
    public void testExtractBitStringValueExpression_null() {
        assertNull(TsStringFunction.extractBitStringValueExpression(null));
    }

    // ========== resolve() - bitStringValue ==========

    @Test
    public void testResolve_bitStringValue_withNumericLiteral() {
        EnumIniField enumField = createMockEnumField("debugFieldF1List", buildEnumMap("Off", "Alpha", "Beta", "Gamma"));
        IniFileModel ini = createMockIni(enumField);

        Map<String, Double> values = new HashMap<>();
        // Index expression is a simple number — use output channel for it
        values.put("debugMode", 2.0);

        String result = TsStringFunction.resolve("bitStringValue(debugFieldF1List, debugMode)", ini, null, values);
        assertEquals("Beta", result);
    }

    @Test
    public void testResolve_bitStringValue_indexZero() {
        EnumIniField enumField = createMockEnumField("myEnum", buildEnumMap("First", "Second", "Third"));
        IniFileModel ini = createMockIni(enumField);

        Map<String, Double> values = new HashMap<>();
        values.put("idx", 0.0);

        String result = TsStringFunction.resolve("{ bitStringValue(myEnum, idx) }", ini, null, values);
        assertEquals("First", result);
    }

    @Test
    public void testResolve_bitStringValue_unknownField() {
        IniFileModel ini = mock(IniFileModel.class);
        when(ini.findIniField("noSuchEnum")).thenReturn(Optional.empty());

        Map<String, Double> values = new HashMap<>();
        values.put("idx", 0.0);

        String result = TsStringFunction.resolve("bitStringValue(noSuchEnum, idx)", ini, null, values);
        assertNull(result);
    }

    @Test
    public void testResolve_bitStringValue_outOfRangeIndex() {
        EnumIniField enumField = createMockEnumField("myEnum", buildEnumMap("A", "B"));
        IniFileModel ini = createMockIni(enumField);

        Map<String, Double> values = new HashMap<>();
        values.put("idx", 99.0);

        String result = TsStringFunction.resolve("bitStringValue(myEnum, idx)", ini, null, values);
        assertNull(result);
    }

    @Test
    public void testResolve_bitStringValue_missingVariable() {
        EnumIniField enumField = createMockEnumField("myEnum", buildEnumMap("A", "B"));
        IniFileModel ini = createMockIni(enumField);

        Map<String, Double> values = new HashMap<>();
        // "idx" is not in the map

        String result = TsStringFunction.resolve("bitStringValue(myEnum, idx)", ini, null, values);
        assertNull(result);
    }

    // ========== resolve() - stringValue ==========

    @Test
    public void testResolve_stringValue() {
        StringIniField stringField = new StringIniField("gpPwmNote1", 100, 16);
        IniFileModel ini = createMockIni(stringField);

        byte[] configContent = new byte[200];
        byte[] noteBytes = "My Custom PWM".getBytes(StandardCharsets.US_ASCII);
        System.arraycopy(noteBytes, 0, configContent, 100, noteBytes.length);
        ConfigurationImage configImage = new ConfigurationImage(configContent);

        String result = TsStringFunction.resolve("{ stringValue(gpPwmNote1) }", ini, configImage, null);
        assertEquals("My Custom PWM", result);
    }

    @Test
    public void testResolve_stringValue_withNulls() {
        StringIniField stringField = new StringIniField("note", 0, 10);
        IniFileModel ini = createMockIni(stringField);

        byte[] configContent = new byte[20];
        configContent[0] = 'H';
        configContent[1] = 'i';
        // rest is null bytes
        ConfigurationImage configImage = new ConfigurationImage(configContent);

        String result = TsStringFunction.resolve("stringValue(note)", ini, configImage, null);
        assertEquals("Hi", result);
    }

    @Test
    public void testResolve_stringValue_noConfigImage() {
        StringIniField stringField = new StringIniField("note", 0, 10);
        IniFileModel ini = createMockIni(stringField);

        String result = TsStringFunction.resolve("stringValue(note)", ini, null, null);
        assertNull(result);
    }

    @Test
    public void testResolve_stringValue_unknownField() {
        IniFileModel ini = mock(IniFileModel.class);
        when(ini.findIniField("noSuchField")).thenReturn(Optional.empty());

        ConfigurationImage configImage = new ConfigurationImage(new byte[100]);

        String result = TsStringFunction.resolve("stringValue(noSuchField)", ini, configImage, null);
        assertNull(result);
    }

    // ========== resolve() - bitStringValue with config image (real-world debugMode scenario) ==========

    @Test
    public void testResolve_bitStringValue_fromConfigImage() {
        // Simulate the real-world case: debugMode is a U08 enum field in config, not an output channel.
        // bitStringValue(debugFieldF1List, debugMode) should read debugMode from config image.
        EnumIniField enumListField = createMockEnumField("debugFieldF1List",
            buildEnumMap("INVALID", "TPS accel", "INVALID", "Stepper Idle"));

        // debugMode is a real EnumIniField at config offset 10, U08, bits [0:5]
        EnumIniField debugModeField = new EnumIniField("debugMode", 10, FieldType.UINT8,
            buildEnumMap("INVALID", "TPS accel", "INVALID", "Stepper Idle"),
            0, 5); // 6-bit field (bitSize0=5 means 6 bits)

        IniFileModel ini = mock(IniFileModel.class);
        when(ini.findIniField("debugFieldF1List")).thenReturn(Optional.of(enumListField));
        when(ini.findIniField("debugMode")).thenReturn(Optional.of(debugModeField));

        // Create config image with debugMode=3 at offset 10
        byte[] configContent = new byte[100];
        configContent[10] = 3; // debugMode value
        ConfigurationImage configImage = new ConfigurationImage(configContent);

        // No output channel values — debugMode must come from config
        String result = TsStringFunction.resolve(
            "{ bitStringValue(debugFieldF1List, debugMode) }", ini, configImage, new HashMap<>());
        assertEquals("Stepper Idle", result);
    }

    @Test
    public void testResolve_bitStringValue_u32BitField() {
        // Real-world: unitsLabels is a PcVariable enum, useMetricOnInterface is a U32 bit field at bit 29
        EnumIniField unitsLabels = createMockEnumField("unitsLabels", buildEnumMap("F", "C"));

        // useMetricOnInterface = bits, U32, 772, [29:29]
        EnumIniField useMetricField = new EnumIniField("useMetricOnInterface", 772, FieldType.INT,
            buildEnumMap("Imperial", "Metric"), 29, 0); // bitPosition=29, bitSize0=0 (1-bit field)

        IniFileModel ini = mock(IniFileModel.class);
        when(ini.findIniField("unitsLabels")).thenReturn(Optional.of(unitsLabels));
        when(ini.findIniField("useMetricOnInterface")).thenReturn(Optional.of(useMetricField));

        // Create config image with useMetricOnInterface=1 (bit 29 set) at offset 772
        byte[] configContent = new byte[800];
        // Bit 29 in a 32-bit LE value: 0x20000000
        // In LE bytes at offset 772: [0x00, 0x00, 0x00, 0x20]
        configContent[772] = 0x00;
        configContent[773] = 0x00;
        configContent[774] = 0x00;
        configContent[775] = 0x20;
        ConfigurationImage configImage = new ConfigurationImage(configContent);

        String result = TsStringFunction.resolve(
            "{bitStringValue(unitsLabels, useMetricOnInterface)}", ini, configImage, new HashMap<>());
        assertEquals("C", result, "useMetricOnInterface=1 (Metric) should resolve to 'C'");
    }

    @Test
    public void testResolve_bitStringValue_u32BitField_imperial() {
        // Same as above but with useMetricOnInterface=0 (Imperial)
        EnumIniField unitsLabels = createMockEnumField("unitsLabels", buildEnumMap("F", "C"));

        EnumIniField useMetricField = new EnumIniField("useMetricOnInterface", 772, FieldType.INT,
            buildEnumMap("Imperial", "Metric"), 29, 0);

        IniFileModel ini = mock(IniFileModel.class);
        when(ini.findIniField("unitsLabels")).thenReturn(Optional.of(unitsLabels));
        when(ini.findIniField("useMetricOnInterface")).thenReturn(Optional.of(useMetricField));

        // Config image with bit 29 NOT set (useMetricOnInterface=0)
        byte[] configContent = new byte[800];
        // All zeros at offset 772 — bit 29 is 0
        ConfigurationImage configImage = new ConfigurationImage(configContent);

        String result = TsStringFunction.resolve(
            "{bitStringValue(unitsLabels, useMetricOnInterface)}", ini, configImage, new HashMap<>());
        assertEquals("F", result, "useMetricOnInterface=0 (Imperial) should resolve to 'F'");
    }

    @Test
    public void testResolve_bitStringValue_outputChannelOverridesConfig() {
        // If debugMode is in output channels, that should take priority over config
        EnumIniField enumField = createMockEnumField("myList", buildEnumMap("A", "B", "C"));

        IniFileModel ini = createMockIni(enumField);

        Map<String, Double> values = new HashMap<>();
        values.put("idx", 1.0);

        // Even with a config image, output channel value should be used
        byte[] configContent = new byte[100];
        ConfigurationImage configImage = new ConfigurationImage(configContent);

        String result = TsStringFunction.resolve("bitStringValue(myList, idx)", ini, configImage, values);
        assertEquals("B", result);
    }

    // ========== resolve() - mixed text ==========

    @Test
    public void testResolve_mixedText() {
        EnumIniField enumField = createMockEnumField("reasonList", buildEnumMap("None", "Too Cold", "Too Hot"));
        IniFileModel ini = createMockIni(enumField);

        Map<String, Double> values = new HashMap<>();
        values.put("reason", 2.0);

        String result = TsStringFunction.resolve("Error: bitStringValue(reasonList, reason)", ini, null, values);
        assertEquals("Error: Too Hot", result);
    }

    // ========== Tokenization verification ==========

    @Test
    public void testTokenization_bitStringValueInGaugeLine() {
        // Verify that splitTokens correctly preserves the brace-enclosed expression
        String gaugeLine = "debugF1Gauge = debugFloatField1, {bitStringValue( debugFieldF1List,  debugMode )}, \"\", 0, 100, 0, 0, 100, 100, 4, 4";
        String[] tokens = IniFileReaderUtil.splitTokens(gaugeLine);
        // Token 0: debugF1Gauge
        assertEquals("debugF1Gauge", tokens[0]);
        // Token 1: debugFloatField1
        assertEquals("debugFloatField1", tokens[1]);
        // Token 2: the full expression preserved (braces protect the inner comma)
        assertEquals("{bitStringValue( debugFieldF1List,  debugMode )}", tokens[2]);
    }

    // ========== Integration test: full INI parsing + resolution ==========

    @Test
    public void testResolve_bitStringValue_fullIniParsing() {
        // Simulate the real INI structure for debugF1Gauge
        String iniContent = "[Constants]\n" +
            "page = 1\n" +
            "debugMode = bits, U08, 10, [0:5], \"INVALID\", \"TPS accel\", \"INVALID\", \"Stepper Idle\", \"Engine Load\"\n" +
            "debugFieldF1List = bits, U08, 20, [0:7], \"Alt Output\", \"From TPS\", \"unused\", \"Idle Output\", \"Load Value\"\n" +
            "[GaugeConfigurations]\n" +
            "gaugeCategory = Debug\n" +
            "debugF1Gauge = debugFloatField1, {bitStringValue( debugFieldF1List,  debugMode )}, \"\", 0, 100, 0, 0, 100, 100, 4, 4\n";

        RawIniFile lines = IniFileReaderUtil.read(new ByteArrayInputStream(iniContent.getBytes()));
        IniFileModel model = readLines(lines);

        // Verify gauge was parsed with expression title
        GaugeModel gauge = model.getGauge("debugF1Gauge");
        assertNotNull(gauge, "debugF1Gauge should exist");
        assertTrue(gauge.getTitleValue().isExpression(), "Title should be an expression");
        assertTrue(TsStringFunction.containsStringFunction(gauge.getTitle()), "Title should contain bitStringValue");

        // Verify the fields are findable
        assertTrue(model.findIniField("debugFieldF1List").isPresent(), "debugFieldF1List should be found");
        assertTrue(model.findIniField("debugMode").isPresent(), "debugMode should be found");
        assertInstanceOf(EnumIniField.class, model.findIniField("debugFieldF1List").get());
        assertInstanceOf(EnumIniField.class, model.findIniField("debugMode").get());

        // Create config image with debugMode=3 at offset 10
        byte[] configContent = new byte[100];
        configContent[10] = 3; // debugMode = "Stepper Idle" (index 3)
        ConfigurationImage configImage = new ConfigurationImage(configContent);

        // Resolve the title
        String resolved = TsStringFunction.resolve(gauge.getTitle(), model, configImage, new HashMap<>());
        assertEquals("Idle Output", resolved, "bitStringValue should resolve using debugMode=3 to get index 3 from debugFieldF1List");
    }

    @Test
    public void testResolve_stringValue_fullIniParsing() {
        String iniContent = "[Constants]\n" +
            "page = 1\n" +
            "gpPwmNote1 = string, ASCII, 50, 16\n" +
            "[GaugeConfigurations]\n" +
            "gaugeCategory = GPPWM\n" +
            "gppwmGauge1 = gppwmOutput1, { stringValue(gpPwmNote1) }, \"\", 0, 100, 0, 0, 100, 100, 0, 0\n";

        RawIniFile lines = IniFileReaderUtil.read(new ByteArrayInputStream(iniContent.getBytes()));
        IniFileModel model = readLines(lines);

        GaugeModel gauge = model.getGauge("gppwmGauge1");
        assertNotNull(gauge);
        assertTrue(gauge.getTitleValue().isExpression());

        // Config image with "My PWM" at offset 50
        byte[] configContent = new byte[100];
        byte[] note = "My PWM".getBytes(StandardCharsets.US_ASCII);
        System.arraycopy(note, 0, configContent, 50, note.length);
        ConfigurationImage configImage = new ConfigurationImage(configContent);

        String resolved = TsStringFunction.resolve(gauge.getTitle(), model, configImage, null);
        assertEquals("My PWM", resolved);
    }

    @Test
    public void testResolve_bitStringValue_unitsLabels_metric() {
        // Real-world CLTGauge scenario: units = {bitStringValue(unitsLabels, useMetricOnInterface)}
        // unitsLabels is in [PcVariables], useMetricOnInterface is a U32 bit field in [Constants]
        String iniContent = "[Constants]\n" +
            "page = 1\n" +
            "useMetricOnInterface = bits, U32, 772, [29:29], \"Imperial\", \"Metric\"\n" +
            "[PcVariables]\n" +
            "unitsLabels = bits, U08, [0:2], \"F\", \"C\"\n" +
            "[GaugeConfigurations]\n" +
            "gaugeCategory = Temperatures\n" +
            "CLTGauge = coolantTemperature, \"Coolant temp\", {bitStringValue(unitsLabels, useMetricOnInterface)}, -40, 140, -15, 1, 95, 110, 1, 1\n";

        RawIniFile lines = IniFileReaderUtil.read(new ByteArrayInputStream(iniContent.getBytes()));
        IniFileModel model = readLines(lines);

        // Verify fields are findable
        assertTrue(model.findIniField("unitsLabels").isPresent(), "unitsLabels from [PcVariables] should be found");
        assertTrue(model.findIniField("useMetricOnInterface").isPresent(), "useMetricOnInterface from [Constants] should be found");

        // Create config image with useMetricOnInterface=1 (Metric: bit 29 set)
        byte[] configContent = new byte[800];
        // Bit 29 in LE: byte at offset 775 = 0x20
        configContent[775] = 0x20;
        ConfigurationImage configImage = new ConfigurationImage(configContent);

        // Resolve gauge units
        GaugeModel gauge = model.getGauge("CLTGauge");
        assertNotNull(gauge);
        assertTrue(gauge.getUnitsValue().isExpression());

        String resolvedUnits = TsStringFunction.resolve(gauge.getUnits(), model, configImage, new HashMap<>());
        assertEquals("C", resolvedUnits, "Metric config should resolve units to 'C'");
    }

    @Test
    public void testResolve_bitStringValue_unitsLabels_imperial() {
        String iniContent = "[Constants]\n" +
            "page = 1\n" +
            "useMetricOnInterface = bits, U32, 772, [29:29], \"Imperial\", \"Metric\"\n" +
            "[PcVariables]\n" +
            "unitsLabels = bits, U08, [0:2], \"F\", \"C\"\n" +
            "[GaugeConfigurations]\n" +
            "gaugeCategory = Temperatures\n" +
            "CLTGauge = coolantTemperature, \"Coolant temp\", {bitStringValue(unitsLabels, useMetricOnInterface)}, -40, 140, -15, 1, 95, 110, 1, 1\n";

        RawIniFile lines = IniFileReaderUtil.read(new ByteArrayInputStream(iniContent.getBytes()));
        IniFileModel model = readLines(lines);

        // Config with useMetricOnInterface=0 (Imperial: bit 29 NOT set)
        byte[] configContent = new byte[800];
        ConfigurationImage configImage = new ConfigurationImage(configContent);

        GaugeModel gauge = model.getGauge("CLTGauge");
        String resolvedUnits = TsStringFunction.resolve(gauge.getUnits(), model, configImage, new HashMap<>());
        assertEquals("F", resolvedUnits, "Imperial config should resolve units to 'F'");
    }

    @Test
    public void testResolve_bitStringValue_pcVariablesSection() {
        // Real-world scenario: debugFieldF1List is in [PcVariables] (no offset), debugMode is in [Constants]
        String iniContent = "[Constants]\n" +
            "page = 1\n" +
            "debugMode = bits, U08, 10, [0:5], \"INVALID\", \"TPS accel\", \"INVALID\", \"Stepper Idle\", \"Engine Load\"\n" +
            "[PcVariables]\n" +
            "debugFieldF1List = bits, U08, [0:7], \"Alt Output\", \"From TPS\", \"unused\", \"Idle Output\", \"Load Value\"\n" +
            "[GaugeConfigurations]\n" +
            "gaugeCategory = Debug\n" +
            "debugF1Gauge = debugFloatField1, {bitStringValue( debugFieldF1List,  debugMode )}, \"\", 0, 100, 0, 0, 100, 100, 4, 4\n";

        RawIniFile lines = IniFileReaderUtil.read(new ByteArrayInputStream(iniContent.getBytes()));
        IniFileModel model = readLines(lines);

        // Verify debugFieldF1List from [PcVariables] is findable
        assertTrue(model.findIniField("debugFieldF1List").isPresent(), "debugFieldF1List from [PcVariables] should be found");
        assertInstanceOf(EnumIniField.class, model.findIniField("debugFieldF1List").get());

        // Verify the enum values were parsed correctly
        EnumIniField enumField = (EnumIniField) model.findIniField("debugFieldF1List").get();
        assertEquals("Alt Output", enumField.getEnums().get(0));
        assertEquals("From TPS", enumField.getEnums().get(1));
        assertEquals("Idle Output", enumField.getEnums().get(3));

        // Create config image with debugMode=3 at offset 10
        byte[] configContent = new byte[100];
        configContent[10] = 3; // debugMode = "Stepper Idle" (index 3)
        ConfigurationImage configImage = new ConfigurationImage(configContent);

        // Resolve the gauge title
        GaugeModel gauge = model.getGauge("debugF1Gauge");
        assertNotNull(gauge);
        String resolved = TsStringFunction.resolve(gauge.getTitle(), model, configImage, new HashMap<>());
        assertEquals("Idle Output", resolved, "bitStringValue should use debugMode=3 to index into debugFieldF1List from [PcVariables]");
    }

    // ========== Helper methods ==========

    private static EnumIniField createMockEnumField(String name, EnumIniField.EnumKeyValueMap enumMap) {
        EnumIniField field = mock(EnumIniField.class);
        when(field.getName()).thenReturn(name);
        when(field.getEnums()).thenReturn(enumMap);
        return field;
    }

    private static EnumIniField.EnumKeyValueMap buildEnumMap(String... values) {
        Map<Integer, String> map = new TreeMap<>();
        for (int i = 0; i < values.length; i++) {
            map.put(i, values[i]);
        }
        return new EnumIniField.EnumKeyValueMap(map);
    }

    private static IniFileModel createMockIni(IniField field) {
        IniFileModel ini = mock(IniFileModel.class);
        when(ini.findIniField(field.getName())).thenReturn(Optional.of(field));
        return ini;
    }
}
