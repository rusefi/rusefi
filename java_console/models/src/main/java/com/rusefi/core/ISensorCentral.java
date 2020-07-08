package com.rusefi.core;

/**
 * 11/16/2017
 * Andrey Belomutskiy, (c) 2013-2020
 */
public interface ISensorCentral {
    double getValue(Sensor sensor);

    void setValue(double value, Sensor sensor);

    void setAnySensorListener(SensorCentral.SensorListener2 anySensorListener);

    void addListener(Sensor sensor, SensorCentral.SensorListener listener);

    void removeListener(Sensor sensor, SensorCentral.SensorListener listener);

    ValueSource getValueSource(Sensor sensor);
}
