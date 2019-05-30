package com.rusefi;

import com.rusefi.enum_reader.Value;
import org.junit.Test;

import java.io.IOException;
import java.io.StringReader;
import java.util.Set;

import static com.rusefi.EnumsReader.isKeyValueLine;
import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertTrue;

public class EnumToStringTest {
    @Test
    public void testParseLine() {
        assertTrue(isKeyValueLine("MIN"));
        assertTrue(isKeyValueLine("MIN = 2,"));
        assertTrue(isKeyValueLine("MIN = -3,"));
    }

    @Test
    public void parseEnum() throws IOException {
        EnumToString.clear();
        EnumToString.process(new StringReader(
                "typedef enum {\n" +
                        "\tGPIO_UNASSIGNED = 0,\n" +
                        "\tGPIO_INVALID = 1,\n" +
                        "}brain_pin_e;"));

        Set<Value> values = EnumsReader.enums.get("brain_pin_e");

        assertEquals("const char *getBrain_pin_e(brain_pin_e value){\r\n" +
                "switch(value) {\r\n" +
                "case GPIO_INVALID:\r\n" +
                "  return \"GPIO_INVALID\";\r\n" +
                "case GPIO_UNASSIGNED:\r\n" +
                "  return \"GPIO_UNASSIGNED\";\r\n" +
                "  }\r\n" +
                " return NULL;\r\n" +
                "}\r\n", EnumToString.cppFileContent.toString());
    }

    @Test
    public void parsePackedEnum() throws IOException {
        EnumToString.clear();
        EnumToString.process(new StringReader(
                "typedef enum __attribute__ ((__packed__)) {\n" +
                        "\tGPIO_UNASSIGNED = 0,\n" +
                        "\tGPIO_INVALID = 1,\n" +
                        "} brain_pin_e ;"));
        assertEquals("const char *getBrain_pin_e(brain_pin_e value){\r\n" +
                "switch(value) {\r\n" +
                "case GPIO_INVALID:\r\n" +
                "  return \"GPIO_INVALID\";\r\n" +
                "case GPIO_UNASSIGNED:\r\n" +
                "  return \"GPIO_UNASSIGNED\";\r\n" +
                "  }\r\n" +
                " return NULL;\r\n" +
                "}\r\n", EnumToString.cppFileContent.toString());
    }
}
