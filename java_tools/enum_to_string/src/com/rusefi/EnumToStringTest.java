package com.rusefi;

import org.junit.Test;

import static com.rusefi.EnumToString.isKeyValueLine;
import static org.junit.Assert.assertTrue;

public class EnumToStringTest {
    @Test
    public void testParseLine() {
        assertTrue(isKeyValueLine("MIN"));
        assertTrue(isKeyValueLine("MIN = 2,"));
        assertTrue(isKeyValueLine("MIN = -3,"));
    }
}
