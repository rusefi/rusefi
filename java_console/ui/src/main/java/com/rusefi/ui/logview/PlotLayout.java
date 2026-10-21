package com.rusefi.ui.logview;

import java.util.ArrayList;
import java.util.Collections;
import java.util.List;

/**
 * Stacked plots and the signals in each. A signal is in at most one plot; colours come from
 * {@link TraceColors}, so a signal keeps its colour when other signals or plots change.
 * New signals go to the active plot. There is always at least one plot.
 */
final class PlotLayout {
    static final int MAX_PLOTS = 8;

    private final TraceColors colors;
    private final List<List<Integer>> plots = new ArrayList<>();
    private final List<Runnable> listeners = new ArrayList<>();
    private int active;

    PlotLayout(int fieldCount) {
        colors = new TraceColors(fieldCount);
        plots.add(new ArrayList<>());
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
            plots.add(new ArrayList<>());
            active = plots.size() - 1;
        }
        addTo(field, active);
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
            plots.add(new ArrayList<>());
            active = plots.size() - 1;
            changed();
        }
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
        if (active == plot) {
            active = target;
        } else if (active == target) {
            active = plot;
        }
        changed();
    }

    int plottedCount() {
        return colors.plottedCount();
    }
}
