package com.rusefi.test;

import com.rusefi.ConfigField;
import com.rusefi.ReaderState;
import org.junit.Test;

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
            assertFalse("isIterate", cf.isIterate);
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
            assertEquals(cf.tsInfo, "ts");
        }
        {
            ConfigField cf = ConfigField.parse(state, "int[3 iterate] field");
            assertEquals(cf.getType(), "int");
            assertEquals(cf.getArraySize(), 3);
            assertTrue("isIterate", cf.isIterate);
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
}
