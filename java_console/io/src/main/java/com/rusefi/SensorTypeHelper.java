package com.rusefi;

import com.rusefi.enums.SensorType;

public class SensorTypeHelper {
    public static SensorType valueOfAnyCase(String name) {
        for (SensorType type : SensorType.values()) {
            if (type.name().equalsIgnoreCase(name))
                return type;
        }
        throw new IllegalArgumentException("SensorType not found " + name);
    }
}
