package com.rusefi.core;

import java.util.HashMap;
import java.util.Map;

public class SensorsHolder implements ISensorHolder {
    private final Map<String, Double> values = new HashMap<>();

    public double getValue(Sensor sensor) {
        return getValue(sensor.name());
    }

    public double getValue(String sensorName) {
        Double value = values.get(sensorName);
        if (value == null)
            return Double.NaN;
        return value;
    }

    public boolean setValue(double value, final Sensor sensor) {
        return setValue(value, sensor.name());
    }

    public boolean setValue(double value, String sensorName) {
        Double oldValue = values.get(sensorName);
        boolean isUpdated = oldValue == null || !oldValue.equals(value);
        values.put(sensorName, value);
        return isUpdated;
    }
}
