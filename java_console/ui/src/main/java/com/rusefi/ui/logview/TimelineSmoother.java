package com.rusefi.ui.logview;

/**
 * Produces display-only values for a timeline trace. The source arrays are never changed, so statistics,
 * cursor values and exported log data can continue to use the recorded samples.
 */
final class TimelineSmoother {
    private TimelineSmoother() {
    }

    /**
     * A centred moving average over {@code windowSeconds}. Each point averages samples no more than half the
     * window away on either side, so the display is not delayed like a trailing average. Invalid samples and
     * timestamps remain unchanged and split the trace into independent runs; smoothing never bridges a gap.
     */
    static double[] centeredMovingAverage(double[] times, double[] values, double windowSeconds) {
        if (times.length != values.length) {
            throw new IllegalArgumentException("Times and values must have the same length");
        }
        if (windowSeconds < 0 || !Double.isFinite(windowSeconds)) {
            throw new IllegalArgumentException("Smoothing window must be finite and non-negative");
        }

        double[] result = values.clone();
        if (windowSeconds == 0 || values.length == 0) {
            return result;
        }

        int runStart = 0;
        while (runStart < values.length) {
            if (!isValid(times, values, runStart)) {
                runStart++;
                continue;
            }

            int runEnd = runStart + 1;
            while (runEnd < values.length
                    && isValid(times, values, runEnd)
                    && times[runEnd] >= times[runEnd - 1]) {
                runEnd++;
            }
            smoothRun(times, values, result, runStart, runEnd, windowSeconds / 2);
            runStart = runEnd;
        }
        return result;
    }

    private static boolean isValid(double[] times, double[] values, int sample) {
        return Double.isFinite(times[sample]) && Double.isFinite(values[sample]);
    }

    private static void smoothRun(double[] times, double[] values, double[] result,
                                  int start, int end, double halfWindow) {
        int left = start;
        int right = start;
        double sum = 0;

        for (int center = start; center < end; center++) {
            double earliest = times[center] - halfWindow;
            double latest = times[center] + halfWindow;

            while (right < end && times[right] <= latest) {
                sum += values[right++];
            }
            while (left < right && times[left] < earliest) {
                sum -= values[left++];
            }
            result[center] = sum / (right - left);
        }
    }
}
