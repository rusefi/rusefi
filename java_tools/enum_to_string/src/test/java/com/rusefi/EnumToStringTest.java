package com.rusefi;

import com.rusefi.enum_reader.Value;
import org.junit.Test;

import java.io.IOException;
import java.io.Reader;
import java.io.StringReader;
import java.util.ArrayList;
import java.util.List;

import static com.rusefi.EnumsReader.isKeyValueLine;
import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertTrue;

public class EnumToStringTest {
    public static EnumsReader process(Reader reader) throws IOException {
        EnumsReader enumsReader = new EnumsReader();
        enumsReader.process(reader);
        EnumToString.outputData(enumsReader);
        return enumsReader;
    }

    @Test
    public void testParseLine() {
        assertTrue(isKeyValueLine("MIN"));
        assertTrue(isKeyValueLine("MIN = 2,"));
        assertTrue(isKeyValueLine("MIN = -3,"));
        assertTrue(isKeyValueLine("Force_4_bytes_size_operation_mode_e = ENUM_32_BITS,"));
        assertTrue(isKeyValueLine("MIN = 0x02,"));
        assertTrue(isKeyValueLine("MIN = 0xa2,"));
        assertTrue(isKeyValueLine("MIN = 0xB2,"));
    }

    @Test
    public void parseEnum() throws IOException {
        EnumToString.clear();
        EnumsReader enumsReader = process(new StringReader(
                "typedef enum {\n" +
                        "\tGPIO_UNASSIGNED = 0,\n" +
                        "\tGPIO_INVALID = 1,\n" +
                        "\tGPIO_HEX = 0xA1,\n" +
                        "}brain_pin_e; // hello"));

        List<Value> values = new ArrayList<>(enumsReader.getEnums().get("brain_pin_e").values());
        assertEquals(3, values.size());
        Value first = values.get(0);
        assertEquals("GPIO_HEX", first.getName());
        assertEquals("0xA1", first.getValue());

        Value second = values.get(1);
        assertEquals("GPIO_INVALID", second.getName());
        assertEquals("1", second.getValue());

        assertEquals("const char *getBrain_pin_e(brain_pin_e value){\n" +
                "switch(value) {\n" +
                "case GPIO_HEX:\n" +
                "  return \"GPIO_HEX\";\n" +
                "case GPIO_INVALID:\n" +
                "  return \"GPIO_INVALID\";\n" +
                "case GPIO_UNASSIGNED:\n" +
                "  return \"GPIO_UNASSIGNED\";\n" +
                "  }\n" +
                " return NULL;\n" +
                "}\n", EnumToString.getCppFileContent());
    }

    @Test
    public void parsePackedEnum() throws IOException {
        EnumToString.clear();
        process(new StringReader(
                "typedef enum __attribute__ ((__packed__)) {\n" +
                        "\tGPIO_UNASSIGNED = 0,\n" +
                        "\tGPIO_INVALID = 1,\n" +
                        "} brain_pin_e ;"));
        assertEquals("const char *getBrain_pin_e(brain_pin_e value){\n" +
                "switch(value) {\n" +
                "case GPIO_INVALID:\n" +
                "  return \"GPIO_INVALID\";\n" +
                "case GPIO_UNASSIGNED:\n" +
                "  return \"GPIO_UNASSIGNED\";\n" +
                "  }\n" +
                " return NULL;\n" +
                "}\n", EnumToString.getCppFileContent());
    }
}
