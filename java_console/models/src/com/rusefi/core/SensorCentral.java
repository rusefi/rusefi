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

    @Override
    public void initialize(EngineState es) {
        addDoubleSensor(RPM_KEY, Sensor.RPM, es);
        addDoubleSensor("mat", Sensor.IAT, es);
        addDoubleSensor("map", Sensor.MAP, es);
        addDoubleSensor("map_r", Sensor.MAP_RAW, es);
        addDoubleSensor("baro", Sensor.BARO, es);
        addDoubleSensor("clt", Sensor.CLT, es);
        addDoubleSensor("tp", Sensor.TPS, es);
        addDoubleSensor("pps", Sensor.TPS, es);
        addDoubleSensor("error", Sensor.lastErrorCode, es);
        addDoubleSensor("warn", Sensor.errorCodeCounter, es);
        addDoubleSensor("knck_c", Sensor.knockCount, es);
        addDoubleSensor("knck_v", Sensor.KnockValue, es);
        addDoubleSensor("target", Sensor.TARGET_AFR, es);

        addDoubleSensor(Sensor.VSS, es);
        addDoubleSensor(Sensor.ENGINE_LOAD, es);
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
        addDoubleSensor(Sensor.knockCount, es);
        addDoubleSensor(Sensor.KnockValue, es);

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
        addDoubleSensor(Sensor.MAFR, es);
        addDoubleSensor("period0", Sensor.PERIOD0, es);
        addDoubleSensor("period1", Sensor.PERIOD0, es);
        addDoubleSensor("duty0", Sensor.DUTY0, es);
        addDoubleSensor("duty1", Sensor.DUTY1, es);
        addDoubleSensor(Sensor.TIMING, es);
        addDoubleSensor(Sensor.TRG_0_DUTY, es);
        addDoubleSensor(Sensor.TRG_1_DUTY, es);

        addDoubleSensor(Sensor.ADC0, es);
        addDoubleSensor(Sensor.ADC1, es);
        addDoubleSensor(Sensor.ADC2, es);
        addDoubleSensor(Sensor.ADC3, es);

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

    public interface SensorListener2 {
        void onSensorUpdate(Sensor sensor, double value);
    }
}
