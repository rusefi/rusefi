package com.rusefi.test;

import com.rusefi.ReaderState;
import com.rusefi.output.JavaSensorsConsumer;
import org.junit.Test;

import java.io.IOException;

import static org.junit.Assert.assertEquals;

public class JavaSensorsConsumerTest {
    @Test
    public void generateJavaSensors() {
        ReaderState state = new ReaderState();
        state.variableRegistry.register("PACK_MULT_PERCENT", 100);
        state.variableRegistry.register("GAUGE_NAME_RPM", "\"hello\"");
        state.variableRegistry.register("GAUGE_NAME_GEAR_RATIO", "ra");

        String outputChannels = "" +
                "\n" +
                "" +
                "struct_no_prefix output_channels_s\n" +
                "bit sd_present\n" +
                "uint16_t autoscale RPMValue;@@GAUGE_NAME_RPM@@;\"RPM\",1, 0, 0, 8000, 0\n" +
                "uint16_t rpmAcceleration;dRPM;\"RPM/s\",1, 0, 0, 5, 0\n" +
                "\tuint16_t autoscale speedToRpmRatio;@@GAUGE_NAME_GEAR_RATIO@@;\"value\",{1/@@PACK_MULT_PERCENT@@}, 0, 0, 0, 0\n" +
                "    float luaTimingMult;\n" +
                "\tuint8_t vehicleSpeedKph\n" +
                "\tint8_t autoscale internalMcuTemperature;mcu;\"deg C\",1, 0, 0, 0, 0\n" +
                "end_struct\n";
        JavaSensorsConsumer javaSensorsConsumer = new JavaSensorsConsumer();
        state.readBufferedReader(outputChannels, javaSensorsConsumer);

        assertEquals("RPMValue(\"hello\", SensorCategory.SENSOR_INPUTS, FieldType.INT16, 4, 1.0, 0.0, 8000.0, \"RPM\"),\n" +
                        "rpmAcceleration(\"dRPM\", SensorCategory.SENSOR_INPUTS, FieldType.INT16, 6, 1.0, 0.0, 5.0, \"RPM/s\"),\n" +
                        "speedToRpmRatio(\"ra\", SensorCategory.SENSOR_INPUTS, FieldType.INT16, 8, 0.01, 0.0, 0.0, \"value\"),\n" +
                        "alignmentFill_at_10(\"need 4 byte alignment\", SensorCategory.SENSOR_INPUTS, FieldType.INT8, 10, 1.0, -20.0, 100.0, \"units\"),\n" +
                        "luaTimingMult(\"luaTimingMult\", SensorCategory.SENSOR_INPUTS, FieldType.INT, 12, 1.0, -1.0, -1.0, \"\"),\n" +
                        "vehicleSpeedKph(\"vehicleSpeedKph\", SensorCategory.SENSOR_INPUTS, FieldType.INT8, 16, 1.0, -1.0, -1.0, \"\"),\n" +
                        "internalMcuTemperature(\"mcu\", SensorCategory.SENSOR_INPUTS, FieldType.INT8, 17, 1.0, 0.0, 0.0, \"deg C\"),\n" +
                        "alignmentFill_at_18(\"need 4 byte alignment\", SensorCategory.SENSOR_INPUTS, FieldType.INT8, 18, 1.0, -20.0, 100.0, \"units\"),\n",
                javaSensorsConsumer.getContent());
        assertEquals(20, javaSensorsConsumer.sensorTsPosition);
    }

    @Test
    public void bitAtTheEndBug() {
        ReaderState state = new ReaderState();
        String outputChannels =
                "struct_no_prefix output_channels_s\n" +
                        "bit sd_present\n" +
                        "end_struct\n";
        JavaSensorsConsumer javaSensorsConsumer = new JavaSensorsConsumer();
        state.readBufferedReader(outputChannels, javaSensorsConsumer);

        assertEquals(4, javaSensorsConsumer.sensorTsPosition);
    }
}
