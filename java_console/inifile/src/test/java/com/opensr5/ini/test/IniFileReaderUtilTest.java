package com.opensr5.ini.test;

import com.rusefi.ini.reader.IniFileReaderUtil;
import org.junit.jupiter.api.Test;

import static org.junit.jupiter.api.Assertions.assertArrayEquals;
import static org.junit.jupiter.api.Assertions.assertEquals;

public class IniFileReaderUtilTest {
    @Test
    public void testSplitWithEmptyUnits() {
        {
            String[] s = IniFileReaderUtil.splitTokens("\tverboseCanBaseAddress\t\t\t= \"\", 1");
            assertArrayEquals(new String[] { "verboseCanBaseAddress", "", "1" }, s);
        }
        {
            String[] s = IniFileReaderUtil.splitTokens("\tverboseCanBaseAddress\t\t\t= scalar, U32,\t756,\t\"\", 1, 0, 0, 536870911, 0");
            assertArrayEquals(new String[] { "verboseCanBaseAddress", "scalar", "U32", "756", "", "1", "0", "0", "536870911", "0" }, s);
        }
    }

    @Test
    public void testSplit() {
        {
            String[] s = IniFileReaderUtil.splitTokens("1");
            assertArrayEquals(new String[] { "1" }, s);
        }
        {
            String[] s = IniFileReaderUtil.splitTokens("hello");
            assertArrayEquals(new String[] { "hello" }, s);
        }
        {
            String[] s = IniFileReaderUtil.splitTokens("\"hello\"");
            assertArrayEquals(new String[] { "hello" }, s);
        }
        {
            String[] s = IniFileReaderUtil.splitTokens("\"hello\",\"w\"");
            assertArrayEquals(new String[] { "hello", "w" }, s);
        }
    }

    @Test
    public void testBraces() {
        {
            String[] result = IniFileReaderUtil.splitTokens(
                "veLoadBins = array, U16, 17056, [16], {bitStringValue(fuelUnits, fuelAlgorithm) }, 1, 0, 0, 1000, 0"
            );
            assertArrayEquals(new String[] {
                "veLoadBins", "array", "U16", "17056", "[16]", "{bitStringValue(fuelUnits, fuelAlgorithm) }", "1", "0",
                "0", "1000", "0"
            }, result);
        }
    }

    @Test
    public void testRealLine() {
        String[] result = IniFileReaderUtil.splitTokens("\tdialog = engineChars,\t\"Base Engine Settings\"");
        assertArrayEquals(new String[] { "dialog", "engineChars", "Base Engine Settings" }, result);
        assertEquals(result.length, 3);
    }

    @Test
    public void testQuotedTokens() {
        {
            String[] result = IniFileReaderUtil.splitTokens("\"hel  lo\"");
            assertArrayEquals(new String[] { "hel  lo" }, result);
        }
    }
}
