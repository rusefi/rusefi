package com.rusefi.core;

public enum SensorCategory {
    OPERATIONS("Operations"),
    FUEL("Fuel-related"),
    SNIFFING("Sniffing"),
    SENSOR_INPUTS("Sensor inputs"),
    OTHERS("Others");

    private final String name;

    private SensorCategory(String name) {
        this.name = name;
    }

    public String getName() {
        return name;
    }
}
