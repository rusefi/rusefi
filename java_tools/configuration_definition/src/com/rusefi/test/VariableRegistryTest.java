package com.rusefi.test;

import com.rusefi.VariableRegistry;
import org.junit.Test;

import static org.junit.Assert.assertEquals;

/**
 * 3/30/2015
 */
public class VariableRegistryTest {
    @Test
    public void testReplace() {
        VariableRegistry.INSTANCE.clear();

        VariableRegistry.INSTANCE.register("var", 256);

        assertEquals("256", VariableRegistry.INSTANCE.processLine("@@var@@"));
        assertEquals("ab256", VariableRegistry.INSTANCE.processLine("ab@@var@@"));
        assertEquals("ab256cd", VariableRegistry.INSTANCE.processLine("ab@@var@@cd"));
        assertEquals("aa256qwe100fff", VariableRegistry.INSTANCE.processLine("aa@@var@@qwe@@var_hex@@fff"));
    }
}
