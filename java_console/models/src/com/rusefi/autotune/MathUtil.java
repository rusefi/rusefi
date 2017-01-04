package com.rusefi.autotune;

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

    static double sumArray(float[][] array2) {
        double result = 0;
        for (float[] array1 : array2) {
            for (float element : array1) {
                result += element;
            }
        }
        return result;
    }
}
