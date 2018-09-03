package com.rusefi.autotune;

import java.util.Arrays;

/**
 * (c) Andrey Belomutskiy 2013-2017
 * 2/18/2016.
 */
public class MathUtil {
    private MathUtil() {
    }

    static float[][] deepCopy(float[][] input) {
        if (input == null)
            return null;
        float[][] result = new float[input.length][];
        for (int r = 0; r < input.length; r++) {
            result[r] = input[r].clone();
        }
        return result;
    }

    static double sumArray(double[][] array2D) {
        double result = 0;
        for (double[] array : array2D) {
            for (double element : array)
                result += element;
        }
        return result;
    }

    static void setArray2D(double[][] array, double value) {
        for (double[] a : array)
            Arrays.setAll(a, i -> value);
    }

    static double square(double value) {
        return value * value;
    }
}
