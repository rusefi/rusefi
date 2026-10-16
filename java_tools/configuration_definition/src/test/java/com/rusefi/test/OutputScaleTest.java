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
    void floatScaleReachesFirmwareConsumers() {
        String[] output = generate(MASS, true);
        assertTrue(output[0].contains("mass = scalar, F32, 0, \"mg\", 1000.0, 0"), output[0]);
        assertTrue(output[1].contains("return engine->outputChannels.mass * 1000.0f;"), output[1]);
        assertTrue(output[2].contains("{test->reference.mass, \"Fuel mass\", \"mg\", 3, \"none\", 1000.0f}"), output[2]);
    }

    @Test
    void legacyFragmentsKeepTheirUnits() {
        String[] output = generate(MASS, false);
        assertTrue(output[1].contains("return engine->outputChannels.mass;"), output[1]);
        assertTrue(output[2].contains("{test->reference.mass, \"Fuel mass\", \"mg\", 3}"), output[2]);
    }

    @Test
    void packedScaleIsNotAppliedTwice() {
        String[] output = generate("int16_t autoscale packed;Packed mass;\"mg\", 0.01, 0, -20, 20, 2\n", true);
        assertTrue(output[0].contains("\"mg\", 0.01, 0"), output[0]);
        assertTrue(output[1].contains("return engine->outputChannels.packed;"), output[1]);
        assertTrue(output[2].contains("{test->reference.packed, \"Packed mass\", \"mg\", 2}"), output[2]);
    }

    @Test
    void fractionalScaleAndCategory() {
        String[] output = generate("float elapsed;Elapsed;\"ms\", {1/1000}, 0, 0, 100, 3, \"Timing\"\n", true);
        assertTrue(output[1].contains("return engine->outputChannels.elapsed * 0.001f;"), output[1]);
        assertTrue(output[2].contains("{test->reference.elapsed, \"Elapsed\", \"ms\", 3, \"Timing\", 0.001f}"), output[2]);
    }

    @Test
    void unsupportedTransformsFailGeneration() {
        assertThrows(IllegalArgumentException.class, () ->
                generate("float ratio;Ratio;\"%\", 100, -1, -100, 100, 1\n", true));
        assertThrows(IllegalArgumentException.class, () ->
                generate("float mass;Mass;\"mg\", {useMg ? 1000 : 1}, 0, 0, 100, 1\n", true));
    }

    private static String[] generate(String fields, boolean applyOutputScale) {
        ReaderStateImpl state = new ReaderStateImpl();
        GetOutputValueConsumer lua = new GetOutputValueConsumer(null, LazyFile.REAL);
        SdCardFieldsContent sd = new SdCardFieldsContent();
        lua.applyOutputScale = applyOutputScale;
        sd.applyOutputScale = applyOutputScale;
        OutputsSectionConsumer ts = new OutputsSectionConsumer(null, LazyFile.REAL);
        state.readBufferedReader("struct_no_prefix state_s\n" + fields + "end_struct\n",
                lua, sd::handleEndStruct, ts);
        return new String[]{ts.getContent(), lua.getContent(), sd.getBody()};
    }
}
