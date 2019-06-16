package com.rusefi.core;

import java.util.EnumMap;
import java.util.List;
import java.util.Map;
import java.util.concurrent.CopyOnWriteArrayList;

/**
 * This class keeps track of {@link Sensor} current values and {@link SensorCentral.SensorListener}
 * <p/>
 * <p/>
 * Date: 1/6/13
 * (c) Andrey Belomutskiy
 */
public class SensorCentral implements ISensorCentral {
    public static final String RPM_KEY = "rpm";
    private static final SensorCentral INSTANCE = new SensorCentral();

    private final Map<Sensor, Double> values = new EnumMap<>(Sensor.class);

    private final Map<Sensor, List<SensorListener>> allListeners = new EnumMap<>(Sensor.class);
    private SensorListener2 anySensorListener;

    public static ISensorCentral getInstance() {
        return INSTANCE;
    }

    private SensorCentral() {
    }

    @Override
    public double getValue(Sensor sensor) {
        Double value = values.get(sensor);
        if (value == null)
            return Double.NaN;
        return value;
    }

    @Override
    public void setValue(double value, final Sensor sensor) {
        Double oldValue = values.get(sensor);
        boolean isUpdated = oldValue == null || !oldValue.equals(value);
        values.put(sensor, value);
        List<SensorListener> listeners;
        synchronized (allListeners) {
            listeners = allListeners.get(sensor);
        }

        // todo: make this just a normal listener?
        applyValueToTables(value, sensor, isUpdated);

        if (listeners == null)
            return;
        for (SensorListener listener : listeners)
            listener.onSensorUpdate(value);
    }

    private void applyValueToTables(double value, final Sensor sensor, boolean isUpdated) {
        if (isUpdated && anySensorListener != null)
            anySensorListener.onSensorUpdate(sensor, value);
    }

    @Override
    public void setAnySensorListener(SensorListener2 anySensorListener) {
        this.anySensorListener = anySensorListener;
    }

    @Override
    public void addListener(Sensor sensor, SensorListener listener) {
        List<SensorListener> listeners;
        synchronized (allListeners) {
            listeners = allListeners.get(sensor);
            if (listeners == null)
                listeners = new CopyOnWriteArrayList<>();
            allListeners.put(sensor, listeners);
        }
        listeners.add(listener);
    }

    @Override
    public void removeListener(Sensor sensor, SensorListener listener) {
        List<SensorListener> listeners;
        synchronized (allListeners) {
            listeners = allListeners.get(sensor);
        }
        if (listeners != null)
            listeners.remove(listener);
    }

    @Override
    public ValueSource getValueSource(Sensor sensor) {
        return () -> SensorCentral.this.getValue(sensor);
    }

    public interface SensorListener {
        void onSensorUpdate(double value);
    }

    public interface SensorListener2 {
        void onSensorUpdate(Sensor sensor, double value);
    }
}
