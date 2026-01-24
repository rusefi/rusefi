package com.opensr5.ini.test;

import com.opensr5.ini.IniFileModel;
import com.opensr5.ini.RawIniFile;
import com.opensr5.ini.field.ArrayIniField;
import com.opensr5.ini.field.EnumIniField;
import com.rusefi.ini.reader.IniFileReaderUtil;
import org.junit.jupiter.api.Test;

import java.io.ByteArrayInputStream;

import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertNotNull;

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
                "\t\tyAxis\t\t= -155,  150, 10\n" +
                "\t\txBins\t\t= scriptCurve1Bins\n" +
                "\t\tyBins\t\t= scriptCurve1\n" +
                "\t\tshowTextValues = true\n";
        RawIniFile lines = IniFileReaderUtil.read(new ByteArrayInputStream(string.getBytes()));
        IniFileModel model = IniFileReaderTest.readLines(lines);
        // technically these are constants?!
        assertEquals(2, model.getAllIniFields().size());
        assertEquals(2, model.getFieldsInUiOrder().size());
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
}
