package com.rusefi.core;

import java.util.function.Consumer;

/**
 * 11/16/2017
 * Andrey Belomutskiy, (c) 2013-2020
 */
public interface ISensorCentral extends ISensorHolder {
    class ListenerToken {
        private final Runnable removeAction;
        private final Consumer<Boolean> activeAction;
        private boolean removed;

        public ListenerToken(Runnable removeAction, Consumer<Boolean> activeAction) {
            this.removeAction = removeAction;
            this.activeAction = activeAction;
        }

        public synchronized void setActive(boolean active) {
            if (!removed) {
                activeAction.accept(active);
            }
        }

        public synchronized void remove() {
            if (!removed) {
                removed = true;
                removeAction.run();
            }
        }
    }

    SensorCentral.ListenerToken addListener(String sensorName, SensorCentral.SensorListener listener);

    void removeListener(String sensorName, SensorCentral.SensorListener listener);

    ValueSource getValueSource(Sensor sensor);

    ValueSource getValueSource(String sensorName);
}
