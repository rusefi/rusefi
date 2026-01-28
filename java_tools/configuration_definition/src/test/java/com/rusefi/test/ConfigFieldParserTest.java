package com.rusefi.test;

import com.rusefi.*;
import com.rusefi.output.*;
import com.rusefi.parse.TypesHelper;
import org.junit.jupiter.api.Assertions;
import org.junit.jupiter.api.Test;
import org.junit.jupiter.api.function.Executable;

import static org.junit.jupiter.api.Assertions.*;

/**
 * Andrey Belomutskiy, (c) 2013-2020
 * 1/15/15
 */
public class ConfigFieldParserTest {
    @Test
    public void testErrorDetection() {
        Assertions.assertThrows(IllegalStateException.class, () -> ConfigFieldImpl.getScaleSpec("2.54", "test"));
    }

    @Test
    public void testByteArray() {
        ReaderStateImpl state = new ReaderStateImpl();
        {
            ConfigFieldImpl cf = ConfigFieldImpl.parse(state, "uint8_t[8] field");
            assertEquals(cf.getTypeName(), "uint8_t");
            assertEquals(cf.getArraySizes().length, 1);
            assertEquals(cf.getArraySizes()[0], 8);
            assertEquals(cf.getSize(null), 8);
            assertFalse(cf.isIterate(), "isIterate");
        }
    }

    @Test
    public void testNameTemplating() {
        ReaderStateImpl state = new ReaderStateImpl();
        state.getVariableRegistry().put("name", "value");
        {
            ConfigFieldImpl cf = ConfigFieldImpl.parse(state, "uint8_t @@name@@");
            assertEquals(cf.getName(), "value");
        }
    }

    @Test
    public void testByte3dArray() {
        ReaderStateImpl state = new ReaderStateImpl();
        {
            ConfigFieldImpl cf = ConfigFieldImpl.parse(state, "uint8_t[8 x 16] field");
            assertEquals(cf.getTypeName(), "uint8_t");
            assertEquals(cf.getArraySizes().length, 2);
            assertEquals(cf.getArraySizes()[0], 8);
            assertEquals(cf.getArraySizes()[1], 16);
            assertEquals(cf.getSize(null), 128);
            assertFalse(cf.isIterate(), "isIterate");
        }
    }

    @Test
    public void testFloatMsAlias() {
        String test = "struct pid_s\n" +
                "floatms_t afr_type;PID dTime;\"ms\",      1.0,      0,       0, 3000,      0, noMsqSave\n" +
                "percent_t afr_typet;PID dTime;\"ms\",      0.1,      0,       0, 3000,      0\n" +
                "end_struct\n";
        ReaderStateImpl state = new ReaderStateImpl();

        TestTSProjectConsumer tsProjectConsumer = new TestTSProjectConsumer(state);
        state.readBufferedReader(test, tsProjectConsumer);
        assertEquals("afr_type = scalar, F32, 0, \"ms\", 1, 0, 0, 3000, 0, noMsqSave\n" +
            "afr_typet = scalar, F32, 4, \"ms\", 0.1, 0, 0, 3000, 0\n" +
            "; total TS size = 8\n", tsProjectConsumer.getContent());
    }

    @Test
    public void testArray2D() {
        String test = "struct pid_s\n" +
                "int[3 x 1] afr_type;PID dTime;\"ms\",      1.0,      0,       0, 3000,      0, noMsqSave\n" +
                "end_struct\n";
        ReaderStateImpl state = new ReaderStateImpl();

        TestTSProjectConsumer tsProjectConsumer = new TestTSProjectConsumer(state);
        state.readBufferedReader(test, tsProjectConsumer);
        assertEquals("afr_type = array, S32, 0, [1x3], \"ms\", 1, 0, 0, 3000, 0, noMsqSave\n" +
                "; total TS size = 12\n", tsProjectConsumer.getContent());
    }

    @Test
    public void testMinValue() {
        assertThrows(ParsingException.class, () -> {
            handle("struct pid_s\n" +
                "uint8_t[3 x 1] afr_type;PID dTime;\"ms\",      1.0,      0,       -10, 3000,      0, noMsqSave\n" +
                "end_struct\n");
        });
    }

    private static ReaderStateImpl handle(String meta) {
        ReaderStateImpl state = new ReaderStateImpl();

        TestTSProjectConsumer tsProjectConsumer = new TestTSProjectConsumer(state);
        state.readBufferedReader(meta, tsProjectConsumer);
        return state;
    }

    @Test
    public void testMaxValue() {
        handle("struct pid_s\n" +
            "uint8_t[6] autoscale values;;\"volts\", {1/100}, 0, 0, 2.5, 2\n" +
            "end_struct\n");
        handle("struct pid_s\n" +
            "uint8_t autoscale maxIdleRegionRpm;Below this RPM, the idle region is active, idle+300 would be a good value;\"RPM\", 50, 0, 0, 12000, 0\n" +
            "end_struct\n");

        assertThrows(ParsingException.class, () -> {
            handle("struct pid_s\n" +
                "int8_t[8] ignitionCltCorrTempBins;CLT-based timing correction;\"C\", 1, 0, -40, 250, 0\n" +
                "end_struct\n");
        });
    }

    @Test
    public void testEmbeddedStructs() {
        assertThrows(IllegalStateException.class, () -> {
            handle("struct pid_s\n" +
                "int afr_type;PID dTime;\"ms\",      1.0,      0,       0, 3000,      0, noMsqSave\n" +
                "struct pid_s\n" +
                "int afr_type;PID dTime;\"ms\",      1.0,      0,       0, 3000,      0, noMsqSave\n" +
                "end_struct\n" +
                "end_struct\n");
        });
    }

    @Test
    public void testArrayBitStringValue() {
        String test = "struct pid_s\n" +
            "int[3 x 1] afr_type;;{bitStringValue(fuelUnits, fuelAlgorithm) },      1.0,      0,       0, 3000,      0, noMsqSave\n" +
            "end_struct\n";
        ReaderStateImpl state = new ReaderStateImpl();

        TestTSProjectConsumer tsProjectConsumer = new TestTSProjectConsumer(state);
        state.readBufferedReader(test, tsProjectConsumer);
        assertEquals("afr_type = array, S32, 0, [1x3], {bitStringValue(fuelUnits, fuelAlgorithm) }, 1, 0, 0, 3000, 0, noMsqSave\n" +
            "; total TS size = 12\n", tsProjectConsumer.getContent());
    }


    @Test
    public void testSameFieldTwice() {
      assertThrows(IllegalStateException.class, () -> {
        String test = "struct pid_s\n" +
          "int afr_type1;PID dTime;\"ms\",      1,      0,       0, 3000,      0\n" +
          "int afr_type2;PID dTime;\"ms\",      1,      0,       0, 3000,      0\n" +
          "int afr_type1;PID dTime;\"ms\",      1,      0,       0, 3000,      0\n" +
          "end_struct\n";
        ReaderStateImpl state = new ReaderStateImpl();

        BaseCHeaderConsumer consumer = new BaseCHeaderConsumer();
        state.readBufferedReader(test, consumer);
      });
    }

    @Test
    public void testCustomEnum() {
        String test = "struct pid_s\n" +
                "#define ego_sensor_e_enum \"BPSX\", \"Innovate\", \"14Point7\"\n" +
                "custom ego_sensor_e 1 bits, S08, @OFFSET@, [0:1], @@ego_sensor_e_enum@@\n" +
                "ego_sensor_e afr_type1;\n" +
                "ego_sensor_e afr_type2;\n" +
                "int16_t int\n" +
                "end_struct\n";
        ReaderStateImpl state = new ReaderStateImpl();

        TestTSProjectConsumer tsProjectConsumer = new TestTSProjectConsumer(state);
        state.readBufferedReader(test, tsProjectConsumer);
        assertEquals("afr_type1 = bits, S08, 0, [0:1], \"BPSX\", \"Innovate\", \"14Point7\", \"INVALID\"\n" +
                "afr_type2 = bits, S08, 1, [0:1], \"BPSX\", \"Innovate\", \"14Point7\", \"INVALID\"\n" +
                "int = scalar, S16, 2, \"\", 1, 0, 0, 32000, 0\n" +
                "; total TS size = 4\n", tsProjectConsumer.getContent());
    }

    @Test
    public void testCustomEnumAlignment() {
        assertThrows(SizeMismatchException.class, new Executable() {
            @Override
            public void execute() throws Throwable {
                String test = "struct pid_s\n" +
                    "#define can_wbo_type_e_enum \"RusEFI\", \"AEM X-series\", \"Disabled/Analog\"\n" +
                    "\tcustom can_wbo_type_e 2 bits, U08, @OFFSET@, [0:1], @@can_wbo_type_e_enum@@\n" +
                    "\tcan_wbo_type_e wboType1;\n" +
                    "end_struct\n";
                ReaderStateImpl state = new ReaderStateImpl();

                TestTSProjectConsumer tsProjectConsumer = new TestTSProjectConsumer(state);
                state.readBufferedReader(test, tsProjectConsumer);
            }
        });
    }

    @Test
    public void testCustomEnumWithTsVariable() {
        String test = "struct pid_s\n" +
            "#define ego_sensor_e_enum \"BPSX\", \"Innovate\", \"14Point7\"\n" +
            "custom ego_sensor_e 1 bits, S08, @OFFSET@, [0:1], $ego_sensor_e_list\n" +
            "ego_sensor_e afr_type1;\n" +
            "int16_t int\n" +
            "end_struct\n";
        ReaderStateImpl state = new ReaderStateImpl();

        TestTSProjectConsumer tsProjectConsumer = new TestTSProjectConsumer(state);
        state.readBufferedReader(test, tsProjectConsumer);
        assertEquals("afr_type1 = bits, S08, 0, [0:1], $ego_sensor_e_list\n" +
            "int = scalar, S16, 2, \"\", 1, 0, 0, 32000, 0\n" +
            "; total TS size = 4\n", tsProjectConsumer.getContent());
    }

    @Test
    public void testShortForm() {
        String test = "struct pid_s\n" +
                "#define ego_sensor_e_enum \"BPSX\"\n" +
                "custom ego_sensor_e 1 bits, S08, @OFFSET@, [0:1], @@ego_sensor_e_enum@@\n" +
                "ego_sensor_e afr_type1;\n" +
                "ego_sensor_e afr_type2;\n" +
                "int8_t int\n" +
                "end_struct\n";
        ReaderStateImpl state = new ReaderStateImpl();

        TestTSProjectConsumer tsProjectConsumer = new TestTSProjectConsumer(state);
        state.readBufferedReader(test, tsProjectConsumer);
        assertEquals("afr_type1 = bits, S08, 0, [0:1], \"BPSX\"\n" +
                "afr_type2 = bits, S08, 1, [0:1], \"BPSX\"\n" +
                "int = scalar, S08, 2, \"\", 1, 0, 0, 100, 0\n" +
                "; total TS size = 4\n", tsProjectConsumer.getContent());
    }

    @Test
    public void test2byteCustomEnum() {
        String test = "struct pid_s\n" +
                "#define ego_sensor_e_enum \"BPSX\", \"Innovate\", \"14Point7\"\n" +
                "custom ego_sensor_e2 2 bits, S16, @OFFSET@, [0:1], @@ego_sensor_e_enum@@\n" +
                "int8_t int\n" +
                "ego_sensor_e2 afr_type1;\n" +
                "ego_sensor_e2 afr_type2;\n" +
                "end_struct\n";
        ReaderStateImpl state = new ReaderStateImpl();

        TestTSProjectConsumer tsProjectConsumer = new TestTSProjectConsumer(state);
        state.readBufferedReader(test, tsProjectConsumer);
        assertEquals("int = scalar, S08, 0, \"\", 1, 0, 0, 100, 0\n" +
                "afr_type1 = bits, S16, 2, [0:1], \"BPSX\", \"Innovate\", \"14Point7\", \"INVALID\"\n" +
                "afr_type2 = bits, S16, 4, [0:1], \"BPSX\", \"Innovate\", \"14Point7\", \"INVALID\"\n" +
                "; total TS size = 8\n", tsProjectConsumer.getContent());
    }

    @Test
    public void test4byteCustomEnum() {
        String test = "struct pid_s\n" +
                "#define ego_sensor_e_enum \"BPSX\", \"Innovate\", \"14Point7\"\n" +
                "custom ego_sensor_e4 4 bits, S32, @OFFSET@, [0:1], @@ego_sensor_e_enum@@\n" +
                "int8_t int2\n" +
                "ego_sensor_e4 afr_type3;\n" +
                "end_struct\n";
        ReaderStateImpl state = new ReaderStateImpl();

        TestTSProjectConsumer tsProjectConsumer = new TestTSProjectConsumer(state);
        state.readBufferedReader(test, tsProjectConsumer);
        assertEquals("int2 = scalar, S08, 0, \"\", 1, 0, 0, 100, 0\n" +
                "afr_type3 = bits, S32, 4, [0:1], \"BPSX\", \"Innovate\", \"14Point7\", \"INVALID\"\n" +
                "; total TS size = 8\n", tsProjectConsumer.getContent());
    }

    @Test
    public void alignFourByteTypes() {
        // we expect padding before each 4 byte field
        String test = "struct pid_s\n" +
                "\tint16_t periodMs1;PID dTime;\"ms\",      1,      0,       0, 3000,      0\n" +
                "\tint periodSec;PID dTime;\"ms\",      1,      0,       0, 3000,      0\n" +
                "\tint16_t periodMs2;PID dTime;\"ms\",      1,      0,       0, 3000,      0\n" +
                "\tint periodSec2;PID dTime;\"ms\",      1,      0,       0, 3000,      0\n" +
                "end_struct\n";
        ReaderStateImpl state = new ReaderStateImpl();

        JavaFieldsConsumer javaFieldsConsumer = new TestJavaFieldsConsumer(state);
        state.readBufferedReader(test, (javaFieldsConsumer));

        assertEquals(16, TypesHelper.getElementSize(state, "pid_s"));

        ConfigStructure structure = state.getStructures().get("pid_s");
        ConfigField firstField = structure.getcFields().get(0);
        assertEquals("ms", firstField.getUnits());
    }

    @Test
    public void alignArray6() {
        // we expect padding before each 4 byte field
        String test = "struct vr_threshold_s\n" +
                "\tuint8_t pin;\n" +
                "\tuint8_t[3] pad;;\"\",1,0,0,0,0\n" +
                "\tuint8_t[6] autoscale rpmBins;;\"rpm\", 1, 0, 0, 12, 0\n" +
                "\tuint8_t[6] autoscale values;;\"volts\", 1, 0, 0, 2.5, 2\n" +
                "end_struct\n\n";
        ReaderStateImpl state = new ReaderStateImpl();

        JavaFieldsConsumer javaFieldsConsumer = new TestJavaFieldsConsumer(state);
        state.readBufferedReader(test, (javaFieldsConsumer));

        assertEquals("\tpublic static final Field PIN = Field.create(\"PIN\", 0, FieldType.INT8).setScale(1.0).setBaseOffset(0);\n" +
                "\tpublic static final Field PAD = Field.create(\"PAD\", 1, FieldType.INT8).setScale(1.0).setBaseOffset(0);\n" +
                "\tpublic static final Field RPMBINS = Field.create(\"RPMBINS\", 4, FieldType.INT8).setScale(1.0).setBaseOffset(0);\n" +
                "\tpublic static final Field VALUES = Field.create(\"VALUES\", 10, FieldType.INT8).setScale(1.0).setBaseOffset(0);\n", javaFieldsConsumer.getContent());
    }

    @Test
    public void manyStartAreNotMultiplication() {
        String test = "struct pid_s\n" +
                "#define ERROR_BUFFER_SIZE \"***\"\n" +
                "end_struct\n" +
                "";
        new ReaderStateImpl().readBufferedReader(test);
    }

    @Test
    public void invalidDefine() {
      assertThrows(IllegalStateException.class, () -> {
        String test = "struct pid_s\n" +
          VariableRegistry.DEFINE + " show show_Hellen121vag_presets true\n" +
          "end_struct\n" +
          "";
        new ReaderStateImpl().readBufferedReader(test);
      });
    }

    @Test
    public void multiplicationInDefine() {
        String test = "struct pid_s\n" +
                "#define ERROR_BUFFER_SIZE 120\n" +
                "#define ERROR_BUFFER_COUNT 120\n" +
                "#define RESULT @@ERROR_BUFFER_SIZE@@*@@ERROR_BUFFER_COUNT@@\n" +
                "\tint16_t periodMs;PID dTime;\"ms\",      1,      0,       0, 3000,      0\n" +
                "end_struct\n" +
                "";

        ReaderStateImpl state = new ReaderStateImpl();
        state.readBufferedReader(test);

        assertEquals("#define ERROR_BUFFER_COUNT 120\n" +
                "#define ERROR_BUFFER_SIZE 120\n" +
                "#define RESULT 14400\n", state.getVariableRegistry().getDefinesSection());
    }
    @Test
    public void expressionInMultiplier() {
        String test = "struct pid_s\n" +
                "\tint16_t autoscale periodMs;PID dTime;\"ms\",      {1/10},      0,       0, 3000,      0\n" +
                "\tint16_t periodMs2;PID dTime;\"ms\",      1,      0,       0, 3000,      0\n" +
                "custom afr_table_t 4x4 array,   U08,   @OFFSET@, [4x4],\"deg\",\t   {1/10},     0,        0,  25.0,     1 \n" +
                "afr_table_t afrTable;\t\t\n" +
                "end_struct\n" +
                "";

        ReaderStateImpl state = new ReaderStateImpl();
        TSProjectConsumer tsProjectConsumer = new TestTSProjectConsumer(state);

        state.readBufferedReader(test, tsProjectConsumer);

        assertEquals("periodMs = scalar, S16, 0, \"ms\", 0.1, 0, 0, 3000, 0\n" +
                "periodMs2 = scalar, S16, 2, \"ms\", 1, 0, 0, 3000, 0\n" +
                "afrTable = array, U08, 4, [4x4],\"deg\", 0.1, 0, 0, 25.0, 1\n" +
                "; total TS size = 20\n", tsProjectConsumer.getContent());
    }

    @Test
    public void useCustomType() {
        ReaderStateImpl state = new ReaderStateImpl();
        String test = "struct pid_s\n" +
                "#define ERROR_BUFFER_SIZE 120\n" +
                "\tcustom critical_error_message_t @@ERROR_BUFFER_SIZE@@ string, ASCII, @OFFSET@, @@ERROR_BUFFER_SIZE@@\n" +
                "\tcritical_error_message_t var;\n" +
                "\tint16_t periodMs;PID dTime;\"ms\",      1,      0,       0, 3000,      0\n" +
                "end_struct\n" +
                "";

        JavaFieldsConsumer javaFieldsConsumer = new TestJavaFieldsConsumer(state);
        state.readBufferedReader(test, javaFieldsConsumer);

        assertEquals("\tpublic static final Field VAR = Field.create(\"VAR\", 0, 120, FieldType.STRING).setScale(1.0).setBaseOffset(0);\n" +
                        "\tpublic static final Field PERIODMS = Field.create(\"PERIODMS\", 120, FieldType.INT16).setScale(1.0).setBaseOffset(0);\n" +
                        "\tpublic static final Field ALIGNMENTFILL_AT_122 = Field.create(\"ALIGNMENTFILL_AT_122\", 122, FieldType.INT8).setScale(1.0).setBaseOffset(0);\n",
                javaFieldsConsumer.getContent());
    }

    @Test
    public void alignmentTestJava() {
        ReaderStateImpl state = new ReaderStateImpl();
        String test = "struct pid_s\n" +
                "\tint16_t periodMs;PID dTime;\"ms\",      1,      0,       0, 3000,      0\n" +
                "\tint8_t periodByte;PID dTime;\"ms\",      1,      0,       0, 30,      0\n" +
                "\tfloat periodFloat;PID dTime;\"ms\",      1,      0,       0, 3000,      0\n" +
                "end_struct\n" +
                "";

        JavaFieldsConsumer javaFieldsConsumer = new TestJavaFieldsConsumer(state);
        BaseCHeaderConsumer cConsumer = new BaseCHeaderConsumer();

        state.readBufferedReader(test, javaFieldsConsumer, cConsumer);

        assertEquals("\tpublic static final Field PERIODMS = Field.create(\"PERIODMS\", 0, FieldType.INT16).setScale(1.0).setBaseOffset(0);\n" +
                        "\tpublic static final Field PERIODBYTE = Field.create(\"PERIODBYTE\", 2, FieldType.INT8).setScale(1.0).setBaseOffset(0);\n" +
                        "\tpublic static final Field ALIGNMENTFILL_AT_3 = Field.create(\"ALIGNMENTFILL_AT_3\", 3, FieldType.INT8).setScale(1.0).setBaseOffset(0);\n" +
                        "\tpublic static final Field PERIODFLOAT = Field.create(\"PERIODFLOAT\", 4, FieldType.FLOAT).setBaseOffset(0);\n",
                javaFieldsConsumer.getContent());

        assertEquals("// start of pid_s\n" +
                "struct pid_s {\n" +
                "\t/**\n" +
                "\t * PID dTime\n" +
                "\t * units: ms\n" +
                "\t * offset 0\n" +
                "\t */\n" +
                "\tint16_t periodMs = (int16_t)0;\n" +
                "\t/**\n" +
                "\t * PID dTime\n" +
                "\t * units: ms\n" +
                "\t * offset 2\n" +
                "\t */\n" +
                "\tint8_t periodByte = (int8_t)0;\n" +
                "\t/**\n" +
                "\t * need 4 byte alignment\n" +
                "\t * units: units\n" +
                "\t * offset 3\n" +
                "\t */\n" +
                "\tuint8_t alignmentFill_at_3[1] = {};\n" +
                "\t/**\n" +
                "\t * PID dTime\n" +
                "\t * units: ms\n" +
                "\t * offset 4\n" +
                "\t */\n" +
                "\tfloat periodFloat = (float)0;\n" +
                "};\n" +
                "static_assert(sizeof(pid_s) == 8);\n" +
                "\n",
                cConsumer.getContent());
    }

    @Test
    public void testDefineChar() {
        ReaderStateImpl state = new ReaderStateImpl();
        String test =
                "#define SD_r 'r'\n" +
                        "";

        JavaFieldsConsumer javaFieldsConsumer = new TestJavaFieldsConsumer(state);
        state.readBufferedReader(test, javaFieldsConsumer);

        assertEquals("\tpublic static final char SD_r = 'r';\n" +
                        "",
                state.getVariableRegistry().getJavaConstants());
    }

    @Test
    public void testDefine() {
        ReaderStateImpl state = new ReaderStateImpl();
        String test =
                "#define ERROR_BUFFER_SIZE 120\n" +
                        "#define ERROR_BUFFER_SIZE_H 0x120\n" +
                "";

        JavaFieldsConsumer javaFieldsConsumer = new TestJavaFieldsConsumer(state);
        state.readBufferedReader(test, (javaFieldsConsumer));

        assertEquals("\tpublic static final int ERROR_BUFFER_SIZE = 120;\n" +
                        "\tpublic static final int ERROR_BUFFER_SIZE_H = 0x120;\n" +
                        "",
                state.getVariableRegistry().getJavaConstants());
    }

    @Test
    public void testLegacyVisible() {
        {
            ReaderStateImpl state = new ReaderStateImpl();
            ConfigFieldImpl cf = ConfigFieldImpl.parse(state, "int field");
            assertEquals(cf.getTypeName(), "int");

            assertEquals(cf.getName(), "field", "Unexpected Field Name");
        }

        {
            ReaderStateImpl state = new ReaderStateImpl();
            String test = "struct pid_s\n" +
                    "\tint16_t offset;Linear addition to PID logic;\"\",      1,      0,       -1000, 1000,      0\n" +
                    "\tint16_t periodMs;PID dTime;\"ms\",      1,      0,       0, 3000,      0\n" +
                    "\tint16_t minValue;Output min value;\"\",        1,     0,  -30000,    30000.0,  0\n" +
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


            assertEquals("\tpublic static final Field OFFSET = Field.create(\"OFFSET\", 0, FieldType.INT16).setScale(1.0).setBaseOffset(0);\n" +
                            "\tpublic static final Field PERIODMS = Field.create(\"PERIODMS\", 2, FieldType.INT16).setScale(1.0).setBaseOffset(0);\n" +
                            "\tpublic static final Field MINVALUE = Field.create(\"MINVALUE\", 4, FieldType.INT16).setScale(1.0).setBaseOffset(0);\n" +
                            "\tpublic static final Field ALIGNMENTFILL_AT_6 = Field.create(\"ALIGNMENTFILL_AT_6\", 6, FieldType.INT8).setScale(1.0).setBaseOffset(0);\n" +
                            "\tpublic static final Field ALTERNATORCONTROL_OFFSET = Field.create(\"ALTERNATORCONTROL_OFFSET\", 0, FieldType.INT16).setScale(1.0).setBaseOffset(0);\n" +
                            "\tpublic static final Field ALTERNATORCONTROL_PERIODMS = Field.create(\"ALTERNATORCONTROL_PERIODMS\", 2, FieldType.INT16).setScale(1.0).setBaseOffset(0);\n" +
                            "\tpublic static final Field ALTERNATORCONTROL_MINVALUE = Field.create(\"ALTERNATORCONTROL_MINVALUE\", 4, FieldType.INT16).setScale(1.0).setBaseOffset(0);\n" +
                            "\tpublic static final Field ALTERNATORCONTROL_ALIGNMENTFILL_AT_6 = Field.create(\"ALTERNATORCONTROL_ALIGNMENTFILL_AT_6\", 6, FieldType.INT8).setScale(1.0).setBaseOffset(0);\n" +
                            "\tpublic static final Field ETB1_OFFSET = Field.create(\"ETB1_OFFSET\", 8, FieldType.INT16).setScale(1.0).setBaseOffset(0);\n" +
                            "\tpublic static final Field ETB1_PERIODMS = Field.create(\"ETB1_PERIODMS\", 10, FieldType.INT16).setScale(1.0).setBaseOffset(0);\n" +
                            "\tpublic static final Field ETB1_MINVALUE = Field.create(\"ETB1_MINVALUE\", 12, FieldType.INT16).setScale(1.0).setBaseOffset(0);\n" +
                            "\tpublic static final Field ETB1_ALIGNMENTFILL_AT_6 = Field.create(\"ETB1_ALIGNMENTFILL_AT_6\", 14, FieldType.INT8).setScale(1.0).setBaseOffset(0);\n" +
                            "\tpublic static final Field ETB2_OFFSET = Field.create(\"ETB2_OFFSET\", 16, FieldType.INT16).setScale(1.0).setBaseOffset(0);\n" +
                            "\tpublic static final Field ETB2_PERIODMS = Field.create(\"ETB2_PERIODMS\", 18, FieldType.INT16).setScale(1.0).setBaseOffset(0);\n" +
                            "\tpublic static final Field ETB2_MINVALUE = Field.create(\"ETB2_MINVALUE\", 20, FieldType.INT16).setScale(1.0).setBaseOffset(0);\n" +
                            "\tpublic static final Field ETB2_ALIGNMENTFILL_AT_6 = Field.create(\"ETB2_ALIGNMENTFILL_AT_6\", 22, FieldType.INT8).setScale(1.0).setBaseOffset(0);\n",
                    javaFieldsConsumer.getContent());
        }
    }

    @Test
    public void test2byteOffset() {
        String test = "struct_no_prefix pid_s\n" +
                "\tint8_t byte1\n" +
                "\tint16_t short\n" +
                "\tint int2\n" +
                "\tint8_t byte2\n" +
                "\tbit periodMs\n" +
                "end_struct\n" +

                "";
        BaseCHeaderConsumer consumer = new BaseCHeaderConsumer();
        ReaderStateImpl state = new ReaderStateImpl();
        JavaFieldsConsumer javaFieldsConsumer = new TestJavaFieldsConsumer(state);
        state.readBufferedReader(test, consumer, javaFieldsConsumer);

        assertEquals(1, state.getDefaultBitNameCounter());

        assertEquals("\tpublic static final Field BYTE1 = Field.create(\"BYTE1\", 0, FieldType.INT8).setScale(1.0).setBaseOffset(0);\n" +
                        "\tpublic static final Field ALIGNMENTFILL_AT_1 = Field.create(\"ALIGNMENTFILL_AT_1\", 1, FieldType.INT8).setScale(1.0).setBaseOffset(0);\n" +
                        "\tpublic static final Field SHORT = Field.create(\"SHORT\", 2, FieldType.INT16).setScale(1.0).setBaseOffset(0);\n" +
                        "\tpublic static final Field INT2 = Field.create(\"INT2\", 4, FieldType.INT).setScale(1.0).setBaseOffset(0);\n" +
                        "\tpublic static final Field BYTE2 = Field.create(\"BYTE2\", 8, FieldType.INT8).setScale(1.0).setBaseOffset(0);\n" +
                        "\tpublic static final Field ALIGNMENTFILL_AT_9 = Field.create(\"ALIGNMENTFILL_AT_9\", 9, FieldType.INT8).setScale(1.0).setBaseOffset(0);\n" +
                        "\tpublic static final Field PERIODMS = Field.create(\"PERIODMS\", 12, FieldType.BIT, 0).setBaseOffset(0);\n",
                javaFieldsConsumer.getContent());
        assertEquals("// start of pid_s\n" +
            "struct pid_s {\n" +
            "\t/**\n" +
            "\t * offset 0\n" +
            "\t */\n" +
            "\tint8_t byte1 = (int8_t)0;\n" +
            "\t/**\n" +
            "\t * need 4 byte alignment\n" +
            "\t * units: units\n" +
            "\t * offset 1\n" +
            "\t */\n" +
            "\tuint8_t alignmentFill_at_1[1] = {};\n" +
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
            "\t * units: units\n" +
            "\t * offset 9\n" +
            "\t */\n" +
            "\tuint8_t alignmentFill_at_9[3] = {};\n" +
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
    public void testArrayOfOne() {
        String test = "struct pid_s\n" +
                "#define ERROR_BUFFER_SIZE 1\n" +
                "int[ERROR_BUFFER_SIZE iterate] autoscale field;;\"ratio\", 0.01, 0, 0, 650, 0\n" +
                "end_struct\n" +
                "";
        BaseCHeaderConsumer consumer = new BaseCHeaderConsumer();
        ReaderStateImpl state = new ReaderStateImpl();
        JavaFieldsConsumer javaFieldsConsumer = new TestJavaFieldsConsumer(state);
        state.readBufferedReader(test, consumer, javaFieldsConsumer);
        assertEquals("\tpublic static final Field FIELD1 = Field.create(\"FIELD1\", 0, FieldType.INT).setScale(0.01).setBaseOffset(0);\n",
                javaFieldsConsumer.getContent());
        assertEquals("// start of pid_s\n" +
            "struct pid_s {\n" +
            "\t/**\n" +
            "\t * units: ratio\n" +
            "\t * offset 0\n" +
            "\t */\n" +
            "\tscaled_channel<int, 100, 1> field[ERROR_BUFFER_SIZE] = {};\n" +
            "};\n" +
            "static_assert(sizeof(pid_s) == 4);\n" +
            "\n", consumer.getContent());
    }

    @Test
    public void testStructAfterByte() {
        String test = "struct struct_s\n" +
                "\tint int2\n" +
                "end_struct\n" +

                "struct_no_prefix pid_s\n" +
                "\tint8_t byte1\n" +
                "\tstruct_s struct\n" +
                "end_struct\n" +
                "";
        BaseCHeaderConsumer consumer = new BaseCHeaderConsumer();
        ReaderStateImpl state = new ReaderStateImpl();
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
                "\t * units: units\n" +
                "\t * offset 1\n" +
                "\t */\n" +
                "\tuint8_t alignmentFill_at_1[3] = {};\n" +
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
    public void justTwoBytes() {
        String test =
                "struct_no_prefix pid_s\n" +
                "\tint8_t byte1\n" +
                "\tint8_t byte2\n" +
                "end_struct\n" +
                "";
        BaseCHeaderConsumer consumer = new BaseCHeaderConsumer();
        ReaderStateImpl state = new ReaderStateImpl();
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
                "\t * units: units\n" +
                "\t * offset 2\n" +
                "\t */\n" +
                "\tuint8_t alignmentFill_at_2[2] = {};\n" +
                "};\n" +
                "static_assert(sizeof(pid_s) == 4);\n" +
                "\n",
                consumer.getContent());
    }

    @Test
    public void testParseLine() {
        ReaderStateImpl state = new ReaderStateImpl();
        assertNull(ConfigFieldImpl.parse(state, "int"));
        {
            ConfigFieldImpl cf = ConfigFieldImpl.parse(state, "int field");
            assertEquals(cf.getTypeName(), "int");
            assertEquals(cf.getName(), "field", "Name");
        }
        {
            ConfigFieldImpl cf = ConfigFieldImpl.parse(state, "int_4 fie4_ld");
            assertEquals(cf.getTypeName(), "int_4");
            assertEquals(cf.getName(), "fie4_ld");
        }
        {
            ConfigFieldImpl cf = ConfigFieldImpl.parse(state, "int_8 fi_eld;comm_;ts,1,1");
            assertEquals(cf.getTypeName(), "int_8");
            assertEquals(cf.getName(), "fi_eld");
            assertEquals(cf.getComment(), "comm_", "Comment");
            assertEquals(cf.getTsInfo(), "ts,1,1");
        }
        {
            ConfigFieldImpl cf = ConfigFieldImpl.parse(state, "int[3 iterate] field");
            assertEquals(cf.getTypeName(), "int");
            assertEquals(cf.getArraySizes().length, 1);
            assertEquals(cf.getArraySizes()[0], 3);
            assertTrue(cf.isIterate(), "isIterate");
        }
        {
            ConfigFieldImpl cf = ConfigFieldImpl.parse(state, "int16_t crankingRpm;This,. value controls what RPM values we consider 'cranking' (any RPM below 'crankingRpm')\\nAnything above 'crankingRpm' would be 'running'");
            assertEquals(cf.getName(), "crankingRpm");
            assertEquals(cf.getArraySizes().length, 0);
            assertEquals(cf.getTypeName(), "int16_t");
        }
        {
            ConfigFieldImpl cf = ConfigFieldImpl.parse(state, "MAP_sensor_config_s map");
            assertEquals(cf.getName(), "map");
            assertEquals(cf.getArraySizes().length, 0);
            assertEquals(cf.getTypeName(), "MAP_sensor_config_s");
        }
        {
            ConfigFieldImpl cf = ConfigFieldImpl.parse(state, "MAP_sensor_config_s map;@see hasMapSensor\\n@see isMapAveragingEnabled");
            assertEquals(cf.getName(), "map");
            assertEquals(cf.getArraySizes().length, 0);
            assertEquals(cf.getTypeName(), "MAP_sensor_config_s");
            assertEquals(cf.getComment(), "@see hasMapSensor\\n@see isMapAveragingEnabled");
        }
    }

    @Test
    public void testParseSize() {
        ReaderStateImpl state = new ReaderStateImpl();
        assertEquals(4, state.parseSize("4", ""));

        assertEquals(12, state.parseSize("4*3", ""));

        state.getVariableRegistry().register("var", 256);

        assertEquals(512, state.parseSize("2*@@var@@", ""));
        assertEquals(512, state.parseSize("2x@@var@@", ""));
    }

    @Test
    public void testStructTooltips() {
        String test = "struct total\n" +
                "struct pid_s\n" +
                "floatms_t afr_type;PID dTime;\"ms\",      1,      0,       0, 3000,      0\n" +
                "percent_t afr_typet\n" +
"bit isForcedInduction;Does the vehicle have a turbo or supercharger?\n" +
                "bit enableFan1WithAc;Turn on this fan when AC is on.\n" +
                "end_struct\n" +
                "pid_s pid;comment\n" +
        "end_struct\n";
        ReaderStateImpl state = new ReaderStateImpl();

        TestTSProjectConsumer tsProjectConsumer = new TestTSProjectConsumer(state);
        state.readBufferedReader(test, tsProjectConsumer);
        assertEquals("pid_afr_type = scalar, F32, 0, \"ms\", 1, 0, 0, 3000, 0\n" +
                "pid_afr_typet = scalar, F32, 4, \"\", 1, 0, 0, 100, 0\n" +
                "pid_isForcedInduction = bits, U32, 8, [0:0], \"false\", \"true\"\n" +
                "pid_enableFan1WithAc = bits, U32, 8, [1:1], \"false\", \"true\"\n" +
                "; total TS size = 12\n", tsProjectConsumer.getContent());
        assertEquals(
                "\tpid_afr_type = \"PID dTime\"\n" +
                "\tpid_isForcedInduction = \"Does the vehicle have a turbo or supercharger?\"\n" +
                        "\tpid_enableFan1WithAc = \"Turn on this fan when AC is on.\"\n", tsProjectConsumer.getSettingContextHelpForUnitTest());
    }

    @Test
    public void testUnquote() {
        assertEquals("x", ConfigFieldImpl.unquote("\"x\""));
        // leave broken opening-only quote!
        assertEquals("\"x", ConfigFieldImpl.unquote("\"x"));
// this does not look great let's document this corner case for now
        assertEquals("x\"\n" +
                "\"y", ConfigFieldImpl.unquote("\"x\"\n\"y\""));
    }

    @Test
    public void testExpressionWithTernaryInMultiplier() {
        String test = "struct pid_s\n" +
                "\tuint8_t autoscale field;;\"units\", {useLambdaOnInterface ? 1/147 : 1/10}, 0, 0, 25, 1\n" +
                "end_struct\n";

        ReaderStateImpl state = new ReaderStateImpl();
        TSProjectConsumer tsProjectConsumer = new TestTSProjectConsumer(state);

        state.readBufferedReader(test, tsProjectConsumer);

        // When scale contains a ternary expression, it should be preserved as-is
        // and autoscale should default to 1:1
        assertEquals("field = scalar, U08, 0, \"units\", {useLambdaOnInterface ? 1/147 : 1/10}, 0, 0, 25, 1\n" +
                "; total TS size = 4\n", tsProjectConsumer.getContent());
    }

    @Test
    public void testExpressionWithTernaryInLimits() {
        String test = "struct pid_s\n" +
                "\tuint8_t[16 x 16] autoscale lambdaTable;;{useLambdaOnInterface ? \"lambda\" : \"afr\"}, {1/@@PACK_MULT_AFR_CFG@@}, 0, {useLambdaOnInterface ? 1.5 : 0}, {useLambdaOnInterface ? 1.5 : 25}, {useLambdaOnInterface ? 2 : 1}\n" +
                "end_struct\n";

        ReaderStateImpl state = new ReaderStateImpl();
        state.getVariableRegistry().register("PACK_MULT_AFR_CFG", 10);
        TSProjectConsumer tsProjectConsumer = new TestTSProjectConsumer(state);

        state.readBufferedReader(test, tsProjectConsumer);

        // Expressions in units, min, max, and digits should be preserved
        String expected = "lambdaTable = array, U08, 0, [16x16], {useLambdaOnInterface ? \"lambda\" : \"afr\"}, 0.1, 0, {useLambdaOnInterface ? 1.5 : 0}, {useLambdaOnInterface ? 1.5 : 25}, {useLambdaOnInterface ? 2 : 1}\n" +
                "; total TS size = 256\n";
        assertEquals(expected, tsProjectConsumer.getContent());
    }
}
