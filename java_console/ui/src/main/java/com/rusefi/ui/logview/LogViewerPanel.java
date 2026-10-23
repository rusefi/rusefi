package com.rusefi.ui.logview;

import com.formdev.flatlaf.FlatClientProperties;
import com.formdev.flatlaf.icons.FlatSearchIcon;
import com.opensr5.ConfigurationImage;
import com.opensr5.ini.IniFileModel;
import com.rusefi.logview.CsvLogReader;
import com.rusefi.ui.widgets.SegmentedControl;

import javax.swing.AbstractAction;
import javax.swing.BorderFactory;
import javax.swing.Box;
import javax.swing.ButtonGroup;
import javax.swing.JToggleButton;
import javax.swing.DefaultListModel;
import javax.swing.JCheckBox;
import javax.swing.JCheckBoxMenuItem;
import javax.swing.JMenu;
import javax.swing.JMenuItem;
import javax.swing.JOptionPane;
import javax.swing.JPopupMenu;
import javax.swing.SwingUtilities;
import javax.swing.DropMode;
import javax.swing.Icon;
import javax.swing.JComponent;
import javax.swing.JLabel;
import javax.swing.JList;
import javax.swing.JPanel;
import javax.swing.JScrollPane;
import javax.swing.JSplitPane;
import javax.swing.JTextField;
import javax.swing.JViewport;
import javax.swing.KeyStroke;
import javax.swing.ListCellRenderer;
import javax.swing.SwingConstants;
import javax.swing.event.DocumentEvent;
import javax.swing.event.DocumentListener;
import java.awt.BasicStroke;
import java.awt.BorderLayout;
import java.awt.CardLayout;
import java.awt.Color;
import java.awt.Component;
import java.awt.Dimension;
import java.awt.FlowLayout;
import java.awt.Font;
import java.awt.Graphics;
import java.awt.Graphics2D;
import java.awt.GridLayout;
import java.awt.GraphicsEnvironment;
import java.awt.Point;
import java.awt.Rectangle;
import java.awt.RenderingHints;
import java.awt.datatransfer.Transferable;
import java.awt.dnd.DragSource;
import java.awt.event.ActionEvent;
import java.awt.event.InputEvent;
import java.awt.event.KeyEvent;
import java.awt.event.MouseAdapter;
import java.awt.event.MouseEvent;
import java.util.ArrayList;
import java.util.List;
import java.util.Locale;
import java.util.Map;
import java.util.Objects;
import java.util.function.BiFunction;
import java.util.function.Consumer;
import java.util.function.Predicate;
import java.util.function.Supplier;

import static com.rusefi.config.generated.VariableRegistryValues.GAUGE_NAME_RPM;
import static com.rusefi.config.generated.VariableRegistryValues.GAUGE_NAME_TPS;
import static com.rusefi.ui.logview.ThemedStrip.themed;

/**
 * Log viewer: a searchable signal list on the left, in collapsible sections (see {@link SignalSections};
 * click or Space adds a signal to the active plot,
 * Shift+click or Shift+Space to a new plot, or drag it onto a plot; a dot shows its trace colour), the
 * stacked plots with their toolbar on the right, and a status strip below. [tag:console_theme]
 */
public final class LogViewerPanel extends JPanel {
    private static final int SIDEBAR_WIDTH = 280;
    /** Width at the right end of a list heading that opens its menu (the "⋯"). */
    private static final int MENU_HOT_WIDTH = 28;
    /** Pointer movement in pixels that still counts as a click; see {@link #clickSlop}. */
    private static final int CLICK_SLOP = 5;
    /** Signals plotted when a log opens, matched by name, those the log has. */
    private static final String[] DEFAULT_PLOTTED = {GAUGE_NAME_RPM, GAUGE_NAME_TPS};
    /** Row a signal is being dragged from in this list, or -1. */
    private int listDragOrigin = -1;

    private final CsvLogReader log;
    private final PlotLayout layout;
    private final ChartSettings settings;
    private final DefaultListModel<SignalSections.Row> rows = new DefaultListModel<>();
    private final JList<SignalSections.Row> fieldList = new JList<SignalSections.Row>(rows) {
        @Override
        public String getToolTipText(MouseEvent event) {
            int index = locationToIndex(event.getPoint());
            if (index < 0 || !getCellBounds(index, index).contains(event.getPoint())) {
                return null;
            }
            SignalSections.Row row = rows.get(index);
            if (row.isHeading()) {
                String toggle = row.expanded ? "Click to collapse" : "Click to expand";
                if (row.kind == SignalSections.Kind.PLOTTED) {
                    return toggle + " · drop a signal here to move it to " + row.section.toLowerCase(Locale.ROOT);
                }
                return row.kind == SignalSections.Kind.LIST
                        ? toggle + " · ⋯ or right-click: list actions · drop a signal here to add it" : toggle;
            }
            if (row.isMissing()) {
                return row.label + " is not in this log · right-click to remove it from " + row.section;
            }
            int field = row.field;
            String unit = log.unit(field);
            String name = log.name(field) + (unit.isEmpty() ? "" : " (" + unit + ")")
                    + (SignalSections.isZero(log, field) ? " · always 0 in this log"
                    : SignalSections.isConstant(log, field) ? " · constant in this log" : "");
            SignalStrip strip = activeStrip();
            if (strip != null) {
                char axis = strip.axisOf(field);
                String put = " · click to put it on " + Character.toUpperCase(strip.activeAxis())
                        + ", or drag it onto an axis";
                return axis != 0 ? name + " · on " + Character.toUpperCase(axis) + put : name + put;
            }
            if (layout.isPlotted(field)) {
                return name + " · in " + plotName(layout.plotOf(field)) + " · click to remove · right-click: lists";
            }
            return name + " · click to add to " + plotName(layout.activePlot())
                    + " · Shift+click: new plot · right-click: lists";
        }

        // Always fit the sidebar: a long name is shortened with "…" instead of pushing units under the scrollbar.
        @Override
        public boolean getScrollableTracksViewportWidth() {
            return true;
        }
    };
    private final JTextField search = new JTextField();
    private final SignalCategories categories;
    /** The ECU's .ini when the log opened, or null: the histogram's tune tables. */
    private final IniFileModel ini;
    /** The working tune, for tune tables' axis values; null without one. */
    private Supplier<ConfigurationImage> tuneSupplier = () -> null;
    private final SignalLists lists;
    /** Asks for a list name (dialog title, initial text); null when cancelled. Replaced in tests. */
    BiFunction<String, String, String> namePrompt = (title, initial) ->
            (String) JOptionPane.showInputDialog(this, "List name:", title, JOptionPane.PLAIN_MESSAGE, null, null,
                    initial);
    /** Asks to go ahead with the given question. Replaced in tests. */
    Predicate<String> confirm = question -> JOptionPane.showConfirmDialog(this, question, "Signal lists",
            JOptionPane.OK_CANCEL_OPTION) == JOptionPane.OK_OPTION;
    Consumer<String> messages = message -> JOptionPane.showMessageDialog(this, message, "Signal lists",
            JOptionPane.WARNING_MESSAGE);
    private final SidebarState state;
    final JCheckBox hideZeroBox = new JCheckBox();
    final JCheckBox hideConstantBox = new JCheckBox();
    private final JLabel plottedLabel = ChartToolbar.muted(new JLabel());
    private final Chip countChip = new Chip("");
    private final LogChart chart;
    private final LogStatusBar statusBar;
    private final ChartToolbar toolbar;
    private final StatsPanel statsPanel;
    private final JPanel plotArea = new JPanel(new BorderLayout());
    /** Chart and Stats panel side by side while the Stats panel is shown. */
    private final JSplitPane statsSplit = new JSplitPane(JSplitPane.HORIZONTAL_SPLIT);
    /** The timeline (toolbar, plots, Stats), the histogram or the scatter plot, right of the sidebar. */
    private final JPanel views = new JPanel(new CardLayout());
    private static final String TIMELINE = HistogramSettings.TIMELINE;
    private static final String HISTOGRAM = HistogramSettings.HISTOGRAM;
    private static final String SCATTER = HistogramSettings.SCATTER;
    private final Consumer<Component> onOpen;
    /** The histogram's options, and which view was shown last. */
    private HistogramSettings histogramSettings = new HistogramSettings();
    private HistogramSettings scatterSettings = new HistogramSettings();
    /** Built the first time each view is shown. */
    private HistogramView histogram;
    private ScatterView scatter;
    private String shownView = TIMELINE;
    /** The view "Back" returns to after Show on timeline. */
    private String returnView = HISTOGRAM;
    /** Timeline / Histogram / Scatter switches: one in each view's toolbar. */
    private final JToggleButton[] timelineSegments = new JToggleButton[3];
    private final JToggleButton[] histogramSegments = new JToggleButton[3];
    private final JToggleButton[] scatterSegments = new JToggleButton[3];
    /** A histogram cell's stays, above the plots, after "Show on timeline". */
    private final VisitsBar visitsBar;

    public LogViewerPanel(CsvLogReader log) {
        this(log, null);
    }

    public LogViewerPanel(CsvLogReader log, String fileName) {
        this(log, fileName, new ChartSettings(), new SignalLists(), null);
    }

    LogViewerPanel(CsvLogReader log, String fileName, ChartSettings settings, SignalLists lists,
                   Consumer<Component> onOpen) {
        this(log, fileName, settings, lists, new SidebarState(), null, onOpen);
    }

    LogViewerPanel(CsvLogReader log, String fileName, ChartSettings settings, SignalLists lists,
                   SidebarState state, IniFileModel ini, Consumer<Component> onOpen) {
        this(log, fileName, settings, lists, state, new SignalColors(), ini, onOpen);
    }

    /**
     * @param settings chart options, shared so they carry over when another log is opened
     * @param lists    Common and the user's signal lists, shared by every log
     * @param state    open sections and the Hide switches, shared by every log
     * @param colors   the colours the user chose for signals, shared by every log
     * @param ini      the ECU's .ini for grouping the signals, or null to group them by name
     * @param onOpen   action of the toolbar's Open button, given the button (e.g. to show a menu under it),
     *                 or null for no button
     */
    LogViewerPanel(CsvLogReader log, String fileName, ChartSettings settings, SignalLists lists,
                   SidebarState state, SignalColors colors, IniFileModel ini, Consumer<Component> onOpen) {
        super(new BorderLayout());
        this.log = log;
        this.settings = settings;
        this.lists = lists;
        this.state = state;
        this.onOpen = onOpen;
        // a log opens with RPM and TPS plotted when it has them; the user picks the rest
        layout = PlotLayout.single(log.fieldCount(), defaultPlotted(log));
        categories = SignalCategories.of(log, ini);
        this.ini = ini;
        chart = new LogChart(log, layout, settings);
        chart.setSignalColors(colors);
        layout.addListener(() -> {
            // the plot sections follow the plots
            filter(search.getText());
            updatePlottedLabel();
        });
        statusBar = new LogStatusBar(fileName, log);
        chart.addViewListener(() -> statusBar.update(chart.timeWindow(), chart.cursorTime()));

        statsPanel = new StatsPanel(chart, () -> setStatsVisible(false));
        toolbar = new ChartToolbar(settings, () -> {
            chart.repaint();
            // the list and table dots use the plot palette, which follows the Light / Dark switch
            fieldList.repaint();
            statsPanel.table.repaint();
        }, onOpen);
        layout.addListener(statsPanel::refresh);
        chart.addColorListener(() -> {
            fieldList.repaint();
            statsPanel.table.repaint();
        });
        chart.addViewListener(statsPanel::refreshValues);
        toolbar.statsButton.addActionListener(e -> setStatsVisible(toolbar.statsButton.isSelected()));
        toolbar.measureButton.addActionListener(e -> chart.setMeasuring(toolbar.measureButton.isSelected()));
        // the A / B keys turn Measure on from the plots
        chart.addMeasuringListener(() -> toolbar.measureButton.setSelected(chart.isMeasuring()));
        statsSplit.setResizeWeight(1);
        statsSplit.setContinuousLayout(true);
        statsSplit.setDividerSize(5);
        statsSplit.setBorder(BorderFactory.createEmptyBorder());
        visitsBar = new VisitsBar(chart, () -> setView(returnView));
        JPanel top = new JPanel(new BorderLayout());
        top.add(toolbar, BorderLayout.NORTH);
        top.add(visitsBar, BorderLayout.SOUTH);
        plotArea.add(top, BorderLayout.NORTH);
        plotArea.add(chart, BorderLayout.CENTER);
        toolbar.addViewSwitch(viewSwitch(timelineSegments));
        views.add(plotArea, TIMELINE);

        JSplitPane split = new JSplitPane(JSplitPane.HORIZONTAL_SPLIT, createSidebar(), views);
        split.setResizeWeight(0);
        split.setContinuousLayout(true);
        split.setDividerSize(5);
        split.setDividerLocation(SIDEBAR_WIDTH);
        split.setBorder(BorderFactory.createEmptyBorder());
        add(split, BorderLayout.CENTER);
        add(statusBar, BorderLayout.SOUTH);
        setPreferredSize(new Dimension(1400, 840));
        statusBar.update(chart.timeWindow(), chart.cursorTime());
        updatePlottedLabel();
    }

    private JComponent createSidebar() {
        JPanel sidebar = new JPanel(new BorderLayout(0, 10));
        sidebar.setBackground(themed("Console.LogViewer.sidebar.background", Color.WHITE));
        sidebar.setBorder(BorderFactory.createEmptyBorder(12, 12, 0, 8));

        JLabel title = new JLabel("Signals");
        title.setFont(title.getFont().deriveFont(Font.BOLD));
        JPanel titleRow = new JPanel(new BorderLayout(8, 0));
        titleRow.setOpaque(false);
        JPanel titleLeft = new JPanel(new FlowLayout(FlowLayout.LEFT, 0, 0));
        titleLeft.setOpaque(false);
        titleLeft.add(title);
        titleLeft.add(Box.createHorizontalStrut(8));
        titleLeft.add(countChip);
        titleRow.add(titleLeft, BorderLayout.WEST);
        titleRow.add(plottedLabel, BorderLayout.EAST);

        search.putClientProperty(FlatClientProperties.PLACEHOLDER_TEXT, "Search signals");
        search.putClientProperty(FlatClientProperties.TEXT_FIELD_LEADING_ICON, new FlatSearchIcon());
        search.putClientProperty(FlatClientProperties.TEXT_FIELD_SHOW_CLEAR_BUTTON, true);
        search.getDocument().addDocumentListener(new DocumentListener() {
            @Override public void insertUpdate(DocumentEvent event) { filter(search.getText()); }
            @Override public void removeUpdate(DocumentEvent event) { filter(search.getText()); }
            @Override public void changedUpdate(DocumentEvent event) { filter(search.getText()); }
        });

        String hiddenWhere = "<br>also while searching; they stay in the plots and the lists</html>";
        setUpFilterBox(hideZeroBox, "Hide zero signals", SignalSections.zeroCount(log), state.hideZero(),
                "<html>Leave signals that are 0 for the whole log (mostly unused features) out of the groups;"
                        + hiddenWhere, state::setHideZero);
        setUpFilterBox(hideConstantBox, "Hide constant signals", SignalSections.constantCount(log),
                state.hideConstant(),
                "<html>Leave signals that hold one other value for the whole log out of the groups.<br>"
                        + "These can matter: an active error, the detected gear, the firmware version;"
                        + hiddenWhere, state::setHideConstant);
        JPanel filterBoxes = new JPanel(new GridLayout(0, 1, 0, 2));
        filterBoxes.setOpaque(false);
        filterBoxes.add(hideZeroBox);
        filterBoxes.add(hideConstantBox);
        updateCount();

        JPanel heading = new JPanel(new BorderLayout(0, 8));
        heading.setOpaque(false);
        heading.add(titleRow, BorderLayout.NORTH);
        heading.add(search, BorderLayout.CENTER);
        heading.add(filterBoxes, BorderLayout.SOUTH);
        sidebar.add(heading, BorderLayout.NORTH);

        // setDragEnabled throws HeadlessException without a display (CI); there is nothing to drag there anyway.
        if (!GraphicsEnvironment.isHeadless()) {
            fieldList.setDragEnabled(true);
        }
        fieldList.setDropMode(DropMode.ON);
        fieldList.setTransferHandler(new ListTransferHandler());
        fieldList.setCellRenderer(new SignalRenderer());
        fieldList.setFixedCellHeight(28);
        fieldList.setToolTipText("");
        fieldList.setBackground(sidebar.getBackground());
        fieldList.addMouseListener(rowMouse);
        fieldList.getInputMap().put(KeyStroke.getKeyStroke(KeyEvent.VK_SPACE, 0), "togglePlot");
        fieldList.getInputMap().put(KeyStroke.getKeyStroke(KeyEvent.VK_ENTER, 0), "togglePlot");
        fieldList.getActionMap().put("togglePlot", new AbstractAction() {
            @Override
            public void actionPerformed(ActionEvent event) {
                SignalSections.Row row = fieldList.getSelectedValue();
                if (row == null) {
                    return;
                }
                if (row.isHeading()) {
                    toggleSection(row.section);
                } else if (row.isSignal() && activeStrip() != null) {
                    activeStrip().assignToActiveAxis(row.field);
                } else if (row.isSignal()) {
                    toggle(row.field);
                }
            }
        });
        fieldList.getInputMap().put(KeyStroke.getKeyStroke(KeyEvent.VK_SPACE, InputEvent.SHIFT_DOWN_MASK), "plotInNewPlot");
        fieldList.getActionMap().put("plotInNewPlot", new AbstractAction() {
            @Override
            public void actionPerformed(ActionEvent event) {
                SignalSections.Row row = fieldList.getSelectedValue();
                if (row != null && row.isSignal()) {
                    addToNewPlot(row.field);
                }
            }
        });
        filter("");
        JScrollPane fieldScroll = new JScrollPane(fieldList);
        fieldScroll.setBorder(BorderFactory.createEmptyBorder());
        fieldScroll.setHorizontalScrollBarPolicy(JScrollPane.HORIZONTAL_SCROLLBAR_NEVER);
        sidebar.add(fieldScroll, BorderLayout.CENTER);
        return sidebar;
    }

    /**
     * Clicks on the signal list's rows: plot a signal, open or close a section, the menus. A click acts on release
     * when the pointer stayed near the press, not on MOUSE_CLICKED, which AWT drops after the slightest movement;
     * a longer move is a drag (drag and drop). The second click of a double-click is ignored, so it neither undoes
     * the first nor hits another row.
     */
    final MouseAdapter rowMouse = new MouseAdapter() {
        private int pressedIndex = -1;
        private Point pressedAt;

        // the popup trigger is on press on some systems and on release on others
        @Override
        public void mousePressed(MouseEvent event) {
            pressedIndex = -1;
            if (showRowMenu(event)) {
                return;
            }
            if (SwingUtilities.isLeftMouseButton(event)) {
                pressedIndex = rowAt(event.getPoint());
                pressedAt = event.getPoint();
            }
        }

        @Override
        public void mouseReleased(MouseEvent event) {
            int index = pressedIndex;
            pressedIndex = -1;
            if (showRowMenu(event) || index < 0 || !SwingUtilities.isLeftMouseButton(event)
                    || event.getClickCount() > 1 || pressedAt.distance(event.getPoint()) > clickSlop()) {
                return;
            }
            clickRow(index, event);
        }
    };

    /**
     * How far the pointer may move between press and release and still click: at least the drag threshold, and
     * never less than a few pixels, since the threshold can be as small as 2 px.
     */
    private static int clickSlop() {
        try {
            return Math.max(CLICK_SLOP, DragSource.getDragThreshold());
        } catch (RuntimeException headless) {
            return CLICK_SLOP;
        }
    }

    private void clickRow(int index, MouseEvent event) {
        SignalSections.Row row = rows.get(index);
        Rectangle cell = fieldList.getCellBounds(index, index);
        if (row.isHeading() && row.kind == SignalSections.Kind.LIST
                && event.getX() >= cell.x + cell.width - MENU_HOT_WIDTH) {
            listMenu(row.section).show(fieldList, cell.x + cell.width - MENU_HOT_WIDTH, cell.y + cell.height);
            return;
        }
        if (row.isHeading()) {
            toggleSection(row.section);
            return;
        }
        if (!row.isSignal()) {
            return;
        }
        if (activeStrip() != null) {
            activeStrip().assignToActiveAxis(row.field);
            return;
        }
        if (event.isShiftDown()) {
            addToNewPlot(row.field);
        } else {
            toggle(row.field);
        }
        keepUnderPointer(row, index);
    }

    /**
     * Plotting a signal adds (or removes) its row in a plot section above, which shifts the rows below; scrolls by the
     * same amount, where the list can scroll, so the clicked row stays under the pointer for the next click.
     */
    private void keepUnderPointer(SignalSections.Row clicked, int oldIndex) {
        JViewport viewport = (JViewport) SwingUtilities.getAncestorOfClass(JViewport.class, fieldList);
        if (viewport == null) {
            return;
        }
        for (int index = 0; index < rows.size(); index++) {
            SignalSections.Row row = rows.get(index);
            if (row.field == clicked.field && row.section.equals(clicked.section)) {
                int shift = fieldList.getCellBounds(index, index).y - fieldList.getCellBounds(oldIndex, oldIndex).y;
                if (shift != 0) {
                    // the list's size follows its rows only after layout, so size it now for the clamp below
                    fieldList.setSize(fieldList.getWidth(), fieldList.getPreferredSize().height);
                    Point view = viewport.getViewPosition();
                    int maxY = Math.max(0, fieldList.getHeight() - viewport.getExtentSize().height);
                    viewport.setViewPosition(new Point(view.x, Math.max(0, Math.min(maxY, view.y + shift))));
                }
                return;
            }
        }
    }

    JList<SignalSections.Row> fieldList() {
        return fieldList;
    }

    SignalCategories categories() {
        return categories;
    }

    SignalLists lists() {
        return lists;
    }

    private int rowAt(Point point) {
        int index = fieldList.locationToIndex(point);
        return index >= 0 && fieldList.getCellBounds(index, index).contains(point) ? index : -1;
    }

    /** Shows the row's menu for a popup trigger; true if the event was one. */
    private boolean showRowMenu(MouseEvent event) {
        if (!event.isPopupTrigger()) {
            return false;
        }
        int index = rowAt(event.getPoint());
        if (index >= 0) {
            fieldList.setSelectedIndex(index);
            JPopupMenu menu = rowMenu(rows.get(index));
            if (menu != null) {
                menu.show(fieldList, event.getX(), event.getY());
            }
        }
        return true;
    }

    /** The right-click menu of a row: list actions for a list heading, signal actions otherwise. */
    JPopupMenu rowMenu(SignalSections.Row row) {
        if (row.isHeading()) {
            return row.kind == SignalSections.Kind.LIST ? listMenu(row.section) : null;
        }
        JPopupMenu menu = new JPopupMenu();
        if (row.isSignal()) {
            int field = row.field;
            boolean plotted = layout.isPlotted(field);
            JMenuItem plot = new JMenuItem(plotted ? "Remove from plot" : "Add to " + plotName(layout.activePlot()));
            plot.addActionListener(e -> toggle(field));
            menu.add(plot);
            JMenuItem newPlot = new JMenuItem("Add to new plot");
            newPlot.setEnabled(!plotted);
            newPlot.addActionListener(e -> addToNewPlot(field));
            menu.add(newPlot);
            menu.addSeparator();
            menu.add(chart.colorSubmenu(field));
            if (plotted) {
                menu.add(chart.scaleSubmenu(field));
            }
            JMenu addTo = new JMenu("Add to list");
            for (String list : listNames()) {
                JCheckBoxMenuItem item = new JCheckBoxMenuItem(list, labels(list).contains(log.name(field)));
                item.setToolTipText(item.isSelected() ? "In " + list + " · click to remove" : "Copy it to " + list);
                item.addActionListener(e -> {
                    if (item.isSelected()) {
                        addToList(list, field);
                    } else {
                        removeFromList(list, log.name(field));
                    }
                });
                addTo.add(item);
            }
            addTo.addSeparator();
            JMenuItem newList = new JMenuItem("New list…");
            newList.addActionListener(e -> newList(field));
            addTo.add(newList);
            menu.add(addTo);
        }
        if (row.kind == SignalSections.Kind.LIST) {
            String label = row.isMissing() ? row.label : log.name(row.field);
            JMenuItem remove = new JMenuItem("Remove from " + row.section);
            remove.addActionListener(e -> removeFromList(row.section, label));
            menu.add(remove);
        }
        return menu;
    }

    /** The ⋯ / right-click menu of a list heading. */
    JPopupMenu listMenu(String list) {
        boolean common = SignalSections.COMMON.equals(list);
        JPopupMenu menu = new JPopupMenu();
        List<Integer> present = presentFields(list);
        JMenuItem plotAll = new JMenuItem("Plot all");
        plotAll.setToolTipText("Adds the list's signals to a new plot");
        plotAll.setEnabled(present.stream().anyMatch(field -> !layout.isPlotted(field)));
        plotAll.addActionListener(e -> plotAll(list));
        menu.add(plotAll);
        menu.addSeparator();
        JMenuItem rename = new JMenuItem("Rename…");
        rename.setEnabled(!common);
        rename.addActionListener(e -> renameList(list));
        menu.add(rename);
        if (common) {
            JMenuItem reset = new JMenuItem("Reset to defaults");
            reset.setEnabled(lists.isCommonEdited());
            reset.addActionListener(e -> {
                lists.resetCommon();
                filter(search.getText());
            });
            menu.add(reset);
        } else {
            JMenuItem delete = new JMenuItem("Delete list");
            delete.addActionListener(e -> deleteList(list));
            menu.add(delete);
        }
        menu.addSeparator();
        JMenuItem newList = new JMenuItem("New list…");
        newList.addActionListener(e -> newList(-1));
        menu.add(newList);
        return menu;
    }

    private List<String> listNames() {
        List<String> names = new ArrayList<>();
        names.add(SignalSections.COMMON);
        names.addAll(lists.customNames());
        return names;
    }

    /** A list's labels; Common's defaults while it is not edited, so the first edit starts from them. */
    List<String> labels(String list) {
        List<String> members = lists.members(list);
        return members != null ? members : new ArrayList<>(categories.defaultCommonLabels());
    }

    /** A list's signals that are in this log, in list order. */
    private List<Integer> presentFields(String list) {
        Map<String, Integer> fieldOfLabel = SignalSections.fieldOfLabel(log);
        List<Integer> fields = new ArrayList<>();
        for (String label : labels(list)) {
            Integer field = fieldOfLabel.get(label);
            if (field != null) {
                fields.add(field);
            }
        }
        return fields;
    }

    /** Copies a signal to a list (the end); nothing if it is already there. */
    void addToList(String list, int field) {
        List<String> labels = labels(list);
        if (!labels.contains(log.name(field))) {
            labels.add(log.name(field));
            lists.setMembers(list, labels);
        }
        filter(search.getText());
    }

    void removeFromList(String list, String label) {
        List<String> labels = labels(list);
        labels.remove(label);
        lists.setMembers(list, labels);
        filter(search.getText());
    }

    /** Adds the list's unplotted signals to a new plot, or to the active plot while it is empty. */
    void plotAll(String list) {
        boolean intoActive = layout.fields(layout.activePlot()).isEmpty();
        for (int field : presentFields(list)) {
            if (layout.isPlotted(field)) {
                continue;
            }
            if (intoActive) {
                layout.add(field);
            } else {
                // the new plot becomes the active one, where the rest follow
                addToNewPlot(field);
                intoActive = true;
            }
        }
    }

    /** Makes a list, holding the given signal unless it is -1. */
    void newList(int field) {
        String name = askListName("New list", "", null);
        if (name == null) {
            return;
        }
        lists.create(name);
        state.setCollapsed(name, false);
        if (field >= 0) {
            addToList(name, field);
        } else {
            filter(search.getText());
        }
    }

    void renameList(String list) {
        String name = askListName("Rename list", list, list);
        if (name == null || name.equals(list)) {
            return;
        }
        lists.rename(list, name);
        state.rename(list, name);
        filter(search.getText());
    }

    void deleteList(String list) {
        if (!confirm.test("Delete the list \"" + list + "\"? Its signals stay in their groups.")) {
            return;
        }
        lists.delete(list);
        state.forget(list);
        filter(search.getText());
    }

    /** A valid, trimmed list name, or null when cancelled or refused (with a message). */
    private String askListName(String title, String initial, String current) {
        String name = namePrompt.apply(title, initial);
        if (name == null) {
            return null;
        }
        String problem = lists.problem(name, current);
        if (problem != null) {
            messages.accept(problem);
            return null;
        }
        return name.trim();
    }

    /** Opens a collapsed section or collapses an open one. */
    void toggleSection(String section) {
        state.setCollapsed(section, !state.isCollapsed(section));
        filter(search.getText());
    }

    JTextField searchField() {
        return search;
    }

    LogChart chart() {
        return chart;
    }

    LogStatusBar statusBar() {
        return statusBar;
    }

    StatsPanel statsPanel() {
        return statsPanel;
    }

    ChartToolbar toolbar() {
        return toolbar;
    }

    boolean isStatsVisible() {
        return statsPanel.getParent() == statsSplit;
    }

    /** The Timeline / Histogram / Scatter switch; {@code segments} receives its three toggles. */
    private JComponent viewSwitch(JToggleButton[] segments) {
        JToggleButton timeline = SegmentedControl.segment("Timeline");
        JToggleButton histogramSegment = SegmentedControl.segment("Histogram");
        JToggleButton scatterSegment = SegmentedControl.segment("Scatter");
        timeline.setToolTipText("Signals over time");
        histogramSegment.setToolTipText("A signal binned by two others, e.g. lambda by RPM and MAP: "
                + "where the engine ran and what it read there");
        scatterSegment.setToolTipText("Every record as a point by two signals, coloured by time spent or a third: "
                + "how signals relate, and where events like knock happen");
        ButtonGroup group = new ButtonGroup();
        group.add(timeline);
        group.add(histogramSegment);
        group.add(scatterSegment);
        timeline.setSelected(true);
        timeline.addActionListener(e -> setView(TIMELINE));
        histogramSegment.addActionListener(e -> setView(HISTOGRAM));
        scatterSegment.addActionListener(e -> setView(SCATTER));
        segments[0] = timeline;
        segments[1] = histogramSegment;
        segments[2] = scatterSegment;
        return new SegmentedControl(timeline, histogramSegment, scatterSegment);
    }

    /** The working tune (or null), read when the histogram bins by a tune table's axes. */
    void setTuneSupplier(Supplier<ConfigurationImage> tuneSupplier) {
        this.tuneSupplier = tuneSupplier;
    }

    /** Where the histogram remembers its signals, filters and options; shared by every log. */
    void setHistogramSettings(HistogramSettings histogramSettings) {
        this.histogramSettings = histogramSettings;
    }

    /** Where the scatter plot remembers its signals, filters and options; shared by every log. */
    void setScatterSettings(HistogramSettings scatterSettings) {
        this.scatterSettings = scatterSettings;
    }

    boolean isHistogramView() {
        return HISTOGRAM.equals(shownView);
    }

    String shownView() {
        return shownView;
    }

    HistogramView histogram() {
        return histogram;
    }

    ScatterView scatter() {
        return scatter;
    }

    /** The shown view's axes and filters (Histogram or Scatter), or null on the timeline. */
    SignalStrip activeStrip() {
        return HISTOGRAM.equals(shownView) && histogram != null ? histogram.strip
                : SCATTER.equals(shownView) && scatter != null ? scatter.strip() : null;
    }

    /** Shows the histogram or the timeline right of the sidebar. */
    void setHistogramView(boolean show) {
        setView(show ? HISTOGRAM : TIMELINE);
    }

    /**
     * Shows a view right of the sidebar ({@link HistogramSettings#TIMELINE}, {@code HISTOGRAM} or {@code SCATTER});
     * the histogram and the scatter plot are built when first shown.
     */
    void setView(String view) {
        if (HISTOGRAM.equals(view) && histogram == null) {
            // the sidebar's axes section follows the axes
            histogram = new HistogramView(log, histogramSettings, chart.cursors(), viewSwitch(histogramSegments),
                    onOpen, () -> filter(search.getText()), () -> ini, () -> tuneSupplier.get());
            histogram.setTimelineAction(this::showOnTimeline);
            views.add(histogram, HISTOGRAM);
        }
        if (SCATTER.equals(view) && scatter == null) {
            scatter = new ScatterView(log, scatterSettings, settings, chart.cursors(), viewSwitch(scatterSegments),
                    onOpen, () -> filter(search.getText()));
            scatter.setTimelineAction(this::showOnTimeline);
            // Light / Dark is one setting for the timeline and the scatter plot
            scatter.setOnDarkChanged(() -> {
                (settings.isDarkPlot() ? toolbar.darkPlot : toolbar.lightPlot).setSelected(true);
                chart.repaint();
            });
            views.add(scatter, SCATTER);
        }
        if (SCATTER.equals(view)) {
            scatter.syncDark();
        }
        shownView = view;
        histogramSettings.setView(view);
        int index = TIMELINE.equals(view) ? 0 : HISTOGRAM.equals(view) ? 1 : 2;
        for (JToggleButton[] segments : new JToggleButton[][]{timelineSegments, histogramSegments, scatterSegments}) {
            if (segments[0] != null) {
                segments[index].setSelected(true);
            }
        }
        ((CardLayout) views.getLayout()).show(views, view);
        boolean timeline = TIMELINE.equals(view);
        // the other views have their own status strip; their X, Y and Z replace the plot sections in the sidebar
        statusBar.setVisible(timeline);
        plottedLabel.setVisible(timeline);
        filter(search.getText());
        revalidate();
        repaint();
    }

    VisitsBar visitsBar() {
        return visitsBar;
    }

    /**
     * Shows stays (a histogram cell's, the scatter plot's selection) on the timeline: shaded, the longest in view,
     * with the bar to step through them and back to the view they came from. An empty timeline first gets that
     * view's X, Y and Z, one plot each, so there is something to see.
     */
    void showOnTimeline(String what, List<double[]> stays) {
        SignalStrip strip = activeStrip();
        returnView = TIMELINE.equals(shownView) ? returnView : shownView;
        visitsBar.setBackLabel(SCATTER.equals(returnView) ? "Back to scatter" : "Back to histogram");
        setView(TIMELINE);
        if (layout.plottedCount() == 0 && strip != null) {
            layout.add(strip.field('x'));
            layout.addToNewPlot(strip.field('y'));
            if (strip.field('z') != strip.field('x') && strip.field('z') != strip.field('y')) {
                layout.addToNewPlot(strip.field('z'));
            }
        }
        visitsBar.show(what, stays);
    }

    /** Shows the Stats panel to the right of the plots (resizable), or the plots alone. */
    void setStatsVisible(boolean visible) {
        toolbar.statsButton.setSelected(visible);
        if (visible == isStatsVisible()) {
            return;
        }
        plotArea.remove(visible ? chart : statsSplit);
        if (visible) {
            statsSplit.setLeftComponent(chart);
            statsSplit.setRightComponent(statsPanel);
            statsPanel.refresh();
            plotArea.add(statsSplit, BorderLayout.CENTER);
        } else {
            statsSplit.remove(chart);
            statsSplit.remove(statsPanel);
            plotArea.add(chart, BorderLayout.CENTER);
        }
        plotArea.revalidate();
        plotArea.repaint();
    }

    /** The signals a log opens with: RPM and TPS, those of them it has. */
    static boolean[] defaultPlotted(CsvLogReader log) {
        boolean[] plotted = new boolean[log.fieldCount()];
        for (String name : DEFAULT_PLOTTED) {
            for (int field = 0; field < log.fieldCount(); field++) {
                if (log.name(field).equalsIgnoreCase(name)) {
                    plotted[field] = true;
                    break;
                }
            }
        }
        return plotted;
    }

    JLabel plottedLabel() {
        return plottedLabel;
    }

    /** Removes a plotted signal, otherwise adds it to the active plot. */
    void toggle(int field) {
        layout.toggle(field);
    }

    /** Adds an unplotted signal to a new plot (Shift+click). */
    void addToNewPlot(int field) {
        layout.addToNewPlot(field);
    }

    private static String plotName(int plot) {
        return "plot " + (plot + 1);
    }

    private void updatePlottedLabel() {
        int count = layout.plottedCount();
        String text = count == 0 ? "none plotted" : count + " plotted";
        plottedLabel.setText(layout.plotCount() > 1 ? text + " · " + layout.plotCount() + " plots" : text);
    }

    /**
     * Rebuilds the sections (see {@link SignalSections}), keeping the selected row. Time (field 0) is the x axis,
     * not a signal.
     */
    private void filter(String query) {
        SignalSections.Row selected = fieldList.getSelectedValue();
        rows.clear();
        SignalStrip strip = activeStrip();
        int[] axisFields = strip != null ? new int[]{strip.field('x'), strip.field('y'), strip.field('z')} : null;
        String axesTitle = SCATTER.equals(shownView) ? SignalSections.SCATTER : SignalSections.HISTOGRAM;
        for (SignalSections.Row row : SignalSections.rows(log, layout, axisFields, axesTitle, categories, lists, query,
                state.hideZero(), state.hideConstant(), state::isCollapsed)) {
            rows.addElement(row);
        }
        if (selected != null) {
            for (int index = 0; index < rows.size(); index++) {
                SignalSections.Row row = rows.get(index);
                if (row.field == selected.field && row.section.equals(selected.section)
                        && Objects.equals(row.label, selected.label)) {
                    fieldList.setSelectedIndex(index);
                    break;
                }
            }
        }
        updateCount();
    }

    /**
     * Signals listed in the groups (each signal is in exactly one; the plots and the lists are copies) out of all
     * signals: what is left after hiding zero or constant signals, or searching.
     */
    private void updateCount() {
        int total = log.fieldCount() - 1;
        int listed = 0;
        for (int index = 0; index < rows.size(); index++) {
            SignalSections.Row row = rows.get(index);
            if (row.isHeading() && row.kind == SignalSections.Kind.GROUP) {
                listed += row.count;
            }
        }
        countChip.setText(listed == total ? String.format(Locale.ROOT, "%,d", total)
                : String.format(Locale.ROOT, "%,d / %,d", listed, total));
        countChip.setToolTipText(listed == total ? "Signals in this log"
                : String.format(Locale.ROOT, "%,d of %,d signals listed", listed, total));
    }

    private void setUpFilterBox(JCheckBox box, String text, int count, boolean selected, String tooltip,
                                Consumer<Boolean> apply) {
        box.setText(String.format(Locale.ROOT, "%s (%,d)", text, count));
        box.setSelected(selected);
        box.setOpaque(false);
        box.setVisible(count > 0);
        box.setToolTipText(tooltip);
        box.addActionListener(e -> {
            apply.accept(box.isSelected());
            filter(search.getText());
        });
    }

    /** "always 0", or "constant · 6 rpm" so a fixed value reads without plotting it. */
    private String flatText(int field) {
        if (SignalSections.isZero(log, field)) {
            return "always 0";
        }
        String unit = log.unit(field);
        return "constant · " + ValueFormat.format(log, field, log.minimum(field)) + (unit.isEmpty() ? "" : " " + unit);
    }

    Chip countChip() {
        return countChip;
    }

    /**
     * Drags a signal out of the list onto a plot. A signal dropped on a list's heading or members is copied to
     * that list; a trace dragged back onto the rest of the list is removed from its plot.
     */
    final class ListTransferHandler extends SignalTransfer.Handler {
        @Override
        protected Transferable createTransferable(JComponent component) {
            SignalSections.Row row = fieldList.getSelectedValue();
            if (row == null || !row.isSignal()) {
                return null;
            }
            listDragOrigin = fieldList.getSelectedIndex();
            return SignalTransfer.of(row.field);
        }

        @Override
        protected void exportDone(JComponent source, Transferable data, int action) {
            listDragOrigin = -1;
        }

        @Override
        boolean canImportSignal(TransferSupport support) {
            return true;
        }

        @Override
        boolean importSignal(TransferSupport support, int field) {
            return dropOnList(field, support.isDrop() ? ((JList.DropLocation) support.getDropLocation()).getIndex() : -1);
        }
    }

    /**
     * A signal dropped on the list at a row: back on the row it was dragged from, it was a click that moved past the
     * drag threshold (2 px on some systems) and toggles the signal; on a list it is copied there; on a plot section
     * it moves to that plot (or is added there); elsewhere a plotted trace leaves its plot.
     */
    boolean dropOnList(int field, int index) {
        if (listDragOrigin >= 0 && index == listDragOrigin) {
            int origin = listDragOrigin;
            SignalSections.Row row = rows.get(origin);
            if (activeStrip() != null) {
                activeStrip().assignToActiveAxis(field);
                return true;
            }
            toggle(field);
            keepUnderPointer(row, origin);
            return true;
        }
        String list = listAt(index);
        if (list != null) {
            addToList(list, field);
            return true;
        }
        int plot = plotAt(index);
        if (plot >= 0) {
            layout.moveTo(field, plot);
            return true;
        }
        if (!layout.isPlotted(field)) {
            return false;
        }
        layout.remove(field);
        return true;
    }

    /** The list whose heading or member is at a row, or null. */
    String listAt(int index) {
        if (index < 0 || index >= rows.size()) {
            return null;
        }
        SignalSections.Row row = rows.get(index);
        return row.kind == SignalSections.Kind.LIST ? row.section : null;
    }

    /** The plot whose section heading or member is at a row, or -1. */
    int plotAt(int index) {
        if (index < 0 || index >= rows.size()) {
            return -1;
        }
        SignalSections.Row row = rows.get(index);
        return row.kind == SignalSections.Kind.PLOTTED ? SignalSections.plotOf(row.section) : -1;
    }

    /** Small rounded count badge, like the Gauges RPM readout. */
    static final class Chip extends JLabel {
        Chip(String text) {
            super(text);
            setBorder(BorderFactory.createEmptyBorder(1, 7, 1, 7));
            setFont(getFont().deriveFont(getFont().getSize2D() - 1));
            setForeground(themed("Console.LogViewer.mutedForeground", Color.GRAY));
        }

        @Override
        protected void paintComponent(Graphics g) {
            Graphics2D g2 = (Graphics2D) g.create();
            try {
                g2.setRenderingHint(RenderingHints.KEY_ANTIALIASING, RenderingHints.VALUE_ANTIALIAS_ON);
                g2.setColor(themed("Console.LogViewer.chipBackground", new Color(0xeef0f3)));
                g2.fillRoundRect(0, 0, getWidth(), getHeight(), getHeight(), getHeight());
            } finally {
                g2.dispose();
            }
            super.paintComponent(g);
        }
    }

    /** The plots (or a view's axes), the lists, the groups: kinds of sections a line separates in the sidebar. */
    static int zone(SignalSections.Row row) {
        switch (row.kind) {
            case PLOTTED:
            case HISTOGRAM:
                return 0;
            case LIST:
                return 1;
            default:
                return 2;
        }
    }

    /** A section heading (arrow, title, count) or a signal (trace dot, name, unit) indented under it. */
    private final class SignalRenderer extends JPanel implements ListCellRenderer<SignalSections.Row> {
        private final TraceDot dot = new TraceDot();
        private final JLabel name = new JLabel(dot, SwingConstants.LEADING);
        private final JLabel unit = new JLabel();
        /** A line above the heading: the first of the lists, or of the groups, below the plots. */
        private boolean divider;

        SignalRenderer() {
            super(new BorderLayout(8, 0));
            setBorder(BorderFactory.createEmptyBorder(0, 6, 0, 8));
            name.setIconTextGap(10);
            add(name, BorderLayout.CENTER);
            add(unit, BorderLayout.EAST);
        }

        // Like DefaultListCellRenderer: lay out whenever the list sizes the row (validate() is a no-op
        // without a native peer, e.g. when painting offscreen) and skip revalidation per row.
        @Override
        public void setBounds(int x, int y, int width, int height) {
            super.setBounds(x, y, width, height);
            doLayout();
        }

        @Override
        public void invalidate() {
        }

        @Override
        protected void paintComponent(Graphics g) {
            super.paintComponent(g);
            if (divider) {
                g.setColor(themed("Console.LogViewer.sectionDivider", Color.LIGHT_GRAY));
                g.fillRect(2, 1, getWidth() - 10, 1);
            }
        }

        @Override
        public void validate() {
        }

        @Override
        public void revalidate() {
        }

        @Override
        public Component getListCellRendererComponent(JList<? extends SignalSections.Row> list, SignalSections.Row row,
                                                      int index, boolean selected, boolean focused) {
            Color muted = themed("Console.LogViewer.mutedForeground", Color.GRAY);
            setBackground(selected ? themed("Console.LogViewer.chipBackground", list.getSelectionBackground())
                    : list.getBackground());
            divider = row.isHeading() && index > 0 && zone(list.getModel().getElementAt(index - 1)) != zone(row);
            if (row.isHeading()) {
                setBorder(BorderFactory.createEmptyBorder(6, 2, 0, 8));
                name.setIcon(null);
                name.setText((row.expanded ? "▾  " : "▸  ") + row.section.toUpperCase(Locale.ROOT));
                name.setFont(list.getFont().deriveFont(Font.BOLD, list.getFont().getSize2D() - 1));
                name.setForeground(muted);
                // a list heading ends with its menu button (MENU_HOT_WIDTH)
                unit.setText(String.format(Locale.ROOT, row.kind == SignalSections.Kind.LIST ? "%,d    ⋯" : "%,d",
                        row.count));
                unit.setFont(list.getFont().deriveFont(list.getFont().getSize2D() - 1));
                unit.setForeground(muted);
                return this;
            }
            setBorder(BorderFactory.createEmptyBorder(0, 16, 0, 8));
            name.setIcon(dot);
            if (row.isMissing()) {
                dot.set(null, new Color(muted.getRed(), muted.getGreen(), muted.getBlue(), 50));
                name.setText(row.label);
                name.setFont(list.getFont());
                name.setForeground(muted);
                unit.setText("not in this log");
                unit.setFont(list.getFont().deriveFont(Font.ITALIC));
                unit.setForeground(muted);
                return this;
            }
            int field = row.field;
            boolean plotted = layout.isPlotted(field);
            if (plotted) {
                Color trace = chart.colorOf(field, chart.palette());
                // a darker outline keeps pale dark-plot colours visible on the light list
                dot.set(trace, trace.darker());
            } else {
                dot.set(null, new Color(muted.getRed(), muted.getGreen(), muted.getBlue(), 110));
            }
            boolean flat = log.isConstant(field);
            name.setText(log.name(field));
            name.setFont(list.getFont().deriveFont(plotted ? Font.BOLD : Font.PLAIN));
            // flat signals (in a plot, Common, search or when not hidden) are muted and give their value
            name.setForeground(flat && !plotted ? muted : list.getForeground());
            unit.setText(flat ? flatText(field) : log.unit(field));
            unit.setFont(flat ? list.getFont().deriveFont(Font.ITALIC) : list.getFont());
            unit.setForeground(muted);
            char axis = activeStrip() != null ? activeStrip().axisOf(field) : 0;
            if (axis != 0) {
                // the histogram's signals are marked with their axis instead of their unit
                unit.setText(Character.toUpperCase(axis) + " axis");
                unit.setFont(list.getFont().deriveFont(Font.BOLD));
                unit.setForeground(themed("Console.LogViewer.histogram.accent", list.getForeground()));
            }
            return this;
        }
    }
}
