package com.rusefi.test;

import com.rusefi.*;
import com.rusefi.output.*;
import org.junit.Test;

import java.io.BufferedReader;
import java.io.CharArrayWriter;
import java.io.IOException;
import java.io.StringReader;
import java.util.Arrays;
import java.util.Collections;

import static org.junit.Assert.*;

/**
 * Andrey Belomutskiy, (c) 2013-2020
 * 1/15/15
 */
public class ConfigFieldParserTest {

    @Test
    public void testByteArray() {
        ReaderState state = new ReaderState();
        {
            ConfigField cf = ConfigField.parse(state, "uint8_t[8] field");
            assertEquals(cf.getType(), "uint8_t");
            assertEquals(cf.getArraySize(), 8);
            assertEquals(cf.getSize(null), 8);
            assertFalse("isIterate", cf.isIterate());
        }
    }

    @Test
    public void testFloatMsAlias() throws IOException {
        String test = "struct pid_s\n" +
                "floatms_t afr_type;PID dTime;\"ms\",      1,      0,       0, 3000,      0\n" +
                "percent_t afr_typet;PID dTime;\"ms\",      1,      0,       0, 3000,      0\n" +
                "end_struct\n";
        ReaderState state = new ReaderState();
        BufferedReader reader = new BufferedReader(new StringReader(test));

        CharArrayWriter writer = new CharArrayWriter();
        TestTSProjectConsumer javaFieldsConsumer = new TestTSProjectConsumer(writer, "", state);
        state.readBufferedReader(reader, Arrays.asList(javaFieldsConsumer));
        assertEquals("afr_type = scalar, F32, 0, \"ms\", 1, 0, 0, 3000, 0\n" +
                "afr_typet = scalar, F32, 4, \"ms\", 1, 0, 0, 3000, 0\n" +
                "; total TS size = 8\n", new String(writer.toCharArray()));
    }

    @Test(expected = IllegalStateException.class)
    public void testSameFieldTwice() throws IOException {
        String test = "struct pid_s\n" +
                "int afr_type1;PID dTime;\"ms\",      1,      0,       0, 3000,      0\n" +
                "int afr_type2;PID dTime;\"ms\",      1,      0,       0, 3000,      0\n" +
                "int afr_type1;PID dTime;\"ms\",      1,      0,       0, 3000,      0\n" +
                "end_struct\n";
        ReaderState state = new ReaderState();
        BufferedReader reader = new BufferedReader(new StringReader(test));

        BaseCHeaderConsumer consumer = new BaseCHeaderConsumer() {
            @Override
            public void endFile() {
            }
        };
        state.readBufferedReader(reader, Arrays.asList(consumer));
    }

    @Test
    public void testCustomEnum() throws IOException {
        String test = "struct pid_s\n" +
                "#define ego_sensor_e_enum \"BPSX\", \"Innovate\", \"14Point7\"\n" +
                "custom ego_sensor_e 4 bits, S32, @OFFSET@, [0:1], @@ego_sensor_e_enum@@\n" +
                "ego_sensor_e afr_type;\n" +
                "end_struct\n";
        ReaderState state = new ReaderState();
        BufferedReader reader = new BufferedReader(new StringReader(test));

        CharArrayWriter writer = new CharArrayWriter();
        TestTSProjectConsumer javaFieldsConsumer = new TestTSProjectConsumer(writer, "", state);
        state.readBufferedReader(reader, Arrays.asList(javaFieldsConsumer));
        assertEquals("afr_type = bits, S32, 0, [0:1], \"BPSX\", \"Innovate\", \"14Point7\", \"INVALID\"\n" +
                "; total TS size = 4\n", new String(writer.toCharArray()));
    }

    @Test
    public void alignFourByteTypes() throws IOException {
        // we expect padding before each 4 byte field
        String test = "struct pid_s\n" +
                "\tint16_t periodMs1;PID dTime;\"ms\",      1,      0,       0, 3000,      0\n" +
                "\tint periodSec;PID dTime;\"ms\",      1,      0,       0, 3000,      0\n" +
                "\tint16_t periodMs2;PID dTime;\"ms\",      1,      0,       0, 3000,      0\n" +
                "\tint periodSec2;PID dTime;\"ms\",      1,      0,       0, 3000,      0\n" +
                "end_struct\n";
        ReaderState state = new ReaderState();
        BufferedReader reader = new BufferedReader(new StringReader(test));

        JavaFieldsConsumer javaFieldsConsumer = new TestJavaFieldsConsumer(state);
        state.readBufferedReader(reader, Arrays.asList(javaFieldsConsumer));

        assertEquals(16, TypesHelper.getElementSize(state, "pid_s"));

        ConfigStructure structure = state.structures.get("pid_s");
        ConfigField firstField = structure.cFields.get(0);
        assertEquals("ms", firstField.getUnits());
    }

    @Test
    public void manyStartAreNotMultiplication() throws IOException {
        String test = "struct pid_s\n" +
                "#define ERROR_BUFFER_SIZE \"***\"\n" +
                "end_struct\n" +
                "";
        BufferedReader reader = new BufferedReader(new StringReader(test));
        new ReaderState().readBufferedReader(reader, Collections.emptyList());
    }

    @Test(expected = IllegalStateException.class)
    public void invalidDefine() throws IOException {
        String test = "struct pid_s\n" +
                VariableRegistry.DEFINE + " show show_Hellen121vag_presets true\n" +
                "end_struct\n" +
                "";
        BufferedReader reader = new BufferedReader(new StringReader(test));
        new ReaderState().readBufferedReader(reader, Collections.emptyList());
    }

    @Test
    public void multiplicationInDefine() throws IOException {
        String test = "struct pid_s\n" +
                "#define ERROR_BUFFER_SIZE 120\n" +
                "#define ERROR_BUFFER_COUNT 120\n" +
                "#define RESULT @@ERROR_BUFFER_SIZE@@*@@ERROR_BUFFER_COUNT@@\n" +
                "\tint16_t periodMs;PID dTime;\"ms\",      1,      0,       0, 3000,      0\n" +
                "end_struct\n" +
                "";

        BufferedReader reader = new BufferedReader(new StringReader(test));
        ReaderState state = new ReaderState();
        state.readBufferedReader(reader, Collections.emptyList());

        assertEquals("#define ERROR_BUFFER_COUNT 120\n" +
                "#define ERROR_BUFFER_SIZE 120\n" +
                "#define RESULT 14400\n", state.variableRegistry.getDefinesSection());
    }
    @Test
    public void expressionInMultiplier() throws IOException {
        String test = "struct pid_s\n" +
                "\tint16_t periodMs;PID dTime;\"ms\",      {1/10},      0,       0, 3000,      0\n" +
                "\tint16_t periodMs2;PID dTime;\"ms\",      1,      0,       0, 3000,      0\n" +
                "custom afr_table_t 4x4 array,   U08,   @OFFSET@, [4x4],\"deg\",\t   {1/10},     0,        0,  25.0,     1 \n" +
                "afr_table_t afrTable;\t\t\n" +
                "end_struct\n" +
                "";

        BufferedReader reader = new BufferedReader(new StringReader(test));

        CharArrayWriter writer = new CharArrayWriter();
        ReaderState state = new ReaderState();
        TSProjectConsumer javaFieldsConsumer = new TestTSProjectConsumer(writer, "", state);

        state.readBufferedReader(reader, Collections.singletonList(javaFieldsConsumer));

        assertEquals("periodMs = scalar, S16, 0, \"ms\", 0.1, 0, 0, 3000, 0\n" +
                "periodMs2 = scalar, S16, 2, \"ms\", 1, 0, 0, 3000, 0\n" +
                "afrTable = array, U08, 4, [4x4],\"deg\", 0.1, 0, 0, 25.0, 1\n" +
                "; total TS size = 20\n", new String(writer.toCharArray()));
    }

    @Test
    public void useCustomType() throws IOException {
        ReaderState state = new ReaderState();
        String test = "struct pid_s\n" +
                "#define ERROR_BUFFER_SIZE 120\n" +
                "\tcustom critical_error_message_t @@ERROR_BUFFER_SIZE@@ string, ASCII, @OFFSET@, @@ERROR_BUFFER_SIZE@@\n" +
                "\tcritical_error_message_t var;\n" +
                "\tint16_t periodMs;PID dTime;\"ms\",      1,      0,       0, 3000,      0\n" +
                "end_struct\n" +
                "";
        BufferedReader reader = new BufferedReader(new StringReader(test));

        JavaFieldsConsumer javaFieldsConsumer = new TestJavaFieldsConsumer(state);
        state.readBufferedReader(reader, Arrays.asList(javaFieldsConsumer));

        assertEquals("\tpublic static final Field VAR = Field.create(\"VAR\", 0, 120, FieldType.STRING);\n" +
                        "\tpublic static final Field PERIODMS = Field.create(\"PERIODMS\", 120, FieldType.INT16);\n",
                javaFieldsConsumer.getJavaFieldsWriter());
    }

    @Test
    public void testDefineChar() throws IOException {
        ReaderState state = new ReaderState();
        String test =
                "#define SD_r 'r'\n" +
                        "";
        BufferedReader reader = new BufferedReader(new StringReader(test));

        JavaFieldsConsumer javaFieldsConsumer = new TestJavaFieldsConsumer(state);
        state.readBufferedReader(reader, Arrays.asList(javaFieldsConsumer));

        assertEquals("\tpublic static final char SD_r = 'r';\n" +
                        "",
                state.variableRegistry.getJavaConstants());
    }

    @Test
    public void testDefine() throws IOException {
        ReaderState state = new ReaderState();
        String test =
                "#define ERROR_BUFFER_SIZE 120\n" +
                        "#define ERROR_BUFFER_SIZE_H 0x120\n" +
                "";
        BufferedReader reader = new BufferedReader(new StringReader(test));

        JavaFieldsConsumer javaFieldsConsumer = new TestJavaFieldsConsumer(state);
        state.readBufferedReader(reader, Arrays.asList(javaFieldsConsumer));

        assertEquals("\tpublic static final int ERROR_BUFFER_SIZE = 120;\n" +
                        "\tpublic static final int ERROR_BUFFER_SIZE_H = 0x120;\n" +
                        "",
                state.variableRegistry.getJavaConstants());
    }

    @Test
    public void testFsioVisible() throws IOException {
        {
            ReaderState state = new ReaderState();
            ConfigField cf = ConfigField.parse(state, "int fsio_visible field");
            assertEquals(cf.getType(), "int");
            assertTrue(cf.isFsioVisible());
            assertEquals("Name", cf.getName(), "field");
        }

        {
            ReaderState state = new ReaderState();
            String test = "struct pid_s\n" +
                    "\tint16_t fsio_visible offset;Linear addition to PID logic;\"\",      1,      0,       -1000, 1000,      0\n" +
                    "\tint16_t periodMs;PID dTime;\"ms\",      1,      0,       0, 3000,      0\n" +
                    "\tint16_t fsio_visible minValue;Output min value;\"\",        1,     0,  -30000,    30000.0,  0\n" +
                    "end_struct\n" +
                    "struct_no_prefix engine_configuration_s\n" +
                    "\tpid_s alternatorControl;\n" +
                    "#define ETB_COUNT 2\n" +
                    "\tpid_s[ETB_COUNT iterate] etb;\n" +
                    "end_struct\n" +
                    "" +
                    "";
            BufferedReader reader = new BufferedReader(new StringReader(test));

            JavaFieldsConsumer javaFieldsConsumer = new TestJavaFieldsConsumer(state);

            FsioSettingsConsumer fsioSettingsConsumer = new FsioSettingsConsumer(state) {
                @Override
                public void startFile() {
                }

                @Override
                public void endFile() {
                }
            };

            state.readBufferedReader(reader, Arrays.asList(javaFieldsConsumer, fsioSettingsConsumer));


            assertEquals("\tpublic static final Field OFFSET = Field.create(\"OFFSET\", 0, FieldType.INT16);\n" +
                            "\tpublic static final Field PERIODMS = Field.create(\"PERIODMS\", 2, FieldType.INT16);\n" +
                            "\tpublic static final Field MINVALUE = Field.create(\"MINVALUE\", 4, FieldType.INT16);\n" +
                            "\tpublic static final Field ALTERNATORCONTROL_OFFSET = Field.create(\"ALTERNATORCONTROL_OFFSET\", 0, FieldType.INT16);\n" +
                            "\tpublic static final Field ALTERNATORCONTROL_PERIODMS = Field.create(\"ALTERNATORCONTROL_PERIODMS\", 2, FieldType.INT16);\n" +
                            "\tpublic static final Field ALTERNATORCONTROL_MINVALUE = Field.create(\"ALTERNATORCONTROL_MINVALUE\", 4, FieldType.INT16);\n" +
                            "\tpublic static final Field ETB1_OFFSET = Field.create(\"ETB1_OFFSET\", 8, FieldType.INT16);\n" +
                            "\tpublic static final Field ETB1_PERIODMS = Field.create(\"ETB1_PERIODMS\", 10, FieldType.INT16);\n" +
                            "\tpublic static final Field ETB1_MINVALUE = Field.create(\"ETB1_MINVALUE\", 12, FieldType.INT16);\n" +
                            "\tpublic static final Field ETB2_OFFSET = Field.create(\"ETB2_OFFSET\", 16, FieldType.INT16);\n" +
                            "\tpublic static final Field ETB2_PERIODMS = Field.create(\"ETB2_PERIODMS\", 18, FieldType.INT16);\n" +
                            "\tpublic static final Field ETB2_MINVALUE = Field.create(\"ETB2_MINVALUE\", 20, FieldType.INT16);\n",
                    javaFieldsConsumer.getJavaFieldsWriter());

            assertEquals("\tcase FSIO_SETTING_OFFSET:\n" +
                            "\t\treturn engineConfiguration->offset;\n" +
                            "\tcase FSIO_SETTING_MINVALUE:\n" +
                            "\t\treturn engineConfiguration->minValue;\n" +
                            "\tcase FSIO_SETTING_ALTERNATORCONTROL_OFFSET:\n" +
                            "\t\treturn engineConfiguration->alternatorControl.offset;\n" +
                            "\tcase FSIO_SETTING_ALTERNATORCONTROL_MINVALUE:\n" +
                            "\t\treturn engineConfiguration->alternatorControl.minValue;\n" +
                            "\tcase FSIO_SETTING_ETB1_OFFSET:\n" +
                            "\t\treturn engineConfiguration->etb[0].offset;\n" +
                            "\tcase FSIO_SETTING_ETB1_MINVALUE:\n" +
                            "\t\treturn engineConfiguration->etb[0].minValue;\n" +
                            "\tcase FSIO_SETTING_ETB2_OFFSET:\n" +
                            "\t\treturn engineConfiguration->etb[1].offset;\n" +
                            "\tcase FSIO_SETTING_ETB2_MINVALUE:\n" +
                            "\t\treturn engineConfiguration->etb[1].minValue;\n",
                    fsioSettingsConsumer.getContent());

            assertEquals("\tFSIO_SETTING_OFFSET = 1000,\n" +
                            "\tFSIO_SETTING_MINVALUE = 1001,\n" +
                            "\tFSIO_SETTING_ALTERNATORCONTROL_OFFSET = 1002,\n" +
                            "\tFSIO_SETTING_ALTERNATORCONTROL_MINVALUE = 1003,\n" +
                            "\tFSIO_SETTING_ETB1_OFFSET = 1004,\n" +
                            "\tFSIO_SETTING_ETB1_MINVALUE = 1005,\n" +
                            "\tFSIO_SETTING_ETB2_OFFSET = 1006,\n" +
                            "\tFSIO_SETTING_ETB2_MINVALUE = 1007,\n",
                    fsioSettingsConsumer.getEnumDefinition());

            assertEquals("static LENameOrdinalPair leoffset(FSIO_SETTING_OFFSET, \"cfg_offset\");\n" +
                    "static LENameOrdinalPair leminValue(FSIO_SETTING_MINVALUE, \"cfg_minValue\");\n" +
                    "static LENameOrdinalPair lealternatorControl_offset(FSIO_SETTING_ALTERNATORCONTROL_OFFSET, \"cfg_alternatorControl_offset\");\n" +
                    "static LENameOrdinalPair lealternatorControl_minValue(FSIO_SETTING_ALTERNATORCONTROL_MINVALUE, \"cfg_alternatorControl_minValue\");\n" +
                    "static LENameOrdinalPair leetb1_offset(FSIO_SETTING_ETB1_OFFSET, \"cfg_etb1_offset\");\n" +
                    "static LENameOrdinalPair leetb1_minValue(FSIO_SETTING_ETB1_MINVALUE, \"cfg_etb1_minValue\");\n" +
                    "static LENameOrdinalPair leetb2_offset(FSIO_SETTING_ETB2_OFFSET, \"cfg_etb2_offset\");\n" +
                    "static LENameOrdinalPair leetb2_minValue(FSIO_SETTING_ETB2_MINVALUE, \"cfg_etb2_minValue\");\n", fsioSettingsConsumer.getNames());

            assertEquals("\tcase FSIO_SETTING_OFFSET:\n" +
                    "\t\treturn \"cfg_offset\";\n" +
                    "\tcase FSIO_SETTING_MINVALUE:\n" +
                    "\t\treturn \"cfg_minValue\";\n" +
                    "\tcase FSIO_SETTING_ALTERNATORCONTROL_OFFSET:\n" +
                    "\t\treturn \"cfg_alternatorControl_offset\";\n" +
                    "\tcase FSIO_SETTING_ALTERNATORCONTROL_MINVALUE:\n" +
                    "\t\treturn \"cfg_alternatorControl_minValue\";\n" +
                    "\tcase FSIO_SETTING_ETB1_OFFSET:\n" +
                    "\t\treturn \"cfg_etb1_offset\";\n" +
                    "\tcase FSIO_SETTING_ETB1_MINVALUE:\n" +
                    "\t\treturn \"cfg_etb1_minValue\";\n" +
                    "\tcase FSIO_SETTING_ETB2_OFFSET:\n" +
                    "\t\treturn \"cfg_etb2_offset\";\n" +
                    "\tcase FSIO_SETTING_ETB2_MINVALUE:\n" +
                    "\t\treturn \"cfg_etb2_minValue\";\n", fsioSettingsConsumer.getStrings());
        }
    }

    @Test
    public void testArrayOfOne() throws IOException {
        String test = "struct pid_s\n" +
                "#define ERROR_BUFFER_SIZE 1\n" +
                "int[ERROR_BUFFER_SIZE iterate] field\n" +
                "end_struct\n" +
                "";
        BufferedReader reader = new BufferedReader(new StringReader(test));
        BaseCHeaderConsumer consumer = new BaseCHeaderConsumer() {
            @Override
            public void endFile() {
            }
        };
        new ReaderState().readBufferedReader(reader, Collections.singletonList(consumer));
        assertEquals("// start of pid_s\n" +
                "struct pid_s {\n" +
                "\t/**\n" +
                "\t * offset 0\n" +
                "\t */\n" +
                "\tint field[ERROR_BUFFER_SIZE];\n" +
                "\t/** total size 4*/\n" +
                "};\n" +
                "\n", consumer.getContent().toString());
    }

    @Test
    public void testParseLine() {
        ReaderState state = new ReaderState();
        assertNull(ConfigField.parse(state, "int"));
        {
            ConfigField cf = ConfigField.parse(state, "int field");
            assertEquals(cf.getType(), "int");
            assertEquals("Name", cf.getName(), "field");
        }
        {
            ConfigField cf = ConfigField.parse(state, "int_4 fie4_ld");
            assertEquals(cf.getType(), "int_4");
            assertEquals(cf.getName(), "fie4_ld");
        }
        {
            ConfigField cf = ConfigField.parse(state, "int_8 fi_eld;comm_;ts");
            assertEquals(cf.getType(), "int_8");
            assertEquals(cf.getName(), "fi_eld");
            assertEquals("Comment", cf.getComment(), "comm_");
            assertEquals(cf.getTsInfo(), "ts");
        }
        {
            ConfigField cf = ConfigField.parse(state, "int[3 iterate] field");
            assertEquals(cf.getType(), "int");
            assertEquals(cf.getArraySize(), 3);
            assertTrue("isIterate", cf.isIterate());
        }
        {
            ConfigField cf = ConfigField.parse(state, "int16_t crankingRpm;This,. value controls what RPM values we consider 'cranking' (any RPM below 'crankingRpm')\\nAnything above 'crankingRpm' would be 'running'");
            assertEquals(cf.getName(), "crankingRpm");
            assertEquals(cf.getArraySize(), 1);
            assertEquals(cf.getType(), "int16_t");
        }
        {
            ConfigField cf = ConfigField.parse(state, "MAP_sensor_config_s map");
            assertEquals(cf.getName(), "map");
            assertEquals(cf.getArraySize(), 1);
            assertEquals(cf.getType(), "MAP_sensor_config_s");
        }
        {
            ConfigField cf = ConfigField.parse(state, "MAP_sensor_config_s map;@see hasMapSensor\\n@see isMapAveragingEnabled");
            assertEquals(cf.getName(), "map");
            assertEquals(cf.getArraySize(), 1);
            assertEquals(cf.getType(), "MAP_sensor_config_s");
            assertEquals(cf.getComment(), "@see hasMapSensor\\n@see isMapAveragingEnabled");
        }
    }

    @Test
    public void testParseSize() {
        ReaderState state = new ReaderState();
        assertEquals(4, state.parseSize("4", ""));

        assertEquals(12, state.parseSize("4*3", ""));

        state.variableRegistry.register("var", 256);

        assertEquals(512, state.parseSize("2*@@var@@", ""));
        assertEquals(512, state.parseSize("2x@@var@@", ""));
    }
}
