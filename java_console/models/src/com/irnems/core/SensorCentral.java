package com.irnems.core;

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
public class SensorCentral {
    public static final String RPM_KEY = "rpm";
    private static final SensorCentral INSTANCE = new SensorCentral();

    private final Map<Sensor, Double> values = new EnumMap<Sensor, Double>(Sensor.class);

    private final Map<Sensor, List<SensorListener>> allListeners = new EnumMap<Sensor, List<SensorListener>>(Sensor.class);

    public static SensorCentral getInstance() {
        return INSTANCE;
    }

    private SensorCentral() {
    }

    public double getValue(Sensor sensor) {
        Double value = values.get(sensor);
        if (value == null)
            throw new NullPointerException("No value for sensor: " + sensor);
        return value;
    }

    public void setValue(double value, Sensor sensor) {
        values.put(sensor, value);
        List<SensorListener> listeners;
        synchronized (allListeners) {
            listeners = allListeners.get(sensor);
        }
        if (listeners == null)
            return;
        for (SensorListener listener : listeners)
            listener.onSensorUpdate(value);
    }

    public static String getInternalAdcRepresentation(double value) {
        double volts = value * 3.3 / 4096;
        return String.format("%.2f (%.2fv)", value, volts);
    }

    public void addListener(Sensor sensor, SensorListener listener) {
        List<SensorListener> listeners;
        synchronized (allListeners) {
            listeners = allListeners.get(sensor);
            if (listeners == null)
                listeners = new CopyOnWriteArrayList<SensorListener>();
            allListeners.put(sensor, listeners);
        }
        listeners.add(listener);
    }

    public void removeListener(Sensor sensor, SensorListener listener) {
        List<SensorListener> listeners;
        synchronized (allListeners) {
            listeners = allListeners.get(sensor);
        }
        if (listeners != null)
            listeners.remove(listener);
    }

    public void initialize(EngineState es) {
        addDoubleSensor("injector0", Sensor.INJECTOR_0_STATUS, true, es);
        addDoubleSensor("injector1", Sensor.INJECTOR_1_STATUS, true, es);
        addDoubleSensor("injector2", Sensor.INJECTOR_2_STATUS, true, es);
        addDoubleSensor("injector3", Sensor.INJECTOR_3_STATUS, true, es);

        addDoubleSensor(RPM_KEY, Sensor.RPM, es);
        addDoubleSensor("mat", Sensor.IAT, es);
        addDoubleSensor("map", Sensor.MAP, es);
        addDoubleSensor("map_r", Sensor.MAP_RAW, es);
        addDoubleSensor("baro", Sensor.BARO, es);
        addDoubleSensor("clt", Sensor.CLT, es);
        addDoubleSensor("tp", Sensor.TPS, es);

        addDoubleSensor(Sensor.DWELL0, es);
        addDoubleSensor(Sensor.DWELL1, es);
        addDoubleSensor(Sensor.DWELL2, es);
        addDoubleSensor(Sensor.DWELL3, es);
        addDoubleSensor(Sensor.TOTAL_DWELL0, es);
        addDoubleSensor(Sensor.TOTAL_DWELL1, es);
        addDoubleSensor(Sensor.TOTAL_DWELL2, es);
        addDoubleSensor(Sensor.TOTAL_DWELL3, es);
        addDoubleSensor(Sensor.ADVANCE0, es);
        addDoubleSensor(Sensor.ADVANCE1, es);
        addDoubleSensor(Sensor.ADVANCE2, es);
        addDoubleSensor(Sensor.ADVANCE3, es);

        addDoubleSensor("tch", Sensor.T_CHARGE, es);
        addDoubleSensor(Sensor.AFR, es);
        addDoubleSensor("d_fuel", Sensor.DEFAULT_FUEL, es);
        addDoubleSensor(Sensor.FUEL, es);
        addDoubleSensor(Sensor.FUEL_BASE, es);
        addDoubleSensor(Sensor.FUEL_LAG, es);
        addDoubleSensor(Sensor.FUEL_CLT, es);
        addDoubleSensor(Sensor.FUEL_IAT, es);
        addDoubleSensor(Sensor.TABLE_SPARK, es);
        addDoubleSensor(Sensor.VREF, es);
        addDoubleSensor(Sensor.VBATT, es);
        addDoubleSensor(Sensor.MAF, es);
        addDoubleSensor("period0", Sensor.PERIOD0, es);
        addDoubleSensor("period1", Sensor.PERIOD0, es);
        addDoubleSensor("duty0", Sensor.DUTY0, es);
        addDoubleSensor("duty1", Sensor.DUTY1, es);
        addDoubleSensor("timing", Sensor.TIMING, es);
        addDoubleSensor(Sensor.TRG_0_DUTY, es);
        addDoubleSensor(Sensor.TRG_1_DUTY, es);

        addDoubleSensor("idl", Sensor.IDLE_SWITCH, es);

        addDoubleSensor("chart", Sensor.CHART_STATUS, true, es);
        addDoubleSensor("chartsize", Sensor.CHARTSIZE, true, es);
        addDoubleSensor("adcDebug", Sensor.ADC_STATUS, true, es);
        addDoubleSensor("adcfast", Sensor.ADC_FAST, es);
        addDoubleSensor("adcfastavg", Sensor.ADC_FAST_AVG, es);
    }

    /**
     * Sensor enum name matches the protocol key
     */
    private void addDoubleSensor(Sensor sensor, EngineState es) {
        addDoubleSensor(sensor.name(), sensor, es);
    }

    private void addDoubleSensor(String key, final Sensor sensor, EngineState es) {
        addDoubleSensor(key, sensor, false, es);
    }

    private void addDoubleSensor(final String key, final Sensor sensor, final boolean verbose, EngineState es) {
        es.registerStringValueAction(key, new EngineState.ValueCallback<String>() {
            @Override
            public void onUpdate(String stringValue) {
                double value;
                try {
                    value = Double.parseDouble(stringValue);
                } catch (NumberFormatException e) {
                    // serial protocol is not safe
                    return;
                }
                setValue(value, sensor);
                if (verbose)
                    MessagesCentral.getInstance().postMessage(EngineState.class, key + "=" + value);
            }
        });
    }

    public interface SensorListener {
        void onSensorUpdate(double value);
    }
}
