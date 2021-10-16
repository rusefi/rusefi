package com.rusefi.test;

import com.rusefi.VariableRegistry;
import org.junit.Test;

import static com.rusefi.VariableRegistry.*;
import static org.junit.Assert.assertEquals;

/**
 * 3/30/2015
 */
public class VariableRegistryTest {
    @Test
    public void testReplace() {
        VariableRegistry registry = new VariableRegistry();
        registry.register("var", 256);
        assertEquals(3, registry.size());

        // trivial key-value substitution
        assertEquals("256", registry.applyVariables("@@var@@"));
        assertEquals("ab256", registry.applyVariables("ab@@var@@"));
        assertEquals("ab256cd", registry.applyVariables("ab@@var@@cd"));
        // both decimal and hex values here
        assertEquals("aa256qwe100fff", registry.applyVariables("aa@@var@@qwe@@var" + _HEX_SUFFIX + "@@fff"));

        assertEquals("\\x01\\x00", registry.applyVariables("@@var" + _16_HEX_SUFFIX + "@@"));
    }

    @Test
    public void testCharHexUsage() {
        VariableRegistry registry = new VariableRegistry();
        registry.register("SD_r", "'r'");

        assertEquals(4, registry.size());
        assertEquals("ab72", registry.applyVariables("ab@@SD_r" + CHAR_SUFFIX + _HEX_SUFFIX +
                "@@"));
        assertEquals("ab'r'", registry.applyVariables("ab@@SD_r@@"));
        assertEquals("abr", registry.applyVariables("ab@@SD_r" + CHAR_SUFFIX + "@@"));
    }

    @Test
    public void testPrepend() {
        VariableRegistry registry = new VariableRegistry();
        registry.register("key_string", "value_1");
        assertEquals("value_1", registry.get("key_string"));

        registry.register("key_string", "value_2");
        assertEquals("value_1", registry.get("key_string"));

        registry.register("key_int", 1);
        assertEquals("1", registry.get("key_int"));
        assertEquals("1", registry.get("key_int" + _HEX_SUFFIX));

        registry.register("key_int", 2);
        assertEquals("1", registry.get("key_int"));
        assertEquals("1", registry.get("key_int" + _HEX_SUFFIX));
    }
}
