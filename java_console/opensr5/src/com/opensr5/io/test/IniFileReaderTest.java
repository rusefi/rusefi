package com.opensr5.io.test;

import com.opensr5.io.IniFileMetaInfo;
import com.opensr5.io.IniFileReader;
import com.opensr5.io.RawIniFile;
import org.junit.Test;

import java.io.ByteArrayInputStream;

import static org.junit.Assert.assertEquals;

/**
 * (c) Andrey Belomutskiy
 * 3/1/2017
 */
public class IniFileReaderTest {
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
                "  signature      = \"unit test\"\n" +
                "    pageReadCommand     = \"X\",       \"X\",     \"X\"\n\n\n\n" +
                "   pageSize            = 288,   64,     288\n";


        RawIniFile content = IniFileReader.read(new ByteArrayInputStream(string.getBytes()));

        IniFileMetaInfo meta = new IniFileMetaInfo(content);

        assertEquals(3, meta.getnPages());
        assertEquals(IniFileMetaInfo.DEFAULT_BLOCKING_FACTOR, meta.getBlockingFactor());
        assertEquals(640, meta.getTotalSize());
        assertEquals("unit test", meta.getSignature());

        assertEquals(64, meta.getPageSize(1));
        assertEquals("X", meta.getPageReadCommand(1));
    }
}
