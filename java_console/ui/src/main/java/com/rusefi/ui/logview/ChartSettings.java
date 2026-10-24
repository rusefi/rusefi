package com.rusefi.ui.logview;

/** Display options of the log chart: grid lines (both start off), cursor snapping, cursor values and plot colours. */
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
    private boolean darkPlot;
    private CursorValues cursorValues = CursorValues.LABELS;
    private boolean pinValues;

    CursorValues getCursorValues() {
        return cursorValues;
    }

    void setCursorValues(CursorValues cursorValues) {
        this.cursorValues = cursorValues;
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
    }

    /** Plot area colours, chosen independently of the console theme; light by default. */
    boolean isDarkPlot() {
        return darkPlot;
    }

    void setDarkPlot(boolean darkPlot) {
        this.darkPlot = darkPlot;
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
    }

    boolean isVertical() {
        return vertical;
    }

    void setVertical(boolean vertical) {
        this.vertical = vertical;
    }

    boolean isHorizontal() {
        return horizontal;
    }

    void setHorizontal(boolean horizontal) {
        this.horizontal = horizontal;
    }

    VerticalMode getVerticalMode() {
        return verticalMode;
    }

    void setVerticalMode(VerticalMode verticalMode) {
        this.verticalMode = verticalMode;
    }

    /** Approximate pixels between time ticks; also spaces the time-axis labels. */
    int getTimeSpacing() {
        return timeSpacing;
    }

    void setTimeSpacing(int pixels) {
        timeSpacing = Math.max(MIN_TIME_SPACING, Math.min(MAX_TIME_SPACING, pixels));
    }

    int getHorizontalDivisions() {
        return horizontalDivisions;
    }

    void setHorizontalDivisions(int divisions) {
        horizontalDivisions = Math.max(MIN_HORIZONTAL_DIVISIONS, Math.min(MAX_HORIZONTAL_DIVISIONS, divisions));
    }
}
