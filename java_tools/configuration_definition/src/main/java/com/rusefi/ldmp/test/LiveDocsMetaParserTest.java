package com.rusefi.ldmp.test;

import com.rusefi.ldmp.*;
import org.junit.Test;

import java.util.List;

import static org.junit.Assert.assertEquals;

public class LiveDocsMetaParserTest {
    public static MetaInfo parse(String s) {
        return LiveDocsMetaParser.parse(s, "unit_test");
    }

    @Test
    public void getClassName() {
        assertEquals("TpsMeta", LiveDocsMetaParser.getClassName("controllers/sensors/tps.cpp"));
        assertEquals("SpeedDensityMeta", LiveDocsMetaParser.getClassName("speed_density.cpp"));
    }

    @Test
    public void parseConfigElements() {
        MetaInfo r = parse("\t\t// TCHARGE_MODE_RPM_TPS\n" +
                "\t\tfloat minRpmKcurrentTPS = interpolateMsg(\"minRpm\", tpMin, DISPLAY_CONFIG(tChargeMinRpmMinTps), tpMax,\n" +
                "\t\t\t\t/***display*/CONFIG(tChargeMinRpmMaxTps), tps);\n" +
                "\t\tfloat maxRpmKcurrentTPS = interpolateMsg(\"maxRpm\", tpMin, DISPLAY_CONFIG(tChargeMaxRpmMinTps), tpMax,\n" +
                "\t\t\t\tDISPLAY_CONFIG(tChargeMaxRpmMaxTps), tps);\n" +
                "\n" +
                "\t\tengine->engineState.Tcharge_coff = interpolateMsg(\"Kcurr\", rpmMin, minRpmKcurrentTPS, rpmMax, maxRpmKcurrentTPS, rpm);\n");
        assertEquals(3, r.first().size());
        assertEquals(new ConfigRequest("tChargeMinRpmMinTps"), r.first().get(0));
    }

    @Test
    public void parseField() {
        MetaInfo r = parse(
                "DISPLAY_STATE(tps)\n" +
                        "\tDISPLAY_TEXT(Analog_MCU_reads);\n" +
                "\tengine->engineState.DISPLAY_FIELD(currentTpsAdc) = adc;\n" +
                "\tDISPLAY_TEXT(ADC_which_equals);\n");
        assertEquals(3, r.first().size());
    }


    @Test
    public void parseTextWithSpecialCharacters() {
        MetaInfo r = parse(
                "DISPLAY_TEXT(\"Analog: !MCU_reads\");"
        );
        assertEquals(1, r.first().size());
        TextRequest request = (TextRequest) r.first().get(0);
        assertEquals("Analog: !MCU_reads", request.getValue());
    }

    @Test
    public void parseTextWithSpecialCharactersAndSpaces() {
        MetaInfo r = parse(
                "DISPLAY_TEXT(\"Analog: !MCU_reads\");"
        );
        assertEquals(1, r.first().size());
        TextRequest request = (TextRequest) r.first().get(0);
        assertEquals("Analog: !MCU_reads", request.getValue());
    }

    @Test
    public void parseDisplayConfig() {
        MetaInfo r = parse("\t\t// DISPLAY_TEXT(interpolate(\")\n" +
                "+\t\tDISPLAY_SENSOR(RPM) DISPLAY_TEXT(TCHARGE_MODE_RPM_TPS)\n" +
                "\t\tfloat minRpmKcurrentTPS = interpolateMsg(\"minRpm\", tpMin, DISPLAY_CONFIG(tChargeMinRpmMinTps), tpMax,\n");
        assertEquals(4, r.first().size());
        // implementation has eaten the bracket :(
        assertEquals(new TextRequest("interpolate"), r.first().get(0));
        assertEquals(new SensorRequest("RPM"), r.first().get(1));

        String javaCode = LiveDocsMetaParser.generateJavaCode(r, "xx");
        assertEquals("package com.rusefi.ldmp.generated;\n" +
                "\n" +
                "import com.rusefi.ldmp.*;\n" +
                "\n" +
                "public class xx {\n" +
                "\tpublic static final Request[] CONTENT = new Request[]{\n" +
                "\t\t\tnew TextRequest(\"interpolate\"),\n" +
                "\t\t\tnew SensorRequest(\"RPM\"),\n" +
                "\t\t\tnew TextRequest(\"TCHARGE_MODE_RPM_TPS\"),\n" +
                "\t\t\tnew ConfigRequest(\"tChargeMinRpmMinTps\"),\n" +
                "\t};\n" +
                "}", javaCode);

    }

    @Test
    public void testField() {
        MetaInfo r = parse(
                "DISPLAY_STATE(tps)\n" +
                        "tm->DISPLAY_FIELD(voltageMCU) = getVoltage(\"term\", config->adcChannel);\n" +

                        "DISPLAY_tag(tag) DISPLAY_FIELD(voltageMCU2)");
        assertEquals(1, r.first().size());
        assertEquals(new FieldRequest("tps", "voltageMCU"), r.first().get(0));

        assertEquals(2, r.map.size());
    }

    @Test
    public void parseIf() {
        MetaInfo r = parse("\tDisPLAY_IF(cond)\t// DISPLAY_TEXT(\"interpolate(\")\n" +
                "+\t\tDISPLAY_SENSOR(RPM)" +
                "/* DISPLAY_ElsE */ DISPLAY_TEXT(\"TCHARGE_MODE_RPM_TPS\")\n" +
                "\t\tfloat minRpmKcurrentTPS = interpolateMsg(\"minRpm\", tpMin, DISPLAY_CONFIG(tChargeMinRpmMinTps), tpMax,\n" +
                "/* DISPLAY_ENDIF */");

        assertEquals(1, r.first().size());
        IfRequest ifRequest = (IfRequest) r.first().get(0);
        List<Request> trueBlock = ifRequest.trueBlock;
        assertEquals(2, trueBlock.size());
        assertEquals(new SensorRequest("RPM"), trueBlock.get(1));

        List<Request> falseBlock = ifRequest.falseBlock;
        assertEquals(2, falseBlock.size());
        assertEquals(new ConfigRequest("tChargeMinRpmMinTps"), falseBlock.get(1));
    }
}
