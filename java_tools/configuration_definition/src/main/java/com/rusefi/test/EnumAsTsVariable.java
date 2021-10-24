package com.rusefi.test;

import com.rusefi.EnumsReader;
import com.rusefi.ReaderState;
import org.junit.Test;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.StringReader;

import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertNotNull;

public class EnumAsTsVariable {
    @Test
    public void testUseEnumAsVariable() throws IOException {
        ReaderState readerState = new ReaderState();
        readerState.read(new BufferedReader(new StringReader("" +
                "typedef enum {\n" +
                "\tFO_1 = 0,\n" +
                "\n" +
                "\t// 2 cylinder\n" +
                "\tFO_1_2 = 8,\n" +
                "Force_4b_firing_order = ENUM_32_BITS,\n" +
                "} firing_order_e;")));

        EnumsReader.EnumState state = readerState.enumsReader.getEnums().get("firing_order_e");
        assertNotNull(state);

        String data = readerState.variableRegistry.get("firing_order_e_FO_1");
        assertEquals("0", data);

        assertEquals("0", readerState.variableRegistry.applyVariables("@@firing_order_e_FO_1@@"));
        assertEquals("\\x00\\x00", readerState.variableRegistry.applyVariables("@@firing_order_e_FO_1_16_hex@@"));
    }
}
