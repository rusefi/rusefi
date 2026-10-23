package com.rusefi.ui.logview;

import com.rusefi.core.preferences.storage.Node;

import java.util.ArrayList;
import java.util.List;

/**
 * What the histogram remembers from one log to the next and across console runs: the view shown, the X / Y / Z
 * signals (by name, so another log with the same signals opens the same way), the cell value, units, bins, the
 * thin-data limit, the filters and whether the cell panel is open. Saved with the console config on exit.
 */
final class HistogramSettings {
    enum Mode {
        MEAN("Mean", "Time-weighted mean of each cell"),
        MIN("Min", "Lowest record in each cell"),
        MAX("Max", "Highest record in each cell"),
        TIME("Time", "Seconds the engine spent in each cell"),
        COUNT("Count", "Records in each cell");

        final String text;
        final String tooltip;

        Mode(String text, String tooltip) {
            this.text = text;
            this.tooltip = tooltip;
        }

        /** The cell shows a Z value, not occupancy. */
        boolean isValue() {
            return this == MEAN || this == MIN || this == MAX;
        }
    }

    static final int DEFAULT_BINS = 16;
    static final double DEFAULT_MIN_TIME = 1.0;
    static final double MAX_MIN_TIME = 60;

    private final Node node;

    /** Settings that are forgotten with the viewer, e.g. in the sandbox. */
    HistogramSettings() {
        this(new Node());
    }

    HistogramSettings(Node node) {
        this.node = node;
    }

    static final String TIMELINE = "timeline";
    static final String HISTOGRAM = "histogram";
    static final String SCATTER = "scatter";

    /** The view shown last: {@link #TIMELINE}, {@link #HISTOGRAM} or {@link #SCATTER}. */
    String view() {
        String view = node.getProperty("view", "");
        if (view.isEmpty()) {
            return node.getBoolProperty("histogram_view", false) ? HISTOGRAM : TIMELINE;
        }
        return view;
    }

    void setView(String view) {
        node.setProperty("view", view);
    }

    boolean isHistogramView() {
        return HISTOGRAM.equals(view());
    }

    void setHistogramView(boolean histogram) {
        setView(histogram ? HISTOGRAM : TIMELINE);
    }

    /** Settings kept under these, e.g. the scatter view's second plot. */
    HistogramSettings child(String name) {
        return new HistogramSettings(node.getChild(name));
    }

    /** Scatter: one plot, or two side by side. */
    int plots() {
        return node.getIntProperty("plots", 1) == 2 ? 2 : 1;
    }

    void setPlots(int plots) {
        node.setProperty("plots", plots == 2 ? 2 : 1);
    }

    /** Scatter: colour by density or by Z. */
    ScatterPlot.ColourBy colourBy() {
        try {
            return ScatterPlot.ColourBy.valueOf(node.getProperty("colour_by", ScatterPlot.ColourBy.DENSITY.name()));
        } catch (IllegalArgumentException e) {
            return ScatterPlot.ColourBy.DENSITY;
        }
    }

    void setColourBy(ScatterPlot.ColourBy colourBy) {
        node.setProperty("colour_by", colourBy.name());
    }

    /** Scatter: the recipe applied last (its guide lines and fade level apply while its signals are on the axes). */
    String recipe() {
        String recipe = node.getProperty("recipe", "");
        return recipe.isEmpty() ? null : recipe;
    }

    void setRecipe(String recipe) {
        node.setProperty("recipe", recipe == null ? "" : recipe);
    }

    /**
     * The signal the user put on an axis ('x', 'y' or 'z'), or null while the axis is picked automatically.
     * Automatic picks are never saved, so a poor one for one log does not stick to the next.
     */
    String signal(char axis) {
        String name = node.getProperty("picked_" + axis, "");
        return name.isEmpty() ? null : name;
    }

    void setSignal(char axis, String name) {
        node.setProperty("picked_" + axis, name == null ? "" : name);
    }

    Mode mode() {
        try {
            return Mode.valueOf(node.getProperty("mode", Mode.MEAN.name()));
        } catch (IllegalArgumentException e) {
            return Mode.MEAN;
        }
    }

    void setMode(Mode mode) {
        node.setProperty("mode", mode.name());
    }

    /** Show a lambda Z as AFR (or an AFR Z as lambda when false). */
    boolean showAfr() {
        return node.getBoolProperty("afr", false);
    }

    void setShowAfr(boolean afr) {
        node.setBoolProperty("afr", afr);
    }

    int columns() {
        return clampBins(node.getIntProperty("columns", DEFAULT_BINS));
    }

    void setColumns(int columns) {
        node.setProperty("columns", clampBins(columns));
    }

    int rows() {
        return clampBins(node.getIntProperty("rows", DEFAULT_BINS));
    }

    void setRows(int rows) {
        node.setProperty("rows", clampBins(rows));
    }

    /** The tune table (.ini name, e.g. veTableTbl) whose axes are the bins, or null for even bins. */
    String binsTable() {
        String table = node.getProperty("bins_table", "");
        return table.isEmpty() ? null : table;
    }

    void setBinsTable(String table) {
        node.setProperty("bins_table", table == null ? "" : table);
    }

    static int clampBins(int bins) {
        return Math.max(HistogramAxis.MIN_BINS, Math.min(HistogramAxis.MAX_BINS, bins));
    }

    /** Cells with less time than this are drawn as thin data (hatched) or hidden. */
    double minTime() {
        double seconds = node.getDoubleProperty("min_time", DEFAULT_MIN_TIME);
        return Double.isFinite(seconds) ? Math.max(0, Math.min(MAX_MIN_TIME, seconds)) : DEFAULT_MIN_TIME;
    }

    void setMinTime(double seconds) {
        node.setProperty("min_time", Math.max(0, Math.min(MAX_MIN_TIME, seconds)));
    }

    boolean hideThin() {
        return node.getBoolProperty("hide_thin", false);
    }

    void setHideThin(boolean hide) {
        node.setBoolProperty("hide_thin", hide);
    }

    /** The cell colours for a Z signal, each signal keeping its own. */
    HistogramColors colors(String zSignal) {
        return new HistogramColors(node.getChild("colors").getChild(zSignal));
    }

    boolean isCellPanelVisible() {
        return node.getBoolProperty("cell_panel", true);
    }

    void setCellPanelVisible(boolean visible) {
        node.setBoolProperty("cell_panel", visible);
    }

    /** Saved one per key ({@code filter_0}, {@code filter_1}...), so no value holds a line break. */
    List<HistogramFilter> filters() {
        List<HistogramFilter> filters = new ArrayList<>();
        int count = node.getIntProperty("filter_count", 0);
        for (int i = 0; i < count; i++) {
            HistogramFilter filter = HistogramFilter.decode(node.getProperty("filter_" + i, ""));
            if (filter != null) {
                filters.add(filter);
            }
        }
        return filters;
    }

    void setFilters(List<HistogramFilter> filters) {
        int old = node.getIntProperty("filter_count", 0);
        for (int i = filters.size(); i < old; i++) {
            node.getConfig().remove("filter_" + i);
        }
        for (int i = 0; i < filters.size(); i++) {
            node.setProperty("filter_" + i, filters.get(i).encode());
        }
        node.setProperty("filter_count", filters.size());
    }
}
