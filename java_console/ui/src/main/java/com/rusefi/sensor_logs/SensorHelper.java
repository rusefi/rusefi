package com.rusefi.sensor_logs;

import com.rusefi.core.Sensor;

import java.util.ArrayList;
import java.util.Collection;
import java.util.List;

public class SensorHelper {

    static int getSensorValueSize(Sensor sensor) {
        switch (sensor.getType()) {
            case UINT8:
                return 0;
            case INT8:
                return 1;
            case UINT16:
                return 2;
            case INT16:
                return 3;
            case INT:
                return 4;
            case FLOAT:
                return 7;
            default:
                throw new UnsupportedOperationException("" + sensor.getType());
        }
    }

    public static BinaryLogEntry valueOf(Sensor sensor) {
        String name = SensorLogger.getSensorName(sensor, 0);
        String unit = sensor.getUnits();

        return new BinaryLogEntry(name, unit, getSensorValueSize(sensor));
    }

    public static List<BinaryLogEntry> values(Collection<Sensor> sensors) {
        List<BinaryLogEntry> result = new ArrayList<>();
        for (Sensor sensor : sensors) {
            result.add(valueOf(sensor));
        }
        return result;
    }
}
