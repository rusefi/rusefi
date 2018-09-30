package com.rusefi;

public interface DataBuffer {
    double[] getValues();

    static double sum(double[] values) {
        double result = 0;
        for (double value : values)
            result += value;
        return result;
    }

    static double average(double[] values) {
        return sum(values) / values.length;
    }

    static double getStandardDeviation(double[] values) {
        double accumulator = 0;
        double avg = average(values);
        for (double value : values) {
            double v = value - avg;
            accumulator += v * v;
        }
        return Math.sqrt(accumulator);
    }
}
