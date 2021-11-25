package com.rusefi.core;

/**
 * 11/16/2017
 * Andrey Belomutskiy, (c) 2013-2020
 */
public interface ISensorCentral extends ISensorHolder {
    void setAnySensorListener(SensorCentral.SensorListener2 anySensorListener);

    class ListenerToken {
        private final ISensorCentral sensorCentralInstance;
        private final Sensor sensor;
        private final SensorCentral.SensorListener listener;

        public ListenerToken(ISensorCentral instance, Sensor sensor, SensorCentral.SensorListener listener) {
            this.sensorCentralInstance = instance;
            this.sensor = sensor;
            this.listener = listener;
        }

        public void remove() {
            sensorCentralInstance.removeListener(sensor, listener);
        }
    }

    SensorCentral.ListenerToken addListener(Sensor sensor, SensorCentral.SensorListener listener);

    void removeListener(Sensor sensor, SensorCentral.SensorListener listener);

    ValueSource getValueSource(Sensor sensor);
}
