package com.rusefi.test;

import org.junit.jupiter.api.Test;

import com.rusefi.ReaderStateImpl;
import com.rusefi.output.ConfigStructure;
import com.rusefi.output.TsOutput;
import static org.junit.jupiter.api.Assertions.*;

class TsOutputTest {

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
