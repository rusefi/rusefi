package com.opensr5.ini.test;

import com.opensr5.ini.IniFileMetaInfo;
import com.opensr5.ini.IniFileModel;
import com.opensr5.ini.IniFileReader;
import com.opensr5.ini.RawIniFile;
import com.opensr5.ini.field.ArrayIniField;
import com.opensr5.ini.field.EnumIniField;
import org.jetbrains.annotations.NotNull;
import org.junit.Test;

import java.io.ByteArrayInputStream;

import static org.junit.Assert.assertEquals;

/**
 * (c) Andrey Belomutskiy
 * 3/1/2017
 */
public class IniFileReaderTest {
    private static final String PAGE_READ = "    pageReadCommand     = \"X\",       \"X\",     \"X\"\n\n\n\n";
    private static final String SIGNATURE_UNIT_TEST = "  signature      = \"unit test\"\n";

    @Test
    public void testSplit() {
        {
            String[] s = IniFileReader.splitTokens("1");
            assertEquals(s.length, 1);
            assertEquals("1", s[0]);
        }
        {
            String[] s = IniFileReader.splitTokens("hello");
            assertEquals(s.length, 1);
            assertEquals("hello", s[0]);
        }
        {
            String[] s = IniFileReader.splitTokens("\"hello\"");
            assertEquals(s.length, 1);
            assertEquals("hello", s[0]);
        }
        {
            String[] s = IniFileReader.splitTokens("\"hello\",\"w\"");
            assertEquals(s.length, 2);
            assertEquals("hello", s[0]);
            assertEquals("w", s[1]);
        }
    }

    @Test
    public void testQuotedTokens() {
        {
            String[] result = IniFileReader.splitTokens("\"hel  lo\"");
            assertEquals(result.length, 1);
            assertEquals("hel  lo", result[0]);
        }
    }

    @Test
    public void testRealLine() {
        String[] result = IniFileReader.splitTokens("\tdialog = engineChars,\t\"Base Engine Settings\"");
        assertEquals(result.length, 3);

        assertEquals("dialog", result[0]);
        assertEquals("engineChars", result[1]);
        assertEquals("Base Engine Settings", result[2]);
    }

    @Test
    public void testTotalPagesSize() {
        String string = "   nPages              = 3\n" +
                SIGNATURE_UNIT_TEST +
                PAGE_READ +
                "   pageSize            = 288,   64,     288\n";


        IniFileMetaInfo meta = new IniFileMetaInfo(fromString(string));

        assertEquals(3, meta.getnPages());
        assertEquals(IniFileMetaInfo.DEFAULT_BLOCKING_FACTOR, meta.getBlockingFactor());
        assertEquals(640, meta.getTotalSize());
        assertEquals("unit test", meta.getSignature());

        assertEquals(64, meta.getPageSize(1));
        assertEquals("X", meta.getPageReadCommand(1));
    }

    @NotNull
    private RawIniFile fromString(String string) {
        return IniFileReader.read(new ByteArrayInputStream(string.getBytes()));
    }

    @Test
    public void testEasyFields() {
        String string = "page = 1\n" +
                "primingSquirtDurationMs\t\t\t= scalar, F32,\t96,\t\"*C\", 1, 0, -40, 200, 1\n" +
                "\tiat_adcChannel\t\t\t\t = bits, U08, 312, [0:7] \"PA0\", \"PA1\", \"PA2\", \"PA3\", \"PA4\", \"PA5\", \"PA6\", \"PA7\", \"PB0\", \"PB1\", \"PC0\", \"PC1\", \"PC2\", \"PC3\", \"PC4\", \"PC5\", \"Disabled\", \"PB12\", \"PB13\", \"PC14\", \"PC15\", \"PC16\", \"PC17\", \"PD3\", \"PD4\", \"PE2\", \"PE6\", \"INVALID\", \"INVALID\", \"INVALID\", \"INVALID\", \"INVALID\"\n";

        RawIniFile lines = IniFileReader.read(new ByteArrayInputStream(string.getBytes()));
        IniFileModel model = new IniFileModel().readIniFile(lines);

        assertEquals(2, model.allIniFields.size());
    }

    @Test
    public void testBitLogic() {
        assertEquals(1, EnumIniField.getBitRange(0xff, 0, 0));

        assertEquals(1, EnumIniField.getBitRange(0xf0, 4, 0));
        assertEquals(2, EnumIniField.getBitRange(0xf0, 3, 1));
    }

    @Test
    public void testBitField() {
        String string = "page = 1\n" +
                "\tname\t= bits,    U32,   \t744, [3:5], \"false\", \"true\"";

        RawIniFile lines = IniFileReader.read(new ByteArrayInputStream(string.getBytes()));
        IniFileModel model = new IniFileModel().readIniFile(lines);

        assertEquals(1, model.allIniFields.size());

        EnumIniField field = (EnumIniField) model.allIniFields.get("name");
        assertEquals(3, field.getBitPosition());
        assertEquals(2, field.getBitSize());
        assertEquals(2, field.getEnums().size());
    }

    @Test
    public void testCurveField() {
        String string = "page = 1\n" +
                " \tname\t\t\t= array, F32,\t108,\t[8],\t\"\", 1, 0, 0.0, 18000, 2\n";

        RawIniFile lines = IniFileReader.read(new ByteArrayInputStream(string.getBytes()));
        IniFileModel model = new IniFileModel().readIniFile(lines);

        assertEquals(1, model.allIniFields.size());
        ArrayIniField field = (ArrayIniField) model.allIniFields.get("name");
        assertEquals(1, field.getCols());
        assertEquals(8, field.getRows());
    }
}
