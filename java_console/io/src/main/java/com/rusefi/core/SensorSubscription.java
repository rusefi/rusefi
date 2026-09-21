package com.rusefi.core;

import java.util.Collections;
import java.util.HashSet;
import java.util.Locale;
import java.util.Set;

/**
 * Tracks what fields exactly a listener is interested in.
 */
public class SensorSubscription {
    private final Set<String> sensorNames = new HashSet<>();

    public SensorSubscription(String... sensorNames) {
        for (String name : sensorNames) {
            this.sensorNames.add(name.toLowerCase(Locale.US));
        }
    }

    public Set<String> getSensorNames() {
        return Collections.unmodifiableSet(sensorNames);
    }

    public boolean isInterestedIn(String sensorName) {
        return sensorNames.isEmpty() || sensorNames.contains(sensorName.toLowerCase(Locale.US));
    }

    public boolean isInterestedInAny(Set<String> updatedSensors) {
        if (sensorNames.isEmpty()) {
            return true;
        }
        for (String updated : updatedSensors) {
            if (sensorNames.contains(updated.toLowerCase(Locale.US))) {
                return true;
            }
        }
        return false;
    }
}
