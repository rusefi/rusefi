package com.rusefi.models;

/**
 * type-safe engine load value
 *
 * @author Andrey Belomutskiy
 *         7/14/2015
 */
public class EngineLoad {
    private final double value;

    public EngineLoad(double value) {
        this.value = value;
    }

    public double getValue() {
        return value;
    }
}
