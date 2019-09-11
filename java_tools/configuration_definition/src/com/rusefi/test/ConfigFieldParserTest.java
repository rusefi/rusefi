package com.rusefi.test;

import com.rusefi.ConfigField;
import com.rusefi.ReaderState;
import com.rusefi.VariableRegistry;
import com.rusefi.output.FsioSettingsConsumer;
import com.rusefi.output.JavaFieldsConsumer;
import org.junit.Test;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.Reader;
import java.io.StringReader;
import java.util.Arrays;

import static org.junit.Assert.*;

/**
 * (c) Andrey Belomutskiy
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
            Reader inputString = new StringReader(test);
            BufferedReader reader = new BufferedReader(inputString);

            JavaFieldsConsumer javaFieldsConsumer = new JavaFieldsConsumer(state) {
                @Override
                public void startFile() {
                }

                @Override
                public void endFile() {
                }
            };


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
        }
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
        assertEquals(4, ReaderState.parseSize("4", ""));

        assertEquals(12, ReaderState.parseSize("4*3", ""));

        VariableRegistry.INSTANCE.clear();

        VariableRegistry.INSTANCE.register("var", 256);

        assertEquals(512, ReaderState.parseSize("2*@@var@@", ""));
        assertEquals(512, ReaderState.parseSize("2x@@var@@", ""));
    }
}
