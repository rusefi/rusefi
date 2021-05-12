package com.rusefi.core;

public enum SensorCategory {
    OPERATIONS("Operations"),
    FUEL("Fuel-related"),
    SNIFFING("Sniffing"),
    SENSOR_INPUTS("Sensor inputs"),
    DEBUG("Debug"),
    STATUS("Status"),
    OTHERS("Others");

    private final String name;

    SensorCategory(String name) {
        this.name = name;
    }

    public String getName() {
        return name;
    }
}
