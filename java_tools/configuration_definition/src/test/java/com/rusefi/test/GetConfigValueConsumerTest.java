package com.rusefi.test;

import com.rusefi.MaybeSemicolorWasMissedException;
import com.rusefi.ReaderStateImpl;
import com.rusefi.output.GetConfigValueConsumer;
import org.junit.Test;

import static org.junit.Assert.assertEquals;

public class GetConfigValueConsumerTest {
    @Test
    public void testStructArrayAndCharArgument() {
        ReaderStateImpl state = new ReaderStateImpl();
        String test = "struct total\n" +
                "custom lua_script_t 200 string, ASCII, @OFFSET@, 200\n" +
                "lua_script_t luaScript;\n" +
                "struct dc_io\n" +
                "\tint disablePin;\n" +
                "end_struct\n" +
                "\tdc_io[2 iterate] etbIn\n" +
                "end_struct\n";
        GetConfigValueConsumer getConfigValueConsumer = new GetConfigValueConsumer();
        state.readBufferedReader(test, getConfigValueConsumer);

        assertEquals(
                "float getConfigValueByName(const char *name) {\n" +
                        "\treturn EFI_ERROR_CODE;\n" +
                        "}\n", getConfigValueConsumer.getCompleteGetterBody());
    }

    @Test
    public void generateEmbeddedStruct() {
        ReaderStateImpl state = new ReaderStateImpl();
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
        GetConfigValueConsumer getConfigValueConsumer = new GetConfigValueConsumer();
        state.readBufferedReader(test, getConfigValueConsumer);

        assertEquals("\tint hash = djb2lowerCase(name);\n" +
                "\tswitch(hash) {\n" +
                "\t\tcase -672272162:\n" +
                "\t{\n" +
                "\t\tconfig->iat.config.tempC_1 = value;\n" +
                "\t\treturn;\n" +
                "\t}\n" +
                "\t\tcase -1237776078:\n" +
                "\t{\n" +
                "\t\tconfig->iat.adcChannel = (int)value;\n" +
                "\t\treturn;\n" +
                "\t}\n" +
                "\t}\n", getConfigValueConsumer.getSetterBody());

        assertEquals("#include \"pch.h\"\n" +
                "#include \"value_lookup.h\"\n" +
                "float getConfigValueByName(const char *name) {\n" +
                "\tint hash = djb2lowerCase(name);\n" +
                "\tswitch(hash) {\n" +
                "\t\tcase -672272162:\n" +
                "\t\t\treturn config->iat.config.tempC_1;\n" +
                "\t\tcase -1237776078:\n" +
                "\t\t\treturn config->iat.adcChannel;\n" +
                "\t}\n" +
                "\treturn EFI_ERROR_CODE;\n" +
                "}\n" +
                "void setConfigValueByName(const char *name, float value) {\n" +
                "\tint hash = djb2lowerCase(name);\n" +
                "\tswitch(hash) {\n" +
                "\t\tcase -672272162:\n" +
                "\t{\n" +
                "\t\tconfig->iat.config.tempC_1 = value;\n" +
                "\t\treturn;\n" +
                "\t}\n" +
                "\t\tcase -1237776078:\n" +
                "\t{\n" +
                "\t\tconfig->iat.adcChannel = (int)value;\n" +
                "\t\treturn;\n" +
                "\t}\n" +
                "\t}\n" +
                "}\n", getConfigValueConsumer.getContent());


        assertEquals("float getConfigValueByName(const char *name) {\n" +
                "\tint hash = djb2lowerCase(name);\n" +
                "\tswitch(hash) {\n" +
                "\t\tcase -672272162:\n" +
                "\t\t\treturn config->iat.config.tempC_1;\n" +
                "\t\tcase -1237776078:\n" +
                "\t\t\treturn config->iat.adcChannel;\n" +
                "\t}\n" +
                "\treturn EFI_ERROR_CODE;\n" +
                "}\n", getConfigValueConsumer.getCompleteGetterBody());
    }

    @Test
    public void generateGetConfig() {
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
                "\tfloat flow;This is your injector flow at the fuel pressure used in the vehicle. cc/min, cubic centimetre per minute\\nBy the way, g/s = 0.125997881 * (lb/hr)\\ng/s = 0.125997881 * (cc/min)/10.5\\ng/s = 0.0119997981 * cc/min;\"cm3/min\", 1, 0, 0, 99999, 2\n" +
                "\n" +
                "float[8] battLagCorr;ms delay between injector open and close dead times;\"ms\", 1, 0, 0, 50, 2\n" +
                "\n" +
                "end_struct\n" +
                "\n" +
                "injector_s injector\n" +
                "\tint[12 iterate] ignitionPins;\n" +
                "\tfloat bias_resistor;Pull-up resistor value on your board;\"Ohm\", 1, 0, 0, 200000, 1\n" +
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
                "bit enableFan1WithAc;Turn on this fan when AC is on.\n" +
                "end_struct\n";
        ReaderStateImpl state = new ReaderStateImpl();
        state.getVariableRegistry().register("PACK_MULT_PERCENT", 100);
        state.getVariableRegistry().register("GAUGE_NAME_FUEL_BASE", "hello");


        GetConfigValueConsumer getConfigValueConsumer = new GetConfigValueConsumer();
        state.readBufferedReader(test, getConfigValueConsumer);

        assertEquals("#include \"pch.h\"\n" +
                "#include \"value_lookup.h\"\n" +
                "float getConfigValueByName(const char *name) {\n" +
                "\tint hash = djb2lowerCase(name);\n" +
                "\tswitch(hash) {\n" +
                "\t\tcase -1832527325:\n" +
                "\t\t\treturn config->clt.config.tempC_1;\n" +
                "\t\tcase 1819278123:\n" +
                "\t\t\treturn config->clt.config.map.sensor.highValue;\n" +
                "\t\tcase 581685574:\n" +
                "\t\t\treturn config->clt.config.map.sensor.hwChannel;\n" +
                "\t\tcase 382574846:\n" +
                "\t\t\treturn config->clt.config.injector.flow;\n" +
                "\t\tcase -653172717:\n" +
                "\t\t\treturn config->clt.config.bias_resistor;\n" +
                "\t\tcase -1144186889:\n" +
                "\t\t\treturn config->clt.adcChannel;\n" +
                "\t\tcase -1571463185:\n" +
                "\t\t\treturn config->issue_294_31;\n" +
                "\t\tcase 727098956:\n" +
                "\t\t\treturn config->baseFuel;\n" +
                "\t\tcase -1120008897:\n" +
                "\t\t\treturn config->afr_type;\n" +
                "\t\tcase -685727673:\n" +
                "\t\t\treturn config->speedToRpmRatio;\n" +
                "\t\tcase 1694412179:\n" +
                "\t\t\treturn config->afr_typet;\n" +
                "\t\tcase -1925174695:\n" +
                "\t\t\treturn config->vehicleSpeedKph;\n" +
                "\t\tcase -617915487:\n" +
                "\t\t\treturn config->isForcedInduction;\n" +
                "\t\tcase -298185774:\n" +
                "\t\t\treturn config->enableFan1WithAc;\n" +
                "\t}\n" +
                "\treturn EFI_ERROR_CODE;\n" +
                "}\n", getConfigValueConsumer.getHeaderAndGetter());


        assertEquals("### clt.config.tempC_1\n" +
                "these values are in Celcius\n" +
                "\n" +
                "### clt.config.map.sensor.highValue\n" +
                "kPa value at high volts\n" +
                "\n" +
                "### clt.config.map.sensor.hwChannel\n" +
                "\n" +
                "\n" +
                "### clt.config.injector.flow\n" +
                "This is your injector flow at the fuel pressure used in the vehicle. cc/min, cubic centimetre per minute\\nBy the way, g/s = 0.125997881 * (lb/hr)\\ng/s = 0.125997881 * (cc/min)/10.5\\ng/s = 0.0119997981 * cc/min\n" +
                "\n" +
                "### clt.config.bias_resistor\n" +
                "Pull-up resistor value on your board\n" +
                "\n" +
                "### clt.adcChannel\n" +
                "\n" +
                "\n" +
                "### issue_294_31\n" +
                "\n" +
                "\n" +
                "### baseFuel\n" +
                "@@GAUGE_NAME_FUEL_BASE@@\\nThis is the raw value we take from the fuel map or base fuel algorithm, before the corrections\n" +
                "\n" +
                "### afr_type\n" +
                "PID dTime\n" +
                "\n" +
                "### speedToRpmRatio\n" +
                "s2rpm\n" +
                "\n" +
                "### afr_typet\n" +
                "\n" +
                "\n" +
                "### vehicleSpeedKph\n" +
                "\n" +
                "\n" +
                "### isForcedInduction\n" +
                "Does the vehicle have a turbo or supercharger?\n" +
                "\n" +
                "### enableFan1WithAc\n" +
                "Turn on this fan when AC is on.\n" +
                "\n", getConfigValueConsumer.getMdContent());
    }

    @Test(expected = MaybeSemicolorWasMissedException.class)
    public void generateSuspiciousTsInfo() {
        String test = "struct total\n" +
                "uint8_t hello;\"unit\", 1, 0, 0, 100, 0\n" +
                "end_struct\n";
        ReaderStateImpl state = new ReaderStateImpl();


        GetConfigValueConsumer getConfigValueConsumer = new GetConfigValueConsumer();
        state.readBufferedReader(test, getConfigValueConsumer);
    }
}
