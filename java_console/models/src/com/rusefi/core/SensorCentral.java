package com.rusefi.core;

import com.romraider.logger.ecu.comms.query.ResponseImpl;
import com.romraider.logger.ecu.definition.EcuDataConvertor;
import com.romraider.logger.ecu.definition.EcuDataType;
import com.romraider.logger.ecu.definition.LoggerData;
import com.romraider.logger.ecu.ui.handler.table.TableUpdateHandler;

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
            return Double.NaN;
        return value;
    }

    EcuDataConvertor convertor = new EcuDataConvertor() {
        @Override
        public double convert(byte[] bytes) {
            return 0;
        }

        @Override
        public String format(double value) {
            return Double.toString(value);
        }

        @Override
        public String getUnits() {
            return null;
        }

        @Override
        public String getFormat() {
            return null;
        }

        @Override
        public String getExpression() {
            return null;
        }

        @Override
        public String getDataType() {
            return null;
        }
    };

    public void setValue(double value, final Sensor sensor) {
        values.put(sensor, value);
        List<SensorListener> listeners;
        synchronized (allListeners) {
            listeners = allListeners.get(sensor);
        }
        if (listeners == null)
            return;
        for (SensorListener listener : listeners)
            listener.onSensorUpdate(value);


        ResponseImpl r = new ResponseImpl();




        LoggerData d = new LoggerData() {
            @Override
            public String getId() {
                return sensor.getName();
            }

            @Override
            public String getName() {
                return sensor.getName();
            }

            @Override
            public String getDescription() {
                return sensor.getName();
            }

            @Override
            public EcuDataConvertor getSelectedConvertor() {
                return convertor;
            }

            @Override
            public EcuDataConvertor[] getConvertors() {
                throw new UnsupportedOperationException();
            }

            @Override
            public void selectConvertor(EcuDataConvertor convertor) {
                throw new UnsupportedOperationException();
            }

            @Override
            public EcuDataType getDataType() {
                throw new UnsupportedOperationException();
            }

            @Override
            public boolean isSelected() {
                throw new UnsupportedOperationException();
            }

            @Override
            public void setSelected(boolean selected) {
                throw new UnsupportedOperationException();
            }
        };

        r.setDataValue(d, value);
        TableUpdateHandler.getInstance().handleDataUpdate(r);
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
        addDoubleSensor(RPM_KEY, Sensor.RPM, es);
        addDoubleSensor("mat", Sensor.IAT, es);
        addDoubleSensor("map", Sensor.MAP, es);
        addDoubleSensor("map_r", Sensor.MAP_RAW, es);
        addDoubleSensor("baro", Sensor.BARO, es);
        addDoubleSensor("clt", Sensor.CLT, es);
        addDoubleSensor("tp", Sensor.TPS, es);

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
