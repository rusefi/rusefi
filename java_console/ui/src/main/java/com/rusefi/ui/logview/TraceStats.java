package com.rusefi.ui.logview;

import com.rusefi.logview.CsvLogReader;

/** Minimum, maximum and average of one field over a time range, and where the extremes occur. */
final class TraceStats {
    static final TraceStats EMPTY = new TraceStats(Double.NaN, Double.NaN, Double.NaN, -1, -1, 0);

    final double min;
    final double max;
    final double average;
    /** First sample holding the minimum, or -1 when the range has no values. */
    final int minSample;
    /** First sample holding the maximum, or -1 when the range has no values. */
    final int maxSample;
    /** Samples with a finite value in the range. */
    final int count;

    private TraceStats(double min, double max, double average, int minSample, int maxSample, int count) {
        this.min = min;
        this.max = max;
        this.average = average;
        this.minSample = minSample;
        this.maxSample = maxSample;
        this.count = count;
    }

    boolean isEmpty() {
        return count == 0;
    }

    /** Statistics over the samples with time in [{@code start}, {@code end}]; missing values are skipped. */
    static TraceStats of(CsvLogReader log, int field, double start, double end) {
        int first = log.firstSampleAtOrAfter(start);
        int last = log.firstSampleAtOrAfter(Math.nextUp(end));
        double min = Double.POSITIVE_INFINITY;
        double max = Double.NEGATIVE_INFINITY;
        double sum = 0;
        int minSample = -1;
        int maxSample = -1;
        int count = 0;
        for (int sample = first; sample < last; sample++) {
            double value = log.value(sample, field);
            if (!Double.isFinite(value)) {
                continue;
            }
            if (value < min) {
                min = value;
                minSample = sample;
            }
            if (value > max) {
                max = value;
                maxSample = sample;
            }
            sum += value;
            count++;
        }
        return count == 0 ? EMPTY : new TraceStats(min, max, sum / count, minSample, maxSample, count);
    }
}
