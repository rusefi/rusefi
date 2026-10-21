package com.rusefi.ui.logview;

import com.rusefi.logview.CsvLogReader;

import java.util.HashMap;
import java.util.Map;

/**
 * {@link TraceStats} for the visible time window, shared by the legend and the Stats panel. Each field is
 * computed once per window; zooming or panning starts over.
 */
final class VisibleStats {
    private final CsvLogReader log;
    private final TimeWindow window;
    private final Map<Integer, TraceStats> cache = new HashMap<>();
    private double start = Double.NaN;
    private double end = Double.NaN;

    VisibleStats(CsvLogReader log, TimeWindow window) {
        this.log = log;
        this.window = window;
    }

    TraceStats get(int field) {
        if (window.start() != start || window.end() != end) {
            cache.clear();
            start = window.start();
            end = window.end();
        }
        return cache.computeIfAbsent(field, f -> TraceStats.of(log, f, start, end));
    }
}
