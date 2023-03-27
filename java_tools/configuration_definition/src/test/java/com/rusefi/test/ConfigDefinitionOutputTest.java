package com.rusefi.test;

import com.rusefi.output.BaseCHeaderConsumer;
import org.junit.Test;

import static org.junit.Assert.assertEquals;

public class ConfigDefinitionOutputTest {
    @Test
    public void testComment() {
        assertEquals("", BaseCHeaderConsumer.packComment("", "\t// "));
        assertEquals("\t// abc\n", BaseCHeaderConsumer.packComment("abc", "\t// "));
        assertEquals("\t// abc\n" +
                "\t// vbn\n", BaseCHeaderConsumer.packComment("abc\\nvbn", "\t// "));
    }
}
