package com.rusefi.models;

/**
 * Date: 3/24/13
 * Andrey Belomutskiy, (c) 2013-2020
 */
public class Range {
    private final float min;
    private final float max;

    public Range(double min, double max) {
        this((float) min, (float) max);
    }

    public Range(float min, float max) {
        this.min = min;
        this.max = max;
    }

    public float getMin() {
        return min;
    }

    public float getMax() {
        return max;
    }

    public float getWidth() {
        return max - min;
    }
}
