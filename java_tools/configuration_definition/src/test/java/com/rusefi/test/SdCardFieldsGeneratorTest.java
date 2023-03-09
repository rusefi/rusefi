package com.rusefi.test;

import static org.junit.Assert.assertEquals;

import com.rusefi.ReaderStateImpl;
import com.rusefi.output.SdCardFieldsConsumer;
import com.rusefi.util.LazyFile;

import org.junit.Test;

public class SdCardFieldsGeneratorTest {
    @Test
    public void outputs() {
        Actor actor = new Actor() {
            @Override
            public void act(ReaderStateImpl state) {
                state.getVariableRegistry().register("PACK_MULT_PERCENT", 100);
                state.getVariableRegistry().register("GAUGE_NAME_RPM", "\"hello\"");
                state.getVariableRegistry().register("GAUGE_NAME_GEAR_RATIO", "ra");
                state.getVariableRegistry().register("GAUGE_NAME_ECU_TEMPERATURE", "te");

            }
        };

        String test = "struct_no_prefix output_channels_s\n" +
                "\tfloat autoscale internalMcuTemperature\n" +
                "uint16_t autoscale RPMValue;@@GAUGE_NAME_RPM@@;\"RPM\",1, 0, 0, 8000, 2, \"myCategory\"\n" +
                "\n" +
                "uint16_t rpmAcceleration;dRPM;\"RPM/s\",1, 0, 0, 5, 2\n" +
                "\n" +
                "\tuint16_t autoscale speedToRpmRatio;@@GAUGE_NAME_GEAR_RATIO@@;\"value\",{1/@@PACK_MULT_PERCENT@@}, 0, 0, 0, 0\n" +
                "end_struct";

        processAndAssert(test, "\t{engine->outputChannels.internalMcuTemperature, \"internalMcuTemperature\", \"\", 0},\n" +
                "\t{engine->outputChannels.RPMValue, \"hello\", \"RPM\", 2, \"myCategory\"},\n" +
                "\t{engine->outputChannels.rpmAcceleration, \"dRPM\", \"RPM/s\", 2},\n" +
                "\t{engine->outputChannels.speedToRpmRatio, \"ra\", \"value\", 0},\n" +
                "", actor);
    }

    @Test
    public void bitAndAlignment() {
        processAndAssert("struct_no_prefix output_channels_s\n" +
                "uint16_t autoscale RPMValue;feee;\"RPM\",1, 0, 0, 8000, 2\n" +
                "bit sd_logging_internal\n" +
                "end_struct", "\t{engine->outputChannels.RPMValue, \"feee\", \"RPM\", 2},\n", readerState -> {

        });
    }

    @Test
    public void array() {
        processAndAssert("struct_no_prefix output_channels_s\n" +
                "uint16_t[4 iterate] recentErrorCode;;\"error\", 1, 0, 0, 0, 0\n" +
                "end_struct", "\t{engine->outputChannels.recentErrorCode[0], \"recentErrorCode 1\", \"error\", 0},\n" +
                "\t{engine->outputChannels.recentErrorCode[1], \"recentErrorCode 2\", \"error\", 0},\n" +
                "\t{engine->outputChannels.recentErrorCode[2], \"recentErrorCode 3\", \"error\", 0},\n" +
                "\t{engine->outputChannels.recentErrorCode[3], \"recentErrorCode 4\", \"error\", 0},\n", readerState -> {

        });
    }

    @Test
    public void bitAndAlignment2() {
        processAndAssert("struct_no_prefix output_channels_s\n" +
                        "    struct pid_status_s\n" +
                        "    \tfloat pTerm;;\"\", 1, 0, -50000, 50000, 2\n" +
                        "    end_struct\n" +
                        "\tpid_status_s alternatorStatus\n" +
                        "end_struct",
                "\t{engine->outputChannels.alternatorStatus.pTerm, \"alternatorStatus.pTerm\", \"\", 2},\n",
                readerState -> {

                });
    }

    interface Actor {
        void act(ReaderStateImpl readerState);
    }

    private static void processAndAssert(String input, String expectedOutput, Actor actor) {
        ReaderStateImpl state = new ReaderStateImpl();
        actor.act(state);

        SdCardFieldsConsumer consumer = new SdCardFieldsConsumer(LazyFile.TEST);
        state.readBufferedReader(input, consumer);
        assertEquals(expectedOutput, consumer.getBody());
    }
}