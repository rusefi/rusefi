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

    @Test
    public void testExpressionsWithQuotesInsideBraces() {
        {
            // Test ternary expression with string literals inside braces
            String[] result = IniFileReaderUtil.splitTokens(
                "lambdaTable = array, U08, 16348, [16x16], {useLambdaOnInterface ? \"lambda\" : \"afr\"}, 0.1, 0, {useLambdaOnInterface ? 1.5 : 0}, {useLambdaOnInterface ? 1.5 : 25}, {useLambdaOnInterface ? 2 : 1}"
            );
            assertArrayEquals(new String[] {
                "lambdaTable", "array", "U08", "16348", "[16x16]",
                "{useLambdaOnInterface ? \"lambda\" : \"afr\"}",
                "0.1", "0",
                "{useLambdaOnInterface ? 1.5 : 0}",
                "{useLambdaOnInterface ? 1.5 : 25}",
                "{useLambdaOnInterface ? 2 : 1}"
            }, result);
        }
        {
            // Test simple expression with quotes inside braces
            String[] result = IniFileReaderUtil.splitTokens("field = {someVar ? \"yes\" : \"no\"}");
            assertArrayEquals(new String[] { "field", "{someVar ? \"yes\" : \"no\"}" }, result);
        }
        {
            // Test nested braces with quotes
            String[] result = IniFileReaderUtil.splitTokens("value = {outer ? {inner ? \"a\" : \"b\"} : \"c\"}");
            assertArrayEquals(new String[] { "value", "{outer ? {inner ? \"a\" : \"b\"} : \"c\"}" }, result);
        }
        {
            // Test expression with function call and quotes
            String[] result = IniFileReaderUtil.splitTokens("units = {bitStringValue(mode, \"on\", \"off\")}");
            assertArrayEquals(new String[] { "units", "{bitStringValue(mode, \"on\", \"off\")}" }, result);
        }
    }
}
