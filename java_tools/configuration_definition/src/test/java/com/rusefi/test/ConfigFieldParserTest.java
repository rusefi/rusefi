package com.rusefi.test;

import com.rusefi.ConfigField;
import com.rusefi.ReaderState;
import com.rusefi.TypesHelper;
import com.rusefi.VariableRegistry;
import com.rusefi.output.BaseCHeaderConsumer;
import com.rusefi.output.ConfigStructure;
import com.rusefi.output.JavaFieldsConsumer;
import com.rusefi.output.TSProjectConsumer;
import org.junit.Test;

import java.io.IOException;

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
            assertEquals(cf.getArraySizes().length, 1);
            assertEquals(cf.getArraySizes()[0], 8);
            assertEquals(cf.getSize(null), 8);
            assertFalse("isIterate", cf.isIterate());
        }
    }

    @Test
    public void testByte3dArray() {
        ReaderState state = new ReaderState();
        {
            ConfigField cf = ConfigField.parse(state, "uint8_t[8 x 16] field");
            assertEquals(cf.getType(), "uint8_t");
            assertEquals(cf.getArraySizes().length, 2);
            assertEquals(cf.getArraySizes()[0], 8);
            assertEquals(cf.getArraySizes()[1], 16);
            assertEquals(cf.getSize(null), 128);
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

        TestTSProjectConsumer tsProjectConsumer = new TestTSProjectConsumer("", state);
        state.readBufferedReader(test, tsProjectConsumer);
        assertEquals("afr_type = scalar, F32, 0, \"ms\", 1, 0, 0, 3000, 0\n" +
                "afr_typet = scalar, F32, 4, \"ms\", 1, 0, 0, 3000, 0\n" +
                "; total TS size = 8\n", tsProjectConsumer.getContent());
    }

    @Test(expected = IllegalStateException.class)
    public void testSameFieldTwice() throws IOException {
        String test = "struct pid_s\n" +
                "int afr_type1;PID dTime;\"ms\",      1,      0,       0, 3000,      0\n" +
                "int afr_type2;PID dTime;\"ms\",      1,      0,       0, 3000,      0\n" +
                "int afr_type1;PID dTime;\"ms\",      1,      0,       0, 3000,      0\n" +
                "end_struct\n";
        ReaderState state = new ReaderState();

        BaseCHeaderConsumer consumer = new BaseCHeaderConsumer();
        state.readBufferedReader(test, consumer);
    }

    @Test
    public void testCustomEnum() throws IOException {
        String test = "struct pid_s\n" +
                "#define ego_sensor_e_enum \"BPSX\", \"Innovate\", \"14Point7\"\n" +
                "custom ego_sensor_e 1 bits, S08, @OFFSET@, [0:1], @@ego_sensor_e_enum@@\n" +
                "ego_sensor_e afr_type1;\n" +
                "ego_sensor_e afr_type2;\n" +
                "int8_t int\n" +
                "end_struct\n";
        ReaderState state = new ReaderState();

        TestTSProjectConsumer tsProjectConsumer = new TestTSProjectConsumer("", state);
        state.readBufferedReader(test, tsProjectConsumer);
        assertEquals("afr_type1 = bits, S08, 0, [0:1], \"BPSX\", \"Innovate\", \"14Point7\"\n" +
                "afr_type2 = bits, S08, 1, [0:1], \"BPSX\", \"Innovate\", \"14Point7\"\n" +
                "int = scalar, S08, 2, \"\", 1, 0, 0, 100, 0\n" +
                "; total TS size = 4\n", tsProjectConsumer.getContent());
    }

    @Test
    public void testShortForm() throws IOException {
        String test = "struct pid_s\n" +
                "#define ego_sensor_e_enum \"BPSX\"\n" +
                "custom ego_sensor_e 1 bits, S08, @OFFSET@, [0:1], @@ego_sensor_e_enum@@\n" +
                "ego_sensor_e afr_type1;\n" +
                "ego_sensor_e afr_type2;\n" +
                "int8_t int\n" +
                "end_struct\n";
        ReaderState state = new ReaderState();

        TestTSProjectConsumer tsProjectConsumer = new TestTSProjectConsumer("", state);
        state.readBufferedReader(test, tsProjectConsumer);
        assertEquals("afr_type1 = bits, S08, 0, [0:1], \"BPSX\"\n" +
                "afr_type2 = bits, S08, 1, [0:1], \"BPSX\"\n" +
                "int = scalar, S08, 2, \"\", 1, 0, 0, 100, 0\n" +
                "; total TS size = 4\n", tsProjectConsumer.getContent());
    }

    @Test
    public void test2byteCustomEnum() throws IOException {
        String test = "struct pid_s\n" +
                "#define ego_sensor_e_enum \"BPSX\", \"Innovate\", \"14Point7\"\n" +
                "custom ego_sensor_e2 2 bits, S16, @OFFSET@, [0:1], @@ego_sensor_e_enum@@\n" +
                "int8_t int\n" +
                "ego_sensor_e2 afr_type1;\n" +
                "ego_sensor_e2 afr_type2;\n" +
                "end_struct\n";
        ReaderState state = new ReaderState();

        TestTSProjectConsumer tsProjectConsumer = new TestTSProjectConsumer("", state);
        state.readBufferedReader(test, tsProjectConsumer);
        assertEquals("int = scalar, S08, 0, \"\", 1, 0, 0, 100, 0\n" +
                "afr_type1 = bits, S16, 2, [0:1], \"BPSX\", \"Innovate\", \"14Point7\"\n" +
                "afr_type2 = bits, S16, 4, [0:1], \"BPSX\", \"Innovate\", \"14Point7\"\n" +
                "; total TS size = 8\n", tsProjectConsumer.getContent());
    }

    @Test
    public void test4byteCustomEnum() throws IOException {
        String test = "struct pid_s\n" +
                "#define ego_sensor_e_enum \"BPSX\", \"Innovate\", \"14Point7\"\n" +
                "custom ego_sensor_e4 4 bits, S32, @OFFSET@, [0:1], @@ego_sensor_e_enum@@\n" +
                "int8_t int2\n" +
                "ego_sensor_e4 afr_type3;\n" +
                "end_struct\n";
        ReaderState state = new ReaderState();

        TestTSProjectConsumer tsProjectConsumer = new TestTSProjectConsumer("", state);
        state.readBufferedReader(test, tsProjectConsumer);
        assertEquals("int2 = scalar, S08, 0, \"\", 1, 0, 0, 100, 0\n" +
                "afr_type3 = bits, S32, 4, [0:1], \"BPSX\", \"Innovate\", \"14Point7\"\n" +
                "; total TS size = 8\n", tsProjectConsumer.getContent());
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

        JavaFieldsConsumer javaFieldsConsumer = new TestJavaFieldsConsumer(state);
        state.readBufferedReader(test, (javaFieldsConsumer));

        assertEquals(16, TypesHelper.getElementSize(state, "pid_s"));

        ConfigStructure structure = state.structures.get("pid_s");
        ConfigField firstField = structure.cFields.get(0);
        assertEquals("ms", firstField.getUnits());
    }

    @Test
    public void alignArray6() throws IOException {
        // we expect padding before each 4 byte field
        String test = "struct vr_threshold_s\n" +
                "\tuint8_t pin;\n" +
                "\tuint8_t[3] pad;;\"\",1,0,0,0,0\n" +
                "\tuint8_t[6] autoscale rpmBins;;\"rpm\", 1, 0, 0, 12000, 0\n" +
                "\tuint8_t[6] autoscale values;;\"volts\", 1, 0, 0, 2.5, 2\n" +
                "end_struct\n\n";
        ReaderState state = new ReaderState();

        JavaFieldsConsumer javaFieldsConsumer = new TestJavaFieldsConsumer(state);
        state.readBufferedReader(test, (javaFieldsConsumer));

        assertEquals("\tpublic static final Field PIN = Field.create(\"PIN\", 0, FieldType.INT8).setScale(1.0);\n" +
                "\tpublic static final Field PAD = Field.create(\"PAD\", 1, FieldType.INT8).setScale(1.0);\n" +
                "\tpublic static final Field RPMBINS = Field.create(\"RPMBINS\", 4, FieldType.INT8).setScale(1.0);\n" +
                "\tpublic static final Field VALUES = Field.create(\"VALUES\", 10, FieldType.INT8).setScale(1.0);\n", javaFieldsConsumer.getContent());
    }

    @Test
    public void manyStartAreNotMultiplication() throws IOException {
        String test = "struct pid_s\n" +
                "#define ERROR_BUFFER_SIZE \"***\"\n" +
                "end_struct\n" +
                "";
        new ReaderState().readBufferedReader(test);
    }

    @Test(expected = IllegalStateException.class)
    public void invalidDefine() throws IOException {
        String test = "struct pid_s\n" +
                VariableRegistry.DEFINE + " show show_Hellen121vag_presets true\n" +
                "end_struct\n" +
                "";
        new ReaderState().readBufferedReader(test);
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

        ReaderState state = new ReaderState();
        state.readBufferedReader(test);

        assertEquals("#define ERROR_BUFFER_COUNT 120\n" +
                "#define ERROR_BUFFER_SIZE 120\n" +
                "#define RESULT 14400\n", state.variableRegistry.getDefinesSection());
    }
    @Test
    public void expressionInMultiplier() throws IOException {
        String test = "struct pid_s\n" +
                "\tint16_t autoscale periodMs;PID dTime;\"ms\",      {1/10},      0,       0, 3000,      0\n" +
                "\tint16_t periodMs2;PID dTime;\"ms\",      1,      0,       0, 3000,      0\n" +
                "custom afr_table_t 4x4 array,   U08,   @OFFSET@, [4x4],\"deg\",\t   {1/10},     0,        0,  25.0,     1 \n" +
                "afr_table_t afrTable;\t\t\n" +
                "end_struct\n" +
                "";

        ReaderState state = new ReaderState();
        TSProjectConsumer tsProjectConsumer = new TestTSProjectConsumer("", state);

        state.readBufferedReader(test, tsProjectConsumer);

        assertEquals("periodMs = scalar, S16, 0, \"ms\", 0.1, 0, 0, 3000, 0\n" +
                "periodMs2 = scalar, S16, 2, \"ms\", 1, 0, 0, 3000, 0\n" +
                "afrTable = array, U08, 4, [4x4],\"deg\", 0.1, 0, 0, 25.0, 1\n" +
                "; total TS size = 20\n", tsProjectConsumer.getContent());
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

        JavaFieldsConsumer javaFieldsConsumer = new TestJavaFieldsConsumer(state);
        state.readBufferedReader(test, javaFieldsConsumer);

        assertEquals("\tpublic static final Field VAR = Field.create(\"VAR\", 0, 120, FieldType.STRING).setScale(1.0);\n" +
                        "\tpublic static final Field PERIODMS = Field.create(\"PERIODMS\", 120, FieldType.INT16).setScale(1.0);\n" +
                        "\tpublic static final Field ALIGNMENTFILL_AT_122 = Field.create(\"ALIGNMENTFILL_AT_122\", 122, FieldType.INT8).setScale(1.0);\n",
                javaFieldsConsumer.getContent());
    }

    @Test
    public void testDefineChar() throws IOException {
        ReaderState state = new ReaderState();
        String test =
                "#define SD_r 'r'\n" +
                        "";

        JavaFieldsConsumer javaFieldsConsumer = new TestJavaFieldsConsumer(state);
        state.readBufferedReader(test, javaFieldsConsumer);

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

        JavaFieldsConsumer javaFieldsConsumer = new TestJavaFieldsConsumer(state);
        state.readBufferedReader(test, (javaFieldsConsumer));

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

            JavaFieldsConsumer javaFieldsConsumer = new TestJavaFieldsConsumer(state);

            state.readBufferedReader(test, (javaFieldsConsumer));


            assertEquals("\tpublic static final Field OFFSET = Field.create(\"OFFSET\", 0, FieldType.INT16).setScale(1.0);\n" +
                            "\tpublic static final Field PERIODMS = Field.create(\"PERIODMS\", 2, FieldType.INT16).setScale(1.0);\n" +
                            "\tpublic static final Field MINVALUE = Field.create(\"MINVALUE\", 4, FieldType.INT16).setScale(1.0);\n" +
                            "\tpublic static final Field ALIGNMENTFILL_AT_6 = Field.create(\"ALIGNMENTFILL_AT_6\", 6, FieldType.INT8).setScale(1.0);\n" +
                            "\tpublic static final Field ALTERNATORCONTROL_OFFSET = Field.create(\"ALTERNATORCONTROL_OFFSET\", 0, FieldType.INT16).setScale(1.0);\n" +
                            "\tpublic static final Field ALTERNATORCONTROL_PERIODMS = Field.create(\"ALTERNATORCONTROL_PERIODMS\", 2, FieldType.INT16).setScale(1.0);\n" +
                            "\tpublic static final Field ALTERNATORCONTROL_MINVALUE = Field.create(\"ALTERNATORCONTROL_MINVALUE\", 4, FieldType.INT16).setScale(1.0);\n" +
                            "\tpublic static final Field ALTERNATORCONTROL_ALIGNMENTFILL_AT_6 = Field.create(\"ALTERNATORCONTROL_ALIGNMENTFILL_AT_6\", 6, FieldType.INT8).setScale(1.0);\n" +
                            "\tpublic static final Field ETB1_OFFSET = Field.create(\"ETB1_OFFSET\", 8, FieldType.INT16).setScale(1.0);\n" +
                            "\tpublic static final Field ETB1_PERIODMS = Field.create(\"ETB1_PERIODMS\", 10, FieldType.INT16).setScale(1.0);\n" +
                            "\tpublic static final Field ETB1_MINVALUE = Field.create(\"ETB1_MINVALUE\", 12, FieldType.INT16).setScale(1.0);\n" +
                            "\tpublic static final Field ETB1_ALIGNMENTFILL_AT_6 = Field.create(\"ETB1_ALIGNMENTFILL_AT_6\", 14, FieldType.INT8).setScale(1.0);\n" +
                            "\tpublic static final Field ETB2_OFFSET = Field.create(\"ETB2_OFFSET\", 16, FieldType.INT16).setScale(1.0);\n" +
                            "\tpublic static final Field ETB2_PERIODMS = Field.create(\"ETB2_PERIODMS\", 18, FieldType.INT16).setScale(1.0);\n" +
                            "\tpublic static final Field ETB2_MINVALUE = Field.create(\"ETB2_MINVALUE\", 20, FieldType.INT16).setScale(1.0);\n" +
                            "\tpublic static final Field ETB2_ALIGNMENTFILL_AT_6 = Field.create(\"ETB2_ALIGNMENTFILL_AT_6\", 22, FieldType.INT8).setScale(1.0);\n",
                    javaFieldsConsumer.getContent());
        }
    }

    @Test
    public void test2byteOffset() throws IOException {
        String test = "struct_no_prefix pid_s\n" +
                "\tint8_t byte1\n" +
                "\tint16_t short\n" +
                "\tint int2\n" +
                "\tint8_t byte2\n" +
                "\tbit periodMs\n" +
                "end_struct\n" +

                "";
        BaseCHeaderConsumer consumer = new BaseCHeaderConsumer();
        ReaderState state = new ReaderState();
        JavaFieldsConsumer javaFieldsConsumer = new TestJavaFieldsConsumer(state);
        state.readBufferedReader(test, consumer, javaFieldsConsumer);
        assertEquals("\tpublic static final Field BYTE1 = Field.create(\"BYTE1\", 0, FieldType.INT8).setScale(1.0);\n" +
                        "\tpublic static final Field ALIGNMENTFILL_AT_1 = Field.create(\"ALIGNMENTFILL_AT_1\", 1, FieldType.INT8).setScale(1.0);\n" +
                        "\tpublic static final Field SHORT = Field.create(\"SHORT\", 2, FieldType.INT16).setScale(1.0);\n" +
                        "\tpublic static final Field INT2 = Field.create(\"INT2\", 4, FieldType.INT).setScale(1.0);\n" +
                        "\tpublic static final Field BYTE2 = Field.create(\"BYTE2\", 8, FieldType.INT8).setScale(1.0);\n" +
                        "\tpublic static final Field ALIGNMENTFILL_AT_9 = Field.create(\"ALIGNMENTFILL_AT_9\", 9, FieldType.INT8).setScale(1.0);\n" +
                        "\tpublic static final Field PERIODMS = Field.create(\"PERIODMS\", 12, FieldType.BIT, 0);\n" +
                        "\tpublic static final Field UNUSEDBIT_7_1 = Field.create(\"UNUSEDBIT_7_1\", 12, FieldType.BIT, 1);\n" +
                        "\tpublic static final Field UNUSEDBIT_7_2 = Field.create(\"UNUSEDBIT_7_2\", 12, FieldType.BIT, 2);\n" +
                        "\tpublic static final Field UNUSEDBIT_7_3 = Field.create(\"UNUSEDBIT_7_3\", 12, FieldType.BIT, 3);\n" +
                        "\tpublic static final Field UNUSEDBIT_7_4 = Field.create(\"UNUSEDBIT_7_4\", 12, FieldType.BIT, 4);\n" +
                        "\tpublic static final Field UNUSEDBIT_7_5 = Field.create(\"UNUSEDBIT_7_5\", 12, FieldType.BIT, 5);\n" +
                        "\tpublic static final Field UNUSEDBIT_7_6 = Field.create(\"UNUSEDBIT_7_6\", 12, FieldType.BIT, 6);\n" +
                        "\tpublic static final Field UNUSEDBIT_7_7 = Field.create(\"UNUSEDBIT_7_7\", 12, FieldType.BIT, 7);\n" +
                        "\tpublic static final Field UNUSEDBIT_7_8 = Field.create(\"UNUSEDBIT_7_8\", 12, FieldType.BIT, 8);\n" +
                        "\tpublic static final Field UNUSEDBIT_7_9 = Field.create(\"UNUSEDBIT_7_9\", 12, FieldType.BIT, 9);\n" +
                        "\tpublic static final Field UNUSEDBIT_7_10 = Field.create(\"UNUSEDBIT_7_10\", 12, FieldType.BIT, 10);\n" +
                        "\tpublic static final Field UNUSEDBIT_7_11 = Field.create(\"UNUSEDBIT_7_11\", 12, FieldType.BIT, 11);\n" +
                        "\tpublic static final Field UNUSEDBIT_7_12 = Field.create(\"UNUSEDBIT_7_12\", 12, FieldType.BIT, 12);\n" +
                        "\tpublic static final Field UNUSEDBIT_7_13 = Field.create(\"UNUSEDBIT_7_13\", 12, FieldType.BIT, 13);\n" +
                        "\tpublic static final Field UNUSEDBIT_7_14 = Field.create(\"UNUSEDBIT_7_14\", 12, FieldType.BIT, 14);\n" +
                        "\tpublic static final Field UNUSEDBIT_7_15 = Field.create(\"UNUSEDBIT_7_15\", 12, FieldType.BIT, 15);\n" +
                        "\tpublic static final Field UNUSEDBIT_7_16 = Field.create(\"UNUSEDBIT_7_16\", 12, FieldType.BIT, 16);\n" +
                        "\tpublic static final Field UNUSEDBIT_7_17 = Field.create(\"UNUSEDBIT_7_17\", 12, FieldType.BIT, 17);\n" +
                        "\tpublic static final Field UNUSEDBIT_7_18 = Field.create(\"UNUSEDBIT_7_18\", 12, FieldType.BIT, 18);\n" +
                        "\tpublic static final Field UNUSEDBIT_7_19 = Field.create(\"UNUSEDBIT_7_19\", 12, FieldType.BIT, 19);\n" +
                        "\tpublic static final Field UNUSEDBIT_7_20 = Field.create(\"UNUSEDBIT_7_20\", 12, FieldType.BIT, 20);\n" +
                        "\tpublic static final Field UNUSEDBIT_7_21 = Field.create(\"UNUSEDBIT_7_21\", 12, FieldType.BIT, 21);\n" +
                        "\tpublic static final Field UNUSEDBIT_7_22 = Field.create(\"UNUSEDBIT_7_22\", 12, FieldType.BIT, 22);\n" +
                        "\tpublic static final Field UNUSEDBIT_7_23 = Field.create(\"UNUSEDBIT_7_23\", 12, FieldType.BIT, 23);\n" +
                        "\tpublic static final Field UNUSEDBIT_7_24 = Field.create(\"UNUSEDBIT_7_24\", 12, FieldType.BIT, 24);\n" +
                        "\tpublic static final Field UNUSEDBIT_7_25 = Field.create(\"UNUSEDBIT_7_25\", 12, FieldType.BIT, 25);\n" +
                        "\tpublic static final Field UNUSEDBIT_7_26 = Field.create(\"UNUSEDBIT_7_26\", 12, FieldType.BIT, 26);\n" +
                        "\tpublic static final Field UNUSEDBIT_7_27 = Field.create(\"UNUSEDBIT_7_27\", 12, FieldType.BIT, 27);\n" +
                        "\tpublic static final Field UNUSEDBIT_7_28 = Field.create(\"UNUSEDBIT_7_28\", 12, FieldType.BIT, 28);\n" +
                        "\tpublic static final Field UNUSEDBIT_7_29 = Field.create(\"UNUSEDBIT_7_29\", 12, FieldType.BIT, 29);\n" +
                        "\tpublic static final Field UNUSEDBIT_7_30 = Field.create(\"UNUSEDBIT_7_30\", 12, FieldType.BIT, 30);\n" +
                        "\tpublic static final Field UNUSEDBIT_7_31 = Field.create(\"UNUSEDBIT_7_31\", 12, FieldType.BIT, 31);\n",
                javaFieldsConsumer.getContent());
        assertEquals("// start of pid_s\n" +
                "struct pid_s {\n" +
                "\t/**\n" +
                "\t * offset 0\n" +
                "\t */\n" +
                "\tint8_t byte1 = (int8_t)0;\n" +
                "\t/**\n" +
                "\t * need 4 byte alignment\n" +
                "\tunits\n" +
                "\t * offset 1\n" +
                "\t */\n" +
                "\tuint8_t alignmentFill_at_1[1];\n" +
                "\t/**\n" +
                "\t * offset 2\n" +
                "\t */\n" +
                "\tint16_t short = (int16_t)0;\n" +
                "\t/**\n" +
                "\t * offset 4\n" +
                "\t */\n" +
                "\tint int2 = (int)0;\n" +
                "\t/**\n" +
                "\t * offset 8\n" +
                "\t */\n" +
                "\tint8_t byte2 = (int8_t)0;\n" +
                "\t/**\n" +
                "\t * need 4 byte alignment\n" +
                "\tunits\n" +
                "\t * offset 9\n" +
                "\t */\n" +
                "\tuint8_t alignmentFill_at_9[3];\n" +
                "\t/**\n" +
                "\toffset 12 bit 0 */\n" +
                "\tbool periodMs : 1 {};\n" +
                "\t/**\n" +
                "\toffset 12 bit 1 */\n" +
                "\tbool unusedBit_7_1 : 1 {};\n" +
                "\t/**\n" +
                "\toffset 12 bit 2 */\n" +
                "\tbool unusedBit_7_2 : 1 {};\n" +
                "\t/**\n" +
                "\toffset 12 bit 3 */\n" +
                "\tbool unusedBit_7_3 : 1 {};\n" +
                "\t/**\n" +
                "\toffset 12 bit 4 */\n" +
                "\tbool unusedBit_7_4 : 1 {};\n" +
                "\t/**\n" +
                "\toffset 12 bit 5 */\n" +
                "\tbool unusedBit_7_5 : 1 {};\n" +
                "\t/**\n" +
                "\toffset 12 bit 6 */\n" +
                "\tbool unusedBit_7_6 : 1 {};\n" +
                "\t/**\n" +
                "\toffset 12 bit 7 */\n" +
                "\tbool unusedBit_7_7 : 1 {};\n" +
                "\t/**\n" +
                "\toffset 12 bit 8 */\n" +
                "\tbool unusedBit_7_8 : 1 {};\n" +
                "\t/**\n" +
                "\toffset 12 bit 9 */\n" +
                "\tbool unusedBit_7_9 : 1 {};\n" +
                "\t/**\n" +
                "\toffset 12 bit 10 */\n" +
                "\tbool unusedBit_7_10 : 1 {};\n" +
                "\t/**\n" +
                "\toffset 12 bit 11 */\n" +
                "\tbool unusedBit_7_11 : 1 {};\n" +
                "\t/**\n" +
                "\toffset 12 bit 12 */\n" +
                "\tbool unusedBit_7_12 : 1 {};\n" +
                "\t/**\n" +
                "\toffset 12 bit 13 */\n" +
                "\tbool unusedBit_7_13 : 1 {};\n" +
                "\t/**\n" +
                "\toffset 12 bit 14 */\n" +
                "\tbool unusedBit_7_14 : 1 {};\n" +
                "\t/**\n" +
                "\toffset 12 bit 15 */\n" +
                "\tbool unusedBit_7_15 : 1 {};\n" +
                "\t/**\n" +
                "\toffset 12 bit 16 */\n" +
                "\tbool unusedBit_7_16 : 1 {};\n" +
                "\t/**\n" +
                "\toffset 12 bit 17 */\n" +
                "\tbool unusedBit_7_17 : 1 {};\n" +
                "\t/**\n" +
                "\toffset 12 bit 18 */\n" +
                "\tbool unusedBit_7_18 : 1 {};\n" +
                "\t/**\n" +
                "\toffset 12 bit 19 */\n" +
                "\tbool unusedBit_7_19 : 1 {};\n" +
                "\t/**\n" +
                "\toffset 12 bit 20 */\n" +
                "\tbool unusedBit_7_20 : 1 {};\n" +
                "\t/**\n" +
                "\toffset 12 bit 21 */\n" +
                "\tbool unusedBit_7_21 : 1 {};\n" +
                "\t/**\n" +
                "\toffset 12 bit 22 */\n" +
                "\tbool unusedBit_7_22 : 1 {};\n" +
                "\t/**\n" +
                "\toffset 12 bit 23 */\n" +
                "\tbool unusedBit_7_23 : 1 {};\n" +
                "\t/**\n" +
                "\toffset 12 bit 24 */\n" +
                "\tbool unusedBit_7_24 : 1 {};\n" +
                "\t/**\n" +
                "\toffset 12 bit 25 */\n" +
                "\tbool unusedBit_7_25 : 1 {};\n" +
                "\t/**\n" +
                "\toffset 12 bit 26 */\n" +
                "\tbool unusedBit_7_26 : 1 {};\n" +
                "\t/**\n" +
                "\toffset 12 bit 27 */\n" +
                "\tbool unusedBit_7_27 : 1 {};\n" +
                "\t/**\n" +
                "\toffset 12 bit 28 */\n" +
                "\tbool unusedBit_7_28 : 1 {};\n" +
                "\t/**\n" +
                "\toffset 12 bit 29 */\n" +
                "\tbool unusedBit_7_29 : 1 {};\n" +
                "\t/**\n" +
                "\toffset 12 bit 30 */\n" +
                "\tbool unusedBit_7_30 : 1 {};\n" +
                "\t/**\n" +
                "\toffset 12 bit 31 */\n" +
                "\tbool unusedBit_7_31 : 1 {};\n" +
                "};\n" +
                "static_assert(sizeof(pid_s) == 16);\n" +
                "\n", consumer.getContent());
    }

    @Test
    public void testArrayOfOne() throws IOException {
        String test = "struct pid_s\n" +
                "#define ERROR_BUFFER_SIZE 1\n" +
                "int[ERROR_BUFFER_SIZE iterate] autoscale field;;\"ratio\", 0.01, 0, 0, 650, 0\n" +
                "end_struct\n" +
                "";
        BaseCHeaderConsumer consumer = new BaseCHeaderConsumer();
        ReaderState state = new ReaderState();
        JavaFieldsConsumer javaFieldsConsumer = new TestJavaFieldsConsumer(state);
        state.readBufferedReader(test, consumer, javaFieldsConsumer);
        assertEquals("\tpublic static final Field FIELD1 = Field.create(\"FIELD1\", 0, FieldType.INT).setScale(0.01);\n",
                javaFieldsConsumer.getContent());
        assertEquals("// start of pid_s\n" +
                "struct pid_s {\n" +
                "\t/**\n" +
                "\tratio\n" +
                "\t * offset 0\n" +
                "\t */\n" +
                "\tscaled_channel<int, 100, 1> field[ERROR_BUFFER_SIZE];\n" +
                "};\n" +
                "static_assert(sizeof(pid_s) == 4);\n" +
                "\n", consumer.getContent());
    }

    @Test
    public void testStructAfterByte() throws IOException {
        String test = "struct struct_s\n" +
                "\tint int2\n" +
                "end_struct\n" +

                "struct_no_prefix pid_s\n" +
                "\tint8_t byte1\n" +
                "\tstruct_s struct\n" +
                "end_struct\n" +
                "";
        BaseCHeaderConsumer consumer = new BaseCHeaderConsumer();
        ReaderState state = new ReaderState();
        state.readBufferedReader(test, consumer);
        assertEquals("// start of struct_s\n" +
                        "struct struct_s {\n" +
                        "\t/**\n" +
                        "\t * offset 0\n" +
                        "\t */\n" +
                        "\tint int2 = (int)0;\n" +
                        "};\n" +
                        "static_assert(sizeof(struct_s) == 4);\n" +
                        "\n" +
                        "// start of pid_s\n" +
                        "struct pid_s {\n" +
                        "\t/**\n" +
                        "\t * offset 0\n" +
                        "\t */\n" +
                        "\tint8_t byte1 = (int8_t)0;\n" +
                        "\t/**\n" +
                        "\t * need 4 byte alignment\n" +
                        "\tunits\n" +
                        "\t * offset 1\n" +
                        "\t */\n" +
                        "\tuint8_t alignmentFill_at_1[3];\n" +
                        "\t/**\n" +
                        "\t * offset 4\n" +
                        "\t */\n" +
                        "\tstruct_s struct;\n" +
                        "};\n" +
                        "static_assert(sizeof(pid_s) == 8);\n" +
                        "\n",
                consumer.getContent());
    }

    @Test
    public void justTwoBytes() throws IOException {
        String test =
                "struct_no_prefix pid_s\n" +
                "\tint8_t byte1\n" +
                "\tint8_t byte2\n" +
                "end_struct\n" +
                "";
        BaseCHeaderConsumer consumer = new BaseCHeaderConsumer();
        ReaderState state = new ReaderState();
        state.readBufferedReader(test, consumer);
        assertEquals("// start of pid_s\n" +
                        "struct pid_s {\n" +
                        "\t/**\n" +
                        "\t * offset 0\n" +
                        "\t */\n" +
                        "\tint8_t byte1 = (int8_t)0;\n" +
                        "\t/**\n" +
                        "\t * offset 1\n" +
                        "\t */\n" +
                        "\tint8_t byte2 = (int8_t)0;\n" +
                        "\t/**\n" +
                        "\t * need 4 byte alignment\n" +
                        "\tunits\n" +
                        "\t * offset 2\n" +
                        "\t */\n" +
                        "\tuint8_t alignmentFill_at_2[2];\n" +
                        "};\n" +
                        "static_assert(sizeof(pid_s) == 4);\n" +
                        "\n",
                consumer.getContent());
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
            ConfigField cf = ConfigField.parse(state, "int_8 fi_eld;comm_;ts,1,1");
            assertEquals(cf.getType(), "int_8");
            assertEquals(cf.getName(), "fi_eld");
            assertEquals("Comment", cf.getComment(), "comm_");
            assertEquals(cf.getTsInfo(), "ts,1,1");
        }
        {
            ConfigField cf = ConfigField.parse(state, "int[3 iterate] field");
            assertEquals(cf.getType(), "int");
            assertEquals(cf.getArraySizes().length, 1);
            assertEquals(cf.getArraySizes()[0], 3);
            assertTrue("isIterate", cf.isIterate());
        }
        {
            ConfigField cf = ConfigField.parse(state, "int16_t crankingRpm;This,. value controls what RPM values we consider 'cranking' (any RPM below 'crankingRpm')\\nAnything above 'crankingRpm' would be 'running'");
            assertEquals(cf.getName(), "crankingRpm");
            assertEquals(cf.getArraySizes().length, 0);
            assertEquals(cf.getType(), "int16_t");
        }
        {
            ConfigField cf = ConfigField.parse(state, "MAP_sensor_config_s map");
            assertEquals(cf.getName(), "map");
            assertEquals(cf.getArraySizes().length, 0);
            assertEquals(cf.getType(), "MAP_sensor_config_s");
        }
        {
            ConfigField cf = ConfigField.parse(state, "MAP_sensor_config_s map;@see hasMapSensor\\n@see isMapAveragingEnabled");
            assertEquals(cf.getName(), "map");
            assertEquals(cf.getArraySizes().length, 0);
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

    @Test
    public void testStructTooltips() throws IOException {
        String test = "struct total\n" +
                "struct pid_s\n" +
                "floatms_t afr_type;PID dTime;\"ms\",      1,      0,       0, 3000,      0\n" +
                "percent_t afr_typet\n" +
"bit isForcedInduction;Does the vehicle have a turbo or supercharger?\n" +
                "bit enableFan1WithAc;+Turn on this fan when AC is on.\n" +
                "end_struct\n" +
                "pid_s pid;comment\n" +
        "end_struct\n";
        ReaderState state = new ReaderState();

        TestTSProjectConsumer tsProjectConsumer = new TestTSProjectConsumer("", state);
        state.readBufferedReader(test, tsProjectConsumer);
        assertEquals("pid_afr_type = scalar, F32, 0, \"ms\", 1, 0, 0, 3000, 0\n" +
                "pid_afr_typet = scalar, F32, 4, \"\", 1, 0, 0, 100, 0\n" +
                "pid_isForcedInduction = bits, U32, 8, [0:0], \"false\", \"true\"\n" +
                "pid_enableFan1WithAc = bits, U32, 8, [1:1], \"false\", \"true\"\n" +
                "pid_unusedBit_4_2 = bits, U32, 8, [2:2], \"false\", \"true\"\n" +
                "pid_unusedBit_4_3 = bits, U32, 8, [3:3], \"false\", \"true\"\n" +
                "pid_unusedBit_4_4 = bits, U32, 8, [4:4], \"false\", \"true\"\n" +
                "pid_unusedBit_4_5 = bits, U32, 8, [5:5], \"false\", \"true\"\n" +
                "pid_unusedBit_4_6 = bits, U32, 8, [6:6], \"false\", \"true\"\n" +
                "pid_unusedBit_4_7 = bits, U32, 8, [7:7], \"false\", \"true\"\n" +
                "pid_unusedBit_4_8 = bits, U32, 8, [8:8], \"false\", \"true\"\n" +
                "pid_unusedBit_4_9 = bits, U32, 8, [9:9], \"false\", \"true\"\n" +
                "pid_unusedBit_4_10 = bits, U32, 8, [10:10], \"false\", \"true\"\n" +
                "pid_unusedBit_4_11 = bits, U32, 8, [11:11], \"false\", \"true\"\n" +
                "pid_unusedBit_4_12 = bits, U32, 8, [12:12], \"false\", \"true\"\n" +
                "pid_unusedBit_4_13 = bits, U32, 8, [13:13], \"false\", \"true\"\n" +
                "pid_unusedBit_4_14 = bits, U32, 8, [14:14], \"false\", \"true\"\n" +
                "pid_unusedBit_4_15 = bits, U32, 8, [15:15], \"false\", \"true\"\n" +
                "pid_unusedBit_4_16 = bits, U32, 8, [16:16], \"false\", \"true\"\n" +
                "pid_unusedBit_4_17 = bits, U32, 8, [17:17], \"false\", \"true\"\n" +
                "pid_unusedBit_4_18 = bits, U32, 8, [18:18], \"false\", \"true\"\n" +
                "pid_unusedBit_4_19 = bits, U32, 8, [19:19], \"false\", \"true\"\n" +
                "pid_unusedBit_4_20 = bits, U32, 8, [20:20], \"false\", \"true\"\n" +
                "pid_unusedBit_4_21 = bits, U32, 8, [21:21], \"false\", \"true\"\n" +
                "pid_unusedBit_4_22 = bits, U32, 8, [22:22], \"false\", \"true\"\n" +
                "pid_unusedBit_4_23 = bits, U32, 8, [23:23], \"false\", \"true\"\n" +
                "pid_unusedBit_4_24 = bits, U32, 8, [24:24], \"false\", \"true\"\n" +
                "pid_unusedBit_4_25 = bits, U32, 8, [25:25], \"false\", \"true\"\n" +
                "pid_unusedBit_4_26 = bits, U32, 8, [26:26], \"false\", \"true\"\n" +
                "pid_unusedBit_4_27 = bits, U32, 8, [27:27], \"false\", \"true\"\n" +
                "pid_unusedBit_4_28 = bits, U32, 8, [28:28], \"false\", \"true\"\n" +
                "pid_unusedBit_4_29 = bits, U32, 8, [29:29], \"false\", \"true\"\n" +
                "pid_unusedBit_4_30 = bits, U32, 8, [30:30], \"false\", \"true\"\n" +
                "pid_unusedBit_4_31 = bits, U32, 8, [31:31], \"false\", \"true\"\n" +
                "; total TS size = 12\n", tsProjectConsumer.getContent());
        assertEquals(
                "\tpid_afr_type = \"PID dTime\"\n" +
                "\tpid_isForcedInduction = \"Does the vehicle have a turbo or supercharger?\"\n" +
                        "\tpid_enableFan1WithAc = \"Turn on this fan when AC is on.\"\n", tsProjectConsumer.getSettingContextHelp().toString());
    }

    @Test
    public void testUnquote() {
        assertEquals("x", ConfigField.unquote("\"x\""));
        // leave broken opening-only quote!
        assertEquals("\"x", ConfigField.unquote("\"x"));
// this does not look great let's document this corner case for now
        assertEquals("x\"\n" +
                "\"y", ConfigField.unquote("\"x\"\n\"y\""));
    }
}
