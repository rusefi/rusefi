package com.rusefi.ui.logview;

import com.formdev.flatlaf.FlatClientProperties;
import com.opensr5.ConfigurationImage;
import com.opensr5.ini.IniFileModel;
import com.opensr5.ini.TableModel;
import com.rusefi.logview.CsvLogReader;
import com.rusefi.ui.widgets.JoinedButtons;
import com.rusefi.ui.util.WrapLayout;
import com.rusefi.ui.widgets.SegmentedControl;

import javax.swing.BorderFactory;
import javax.swing.Box;
import javax.swing.ButtonGroup;
import javax.swing.JButton;
import javax.swing.JCheckBox;
import javax.swing.JComponent;
import javax.swing.JLabel;
import javax.swing.JMenu;
import javax.swing.JMenuItem;
import javax.swing.JRadioButtonMenuItem;
import javax.swing.JPanel;
import javax.swing.JPopupMenu;
import javax.swing.JSeparator;
import javax.swing.JSpinner;
import javax.swing.JSplitPane;
import javax.swing.JToggleButton;
import javax.swing.KeyStroke;
import javax.swing.SpinnerNumberModel;
import java.awt.BorderLayout;
import java.awt.Color;
import java.awt.Component;
import java.awt.FlowLayout;
import java.awt.Graphics2D;
import java.awt.GridBagConstraints;
import java.awt.GridBagLayout;
import java.awt.Insets;
import java.awt.event.InputEvent;
import java.awt.event.KeyEvent;
import java.util.List;
import java.util.Locale;
import java.util.Set;
import java.util.function.BiConsumer;
import java.util.function.Consumer;
import java.util.function.Supplier;

import static com.rusefi.ui.logview.ThemedStrip.themed;

/**
 * The log viewer's Histogram view: a Z signal binned by X and Y signals (lambda by RPM and MAP by default) as a
 * coloured table, for seeing where the engine ran and how the mixture looked there before looking at the
 * timeline. Above the table: the X / Y / Z signals (a click on a signal in the sidebar puts it on the highlighted
 * axis, or drag one onto an axis), a warning when Z looks saturated, and the filters as pills that switch on and
 * off. To the right, the selected cell in detail. [tag:console_theme]
 */
final class HistogramView extends JPanel {
    static final char[] AXES = {'x', 'y', 'z'};

    private final CsvLogReader log;
    private final HistogramSettings settings;
    private final MeasureCursors cursors;
    private final Runnable onSignalsChanged;
    final HistogramTable table = new HistogramTable();
    final HistogramCellPanel cellPanel;
    private final JSplitPane cellSplit = new JSplitPane(JSplitPane.HORIZONTAL_SPLIT);
    private final JPanel tableArea = new JPanel(new BorderLayout());

    final JToggleButton[] modeButtons = new JToggleButton[HistogramSettings.Mode.values().length];
    final JToggleButton lambdaButton = SegmentedControl.segment("λ");
    final JToggleButton afrButton = SegmentedControl.segment("AFR");
    final JToggleButton wholeLogButton = SegmentedControl.segment("Whole log");
    final JToggleButton betweenCursorsButton = SegmentedControl.segment("A–B");
    final JButton binsButton = new JButton();
    final JButton colorsButton = new JButton();
    final JPopupMenu colorsPopup = new JPopupMenu();
    /** The colour settings shown in {@link #colorsPopup}; rebuilt as it opens and when its choices change. */
    HistogramColorsPanel colorsPanel;
    final JToggleButton cellButton = new JToggleButton("Cell");
    final JSpinner columnsSpinner;
    final JSpinner rowsSpinner;
    final JSpinner minTimeSpinner;
    final JCheckBox hideThinBox = new JCheckBox("Hide them");
    private final JPopupMenu binsPopup = new JPopupMenu();

    /** The axes, the saturation warning and the filters above the table. */
    final SignalStrip strip;
    // the strip's parts, named as the tests know them
    final SignalStrip.AxisSlot[] slots;
    final JButton swapButton;
    final JPanel banner;
    final JLabel bannerText;
    final JButton excludeButton;
    final JButton keepButton;
    final JPanel filterRow;
    final JButton addFilterButton;
    final JLabel mixtureMissingLabel;
    final JLabel binsNoticeLabel;

    final JLabel recordsLabel = new JLabel();
    final JLabel hiddenLabel = ChartToolbar.muted(new JLabel());
    final JLabel timeLabel = ChartToolbar.muted(new JLabel());
    final JLabel selectedLabel = ChartToolbar.muted(new JLabel());

    private BiConsumer<String, List<double[]>> timelineAction = (cell, stays) -> {
    };

    static final String MIXTURE_MISSING = "No lambda or AFR signal in this log: pick a Z signal";
    private final Supplier<IniFileModel> iniSource;
    private final Supplier<ConfigurationImage> tuneSource;
    /** Why the bins table is not used, or null. */
    private String binsProblem;
    private JPanel binsPanel;

    /**
     * @param cursors          the timeline's A and B cursors, for the A–B range
     * @param viewSwitch       the Timeline / Histogram switch, shown at the start of the toolbar
     * @param onOpen           action of the Open button, given the button, or null for no button
     * @param onSignalsChanged called when an axis signal changes, e.g. to mark them in the sidebar
     * @param ini              the ECU's .ini (or null): the tune tables and their axis signals
     * @param tune             the working tune (or null): the tables' axis values
     */
    HistogramView(CsvLogReader log, HistogramSettings settings, MeasureCursors cursors, JComponent viewSwitch,
                  Consumer<Component> onOpen, Runnable onSignalsChanged, Supplier<IniFileModel> ini,
                  Supplier<ConfigurationImage> tune) {
        super(new BorderLayout());
        this.log = log;
        this.settings = settings;
        this.cursors = cursors;
        this.onSignalsChanged = onSignalsChanged;
        this.iniSource = ini;
        this.tuneSource = tune;
        // a new signal starts the selection afresh
        strip = new SignalStrip(log, settings, this::automaticPick, "histogram", this::recompute, () -> {
            table.clearSelection();
            onSignalsChanged.run();
        });
        slots = strip.slots;
        swapButton = strip.swapButton;
        banner = strip.banner;
        bannerText = strip.bannerText;
        excludeButton = strip.excludeButton;
        keepButton = strip.keepButton;
        filterRow = strip.filterRow;
        addFilterButton = strip.addFilterButton;
        mixtureMissingLabel = strip.zNoticeLabel;
        binsNoticeLabel = strip.noticeLabel;

        columnsSpinner = new JSpinner(new SpinnerNumberModel(settings.columns(), HistogramAxis.MIN_BINS,
                HistogramAxis.MAX_BINS, 1));
        rowsSpinner = new JSpinner(new SpinnerNumberModel(settings.rows(), HistogramAxis.MIN_BINS,
                HistogramAxis.MAX_BINS, 1));
        minTimeSpinner = new JSpinner(new SpinnerNumberModel(settings.minTime(), 0.0, HistogramSettings.MAX_MIN_TIME, 0.5));
        cellPanel = new HistogramCellPanel(() -> setCellPanelVisible(false));

        add(toolbar(viewSwitch, onOpen), BorderLayout.NORTH);
        JPanel main = new JPanel(new BorderLayout());
        main.setBackground(themed("Console.LogViewer.sidebar.background", Color.WHITE));
        main.add(strip, BorderLayout.NORTH);
        table.setBorder(BorderFactory.createEmptyBorder());
        main.add(table, BorderLayout.CENTER);
        tableArea.add(main, BorderLayout.CENTER);
        cellSplit.setResizeWeight(1);
        cellSplit.setContinuousLayout(true);
        cellSplit.setDividerSize(5);
        cellSplit.setBorder(BorderFactory.createEmptyBorder());
        add(tableArea, BorderLayout.CENTER);
        add(statusStrip(), BorderLayout.SOUTH);

        // a click on the legend opens the colour settings just above it
        table.setLegendListener(bounds -> {
            rebuildColorsPanel();
            colorsPopup.show(table, bounds.x, bounds.y - colorsPopup.getPreferredSize().height - 4);
        });
        table.addSelectionListener(() -> {
            cellPanel.show(table);
            updateSelectedLabel();
        });
        table.setActivateListener(this::showSelectedOnTimeline);
        table.setCopyListener(selectedLabel::setText);
        table.setComponentPopupMenu(tableMenu());
        cellPanel.timelineButton.addActionListener(e -> showSelectedOnTimeline());
        cellPanel.copyButton.addActionListener(e -> table.copy(false));
        cursors.addListener(this::cursorsChanged);
        setCellPanelVisible(settings.isCellPanelVisible());
        recompute();
    }

    // ---- signals ----

    /**
     * The histogram's pick for an axis the user did not set: the bins table's (or VE table's) axis signals, else
     * RPM and MAP; for Z the measured lambda (never a target).
     */
    private int automaticPick(int axis) {
        int field = axis < 2 ? tableSignal(axis) : -1;
        if (field >= 0) {
            return field;
        }
        return axis == 0 ? HistogramSignals.find(log, HistogramSignals.X_NAMES)
                : axis == 1 ? HistogramSignals.find(log, HistogramSignals.Y_NAMES)
                : HistogramSignals.findMixture(log);
    }

    /**
     * The log signal behind the bins table's X (axis 0) or Y (axis 1), or, with even bins, behind the VE table's:
     * the load the fuel strategy looks up (MAP, TPS or airmass); -1 when the .ini or the log lacks it.
     */
    private int tableSignal(int axis) {
        IniFileModel model = ini();
        if (model == null) {
            return -1;
        }
        String name = settings.binsTable() != null ? settings.binsTable() : TuneTables.VE_TABLE;
        TableModel table = model.getTables().get(name);
        if (table == null) {
            return -1;
        }
        return TuneTables.logField(log, model, axis == 0 ? table.getXBinsChannel() : table.getYBinsChannel());
    }

    IniFileModel ini() {
        try {
            return iniSource.get();
        } catch (RuntimeException e) {
            // no .ini only means no tune tables
            return null;
        }
    }

    private ConfigurationImage tune() {
        try {
            return tuneSource.get();
        } catch (RuntimeException e) {
            return null;
        }
    }

    /** Takes a tune table's axes as the bins (null: even bins), with its axis signals where not picked by hand. */
    void setBinsTable(String tableName) {
        settings.setBinsTable(tableName);
        strip.pickSignals();
        table.clearSelection();
        recompute();
        onSignalsChanged.run();
    }

    /** Z was picked automatically and is no lambda or AFR: the log has no measured mixture. */
    boolean isMixtureMissing() {
        return strip.isAutomatic(2)
                && HistogramTable.Mixture.of(log, field('z'), false) == HistogramTable.Mixture.NONE;
    }

    // the strip's signals and filters, for the sidebar and the tests

    int field(char axis) {
        return strip.field(axis);
    }

    char activeAxis() {
        return strip.activeAxis();
    }

    char axisOf(int field) {
        return strip.axisOf(field);
    }

    void setActiveAxis(char axis) {
        strip.setActiveAxis(axis);
    }

    void assignToActiveAxis(int field) {
        strip.assignToActiveAxis(field);
    }

    void assign(char axis, int field) {
        strip.assign(axis, field);
    }

    void swapXY() {
        strip.swapXY();
    }

    List<HistogramFilter> filters() {
        return strip.filters();
    }

    void addFilter(HistogramFilter filter) {
        strip.addFilter(filter);
    }

    void removeFilter(int index) {
        strip.removeFilter(index);
    }

    void toggleFilter(int index) {
        strip.toggleFilter(index);
    }

    void editFilter(int index) {
        strip.editFilter(index);
    }

    boolean isSaturationWarningShown() {
        return strip.isSaturationWarningShown();
    }

    // ---- computing ----

    boolean isBetweenCursors() {
        return betweenCursorsButton.isSelected() && cursors.isRange();
    }

    /** Bins the log again after a change of signals, filters, range, bins or how cells are shown. */
    void recompute() {
        boolean[] hidden = HistogramFilter.hidden(log, strip.filters());
        double start = isBetweenCursors() ? cursors.start() : Double.NaN;
        double end = isBetweenCursors() ? cursors.end() : Double.NaN;
        TuneTables.Bins tableBins = null;
        binsProblem = null;
        if (settings.binsTable() != null) {
            try {
                tableBins = TuneTables.read(ini(), tune(), settings.binsTable());
            } catch (TuneTables.Problem problem) {
                // even bins until the tune is there
                binsProblem = problem.getMessage();
            }
        }
        Histogram histogram = tableBins != null
                ? Histogram.compute(log, field('x'), field('y'), field('z'), tableBins.x, tableBins.y, hidden, start, end)
                : Histogram.compute(log, field('x'), field('y'), field('z'), settings.columns(), settings.rows(),
                hidden, start, end);
        updateBinsNotice(tableBins);
        boolean mixture = HistogramTable.Mixture.of(log, field('z'), false) != HistogramTable.Mixture.NONE;
        lambdaButton.setEnabled(mixture);
        afrButton.setEnabled(mixture);
        HistogramColors colors = colors();
        table.set(histogram, settings.mode(), HistogramTable.Mixture.of(log, field('z'), settings.showAfr()),
                settings.minTime(), settings.hideThin(), colors);
        colorsButton.setText("Colours  ▾");
        strip.setZNotice(isMixtureMissing() ? MIXTURE_MISSING : null);
        strip.refresh();
        recordsLabel.setText(String.format(Locale.ROOT, "%,d records used", histogram.usedRecords));
        hiddenLabel.setText(String.format(Locale.ROOT, "%,d hidden by filters", histogram.hiddenRecords));
        timeLabel.setText(duration(histogram.totalTime) + " of data"
                + (isBetweenCursors() ? String.format(Locale.ROOT, "  ·  between A and B, %.2f–%.2f s",
                cursors.start(), cursors.end()) : ""));
        binsButton.setText(tableBins != null ? "Bins: " + TuneTables.describe(tableBins) + "  ▾"
                : String.format(Locale.ROOT, "Bins: %d × %d%s  ▾", settings.columns(), settings.rows(),
                binsProblem != null ? " (no tune)" : ""));
        binsButton.setToolTipText(binsProblem != null ? binsProblem
                : "Even bins or a tune table's axes, and the thin-data limit");
        columnsSpinner.setEnabled(tableBins == null);
        rowsSpinner.setEnabled(tableBins == null);
        cellPanel.show(table);
        updateSelectedLabel();
        revalidate();
        repaint();
    }

    /**
     * Says when the bins table cannot be used, or when X or Y is not the signal behind the table's axis, so the
     * bins and the values would be in different units (e.g. kPa bins over TPS).
     */
    private void updateBinsNotice(TuneTables.Bins tableBins) {
        String notice = null;
        if (binsProblem != null) {
            notice = binsProblem;
        } else if (tableBins != null) {
            String[] channels = {tableBins.xChannel, tableBins.yChannel};
            for (int i = 0; i < 2 && notice == null; i++) {
                int expected = TuneTables.logField(log, ini(), channels[i]);
                int actual = strip.field(SignalStrip.AXES[i]);
                if (expected >= 0 && expected != actual) {
                    String axis = i == 0 ? "X" : "Y";
                    notice = String.format(Locale.ROOT, "%s is %s, but the %s's %s axis is %s", axis,
                            log.name(actual), tableBins.title, axis, log.name(expected));
                }
            }
        }
        strip.setNotice(notice);
    }

    boolean isBinsNoticeShown() {
        return binsNoticeLabel.isVisible();
    }

    static String duration(double seconds) {
        if (seconds < 60) {
            return String.format(Locale.ROOT, "%.1f s", seconds);
        }
        long whole = Math.round(seconds);
        return whole >= 3600 ? String.format(Locale.ROOT, "%d h %d min", whole / 3600, whole % 3600 / 60)
                : String.format(Locale.ROOT, "%d min %d s", whole / 60, whole % 60);
    }

    private void updateSelectedLabel() {
        Histogram histogram = table.histogram();
        int x = table.selectedX();
        int[] block = table.block();
        if (block != null && (block[0] != block[2] || block[1] != block[3])) {
            selectedLabel.setText(String.format(Locale.ROOT, "Selected %d × %d cells  ·  Ctrl+C copies",
                    block[2] - block[0] + 1, block[3] - block[1] + 1));
            return;
        }
        Histogram.Cell cell = x < 0 ? null : histogram.cell(x, table.selectedY());
        if (x < 0) {
            selectedLabel.setText("Click a cell for its details  ·  arrows move, Enter shows it on the timeline");
            return;
        }
        if (cell == null) {
            selectedLabel.setText("Selected " + cellName(x, table.selectedY()) + "  —  no records");
            return;
        }
        selectedLabel.setText(String.format(Locale.ROOT, "Selected %s  —  mean %s · %.1f s",
                cellName(x, table.selectedY()), table.formatZ(cell.mean() * table.mixture().factor), cell.time));
    }

    /** Moving A or B rebins only while the histogram shows A–B; clearing them goes back to the whole log. */
    private void cursorsChanged() {
        boolean between = betweenCursorsButton.isSelected();
        updateRangeButton();
        if (!cursors.isRange() && between) {
            wholeLogButton.setSelected(true);
        }
        if (between) {
            recompute();
        }
    }

    private void updateRangeButton() {
        betweenCursorsButton.setEnabled(cursors.isRange());
        betweenCursorsButton.setToolTipText(cursors.isRange()
                ? String.format(Locale.ROOT, "Only the records between the timeline's A and B cursors (%.2f–%.2f s)",
                cursors.start(), cursors.end())
                : "Set the A and B cursors on the timeline (Measure) to bin only the time between them");
    }

    // ---- timeline and copy ----

    /** Receives a cell's name and its stays when they are to be shown on the timeline. */
    void setTimelineAction(BiConsumer<String, List<double[]>> timelineAction) {
        this.timelineAction = timelineAction;
    }

    /** "RPM 2509 · MAP 37.9": a cell by its bin centres. */
    String cellName(int x, int y) {
        Histogram histogram = table.histogram();
        return String.format(Locale.ROOT, "%s %s · %s %s", log.name(histogram.xField),
                ValueFormat.format(log, histogram.xField, histogram.xAxis.center(x)), log.name(histogram.yField),
                ValueFormat.format(log, histogram.yField, histogram.yAxis.center(y)));
    }

    /** Shows the selected cell's stays on the timeline (Enter, a double-click, or the cell panel's button). */
    void showSelectedOnTimeline() {
        int x = table.selectedX();
        int y = table.selectedY();
        if (x < 0) {
            return;
        }
        List<double[]> stays = table.histogram().visits(x, y);
        if (stays.isEmpty()) {
            selectedLabel.setText("No records in " + cellName(x, y));
            return;
        }
        timelineAction.accept(cellName(x, y), stays);
    }

    /** Right-click on the table. */
    private JPopupMenu tableMenu() {
        JPopupMenu menu = new JPopupMenu();
        JMenuItem timeline = new JMenuItem("Show on timeline");
        timeline.setAccelerator(KeyStroke.getKeyStroke(KeyEvent.VK_ENTER, 0));
        timeline.addActionListener(e -> showSelectedOnTimeline());
        JMenuItem copy = new JMenuItem("Copy values");
        copy.setAccelerator(KeyStroke.getKeyStroke(KeyEvent.VK_C, InputEvent.CTRL_DOWN_MASK));
        copy.setToolTipText("Tab-separated, ready to paste into a TunerStudio table");
        copy.addActionListener(e -> table.copy(false));
        JMenuItem copyAxes = new JMenuItem("Copy with axes");
        copyAxes.setAccelerator(KeyStroke.getKeyStroke(KeyEvent.VK_C, InputEvent.CTRL_DOWN_MASK | InputEvent.SHIFT_DOWN_MASK));
        copyAxes.setToolTipText("With the X values on top and the Y values on the left, e.g. for a spreadsheet");
        copyAxes.addActionListener(e -> table.copy(true));
        JMenuItem selectAll = new JMenuItem("Select all");
        selectAll.setAccelerator(KeyStroke.getKeyStroke(KeyEvent.VK_A, InputEvent.CTRL_DOWN_MASK));
        selectAll.addActionListener(e -> table.selectAll());
        menu.add(timeline);
        menu.addSeparator();
        menu.add(copy);
        menu.add(copyAxes);
        menu.add(selectAll);
        return menu;
    }

    // ---- colours ----

    /** The colour settings of the current Z signal. */
    HistogramColors colors() {
        return settings.colors(log.name(field('z')));
    }

    /** Opens the colour settings under the toolbar button or over the legend. */
    void showColors(Component invoker, int x, int y) {
        rebuildColorsPanel();
        colorsPopup.show(invoker, x, y);
    }

    /** Fills the popup with the current Z signal's settings, e.g. after a choice that shows or hides fields. */
    void rebuildColorsPanel() {
        colorsPanel = new HistogramColorsPanel(colors(), log.name(field('z')), table, () -> {
            recompute();
            if (colorsPopup.isVisible()) {
                rebuildColorsPanel();
            }
        }, this::recompute);
        colorsPopup.removeAll();
        colorsPopup.add(colorsPanel);
        colorsPopup.pack();
    }

    // ---- cell panel ----

    boolean isCellPanelVisible() {
        return cellPanel.getParent() == cellSplit;
    }

    /** Shows the cell panel to the right of the table (resizable), or the table alone. */
    void setCellPanelVisible(boolean visible) {
        cellButton.setSelected(visible);
        settings.setCellPanelVisible(visible);
        if (visible == isCellPanelVisible()) {
            return;
        }
        Component main = visible ? tableArea.getComponent(0) : cellSplit.getLeftComponent();
        tableArea.removeAll();
        if (visible) {
            cellSplit.setLeftComponent(main);
            cellSplit.setRightComponent(cellPanel);
            cellPanel.show(table);
            tableArea.add(cellSplit, BorderLayout.CENTER);
        } else {
            cellSplit.remove(main);
            cellSplit.remove(cellPanel);
            tableArea.add(main, BorderLayout.CENTER);
        }
        tableArea.revalidate();
        tableArea.repaint();
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

        ButtonGroup modes = new ButtonGroup();
        for (HistogramSettings.Mode mode : HistogramSettings.Mode.values()) {
            JToggleButton button = SegmentedControl.segment(mode.text);
            button.setToolTipText(mode.tooltip);
            button.setSelected(mode == settings.mode());
            button.addActionListener(e -> {
                settings.setMode(mode);
                recompute();
            });
            modes.add(button);
            modeButtons[mode.ordinal()] = button;
        }
        bar.add(ChartToolbar.labelled("Show", new SegmentedControl(modeButtons)));
        bar.add(Box.createHorizontalStrut(JoinedButtons.ROW_GAP));

        ButtonGroup units = new ButtonGroup();
        units.add(lambdaButton);
        units.add(afrButton);
        (settings.showAfr() ? afrButton : lambdaButton).setSelected(true);
        lambdaButton.setToolTipText("Show lambda or AFR signals as lambda");
        afrButton.setToolTipText("Show lambda or AFR signals as AFR (stoichiometric "
                + HistogramFilter.number(HistogramSignals.STOICH_AFR) + ")");
        lambdaButton.addActionListener(e -> {
            settings.setShowAfr(false);
            recompute();
        });
        afrButton.addActionListener(e -> {
            settings.setShowAfr(true);
            recompute();
        });
        bar.add(ChartToolbar.labelled("Units", new SegmentedControl(lambdaButton, afrButton)));
        bar.add(Box.createHorizontalStrut(JoinedButtons.ROW_GAP));

        ButtonGroup range = new ButtonGroup();
        range.add(wholeLogButton);
        range.add(betweenCursorsButton);
        wholeLogButton.setSelected(true);
        wholeLogButton.setToolTipText("Every record of the log");
        wholeLogButton.addActionListener(e -> recompute());
        betweenCursorsButton.addActionListener(e -> recompute());
        bar.add(ChartToolbar.labelled("Range", new SegmentedControl(wholeLogButton, betweenCursorsButton)));
        bar.add(Box.createHorizontalStrut(JoinedButtons.ROW_GAP));
        updateRangeButton();

        buildBinsPopup();
        binsButton.setToolTipText("Number of bins and the thin-data limit");
        binsButton.addActionListener(e -> {
            rebuildBinsPopup();
            binsPopup.show(binsButton, 0, binsButton.getHeight() + 4);
        });
        bar.add(new JoinedButtons(binsButton));
        bar.add(Box.createHorizontalStrut(JoinedButtons.ROW_GAP));
        colorsButton.setToolTipText("Palette, colour range and centre, kept for each Z signal; "
                + "also opened by a click on the legend");
        colorsButton.addActionListener(e -> showColors(colorsButton, 0, colorsButton.getHeight() + 4));
        bar.add(new JoinedButtons(colorsButton));
        bar.add(Box.createHorizontalStrut(JoinedButtons.ROW_GAP));
        // a light accent tint when on, like Stats on the timeline [tag:console_theme]
        cellButton.putClientProperty(FlatClientProperties.STYLE_CLASS, "viewToggle");
        cellButton.setToolTipText("Show the selected cell's statistics, readings and visits");
        cellButton.addActionListener(e -> setCellPanelVisible(cellButton.isSelected()));
        bar.add(new JoinedButtons(cellButton));
        return bar;
    }

    private void buildBinsPopup() {
        JPanel panel = new JPanel(new GridBagLayout());
        panel.setBorder(BorderFactory.createEmptyBorder(10, 12, 10, 12));
        columnsSpinner.setToolTipText("Bins along X (" + HistogramAxis.MIN_BINS + "–" + HistogramAxis.MAX_BINS + ")");
        rowsSpinner.setToolTipText("Bins along Y (" + HistogramAxis.MIN_BINS + "–" + HistogramAxis.MAX_BINS + ")");
        minTimeSpinner.setToolTipText("Cells with less time than this are hatched: too little data to trust");
        hideThinBox.setSelected(settings.hideThin());
        hideThinBox.setToolTipText("Leave cells under the limit empty instead of hatching them");
        columnsSpinner.addChangeListener(e -> {
            settings.setColumns((Integer) columnsSpinner.getValue());
            table.clearSelection();
            recompute();
        });
        rowsSpinner.addChangeListener(e -> {
            settings.setRows((Integer) rowsSpinner.getValue());
            table.clearSelection();
            recompute();
        });
        minTimeSpinner.addChangeListener(e -> {
            settings.setMinTime((Double) minTimeSpinner.getValue());
            recompute();
        });
        hideThinBox.addActionListener(e -> {
            settings.setHideThin(hideThinBox.isSelected());
            recompute();
        });
        int row = 0;
        addRow(panel, ChartToolbar.muted(new JLabel("Columns (X)")), columnsSpinner, row++);
        addRow(panel, ChartToolbar.muted(new JLabel("Rows (Y)")), rowsSpinner, row++);
        GridBagConstraints separator = new GridBagConstraints();
        separator.gridy = row++;
        separator.gridwidth = 2;
        separator.fill = GridBagConstraints.HORIZONTAL;
        separator.insets = new Insets(8, 0, 8, 0);
        panel.add(new JSeparator(), separator);
        JPanel minTime = new JPanel(new FlowLayout(FlowLayout.LEFT, 6, 0));
        minTime.setOpaque(false);
        minTime.add(minTimeSpinner);
        minTime.add(ChartToolbar.muted(new JLabel("s")));
        addRow(panel, ChartToolbar.muted(new JLabel("Thin data under")), minTime, row++);
        addRow(panel, hideThinBox, null, row++);
        binsPanel = panel;
        rebuildBinsPopup();
    }

    /**
     * The bins options, then where the bins come from: even over the log, or a tune table's axes (the main
     * tables first, the rest under Other tables). Rebuilt as it opens, since the .ini and tune can change.
     */
    void rebuildBinsPopup() {
        binsPopup.removeAll();
        binsPopup.add(binsPanel);
        binsPopup.addSeparator();
        JMenuItem heading = new JMenuItem("Bins from");
        heading.setEnabled(false);
        binsPopup.add(heading);
        String current = settings.binsTable();
        JRadioButtonMenuItem even = new JRadioButtonMenuItem(String.format(Locale.ROOT,
                "Even over the log (%d × %d)", settings.columns(), settings.rows()), current == null);
        even.setToolTipText("Columns and rows evenly spaced over the range of the X and Y signals");
        even.addActionListener(e -> setBinsTable(null));
        binsPopup.add(even);
        IniFileModel model = ini();
        boolean tuneReady = model != null && tune() != null;
        List<TableModel> tables = TuneTables.tables(model);
        JMenu others = new JMenu("Other tables");
        for (TableModel tableModel : tables) {
            String name = tableModel.getTableId();
            JRadioButtonMenuItem item = new JRadioButtonMenuItem(TuneTables.title(tableModel), name.equals(current));
            item.setEnabled(tuneReady);
            item.setToolTipText(tuneReady
                    ? "Bin by this table's X and Y axes from the tune, so cells line up with the table's"
                    : "Connect to the ECU or open a tune to use its table axes");
            item.addActionListener(e -> setBinsTable(name));
            if (java.util.Arrays.asList(TuneTables.PREFERRED).contains(name)) {
                binsPopup.add(item);
            } else {
                others.add(item);
            }
        }
        if (others.getItemCount() > 0) {
            binsPopup.add(others);
        }
        if (tables.isEmpty()) {
            JMenuItem none = new JMenuItem("Tune tables: connect to the ECU or open a tune");
            none.setEnabled(false);
            binsPopup.add(none);
        }
        binsPopup.pack();
    }

    private static void addRow(JPanel panel, Component label, Component control, int row) {
        GridBagConstraints left = new GridBagConstraints();
        left.gridx = 0;
        left.gridy = row;
        left.gridwidth = control == null ? 2 : 1;
        left.anchor = GridBagConstraints.WEST;
        left.insets = new Insets(3, 0, 3, 0);
        panel.add(label, left);
        if (control != null) {
            GridBagConstraints right = new GridBagConstraints();
            right.gridx = 1;
            right.gridy = row;
            right.anchor = GridBagConstraints.WEST;
            right.insets = new Insets(3, 12, 3, 0);
            panel.add(control, right);
        }
    }

    private JComponent statusStrip() {
        ThemedStrip bar = new ThemedStrip(new BorderLayout(), "Console.StatusBar.background", true);
        bar.setBorder(BorderFactory.createEmptyBorder(7, 12, 7, 12));
        JPanel left = new JPanel(new FlowLayout(FlowLayout.LEFT, 14, 0));
        left.setOpaque(false);
        left.add(recordsLabel);
        left.add(hiddenLabel);
        left.add(timeLabel);
        bar.add(left, BorderLayout.CENTER);
        bar.add(selectedLabel, BorderLayout.EAST);
        return bar;
    }
}
