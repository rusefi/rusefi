package com.rusefi.core;

import java.util.HashMap;
import java.util.HashSet;
import java.util.Locale;
import java.util.Map;
import java.util.Set;

public class SensorsHolder {
    // Keys are normalized to lower-case (Locale.US) so that "TPSValue", "tpsvalue", "TPSVALUE"
    // all resolve to the same entry, with O(1) HashMap lookup instead of O(log N) TreeMap comparison.
    // Values stored as single-element double[] to avoid boxing on every update.
    // The array is allocated once per channel name and mutated in place thereafter.
    // todo: probably not worth migrating to AtomicDouble with longBitsToDouble
    private final Object lock = new Object();
    private final Map<String, double[]> values = new HashMap<>();

    public Set<String> getSensorNames() {
        synchronized (lock) {
            return new HashSet<>(values.keySet());
        }
    }

    public double getValue(Sensor sensor) {
        return getValue(sensor.getNativeName());
    }

    public double getValue(String sensorName) {
        synchronized (lock) {
            double[] cell = values.get(sensorName.toLowerCase(Locale.US));
            return cell == null ? Double.NaN : cell[0];
        }
    }

    public boolean setValue(double value, final Sensor sensor) {
        // "RPMGauge" => "RPMValue"
        return setValue(value, sensor.getNativeName());
    }

    public boolean setValue(double value, String sensorName) {
        String key = sensorName.toLowerCase(Locale.US);
        synchronized (lock) {
            double[] cell = values.get(key);
            if (cell == null) {
                values.put(key, new double[]{value});
                return true;
            }
            boolean isUpdated = cell[0] != value;
            cell[0] = value;
            return isUpdated;
        }
    }

    public void reset() {
        synchronized (lock) {
            values.clear();
        }
    }
}
