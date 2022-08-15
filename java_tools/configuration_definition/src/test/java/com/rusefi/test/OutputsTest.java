package com.rusefi.test;

import com.rusefi.BitState;
import com.rusefi.ReaderState;
import com.rusefi.output.DataLogConsumer;
import com.rusefi.output.GaugeConsumer;
import com.rusefi.output.GetOutputValueConsumer;
import com.rusefi.output.OutputsSectionConsumer;
import org.junit.Test;

import java.io.IOException;

import static com.rusefi.test.newParse.NewParseHelper.parseToOutputChannels;
import static org.junit.Assert.assertEquals;

public class OutputsTest {
    @Test
    public void generateDataLog() throws IOException {
        String test = "struct total\n" +
                "bit issue_294_31,\"si_example\",\"nada_example\"\n" +
                "uint8_t[2 iterate] autoscale knock;;\"\",1, 0, 0, 0, 0\n" +
                "uint8_t[2 iterate] autoscale withName;\"MyNameIsEarl\";\"\",1, 0, 0, 0, 0\n" +
                "\tuint16_t autoscale baseFuel;@@GAUGE_NAME_FUEL_BASE@@\\nThis is the raw value we take from the fuel map or base fuel algorithm, before the corrections;\"mg\",{1/@@PACK_MULT_PERCENT@@}, 0, 0, 0, 0\n" +
                "float afr_type;PID dTime;\"ms\",      1,      0,       0, 3000,      0\n" +
                "uint16_t autoscale speedToRpmRatio;s2rpm;\"value\",{1/@@PACK_MULT_PERCENT@@}, 0, 0, 0, 0\n" +
                "uint8_t afr_typet;;\"ms\",      1,      0,       0, 3000,      0\n" +
                "uint8_t autoscale vehicleSpeedKph;;\"kph\",1, 0, 0, 0, 0\n" +
                "bit isForcedInduction;Does the vehicle have a turbo or supercharger?\n" +
                "\tuint8_t unused37;;\"\",1, 0, 0, 0, 0\n" +
                "bit enableFan1WithAc;+Turn on this fan when AC is on.\n" +
                "end_struct\n";
        ReaderState state = new ReaderState();
        state.variableRegistry.register("PACK_MULT_PERCENT", 100);
        state.variableRegistry.register("GAUGE_NAME_FUEL_BASE", "hello");
    }

    @Test
    public void generateGetOutputs() throws IOException {
        String test = "struct_no_prefix ts_outputs_s\n" +
                "bit issue_294_31,\"si_example\",\"nada_example\"\n" +
                "bit enableFan1WithAc;+Turn on this fan when AC is on.\n" +
                "int hwChannel;\n" +
                "end_struct\n";
        ReaderState state = new ReaderState();

        GetOutputValueConsumer outputValueConsumer = new GetOutputValueConsumer(null);
        state.readBufferedReader(test, (outputValueConsumer));
        assertEquals(
                "#include \"pch.h\"\n" +
                        "#include \"value_lookup.h\"\n" +
                        "float getOutputValueByName(const char *name) {\n" +
                        "\tif (strEqualCaseInsensitive(name, \"issue_294_31\"))\n" +
                        "\t\treturn engine->outputChannels.issue_294_31;\n" +
                        "\tif (strEqualCaseInsensitive(name, \"enableFan1WithAc\"))\n" +
                        "\t\treturn engine->outputChannels.enableFan1WithAc;\n" +
                        "\tif (strEqualCaseInsensitive(name, \"hwChannel\"))\n" +
                        "\t\treturn engine->outputChannels.hwChannel;\n" +
                        "\treturn EFI_ERROR_CODE;\n" +
                        "}\n", outputValueConsumer.getContent());
    }

    @Test
    public void sensorStruct() throws IOException {
        String test = "struct total\n" +
                "    struct pid_status_s\n" +
                "    \tfloat iTerm;;\"v\", 1, 0, -10000, 10000, 4\n" +
                "    \tfloat dTerm;;\"v\", 1, 0, -10000, 10000, 4\n" +
                "    end_struct\n" +
                "\tpid_status_s alternatorStatus\n" +
                "\tpid_status_s idleStatus\n" +
                "end_struct\n";

        ReaderState state = new ReaderState();
        GaugeConsumer gaugeConsumer = new GaugeConsumer(null);
        state.readBufferedReader(test, gaugeConsumer);
        assertEquals("alternatorStatus_iTermGauge = alternatorStatus_iTerm,\"alternatorStatus_ iTerm\", \"v\", -10000.0,10000.0, -10000.0,10000.0, -10000.0,10000.0, 4,4\n" +
                        "alternatorStatus_dTermGauge = alternatorStatus_dTerm,\"alternatorStatus_ dTerm\", \"v\", -10000.0,10000.0, -10000.0,10000.0, -10000.0,10000.0, 4,4\n" +
                        "idleStatus_iTermGauge = idleStatus_iTerm,\"idleStatus_ iTerm\", \"v\", -10000.0,10000.0, -10000.0,10000.0, -10000.0,10000.0, 4,4\n" +
                        "idleStatus_dTermGauge = idleStatus_dTerm,\"idleStatus_ dTerm\", \"v\", -10000.0,10000.0, -10000.0,10000.0, -10000.0,10000.0, 4,4\n",
                gaugeConsumer.getContent());

    }

    @Test
    public void testLongIterate() throws IOException {
        ReaderState state = new ReaderState();
        String test = "struct total\n" +
"\tint[3 iterate] triggerSimulatorPins;Each rusEFI piece can provide synthetic trigger signal for external ECU. Sometimes these wires are routed back into trigger inputs of the same rusEFI board.\\nSee also directSelfStimulation which is different.\n" +
                "end_struct\n";
        TestTSProjectConsumer tsProjectConsumer = new TestTSProjectConsumer("", state);
        state.readBufferedReader(test, tsProjectConsumer);
        assertEquals(
                "\ttriggerSimulatorPins1 = \"Each rusEFI piece can provide synthetic trigger signal for external ECU. Sometimes these wires are routed back into trigger inputs of the same rusEFI board.\"\n" +
                        "\ttriggerSimulatorPins2 = \"Each rusEFI piece can provide synthetic trigger signal for external ECU. Sometimes these wires are routed back into trigger inputs of the same rusEFI board.\"\n" +
                        "\ttriggerSimulatorPins3 = \"Each rusEFI piece can provide synthetic trigger signal for external ECU. Sometimes these wires are routed back into trigger inputs of the same rusEFI board.\"\n", tsProjectConsumer.getSettingContextHelp().toString());
    }
}
