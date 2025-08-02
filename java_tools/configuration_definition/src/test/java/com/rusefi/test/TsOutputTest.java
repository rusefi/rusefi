package com.rusefi.test;

import org.junit.jupiter.api.Test;

import com.rusefi.ReaderStateImpl;
import com.rusefi.output.ConfigStructure;
import com.rusefi.output.TsOutput;
import static org.junit.jupiter.api.Assertions.*;

import java.io.IOException;

class TsOutputTest {
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

        // Celsius content
        assertTrue(content.contains("\"C\", 1, 0"));

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
}
