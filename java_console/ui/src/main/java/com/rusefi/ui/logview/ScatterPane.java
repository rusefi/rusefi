package com.rusefi.ui.logview;

import com.rusefi.logview.CsvLogReader;

import javax.swing.BorderFactory;
import javax.swing.JMenuItem;
import javax.swing.JPanel;
import javax.swing.JPopupMenu;
import javax.swing.event.PopupMenuEvent;
import javax.swing.event.PopupMenuListener;
import java.awt.BorderLayout;
import java.awt.Color;
import java.awt.Component;
import java.awt.event.MouseAdapter;
import java.awt.event.MouseEvent;
import java.util.ArrayList;
import java.util.Collections;
import java.util.List;
import java.util.Locale;

import static com.rusefi.ui.logview.ThemedStrip.themed;

/**
 * One scatter plot with its own signal and filter strip, colouring and recipe: the Scatter view shows one, or two
 * side by side to compare (e.g. a knock map next to lambda against its target). Its settings are remembered apart
 * from the other plot's. A press anywhere on it makes it the plot the toolbar and the sidebar act on.
 * [tag:console_theme]
 */
final class ScatterPane extends JPanel {
    private final CsvLogReader log;
    final HistogramSettings settings;
    private final ScatterView owner;
    final SignalStrip strip;
    final ScatterPlot plot = new ScatterPlot();

    ScatterPane(CsvLogReader log, HistogramSettings settings, ScatterView owner, Runnable onSignalsChanged) {
        super(new BorderLayout());
        this.log = log;
        this.settings = settings;
        this.owner = owner;
        setBackground(themed("Console.LogViewer.sidebar.background", Color.WHITE));
        strip = new SignalStrip(log, settings, this::automaticPick, "scatter plot", this::recompute, () -> {
            // a recipe's guide lines belong to its signals (the recompute already found they no longer match)
            settings.setRecipe(null);
            onSignalsChanged.run();
        });
        add(strip, BorderLayout.NORTH);
        add(plot, BorderLayout.CENTER);
        plot.setMenu(plotMenu());
        plot.setActivateListener(this::showSelectionOnTimeline);
        plot.setSelectionListener(owner::updateInfo);
        plot.setHoverListener(() -> owner.updateInfo(this));
        plot.setLegendListener(bounds -> owner.showColors(this, plot, bounds.x, bounds.y));
        // a press anywhere on the plot or its strip makes this the active plot
        MouseAdapter activate = new MouseAdapter() {
            @Override
            public void mousePressed(MouseEvent event) {
                owner.activate(ScatterPane.this);
            }
        };
        for (Component component : new Component[]{this, plot, strip, strip.swapButton, strip.addFilterButton,
                strip.filterRow}) {
            component.addMouseListener(activate);
        }
        for (SignalStrip.AxisSlot slot : strip.slots) {
            slot.addMouseListener(activate);
        }
    }

    /** RPM and MAP, Z the measured lambda, like the histogram, when the user did not set the axis. */
    private int automaticPick(int axis) {
        return axis == 0 ? HistogramSignals.find(log, HistogramSignals.X_NAMES)
                : axis == 1 ? HistogramSignals.find(log, HistogramSignals.Y_NAMES)
                : HistogramSignals.findMixture(log);
    }

    /** Outlined in the accent while it is the active one of two plots. */
    void setMarked(boolean marked) {
        setBorder(marked ? BorderFactory.createMatteBorder(2, 2, 2, 2,
                themed("Console.LogViewer.histogram.accent", new Color(0x1d4ed8)))
                : BorderFactory.createEmptyBorder(2, 2, 2, 2));
    }

    // ---- recipes ----

    /** The applied recipe while its signals are still on the axes, or null. */
    ScatterRecipes.Recipe activeRecipe() {
        ScatterRecipes.Recipe recipe = settings.recipe() == null ? null : ScatterRecipes.byTitle(settings.recipe());
        if (recipe == null) {
            return null;
        }
        int[] fields = recipe.fields(log);
        return fields != null && fields[0] == strip.field('x') && fields[1] == strip.field('y')
                && fields[2] == strip.field('z') ? recipe : null;
    }

    /** The recipes this log has the signals for. */
    List<ScatterRecipes.Recipe> availableRecipes() {
        List<ScatterRecipes.Recipe> available = new ArrayList<>();
        for (ScatterRecipes.Recipe recipe : ScatterRecipes.ALL) {
            if (recipe.fields(log) != null) {
                available.add(recipe);
            }
        }
        return available;
    }

    void apply(ScatterRecipes.Recipe recipe) {
        int[] fields = recipe.fields(log);
        if (fields == null) {
            return;
        }
        settings.setColourBy(recipe.colourBy);
        plot.resetZoom();
        // the strip recomputes; the recipe is set after, since a signal change clears it
        strip.assignAll(fields[0], fields[1], fields[2]);
        settings.setRecipe(recipe.title);
        recompute();
    }

    // ---- computing ----

    /** Gathers the points again after a change of signals, filters, range or colouring. */
    void recompute() {
        boolean[] hidden = HistogramFilter.hidden(log, strip.filters());
        double[] range = owner.timeRange();
        ScatterData data = ScatterData.collect(log, strip.field('x'), strip.field('y'), strip.field('z'), hidden,
                range[0], range[1]);
        ScatterRecipes.Recipe recipe = activeRecipe();
        HistogramTable.Mixture mixture = HistogramTable.Mixture.of(log, strip.field('z'), false);
        plot.setDark(owner.isDark());
        plot.set(data, settings.colourBy(), colors(), mixture,
                recipe == null ? Double.NaN : recipe.fadeFor(log, strip.field('z')),
                recipe == null ? Collections.emptyList() : recipe.references);
        strip.refresh();
        owner.paneChanged(this);
    }

    /** The colour settings of the current Z signal, shared with the histogram's for the same signal. */
    HistogramColors colors() {
        return settings.colors(log.name(strip.field('z')));
    }

    /** "RPM 2509 · MAP 37.9 · Lambda 1 0.98 at 12.34 s" for a point. */
    String describe(int point) {
        ScatterData data = plot.data();
        StringBuilder text = new StringBuilder();
        text.append(log.name(data.xField)).append(' ').append(ValueFormat.format(log, data.xField, data.x[point]))
                .append(" · ").append(log.name(data.yField)).append(' ')
                .append(ValueFormat.format(log, data.yField, data.y[point]));
        if (data.zField != data.xField && data.zField != data.yField) {
            text.append(" · ").append(log.name(data.zField)).append(' ')
                    .append(ValueFormat.format(log, data.zField, data.z[point]));
        }
        return text.append(String.format(Locale.ROOT, " at %.2f s", data.time(point))).toString();
    }

    /** "RPM 2000–3000 · MAP 40–60" for the selected box. */
    String describeBox() {
        ScatterData data = plot.data();
        double[] box = plot.box();
        return String.format(Locale.ROOT, "%s %s–%s · %s %s–%s", log.name(data.xField),
                ValueFormat.format(log, data.xField, box[0]), ValueFormat.format(log, data.xField, box[1]),
                log.name(data.yField), ValueFormat.format(log, data.yField, box[2]),
                ValueFormat.format(log, data.yField, box[3]));
    }

    // ---- timeline and filters from the plot ----

    /** The picked point, or the stays of the points in the box, on the timeline. */
    void showSelectionOnTimeline() {
        owner.activate(this);
        ScatterData data = plot.data();
        if (plot.box() != null && !plot.selected().isEmpty()) {
            owner.showOnTimeline(describeBox(), data.stays(plot.selected()));
        } else if (plot.picked() >= 0) {
            int point = plot.picked();
            owner.showOnTimeline(describe(point), data.stays(Collections.singletonList(point)));
        }
    }

    /** A filter that hides the box's points ({@code hide}), or keeps only them. */
    void filterBox(boolean hide) {
        double[] box = plot.box();
        if (box == null) {
            return;
        }
        strip.addFilter(HistogramFilter.box(log.name(strip.field('x')), box[0], box[1], log.name(strip.field('y')),
                box[2], box[3], hide));
    }

    private JPopupMenu plotMenu() {
        JPopupMenu menu = new JPopupMenu();
        JMenuItem timeline = new JMenuItem("Show on timeline");
        timeline.setToolTipText("The picked point, or every stay of the points in the box, shaded on the timeline");
        timeline.addActionListener(e -> showSelectionOnTimeline());
        JMenuItem zoom = new JMenuItem("Zoom to the box");
        zoom.addActionListener(e -> plot.zoomToSelection());
        JMenuItem hide = new JMenuItem("Hide these points");
        hide.setToolTipText("Adds a filter that leaves the records in the box out of this plot, e.g. a decel patch");
        hide.addActionListener(e -> filterBox(true));
        JMenuItem keep = new JMenuItem("Keep only these points");
        keep.setToolTipText("Adds a filter that leaves out every record outside the box");
        keep.addActionListener(e -> filterBox(false));
        JMenuItem clear = new JMenuItem("Clear selection");
        clear.addActionListener(e -> plot.clearSelection());
        JMenuItem whole = new JMenuItem("Whole plot");
        whole.addActionListener(e -> plot.resetZoom());
        menu.add(timeline);
        menu.addSeparator();
        menu.add(zoom);
        menu.add(hide);
        menu.add(keep);
        menu.add(clear);
        menu.addSeparator();
        menu.add(whole);
        menu.addPopupMenuListener(new PopupMenuListener() {
            @Override
            public void popupMenuWillBecomeVisible(PopupMenuEvent event) {
                owner.activate(ScatterPane.this);
                boolean hasBox = plot.box() != null;
                timeline.setEnabled(hasBox && !plot.selected().isEmpty() || plot.picked() >= 0);
                zoom.setEnabled(hasBox);
                hide.setEnabled(hasBox && !plot.selected().isEmpty());
                keep.setEnabled(hasBox && !plot.selected().isEmpty());
                clear.setEnabled(hasBox || plot.picked() >= 0);
                whole.setEnabled(plot.isZoomed());
            }

            @Override
            public void popupMenuWillBecomeInvisible(PopupMenuEvent event) {
            }

            @Override
            public void popupMenuCanceled(PopupMenuEvent event) {
            }
        });
        return menu;
    }
}
