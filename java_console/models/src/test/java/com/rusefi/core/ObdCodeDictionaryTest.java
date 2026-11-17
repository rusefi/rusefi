package com.rusefi.core;

import org.junit.jupiter.api.Test;

import java.io.IOException;
import java.io.StringReader;

import static org.junit.jupiter.api.Assertions.*;

public class ObdCodeDictionaryTest {
    @Test
    public void parse() throws IOException {
        ObdCodeDictionary dictionary = ObdCodeDictionary.parse(new StringReader(
            "#pragma once\n" +
                "unrelated = 5,\n" +
                "enum class ObdCode : uint16_t {\n" +
                "\tNone = 0,\n" +
                "//P0001 Fuel Volume Regulator Control Circuit/Open\n" +
                "\tOBD_Random_Misfire = 300,       //P0300 Random/Multiple Cylinder Misfire Detected\n" +
                "\tCUSTOM_ERR_6700 = 6700,\n" +
                "\n" +
                "\tCUSTOM_CJ125_0 = 6700,\n" +
                "\tCUSTOM_Ignition_Coil_Overcharge_1 = 9351,\n" +
                "};\n" +
                "after = 7,\n"));

        assertNull(dictionary.get(5));
        assertNull(dictionary.get(7));
        assertEquals("None", dictionary.get(0).name);
        assertEquals("OBD_Random_Misfire", dictionary.get(300).name);
        assertEquals("P0300 Random/Multiple Cylinder Misfire Detected", dictionary.get(300).comment);
        // first name wins for duplicated values
        assertEquals("CUSTOM_ERR_6700", dictionary.get(6700).name);
        assertEquals(4, dictionary.size());

        assertEquals("Random Misfire", dictionary.getDescription(300));
        assertEquals("Ignition Coil Overcharge 1", dictionary.getDescription(9351));
        assertEquals("Unknown code 1234", dictionary.getDescription(1234));
    }

    @Test
    public void formatCode() {
        assertEquals("P0335", ObdCodeDictionary.formatCode(335));
        assertEquals("P2133", ObdCodeDictionary.formatCode(2133));
        assertEquals("6700", ObdCodeDictionary.formatCode(6700));
    }

    @Test
    public void bundledFirmwareHeader() {
        ObdCodeDictionary dictionary = ObdCodeDictionary.getInstance();
        assertTrue(dictionary.size() > 100, "obd_error_codes.h resource should be bundled");
        assertEquals("OBD_Crankshaft_Position_Sensor_A_Circuit_Malfunction", dictionary.get(335).name);
    }
}
