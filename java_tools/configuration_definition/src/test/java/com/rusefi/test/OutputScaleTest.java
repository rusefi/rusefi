package com.rusefi.test;

import com.rusefi.ReaderStateImpl;
import com.rusefi.output.GetOutputValueConsumer;
import com.rusefi.output.OutputsSectionConsumer;
import com.rusefi.output.SdCardFieldsContent;
import com.rusefi.util.LazyFile;
import org.junit.jupiter.api.Test;

import static org.junit.jupiter.api.Assertions.*;

class OutputScaleTest {
    private static final String MASS = "float mass;Fuel mass;\"mg\", 1000, 0, -20, 20, 3\n";

    @Test
    void defaultConsumersApplyIniConversion() {
        ReaderStateImpl state = new ReaderStateImpl();
        GetOutputValueConsumer lua = new GetOutputValueConsumer(null, LazyFile.REAL);
        SdCardFieldsContent sd = new SdCardFieldsContent();
        state.readBufferedReader("struct_no_prefix state_s\n"
                + "float trim;Trim;\"%\", 100, -1, -100, 100, 1\nend_struct\n", lua, sd::handleEndStruct);
        // Ordinary fragments use the declared conversion without extra configuration.
        assertTrue(lua.getContent().contains("return (engine->outputChannels.trim + -1.0f) * 100.0f;"), lua.getContent());
        assertTrue(sd.getBody().contains("{MLG_SOURCE(test->reference.trim), \"Trim\", \"%\", 1, \"none\", 100.0f, -1.0f}"), sd.getBody());
    }

    @Test
    void floatScaleReachesFirmwareConsumers() {
        String[] output = generate(MASS);
        assertTrue(output[0].contains("mass = scalar, F32, 0, \"mg\", 1000.0, 0"), output[0]);
        assertTrue(output[1].contains("return engine->outputChannels.mass * 1000.0f;"), output[1]);
        assertTrue(output[2].contains("{MLG_SOURCE(test->reference.mass), \"Fuel mass\", \"mg\", 3, \"none\", 1000.0f}"), output[2]);
    }

    @Test
    void packedScaleIsNotAppliedTwice() {
        String[] output = generate("int16_t autoscale packed;Packed mass;\"mg\", 0.01, 0, -20, 20, 2\n");
        assertTrue(output[0].contains("\"mg\", 0.01, 0"), output[0]);
        assertTrue(output[1].contains("return engine->outputChannels.packed;"), output[1]);
        assertTrue(output[2].contains("{MLG_SOURCE(test->reference.packed), \"Packed mass\", \"mg\", 2}"), output[2]);
    }

    @Test
    void fractionalScaleAndCategory() {
        String[] output = generate("float elapsed;Elapsed;\"ms\", {1/1000}, 0, 0, 100, 3, \"Timing\"\n");
        assertTrue(output[1].contains("return engine->outputChannels.elapsed * 0.001f;"), output[1]);
        assertTrue(output[2].contains("{MLG_SOURCE(test->reference.elapsed), \"Elapsed\", \"ms\", 3, \"Timing\", 0.001f}"), output[2]);
    }

    @Test
    void nonzeroOffsetsReachFirmwareConsumers() {
        // [tag:live_data_output_scale] SD uses raw offsets; Lua offsets account for C++ unpacking.
        String[] trim = generate("float trim;Trim;\"%\", 100, -1, -100, 100, 1\n");
        assertTrue(trim[0].contains("trim = scalar, F32, 0, \"%\", 100.0, -1"), trim[0]);
        assertTrue(trim[1].contains("return (engine->outputChannels.trim + -1.0f) * 100.0f;"), trim[1]);
        assertTrue(trim[2].contains("{MLG_SOURCE(test->reference.trim), \"Trim\", \"%\", 1, \"none\", 100.0f, -1.0f}"), trim[2]);

        String[] packed = generate("int16_t autoscale packed;Packed;\"unit\", 0.1, 100, -100, 100, 1\n");
        assertTrue(packed[0].contains("\"unit\", 0.1, 100"), packed[0]);
        assertTrue(packed[1].contains("return (engine->outputChannels.packed + 10.0f);"), packed[1]);
        assertTrue(packed[2].contains("{MLG_SOURCE(test->reference.packed), \"Packed\", \"unit\", 1, \"none\", 1.0f, 100.0f}"), packed[2]);
    }

    @Test
    void offsetWithoutScale() {
        String field = "int16_t temperature;Temperature;\"C\", 1, {-5/2}, -100, 100, 1, \"Sensors\"\n";
        String[] converted = generate(field);
        assertTrue(converted[1].contains("return (engine->outputChannels.temperature + -2.5f);"), converted[1]);
        assertTrue(converted[2].contains("MLG_SOURCE(test->reference.temperature), \"Temperature\", \"C\", 1, \"Sensors\", 1.0f, -2.5f}"), converted[2]);
    }

    @Test
    void unsupportedTransformsFailGeneration() {
        assertThrows(IllegalArgumentException.class, () ->
                generate("float mass;Mass;\"mg\", {useMg ? 1000 : 1}, 0, 0, 100, 1\n"));
        assertThrows(IllegalArgumentException.class, () ->
                generate("int16_t autoscale mass;Mass;\"mg\", {useMg ? 1000 : 1}, 1, 0, 100, 1\n"));
        assertThrows(IllegalArgumentException.class, () ->
                generate("float temperature;Temperature;\"C\", 1, {useC ? 0 : 32}, 0, 100, 1\n"));
        for (String value : new String[]{"NaN", "Infinity", "{1/0}", "1e100"}) {
            assertThrows(IllegalArgumentException.class, () ->
                    generate("float value;Value;\"\", 1, " + value + ", 0, 100, 1\n"));
        }
    }

    private static String[] generate(String fields) {
        ReaderStateImpl state = new ReaderStateImpl();
        GetOutputValueConsumer lua = new GetOutputValueConsumer(null, LazyFile.REAL);
        SdCardFieldsContent sd = new SdCardFieldsContent();
        OutputsSectionConsumer ts = new OutputsSectionConsumer(null, LazyFile.REAL);
        state.readBufferedReader("struct_no_prefix state_s\n" + fields + "end_struct\n",
                lua, sd::handleEndStruct, ts);
        return new String[]{ts.getContent(), lua.getContent(), sd.getBody()};
    }
}
