package com.rusefi.ui.logview;

import com.formdev.flatlaf.FlatClientProperties;
import com.formdev.flatlaf.icons.FlatSearchIcon;
import com.opensr5.ini.IniFileModel;
import com.rusefi.logview.CsvLogReader;

import javax.swing.AbstractAction;
import javax.swing.BorderFactory;
import javax.swing.Box;
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
import javax.swing.KeyStroke;
import javax.swing.ListCellRenderer;
import javax.swing.SwingConstants;
import javax.swing.event.DocumentEvent;
import javax.swing.event.DocumentListener;
import java.awt.BasicStroke;
import java.awt.BorderLayout;
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

    /**
     * @param settings chart options, shared so they carry over when another log is opened
     * @param lists    Common and the user's signal lists, shared by every log
     * @param state    open sections and the Hide switches, shared by every log
     * @param ini      the ECU's .ini for grouping the signals, or null to group them by name
     * @param onOpen   action of the toolbar's Open button, given the button (e.g. to show a menu under it),
     *                 or null for no button
     */
    LogViewerPanel(CsvLogReader log, String fileName, ChartSettings settings, SignalLists lists,
                   SidebarState state, IniFileModel ini, Consumer<Component> onOpen) {
        super(new BorderLayout());
        this.log = log;
        this.settings = settings;
        this.lists = lists;
        this.state = state;
        // a log opens with an empty plot: the user picks what to see (Plot all on a list plots a set at once)
        layout = PlotLayout.single(log.fieldCount(), new boolean[log.fieldCount()]);
        categories = SignalCategories.of(log, ini);
        chart = new LogChart(log, layout, settings);
        layout.addListener(() -> {
            // the Plotted section follows the plots
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
        chart.addViewListener(statsPanel::refreshValues);
        toolbar.statsButton.addActionListener(e -> setStatsVisible(toolbar.statsButton.isSelected()));
        statsSplit.setResizeWeight(1);
        statsSplit.setContinuousLayout(true);
        statsSplit.setDividerSize(5);
        statsSplit.setBorder(BorderFactory.createEmptyBorder());
        plotArea.add(toolbar, BorderLayout.NORTH);
        plotArea.add(chart, BorderLayout.CENTER);

        JSplitPane split = new JSplitPane(JSplitPane.HORIZONTAL_SPLIT, createSidebar(), plotArea);
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

        String stillFound = "<br>they stay in Plotted and the lists and are found by search</html>";
        setUpFilterBox(hideZeroBox, "Hide zero signals", SignalSections.zeroCount(log), state.hideZero(),
                "<html>Leave signals that are 0 for the whole log (mostly unused features) out of the groups;"
                        + stillFound, state::setHideZero);
        setUpFilterBox(hideConstantBox, "Hide constant signals", SignalSections.constantCount(log),
                state.hideConstant(),
                "<html>Leave signals that hold one other value for the whole log out of the groups.<br>"
                        + "These can matter: an active error, the detected gear, the firmware version;"
                        + stillFound, state::setHideConstant);
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
        fieldList.addMouseListener(new MouseAdapter() {
            @Override
            public void mouseClicked(MouseEvent event) {
                int index = rowAt(event.getPoint());
                if (index < 0 || !SwingUtilities.isLeftMouseButton(event)) {
                    return;
                }
                SignalSections.Row row = rows.get(index);
                Rectangle cell = fieldList.getCellBounds(index, index);
                if (row.isHeading() && row.kind == SignalSections.Kind.LIST
                        && event.getX() >= cell.x + cell.width - MENU_HOT_WIDTH) {
                    listMenu(row.section).show(fieldList, cell.x + cell.width - MENU_HOT_WIDTH, cell.y + cell.height);
                } else if (row.isHeading()) {
                    toggleSection(row.section);
                } else if (!row.isSignal()) {
                    return;
                } else if (event.isShiftDown()) {
                    addToNewPlot(row.field);
                } else {
                    toggle(row.field);
                }
            }

            // the popup trigger is on press on some systems and on release on others
            @Override
            public void mousePressed(MouseEvent event) {
                showRowMenu(event);
            }

            @Override
            public void mouseReleased(MouseEvent event) {
                showRowMenu(event);
            }
        });
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

    private void showRowMenu(MouseEvent event) {
        if (!event.isPopupTrigger()) {
            return;
        }
        int index = rowAt(event.getPoint());
        if (index < 0) {
            return;
        }
        fieldList.setSelectedIndex(index);
        JPopupMenu menu = rowMenu(rows.get(index));
        if (menu != null) {
            menu.show(fieldList, event.getX(), event.getY());
        }
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
        for (SignalSections.Row row : SignalSections.rows(log, layout, categories, lists, query,
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
     * Signals listed in the groups (each signal is in exactly one; Plotted and the lists are copies) out of all
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
            return row == null || !row.isSignal() ? null : SignalTransfer.of(row.field);
        }

        @Override
        boolean canImportSignal(TransferSupport support) {
            return true;
        }

        @Override
        boolean importSignal(TransferSupport support, int field) {
            String list = support.isDrop() ? listAt(((JList.DropLocation) support.getDropLocation()).getIndex()) : null;
            if (list != null) {
                addToList(list, field);
                return true;
            }
            if (!layout.isPlotted(field)) {
                return false;
            }
            layout.remove(field);
            return true;
        }
    }

    /** The list whose heading or member is at a row, or null. */
    String listAt(int index) {
        if (index < 0 || index >= rows.size()) {
            return null;
        }
        SignalSections.Row row = rows.get(index);
        return row.kind == SignalSections.Kind.LIST ? row.section : null;
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

    /** A section heading (arrow, title, count) or a signal (trace dot, name, unit) indented under it. */
    private final class SignalRenderer extends JPanel implements ListCellRenderer<SignalSections.Row> {
        private final TraceDot dot = new TraceDot();
        private final JLabel name = new JLabel(dot, SwingConstants.LEADING);
        private final JLabel unit = new JLabel();

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
                Color trace = chart.palette().trace(layout.colors().slot(field));
                // a darker outline keeps pale dark-plot colours visible on the light list
                dot.set(trace, trace.darker());
            } else {
                dot.set(null, new Color(muted.getRed(), muted.getGreen(), muted.getBlue(), 110));
            }
            boolean flat = log.isConstant(field);
            name.setText(log.name(field));
            name.setFont(list.getFont().deriveFont(plotted ? Font.BOLD : Font.PLAIN));
            // flat signals (in Plotted, Common, search or when not hidden) are muted and give their value
            name.setForeground(flat && !plotted ? muted : list.getForeground());
            unit.setText(flat ? flatText(field) : log.unit(field));
            unit.setFont(flat ? list.getFont().deriveFont(Font.ITALIC) : list.getFont());
            unit.setForeground(muted);
            return this;
        }
    }
}
