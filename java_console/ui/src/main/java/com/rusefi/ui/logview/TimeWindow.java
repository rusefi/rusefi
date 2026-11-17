package com.rusefi.ui.logview;

/** Visible part of a log's time axis; zoom and pan keep it inside the full log. */
final class TimeWindow {
    private final double fullStart;
    private final double fullEnd;
    private final double minimumSpan;
    private double start;
    private double end;

    TimeWindow(double fullStart, double fullEnd, double minimumSpan) {
        this.fullStart = fullStart;
        this.fullEnd = Math.max(fullStart, fullEnd);
        this.minimumSpan = Math.min(Math.max(minimumSpan, 1e-9), Math.max(this.fullEnd - fullStart, 1e-9));
        reset();
    }

    double start() {
        return start;
    }

    double end() {
        return end;
    }

    double span() {
        return Math.max(end - start, 1e-9);
    }

    double fullStart() {
        return fullStart;
    }

    double fullEnd() {
        return fullEnd;
    }

    boolean isFull() {
        return start <= fullStart && end >= fullEnd;
    }

    void reset() {
        start = fullStart;
        end = fullEnd;
    }

    /** Scales the span by {@code factor} (below 1 zooms in) while {@code anchor} stays at the same screen position. */
    void zoom(double anchor, double factor) {
        double span = span();
        double fraction = Math.min(1, Math.max(0, (anchor - start) / span));
        double newSpan = Math.min(fullEnd - fullStart, Math.max(minimumSpan, span * factor));
        place(anchor - fraction * newSpan, newSpan);
    }

    void pan(double delta) {
        place(start + delta, end - start);
    }

    /** Shows {@code [from, to]} in either order, widened to the minimum span if needed. */
    void show(double from, double to) {
        double low = Math.min(from, to);
        double high = Math.max(from, to);
        double span = Math.min(fullEnd - fullStart, Math.max(minimumSpan, high - low));
        place((low + high - span) / 2, span);
    }

    private void place(double newStart, double span) {
        newStart = Math.max(fullStart, Math.min(newStart, fullEnd - span));
        start = newStart;
        end = newStart + span;
    }
}
