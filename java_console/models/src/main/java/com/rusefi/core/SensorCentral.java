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
 * Andrey Belomutskiy, (c) 2013-2020
 * @see SensorLog
 */
public class SensorCentral implements ISensorCentral {
    private static final SensorCentral INSTANCE = new SensorCentral();

    private final SensorsHolder sensorsHolder = new SensorsHolder();

    private final Map<Sensor, List<SensorListener>> allListeners = new EnumMap<>(Sensor.class);
    private byte[] response;

    public static SensorCentral getInstance() {
        return INSTANCE;
    }

    private SensorCentral() {
    }

    @Override
    public void grabSensorValues(byte[] response) {
        this.response = response;
        ISensorCentral.super.grabSensorValues(response);
    }

    public byte[] getResponse() {
        return response;
    }

    @Override
    public double getValue(Sensor sensor) {
        return sensorsHolder.getValue(sensor);
    }

    @Override
    public boolean setValue(double value, final Sensor sensor) {
        boolean isUpdated = sensorsHolder.setValue(value, sensor);
        List<SensorListener> listeners;
        synchronized (allListeners) {
            listeners = allListeners.get(sensor);
        }

        if (listeners == null)
            return isUpdated;
        for (SensorListener listener : listeners)
            listener.onSensorUpdate(value);
        return isUpdated;
    }

    @Override
    public ListenerToken addListener(Sensor sensor, SensorListener listener) {
        List<SensorListener> listeners;
        synchronized (allListeners) {
            listeners = allListeners.get(sensor);
            if (listeners == null)
                listeners = new CopyOnWriteArrayList<>();
            allListeners.put(sensor, listeners);
        }
        listeners.add(listener);

        return new SensorCentral.ListenerToken(this, sensor, listener);
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
}
