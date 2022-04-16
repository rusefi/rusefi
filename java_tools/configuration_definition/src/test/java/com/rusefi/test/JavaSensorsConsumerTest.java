package com.rusefi.test;

import com.rusefi.ReaderState;
import com.rusefi.output.JavaSensorsConsumer;
import org.junit.Test;

import java.io.IOException;

import static org.junit.Assert.assertEquals;

public class JavaSensorsConsumerTest {
    @Test
    public void generateJavaSensors() throws IOException {
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
                "\tuint8_t autoscale vehicleSpeedKph;name2;\"kph\",1, 0, 0, 0, 0\n" +
                "\tint8_t autoscale internalMcuTemperature;mcu;\"deg C\",1, 0, 0, 0, 0\n" +
                "end_struct\n";
        JavaSensorsConsumer javaSensorsConsumer = new JavaSensorsConsumer(state, 0);
        state.readBufferedReader(outputChannels, javaSensorsConsumer);

        assertEquals("RPMValue(\"hello\", SensorCategory.SENSOR_INPUTS, FieldType.INT16, 4, 1.0, 0.0, 8000.0, \"RPM\"),\n" +
                        "rpmAcceleration(\"dRPM\", SensorCategory.SENSOR_INPUTS, FieldType.INT16, 6, 1.0, 0.0, 5.0, \"RPM/s\"),\n" +
                        "speedToRpmRatio(\"ra\", SensorCategory.SENSOR_INPUTS, FieldType.INT16, 8, 0.01, 0.0, 0.0, \"value\"),\n" +
                        "vehicleSpeedKph(\"name2\", SensorCategory.SENSOR_INPUTS, FieldType.INT8, 10, 1.0, 0.0, 0.0, \"kph\"),\n" +
                        "internalMcuTemperature(\"mcu\", SensorCategory.SENSOR_INPUTS, FieldType.INT8, 11, 1.0, 0.0, 0.0, \"deg C\"),\n",
                javaSensorsConsumer.getContent());


    }
}
