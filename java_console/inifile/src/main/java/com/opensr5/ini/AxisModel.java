package com.opensr5.ini;

public class AxisModel {
    private final double min;
    private final double max;
    private final int step;

    public AxisModel(double min, double max, int step) {
        this.min = min;
        this.max = max;
        this.step = step;
    }

    public double getMin() {
        return min;
    }

    public double getMax() {
        return max;
    }

    public int getStep() {
        return step;
    }

    @Override
    public String toString() {
        return "AxisModel{" +
                "min=" + min +
                ", max=" + max +
                ", step=" + step +
                '}';
    }
}
