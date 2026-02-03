package com.rusefi.core;

import java.util.Map;
import java.util.TreeMap;

public class SensorsHolder implements ISensorHolder {
    // ini file uses "TPSValue" but Sensor enum uses "TPSVALUE"
    private final Map<String, Double> values = new TreeMap<>(String.CASE_INSENSITIVE_ORDER);

    public double getValue(Sensor sensor) {
        return getValue(sensor.getNativeName());
    }

    public double getValue(String sensorName) {
        Double value = values.get(sensorName);
        if (value == null)
            return Double.NaN;
        return value;
    }

    public boolean setValue(double value, final Sensor sensor) {
        // "RPMGauge" => "RPMValue"
        return setValue(value, sensor.getNativeName());
    }

    public boolean setValue(double value, String sensorName) {
        Double oldValue = values.get(sensorName);
        boolean isUpdated = oldValue == null || !oldValue.equals(value);
        values.put(sensorName, value);
        return isUpdated;
    }
}
