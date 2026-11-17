package com.rusefi.test;

import com.rusefi.ReaderStateImpl;
import com.rusefi.ldmp.TestFileCaptor;
import com.rusefi.output.DataLogConsumer;
import com.rusefi.output.DuplicateFieldNameException;
import com.rusefi.output.GaugeConsumer;
import com.rusefi.output.OutputsSectionConsumer;
import com.rusefi.output.SdCardFieldsContent;
import org.jetbrains.annotations.NotNull;
import org.junit.jupiter.api.Test;

import static com.rusefi.AssertCompatibility.assertEquals;
import static com.rusefi.AssertCompatibility.assertThrows;
import static com.rusefi.AssertCompatibility.assertTrue;

public class OutputsTest {
    @Test
    public void generateSomething() {
        ReaderStateImpl state = new ReaderStateImpl();
        state.getVariableRegistry().register("GAUGE_NAME_FUEL_WALL_CORRECTION", "wall");
        String test = "struct total\n" +
                "float afr_type;PID dTime;\"ms\",      1,      0,       0, 3000,      0\n" +
                "uint8_t afr_typet;@@GAUGE_NAME_FUEL_WALL_CORRECTION@@;\"ms\",      1,      0,       0, 30,      0\n" +
                "bit isForcedInduction;isForcedInduction\\nDoes the vehicle have a turbo or supercharger?\n" +
                "bit enableFan1WithAc;+Turn on this fan when AC is on.\n" +
                "angle_t m_requested_pump;Computed requested pump \n" +
                "float tCharge;speed density\n" +
                "end_struct\n";

        String expectedLegacy = "afr_type = scalar, F32, 0, \"ms\", 1, 0\n" +
                "afr_typet = scalar, U08, 4, \"ms\", 1, 0\n" +
                "isForcedInduction = bits, U32, 8, [0:0]\n" +
                "enableFan1WithAc = bits, U32, 8, [1:1]\n" +
                "m_requested_pump = scalar, F32, 12, \"\", 1, 0\n" +
                "tCharge = scalar, F32, 16, \"\", 1, 0\n" +
                "; total TS size = 20\n";
        assertEquals(expectedLegacy, runOriginalImplementation(test, state).getContent());
    }

    @Test
    public void fortyBitsInARow() {
        StringBuilder sb = new StringBuilder();
        for (int i = 0; i < 40; i++)
            sb.append("bit b" + i + "\n");
        String test = "struct total\n" +
            sb +
            "end_struct\n";

        // bits roll over into a new 32-bit word after [31:31], the second word is padded to 32 bits
        StringBuilder expected = new StringBuilder();
        for (int i = 0; i < 40; i++)
            expected.append("b" + i + " = bits, U32, " + (i / 32) * 4 + ", [" + (i % 32) + ":" + (i % 32) + "]\n");
        expected.append("; total TS size = 8\n");
        assertEquals(expected.toString(), runOriginalImplementation(test).getContent());
    }

    private static OutputsSectionConsumer runOriginalImplementation(String test) {
        ReaderStateImpl state = new ReaderStateImpl();

        return runOriginalImplementation(test, state);
    }

    @NotNull
    private static OutputsSectionConsumer runOriginalImplementation(String test, ReaderStateImpl state) {
        OutputsSectionConsumer tsProjectConsumer = new OutputsSectionConsumer(null, new TestFileCaptor());
        state.readBufferedReader(test, tsProjectConsumer);
        return tsProjectConsumer;
    }

    @Test
    public void generatePidStateDataLog() {
        String test = "struct total\n" +
                "\tstruct pid_status_s\n" +
                "\t\tfloat pTerm;;\"\", 1, 0, -50000, 50000, 2\n" +
                "\t\tint16_t autoscale dTerm;;\"\", 0.01, 0, -327, 327, 2\n" +
                "\tend_struct\n" +
                "\tpid_status_s[2 iterate] vvtStatus\n" +
                "\tpid_status_s wastegateDcStatus\n" +
                "\n" +
                "end_struct\n";
        ReaderStateImpl state = new ReaderStateImpl();
        DataLogConsumer dataLogConsumer = new DataLogConsumer(null, new TestFileCaptor());
        state.readBufferedReader(test, dataLogConsumer);
        assertEquals(
                "entry = vvtStatus1_pTerm, \"vvtStatus1_pTerm\", float,  \"%.3f\"\n" +
                        "entry = vvtStatus1_dTerm, \"vvtStatus1_dTerm\", float,  \"%.3f\"\n" +
                        "entry = vvtStatus2_pTerm, \"vvtStatus2_pTerm\", float,  \"%.3f\"\n" +
                        "entry = vvtStatus2_dTerm, \"vvtStatus2_dTerm\", float,  \"%.3f\"\n" +
                        "entry = wastegateDcStatus_pTerm, \"wastegateDcStatus_pTerm\", float,  \"%.3f\"\n" +
                        "entry = wastegateDcStatus_dTerm, \"wastegateDcStatus_dTerm\", float,  \"%.3f\"\n",
                dataLogConsumer.getContent());
    }

    @Test
    public void generateDataLog() {
        String test = "struct total\n" +
                "bit issue_294_31,\"si_example\",\"nada_example\"\n" +
                "uint8_t[2 iterate] autoscale knock;;\"\",1, 0, 0, 0, 0\n" +
                "uint8_t[2 iterate] autoscale withName;\"MyNameIsEarl\";\"\",1, 0, 0, 0, 0\n" +
                "uint8_t[2 iterate] withNameNoScale;\"HisNameIsEarl\"\n" +
                "uint8_t yourMonwithNameNoScale;\"HisMon\"\n" +
                "\tuint16_t autoscale baseFuel;@@GAUGE_NAME_FUEL_BASE@@\\nThis is the raw value we take from the fuel map or base fuel algorithm, before the corrections;\"mg\",{1/@@PACK_MULT_PERCENT@@}, 0, 0, 0, 0\n" +
                "float afr_type;PID dTime;\"ms\"\n" +
                "uint16_t autoscale speedToRpmRatio;s2rpm;\"value\",{1/@@PACK_MULT_PERCENT@@}, 0, 0, 0, 0\n" +
                "uint8_t afr_typet;;\"ms\",      1,      0,       0, 30,      0\n" +
                "uint8_t autoscale vehicleSpeedKph;;\"kph\",1, 0, 0, 0, 0\n" +
                "bit isBrakePedalDown;is pedal down?\n" +
                "\tuint8_t unused37;;\"\",1, 0, 0, 0, 0\n" +
                "bit enableFan1WithAc,yes,no;Turn on this fan when AC is on.\n" +
                "end_struct\n";
        ReaderStateImpl state = new ReaderStateImpl();
        state.getVariableRegistry().register("PACK_MULT_PERCENT", 100);
        state.getVariableRegistry().register("GAUGE_NAME_FUEL_BASE", "hello");

        DataLogConsumer dataLogConsumer = new DataLogConsumer(null, new TestFileCaptor());
        state.readBufferedReader(test, dataLogConsumer);
        assertEquals(
                "entry = issue_294_31, \"issue_294_31\", int,    \"%d\"\n" +
                        "entry = knock1, \"knock 1\", int,    \"%d\"\n" +
                        "entry = knock2, \"knock 2\", int,    \"%d\"\n" +
                        "entry = withName1, \"MyNameIsEarl 1\", int,    \"%d\"\n" +
                        "entry = withName2, \"MyNameIsEarl 2\", int,    \"%d\"\n" +
                        "entry = withNameNoScale1, \"HisNameIsEarl 1\", int,    \"%d\"\n" +
                        "entry = withNameNoScale2, \"HisNameIsEarl 2\", int,    \"%d\"\n" +
                        "entry = yourMonwithNameNoScale, \"HisMon\", int,    \"%d\"\n" +
                        "entry = baseFuel, \"hello\", float,  \"%.3f\"\n" +
                        "entry = afr_type, \"PID dTime\", float,  \"%.3f\"\n" +
                        "entry = speedToRpmRatio, \"s2rpm\", float,  \"%.3f\"\n" +
                        "entry = afr_typet, \"afr_typet\", int,    \"%d\"\n" +
                        "entry = vehicleSpeedKph, \"vehicleSpeedKph\", int,    \"%d\"\n" +
                        "entry = isBrakePedalDown, \"is pedal down?\", int,    \"%d\"\n" +
                        "entry = enableFan1WithAc, \"Turn on this fan when AC is on.\", int,    \"%d\"\n", dataLogConsumer.getContent());

    }

    @Test
    public void generateDataLogWithPrefix() {
        String test = "struct total\n" +
                "set_prefix \"Idle\"\n" +
                "bit isBlipping\n" +
                "uint16_t idleTarget;Target RPM\n" +
                "float position;Idle: Position\n" +
                "set_prefix Fan\n" +
                "float fanDuty\n" +
                "set_prefix\n" +
                "float noPrefix\n" +
                "end_struct\n";
        ReaderStateImpl state = new ReaderStateImpl();

        DataLogConsumer dataLogConsumer = new DataLogConsumer(null, new TestFileCaptor());
        state.readBufferedReader(test, dataLogConsumer);
        assertEquals(
                "entry = isBlipping, \"Idle: isBlipping\", int,    \"%d\"\n" +
                        "entry = idleTarget, \"Idle: Target RPM\", int,    \"%d\"\n" +
                        "entry = position, \"Idle: Position\", float,  \"%.3f\"\n" +
                        "entry = fanDuty, \"Fan: fanDuty\", float,  \"%.3f\"\n" +
                        "entry = noPrefix, \"noPrefix\", float,  \"%.3f\"\n", dataLogConsumer.getContent());

        // prefix does not leak into the next file
        DataLogConsumer secondFile = new DataLogConsumer(null, new TestFileCaptor());
        ReaderStateImpl reusedState = new ReaderStateImpl();
        reusedState.readBufferedReader("set_prefix Idle\nstruct a\nfloat x\nend_struct\n", secondFile);
        reusedState.readBufferedReader("struct b\nfloat y\nend_struct\n", secondFile);
        assertEquals(
                "entry = x, \"Idle: x\", float,  \"%.3f\"\n" +
                        "entry = y, \"y\", float,  \"%.3f\"\n", secondFile.getContent());
    }

    @Test
    public void generateDataLogWithPrefixNames() {
        String test = "set_prefix \"WBO\"\n" +
                "struct_no_prefix wideband_state_s\n" +
                "uint8_t heaterDuty;Heater duty;\"%\", 1, 0, 0, 100, 0, @@GAUGE_CATEGORY@@\n" +
                "uint8_t pumpDuty;WBO: Pump duty\n" +
                "set_prefix\n" +
                "uint16_t esr;ESR\n" +
                "end_struct\n";
        ReaderStateImpl state = new ReaderStateImpl();
        state.getVariableRegistry().register("GAUGE_CATEGORY", "Wideband");

        DataLogConsumer dataLogConsumer = new DataLogConsumer(null, new TestFileCaptor());
        dataLogConsumer.outputNames = new String[]{"wb1", "wb2"};
        dataLogConsumer.prefixNames = new String[]{"WBO1", "WBO2"};
        GaugeConsumer gaugeConsumer = new GaugeConsumer(null, new TestFileCaptor());
        gaugeConsumer.outputNames = dataLogConsumer.outputNames;
        gaugeConsumer.prefixNames = dataLogConsumer.prefixNames;
        state.readBufferedReader(test, dataLogConsumer, gaugeConsumer);
        assertEquals(
                "entry = wb1heaterDuty, \"WBO1 WBO: Heater duty\", int,    \"%d\"\n" +
                        "entry = wb1pumpDuty, \"WBO1 WBO: Pump duty\", int,    \"%d\"\n" +
                        "entry = wb1esr, \"WBO1: ESR\", int,    \"%d\"\n" +
                        "entry = wb2heaterDuty, \"WBO2 WBO: Heater duty\", int,    \"%d\"\n" +
                        "entry = wb2pumpDuty, \"WBO2 WBO: Pump duty\", int,    \"%d\"\n" +
                        "entry = wb2esr, \"WBO2: ESR\", int,    \"%d\"\n",
                dataLogConsumer.getContent());

        assertEquals("\tgaugeCategory = Wideband\n" +
                        "wb1heaterDutyGauge = wb1heaterDuty,\"WBO1 WBO: Heater duty\", \"%\", 0.0,100.0, 0.0,100.0, 0.0,100.0, 0,0\n" +
                        "wb2heaterDutyGauge = wb2heaterDuty,\"WBO2 WBO: Heater duty\", \"%\", 0.0,100.0, 0.0,100.0, 0.0,100.0, 0,0\n",
                gaugeConsumer.getContent());
    }

    @Test
    public void prefixIsNotAppliedToAlignmentFill() {
        String test = "set_prefix \"Rolling Launch\"\n" +
                "struct_no_prefix total\n" +
                "uint8_t a;;\"\", 1, 0, 0, 100, 0, @@GAUGE_CATEGORY@@\n" +
                "uint16_t b\n" +
                "end_struct\n";
        ReaderStateImpl state = new ReaderStateImpl();
        state.getVariableRegistry().register("GAUGE_CATEGORY", "Launch");

        DataLogConsumer dataLogConsumer = new DataLogConsumer(null, new TestFileCaptor());
        GaugeConsumer gaugeConsumer = new GaugeConsumer(null, new TestFileCaptor());
        state.readBufferedReader(test, dataLogConsumer, gaugeConsumer);
        assertEquals(
                "entry = a, \"Rolling Launch: a\", int,    \"%d\"\n" +
                        "entry = b, \"Rolling Launch: b\", int,    \"%d\"\n",
                dataLogConsumer.getContent());
        assertEquals("\tgaugeCategory = Launch\n" +
                        "aGauge = a,\"Rolling Launch: a\", \"\", 0.0,100.0, 0.0,100.0, 0.0,100.0, 0,0\n",
                gaugeConsumer.getContent());
    }

    @Test
    public void generateDataLogMultiLineCommentWithQuotes() {
        String test = "#define GAUGE_NAME_FUEL_BASE \"fuel: base mass\"\n" +
                "struct total\n" +
                "\tuint16_t autoscale baseFuel;@@GAUGE_NAME_FUEL_BASE@@\\nThis is the raw value we take from the fuel map or base fuel algorithm, before the corrections;\"mg\",1, 0, 0, 0, 0\n" +
                "\tuint16_t autoscale baseFuel2;\"line1\\nline2\";\"mg\",1, 0, 0, 0, 0\n" +
                "end_struct\n";
        ReaderStateImpl state = new ReaderStateImpl();

        DataLogConsumer dataLogConsumer = new DataLogConsumer(null, new TestFileCaptor());
        state.readBufferedReader(test, dataLogConsumer);

        assertEquals("\"fuel: base mass\"", state.getVariableRegistry().get("GAUGE_NAME_FUEL_BASE"));
        assertEquals(
                "entry = baseFuel, \"fuel: base mass\", int,    \"%d\"\n" +
                        "entry = baseFuel2, \"line1\", int,    \"%d\"\n"
                , dataLogConsumer.getContent());

    }

    @Test
    public void prefixWithMultiLineQuotedDefine() {
        String test = "#define GAUGE_NAME_FUEL_BASE \"Fuel: base mass\"\n" +
                "set_prefix \"Fuel\"\n" +
                "struct total\n" +
                "\tuint16_t autoscale baseFuel;@@GAUGE_NAME_FUEL_BASE@@\\nThis is the raw value;\"mg\",1, 0, 0, 0, 0\n" +
                "\tuint16_t autoscale baseFuel2;\"line1\\nline2\";\"mg\",1, 0, 0, 0, 0\n" +
                "end_struct\n";
        ReaderStateImpl state = new ReaderStateImpl();

        DataLogConsumer dataLogConsumer = new DataLogConsumer(null, new TestFileCaptor());
        SdCardFieldsContent sdCardFieldsConsumer = new SdCardFieldsContent();
        state.readBufferedReader(test, dataLogConsumer, sdCardFieldsConsumer::handleEndStruct);

        assertEquals(
                "entry = baseFuel, \"Fuel: base mass\", int,    \"%d\"\n" +
                        "entry = baseFuel2, \"Fuel: line1\", int,    \"%d\"\n"
                , dataLogConsumer.getContent());
        assertTrue(sdCardFieldsConsumer.getBody(), sdCardFieldsConsumer.getBody().contains("\"Fuel: base mass\", \"mg\""));
    }

    @Test
    public void sensorStruct() {
        String test = "struct total\n" +
                "    struct pid_status_s\n" +
                "    \tfloat iTerm;;\"v\", 1, 0, -10000, 10000, 4, @@GAUGE_CATEGORY@@\n" +
                "    \tfloat dTerm;;\"v\", 1, 0, -10000, 10000, 4, @@GAUGE_CATEGORY@@\n" +
                "    end_struct\n" +
                "\tpid_status_s alternatorStatus\n" +
                "\tpid_status_s idleStatus\n" +
                "end_struct\n";

        ReaderStateImpl state = new ReaderStateImpl();
        state.getVariableRegistry().register("GAUGE_CATEGORY", "Alternator");
        DataLogConsumer dataLogConsumer = new DataLogConsumer(null, new TestFileCaptor());
        GaugeConsumer gaugeConsumer = new GaugeConsumer(null, new TestFileCaptor());
        state.readBufferedReader(test, dataLogConsumer, gaugeConsumer);
        assertEquals(
                "entry = alternatorStatus_iTerm, \"alternatorStatus_iTerm\", float,  \"%.3f\"\n" +
                        "entry = alternatorStatus_dTerm, \"alternatorStatus_dTerm\", float,  \"%.3f\"\n" +
                        "entry = idleStatus_iTerm, \"idleStatus_iTerm\", float,  \"%.3f\"\n" +
                        "entry = idleStatus_dTerm, \"idleStatus_dTerm\", float,  \"%.3f\"\n",
                dataLogConsumer.getContent());

        assertEquals("\tgaugeCategory = Alternator\n" +
                        "alternatorStatus_iTermGauge = alternatorStatus_iTerm,\"alternatorStatus_ iTerm\", \"v\", -10000.0,10000.0, -10000.0,10000.0, -10000.0,10000.0, 4,4\n" +
                        "alternatorStatus_dTermGauge = alternatorStatus_dTerm,\"alternatorStatus_ dTerm\", \"v\", -10000.0,10000.0, -10000.0,10000.0, -10000.0,10000.0, 4,4\n" +
                        "idleStatus_iTermGauge = idleStatus_iTerm,\"idleStatus_ iTerm\", \"v\", -10000.0,10000.0, -10000.0,10000.0, -10000.0,10000.0, 4,4\n" +
                        "idleStatus_dTermGauge = idleStatus_dTerm,\"idleStatus_ dTerm\", \"v\", -10000.0,10000.0, -10000.0,10000.0, -10000.0,10000.0, 4,4\n",
                gaugeConsumer.getContent());

    }

    @Test
    public void testLongTooltipsIterate() {
        ReaderStateImpl state = new ReaderStateImpl();
        String test = "struct total\n" +
                "\tint[3 iterate] triggerSimulatorPins;Each rusEFI piece can provide synthetic trigger signal for external ECU. Sometimes these wires are routed back into trigger inputs of the same rusEFI board.\\nSee also directSelfStimulation which is different.\n" +
                "end_struct\n";
        TestTSProjectConsumer tsProjectConsumer = new TestTSProjectConsumer(state);
        state.readBufferedReader(test, tsProjectConsumer);
        assertEquals(
"\ttriggerSimulatorPins1 = \"Each rusEFI piece can provide synthetic trigger signal for external ECU. Sometimes these wires are routed back into trigger inputs of the same rusEFI board.\\nSee also directSelfStimulation which is different. 1\"\n" +
        "\ttriggerSimulatorPins2 = \"Each rusEFI piece can provide synthetic trigger signal for external ECU. Sometimes these wires are routed back into trigger inputs of the same rusEFI board.\\nSee also directSelfStimulation which is different. 2\"\n" +
        "\ttriggerSimulatorPins3 = \"Each rusEFI piece can provide synthetic trigger signal for external ECU. Sometimes these wires are routed back into trigger inputs of the same rusEFI board.\\nSee also directSelfStimulation which is different. 3\"\n", tsProjectConsumer.getSettingContextHelpForUnitTest());
    }

    @Test
    public void iteratedStructNoPrefixUniqueNames() {
        ReaderStateImpl state = new ReaderStateImpl();
        String test = "struct_no_prefix acc_s\n" +
                "uint8_t acc_max;Max accumulator value;\"\", 1, 0, 0, 255, 0\n" +
                "end_struct\n" +
                "struct total\n" +
                "acc_s[3 iterate] accumulators;Accumulators\n" +
                "end_struct\n";
        TestTSProjectConsumer tsProjectConsumer = new TestTSProjectConsumer(state);
        state.readBufferedReader(test, tsProjectConsumer);
        String content = tsProjectConsumer.getContent();
        assertTrue("Expected accumulators1_acc_max but got: " + content, content.contains("accumulators1_acc_max"));
        assertTrue("Expected accumulators2_acc_max but got: " + content, content.contains("accumulators2_acc_max"));
        assertTrue("Expected accumulators3_acc_max but got: " + content, content.contains("accumulators3_acc_max"));
    }

    @Test
    public void nameDuplicate() {
      assertThrows(DuplicateFieldNameException.class, () -> {
        String test = "struct total\n" +
          "float afr_type;PID dTime;\"ms\",      1,      0,       0, 3000,      0\n" +
          "uint8_t afr_type;123;\"ms\",      1,      0,       0, 30,      0\n" +
          "end_struct\n";


        runOriginalImplementation(test);
      });
    }

    @Test
    public void nameNotDuplicate() {
        String test = "struct total\n" +
                "float afr_type;PID dTime;\"ms\",      1,      0,       0, 3000,      0\n" +
                "struct afr_type\n" +
                "float afr_type2;PID dTime;\"ms\",      1,      0,       0, 3000,      0\n" +
                "end_struct\n" +
                "end_struct\n";

        runOriginalImplementation(test);
    }
}
