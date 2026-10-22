package com.rusefi.ui.logview;

import com.formdev.flatlaf.FlatClientProperties;
import com.formdev.flatlaf.icons.FlatSearchIcon;
import com.rusefi.logview.CsvLogReader;

import javax.swing.AbstractAction;
import javax.swing.BorderFactory;
import javax.swing.Box;
import javax.swing.DefaultListModel;
import javax.swing.JCheckBox;
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
import java.awt.RenderingHints;
import java.awt.datatransfer.Transferable;
import java.awt.event.ActionEvent;
import java.awt.event.InputEvent;
import java.awt.event.KeyEvent;
import java.awt.event.MouseAdapter;
import java.awt.event.MouseEvent;
import java.util.Locale;
import java.util.Set;
import java.util.function.Consumer;

import static com.rusefi.ui.logview.ThemedStrip.themed;

/**
 * Log viewer: a searchable signal list on the left, in collapsible sections (see {@link SignalSections};
 * click or Space adds a signal to the active plot,
 * Shift+click or Shift+Space to a new plot, or drag it onto a plot; a dot shows its trace colour), the
 * stacked plots with their toolbar on the right, and a status strip below. [tag:console_theme]
 */
public final class LogViewerPanel extends JPanel {
    private static final int SIDEBAR_WIDTH = 280;

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
                return row.expanded ? "Click to collapse" : "Click to expand";
            }
            int field = row.field;
            String unit = log.unit(field);
            String name = log.name(field) + (unit.isEmpty() ? "" : " (" + unit + ")")
                    + (SignalSections.isZero(log, field) ? " · always 0 in this log"
                    : SignalSections.isConstant(log, field) ? " · constant in this log" : "");
            if (layout.isPlotted(field)) {
                return name + " · in " + plotName(layout.plotOf(field)) + " · click to remove";
            }
            return name + " · click to add to " + plotName(layout.activePlot()) + " · Shift+click: new plot";
        }

        // Always fit the sidebar: a long name is shortened with "…" instead of pushing units under the scrollbar.
        @Override
        public boolean getScrollableTracksViewportWidth() {
            return true;
        }
    };
    private final JTextField search = new JTextField();
    private final SignalCategories categories;
    private final Set<String> collapsed = SignalSections.defaultCollapsed();
    private boolean hideZero = true;
    private boolean hideConstant = false;
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
        this(log, fileName, new ChartSettings(), null);
    }

    /**
     * @param settings chart options, shared so they carry over when another log is opened
     * @param onOpen   action of the toolbar's Open button, given the button (e.g. to show a menu under it),
     *                 or null for no button
     */
    LogViewerPanel(CsvLogReader log, String fileName, ChartSettings settings, Consumer<Component> onOpen) {
        super(new BorderLayout());
        this.log = log;
        this.settings = settings;
        boolean[] initial = new boolean[log.fieldCount()];
        for (int field = 1; field < log.fieldCount(); field++) {
            String name = log.name(field);
            initial[field] = name.equalsIgnoreCase("RPM") || name.equalsIgnoreCase("MAP")
                    || name.equalsIgnoreCase("TPS");
        }
        layout = PlotLayout.single(log.fieldCount(), initial);
        categories = SignalCategories.of(log, null);
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

        String stillFound = "<br>they stay in Plotted and Common and are found by search</html>";
        setUpFilterBox(hideZeroBox, "Hide zero signals", SignalSections.zeroCount(log), hideZero,
                "<html>Leave signals that are 0 for the whole log (mostly unused features) out of the groups;"
                        + stillFound, selected -> hideZero = selected);
        setUpFilterBox(hideConstantBox, "Hide constant signals", SignalSections.constantCount(log), hideConstant,
                "<html>Leave signals that hold one other value for the whole log out of the groups.<br>"
                        + "These can matter: an active error, the detected gear, the firmware version;"
                        + stillFound, selected -> hideConstant = selected);
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
                int index = fieldList.locationToIndex(event.getPoint());
                if (index >= 0 && fieldList.getCellBounds(index, index).contains(event.getPoint())) {
                    SignalSections.Row row = rows.get(index);
                    if (row.isHeading()) {
                        toggleSection(row.section);
                    } else if (event.isShiftDown()) {
                        addToNewPlot(row.field);
                    } else {
                        toggle(row.field);
                    }
                }
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
                } else {
                    toggle(row.field);
                }
            }
        });
        fieldList.getInputMap().put(KeyStroke.getKeyStroke(KeyEvent.VK_SPACE, InputEvent.SHIFT_DOWN_MASK), "plotInNewPlot");
        fieldList.getActionMap().put("plotInNewPlot", new AbstractAction() {
            @Override
            public void actionPerformed(ActionEvent event) {
                SignalSections.Row row = fieldList.getSelectedValue();
                if (row != null && !row.isHeading()) {
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

    /** Opens a collapsed section or collapses an open one. */
    void toggleSection(String section) {
        if (!collapsed.remove(section)) {
            collapsed.add(section);
        }
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
        for (SignalSections.Row row : SignalSections.rows(log, layout, categories, query, hideZero, hideConstant,
                collapsed)) {
            rows.addElement(row);
        }
        if (selected != null) {
            for (int index = 0; index < rows.size(); index++) {
                SignalSections.Row row = rows.get(index);
                if (row.field == selected.field && row.section.equals(selected.section)) {
                    fieldList.setSelectedIndex(index);
                    break;
                }
            }
        }
        updateCount();
    }

    /**
     * Signals listed in the groups (each signal is in exactly one; Plotted and Common are copies) out of all
     * signals: what is left after hiding zero or constant signals, or searching.
     */
    private void updateCount() {
        int total = log.fieldCount() - 1;
        int listed = 0;
        for (int index = 0; index < rows.size(); index++) {
            SignalSections.Row row = rows.get(index);
            if (row.isHeading() && !row.section.equals(SignalSections.PLOTTED)
                    && !row.section.equals(SignalSections.COMMON)) {
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

    /** Drags a signal out of the list onto a plot; a trace dragged back onto the list is removed. */
    final class ListTransferHandler extends SignalTransfer.Handler {
        @Override
        protected Transferable createTransferable(JComponent component) {
            SignalSections.Row row = fieldList.getSelectedValue();
            return row == null || row.isHeading() ? null : SignalTransfer.of(row.field);
        }

        @Override
        boolean canImportSignal(TransferSupport support) {
            return true;
        }

        @Override
        boolean importSignal(TransferSupport support, int field) {
            if (!layout.isPlotted(field)) {
                return false;
            }
            layout.remove(field);
            return true;
        }
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
                unit.setText(String.format(Locale.ROOT, "%,d", row.count));
                unit.setFont(list.getFont().deriveFont(list.getFont().getSize2D() - 1));
                unit.setForeground(muted);
                return this;
            }
            int field = row.field;
            setBorder(BorderFactory.createEmptyBorder(0, 16, 0, 8));
            name.setIcon(dot);
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
