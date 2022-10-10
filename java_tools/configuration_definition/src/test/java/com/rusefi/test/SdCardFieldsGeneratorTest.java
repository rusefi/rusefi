package com.rusefi.test;

import static org.junit.Assert.assertEquals;

import com.rusefi.ReaderState;
import com.rusefi.output.SdCardFieldsConsumer;
import com.rusefi.util.LazyFile;

import org.junit.Test;

public class SdCardFieldsGeneratorTest {
    @Test
    public void outputs() {
        String test = "struct_no_prefix output_channels_s\n" +
                "uint16_t autoscale RPMValue;@@GAUGE_NAME_RPM@@;\"RPM\",1, 0, 0, 8000, 0\n" +
                "\n" +
                "uint16_t rpmAcceleration;dRPM;\"RPM/s\",1, 0, 0, 5, 0\n" +
                "\n" +
                "\tuint16_t autoscale speedToRpmRatio;@@GAUGE_NAME_GEAR_RATIO@@;\"value\",{1/@@PACK_MULT_PERCENT@@}, 0, 0, 0, 0\n" +
                "\tint8_t autoscale internalMcuTemperature;@@GAUGE_NAME_CPU_TEMP@@;\"deg C\",1, 0, 0, 0, 0\n" +
                "end_struct";

        ReaderState state = new ReaderState();
        state.variableRegistry.register("PACK_MULT_PERCENT", 100);
        state.variableRegistry.register("GAUGE_NAME_RPM", "\"hello\"");
        state.variableRegistry.register("GAUGE_NAME_GEAR_RATIO", "ra");
        state.variableRegistry.register("GAUGE_NAME_CPU_TEMP", "te");


        SdCardFieldsConsumer consumer = new SdCardFieldsConsumer(LazyFile.TEST);
        state.readBufferedReader(test, consumer);
        assertEquals("\t{engine->outputChannels.RPMValue, \"hello\", \"RPM\", 0},\n" +
                "\t{engine->outputChannels.rpmAcceleration, \"dRPM\", \"RPM/s\", 0},\n" +
                "\t{engine->outputChannels.speedToRpmRatio, \"ra\", \"value\", 0},\n" +
                "\t{engine->outputChannels.internalMcuTemperature, \"te\", \"deg C\", 0},\n" +
                "\t{engine->outputChannels.alignmentFill_at_7, \"need 4 byte alignment\", \"units\", 0},\n", consumer.getBody());
    }
}
