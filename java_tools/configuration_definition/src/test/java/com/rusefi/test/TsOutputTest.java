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
        String expectedFahrenheit = "\"F\",{ 9 / 5 },17.77777,-40.0,392.0, 0";
        assertEquals(expectedFahrenheit, tsOutput.formatTemperatureTsInfo(temperatureInput, true));
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
        assertTrue(content.contains("#if USE_METRIC_UNITS"));
        assertTrue(content.contains("#else"));
        assertTrue(content.contains("#endif"));

        // Celsius content
        assertTrue(content.contains("\"C\", 1, 0"));
        
        // Fahrenheit content
        assertTrue(content.contains("\"F\", 1.8,17.77777"));
        
        // Verify the field name appears twice (once for each temperature scale)
        assertEquals(2, content.split("minClt").length - 1);
        
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
