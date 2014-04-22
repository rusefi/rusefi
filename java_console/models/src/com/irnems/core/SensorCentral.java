package com.irnems.core;

import java.util.EnumMap;
import java.util.List;
import java.util.Map;
import java.util.concurrent.CopyOnWriteArrayList;

/**
 * Date: 1/6/13
 * (c) Andrey Belomutskiy
 */
public class SensorCentral {
    private static final SensorCentral INSTANCE = new SensorCentral();

    private final Map<Sensor, Double> values = new EnumMap<Sensor, Double>(Sensor.class);

    private final Map<Sensor, List<AdcListener>> allListeners = new EnumMap<Sensor, List<AdcListener>>(Sensor.class);


//    private final List<AdcListener> listeners = new CopyOnWriteArrayList<AdcListener>();

    public static SensorCentral getInstance() {
        return INSTANCE;
    }

    private SensorCentral() {
    }

//    public static String getTitle(int i) {
//        Sensor sensor = Sensor.findByMazdaIndex(i);
//        return "adc " + i + sensor.getName();
//    }

//    public String getSniffedAdcRepresentation(Sensor channel) {
//        double value = getValue(channel);
//
//        double volts = 5 * value / 255;
//        return "" + value + " (" + volts + ")";
//    }

    public double getValue(Sensor sensor) {
        Double value = values.get(sensor);
        if (value == null)
            throw new NullPointerException("No value for sensor: " + sensor);
        return value;
    }

    public void setValue(double value, Sensor sensor) {
        values.put(sensor, value);
        List<AdcListener> listeners;
        synchronized (allListeners) {
            listeners = allListeners.get(sensor);
        }
        if (listeners == null)
            return;
        for (AdcListener listener : listeners)
            listener.onAdcUpdate(this, value);
    }

    public static String getInternalAdcRepresentation(double value) {
        double volts = value * 3.3 / 4096;
        return String.format("%.2f (%.2fv)", value, volts);
    }

    public void addListener(Sensor sensor, AdcListener listener) {
        List<AdcListener> listeners;
        synchronized (allListeners) {
            listeners = allListeners.get(sensor);
            if (listeners == null)
                listeners = new CopyOnWriteArrayList<AdcListener>();
            allListeners.put(sensor, listeners);
        }
        listeners.add(listener);
    }

    public void removeListener(Sensor sensor, AdcListener listener) {
        List<AdcListener> listeners;
        synchronized (allListeners) {
            listeners = allListeners.get(sensor);
        }
        if (listeners != null)
            listeners.remove(listener);
    }

    public interface AdcListener {
        void onAdcUpdate(SensorCentral model, double value);
    }
}
