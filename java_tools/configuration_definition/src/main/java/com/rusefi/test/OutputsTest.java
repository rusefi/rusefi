package com.rusefi.test;

import com.rusefi.BitState;
import com.rusefi.ReaderState;
import com.rusefi.output.*;
import org.junit.Test;

import java.io.IOException;
import java.util.Arrays;
import java.util.Collections;

import static org.junit.Assert.assertEquals;

public class OutputsTest {
    @Test
    public void generateSomething() throws IOException {
        String test = "struct total\n" +
                "float afr_type;PID dTime;\"ms\",      1,      0,       0, 3000,      0\n" +
                "uint8_t afr_typet;PID dTime;\"ms\",      1,      0,       0, 3000,      0\n" +
                "bit isForcedInduction;Does the vehicle have a turbo or supercharger?\n" +
                "bit enableFan1WithAc;+Turn on this fan when AC is on.\n" +
                "end_struct\n";
        ReaderState state = new ReaderState();

        OutputsSectionConsumer tsProjectConsumer = new OutputsSectionConsumer(null, state);
        state.readBufferedReader(test, Collections.singletonList(tsProjectConsumer));


        assertEquals("afr_type = scalar, F32, 0, \"ms\", 1, 0\n" +
                "afr_typet = scalar, U08, 4, \"ms\", 1, 0\n" +
                "isForcedInduction = bits, U32, 5, [0:0]\n" +
                "enableFan1WithAc = bits, U32, 5, [1:1]\n" +
                "unusedBit_4_2 = bits, U32, 5, [2:2]\n" +
                "unusedBit_4_3 = bits, U32, 5, [3:3]\n" +
                "unusedBit_4_4 = bits, U32, 5, [4:4]\n" +
                "unusedBit_4_5 = bits, U32, 5, [5:5]\n" +
                "unusedBit_4_6 = bits, U32, 5, [6:6]\n" +
                "unusedBit_4_7 = bits, U32, 5, [7:7]\n" +
                "unusedBit_4_8 = bits, U32, 5, [8:8]\n" +
                "unusedBit_4_9 = bits, U32, 5, [9:9]\n" +
                "unusedBit_4_10 = bits, U32, 5, [10:10]\n" +
                "unusedBit_4_11 = bits, U32, 5, [11:11]\n" +
                "unusedBit_4_12 = bits, U32, 5, [12:12]\n" +
                "unusedBit_4_13 = bits, U32, 5, [13:13]\n" +
                "unusedBit_4_14 = bits, U32, 5, [14:14]\n" +
                "unusedBit_4_15 = bits, U32, 5, [15:15]\n" +
                "unusedBit_4_16 = bits, U32, 5, [16:16]\n" +
                "unusedBit_4_17 = bits, U32, 5, [17:17]\n" +
                "unusedBit_4_18 = bits, U32, 5, [18:18]\n" +
                "unusedBit_4_19 = bits, U32, 5, [19:19]\n" +
                "unusedBit_4_20 = bits, U32, 5, [20:20]\n" +
                "unusedBit_4_21 = bits, U32, 5, [21:21]\n" +
                "unusedBit_4_22 = bits, U32, 5, [22:22]\n" +
                "unusedBit_4_23 = bits, U32, 5, [23:23]\n" +
                "unusedBit_4_24 = bits, U32, 5, [24:24]\n" +
                "unusedBit_4_25 = bits, U32, 5, [25:25]\n" +
                "unusedBit_4_26 = bits, U32, 5, [26:26]\n" +
                "unusedBit_4_27 = bits, U32, 5, [27:27]\n" +
                "unusedBit_4_28 = bits, U32, 5, [28:28]\n" +
                "unusedBit_4_29 = bits, U32, 5, [29:29]\n" +
                "unusedBit_4_30 = bits, U32, 5, [30:30]\n" +
                "unusedBit_4_31 = bits, U32, 5, [31:31]\n" +
                "alignmentFill_at_9 = array, U08, 9, [3], \"units\", 1, 0\n", new String(tsProjectConsumer.getTsWriter().toCharArray()));

    }

    @Test(expected = BitState.TooManyBitsInARow.class)
    public void tooManyBits() throws IOException {
        StringBuilder sb = new StringBuilder();
        for (int i = 0; i < 40; i++)
            sb.append("bit b" + i + "\n");
        String test = "struct total\n" +
                sb +
                "end_struct\n";
        ReaderState state = new ReaderState();

        OutputsSectionConsumer tsProjectConsumer = new OutputsSectionConsumer(null, state);
        state.readBufferedReader(test, Collections.singletonList(tsProjectConsumer));
    }

    @Test
    public void generateDataLog() throws IOException {
        String test = "struct total\n" +
                "bit issue_294_31,\"si_example\",\"nada_example\"\n" +
                "uint8_t[2 iterate] autoscale knock;;\"\",1, 0, 0, 0, 0\n" +
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

        DataLogConsumer dataLogConsumer = new DataLogConsumer(null);
        state.readBufferedReader(test, Collections.singletonList(dataLogConsumer));
        assertEquals(
                "entry = issue_294_31, \"issue_294_31\", int,    \"%d\"\n" +
                        "entry = knock1, \"knock1\", int,    \"%d\"\n" +
                        "entry = knock2, \"knock2\", int,    \"%d\"\n" +
                        "entry = baseFuel, \"hello\", float,  \"%.3f\"\n" +
                        "entry = afr_type, \"PID dTime\", float,  \"%.3f\"\n" +
                        "entry = speedToRpmRatio, \"s2rpm\", float,  \"%.3f\"\n" +
                        "entry = afr_typet, \"afr_typet\", int,    \"%d\"\n" +
                        "entry = vehicleSpeedKph, \"vehicleSpeedKph\", int,    \"%d\"\n" +
                        "entry = isForcedInduction, \"Does the vehicle have a turbo or supercharger?\", int,    \"%d\"\n" +
                        "entry = enableFan1WithAc, \"+Turn on this fan when AC is on.\", int,    \"%d\"\n", new String(dataLogConsumer.getTsWriter().toCharArray()));

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
        state.readBufferedReader(test, Collections.singletonList(outputValueConsumer));
        assertEquals(
                "#include \"pch.h\"\n" +
                        "float getOutputValueByName(const char *name) {\n" +
                        "\tif (strEqualCaseInsensitive(name, \"hwChannel\"))\n" +
                        "\t\treturn tsOutputChannels->hwChannel;\n" +
                        "\treturn EFI_ERROR_CODE;\n" +
                        "}\n", outputValueConsumer.getContent());
    }

    @Test
    public void testStructArrayAndCharArgument() throws IOException {
        ReaderState state = new ReaderState();
        String test = "struct total\n" +
                "custom lua_script_t 200 string, ASCII, @OFFSET@, 200\n" +
                "lua_script_t luaScript;\n" +
                "struct dc_io\n" +
                "\tint disablePin;\n" +
                "end_struct\n" +
                "\tdc_io[2 iterate] etbIn\n" +
                "end_struct\n";
        GetConfigValueConsumer getConfigValueConsumer = new GetConfigValueConsumer(null);
        state.readBufferedReader(test, Collections.singletonList(getConfigValueConsumer));

        assertEquals("#include \"pch.h\"\n" +
                "float getConfigValueByName(const char *name) {\n" +
                "\treturn EFI_ERROR_CODE;\n" +
                "}\n", getConfigValueConsumer.getGetterForUnitTest());
    }

    @Test
    public void generateEmbeddedStruct() throws IOException {
        ReaderState state = new ReaderState();
        String test = "struct total\n" +
                "struct_no_prefix thermistor_conf_s @brief Thermistor known values\n" +
                "float tempC_1;these values are in Celcius;\"*C\", 1, 0, -40, 200, 1\n" +
                "\n" +
                "end_struct\n" +
                "struct ThermistorConf @brief Thermistor curve parameters\n" +
                "\tthermistor_conf_s config;\n" +
                "\tint adcChannel;\n" +
                "end_struct\n" +
                "ThermistorConf iat;\n" +
                "end_struct\n";
        GetConfigValueConsumer getConfigValueConsumer = new GetConfigValueConsumer(null);
        state.readBufferedReader(test, Collections.singletonList(getConfigValueConsumer));

        assertEquals("\tif (strEqualCaseInsensitive(name, \"iat.config.tempC_1\"))\n" +
                "\t{\n" +
                "\t\tconfig->iat.config.tempC_1 = value;\n" +
                "\t\treturn;\n" +
                "\t}\n" +
                "\tif (strEqualCaseInsensitive(name, \"iat.adcChannel\"))\n" +
                "\t{\n" +
                "\t\tconfig->iat.adcChannel = (int)value;\n" +
                "\t\treturn;\n" +
                "\t}\n", getConfigValueConsumer.getSetterBody());

        assertEquals("#include \"pch.h\"\n" +
                "float getConfigValueByName(const char *name) {\n" +
                "\tif (strEqualCaseInsensitive(name, \"iat.config.tempC_1\"))\n" +
                "\t\treturn config->iat.config.tempC_1;\n" +
                "\tif (strEqualCaseInsensitive(name, \"iat.adcChannel\"))\n" +
                "\t\treturn config->iat.adcChannel;\n" +
                "\treturn EFI_ERROR_CODE;\n" +
                "}\n", getConfigValueConsumer.getGetterForUnitTest());
    }

    @Test
    public void generateGetConfig() throws IOException {
        String test = "struct total\n" +
                "struct_no_prefix thermistor_conf_s @brief Thermistor known values\n" +
                "float tempC_1;these values are in Celcius;\"*C\", 1, 0, -40, 200, 1\n" +
                "\n" +
                "custom air_pressure_sensor_type_e 4 bits, U32, @OFFSET@, [0:4], \"Custom\", \"DENSO183\", \"MPX4250\", \"HONDA3BAR\", \"NEON_2003\", \"22012AA090\", \"3 Bar\", \"MPX4100\", \"Toyota 89420-02010\", \"MPX4250A\", \"Bosch 2.5\", \"Mazda1Bar\", \"GM 2 Bar\", \"GM 1 Bar\", \"MPXH6400\"\n" +
                "struct air_pressure_sensor_config_s\n" +
                "float highValue;kPa value at high volts;\"kpa\", 1, 0, -400, 800, 2\n" +
                "air_pressure_sensor_type_e type;\n" +
                "int hwChannel;\n" +
                "uint8_t[3] alignmentFill;;\"unit\", 1, 0, 0, 100, 0\n" +
                "\n" +
                "end_struct\n" +
                "\n" +
                "struct MAP_sensor_config_s @brief MAP averaging configuration\n" +
                "float[6] samplingAngleBins;;\"\", 1, 0, 0, 18000, 2\n" +
                "air_pressure_sensor_config_s sensor\n" +
                "end_struct\n" +
                "MAP_sensor_config_s map;@see isMapAveragingEnabled\n" +
                "struct injector_s\n" +
                "\tfloat flow;+This is your injector flow at the fuel pressure used in the vehicle. cc/min, cubic centimetre per minute\\nBy the way, g/s = 0.125997881 * (lb/hr)\\ng/s = 0.125997881 * (cc/min)/10.5\\ng/s = 0.0119997981 * cc/min;\"cm3/min\", 1, 0, 0, 99999, 2\n" +
                "\n" +
                "float[8] battLagCorr;ms delay between injector open and close dead times;\"ms\", 1, 0, 0, 50, 2\n" +
                "\n" +
                "end_struct\n" +
                "\n" +
                "injector_s injector\n" +
         "\tint[12 iterate] ignitionPins;\n" +
                "\tfloat bias_resistor;+Pull-up resistor value on your board;\"Ohm\", 1, 0, 0, 200000, 1\n" +
                "end_struct\n" +
                "struct ThermistorConf @brief Thermistor curve parameters\n" +
                "\tthermistor_conf_s config;\n" +
                "\tint adcChannel;\n" +
                "end_struct\n" +
                "ThermistorConf clt;todo: merge with channel settings, use full-scale Thermistor here!\n" +
                "bit issue_294_31,\"si_example\",\"nada_example\"\n" +
                "uint8_t[2 iterate] autoscale knock;;\"\",1, 0, 0, 0, 0\n" +
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


        GetConfigValueConsumer getConfigValueConsumer = new GetConfigValueConsumer(null);
        state.readBufferedReader(test, Collections.singletonList(getConfigValueConsumer));

        assertEquals("#include \"pch.h\"\n" +
                "float getConfigValueByName(const char *name) {\n" +
                "\tif (strEqualCaseInsensitive(name, \"clt.config.tempC_1\"))\n" +
                "\t\treturn config->clt.config.tempC_1;\n" +
                "\tif (strEqualCaseInsensitive(name, \"clt.config.map.sensor.highValue\"))\n" +
                "\t\treturn config->clt.config.map.sensor.highValue;\n" +
                "\tif (strEqualCaseInsensitive(name, \"clt.config.map.sensor.hwChannel\"))\n" +
                "\t\treturn config->clt.config.map.sensor.hwChannel;\n" +
                "\tif (strEqualCaseInsensitive(name, \"clt.config.injector.flow\"))\n" +
                "\t\treturn config->clt.config.injector.flow;\n" +
                "\tif (strEqualCaseInsensitive(name, \"clt.config.bias_resistor\"))\n" +
                "\t\treturn config->clt.config.bias_resistor;\n" +
                "\tif (strEqualCaseInsensitive(name, \"clt.adcChannel\"))\n" +
                "\t\treturn config->clt.adcChannel;\n" +
                "\tif (strEqualCaseInsensitive(name, \"baseFuel\"))\n" +
                "\t\treturn config->baseFuel;\n" +
                "\tif (strEqualCaseInsensitive(name, \"afr_type\"))\n" +
                "\t\treturn config->afr_type;\n" +
                "\tif (strEqualCaseInsensitive(name, \"speedToRpmRatio\"))\n" +
                "\t\treturn config->speedToRpmRatio;\n" +
                "\tif (strEqualCaseInsensitive(name, \"afr_typet\"))\n" +
                "\t\treturn config->afr_typet;\n" +
                "\tif (strEqualCaseInsensitive(name, \"vehicleSpeedKph\"))\n" +
                "\t\treturn config->vehicleSpeedKph;\n" +
                "\treturn EFI_ERROR_CODE;\n" +
                "}\n", getConfigValueConsumer.getGetterForUnitTest());

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
        DataLogConsumer dataLogConsumer = new DataLogConsumer(null);
        GaugeConsumer gaugeConsumer = new GaugeConsumer(null, state);
        state.readBufferedReader(test, Arrays.asList(dataLogConsumer, gaugeConsumer));
        assertEquals(
                "entry = alternatorStatus_iTerm, \"alternatorStatus_iTerm\", float,  \"%.3f\"\n" +
                        "entry = alternatorStatus_dTerm, \"alternatorStatus_dTerm\", float,  \"%.3f\"\n" +
                        "entry = idleStatus_iTerm, \"idleStatus_iTerm\", float,  \"%.3f\"\n" +
                        "entry = idleStatus_dTerm, \"idleStatus_dTerm\", float,  \"%.3f\"\n",
                new String(dataLogConsumer.getTsWriter().toCharArray()));

        assertEquals("alternatorStatus_iTermGauge = alternatorStatus_iTerm,\"alternatorStatus_ iTerm\", \"v\", -10000.0,10000.0, -10000.0,10000.0, -10000.0,10000.0, 4,4\n" +
                        "alternatorStatus_dTermGauge = alternatorStatus_dTerm,\"alternatorStatus_ dTerm\", \"v\", -10000.0,10000.0, -10000.0,10000.0, -10000.0,10000.0, 4,4\n" +
                        "idleStatus_iTermGauge = idleStatus_iTerm,\"idleStatus_ iTerm\", \"v\", -10000.0,10000.0, -10000.0,10000.0, -10000.0,10000.0, 4,4\n" +
                        "idleStatus_dTermGauge = idleStatus_dTerm,\"idleStatus_ dTerm\", \"v\", -10000.0,10000.0, -10000.0,10000.0, -10000.0,10000.0, 4,4\n",
                new String(gaugeConsumer.getTsWriter().toCharArray()));

    }
}
