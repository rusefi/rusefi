package com.rusefi.core;

import java.util.Map;
import java.util.TreeMap;

public class SensorsHolder {
    // ini file uses "TPSValue" but Sensor enum uses "TPSVALUE"
    // Values stored as single-element double[] to avoid boxing on every update.
    // The array is allocated once per channel name and mutated in place thereafter.
    private final Map<String, double[]> values = new TreeMap<>(String.CASE_INSENSITIVE_ORDER);

    public double getValue(Sensor sensor) {
        return getValue(sensor.getNativeName());
    }

    public double getValue(String sensorName) {
        double[] cell = values.get(sensorName);
        return cell == null ? Double.NaN : cell[0];
    }

    public boolean setValue(double value, final Sensor sensor) {
        // "RPMGauge" => "RPMValue"
        return setValue(value, sensor.getNativeName());
    }

    public boolean setValue(double value, String sensorName) {
        double[] cell = values.get(sensorName);
        if (cell == null) {
            values.put(sensorName, new double[]{value});
            return true;
        }
        boolean isUpdated = cell[0] != value;
        cell[0] = value;
        return isUpdated;
    }
}
