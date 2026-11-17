package com.rusefi.ui.logview;

import java.util.Arrays;

/**
 * The bins along one histogram axis, given by their centre values in increasing order like the axis of a
 * tune table. A value belongs to the nearest centre, so bins meet half way between centres; values beyond
 * the first or last centre go to the end bins, as a tune table clamps its lookups.
 */
final class HistogramAxis {
    static final int MIN_BINS = 2;
    static final int MAX_BINS = 64;

    private final double[] centers;

    HistogramAxis(double[] centers) {
        if (centers.length == 0) {
            throw new IllegalArgumentException("An axis needs at least one bin");
        }
        for (int i = 0; i < centers.length; i++) {
            if (!Double.isFinite(centers[i]) || i > 0 && !(centers[i] > centers[i - 1])) {
                throw new IllegalArgumentException("Bin centres must be finite and increasing: "
                        + Arrays.toString(centers));
            }
        }
        this.centers = centers.clone();
    }

    /**
     * {@code count} evenly spaced bins from {@code min} to {@code max}, the lowest and highest values the
     * axis signal takes; a single bin when they are equal.
     */
    static HistogramAxis even(double min, double max, int count) {
        if (!(max > min) || count < 2) {
            return new HistogramAxis(new double[]{Double.isFinite(min) ? min : 0});
        }
        double[] centers = new double[count];
        for (int i = 0; i < count; i++) {
            centers[i] = min + (max - min) * i / (count - 1);
        }
        return new HistogramAxis(centers);
    }

    int size() {
        return centers.length;
    }

    double center(int bin) {
        return centers[bin];
    }

    /** Where the bin starts, half way to the previous centre; the first bin reaches as far below its centre. */
    double lower(int bin) {
        if (centers.length == 1) {
            return centers[0];
        }
        return bin == 0 ? centers[0] - (centers[1] - centers[0]) / 2 : (centers[bin - 1] + centers[bin]) / 2;
    }

    /** Where the bin ends, half way to the next centre; the last bin reaches as far above its centre. */
    double upper(int bin) {
        int last = centers.length - 1;
        if (last == 0) {
            return centers[0];
        }
        return bin == last ? centers[last] + (centers[last] - centers[last - 1]) / 2
                : (centers[bin] + centers[bin + 1]) / 2;
    }

    /** The bin of a value: the nearest centre, the end bins beyond the ends; -1 for a missing value. */
    int index(double value) {
        if (Double.isNaN(value)) {
            return -1;
        }
        int after = Arrays.binarySearch(centers, value);
        if (after >= 0) {
            return after;
        }
        after = -after - 1;
        if (after == 0) {
            return 0;
        }
        if (after == centers.length) {
            return centers.length - 1;
        }
        return value - centers[after - 1] <= centers[after] - value ? after - 1 : after;
    }
}
