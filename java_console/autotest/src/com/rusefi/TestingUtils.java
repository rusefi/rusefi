package com.rusefi;

import java.util.Arrays;

import static com.rusefi.waves.WaveReport.isCloseEnough;

/**
 * @author Andrey Belomutskiy
 *         3/19/14.
 */
public class TestingUtils {
    static void assertTrue(String msg, boolean b) {
        if (!b)
            throw new IllegalStateException("Not true: " + msg);
    }

    static void assertCloseEnough(String msg, double current, double... expectations) {
        for (double expected : expectations) {
            if (isCloseEnough(expected, current))
                return;
        }
        throw new IllegalStateException(msg + ": Got " + current + " while expecting " + Arrays.toString(expectations));
    }

    static void assertTrue(boolean b) {
        if (!b)
            throw new IllegalStateException("Not true");
    }
}
