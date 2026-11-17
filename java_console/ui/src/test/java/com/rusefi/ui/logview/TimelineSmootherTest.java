package com.rusefi.ui.logview;

import org.junit.jupiter.api.Test;

import static org.junit.jupiter.api.Assertions.assertArrayEquals;
import static org.junit.jupiter.api.Assertions.assertNotSame;
import static org.junit.jupiter.api.Assertions.assertThrows;

class TimelineSmootherTest {
    @Test
    void rawWindowReturnsAnIndependentCopy() {
        double[] values = {1, 2, 3};

        double[] result = TimelineSmoother.centeredMovingAverage(new double[]{0, 1, 2}, values, 0);

        assertArrayEquals(values, result);
        assertNotSame(values, result, "display smoothing must never expose the recorded values for mutation");
    }

    @Test
    void windowUsesTimeWithUnevenSamples() {
        double[] result = TimelineSmoother.centeredMovingAverage(
                new double[]{0, 0.01, 0.09, 0.10},
                new double[]{0, 10, 90, 100},
                0.10);

        assertArrayEquals(new double[]{5, 5, 95, 95}, result, 1e-12);
    }

    @Test
    void centeredWindowDoesNotLagTheTrace() {
        double[] result = TimelineSmoother.centeredMovingAverage(
                new double[]{0, 1, 2},
                new double[]{0, 9, 0},
                2);

        assertArrayEquals(new double[]{4.5, 3, 4.5}, result, 1e-12);
    }

    @Test
    void invalidSamplesSplitRunsInsteadOfBeingBridged() {
        double[] result = TimelineSmoother.centeredMovingAverage(
                new double[]{0, 0.04, 0.08, 0.12, 0.16},
                new double[]{0, 10, Double.NaN, 30, 40},
                1);

        assertArrayEquals(new double[]{5, 5, Double.NaN, 35, 35}, result, 1e-12);
    }

    @Test
    void rejectsInvalidInputs() {
        assertThrows(IllegalArgumentException.class,
                () -> TimelineSmoother.centeredMovingAverage(new double[]{0}, new double[]{1, 2}, 1));
        assertThrows(IllegalArgumentException.class,
                () -> TimelineSmoother.centeredMovingAverage(new double[]{0}, new double[]{1}, -1));
        assertThrows(IllegalArgumentException.class,
                () -> TimelineSmoother.centeredMovingAverage(new double[]{0}, new double[]{1}, Double.NaN));
    }
}
