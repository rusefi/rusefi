package com.rusefi.core;

import com.opensr5.ConfigurationImage;
import com.opensr5.ini.IniFileModel;
import org.jetbrains.annotations.NotNull;
import org.jetbrains.annotations.Nullable;

import java.util.List;
import java.util.Map;
import java.util.TreeMap;
import java.util.concurrent.CopyOnWriteArrayList;

/**
 * This class keeps track of {@link Sensor} current values and {@link SensorCentral.SensorListener}
 * <p/>
 * <p/>
 * Date: 1/6/13
 * Andrey Belomutskiy, (c) 2013-2020
 */
public class SensorCentral implements ISensorCentral {
    private static final SensorCentral INSTANCE = new SensorCentral();

    private final SensorsHolder sensorsHolder = new SensorsHolder();

    // ini uses "coolant" but Sensor enum uses "COOLANT"
    private final Map<String, List<SensorListener>> sensorListeners = new TreeMap<>(String.CASE_INSENSITIVE_ORDER);
    private final List<ResponseListener> listeners = new CopyOnWriteArrayList<>();
    private byte[] response;

    public static SensorCentral getInstance() {
        return INSTANCE;
    }

    private SensorCentral() {
    }

    @Override
    public void grabSensorValues(byte[] response, @NotNull IniFileModel ini, @Nullable ConfigurationImage configImage) {
        this.response = response;
        ISensorCentral.super.grabSensorValues(response, ini, configImage);
        for (ResponseListener listener : listeners)
            listener.onSensorUpdate();
    }

    public byte[] getResponse() {
        return response;
    }

    @Override
    public double getValue(Sensor sensor) {
        return getValue(sensor.getNativeName());
    }

    @Override
    public double getValue(String sensorName) {
        return sensorsHolder.getValue(sensorName);
    }

    @Override
    public boolean setValue(double value, final Sensor sensor) {
        return setValue(value, sensor.getNativeName());
    }

    @Override
    public boolean setValue(double value, String sensorName) {
        boolean isUpdated = sensorsHolder.setValue(value, sensorName);
        List<SensorListener> listeners;
        synchronized (sensorListeners) {
            listeners = sensorListeners.get(sensorName);
        }

        if (listeners == null)
            return isUpdated;
        for (SensorListener listener : listeners)
            listener.onSensorUpdate(value);
        return isUpdated;
    }

    public void addListener(ResponseListener listener) {
        listeners.add(listener);
    }

    @Override
    public ListenerToken addListener(Sensor sensor, SensorListener listener) {
        return addListener(sensor.getNativeName(), listener);
    }

    @Override
    public ListenerToken addListener(String sensorName, SensorListener listener) {
        List<SensorListener> listeners;
        synchronized (sensorListeners) {
            listeners = sensorListeners.get(sensorName);
            if (listeners == null)
                listeners = new CopyOnWriteArrayList<>();
            sensorListeners.put(sensorName, listeners);
        }
        listeners.add(listener);

        return new SensorCentral.ListenerToken(this, sensorName, listener);
    }

    @Override
    public void removeListener(String sensorName, SensorListener listener) {
        List<SensorListener> listeners;
        synchronized (sensorListeners) {
            listeners = sensorListeners.get(sensorName);
        }
        if (listeners != null)
            listeners.remove(listener);
    }

    @Override
    public ValueSource getValueSource(Sensor sensor) {
        return getValueSource(sensor.name());
    }

    @Override
    public ValueSource getValueSource(String sensorName) {
        return () -> SensorCentral.this.getValue(sensorName);
    }

    public interface SensorListener {
        void onSensorUpdate(double value);
    }

    public interface ResponseListener {
        void onSensorUpdate();
    }
}
