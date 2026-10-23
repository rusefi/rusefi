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
import javax.swing.BoxLayout;
import javax.swing.ButtonGroup;
import javax.swing.JButton;
import javax.swing.JCheckBox;
import javax.swing.JComponent;
import javax.swing.JLabel;
import javax.swing.JMenu;
import javax.swing.JMenuItem;
import javax.swing.JRadioButtonMenuItem;
import javax.swing.JOptionPane;
import javax.swing.JPanel;
import javax.swing.JPopupMenu;
import javax.swing.JSeparator;
import javax.swing.JSpinner;
import javax.swing.JSplitPane;
import javax.swing.JToggleButton;
import javax.swing.KeyStroke;
import javax.swing.SpinnerNumberModel;
import java.awt.BasicStroke;
import java.awt.BorderLayout;
import java.awt.Color;
import java.awt.Component;
import java.awt.Dimension;
import java.awt.FlowLayout;
import java.awt.Font;
import java.awt.Graphics;
import java.awt.Graphics2D;
import java.awt.GridBagConstraints;
import java.awt.GridBagLayout;
import java.awt.GridLayout;
import java.awt.Insets;
import java.awt.RenderingHints;
import java.awt.event.InputEvent;
import java.awt.event.KeyEvent;
import java.awt.event.MouseAdapter;
import java.awt.event.MouseEvent;
import java.util.ArrayList;
import java.util.HashSet;
import java.util.List;
import java.util.Locale;
import java.util.Set;
import java.util.function.BiConsumer;
import java.util.function.BiFunction;
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

    final AxisSlot[] slots = new AxisSlot[3];
    final JButton swapButton = new JButton("Swap X ↔ Y");
    final JPanel banner = new Banner();
    final JLabel bannerText = new JLabel();
    final JLabel bannerDetail = ChartToolbar.muted(new JLabel());
    final JButton excludeButton = fullLabel("Exclude them");
    final JButton keepButton = fullLabel("Keep");
    final JPanel filterRow = new JPanel(new WrapLayout(FlowLayout.LEFT, 6, 4));
    final JButton addFilterButton = new JButton("+ Add filter");

    final JLabel recordsLabel = new JLabel();
    final JLabel hiddenLabel = ChartToolbar.muted(new JLabel());
    final JLabel timeLabel = ChartToolbar.muted(new JLabel());
    final JLabel selectedLabel = ChartToolbar.muted(new JLabel());

    private BiConsumer<String, List<double[]>> timelineAction = (cell, stays) -> {
    };

    /** Edits a filter: dialog title and the filter to start from (null for a new one); null when cancelled. */
    BiFunction<String, HistogramFilter, HistogramFilter> filterPrompt = this::promptFilter;

    private final int[] fields = new int[3];
    /** Per axis: picked by default rather than by the user, so not remembered. */
    private final boolean[] automatic = new boolean[3];
    final JLabel mixtureMissingLabel = new JLabel("No lambda or AFR signal in this log: pick a Z signal");
    final JLabel binsNoticeLabel = new JLabel();
    private final Supplier<IniFileModel> iniSource;
    private final Supplier<ConfigurationImage> tuneSource;
    /** Why the bins table is not used, or null. */
    private String binsProblem;
    private JPanel binsPanel;
    private char activeAxis = 'z';
    private final List<HistogramFilter> filters;
    /** Z signals whose saturation warning was answered with Keep, for this log. */
    private final Set<String> keptSaturation = new HashSet<>();
    private String saturationSignal;
    private HistogramSignals.Saturation saturation;

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
        filters = settings.filters();
        pickSignals();

        columnsSpinner = new JSpinner(new SpinnerNumberModel(settings.columns(), HistogramAxis.MIN_BINS,
                HistogramAxis.MAX_BINS, 1));
        rowsSpinner = new JSpinner(new SpinnerNumberModel(settings.rows(), HistogramAxis.MIN_BINS,
                HistogramAxis.MAX_BINS, 1));
        minTimeSpinner = new JSpinner(new SpinnerNumberModel(settings.minTime(), 0.0, HistogramSettings.MAX_MIN_TIME, 0.5));
        cellPanel = new HistogramCellPanel(() -> setCellPanelVisible(false));

        add(toolbar(viewSwitch, onOpen), BorderLayout.NORTH);
        JPanel main = new JPanel(new BorderLayout());
        main.setBackground(themed("Console.LogViewer.sidebar.background", Color.WHITE));
        main.add(controls(), BorderLayout.NORTH);
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
     * The signals the user picked where this log has them, otherwise RPM, MAP and the measured lambda (never a
     * target), or the first other signals.
     */
    private void pickSignals() {
        for (int i = 0; i < 3; i++) {
            int field = HistogramSignals.byName(log, settings.signal(AXES[i]));
            automatic[i] = field < 0;
            if (field < 0 && i < 2) {
                field = tableSignal(i);
            }
            if (field < 0) {
                field = i == 0 ? HistogramSignals.find(log, HistogramSignals.X_NAMES)
                        : i == 1 ? HistogramSignals.find(log, HistogramSignals.Y_NAMES)
                        : HistogramSignals.findMixture(log);
            }
            for (int j = 0; j < i; j++) {
                if (fields[j] == field) {
                    field = -1;
                }
            }
            fields[i] = field >= 0 ? field : HistogramSignals.fallbackExcept(log,
                    i == 2 ? HistogramSignals.Z_EXCLUDED : null, java.util.Arrays.copyOf(fields, i));
        }
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
        pickSignals();
        table.clearSelection();
        recompute();
        onSignalsChanged.run();
    }

    /** Z was picked automatically and is no lambda or AFR: the log has no measured mixture. */
    boolean isMixtureMissing() {
        return automatic[2] && HistogramTable.Mixture.of(log, fields[2], false) == HistogramTable.Mixture.NONE;
    }

    int field(char axis) {
        return fields[axisIndex(axis)];
    }

    char activeAxis() {
        return activeAxis;
    }

    /** The axis a field is on ('x', 'y', 'z'), or 0. */
    char axisOf(int field) {
        for (int i = 0; i < 3; i++) {
            if (fields[i] == field) {
                return AXES[i];
            }
        }
        return 0;
    }

    private static int axisIndex(char axis) {
        return axis == 'x' ? 0 : axis == 'y' ? 1 : 2;
    }

    void setActiveAxis(char axis) {
        activeAxis = axis;
        slots[axisIndex(axis)].setSelected(true);
    }

    /** A signal clicked in the sidebar goes on the highlighted axis. */
    void assignToActiveAxis(int field) {
        assign(activeAxis, field);
    }

    /** Puts a signal on an axis; a signal already on another axis trades places with it. */
    void assign(char axis, int field) {
        int index = axisIndex(axis);
        if (fields[index] == field) {
            return;
        }
        char other = axisOf(field);
        if (other != 0) {
            // trading places is the user's choice for both axes
            int otherIndex = axisIndex(other);
            fields[otherIndex] = fields[index];
            automatic[otherIndex] = false;
            settings.setSignal(other, log.name(fields[otherIndex]));
        }
        fields[index] = field;
        automatic[index] = false;
        settings.setSignal(axis, log.name(field));
        table.clearSelection();
        recompute();
        onSignalsChanged.run();
    }

    void swapXY() {
        assign('x', fields[1]);
    }

    // ---- filters ----

    List<HistogramFilter> filters() {
        return new ArrayList<>(filters);
    }

    void addFilter(HistogramFilter filter) {
        filters.add(filter);
        filtersChanged();
    }

    void replaceFilter(int index, HistogramFilter filter) {
        filters.set(index, filter);
        filtersChanged();
    }

    void removeFilter(int index) {
        filters.remove(index);
        filtersChanged();
    }

    void toggleFilter(int index) {
        filters.set(index, filters.get(index).withEnabled(!filters.get(index).enabled));
        filtersChanged();
    }

    private void filtersChanged() {
        settings.setFilters(filters);
        recompute();
    }

    private HistogramFilter promptFilter(String title, HistogramFilter initial) {
        HistogramFilterEditor editor = new HistogramFilterEditor(log, initial, field('x'), field('y'), field('z'));
        // typing goes straight to the signal search
        editor.search.addAncestorListener(new javax.swing.event.AncestorListener() {
            @Override
            public void ancestorAdded(javax.swing.event.AncestorEvent event) {
                editor.search.requestFocusInWindow();
            }

            @Override
            public void ancestorRemoved(javax.swing.event.AncestorEvent event) {
            }

            @Override
            public void ancestorMoved(javax.swing.event.AncestorEvent event) {
            }
        });
        while (JOptionPane.showConfirmDialog(this, editor, title, JOptionPane.OK_CANCEL_OPTION,
                JOptionPane.PLAIN_MESSAGE) == JOptionPane.OK_OPTION) {
            HistogramFilter filter = editor.filter();
            if (filter != null) {
                return filter;
            }
            JOptionPane.showMessageDialog(this, "The value must be a number.", title, JOptionPane.WARNING_MESSAGE);
        }
        return null;
    }

    void newFilter() {
        HistogramFilter filter = filterPrompt.apply("Add filter", null);
        if (filter != null) {
            addFilter(filter);
        }
    }

    void editFilter(int index) {
        HistogramFilter old = filters.get(index);
        HistogramFilter filter = filterPrompt.apply("Edit filter", old);
        if (filter != null) {
            replaceFilter(index, filter.withEnabled(old.enabled));
        }
    }

    /** The saturation warning is shown: Z has a pile of readings at a limit and no filter for it yet. */
    boolean isSaturationWarningShown() {
        return banner.isVisible();
    }

    void excludeSaturated() {
        if (saturation != null) {
            addFilter(saturation.filter(saturationSignal));
        }
    }

    void keepSaturated() {
        keptSaturation.add(saturationSignal);
        updateBanner();
    }

    private void updateBanner() {
        String zName = log.name(field('z'));
        if (!zName.equals(saturationSignal)) {
            saturationSignal = zName;
            saturation = HistogramSignals.saturation(log, field('z'));
        }
        boolean show = saturation != null && !keptSaturation.contains(zName)
                && filters.stream().noneMatch(f -> f.signal.equals(zName) && f.condition == saturation.filter(zName).condition
                && f.value == saturation.value);
        if (show) {
            bannerText.setText(String.format(Locale.ROOT, "%s reads exactly %s in %,d records (%.1f%%)", zName,
                    ValueFormat.format(log, field('z'), saturation.value), saturation.records, saturation.share * 100));
            bannerDetail.setText(String.format(Locale.ROOT, "That looks like the sensor's %s limit, not real readings",
                    saturation.low ? "lower" : "upper"));
            // a narrow window shortens the text, never the buttons: the whole message stays in the tooltip
            banner.setToolTipText(bannerText.getText() + ". " + bannerDetail.getText() + ".");
            bannerText.setToolTipText(banner.getToolTipText());
            bannerDetail.setToolTipText(banner.getToolTipText());
        }
        banner.setVisible(show);
    }

    // ---- computing ----

    boolean isBetweenCursors() {
        return betweenCursorsButton.isSelected() && cursors.isRange();
    }

    /** Bins the log again after a change of signals, filters, range, bins or how cells are shown. */
    void recompute() {
        boolean[] hidden = HistogramFilter.hidden(log, filters);
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
        for (int i = 0; i < 3; i++) {
            slots[i].setSignal(fields[i]);
        }
        mixtureMissingLabel.setVisible(isMixtureMissing());
        rebuildFilterRow();
        updateBanner();
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
                if (expected >= 0 && expected != fields[i]) {
                    notice = String.format(Locale.ROOT, "%s is %s, but the %s's %s axis is %s", AXES[i] == 'x' ? "X" : "Y",
                            log.name(fields[i]), tableBins.title, AXES[i] == 'x' ? "X" : "Y", log.name(expected));
                }
            }
        }
        binsNoticeLabel.setText(notice == null ? "" : notice);
        binsNoticeLabel.setVisible(notice != null);
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
        bar.add(ChartToolbar.muted(new JLabel("Show")));
        bar.add(new SegmentedControl(modeButtons));
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
        bar.add(ChartToolbar.muted(new JLabel("Units")));
        bar.add(new SegmentedControl(lambdaButton, afrButton));
        bar.add(Box.createHorizontalStrut(JoinedButtons.ROW_GAP));

        ButtonGroup range = new ButtonGroup();
        range.add(wholeLogButton);
        range.add(betweenCursorsButton);
        wholeLogButton.setSelected(true);
        wholeLogButton.setToolTipText("Every record of the log");
        wholeLogButton.addActionListener(e -> recompute());
        betweenCursorsButton.addActionListener(e -> recompute());
        bar.add(ChartToolbar.muted(new JLabel("Range")));
        bar.add(new SegmentedControl(wholeLogButton, betweenCursorsButton));
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

    /** Axes, the saturation warning and the filters, above the table. */
    private JComponent controls() {
        JPanel axes = new JPanel(new WrapLayout(FlowLayout.LEFT, 6, 4));
        axes.setOpaque(false);
        ButtonGroup group = new ButtonGroup();
        for (int i = 0; i < 3; i++) {
            char axis = AXES[i];
            AxisSlot slot = new AxisSlot(axis);
            slot.addActionListener(e -> activeAxis = axis);
            group.add(slot);
            slots[i] = slot;
            axes.add(new JoinedButtons(slot));
        }
        slots[2].setSelected(true);
        swapButton.setToolTipText("Swap the X and Y signals");
        swapButton.addActionListener(e -> swapXY());
        axes.add(new JoinedButtons(swapButton));
        axes.add(Box.createHorizontalStrut(6));
        mixtureMissingLabel.setForeground(themed("Console.Banner.warning.accent", new Color(0xb7791f)));
        mixtureMissingLabel.setFont(mixtureMissingLabel.getFont().deriveFont(Font.BOLD));
        mixtureMissingLabel.setVisible(false);
        axes.add(mixtureMissingLabel);
        binsNoticeLabel.setForeground(themed("Console.Banner.warning.accent", new Color(0xb7791f)));
        binsNoticeLabel.setFont(binsNoticeLabel.getFont().deriveFont(Font.BOLD));
        binsNoticeLabel.setVisible(false);
        axes.add(binsNoticeLabel);
        axes.add(ChartToolbar.muted(new JLabel("Click a signal in the list to put it on the highlighted axis, "
                + "or drag it onto an axis")));

        // the message on two short lines that may shorten; the buttons always keep their full labels
        banner.setLayout(new GridBagLayout());
        banner.setBorder(BorderFactory.createEmptyBorder(8, 12, 8, 8));
        banner.setVisible(false);
        bannerText.setForeground(themed("Label.foreground", Color.BLACK));
        bannerText.setFont(bannerText.getFont().deriveFont(Font.BOLD));
        JPanel message = new JPanel(new GridLayout(0, 1, 0, 2)) {
            @Override
            public Dimension getMinimumSize() {
                return new Dimension(0, getPreferredSize().height);
            }
        };
        message.setOpaque(false);
        message.add(bannerText);
        message.add(bannerDetail);
        excludeButton.putClientProperty(FlatClientProperties.STYLE_CLASS, "primary");
        excludeButton.setToolTipText("Add a filter that leaves these readings out");
        excludeButton.addActionListener(e -> excludeSaturated());
        keepButton.setToolTipText("Keep them and hide this warning");
        keepButton.addActionListener(e -> keepSaturated());
        GridBagConstraints text = new GridBagConstraints();
        text.weightx = 1;
        text.fill = GridBagConstraints.HORIZONTAL;
        text.anchor = GridBagConstraints.WEST;
        banner.add(message, text);
        GridBagConstraints button = new GridBagConstraints();
        button.insets = new Insets(0, 12, 0, 0);
        banner.add(excludeButton, button);
        button.insets = new Insets(0, 6, 0, 0);
        banner.add(keepButton, button);

        filterRow.setOpaque(false);
        addFilterButton.setToolTipText("Leave records out of the histogram by a signal's value or rate of change");
        addFilterButton.addActionListener(e -> newFilter());

        JPanel column = new JPanel();
        column.setLayout(new BoxLayout(column, BoxLayout.Y_AXIS));
        column.setOpaque(false);
        column.setBorder(BorderFactory.createEmptyBorder(10, 12, 4, 12));
        for (JComponent row : new JComponent[]{axes, banner, filterRow}) {
            row.setAlignmentX(Component.LEFT_ALIGNMENT);
            column.add(row);
            column.add(Box.createVerticalStrut(8));
        }
        return column;
    }

    private void rebuildFilterRow() {
        filterRow.removeAll();
        JLabel title = ChartToolbar.muted(new JLabel("FILTERS"));
        title.setFont(title.getFont().deriveFont(Font.BOLD, title.getFont().getSize2D() - 1));
        filterRow.add(title);
        for (int i = 0; i < filters.size(); i++) {
            filterRow.add(new FilterChip(i, filters.get(i)));
        }
        filterRow.add(new JoinedButtons(addFilterButton));
        filterRow.revalidate();
        filterRow.repaint();
    }

    private JComponent statusStrip() {
        ThemedStrip strip = new ThemedStrip(new BorderLayout(), "Console.StatusBar.background", true);
        strip.setBorder(BorderFactory.createEmptyBorder(7, 12, 7, 12));
        JPanel left = new JPanel(new FlowLayout(FlowLayout.LEFT, 14, 0));
        left.setOpaque(false);
        left.add(recordsLabel);
        left.add(hiddenLabel);
        left.add(timeLabel);
        strip.add(left, BorderLayout.CENTER);
        strip.add(selectedLabel, BorderLayout.EAST);
        return strip;
    }

    /** One axis: its letter and signal; selected when a sidebar click goes to it; takes dropped signals. */
    final class AxisSlot extends JToggleButton {
        final char axis;

        AxisSlot(char axis) {
            this.axis = axis;
            putClientProperty(FlatClientProperties.STYLE_CLASS, "viewToggle");
            setTransferHandler(new SignalTransfer.Handler() {
                @Override
                boolean canImportSignal(TransferSupport support) {
                    return true;
                }

                @Override
                boolean importSignal(TransferSupport support, int field) {
                    assign(axis, field);
                    setActiveAxis(axis);
                    return true;
                }
            });
        }

        void setSignal(int field) {
            String unit = log.unit(field);
            setText("<html><b>" + Character.toUpperCase(axis) + "</b>&nbsp;&nbsp;" + escape(log.name(field)) + "</html>");
            setToolTipText(log.name(field) + (unit.isEmpty() ? "" : " (" + unit + ")") + " on "
                    + Character.toUpperCase(axis) + " · highlighted: a click on a signal in the list puts it here; "
                    + "or drag a signal onto it");
        }
    }

    /** A button no layout shrinks below its preferred size, so its label is never cut to "…". */
    static JButton fullLabel(String text) {
        return new JButton(text) {
            @Override
            public Dimension getMinimumSize() {
                return getPreferredSize();
            }
        };
    }

    private static String escape(String text) {
        return text.replace("&", "&amp;").replace("<", "&lt;").replace(">", "&gt;");
    }

    /** A filter as a pill: click switches it on or off, right-click or double-click edits or removes it. */
    final class FilterChip extends JToggleButton {
        final int index;

        FilterChip(int index, HistogramFilter filter) {
            this.index = index;
            int hidden = filter.field(log) < 0 ? 0 : filter.count(log);
            String hold = filter.holdLabel();
            setText(filter.label() + (hold.isEmpty() ? "" : "  " + hold)
                    + (filter.field(log) < 0 ? "  · not in this log" : String.format(Locale.ROOT, "  −%,d", hidden)));
            setSelected(filter.enabled);
            setForeground(filter.enabled ? themed("Console.Banner.info.accent", new Color(0x1d4ed8))
                    : themed("Console.LogViewer.mutedForeground", Color.GRAY));
            setContentAreaFilled(false);
            setBorderPainted(false);
            setFocusPainted(false);
            setBorder(BorderFactory.createEmptyBorder(5, 24, 5, 12));
            setToolTipText("<html>" + escape(filter.toString()) + String.format(Locale.ROOT,
                    "<br>Hides %,d records on its own · click to switch %s · right-click to edit or remove</html>",
                    hidden, filter.enabled ? "off" : "on"));
            addActionListener(e -> toggleFilter(index));
            addMouseListener(new MouseAdapter() {
                @Override
                public void mousePressed(MouseEvent event) {
                    menu(event);
                }

                @Override
                public void mouseReleased(MouseEvent event) {
                    menu(event);
                }

                private void menu(MouseEvent event) {
                    if (event.isPopupTrigger()) {
                        chipMenu(index).show(FilterChip.this, event.getX(), event.getY());
                    }
                }
            });
        }

        @Override
        public Dimension getPreferredSize() {
            Dimension size = super.getPreferredSize();
            size.height = JoinedButtons.HEIGHT - 6;
            return size;
        }

        @Override
        protected void paintComponent(Graphics graphics) {
            Graphics2D g = (Graphics2D) graphics.create();
            try {
                g.setRenderingHint(RenderingHints.KEY_ANTIALIASING, RenderingHints.VALUE_ANTIALIAS_ON);
                boolean on = isSelected();
                int arc = getHeight();
                g.setColor(on ? themed("Console.Banner.info.background", new Color(0xeef4ff))
                        : themed("Console.Card.background", Color.WHITE));
                g.fillRoundRect(0, 0, getWidth() - 1, getHeight() - 1, arc, arc);
                g.setColor(on ? themed("Console.Banner.info.borderColor", new Color(0xc9dafb))
                        : themed("Separator.foreground", Color.LIGHT_GRAY));
                g.drawRoundRect(0, 0, getWidth() - 1, getHeight() - 1, arc, arc);
                Color accent = themed("Console.Banner.info.accent", new Color(0x1d4ed8));
                Color muted = themed("Console.LogViewer.mutedForeground", Color.GRAY);
                int d = 8;
                int cx = 11;
                int cy = (getHeight() - d) / 2;
                if (on) {
                    g.setColor(accent);
                    g.fillOval(cx, cy, d, d);
                } else {
                    g.setColor(muted);
                    g.setStroke(new BasicStroke(1.5f));
                    g.drawOval(cx, cy, d, d);
                }
            } finally {
                g.dispose();
            }
            super.paintComponent(graphics);
        }
    }

    JPopupMenu chipMenu(int index) {
        JPopupMenu menu = new JPopupMenu();
        JMenuItem toggle = new JMenuItem(filters.get(index).enabled ? "Switch off" : "Switch on");
        toggle.addActionListener(e -> toggleFilter(index));
        menu.add(toggle);
        JMenuItem edit = new JMenuItem("Edit…");
        edit.addActionListener(e -> editFilter(index));
        menu.add(edit);
        JMenuItem remove = new JMenuItem("Remove");
        remove.addActionListener(e -> removeFilter(index));
        menu.add(remove);
        return menu;
    }

    /** The warning strip's rounded background and border, in the theme's warning banner colours. */
    private static final class Banner extends JPanel {
        Banner() {
            setOpaque(false);
        }

        @Override
        public Dimension getMaximumSize() {
            return new Dimension(Integer.MAX_VALUE, getPreferredSize().height);
        }

        @Override
        protected void paintComponent(Graphics graphics) {
            Graphics2D g = (Graphics2D) graphics.create();
            try {
                g.setRenderingHint(RenderingHints.KEY_ANTIALIASING, RenderingHints.VALUE_ANTIALIAS_ON);
                g.setColor(themed("Console.Banner.warning.background", new Color(0xfff8e6)));
                g.fillRoundRect(0, 0, getWidth() - 1, getHeight() - 1, 10, 10);
                g.setColor(themed("Console.Banner.warning.borderColor", new Color(0xf3dca7)));
                g.drawRoundRect(0, 0, getWidth() - 1, getHeight() - 1, 10, 10);
            } finally {
                g.dispose();
            }
        }
    }
}
