package com.rusefi.ui.config.test;

import com.rusefi.ui.config.IniFileModel;
import org.junit.Test;

import java.util.Arrays;

import static org.junit.Assert.assertEquals;

/**
 * (c) Andrey Belomutskiy
 * 12/23/2015.
 */
public class IniFileModelTest {
    @Test
    public void testSplit() {
        {
            String[] s = IniFileModel.split("1");
            assertEquals(s.length, 1);
        }
        {
            String[] s = IniFileModel.split("hello");
            assertEquals(s.length, 1);
        }
        {
            String[] s = IniFileModel.split("\"hello\"");
            assertEquals(s.length, 1);
        }
        {
            String[] s = IniFileModel.split("\"hello\",\"w\"");
            assertEquals(s.length, 2);
        }
    }

    @Test
    public void testQuotedTokens() {
        {
            String[] s = IniFileModel.split("\"hel  lo\"");
            assertEquals(s.length, 1);
        }
    }

    @Test
    public void testRealLine() {
        String[] s = IniFileModel.split("\tdialog = engineChars,\t\"Base Engine Settings\"");
        assertEquals(s.length, 3);
        System.out.println(Arrays.toString(s));
    }
}
