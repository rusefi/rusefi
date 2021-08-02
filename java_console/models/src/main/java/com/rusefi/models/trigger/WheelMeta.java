package com.rusefi.models.trigger;

public class WheelMeta {
    private final String name;
    private final boolean isFirstCrankBased;

    public WheelMeta(String name, boolean isFirstCrankBased) {
        this.name = name;
        this.isFirstCrankBased = isFirstCrankBased;
    }
}
