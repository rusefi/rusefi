package com.rusefi.ui.logview;

import com.rusefi.logview.CsvLogReader;
import com.rusefi.ui.util.WrapLayout;
import com.rusefi.ui.widgets.JoinedButtons;
import com.rusefi.ui.widgets.SegmentedControl;

import javax.swing.BorderFactory;
import javax.swing.Box;
import javax.swing.ButtonGroup;
import javax.swing.JButton;
import javax.swing.JComponent;
import javax.swing.JLabel;
import javax.swing.JMenuItem;
import javax.swing.JPanel;
import javax.swing.JPopupMenu;
import javax.swing.JSplitPane;
import javax.swing.JToggleButton;
import java.awt.BorderLayout;
import java.awt.Component;
import java.awt.FlowLayout;
import java.util.List;
import java.util.Locale;
import java.util.function.BiConsumer;
import java.util.function.Consumer;

/**
 * The log viewer's Scatter view: every shown record as a point at its X and Y, coloured by the time spent there, by
 * Z or by when it was logged, for seeing how two signals relate (oil pressure against RPM, lambda against its
 * target) and where events such as knock happen. One plot, or two side by side to compare; each has the
 * Histogram's signal and filter strip, its own colouring and recipe. The toolbar acts on the active plot (the one
 * last pressed, outlined when there are two); the range and Light / Dark apply to both. [tag:console_theme]
 */
final class ScatterView extends JPanel {
    private final CsvLogReader log;
    private final HistogramSettings settings;
    private final ChartSettings chartSettings;
    private final MeasureCursors cursors;
    private final Runnable onSignalsChanged;
    final ScatterPane first;
    /** Built when two plots are first shown. */
    private ScatterPane second;
    private ScatterPane active;
    private final JPanel plotArea = new JPanel(new BorderLayout());
    private final JSplitPane split = new JSplitPane(JSplitPane.HORIZONTAL_SPLIT);

    final JToggleButton densityButton = SegmentedControl.segment(ScatterPlot.ColourBy.DENSITY.text);
    final JToggleButton zButton = SegmentedControl.segment(ScatterPlot.ColourBy.Z.text);
    final JToggleButton timeButton = SegmentedControl.segment(ScatterPlot.ColourBy.TIME.text);
    final JToggleButton wholeLogButton = SegmentedControl.segment("Whole log");
    final JToggleButton betweenCursorsButton = SegmentedControl.segment("A–B");
    final JToggleButton lightButton = SegmentedControl.segment("Light");
    final JToggleButton darkButton = SegmentedControl.segment("Dark");
    // "1 | 2" read as "show plot 1 / plot 2"; this says what it does: one plot, or two at once
    final JToggleButton onePlotButton = SegmentedControl.segment("Single");
    final JToggleButton twoPlotsButton = SegmentedControl.segment("Side by side");
    final JButton recipesButton = new JButton("Recipes  ▾");
    final JButton colorsButton = new JButton("Colours  ▾");
    final JButton resetZoomButton = new JButton("Whole plot");
    final JPopupMenu colorsPopup = new JPopupMenu();
    HistogramColorsPanel colorsPanel;

    final JLabel pointsLabel = new JLabel();
    final JLabel hiddenLabel = ChartToolbar.muted(new JLabel());
    final JLabel timeLabel = ChartToolbar.muted(new JLabel());
    final JLabel infoLabel = ChartToolbar.muted(new JLabel());

    private BiConsumer<String, List<double[]>> timelineAction = (name, stays) -> {
    };
    /** After Light / Dark here, so the timeline's switch follows. */
    private Runnable onDarkChanged = () -> {
    };

    /**
     * @param settings      the first plot's settings, and the view's (the second plot's are kept under them)
     * @param chartSettings the timeline's settings, for the shared Light / Dark plot background
     * @param cursors       the timeline's A and B cursors, for the A–B range
     * @param viewSwitch    the Timeline / Histogram / Scatter switch, shown at the start of the toolbar
     * @param onOpen        action of the Open button, given the button, or null for no button
     * @param onSignalsChanged after the active plot's signals change, or another plot becomes active: the sidebar
     */
    ScatterView(CsvLogReader log, HistogramSettings settings, ChartSettings chartSettings, MeasureCursors cursors,
                JComponent viewSwitch, Consumer<Component> onOpen, Runnable onSignalsChanged) {
        super(new BorderLayout());
        this.log = log;
        this.settings = settings;
        this.chartSettings = chartSettings;
        this.cursors = cursors;
        this.onSignalsChanged = onSignalsChanged;
        first = new ScatterPane(log, settings, this, onSignalsChanged);
        active = first;
        split.setResizeWeight(0.5);
        split.setContinuousLayout(true);
        split.setDividerSize(6);
        split.setBorder(BorderFactory.createEmptyBorder());

        add(toolbar(viewSwitch, onOpen), BorderLayout.NORTH);
        add(plotArea, BorderLayout.CENTER);
        add(statusStrip(), BorderLayout.SOUTH);
        cursors.addListener(this::cursorsChanged);
        first.recompute();
        setPlots(settings.plots());
    }

    // ---- the plots ----

    ScatterPane active() {
        return active;
    }

    ScatterPane second() {
        return second;
    }

    /** The active plot's strip, e.g. for the sidebar. */
    SignalStrip strip() {
        return active.strip;
    }

    ScatterPlot plot() {
        return active.plot;
    }

    int plots() {
        return second != null && second.getParent() == split ? 2 : 1;
    }

    /** One plot, or two side by side; the second keeps its own signals, filters and colouring. */
    void setPlots(int plots) {
        settings.setPlots(plots);
        (plots == 2 ? twoPlotsButton : onePlotButton).setSelected(true);
        plotArea.removeAll();
        if (plots == 2) {
            if (second == null) {
                second = new ScatterPane(log, settings.child("second"), this, onSignalsChanged);
                second.recompute();
            }
            split.setLeftComponent(first);
            split.setRightComponent(second);
            plotArea.add(split, BorderLayout.CENTER);
        } else {
            // not removeAll(): that would take the divider too, and the two plots could no longer be resized
            split.setLeftComponent(null);
            split.setRightComponent(null);
            plotArea.add(first, BorderLayout.CENTER);
            if (active != first) {
                activate(first);
            }
        }
        markActive();
        plotArea.revalidate();
        plotArea.repaint();
    }

    /** Makes a plot the one the toolbar and the sidebar act on. */
    void activate(ScatterPane pane) {
        if (pane == active) {
            return;
        }
        active = pane;
        markActive();
        syncToolbar();
        updateInfo();
        onSignalsChanged.run();
    }

    private void markActive() {
        boolean two = plots() == 2;
        first.setMarked(two && active == first);
        if (second != null) {
            second.setMarked(two && active == second);
        }
    }

    /** The toolbar shows the active plot's colouring and recipe. */
    private void syncToolbar() {
        ScatterPlot.ColourBy colourBy = active.settings.colourBy();
        (colourBy == ScatterPlot.ColourBy.Z ? zButton : colourBy == ScatterPlot.ColourBy.TIME ? timeButton
                : densityButton).setSelected(true);
        ScatterRecipes.Recipe recipe = active.activeRecipe();
        recipesButton.setText(recipe == null ? "Recipes  ▾" : "Recipe: " + recipe.title + "  ▾");
    }

    /** After a plot recomputed: the toolbar and the status strip, when it is the active one. */
    void paneChanged(ScatterPane pane) {
        if (pane == active) {
            syncToolbar();
            updateInfo();
        }
    }

    // ---- the active plot, for the toolbar and the tests ----

    ScatterRecipes.Recipe activeRecipe() {
        return active.activeRecipe();
    }

    List<ScatterRecipes.Recipe> availableRecipes() {
        return active.availableRecipes();
    }

    void apply(ScatterRecipes.Recipe recipe) {
        active.apply(recipe);
    }

    String describe(int point) {
        return active.describe(point);
    }

    void showSelectionOnTimeline() {
        active.showSelectionOnTimeline();
    }

    void filterBox(boolean hide) {
        active.filterBox(hide);
    }

    private void setColourBy(ScatterPlot.ColourBy colourBy) {
        active.settings.setColourBy(colourBy);
        active.recompute();
    }

    private JPopupMenu recipesMenu() {
        JPopupMenu menu = new JPopupMenu();
        List<ScatterRecipes.Recipe> available = availableRecipes();
        for (ScatterRecipes.Recipe recipe : ScatterRecipes.ALL) {
            JMenuItem item = new JMenuItem(recipe.title);
            boolean ok = available.contains(recipe);
            item.setEnabled(ok);
            item.setToolTipText(ok ? recipe.description : recipe.description + " · this log lacks its signals");
            item.addActionListener(e -> apply(recipe));
            menu.add(item);
        }
        return menu;
    }

    void rebuildColorsPanel() {
        ScatterPane pane = active;
        colorsPanel = new HistogramColorsPanel(pane.colors(), log.name(pane.strip.field('z')), pane.plot, () -> {
            pane.recompute();
            if (colorsPopup.isVisible()) {
                rebuildColorsPanel();
            }
        }, pane::recompute);
        colorsPopup.removeAll();
        colorsPopup.add(colorsPanel);
        colorsPopup.pack();
    }

    /** The colours of a plot, from its legend: just above it. */
    void showColors(ScatterPane pane, Component invoker, int x, int y) {
        activate(pane);
        rebuildColorsPanel();
        colorsPopup.show(invoker, x, y - colorsPopup.getPreferredSize().height - 4);
    }

    // ---- shared by both plots ----

    boolean isBetweenCursors() {
        return betweenCursorsButton.isSelected() && cursors.isRange();
    }

    /** {start, end} of the range shown, NaN for the whole log. */
    double[] timeRange() {
        return isBetweenCursors() ? new double[]{cursors.start(), cursors.end()}
                : new double[]{Double.NaN, Double.NaN};
    }

    boolean isDark() {
        return chartSettings.isDarkPlot();
    }

    void setTimelineAction(BiConsumer<String, List<double[]>> timelineAction) {
        this.timelineAction = timelineAction;
    }

    void showOnTimeline(String what, List<double[]> stays) {
        timelineAction.accept(what, stays);
    }

    void setOnDarkChanged(Runnable onDarkChanged) {
        this.onDarkChanged = onDarkChanged;
    }

    /** Follows the timeline's Light / Dark switch. */
    void syncDark() {
        (chartSettings.isDarkPlot() ? darkButton : lightButton).setSelected(true);
        first.plot.setDark(chartSettings.isDarkPlot());
        if (second != null) {
            second.plot.setDark(chartSettings.isDarkPlot());
        }
    }

    private void recomputeAll() {
        first.recompute();
        if (second != null) {
            second.recompute();
        }
    }

    private void cursorsChanged() {
        boolean between = betweenCursorsButton.isSelected();
        betweenCursorsButton.setEnabled(cursors.isRange());
        if (!cursors.isRange() && between) {
            wholeLogButton.setSelected(true);
        }
        if (between) {
            recomputeAll();
        }
    }

    // ---- status ----

    void updateInfo() {
        updateInfo(active);
    }

    /** The status strip: the active plot's points, then a hovered point (either plot), its selection, or a hint. */
    void updateInfo(ScatterPane hoveredPane) {
        ScatterData data = active.plot.data();
        if (data == null) {
            return;
        }
        pointsLabel.setText(String.format(Locale.ROOT, "%,d points", data.count));
        hiddenLabel.setText(String.format(Locale.ROOT, "%,d hidden by filters", data.hiddenRecords));
        timeLabel.setText(HistogramView.duration(data.totalTime) + " of data"
                + (isBetweenCursors() ? String.format(Locale.ROOT, "  ·  between A and B, %.2f–%.2f s",
                cursors.start(), cursors.end()) : ""));
        ScatterPlot plot = active.plot;
        if (hoveredPane.plot.hovered() >= 0) {
            infoLabel.setText(hoveredPane.describe(hoveredPane.plot.hovered()));
        } else if (plot.box() != null) {
            double seconds = 0;
            for (int point : plot.selected()) {
                seconds += data.weight[point];
            }
            infoLabel.setText(String.format(Locale.ROOT, "Selected %,d points (%s)  ·  Enter or right-click: "
                    + "show on timeline", plot.selected().size(), HistogramView.duration(seconds)));
        } else if (plot.picked() >= 0) {
            infoLabel.setText(active.describe(plot.picked()) + "  ·  Enter: show on timeline");
        } else {
            infoLabel.setText("Drag a box to select · double-click a point for the timeline · wheel zooms");
        }
    }

    // ---- building ----

    private JComponent toolbar(JComponent viewSwitch, Consumer<Component> onOpen) {
        ThemedStrip bar = new ThemedStrip(new WrapLayout(FlowLayout.LEFT, JoinedButtons.ROW_GAP, 4),
                "Console.Toolbar.background", false);
        bar.setBorder(BorderFactory.createEmptyBorder(4, JoinedButtons.ROW_END_INSET - JoinedButtons.ROW_GAP, 5, 0));
        if (onOpen != null) {
            JButton open = LogViewerHost.openLogButton("Open  ▾");
            open.addActionListener(e -> onOpen.accept(open));
            bar.add(new JoinedButtons(open));
            bar.add(Box.createHorizontalStrut(JoinedButtons.ROW_GAP));
        }
        bar.add(viewSwitch);
        bar.add(Box.createHorizontalStrut(JoinedButtons.ROW_GAP));

        // right after the view switch, where the timeline's toolbar has it, so it stays put when switching views
        ButtonGroup background = new ButtonGroup();
        background.add(lightButton);
        background.add(darkButton);
        (chartSettings.isDarkPlot() ? darkButton : lightButton).setSelected(true);
        lightButton.setToolTipText("Plot area with a light background (the timeline's too)");
        darkButton.setToolTipText("Plot area with a dark background, dense clouds stand out (the timeline's too)");
        lightButton.addActionListener(e -> setDark(false));
        darkButton.addActionListener(e -> setDark(true));
        bar.add(ChartToolbar.labelled("Plot", new SegmentedControl(lightButton, darkButton)));
        bar.add(Box.createHorizontalStrut(JoinedButtons.ROW_GAP));

        ButtonGroup colourBy = new ButtonGroup();
        JToggleButton[] colourButtons = {densityButton, zButton, timeButton};
        ScatterPlot.ColourBy[] colourings = ScatterPlot.ColourBy.values();
        for (int i = 0; i < colourButtons.length; i++) {
            ScatterPlot.ColourBy colouring = colourings[i];
            colourBy.add(colourButtons[i]);
            colourButtons[i].setToolTipText(colouring.tooltip + " (the active plot)");
            colourButtons[i].addActionListener(e -> setColourBy(colouring));
        }
        bar.add(ChartToolbar.labelled("Colour by", new SegmentedControl(colourButtons)));
        bar.add(Box.createHorizontalStrut(JoinedButtons.ROW_GAP));

        ButtonGroup range = new ButtonGroup();
        range.add(wholeLogButton);
        range.add(betweenCursorsButton);
        wholeLogButton.setSelected(true);
        wholeLogButton.setToolTipText("Every record of the log");
        betweenCursorsButton.setEnabled(cursors.isRange());
        betweenCursorsButton.setToolTipText("Only the records between the timeline's A and B cursors (Measure)");
        wholeLogButton.addActionListener(e -> recomputeAll());
        betweenCursorsButton.addActionListener(e -> recomputeAll());
        bar.add(ChartToolbar.labelled("Range", new SegmentedControl(wholeLogButton, betweenCursorsButton)));
        bar.add(Box.createHorizontalStrut(JoinedButtons.ROW_GAP));

        recipesButton.setToolTipText("Ready-made plots for common questions: knock, lambda against target, "
                + "oil pressure, injector headroom, heat soak (the active plot)");
        recipesButton.addActionListener(e -> recipesMenu().show(recipesButton, 0, recipesButton.getHeight() + 4));
        bar.add(new JoinedButtons(recipesButton));
        bar.add(Box.createHorizontalStrut(JoinedButtons.ROW_GAP));
        colorsButton.setToolTipText("Palette, colour range and centre, kept for each Z signal; "
                + "also opened by a click on a legend");
        colorsButton.addActionListener(e -> {
            rebuildColorsPanel();
            colorsPopup.show(colorsButton, 0, colorsButton.getHeight() + 4);
        });
        bar.add(new JoinedButtons(colorsButton));
        bar.add(Box.createHorizontalStrut(JoinedButtons.ROW_GAP));

        ButtonGroup plots = new ButtonGroup();
        plots.add(onePlotButton);
        plots.add(twoPlotsButton);
        onePlotButton.setToolTipText("One plot");
        twoPlotsButton.setToolTipText("Two plots shown together, left and right, to compare: each has its own "
                + "signals, filters and colouring; the toolbar acts on the one last clicked (outlined)");
        onePlotButton.addActionListener(e -> setPlots(1));
        twoPlotsButton.addActionListener(e -> setPlots(2));
        bar.add(ChartToolbar.labelled("Layout", new SegmentedControl(onePlotButton, twoPlotsButton)));
        bar.add(Box.createHorizontalStrut(JoinedButtons.ROW_GAP));

        resetZoomButton.setToolTipText("Show every point of the active plot again (double-click an empty spot, or 0)");
        resetZoomButton.addActionListener(e -> active.plot.resetZoom());
        bar.add(new JoinedButtons(resetZoomButton));
        return bar;
    }

    private void setDark(boolean dark) {
        chartSettings.setDarkPlot(dark);
        syncDark();
        onDarkChanged.run();
    }

    private JComponent statusStrip() {
        ThemedStrip bar = new ThemedStrip(new BorderLayout(), "Console.StatusBar.background", true);
        bar.setBorder(BorderFactory.createEmptyBorder(7, 12, 7, 12));
        JPanel left = new JPanel(new FlowLayout(FlowLayout.LEFT, 14, 0));
        left.setOpaque(false);
        left.add(pointsLabel);
        left.add(hiddenLabel);
        left.add(timeLabel);
        bar.add(left, BorderLayout.CENTER);
        bar.add(infoLabel, BorderLayout.EAST);
        return bar;
    }
}
