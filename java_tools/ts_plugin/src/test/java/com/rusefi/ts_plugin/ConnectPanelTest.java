package com.rusefi.ts_plugin;

import org.junit.Test;

import static org.junit.Assert.assertEquals;

public class ConnectPanelTest {
    @Test
    public void testFileName() {
        assertEquals("1234", ConnectPanel.getLastFour("aaa1234.a"));
        assertEquals("1234", ConnectPanel.getLastFour("a1234.a"));
        assertEquals("123", ConnectPanel.getLastFour("123.a"));
        assertEquals("1", ConnectPanel.getLastFour("1.a"));
    }
}
