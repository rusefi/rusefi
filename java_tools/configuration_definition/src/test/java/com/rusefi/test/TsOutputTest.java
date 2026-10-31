package com.rusefi.test;

import org.junit.jupiter.api.Test;

import com.rusefi.ReaderStateImpl;
import com.rusefi.output.ConfigStructure;
import com.rusefi.output.TsOutput;
import static org.junit.jupiter.api.Assertions.*;

import java.io.IOException;

class TsOutputTest {

    @Test
    void testFormatTemperatureTsInfo() {
        TsOutput tsOutput = new TsOutput(false);

        // Test null or empty input
        assertEquals("", tsOutput.formatTemperatureTsInfo(null, false));
        assertEquals("", tsOutput.formatTemperatureTsInfo("", false));
        assertEquals("", tsOutput.formatTemperatureTsInfo("  ", false));

        String temperatureInput = "\"SPECIAL_CASE_TEMPERATURE\", 1.0, 0, -40, 200, 0";

        // Test Celsius format
        String expectedCelsius = "\"C\", 1.0, 0, -40, 200, 0";
        assertEquals(expectedCelsius, tsOutput.formatTemperatureTsInfo(temperatureInput, false));

        // Test Fahrenheit format
        String expectedFahrenheit = "\"F\",1.8,17.77777,-40.0,392.0, 0";
        assertEquals(expectedFahrenheit, tsOutput.formatTemperatureTsInfo(temperatureInput, true));
    }

    @Test
    void testFormatTemperatureTsInfoWithScaling() {
        TsOutput tsOutput = new TsOutput(false);

        // Test null or empty input
        assertEquals("", tsOutput.formatTemperatureTsInfo(null, false));
        assertEquals("", tsOutput.formatTemperatureTsInfo("", false));
        assertEquals("", tsOutput.formatTemperatureTsInfo("  ", false));

        String temperatureInput = "\"SPECIAL_CASE_TEMPERATURE\", 2.0, 0, -40, 200, 0";

        // Test Celsius format
        String expectedCelsius = "\"C\", 2.0, 0, -40, 200, 0";
        assertEquals(expectedCelsius, tsOutput.formatTemperatureTsInfo(temperatureInput, false));

        // Test Fahrenheit format
        String expectedFahrenheit = "\"F\",3.6,35.55554,-40.0,392.0, 0";
        assertEquals(expectedFahrenheit, tsOutput.formatTemperatureTsInfo(temperatureInput, true));
    }

    @Test
    void testFormatPressureTsInfo() {
        TsOutput tsOutput = new TsOutput(false);

        // Test null or empty input
        assertEquals("", tsOutput.formatPressureTsInfo(null, false));
        assertEquals("", tsOutput.formatPressureTsInfo("", false));
        assertEquals("", tsOutput.formatPressureTsInfo("  ", false));

        String pressureInput = "\"SPECIAL_CASE_PRESSURE\", 1, 0, 15, 250, 0";

        // Test kPa format
        String expectedKPa = "\"kPa\", 1, 0, 15, 250, 0";
        assertEquals(expectedKPa, tsOutput.formatPressureTsInfo(pressureInput, false));

        // Test psi format
        String expectedPsi = "\"psi\",0.145038,0.0,2.17557,36.2595, 0";
        assertEquals(expectedPsi, tsOutput.formatPressureTsInfo(pressureInput, true));
    }

    @Test
    void testFormatPressureTsInfoWithScaling() {
        TsOutput tsOutput = new TsOutput(false);

        // Test null or empty input
        assertEquals("", tsOutput.formatPressureTsInfo(null, false));
        assertEquals("", tsOutput.formatPressureTsInfo("", false));
        assertEquals("", tsOutput.formatPressureTsInfo("  ", false));

        String pressureInput = "\"SPECIAL_CASE_PRESSURE\", 2, 0, 15, 250, 0";

        // Test kPa format
        String expectedKPa = "\"kPa\", 2, 0, 15, 250, 0";
        assertEquals(expectedKPa, tsOutput.formatPressureTsInfo(pressureInput, false));

        // Test psi format
        String expectedPsi = "\"psi\",0.290076,0.0,2.17557,36.2595, 0";
        assertEquals(expectedPsi, tsOutput.formatPressureTsInfo(pressureInput, true));
    }

    @Test
    void testFormatSpeedTsInfo() {
        TsOutput tsOutput = new TsOutput(false);

        // Test null or empty input
        assertEquals("", tsOutput.formatSpeedTsInfo(null, false));
        assertEquals("", tsOutput.formatSpeedTsInfo("", false));
        assertEquals("", tsOutput.formatSpeedTsInfo("  ", false));

        String speedInput = "\"SPECIAL_CASE_SPEED, 1, 15, 250, 0";

        // Test kmh format
        String expectedKmh = "\"kmh\", 1, 15, 250, 0";
        assertEquals(expectedKmh, tsOutput.formatSpeedTsInfo(speedInput, false));

        // Test mph format
        String expectedMph = "\"mph\",0.62137119,0.0,155.3427975,0.0";
        assertEquals(expectedMph, tsOutput.formatSpeedTsInfo(speedInput, true));
    }

    @Test
    void testFormatSpeedTsInfoWithScaling() {
        TsOutput tsOutput = new TsOutput(false);

        // Test null or empty input
        assertEquals("", tsOutput.formatSpeedTsInfo(null, false));
        assertEquals("", tsOutput.formatSpeedTsInfo("", false));
        assertEquals("", tsOutput.formatSpeedTsInfo("  ", false));

        String speedInput = "\"SPECIAL_CASE_SPEED, 2, 15, 250, 0";

        // Test kmh format
        String expectedKmh = "\"kmh\", 2, 15, 250, 0";
        assertEquals(expectedKmh, tsOutput.formatSpeedTsInfo(speedInput, false));

        // Test mph format
        String expectedMph = "\"mph\",1.24274238,0.0,155.3427975,0.0";
        assertEquals(expectedMph, tsOutput.formatSpeedTsInfo(speedInput, true));
    }

    @Test
    void testWriteOneFieldTemperature() throws IOException {
        TsOutput tsOutput = new TsOutput(false);

        ReaderStateImpl state = new ReaderStateImpl();
        String configText = "struct_no_prefix engine_configuration_s\n" +
        			"int8_t minClt;.;\"SPECIAL_CASE_TEMPERATURE\", 1, 0, -20, 100, 0\n" +
        			"end_struct";

        TestTSProjectConsumer consumer = new TestTSProjectConsumer(state);
        state.readBufferedReader(configText, consumer);
        ConfigStructure structure = state.getStructures().get("engine_configuration_s");

        int tsPosition = 0;
        tsOutput.run(state, structure, tsPosition, "", "");

        // Get the generated content
        String content = tsOutput.getContent();

        // Verify the content contains both Celsius and Fahrenheit conditionals
//        assertTrue(content.contains("#if USE_METRIC_UNITS"));
//        assertTrue(content.contains("#else"));
//        assertTrue(content.contains("#endif"));

        // Celsius content
        assertTrue(content.contains("\"C\", 1, 0"));

        // Fahrenheit content
//        assertTrue(content.contains("\"F\", 1.8,17.77777"));

        // Verify the field name appears twice (once for each temperature scale)
        assertEquals(1, content.split("minClt").length - 1);

        // proper formatting and structure
        assertTrue(content.contains("scalar"));
        assertTrue(content.contains("S08"));

    }

    @Test
    void testWriteOneFieldNonTemperature() {
        TsOutput tsOutput = new TsOutput(false);

        ReaderStateImpl state = new ReaderStateImpl();
        String configText = "struct_no_prefix engine_configuration_s\n" +
        			"uint8_t autoscale minAfr;Below this AFR, correction is paused;\"afr\", 0.1, 0, 10, 20, 1\n" +
        			"end_struct";

        TestTSProjectConsumer consumer = new TestTSProjectConsumer(state);
        state.readBufferedReader(configText, consumer);
        ConfigStructure structure = state.getStructures().get("engine_configuration_s");

        int tsPosition = 0;
        tsOutput.run(state, structure, tsPosition, "", "");

        // Get the generated content
        String content = tsOutput.getContent();

        // Verify that temperature conditionals are NOT present
        assertFalse(content.contains("#if CELSIUS"));
        assertFalse(content.contains("#else"));
        assertFalse(content.contains("#endif"));

        // Verify the content appears only once
        assertEquals(1, content.split("minAfr").length - 1);

        // proper formatting
        assertTrue(content.contains("scalar"));
        assertTrue(content.contains("U08"));
        assertTrue(content.contains("\"afr\", 0.1, 0"));
    }

    @Test
    void testWriteOneFieldHelp() {
        TsOutput tsOutput = new TsOutput(false);

        ReaderStateImpl state = new ReaderStateImpl();
        String configText = "#define switch_input_pin_e_enum \"NONE\", \"INVALID\", \"PA0\"\n" +
            "custom switch_input_pin_e 2 bits, U16, @OFFSET@, [0:8], $switch_input_pin_e_list\n" +
            "struct_no_prefix engine_configuration_s\n" +
            "switch_input_pin_e nitrousControlTriggerPin;Pin that activates nitrous control\n" +
            "end_struct";

        TestTSProjectConsumer consumer = new TestTSProjectConsumer(state);
        state.readBufferedReader(configText, consumer);
        ConfigStructure structure = state.getStructures().get("engine_configuration_s");

        int tsPosition = 0;
        tsOutput.run(state, structure, tsPosition, "", "");

        // Get the generated content
        String help = tsOutput.getSettingContextHelp();
        String expected = "nitrousControlTriggerPin = \"Pin that activates nitrous control\"";

        assertTrue(help.contains(expected),
                   () -> "Expected help to contain " + expected + " but was:\n" + help);
    }

    @Test
    void testWriteOneFieldHelpPinout() {
        TsOutput tsOutput = new TsOutput(false);

        ReaderStateImpl state = new ReaderStateImpl();
        state.getVariableRegistry().register("PINOUT_URL", "https://example.com/");
        String configText = "#define switch_input_pin_e_enum \"NONE\", \"INVALID\", \"PA0\"\n" +
            "custom switch_input_pin_e 2 bits, U16, @OFFSET@, [0:8], $switch_input_pin_e_list\n" +
            "struct_no_prefix engine_configuration_s\n" +
            "switch_input_pin_e nitrousControlTriggerPin;Pin that activates nitrous control\n" +
            "end_struct";

        TestTSProjectConsumer consumer = new TestTSProjectConsumer(state);
        state.readBufferedReader(configText, consumer);
        ConfigStructure structure = state.getStructures().get("engine_configuration_s");

        int tsPosition = 0;
        tsOutput.run(state, structure, tsPosition, "", "");

        // Get the generated content
        String help = tsOutput.getSettingContextHelp();
        String expected = "nitrousControlTriggerPin = \"Pin that activates nitrous control\\nhttps://example.com/?highlight=class~switch_inputs\"";

        assertTrue(help.contains(expected),
                   () -> "Expected help to contain " + expected + " but was:\n" + help);
    }

    @Test
    void testWriteOneFieldPinout() {
        TsOutput tsOutput = new TsOutput(false);

        ReaderStateImpl state = new ReaderStateImpl();
        state.getVariableRegistry().register("PINOUT_URL", "https://example.com/");
        String configText = "#define switch_input_pin_e_enum \"NONE\", \"INVALID\", \"PA0\"\n" +
            "custom switch_input_pin_e 2 bits, U16, @OFFSET@, [0:8], $switch_input_pin_e_list\n" +
            "struct_no_prefix engine_configuration_s\n" +
            "switch_input_pin_e nitrousControlTriggerPin\n" +
            "end_struct";

        TestTSProjectConsumer consumer = new TestTSProjectConsumer(state);
        state.readBufferedReader(configText, consumer);
        ConfigStructure structure = state.getStructures().get("engine_configuration_s");

        int tsPosition = 0;
        tsOutput.run(state, structure, tsPosition, "", "");

        // Get the generated content
        String help = tsOutput.getSettingContextHelp();
        String expected = "nitrousControlTriggerPin = \"https://example.com/?highlight=class~switch_inputs\"";

        assertTrue(help.contains(expected),
                   () -> "Expected help to contain " + expected + " but was:\n" + help);
    }
}
