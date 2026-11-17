package com.rusefi.ui.logview;

import com.rusefi.logview.CsvLogReader;

/** Millisecond input limits derived from a log's typical sample interval and duration. */
final class TimelineSmoothingWindow {
    final int minimumMs;
    final int maximumMs;

    TimelineSmoothingWindow(int minimumMs, int maximumMs) {
        this.minimumMs = Math.max(1, minimumMs);
        this.maximumMs = Math.max(this.minimumMs, maximumMs);
    }

    static TimelineSmoothingWindow of(CsvLogReader log) {
        double typicalSeconds = Histogram.typicalInterval(log);
        int minimumMs = finiteMilliseconds(typicalSeconds, 1);
        double durationSeconds = log.maximum(0) - log.minimum(0);
        int maximumMs = finiteMilliseconds(durationSeconds, minimumMs);
        return new TimelineSmoothingWindow(minimumMs, Math.max(minimumMs, maximumMs));
    }

    /** Used by isolated toolbar tests that have no log. */
    static TimelineSmoothingWindow unrestricted() {
        return new TimelineSmoothingWindow(1, Integer.MAX_VALUE);
    }

    int initialMilliseconds(double seconds) {
        if (!(seconds > 0)) {
            return Math.min(maximumMs, Math.max(minimumMs, 100));
        }
        long milliseconds = Math.round(seconds * 1000);
        return (int) Math.max(minimumMs, Math.min(maximumMs, milliseconds));
    }

    static String label(double seconds) {
        return seconds == 0 ? "Raw" : Math.round(seconds * 1000) + " ms";
    }

    private static int finiteMilliseconds(double seconds, int fallback) {
        if (!(seconds > 0) || !Double.isFinite(seconds)) {
            return fallback;
        }
        return (int) Math.min(Integer.MAX_VALUE, Math.ceil(seconds * 1000));
    }
}
