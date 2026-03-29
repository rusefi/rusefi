package com.rusefi.core;

import com.opensr5.ConfigurationImage;
import com.opensr5.ini.IniFileModel;
import org.jetbrains.annotations.NotNull;
import org.jetbrains.annotations.Nullable;

import com.opensr5.ini.LowercaseHashMap;

import java.util.Collections;
import java.util.HashMap;
import java.util.List;
import java.util.Locale;
import java.util.Map;
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
    // Reused every grabSensorValues call to avoid allocating a fresh map each ECU frame.
    // LowercaseHashMap gives case-insensitive semantics with O(1) lookup vs TreeMap's O(log N).
    private final Map<String, Double> outputChannelCache = new LowercaseHashMap<>();

    // Keys normalized to lower-case (Locale.US) for O(1) HashMap lookup.
    // "coolant", "COOLANT", "Coolant" all resolve to the same listener list.
    private final Map<String, List<SensorListener>> sensorListeners = new HashMap<>();
    private final List<ResponseListener> listeners = new CopyOnWriteArrayList<>();
    private volatile Map<String, ResolvedGaugeLabels> resolvedGaugeLabels = Collections.emptyMap();
    private byte[] response;

    public static SensorCentral getInstance() {
        return INSTANCE;
    }

    private SensorCentral() {
    }

    @Override
    public Map<String, Double> getOutputChannelMap() {
        return outputChannelCache;
    }

    @Override
    public void grabSensorValues(byte[] response, @NotNull IniFileModel ini, @Nullable ConfigurationImage configImage) {
        this.response = response;
        ISensorCentral.super.grabSensorValues(response, ini, configImage);
        for (ResponseListener listener : listeners)
            listener.onSensorUpdate();
    }

    @Override
    public void onGaugeLabelsResolved(Map<String, ResolvedGaugeLabels> labels) {
        this.resolvedGaugeLabels = labels;
    }

    /**
     * Get the resolved gauge labels from the most recent update cycle.
     * @return map of gauge name to resolved title/units
     */
    public Map<String, ResolvedGaugeLabels> getResolvedGaugeLabels() {
        return resolvedGaugeLabels;
    }

    /**
     * Get resolved labels for a specific gauge.
     * @return resolved labels, or null if no string functions were resolved for this gauge
     */
    @Nullable
    public ResolvedGaugeLabels getResolvedLabels(String gaugeName) {
        return resolvedGaugeLabels.get(gaugeName);
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
    public boolean setValue(double value, String sensorName) {
        boolean isUpdated = sensorsHolder.setValue(value, sensorName);
        if (!isUpdated)
            return false;
        List<SensorListener> listeners;
        synchronized (sensorListeners) {
            listeners = sensorListeners.get(sensorName.toLowerCase(Locale.US));
        }

        if (listeners == null)
            return true;
        for (SensorListener listener : listeners)
            listener.onSensorUpdate(value);
        return true;
    }

    public void addListener(ResponseListener listener) {
        listeners.add(listener);
    }

    public void removeListener(ResponseListener listener) {
        listeners.remove(listener);
    }

    @Override
    public ListenerToken addListener(String sensorName, SensorListener listener) {
        String key = sensorName.toLowerCase(Locale.US);
        List<SensorListener> listeners;
        synchronized (sensorListeners) {
            listeners = sensorListeners.get(key);
            if (listeners == null)
                listeners = new CopyOnWriteArrayList<>();
            sensorListeners.put(key, listeners);
        }
        listeners.add(listener);

        return new SensorCentral.ListenerToken(this, sensorName, listener);
    }

    @Override
    public void removeListener(String sensorName, SensorListener listener) {
        List<SensorListener> listeners;
        synchronized (sensorListeners) {
            listeners = sensorListeners.get(sensorName.toLowerCase(Locale.US));
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
