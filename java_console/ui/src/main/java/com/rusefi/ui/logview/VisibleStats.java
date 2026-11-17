package com.rusefi.ui.logview;

import com.rusefi.logview.CsvLogReader;

import java.util.HashMap;
import java.util.Map;

/**
 * {@link TraceStats} for the visible time window, or between the A and B cursors while both are set; shared by
 * the legend, markers, card and the Stats panel. Each field is computed once per range; zooming, panning or
 * moving a cursor starts over.
 */
final class VisibleStats {
    private final CsvLogReader log;
    private final TimeWindow window;
    private final MeasureCursors cursors;
    private final Map<Integer, TraceStats> cache = new HashMap<>();
    private double start = Double.NaN;
    private double end = Double.NaN;

    VisibleStats(CsvLogReader log, TimeWindow window) {
        this(log, window, new MeasureCursors());
    }

    VisibleStats(CsvLogReader log, TimeWindow window, MeasureCursors cursors) {
        this.log = log;
        this.window = window;
        this.cursors = cursors;
    }

    /** True while the statistics are between the A and B cursors rather than over the visible time. */
    boolean isBetweenCursors() {
        return cursors.isRange();
    }

    double rangeStart() {
        return cursors.isRange() ? cursors.start() : window.start();
    }

    double rangeEnd() {
        return cursors.isRange() ? cursors.end() : window.end();
    }

    TraceStats get(int field) {
        if (rangeStart() != start || rangeEnd() != end) {
            cache.clear();
            start = rangeStart();
            end = rangeEnd();
        }
        return cache.computeIfAbsent(field, f -> TraceStats.of(log, f, start, end));
    }
}
