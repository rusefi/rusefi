package com.opensr5.io.test;

import com.opensr5.io.IniFileReader;
import org.junit.Test;

import static org.junit.Assert.assertEquals;

/**
 * (c) Andrey Belomutskiy
 * 3/1/2017
 */
public class IniFileReaderTest {
    @Test
    public void testSplit() {
        {
            String[] s = IniFileReader.split("1");
            assertEquals(s.length, 1);
            assertEquals("1", s[0]);
        }
        {
            String[] s = IniFileReader.split("hello");
            assertEquals(s.length, 1);
            assertEquals("hello", s[0]);
        }
        {
            String[] s = IniFileReader.split("\"hello\"");
            assertEquals(s.length, 1);
            assertEquals("hello", s[0]);
        }
        {
            String[] s = IniFileReader.split("\"hello\",\"w\"");
            assertEquals(s.length, 2);
            assertEquals("hello", s[0]);
            assertEquals("w", s[1]);
        }
    }
    @Test
    public void testQuotedTokens() {
        {
            String[] result = IniFileReader.split("\"hel  lo\"");
            assertEquals(result.length, 1);
            assertEquals("hel  lo", result[0]);
        }
    }

    @Test
    public void testRealLine() {
        String[] result = IniFileReader.split("\tdialog = engineChars,\t\"Base Engine Settings\"");
        assertEquals(result.length, 3);

        assertEquals("dialog", result[0]);
        assertEquals("engineChars", result[1]);
        assertEquals("Base Engine Settings", result[2]);
    }
}
