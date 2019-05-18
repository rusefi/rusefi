package com.rusefi.test;

import com.rusefi.ConfigDefinition;
import org.junit.Test;

import static org.junit.Assert.assertEquals;

public class ConfigDefinitionOutputTest {
    @Test
    public void testComment() {
        assertEquals("", ConfigDefinition.packComment("", "\t"));
        assertEquals("\t * abc\n", ConfigDefinition.packComment("abc", "\t"));
        assertEquals("\t * abc\n" +
                "\t * vbn\n", ConfigDefinition.packComment("abc\\nvbn", "\t"));
    }
}
