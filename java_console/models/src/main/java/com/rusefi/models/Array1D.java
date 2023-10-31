package com.rusefi.models;

import com.rusefi.config.Field;

/**
 * @author Andrey Belomutskiy
 *         7/14/2015
 */
public class Array1D {
    private final double[] values;

    public Array1D(double[] values) {
        this.values = values;
        for (int i = 1; i < values.length; i++) {
            double prev = values[i - 1];
            double current = values[i];
            if (prev >= current)
                throw new IllegalStateException("Out of order at index " + i);
        }

    }

    public static Array1D create(Field field, int offset, int size) {
        double result[] = new double[size];

        return new Array1D(result);
    }

    public int findIndex(double v) {
        for (int i = 0; i < values.length; i++) {
            if (v < values[i])
                return i - 1;
        }
        return values.length;
    }
}
