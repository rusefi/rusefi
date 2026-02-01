package com.rusefi.core;

/**
 * 11/16/2017
 * Andrey Belomutskiy, (c) 2013-2020
 */
public interface ISensorCentral extends ISensorHolder {
    class ListenerToken {
        private final ISensorCentral sensorCentralInstance;
        private final String sensorName;
        private final SensorCentral.SensorListener listener;

        public ListenerToken(ISensorCentral instance, String sensorName, SensorCentral.SensorListener listener) {
            this.sensorCentralInstance = instance;
            this.sensorName = sensorName;
            this.listener = listener;
        }

        public void remove() {
            sensorCentralInstance.removeListener(sensorName, listener);
        }
    }

    SensorCentral.ListenerToken addListener(Sensor sensor, SensorCentral.SensorListener listener);

    SensorCentral.ListenerToken addListener(String sensorName, SensorCentral.SensorListener listener);

    void removeListener(String sensorName, SensorCentral.SensorListener listener);

    ValueSource getValueSource(Sensor sensor);

    ValueSource getValueSource(String sensorName);
}
