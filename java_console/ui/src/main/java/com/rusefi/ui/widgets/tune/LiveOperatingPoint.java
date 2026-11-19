package com.rusefi.ui.widgets.tune;

/**
 * Where a live output-channel value falls on a table axis, as a fractional bin index: 2.25 is a
 * quarter of the way from bin 2 to bin 3. Values beyond the axis clamp to the nearer end bin, as
 * the firmware's table interpolation does. Bins may run in either direction, because the table
 * view can show an inverted axis.
 */
final class LiveOperatingPoint {
    private LiveOperatingPoint() {
    }

    /**
     * @param bins  axis bins in displayed order
     * @param count number of data cells along this axis; bins beyond it are ignored
     * @return the fractional index, or null when there is no usable value or axis
     */
    static Double axisPosition(Double[] bins, int count, double value) {
        if (bins == null || Double.isNaN(value) || Double.isInfinite(value)) {
            return null;
        }
        int size = Math.min(bins.length, count);
        if (size <= 0) {
            return null;
        }
        for (int i = 0; i < size; i++) {
            if (bins[i] == null || Double.isNaN(bins[i])) {
                return null;
            }
        }
        for (int i = 0; i < size - 1; i++) {
            double from = bins[i];
            double to = bins[i + 1];
            if (value >= Math.min(from, to) && value <= Math.max(from, to)) {
                return from == to ? (double) i : i + (value - from) / (to - from);
            }
        }
        return Math.abs(value - bins[0]) <= Math.abs(value - bins[size - 1]) ? 0.0 : (double) (size - 1);
    }
}
