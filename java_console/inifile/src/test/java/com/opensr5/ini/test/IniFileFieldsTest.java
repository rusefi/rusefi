package com.opensr5.ini.test;

import com.opensr5.ini.CurveModel;
import com.opensr5.ini.IniFileModel;
import com.opensr5.ini.RawIniFile;
import com.opensr5.ini.field.ArrayIniField;
import com.opensr5.ini.field.EnumIniField;
import com.opensr5.ini.field.IniField;
import com.rusefi.config.FieldType;
import com.rusefi.ini.reader.IniFileReaderUtil;
import org.junit.jupiter.api.Test;

import java.io.ByteArrayInputStream;
import java.util.Optional;

import static org.junit.jupiter.api.Assertions.*;

public class IniFileFieldsTest {
    @Test
    public void testCurve() {
        String string =
            "   ; generates [SettingContextHelp]\n" +
                "[Constants]\n" +
                "page = 1\n" +
                "scriptCurve1Bins = array, F32, 4828, [16], \"x\", 1, 0, -10000, 10000, 3\n" +
                "scriptCurve1 = array, F32, 4892, [16], \"y\", 1, 0, -10000, 10000, 3\n " +
                "[CurveEditor]\n" +
                "\tcurve = scriptCurve1, \"Script Curve #1\"\n" +
                "\t\tcolumnLabel = \"X\", \"Y\"\n" +
                "\t\txAxis\t\t=  0, 128, 10\n" +
                "\t\tyAxis\t\t= -155,  150, 12\n" +
                "\t\txBins\t\t= scriptCurve1Bins\n" +
                "\t\tyBins\t\t= scriptCurve1\n" +
                "\t\tshowTextValues = true\n";
        RawIniFile lines = IniFileReaderUtil.read(new ByteArrayInputStream(string.getBytes()));
        IniFileModel model = IniFileReaderTest.readLines(lines);
        // technically these are constants?!
        assertEquals(2, model.getAllIniFields().size());
        assertEquals(2, model.getFieldsInUiOrder().size());

        assertNotNull(model.getCurves());
        CurveModel curve = model.getCurves().get("scriptCurve1");
        assertNotNull(curve);
        assertEquals(0, curve.getxAxis().getMin());
        assertEquals(128, curve.getxAxis().getMax());
        assertEquals(10, curve.getxAxis().getStep());

        assertEquals(-155, curve.getyAxis().getMin());
        assertEquals(150, curve.getyAxis().getMax());
        assertEquals(12, curve.getyAxis().getStep());
    }

    @Test
    public void testEasyFields() {
        String string = "page = 1\n" +
            "[Constants]\n" +
            "primingSquirtDurationMs\t\t\t= scalar, F32,\t96,\t\"*C\", 1, 0, -40, 200, 1\n" +
            "\tiat_adcChannel\t\t\t\t = bits, U08, 312, [0:7] \"PA0\", \"PA1\", \"PA2\", \"PA3\", \"PA4\", \"PA5\", \"PA6\", \"PA7\", \"PB0\", \"PB1\", \"PC0\", \"PC1\", \"PC2\", \"PC3\", \"PC4\", \"PC5\", \"Disabled\", \"PB12\", \"PB13\", \"PC14\", \"PC15\", \"PC16\", \"PC17\", \"PD3\", \"PD4\", \"PE2\", \"PE6\", \"INVALID\", \"INVALID\", \"INVALID\", \"INVALID\", \"INVALID\"\n";

        RawIniFile lines = IniFileReaderUtil.read(new ByteArrayInputStream(string.getBytes()));
        IniFileModel model = IniFileReaderTest.readLines(lines);

        assertEquals(2, model.getAllIniFields().size());
    }

    @Test
    public void testEnumListFields() {
        String string = "#define gpio_list=\"NONE\", \"INVALID\", \"PA0\", \"PA1\", \"PA2\", \"PA3\", \"PA4\"\n" +
            "page = 1\n" +
            "[Constants]\n" +
            "primingSquirtDurationMs\t\t\t= scalar, F32,\t96,\t\"*C\", 1, 0, -40, 200, 1\n" +
            "\tiat_adcChannel\t\t\t\t = bits, U08, 312, [0:7] $gpio_list\n";

        RawIniFile lines = IniFileReaderUtil.read(new ByteArrayInputStream(string.getBytes()));
        IniFileModel model = IniFileReaderTest.readLines(lines);
        assertEquals(1, model.getDefines().size());

        EnumIniField field = (EnumIniField) model.getAllIniFields().get("iat_adcChannel");
        assertEquals(7, field.getEnums().size());
        assertEquals(2, model.getAllIniFields().size());
    }

    @Test
    public void testBitField() {
        String string = "page = 1\n" +
            "[Constants]\n" +
            "\tname\t= bits,    U32,   \t744, [3:5], \"false\", \"true\"";

        RawIniFile lines = IniFileReaderUtil.read(new ByteArrayInputStream(string.getBytes()));
        IniFileModel model = IniFileReaderTest.readLines(lines);

        assertEquals(1, model.getAllIniFields().size());

        EnumIniField field = (EnumIniField) model.getAllIniFields().get("name");
        assertEquals(3, field.getBitPosition());
        assertEquals(2, field.getBitSize0());
        assertEquals(2, field.getEnums().size());
    }

    @Test
    public void testCurveField() {
        String string = "page = 1\n" +
            " \tname2\t\t\t= array, F32,\t108,\t[8],\t\"\", 1, 0, 0.0, 18000, 2\n" +
            "[Constants]\n" +
            " \tname\t\t\t= array, F32,\t108,\t[8],\t\"\", 1, 0, 0.0, 18000, 2\n" +
            "[PcVariables]\n" +
            " \tname3\t\t\t= array, F32,\t108,\t[8],\t\"\", 1, 0, 0.0, 18000, 2\n";

        RawIniFile lines = IniFileReaderUtil.read(new ByteArrayInputStream(string.getBytes()));
        IniFileModel model = IniFileReaderTest.readLines(lines);

        assertEquals(1, model.getAllIniFields().size());
        ArrayIniField field = (ArrayIniField) model.getAllIniFields().get("name");
        assertNotNull(field);
        assertEquals(1, field.getCols());
        assertEquals(8, field.getRows());
    }

    @Test
    public void testPcVariableBitsField() {
        String string = "page = 1\n" +
            "[Constants]\n" +
            "primingSquirtDurationMs\t\t\t= scalar, F32,\t96,\t\"*C\", 1, 0, -40, 200, 1\n" +
            "[PcVariables]\n" +
            "\tdisplayMode\t= bits, U08, [0:7], \"Mode A\", \"Mode B\", \"Mode C\"\n";

        RawIniFile lines = IniFileReaderUtil.read(new ByteArrayInputStream(string.getBytes()));
        IniFileModel model = IniFileReaderTest.readLines(lines);

        // PcVariables bits field should NOT be in primary fields
        assertEquals(1, model.getAllIniFields().size());
        assertNull(model.getAllIniFields().get("displayMode"));

        // It should be in secondary fields
        assertTrue(model.getSecondaryIniFields().containsKey("displayMode"));
        EnumIniField field = (EnumIniField) model.getSecondaryIniFields().get("displayMode");
        assertNotNull(field);
        assertEquals("displayMode", field.getName());
        assertEquals(0, field.getOffset()); // PcVariables always use offset 0
        assertEquals(FieldType.UINT8, field.getType());
        assertEquals(0, field.getBitPosition());
        assertEquals(7, field.getBitSize0());
        assertEquals(3, field.getEnums().size());
        assertEquals("Mode A", field.getEnums().get(0));
        assertEquals("Mode B", field.getEnums().get(1));
        assertEquals("Mode C", field.getEnums().get(2));

        // It should be findable via findIniField()
        Optional<IniField> found = model.findIniField("displayMode");
        assertTrue(found.isPresent());
        assertSame(field, found.get());
    }

    @Test
    public void testPcVariableIgnoresNonBitsType() {
        String string = "page = 1\n" +
            "[PcVariables]\n" +
            "\tscalarVar\t= scalar, F32, 0, \"unit\", 1, 0, 0, 100, 1\n" +
            "\tbitsVar\t= bits, U08, [0:1], \"Off\", \"On\"\n";

        RawIniFile lines = IniFileReaderUtil.read(new ByteArrayInputStream(string.getBytes()));
        IniFileModel model = IniFileReaderTest.readLines(lines);

        // Only bits type should be parsed; scalar should be ignored
        assertEquals(0, model.getAllIniFields().size());
        assertEquals(1, model.getSecondaryIniFields().size());
        assertTrue(model.getSecondaryIniFields().containsKey("bitsVar"));
        assertFalse(model.getSecondaryIniFields().containsKey("scalarVar"));
    }

    @Test
    public void testPcVariableShortLineIgnored() {
        String string = "page = 1\n" +
            "[PcVariables]\n" +
            "\ttooShort\t= bits, U08\n" +
            "\tgoodVar\t= bits, U08, [0:3], \"A\", \"B\"\n";

        RawIniFile lines = IniFileReaderUtil.read(new ByteArrayInputStream(string.getBytes()));
        IniFileModel model = IniFileReaderTest.readLines(lines);

        // Line with fewer than 4 tokens should be ignored
        assertEquals(1, model.getSecondaryIniFields().size());
        assertTrue(model.getSecondaryIniFields().containsKey("goodVar"));
    }
}
