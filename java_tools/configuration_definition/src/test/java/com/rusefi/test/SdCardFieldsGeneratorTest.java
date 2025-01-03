package com.rusefi.test;

import static com.rusefi.AssertCompatibility.assertEquals;

import com.rusefi.ReaderStateImpl;

import com.rusefi.util.LazyFile;
import org.junit.jupiter.api.Test;

public class SdCardFieldsGeneratorTest {
    @Test
    public void outputs() {
        Actor actor = new Actor() {
            @Override
            public void act(ReaderStateImpl state) {
                state.getVariableRegistry().register("PACK_MULT_PERCENT", 100);
                state.getVariableRegistry().register("GAUGE_NAME_RPM", "\"hello\"");
                state.getVariableRegistry().register("GAUGE_NAME_GEAR_RATIO", "ra");
                state.getVariableRegistry().register("GAUGE_NAME_CPU_TEMP", "te");

            }
        };

        String test = "struct_no_prefix output_channels_s\n" +
                "\tfloat autoscale internalMcuTemperature\n" +
                "uint16_t autoscale RPMValue;@@GAUGE_NAME_RPM@@;\"RPM\",1, 0, 0, 8000, 2, \"myCategory\"\n" +
                "\n" +
            "\t\tfloat fu2elAdd;Lua: Fuel add;\"g\", 1, 0, 0, 1, 3\n" +
            "\t\tfloat fue2lMult;Lua: Fuel mult;\n" +
                "uint16_t rpmAcceleration;dRPM;\"RPM/s\",1, 0, 0, 5, 2\n" +
            "\tstruct LuaAdjustments\n" +
            "\t\tfloat fuelMult;Lua: Fuel mult;\n" +
            "\n" +
            "\t\tbit clutchUpState\n" +
            "\t\tbit brakePedalState\n" +
            "\t\tbit disableDecelerationFuelCutOff\n" +
            "\t\tbit torqueReductionState\n" +
            "\tend_struct\n" +
                "LuaAdjustments lua\n" +
                "\tuint16_t autoscale speedToRpmRatio;@@GAUGE_NAME_GEAR_RATIO@@;\"value\",{1/@@PACK_MULT_PERCENT@@}, 0, 0, 0, 0\n" +
                "end_struct";

        processAndAssert(test, "\t{test->reference.internalMcuTemperature, \"internalMcuTemperature\", \"\", 0},\n" +
            "\t{test->reference.RPMValue, \"hello\", \"RPM\", 2, \"myCategory\"},\n" +
            "\t{test->reference.fu2elAdd, \"Lua: Fuel add\", \"g\", 3},\n" +
            "\t{test->reference.fue2lMult, \"Lua: Fuel mult\", \"\", 0},\n" +
            "\t{test->reference.rpmAcceleration, \"dRPM\", \"RPM/s\", 2},\n" +
            "\t{test->reference.lua.fuelMult, \"Lua: Fuel mult\", \"\", 0},\n" +
            "\t{test->reference, 24, 0, \"lua.clutchUpState\", \"\"},\n" +
            "\t{test->reference, 24, 1, \"lua.brakePedalState\", \"\"},\n" +
            "\t{test->reference, 24, 2, \"lua.disableDecelerationFuelCutOff\", \"\"},\n" +
            "\t{test->reference, 24, 3, \"lua.torqueReductionState\", \"\"},\n" +
            "\t{test->reference.speedToRpmRatio, \"ra\", \"value\", 0},\n", actor, false);
    }

    @Test
    public void bitAndAlignment() {
        String expectedOutput = "\t{test->reference.RPMValue, \"feee\", \"RPM\", 2},\n" +
            "\t{test->reference, 4, 0, \"sd_logging_internal\", \"\"},\n" +
            "\t{test->reference.Value, \"feee\", \"RPM\", 2},\n" +
            "\t{test->reference, 12, 0, \"sd_logging2\", \"\"},\n" +
            "\t{test->reference, 12, 1, \"sd_logging3\", \"\"},\n";

        processAndAssert("struct_no_prefix output_channels_s\n" +
                "uint16_t autoscale RPMValue;feee;\"RPM\",1, 0, 0, 8000, 2\n" +
                "bit sd_logging_internal\n" +
                "uint16_t autoscale Value;feee;\"RPM\",1, 0, 0, 8000, 2\n" +
                "bit sd_logging2\n" +
                "bit sd_logging3\n" +
                "end_struct", expectedOutput, readerState -> {

        }, false);
    }

    @Test
    public void array() {
        processAndAssert("struct_no_prefix output_channels_s\n" +
                "uint16_t[4 iterate] recentErrorCode;;\"error\", 1, 0, 0, 0, 0\n" +
                "end_struct", "\t{test->reference.recentErrorCode[0], \"recentErrorCode 1\", \"error\", 0},\n" +
                "\t{test->reference.recentErrorCode[1], \"recentErrorCode 2\", \"error\", 0},\n" +
                "\t{test->reference.recentErrorCode[2], \"recentErrorCode 3\", \"error\", 0},\n" +
                "\t{test->reference.recentErrorCode[3], \"recentErrorCode 4\", \"error\", 0},\n", readerState -> {

        }, false);
    }

    @Test
    public void bitAndAlignment2() {
        processAndAssert("struct_no_prefix output_channels_s\n" +
                        "    struct pid_status_s\n" +
                        "    \tfloat pTerm;;\"\", 1, 0, -50000, 50000, 2\n" +
                        "    end_struct\n" +
                        "\tpid_status_s[2 iterate] vvtStatus\n" +
                        "\tpid_status_s alternatorStatus\n" +
                        "end_struct",
                "\t{test->reference->alternatorStatus.pTerm, \"alternatorStatus.pTerm\", \"\", 2},\n",
                readerState -> {

                }, true);
    }

    @Test
    public void enumField() {
        processAndAssert("struct_no_prefix output_channels_s\n" +
                        "    custom idle_state_e 4 bits, S32, @OFFSET@, [0:2], \"not important\"\n" +
                        "    idle_state_e idleState\n" +
                        "end_struct",
                "",
                readerState -> {

                }, true);
    }


    interface Actor {
        void act(ReaderStateImpl readerState);
    }

    private static void processAndAssert(String input, String expectedOutput, Actor actor, boolean isPtr) {
        ReaderStateImpl state = new ReaderStateImpl(null, LazyFile.REAL);
        actor.act(state);

        SdCardFieldsTestConsumer consumer = new SdCardFieldsTestConsumer(LazyFile.TEST, isPtr);
        state.readBufferedReader(input, consumer);
        assertEquals(expectedOutput, consumer.getBody());
    }
}
