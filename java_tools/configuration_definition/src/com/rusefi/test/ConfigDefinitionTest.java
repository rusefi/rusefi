package com.rusefi.test;

import com.rusefi.ConfigDefinition;
import com.rusefi.ConfigField;
import com.rusefi.ReaderState;
import org.junit.Test;

import static org.junit.Assert.*;

/**
 * (c) Andrey Belomutskiy
 * 1/15/15
 */
public class ConfigDefinitionTest {
    @Test
    public void testComment() {
        assertEquals("", ConfigDefinition.packComment("", "\t"));
        assertEquals("\t * abc\n", ConfigDefinition.packComment("abc", "\t"));
        assertEquals("\t * abc\n" +
                "\t * vbn\n", ConfigDefinition.packComment("abc\\nvbn", "\t"));
    }

    @Test
    public void testByteArray() {
        ReaderState state = new ReaderState();
        {
            ConfigField cf = ConfigField.parse(state, "uint8_t[8] field");
            assertEquals(cf.type, "uint8_t");
            assertEquals(cf.arraySize, 8);
            assertEquals(cf.getSize(null), 8);
            assertFalse("isIterate", cf.isIterate);
        }

    }

    @Test
    public void testParseLine() {
        ReaderState state = new ReaderState();
        assertNull(ConfigField.parse(state, "int"));
        {
            ConfigField cf = ConfigField.parse(state, "int field");
            assertEquals(cf.type, "int");
            assertEquals("Name", cf.name, "field");
        }
        {
            ConfigField cf = ConfigField.parse(state, "int_4 fie4_ld");
            assertEquals(cf.type, "int_4");
            assertEquals(cf.name, "fie4_ld");
        }
        {
            ConfigField cf = ConfigField.parse(state, "int_8 fi_eld;comm_;ts");
            assertEquals(cf.type, "int_8");
            assertEquals(cf.name, "fi_eld");
            assertEquals("Comment", cf.comment, "comm_");
            assertEquals(cf.tsInfo, "ts");
        }
        {
            ConfigField cf = ConfigField.parse(state, "int[3 iterate] field");
            assertEquals(cf.type, "int");
            assertEquals(cf.arraySize, 3);
            assertTrue("isIterate", cf.isIterate);
        }
        {
            ConfigField cf = ConfigField.parse(state, "int16_t crankingRpm;This,. value controls what RPM values we consider 'cranking' (any RPM below 'crankingRpm')\\nAnything above 'crankingRpm' would be 'running'");
            assertEquals(cf.name, "crankingRpm");
            assertEquals(cf.arraySize, 1);
            assertEquals(cf.type, "int16_t");
        }
        {
            ConfigField cf = ConfigField.parse(state, "MAP_sensor_config_s map");
            assertEquals(cf.name, "map");
            assertEquals(cf.arraySize, 1);
            assertEquals(cf.type, "MAP_sensor_config_s");
        }
        {
            ConfigField cf = ConfigField.parse(state, "MAP_sensor_config_s map;@see hasMapSensor\\n@see isMapAveragingEnabled");
            assertEquals(cf.name, "map");
            assertEquals(cf.arraySize, 1);
            assertEquals(cf.type, "MAP_sensor_config_s");
            assertEquals(cf.comment, "@see hasMapSensor\\n@see isMapAveragingEnabled");
        }
    }
}
