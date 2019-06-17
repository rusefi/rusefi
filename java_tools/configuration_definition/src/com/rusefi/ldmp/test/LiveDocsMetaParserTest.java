package com.rusefi.ldmp.test;

import com.rusefi.ldmp.*;
import org.junit.Test;

import java.util.List;

import static org.junit.Assert.assertEquals;

public class LiveDocsMetaParserTest {
    @Test
    public void parseConfigElements() {
        List<Request> r = LiveDocsMetaParser.parse("\t\t// TCHARGE_MODE_RPM_TPS\n" +
                "\t\tfloat minRpmKcurrentTPS = interpolateMsg(\"minRpm\", tpMin, DISPLAY_CONFIG(tChargeMinRpmMinTps), tpMax,\n" +
                "\t\t\t\t/***display*/CONFIG(tChargeMinRpmMaxTps), tps);\n" +
                "\t\tfloat maxRpmKcurrentTPS = interpolateMsg(\"maxRpm\", tpMin, DISPLAY_CONFIG(tChargeMaxRpmMinTps), tpMax,\n" +
                "\t\t\t\tDISPLAY_CONFIG(tChargeMaxRpmMaxTps), tps);\n" +
                "\n" +
                "\t\tengine->engineState.Tcharge_coff = interpolateMsg(\"Kcurr\", rpmMin, minRpmKcurrentTPS, rpmMax, maxRpmKcurrentTPS, rpm);\n");
        assertEquals(3, r.size());
        assertEquals(new ConfigRequest("tChargeMinRpmMinTps"), r.get(0));
    }

    @Test
    public void parseDisplayConfig() {
        List<Request> r = LiveDocsMetaParser.parse("\t\t// DISPLAY_TEXT(interpolate(\")\n" +
                "+\t\tDISPLAY_SENSOR(RPM) DISPLAY_TEXT(TCHARGE_MODE_RPM_TPS)\n" +
                "\t\tfloat minRpmKcurrentTPS = interpolateMsg(\"minRpm\", tpMin, DISPLAY_CONFIG(tChargeMinRpmMinTps), tpMax,\n");
        assertEquals(4, r.size());
        // implementation has eaten the bracket :(
        assertEquals(new TextRequest("interpolate"), r.get(0));
        assertEquals(new SensorRequest("RPM"), r.get(1));

        String javaCode = LiveDocsMetaParser.generateJavaCode(r, "xx");
        assertEquals("package com.rusefi.ldmp.generated;\n" +
                "\n" +
                "import com.rusefi.ldmp.*;\n" +
                "\n" +
                "public class xx {\n" +
                "\tpublic static final Request[] CONTENT = new Request[]{\n" +
                "\t\t\tnew TextRequest(\"interpolate\"),\n" +
                "/*sens*/\n" +
                "\t\t\tnew TextRequest(\"TCHARGE_MODE_RPM_TPS\"),\n" +
                "\t\t\tnew ConfigRequest(\"tChargeMinRpmMinTps\"),\n" +
                "\t};\n" +
                "}", javaCode);

    }

    @Test
    public void testField() {
        List<Request> r = LiveDocsMetaParser.parse("tm->DISPLAY_FIELD(voltageMCU) = getVoltage(\"term\", config->adcChannel);");
        assertEquals(1, r.size());
        assertEquals(new FieldRequest("voltageMCU"), r.get(0));
    }

    @Test
    public void parseIf() {
        List<Request> r = LiveDocsMetaParser.parse("\tDISPLAY_IF(cond)\t// DISPLAY_TEXT(\"interpolate(\")\n" +
                "+\t\tDISPLAY_SENSOR(RPM)" +
                "/* DISPLAY_ELSE */ DISPLAY_TEXT(\"TCHARGE_MODE_RPM_TPS\")\n" +
                "\t\tfloat minRpmKcurrentTPS = interpolateMsg(\"minRpm\", tpMin, DISPLAY_CONFIG(tChargeMinRpmMinTps), tpMax,\n" +
                "/* DISPLAY_ENDIF */");

        assertEquals(1, r.size());
        IfRequest ifRequest = (IfRequest) r.get(0);
        List<Request> trueBlock = ifRequest.trueBlock;
        assertEquals(2, trueBlock.size());
        assertEquals(new SensorRequest("RPM"), trueBlock.get(1));

        List<Request> falseBlock = ifRequest.falseBlock;
        assertEquals(2, falseBlock.size());
        assertEquals(new ConfigRequest("tChargeMinRpmMinTps"), falseBlock.get(1));
    }
}
