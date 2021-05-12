package com.rusefi.core;

import java.util.EnumMap;
import java.util.Map;

public class SensorsHolder implements ISensorHolder {
    private final Map<Sensor, Double> values = new EnumMap<>(Sensor.class);

    public double getValue(Sensor sensor) {
        Double value = values.get(sensor);
        if (value == null)
            return Double.NaN;
        return value;
    }

    public boolean setValue(double value, final Sensor sensor) {
        Double oldValue = values.get(sensor);
        boolean isUpdated = oldValue == null || !oldValue.equals(value);
        values.put(sensor, value);
        return isUpdated;
    }
}
