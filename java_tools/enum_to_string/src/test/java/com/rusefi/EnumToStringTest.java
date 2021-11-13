package com.rusefi;

import com.rusefi.enum_reader.Value;

import org.junit.Test;

import java.io.IOException;
import java.io.StringReader;
import java.util.ArrayList;
import java.util.List;
import java.util.Map;

import static com.rusefi.EnumsReader.isKeyValueLine;
import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertTrue;

public class EnumToStringTest {
    public static EnumToString process(EnumsReader enumsReader) {
        EnumToString enumToString = new EnumToString();
        enumToString.outputData(enumsReader);
        return enumToString;
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
        final StringReader reader = new StringReader(
                "typedef enum {\n" +
                        "\tGPIO_UNASSIGNED = 0,\n" +
                        "\tGPIO_INVALID = 1,\n" +
                        "\tGPIO_HEX = 0xA1,\n" +
                        "}brain_pin_e; // hello");
        EnumsReader enumsReader = new EnumsReader().read(reader);
        EnumToString enumToString = process(enumsReader);

        List<Value> values = new ArrayList<>(enumsReader.getEnums().get("brain_pin_e").values.values());
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
                "}\n", enumToString.getCppFileContent());
    }

    @Test
    public void parsePackedEnum() throws IOException {
        final StringReader reader = new StringReader(
                "typedef enum __attribute__ ((__packed__)) {\n" +
                        "\tGPIO_UNASSIGNED = 0,\n" +
                        "\tGPIO_INVALID = 1,\n" +
                        "} brain_pin_e ;");
        EnumsReader enumsReader = new EnumsReader().read(reader);
        EnumToString enumToString = process(enumsReader);
        assertEquals("const char *getBrain_pin_e(brain_pin_e value){\n" +
                "switch(value) {\n" +
                "case GPIO_INVALID:\n" +
                "  return \"GPIO_INVALID\";\n" +
                "case GPIO_UNASSIGNED:\n" +
                "  return \"GPIO_UNASSIGNED\";\n" +
                "  }\n" +
                " return NULL;\n" +
                "}\n", enumToString.getCppFileContent());
    }

    @Test
    public void parsePackedFancyEnum() throws IOException {
        final StringReader reader = new StringReader(
                "enum class myEnum : uint8_t {\n" +
                        "\tGPIO_UNASSIGNED = 0,\n" +
                        "\tGPIO_INVALID = 1,\n" +
                        "} brain_pin_e ;");
        EnumsReader enumsReader = new EnumsReader().read(reader);
        EnumToString enumToString = process(enumsReader);
        assertEquals("const char *getMyEnum(myEnum value){\n" +
                "switch(value) {\n" +
                "case myEnum::GPIO_INVALID:\n" +
                "  return \"GPIO_INVALID\";\n" +
                "case myEnum::GPIO_UNASSIGNED:\n" +
                "  return \"GPIO_UNASSIGNED\";\n" +
                "  }\n" +
                " return NULL;\n" +
                "}\n", enumToString.getCppFileContent());
    }

    @Test
    public void parseEnumWithoutExplicitValues() throws IOException {
        final StringReader reader = new StringReader(
                "typedef enum {\n" +
                        "\tGPIO_UNASSIGNED,\n" +
                        "\tGPIO_INVALID,\n" +
                        "\tGPIO_HEX,\n" +
                        "}brain_pin_e; // hello");
        EnumsReader enumsReader = new EnumsReader().read(reader);
        EnumsReader.EnumState brain_pin_e = enumsReader.getEnums().get("brain_pin_e");
        assertEquals(2, brain_pin_e.values.get("GPIO_HEX").getIntValue());

        VariableRegistry registry = new VariableRegistry();
        List<Value> listByOrdinal = EnumsReader.getSortedByOrder(registry, brain_pin_e.values);
        assertEquals(0, listByOrdinal.get(0).getIntValue());

        for (Map.Entry<String /*enum name*/, EnumsReader.EnumState> e : enumsReader.getEnums().entrySet()) {

            String a = ToJavaEnum.generate(registry, e.getKey(), e.getValue());

            assertEquals("package com.rusefi.enums;\n" +
                    "//auto-generated by ToJavaEnum.java\n" +
                    "\n" +
                    "\n" +
                    "\n" +
                    "public enum brain_pin_e {\n" +
                    "\tGPIO_UNASSIGNED,\n" +
                    "\tGPIO_INVALID,\n" +
                    "\tGPIO_HEX,\n" +
                    "}\n", a);
        }
    }

    @Test
    public void testTwoEnums() throws IOException {
        final StringReader reader = new StringReader(
                "typedef enum {\n" +
                        "  AUTOTUNER_OFF = 0,\n" +
                        "  STEADY_STATE_AT_BASELINE = 1,\n" +
                        "} PidAutoTune_AutoTunerState;\n" +
                        "\n" +
                        "typedef enum {\n" +
                        "\tINIT = 0,\n" +
                        "\tTPS_THRESHOLD = 1,\n" +
                        "\t/**\n" +
                        "\t * Live Docs reads 4 byte value so we want 4 byte enum\n" +
                        "\t */\n" +
                        "\tForce_4bytes_size_idle_state_e = ENUM_32_BITS,\n" +
                        "} idle_state_e;\n");

        EnumsReader enumsReader = new EnumsReader().read(reader);
        EnumToString enumToString = process(enumsReader);
        assertEquals("const char *getPidAutoTune_AutoTunerState(PidAutoTune_AutoTunerState value){\n" +
                "switch(value) {\n" +
                "case AUTOTUNER_OFF:\n" +
                "  return \"AUTOTUNER_OFF\";\n" +
                "case STEADY_STATE_AT_BASELINE:\n" +
                "  return \"STEADY_STATE_AT_BASELINE\";\n" +
                "  }\n" +
                " return NULL;\n" +
                "}\n" +
                "const char *getIdle_state_e(idle_state_e value){\n" +
                "switch(value) {\n" +
                "case Force_4bytes_size_idle_state_e:\n" +
                "  return \"Force_4bytes_size_idle_state_e\";\n" +
                "case INIT:\n" +
                "  return \"INIT\";\n" +
                "case TPS_THRESHOLD:\n" +
                "  return \"TPS_THRESHOLD\";\n" +
                "  }\n" +
                " return NULL;\n" +
                "}\n", enumToString.getCppFileContent());
    }

    @Test
    public void testWithInput() throws IOException {
        final StringReader reader = new StringReader(
                "typedef enum {\n" +
                        "\tGPIO_UNASSIGNED = XXXX,\n" +
                        "}brain_pin_e; // hello");

        VariableRegistry registry = new VariableRegistry();
        registry.readPrependValues(new StringReader("#define XXXX 0"));

        EnumsReader enumsReader = new EnumsReader().read(reader);
        for (Map.Entry<String /*enum name*/, EnumsReader.EnumState> e : enumsReader.getEnums().entrySet()) {
            String java = ToJavaEnum.generate(registry, e.getKey(), e.getValue());
            assertEquals("package com.rusefi.enums;\n" +
                    "//auto-generated by ToJavaEnum.java\n" +
                    "\n" +
                    "\n" +
                    "\n" +
                    "public enum brain_pin_e {\n" +
                    "\tGPIO_UNASSIGNED,\n" +
                    "}\n", java);
        }
    }

    @Test(expected = IllegalStateException.class)
    public void testWithInputOutOfOrder() throws IOException {
        final StringReader reader = new StringReader(
                "typedef enum {\n" +
                        "\tGPIO_UNASSIGNED = XXXX,\n" +
                        "}brain_pin_e; // hello");

        VariableRegistry registry = new VariableRegistry();
        registry.readPrependValues(new StringReader("#define XXXX 12"));

        EnumsReader enumsReader = new EnumsReader().read(reader);
        for (Map.Entry<String /*enum name*/, EnumsReader.EnumState> e : enumsReader.getEnums().entrySet()) {
            String java = ToJavaEnum.generate(registry, e.getKey(), e.getValue());
            assertEquals("package com.rusefi.enums;\n" +
                    "//auto-generated by ToJavaEnum.java\n" +
                    "\n" +
                    "\n" +
                    "\n" +
                    "public enum brain_pin_e {\n" +
                    "\tGPIO_UNASSIGNED,\n" +
                    "}\n", java);
        }
    }


    @Test
    public void parseCommentedOutEnumWithoutExplicitValues() throws IOException {
        final StringReader reader = new StringReader(
                "typedef enum {\n" +
                        "\t//GPIO_UNASSIGNED,\n" +
                        "\tGPIO_INVALID,\n" +
                        "\tGPIO_HEX,\n" +
                        "}brain_pin_e; // hello");
        EnumsReader enumsReader = new EnumsReader().read(reader);

        VariableRegistry registry = new VariableRegistry();

        for (Map.Entry<String /*enum name*/, EnumsReader.EnumState> e : enumsReader.getEnums().entrySet()) {

            String a = ToJavaEnum.generate(registry, e.getKey(), e.getValue());

            assertEquals("package com.rusefi.enums;\n" +
                    "//auto-generated by ToJavaEnum.java\n" +
                    "\n" +
                    "\n" +
                    "\n" +
                    "public enum brain_pin_e {\n" +
                    "\tGPIO_INVALID,\n" +
                    "\tGPIO_HEX,\n" +
                    "}\n", a);
        }
    }

}
