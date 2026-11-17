package com.rusefi.ui.logview;

import java.util.ArrayList;
import java.util.Collections;
import java.util.List;

/**
 * Stacked plots and the signals in each. A signal is in at most one plot; colours come from
 * {@link TraceColors}, so a signal keeps its colour when other signals or plots change.
 * New signals go to the active plot. There is always at least one plot. Each plot has a height weight;
 * plots share the chart height in proportion to their weights. In each plot, signals with the same unit share
 * one scale unless the plot scales them separately.
 */
final class PlotLayout {
    static final int MAX_PLOTS = 8;

    private final TraceColors colors;
    private final List<List<Integer>> plots = new ArrayList<>();
    private final List<Double> weights = new ArrayList<>();
    /** Per plot: signals with the same unit share one scale (the default). */
    private final List<Boolean> sameUnitShared = new ArrayList<>();
    /** Per plot: the unit the user chose for the left / right y axis, or null for automatic. */
    private final List<String> leftAxisUnit = new ArrayList<>();
    private final List<String> rightAxisUnit = new ArrayList<>();
    /** A right axis choice of no axis at all. */
    static final String NO_AXIS = "";
    private final List<Runnable> listeners = new ArrayList<>();
    private int active;

    PlotLayout(int fieldCount) {
        colors = new TraceColors(fieldCount);
        plots.add(new ArrayList<>());
        weights.add(1.0);
        sameUnitShared.add(true);
        leftAxisUnit.add(null);
        rightAxisUnit.add(null);
    }

    /** One plot holding the given fields, in field order. */
    static PlotLayout single(int fieldCount, boolean[] displayed) {
        PlotLayout layout = new PlotLayout(fieldCount);
        for (int field = 0; field < displayed.length; field++) {
            if (displayed[field]) {
                layout.add(field);
            }
        }
        return layout;
    }

    /** The same plots, signals, colours, heights and axes, without the listeners; e.g. for a trimmed copy of the log. */
    PlotLayout copy() {
        PlotLayout copy = new PlotLayout(colors.fieldCount());
        copy.plots.clear();
        copy.weights.clear();
        copy.sameUnitShared.clear();
        copy.leftAxisUnit.clear();
        copy.rightAxisUnit.clear();
        for (List<Integer> fields : plots) {
            copy.plots.add(new ArrayList<>(fields));
        }
        copy.weights.addAll(weights);
        copy.sameUnitShared.addAll(sameUnitShared);
        copy.leftAxisUnit.addAll(leftAxisUnit);
        copy.rightAxisUnit.addAll(rightAxisUnit);
        copy.colors.copyFrom(colors);
        copy.active = active;
        return copy;
    }

    void addListener(Runnable listener) {
        listeners.add(listener);
    }

    private void changed() {
        for (Runnable listener : listeners) {
            listener.run();
        }
    }

    TraceColors colors() {
        return colors;
    }

    int fieldCount() {
        return colors.fieldCount();
    }

    int plotCount() {
        return plots.size();
    }

    List<Integer> fields(int plot) {
        return Collections.unmodifiableList(plots.get(plot));
    }

    int activePlot() {
        return active;
    }

    void setActive(int plot) {
        if (plot != active && plot >= 0 && plot < plots.size()) {
            active = plot;
            changed();
        }
    }

    boolean isPlotted(int field) {
        return colors.isPlotted(field);
    }

    /** Index of the plot holding the field, or -1. */
    int plotOf(int field) {
        for (int plot = 0; plot < plots.size(); plot++) {
            if (plots.get(plot).contains(field)) {
                return plot;
            }
        }
        return -1;
    }

    boolean canAddPlot() {
        return plots.size() < MAX_PLOTS;
    }

    /** Adds the field to the active plot; does nothing if it is already plotted. */
    void add(int field) {
        addTo(field, active);
    }

    void addTo(int field, int plot) {
        if (colors.isPlotted(field)) {
            return;
        }
        plots.get(plot).add(field);
        colors.plot(field);
        changed();
    }

    /** Adds the field to a new plot below the others, which becomes active; the active plot when full. */
    void addToNewPlot(int field) {
        if (colors.isPlotted(field)) {
            return;
        }
        if (canAddPlot()) {
            appendPlot();
        }
        addTo(field, active);
    }

    /** Moves a field to another plot, keeping its colour; an unplotted field is added there. */
    void moveTo(int field, int plot) {
        int from = plotOf(field);
        if (from == plot) {
            return;
        }
        if (from < 0) {
            addTo(field, plot);
            return;
        }
        plots.get(from).remove(Integer.valueOf(field));
        plots.get(plot).add(field);
        changed();
    }

    /** Moves a field to a new plot below the others, which becomes active; to the active plot when full. */
    void moveToNewPlot(int field) {
        if (!canAddPlot()) {
            moveTo(field, active);
            return;
        }
        int from = plotOf(field);
        appendPlot();
        if (from < 0) {
            addTo(field, active);
            return;
        }
        plots.get(from).remove(Integer.valueOf(field));
        plots.get(active).add(field);
        changed();
    }

    private void appendPlot() {
        plots.add(new ArrayList<>());
        weights.add(1.0);
        sameUnitShared.add(true);
        leftAxisUnit.add(null);
        rightAxisUnit.add(null);
        active = plots.size() - 1;
    }

    double weight(int plot) {
        return weights.get(plot);
    }

    /** Sets the plots' height weights, e.g. their pixel heights after the user dragged a divider. */
    void setWeights(double[] newWeights) {
        for (int plot = 0; plot < newWeights.length && plot < weights.size(); plot++) {
            weights.set(plot, Math.max(newWeights[plot], 1e-6));
        }
        changed();
    }

    /** Gives every plot the same height. */
    void resetWeights() {
        for (int plot = 0; plot < weights.size(); plot++) {
            weights.set(plot, 1.0);
        }
        changed();
    }

    void remove(int field) {
        int plot = plotOf(field);
        if (plot >= 0) {
            plots.get(plot).remove(Integer.valueOf(field));
            colors.remove(field);
            changed();
        }
    }

    /** Removes a plotted field, otherwise adds it to the active plot. */
    void toggle(int field) {
        if (colors.isPlotted(field)) {
            remove(field);
        } else {
            add(field);
        }
    }

    /** Adds an empty plot below the others and makes it active. */
    void addPlot() {
        if (canAddPlot()) {
            appendPlot();
            changed();
        }
    }

    /** Removes every trace of the plot, which stays (empty) in its place. */
    void clearPlot(int plot) {
        List<Integer> fields = plots.get(plot);
        if (fields.isEmpty()) {
            return;
        }
        for (int field : fields) {
            colors.remove(field);
        }
        fields.clear();
        changed();
    }

    boolean canRemovePlot() {
        return plots.size() > 1;
    }

    /** Removes the plot and its traces; the last remaining plot stays. */
    void removePlot(int plot) {
        if (!canRemovePlot()) {
            return;
        }
        for (int field : plots.remove(plot)) {
            colors.remove(field);
        }
        weights.remove(plot);
        sameUnitShared.remove(plot);
        leftAxisUnit.remove(plot);
        rightAxisUnit.remove(plot);
        if (active > plot || active == plots.size()) {
            active--;
        }
        changed();
    }

    /** Moves a plot one place up (-1) or down (+1); the active plot moves with it. */
    void move(int plot, int direction) {
        int target = plot + direction;
        if (target < 0 || target >= plots.size()) {
            return;
        }
        Collections.swap(plots, plot, target);
        Collections.swap(weights, plot, target);
        Collections.swap(sameUnitShared, plot, target);
        Collections.swap(leftAxisUnit, plot, target);
        Collections.swap(rightAxisUnit, plot, target);
        if (active == plot) {
            active = target;
        } else if (active == target) {
            active = plot;
        }
        changed();
    }

    boolean isSameUnitShared(int plot) {
        return sameUnitShared.get(plot);
    }

    void setSameUnitShared(int plot, boolean shared) {
        if (sameUnitShared.get(plot) != shared) {
            sameUnitShared.set(plot, shared);
            changed();
        }
    }

    /** The unit chosen for a plot's left axis, or null for automatic. */
    String leftAxisUnit(int plot) {
        return leftAxisUnit.get(plot);
    }

    /** The unit chosen for a plot's right axis, {@link #NO_AXIS} for none, or null for automatic. */
    String rightAxisUnit(int plot) {
        return rightAxisUnit.get(plot);
    }

    void setLeftAxisUnit(int plot, String unit) {
        leftAxisUnit.set(plot, unit);
        changed();
    }

    void setRightAxisUnit(int plot, String unit) {
        rightAxisUnit.set(plot, unit);
        changed();
    }

    int plottedCount() {
        return colors.plottedCount();
    }
}
