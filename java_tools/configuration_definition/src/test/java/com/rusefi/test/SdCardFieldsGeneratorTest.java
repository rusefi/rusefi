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
                "\tfloat autoscale internalMcuTemperature\n" +
                "uint16_t autoscale RPMValue;@@GAUGE_NAME_RPM@@;\"RPM\",1, 0, 0, 8000, 2\n" +
                "\n" +
                "uint16_t rpmAcceleration;dRPM;\"RPM/s\",1, 0, 0, 5, 2\n" +
                "\n" +
                "\tuint16_t autoscale speedToRpmRatio;@@GAUGE_NAME_GEAR_RATIO@@;\"value\",{1/@@PACK_MULT_PERCENT@@}, 0, 0, 0, 0\n" +
                "end_struct";

        ReaderState state = new ReaderState();
        state.variableRegistry.register("PACK_MULT_PERCENT", 100);
        state.variableRegistry.register("GAUGE_NAME_RPM", "\"hello\"");
        state.variableRegistry.register("GAUGE_NAME_GEAR_RATIO", "ra");
        state.variableRegistry.register("GAUGE_NAME_CPU_TEMP", "te");


        SdCardFieldsConsumer consumer = new SdCardFieldsConsumer(LazyFile.TEST);
        state.readBufferedReader(test, consumer);
        assertEquals(
                "\t{engine->outputChannels.internalMcuTemperature, \"internalMcuTemperature\", \"\", -1},\n" +
                        "\t{engine->outputChannels.RPMValue, \"hello\", \"RPM\", 2},\n" +
                        "\t{engine->outputChannels.rpmAcceleration, \"dRPM\", \"RPM/s\", 2},\n" +
                        "\t{engine->outputChannels.speedToRpmRatio, \"ra\", \"value\", 0},\n" +
                ""
                , consumer.getBody());
    }

    @Test
    public void bitAndAlignment() {
        String test = "struct_no_prefix output_channels_s\n" +
                "uint16_t autoscale RPMValue;feee;\"RPM\",1, 0, 0, 8000, 2\n" +
                "bit sd_logging_internal\n" +
                "end_struct";

        ReaderState state = new ReaderState();

        SdCardFieldsConsumer consumer = new SdCardFieldsConsumer(LazyFile.TEST);
        state.readBufferedReader(test, consumer);
        assertEquals("\t{engine->outputChannels.RPMValue, \"feee\", \"RPM\", 2},\n", consumer.getBody());
    }

    @Test
    public void array() {
        String test = "struct_no_prefix output_channels_s\n" +
                "uint16_t[4 iterate] recentErrorCode;;\"error\", 1, 0, 0, 0, 0\n" +
                "end_struct";

        ReaderState state = new ReaderState();

        SdCardFieldsConsumer consumer = new SdCardFieldsConsumer(LazyFile.TEST);
        state.readBufferedReader(test, consumer);
        assertEquals("\t{engine->outputChannels.recentErrorCode[0], \"recentErrorCode 1\", \"error\", 0},\n" +
                "\t{engine->outputChannels.recentErrorCode[1], \"recentErrorCode 2\", \"error\", 0},\n" +
                "\t{engine->outputChannels.recentErrorCode[2], \"recentErrorCode 3\", \"error\", 0},\n" +
                "\t{engine->outputChannels.recentErrorCode[3], \"recentErrorCode 4\", \"error\", 0},\n", consumer.getBody());
    }
}
