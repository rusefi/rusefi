package com.rusefi.ui.config.test;

import com.rusefi.ui.config.IniFileModel;
import org.junit.Test;

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
}
