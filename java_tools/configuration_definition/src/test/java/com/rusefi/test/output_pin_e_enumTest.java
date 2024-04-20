package com.rusefi.test;

import com.rusefi.VariableRegistry;
import org.junit.jupiter.api.Test;

import java.io.IOException;
import java.util.Arrays;

import static com.rusefi.VariableRegistry.unquote;
import static com.rusefi.AssertCompatibility.assertEquals;

public class output_pin_e_enumTest {
    @Test
    public void test() throws IOException {
        VariableRegistry r = ConfigDefinitionTest.readRealConfig();
        String packed = r.get("Gpio_enum");
        String[] array = packed.split("\\s+");
        assertEquals(unquote(array[0]), "NONE");
        assertEquals(unquote(array[1]), "INVALID");
        assertEquals(unquote(array[49]), "PC15");
        assertEquals(unquote(array[81]), "PE15");
    }
}
