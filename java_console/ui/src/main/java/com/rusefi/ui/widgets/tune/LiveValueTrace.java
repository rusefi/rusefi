package com.rusefi.ui.widgets.tune;

import com.rusefi.ui.widgets.CircularGauge;
import com.rusefi.ui.widgets.GaugeStyle;

import java.awt.*;
import java.awt.geom.Path2D;

/**
 * What a live value field draws behind its number: a fill by where the value sits between the
 * gauge's min and max, and a faint line of the last {@link #HISTORY_MS} ms, so oscillation and
 * erratic readings show without reading the digits. Inside a warning or danger zone the fill and
 * text take the round gauge's colours for that zone. Without a usable range nothing is drawn.
 * Used by the Tuning tab's gauge strip ({@code TextGauge}) and dialog readouts ({@link LiveValueBar}).
 * Colours are the Gauges tab's [tag:console_theme].
 */
public class LiveValueTrace {
    /** Long enough for several idle hunting cycles, short enough that fast oscillation stays visible. */
    static final long HISTORY_MS = 5000;
    /** Outlasts the window at the console's output-channel polling rate; older samples are dropped. */
    private static final int HISTORY_CAPACITY = 512;
    private static final float HISTORY_ALPHA = 0.45f;
    /**
     * The line zooms to its own recent spread, so idle hunting of 100 RPM on an 8000 RPM scale
     * spans most of its height; but never closer than this share of the range, so sensor noise
     * stays a flat line.
     */
    static final double HISTORY_MIN_SPAN = 0.02;

    private double min = Double.NaN;
    private double max = Double.NaN;
    private double value = Double.NaN;
    private CircularGauge.Zones zones;
    // ring buffer of recent samples; NaN values break the line
    private final double[] historyValues = new double[HISTORY_CAPACITY];
    private final long[] historyTimes = new long[HISTORY_CAPACITY];
    private int historyStart;
    private int historySize;

    /** Sets the range and clears the history; a missing or empty range draws nothing. */
    public void setRange(double min, double max) {
        this.min = min;
        this.max = max;
        clear();
    }

    /** Warning and danger limits for the fill and text colour; null for none. */
    public void setZones(CircularGauge.Zones zones) {
        this.zones = zones;
    }

    /** Forgets the value and its history, e.g. after a disconnect. */
    public void clear() {
        value = Double.NaN;
        historyStart = 0;
        historySize = 0;
    }

    public boolean hasRange() {
        return Double.isFinite(min) && Double.isFinite(max) && max > min;
    }

    public void add(double value) {
        add(value, System.currentTimeMillis());
    }

    public void add(double value, long nowMs) {
        this.value = value;
        if (!hasRange()) {
            return;
        }
        int end = (historyStart + historySize) % HISTORY_CAPACITY;
        historyValues[end] = value;
        historyTimes[end] = nowMs;
        if (historySize < HISTORY_CAPACITY) {
            historySize++;
        } else {
            historyStart = (historyStart + 1) % HISTORY_CAPACITY;
        }
        while (historySize > 0 && nowMs - historyTimes[historyStart] > HISTORY_MS) {
            historyStart = (historyStart + 1) % HISTORY_CAPACITY;
            historySize--;
        }
    }

    public CircularGauge.Zone zone() {
        return zones == null || !hasRange() ? CircularGauge.Zone.NORMAL : zones.zoneOf(value, 0);
    }

    /** The number's colour: the zone's, or {@code normal} outside any zone. */
    public Color textColor(Color normal) {
        CircularGauge.Zone zone = zone();
        return zone == CircularGauge.Zone.DANGER ? GaugeStyle.danger()
            : zone == CircularGauge.Zone.WARNING ? GaugeStyle.warningText()
            : zone == CircularGauge.Zone.BELOW_SCALE ? GaugeStyle.tick() : normal;
    }

    /** Filled share of the bar, clamped to 0..1; NaN without a range or value. */
    static double fraction(double value, double min, double max) {
        if (!Double.isFinite(value) || !Double.isFinite(min) || !Double.isFinite(max) || max <= min) {
            return Double.NaN;
        }
        return Math.max(0, Math.min(1, (value - min) / (max - min)));
    }

    public double fraction() {
        return fraction(value, min, max);
    }

    /** Samples kept for the history line. */
    public int historySize() {
        return historySize;
    }

    /**
     * Bottom and top of the history line: the recent samples' spread, centred and widened to at
     * least {@link #HISTORY_MIN_SPAN} of the range, clamped into that range.
     */
    double[] historyScale() {
        double lo = Double.POSITIVE_INFINITY;
        double hi = Double.NEGATIVE_INFINITY;
        for (int i = 0; i < historySize; i++) {
            double v = historyValues[(historyStart + i) % HISTORY_CAPACITY];
            if (Double.isFinite(v)) {
                lo = Math.min(lo, Math.max(min, Math.min(max, v)));
                hi = Math.max(hi, Math.max(min, Math.min(max, v)));
            }
        }
        if (lo > hi) {
            return new double[]{min, max};
        }
        double span = Math.max(hi - lo, HISTORY_MIN_SPAN * (max - min));
        double bottom = Math.max(min, Math.min(max - span, (lo + hi - span) / 2));
        return new double[]{bottom, bottom + span};
    }

    /**
     * Paints the fill and history line into {@code area}; the caller clips to its own shape.
     * {@code track} paints the unfilled part too (null leaves the caller's background).
     */
    public void paint(Graphics2D graphics, Rectangle area, Color track, Color lineColor) {
        if (!hasRange()) {
            return;
        }
        Graphics2D g = (Graphics2D) graphics.create();
        try {
            g.setRenderingHint(RenderingHints.KEY_ANTIALIASING, RenderingHints.VALUE_ANTIALIAS_ON);
            if (track != null) {
                g.setColor(track);
                g.fill(area);
            }
            double fraction = fraction();
            if (Double.isFinite(fraction) && fraction > 0) {
                CircularGauge.Zone zone = zone();
                g.setColor(zone == CircularGauge.Zone.DANGER ? GaugeStyle.dangerFill()
                    : zone == CircularGauge.Zone.WARNING ? GaugeStyle.warningFill() : GaugeStyle.progress());
                g.fillRect(area.x, area.y, (int) Math.round(area.width * fraction), area.height);
            }
            paintHistory(g, area, lineColor);
        } finally {
            g.dispose();
        }
    }

    /** Right-aligned to the newest sample, scaled by {@link #historyScale}, faint enough to keep text readable. */
    private void paintHistory(Graphics2D g, Rectangle area, Color lineColor) {
        if (historySize < 2) {
            return;
        }
        g.setComposite(AlphaComposite.getInstance(AlphaComposite.SRC_OVER, HISTORY_ALPHA));
        g.setColor(lineColor);
        g.setStroke(new BasicStroke(1.2f, BasicStroke.CAP_ROUND, BasicStroke.JOIN_ROUND));
        long newest = historyTimes[(historyStart + historySize - 1) % HISTORY_CAPACITY];
        double inset = 2;
        double height = area.height - 2 * inset;
        double[] scale = historyScale();
        Path2D.Double line = new Path2D.Double();
        boolean drawing = false;
        for (int i = 0; i < historySize; i++) {
            int index = (historyStart + i) % HISTORY_CAPACITY;
            double fraction = fraction(historyValues[index], scale[0], scale[1]);
            if (!Double.isFinite(fraction)) {
                drawing = false;
                continue;
            }
            double x = area.x + area.width * (1 - (newest - historyTimes[index]) / (double) HISTORY_MS);
            double y = area.y + inset + height * (1 - fraction);
            if (drawing) {
                line.lineTo(x, y);
            } else {
                line.moveTo(x, y);
                drawing = true;
            }
        }
        g.draw(line);
    }
}
