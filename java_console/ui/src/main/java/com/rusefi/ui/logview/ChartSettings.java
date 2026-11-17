package com.rusefi.ui.logview;

import com.rusefi.core.preferences.storage.Node;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

/**
 * Display options of the log chart: grid lines, cursor, values, min/max markers, plot colours and display-only trace
 * smoothing. Kept in the console config, so they carry over from one log to the next and survive a restart.
 */
final class ChartSettings {
    enum VerticalMode {
        /** One line per time-axis tick; spacing follows {@link #getTimeSpacing()}. */
        TIME_TICKS,
        /** One line per logged sample, drawn only once zoomed in far enough to tell them apart. */
        SAMPLES
    }

    /** How the values at the cursor are shown beside the cursor line, in addition to the legends. */
    enum CursorValues {
        /** Only in the legends. */
        OFF,
        /**
         * A tag beside each trace where it crosses the cursor line; tags of close or crossing traces are pushed
         * apart, with a leader line back to their trace. A plot whose tags do not fit falls back to {@link #LIST}.
         */
        LABELS,
        /** One box per plot listing its values. */
        LIST
    }

    static final int MIN_TIME_SPACING = 40;
    static final int MAX_TIME_SPACING = 300;
    static final int DEFAULT_TIME_SPACING = 90;
    static final int MIN_HORIZONTAL_DIVISIONS = 2;
    static final int MAX_HORIZONTAL_DIVISIONS = 20;
    static final int DEFAULT_HORIZONTAL_DIVISIONS = 4;
    /** Sample lines are hidden when there would be more than one per this many pixels on average. */
    static final int MIN_SAMPLE_LINE_SPACING = 4;

    private boolean vertical;
    private boolean horizontal;
    private VerticalMode verticalMode = VerticalMode.TIME_TICKS;
    private int timeSpacing = DEFAULT_TIME_SPACING;
    private int horizontalDivisions = DEFAULT_HORIZONTAL_DIVISIONS;
    private boolean snapCursor = true;
    // plots start in the console theme's brightness [tag:console_dark_mode]
    private boolean darkPlot = com.rusefi.ui.basic.UiHelper.isDarkTheme();
    private CursorValues cursorValues = CursorValues.LABELS;
    private boolean pinValues;
    private boolean alwaysMinMax;
    /** Width of the centred display-only moving average; zero draws recorded samples. */
    private double smoothingWindowSeconds;
    /** Per-signal windows by log field name; absent means use {@link #smoothingWindowSeconds}. */
    private final Map<String, Double> smoothingOverrides = new HashMap<>();

    private static final String VERTICAL = "vertical";
    private static final String HORIZONTAL = "horizontal";
    private static final String VERTICAL_MODE = "verticalMode";
    private static final String TIME_SPACING = "timeSpacing";
    private static final String HORIZONTAL_DIVISIONS = "horizontalDivisions";
    private static final String SNAP_CURSOR = "snapCursor";
    private static final String DARK_PLOT = "darkPlot";
    private static final String CURSOR_VALUES = "cursorValues";
    private static final String PIN_VALUES = "pinValues";
    private static final String ALWAYS_MIN_MAX = "alwaysMinMax";
    private static final String SMOOTHING = "smoothing";
    private static final String SMOOTHING_OVERRIDES = "smoothingSignals";

    private final Node node;
    private final List<Runnable> listeners = new ArrayList<>();

    /** Options that are forgotten with the viewer, e.g. in the sandbox. */
    ChartSettings() {
        this(new Node());
    }

    /** Options read from {@code node} and written back to it on every change; unset ones keep their defaults. */
    ChartSettings(Node node) {
        this.node = node;
        vertical = node.getBoolProperty(VERTICAL, vertical);
        horizontal = node.getBoolProperty(HORIZONTAL, horizontal);
        verticalMode = enumProperty(VERTICAL_MODE, VerticalMode.values(), verticalMode);
        timeSpacing = clamp(node.getIntProperty(TIME_SPACING, timeSpacing), MIN_TIME_SPACING, MAX_TIME_SPACING);
        horizontalDivisions = clamp(node.getIntProperty(HORIZONTAL_DIVISIONS, horizontalDivisions),
                MIN_HORIZONTAL_DIVISIONS, MAX_HORIZONTAL_DIVISIONS);
        snapCursor = node.getBoolProperty(SNAP_CURSOR, snapCursor);
        darkPlot = node.getBoolProperty(DARK_PLOT, darkPlot);
        cursorValues = enumProperty(CURSOR_VALUES, CursorValues.values(), cursorValues);
        pinValues = node.getBoolProperty(PIN_VALUES, pinValues);
        alwaysMinMax = node.getBoolProperty(ALWAYS_MIN_MAX, alwaysMinMax);
        smoothingWindowSeconds = validWindow(node.getDoubleProperty(SMOOTHING, 0), 0);
        for (Map.Entry<String, Object> entry : node.getChild(SMOOTHING_OVERRIDES).getConfig().entrySet()) {
            if (entry.getValue() instanceof String && Node.isNumeric((String) entry.getValue())) {
                double seconds = validWindow(Double.parseDouble((String) entry.getValue()), -1);
                if (seconds >= 0) {
                    smoothingOverrides.put(entry.getKey(), seconds);
                }
            }
        }
    }

    /** Called after every change, e.g. to save the config. */
    void addListener(Runnable listener) {
        listeners.add(listener);
    }

    private <E extends Enum<E>> E enumProperty(String key, E[] values, E fallback) {
        String name = node.getProperty(key, null);
        for (E value : values) {
            if (value.name().equals(name)) {
                return value;
            }
        }
        return fallback;
    }

    private static int clamp(int value, int min, int max) {
        return Math.max(min, Math.min(max, value));
    }

    private static double validWindow(double seconds, double fallback) {
        return seconds >= 0 && Double.isFinite(seconds) ? seconds : fallback;
    }

    /** Writes every option to the node and tells the listeners. */
    private void changed() {
        node.setProperty(VERTICAL, vertical);
        node.setProperty(HORIZONTAL, horizontal);
        node.setProperty(VERTICAL_MODE, verticalMode.name());
        node.setProperty(TIME_SPACING, timeSpacing);
        node.setProperty(HORIZONTAL_DIVISIONS, horizontalDivisions);
        node.setProperty(SNAP_CURSOR, snapCursor);
        // stored only when it differs from the console theme, so an untouched choice keeps following the theme
        if (darkPlot == com.rusefi.ui.basic.UiHelper.isDarkTheme()) {
            node.getConfig().remove(DARK_PLOT);
        } else {
            node.setProperty(DARK_PLOT, darkPlot);
        }
        node.setProperty(CURSOR_VALUES, cursorValues.name());
        node.setProperty(PIN_VALUES, pinValues);
        node.setProperty(ALWAYS_MIN_MAX, alwaysMinMax);
        node.setProperty(SMOOTHING, smoothingWindowSeconds);
        Node overrides = node.getChild(SMOOTHING_OVERRIDES);
        overrides.getConfig().clear();
        for (Map.Entry<String, Double> entry : smoothingOverrides.entrySet()) {
            overrides.setProperty(entry.getKey(), entry.getValue());
        }
        for (Runnable listener : listeners) {
            listener.run();
        }
    }

    double getSmoothingWindowSeconds() {
        return smoothingWindowSeconds;
    }

    void setSmoothingWindowSeconds(double smoothingWindowSeconds) {
        validateSmoothingWindow(smoothingWindowSeconds);
        this.smoothingWindowSeconds = smoothingWindowSeconds;
        changed();
    }

    double getSmoothingWindowSeconds(String signal) {
        return smoothingOverrides.getOrDefault(signal, smoothingWindowSeconds);
    }

    boolean usesGlobalSmoothing(String signal) {
        return !smoothingOverrides.containsKey(signal);
    }

    void setSmoothingWindowSeconds(String signal, double smoothingWindowSeconds) {
        validateSmoothingWindow(smoothingWindowSeconds);
        smoothingOverrides.put(signal, smoothingWindowSeconds);
        changed();
    }

    void useGlobalSmoothing(String signal) {
        smoothingOverrides.remove(signal);
        changed();
    }

    private static void validateSmoothingWindow(double smoothingWindowSeconds) {
        if (smoothingWindowSeconds < 0 || !Double.isFinite(smoothingWindowSeconds)) {
            throw new IllegalArgumentException("Smoothing window must be finite and non-negative");
        }
    }

    CursorValues getCursorValues() {
        return cursorValues;
    }

    void setCursorValues(CursorValues cursorValues) {
        this.cursorValues = cursorValues;
        changed();
    }

    /**
     * False: the values follow the cursor while the pointer is over the traces. True: they stay at the clicked time
     * until another click there or Esc.
     */
    boolean isPinValues() {
        return pinValues;
    }

    void setPinValues(boolean pinValues) {
        this.pinValues = pinValues;
        changed();
    }

    /**
     * False: min/max markers show only on the hovered or pinned trace. True: every trace shows its visible min and
     * max; the hovered or pinned one keeps the full-size markers.
     */
    boolean isAlwaysMinMax() {
        return alwaysMinMax;
    }

    void setAlwaysMinMax(boolean alwaysMinMax) {
        this.alwaysMinMax = alwaysMinMax;
        changed();
    }

    /** Plot area colours, chosen independently of the console theme; dark by default in the dark theme. */
    boolean isDarkPlot() {
        return darkPlot;
    }

    void setDarkPlot(boolean darkPlot) {
        this.darkPlot = darkPlot;
        changed();
    }

    /**
     * True: the cursor line jumps to the nearest logged sample. False: it follows the mouse freely;
     * the values shown still come from the nearest sample, which is marked on each trace.
     */
    boolean isSnapCursor() {
        return snapCursor;
    }

    void setSnapCursor(boolean snapCursor) {
        this.snapCursor = snapCursor;
        changed();
    }

    boolean isVertical() {
        return vertical;
    }

    void setVertical(boolean vertical) {
        this.vertical = vertical;
        changed();
    }

    boolean isHorizontal() {
        return horizontal;
    }

    void setHorizontal(boolean horizontal) {
        this.horizontal = horizontal;
        changed();
    }

    VerticalMode getVerticalMode() {
        return verticalMode;
    }

    void setVerticalMode(VerticalMode verticalMode) {
        this.verticalMode = verticalMode;
        changed();
    }

    /** Approximate pixels between time ticks; also spaces the time-axis labels. */
    int getTimeSpacing() {
        return timeSpacing;
    }

    void setTimeSpacing(int pixels) {
        timeSpacing = clamp(pixels, MIN_TIME_SPACING, MAX_TIME_SPACING);
        changed();
    }

    int getHorizontalDivisions() {
        return horizontalDivisions;
    }

    void setHorizontalDivisions(int divisions) {
        horizontalDivisions = clamp(divisions, MIN_HORIZONTAL_DIVISIONS, MAX_HORIZONTAL_DIVISIONS);
        changed();
    }
}
