package com.rusefi.ui.widgets.tune;

import com.rusefi.ui.widgets.ConsoleFonts;

import javax.swing.BorderFactory;
import javax.swing.JButton;
import javax.swing.JCheckBox;
import javax.swing.JComponent;
import javax.swing.JLabel;
import javax.swing.LayoutFocusTraversalPolicy;
import javax.swing.JPanel;
import javax.swing.JScrollBar;
import javax.swing.JScrollPane;
import javax.swing.JSplitPane;
import javax.swing.JTabbedPane;
import javax.swing.JTable;
import javax.swing.JTextArea;
import javax.swing.ListSelectionModel;
import javax.swing.KeyStroke;
import javax.swing.SwingConstants;
import javax.swing.SwingUtilities;
import javax.swing.table.AbstractTableModel;
import javax.swing.table.DefaultTableCellRenderer;
import java.awt.BorderLayout;
import java.awt.BasicStroke;
import java.awt.Color;
import java.awt.Component;
import java.awt.Container;
import java.awt.Dimension;
import java.awt.Font;
import java.awt.FontMetrics;
import java.awt.Graphics;
import java.awt.Graphics2D;
import java.awt.Insets;
import java.awt.Rectangle;
import java.awt.RenderingHints;
import java.awt.event.HierarchyEvent;
import java.awt.geom.Path2D;
import java.util.LinkedHashSet;
import java.util.List;
import java.util.Set;
import java.util.concurrent.CompletableFuture;
import java.util.function.Consumer;
import java.util.function.Function;

/** Review changed tune values before applying selected changes to ECU RAM. */
final class TuneReviewDialog {
    private TuneReviewDialog() {
    }

    static final class Decision {
        final Set<TuneLoadPlan.Change> selected;
        final boolean burn;

        Decision(Set<TuneLoadPlan.Change> selected, boolean burn) {
            this.selected = Set.copyOf(selected);
            this.burn = burn;
        }
    }

    static void show(String fileName, TuneLoadPlan plan, boolean connected, Function<JComponent, Runnable> host,
                     CompletableFuture<Decision> decision) {
        show(fileName, plan, connected, false, host, decision);
    }

    static void show(String fileName, TuneLoadPlan plan, boolean connected, boolean restoring, Function<JComponent, Runnable> host,
                     CompletableFuture<Decision> decision) {
        if (decision.isDone()) {
            return;
        }
        Review review = new Review(plan.changes(), plan.warnings(), connected, restoring);
        review.onDecision = decision::complete;
        JPanel card = new JPanel(new BorderLayout(0, 4));
        TuningStyle.styleCard(card, 0, 0);
        JLabel title = new JLabel((restoring ? "Review restore point: " : "Review tune: ") + fileName);
        title.setFont(title.getFont().deriveFont(Font.BOLD));
        title.setBorder(BorderFactory.createEmptyBorder(10, 10, 0, 10));
        card.add(title, BorderLayout.NORTH);
        card.add(review.content, BorderLayout.CENTER);
        review.content.setOpaque(false);
        JPanel modal = new JPanel(null) {
            @Override
            public void doLayout() {
                int width = (int) Math.round(getWidth() * 0.8);
                int margin = Math.min(24, getHeight() / 20);
                card.setBounds((getWidth() - width) / 2, margin, width, getHeight() - 2 * margin);
            }
        };
        modal.add(card);
        modal.setFocusCycleRoot(true);
        modal.setFocusTraversalPolicy(new LayoutFocusTraversalPolicy() {
            @Override
            public Component getDefaultComponent(Container focusCycleRoot) {
                return review.cancel;
            }
        });
        modal.registerKeyboardAction(e -> decision.complete(null), KeyStroke.getKeyStroke("ESCAPE"),
            JComponent.WHEN_ANCESTOR_OF_FOCUSED_COMPONENT);
        modal.addHierarchyListener(e -> {
            if (((e.getChangeFlags() & HierarchyEvent.PARENT_CHANGED) != 0 && modal.getParent() == null)
                    || ((e.getChangeFlags() & HierarchyEvent.SHOWING_CHANGED) != 0 && !modal.isShowing())) {
                decision.complete(null);
            }
        });
        Runnable close = host.apply(modal);
        if (close == null) {
            decision.complete(null);
            return;
        }
        decision.whenComplete((result, error) -> {
            if (SwingUtilities.isEventDispatchThread()) {
                close.run();
            } else {
                SwingUtilities.invokeLater(close);
            }
        });
        SwingUtilities.invokeLater(review.cancel::requestFocusInWindow);
    }

    static Rectangle curvePlotArea(int width, int height, Insets border, FontMetrics font, String[] xTicks, String[] yTicks) {
        int left = border.left + 10 + java.util.Arrays.stream(yTicks).mapToInt(font::stringWidth).max().orElse(0);
        left = Math.max(left, border.left + font.stringWidth(xTicks[0]) / 2 + 4);
        int right = width - border.right - font.stringWidth(xTicks[xTicks.length - 1]) / 2 - 4;
        int top = border.top + 2 * font.getHeight() + 12;
        int bottom = height - border.bottom - 2 * font.getHeight() - 12;
        return new Rectangle(left, top, right - left, bottom - top);
    }

    static final class Review {
        private final List<TuneLoadPlan.Change> changes;
        private final boolean[] selected;
        final JPanel content = new JPanel(new BorderLayout(8, 8));
        private final JTable changeTable;
        private final JLabel previewTitle = new JLabel("Select a changed setting to review it.");
        private final JPanel preview = new JPanel(new BorderLayout(6, 6));
        private final JButton apply = new JButton();
        private final JButton applyAndBurn = new JButton("Apply and Burn");
        private JCheckBox selectAll;
        private final JButton cancel = new JButton();
        private final JLabel partialSelection = new JLabel("<html><b>New</b> shows the full incoming tune. "
            + "If you skip unit or axis settings, the selected values may be interpreted differently.</html>");
        private Consumer<Decision> onDecision = result -> { };

        Review(List<TuneLoadPlan.Change> changes, List<String> warnings, boolean connected) {
            this(changes, warnings, connected, false);
        }

        Review(List<TuneLoadPlan.Change> changes, List<String> warnings, boolean connected, boolean restoring) {
            this.changes = changes;
            selected = new boolean[changes.size()];
            for (int i = 0; i < selected.length; i++) {
                selected[i] = true;
            }

            content.setBorder(BorderFactory.createEmptyBorder(10, 10, 10, 10));
            JPanel heading = new JPanel(new BorderLayout(4, 4));
            heading.add(new JLabel(changes.isEmpty()
                    ? "<html><b>No valid tune differences to apply.</b></html>"
                    : connected
                         ? "<html><b>Review tune changes</b> — choose which settings to apply to the ECU.</html>"
                         : "<html><b>Review tune changes</b> — choose which settings to keep in the offline tune.</html>"),
                    BorderLayout.NORTH);
            if (!warnings.isEmpty()) {
                JTextArea warningText = new JTextArea(String.join("\n", warnings));
                warningText.setEditable(false);
                warningText.setLineWrap(true);
                warningText.setWrapStyleWord(true);
                warningText.setBorder(BorderFactory.createTitledBorder("Warnings"));
                JScrollPane warningScroll = new JScrollPane(warningText);
                warningScroll.setPreferredSize(new Dimension(600, 100));
                heading.add(warningScroll, BorderLayout.CENTER);
            }
            heading.add(partialSelection, BorderLayout.SOUTH);
            content.add(heading, BorderLayout.NORTH);

            changeTable = new JTable(new ChangeTableModel());
            changeTable.setSelectionMode(ListSelectionModel.SINGLE_SELECTION);
            changeTable.setFillsViewportHeight(true);
            changeTable.getColumnModel().getColumn(0).setMaxWidth(64);
            changeTable.getColumnModel().getColumn(0).setPreferredWidth(56);
            changeTable.getColumnModel().getColumn(1).setPreferredWidth(230);
            changeTable.getSelectionModel().addListSelectionListener(e -> {
                if (!e.getValueIsAdjusting() && changeTable.getSelectedRow() >= 0) {
                    showPreview(changes.get(changeTable.convertRowIndexToModel(changeTable.getSelectedRow())));
                }
            });
            if (!changes.isEmpty()) {
                changeTable.setRowSelectionInterval(0, 0);
            } else {
                previewTitle.setText("The tune review found no valid differences to apply.");
            }

            selectAll = new JCheckBox("Select all changes", !changes.isEmpty());
            selectAll.setEnabled(!changes.isEmpty());
            selectAll.addActionListener(e -> {
                if (changeTable.isEditing()) {
                    changeTable.getCellEditor().cancelCellEditing();
                }
                for (int i = 0; i < selected.length; i++) {
                    selected[i] = selectAll.isSelected();
                }
                changeTable.repaint();
                updateApply();
            });
            JPanel listPanel = new JPanel(new BorderLayout(4, 4));
            listPanel.add(selectAll, BorderLayout.NORTH);
            listPanel.add(new JScrollPane(changeTable), BorderLayout.CENTER);
            listPanel.setBorder(BorderFactory.createTitledBorder("Changed settings"));

            preview.setBorder(BorderFactory.createEmptyBorder(4, 4, 4, 4));
            preview.add(previewTitle, BorderLayout.NORTH);
            JSplitPane split = new JSplitPane(JSplitPane.HORIZONTAL_SPLIT, listPanel, preview);
            split.setDividerLocation(300);
            content.add(split, BorderLayout.CENTER);

            JPanel actions = new JPanel(new BorderLayout(8, 0));
            String caution = restoring ? "Restoring is one undoable edit. No changes are burned or saved to disk."
                    : connected
                    ? "Apply to RAM is temporary. Apply and Burn saves the affected pages, including any edits already in ECU RAM on those pages."
                    : "Changes are made to the offline tune only.";
            String runningWarning = " If the engine is running, applying tune changes may affect operation.";
            actions.add(new JLabel("<html>" + caution + (connected ? runningWarning : "") + "</html>"),
                    BorderLayout.CENTER);
            JPanel buttons = new JPanel();
            cancel.setText(changes.isEmpty() ? "Close" : "Cancel");
            apply.setText(restoring ? "Restore selected as changes" : connected ? "Apply selected to ECU RAM" : "Apply selected offline");
            apply.setToolTipText(restoring ? "Restore only the selected settings without burning or saving."
                : connected
                    ? "Apply selected tune changes to ECU RAM. Burn/save is a separate action."
                    : "Accept selected tune changes for offline handling; no ECU is connected.");
            apply.addActionListener(e -> accept(false));
            applyAndBurn.setToolTipText("Apply selected changes and save the affected ECU pages to flash.");
            applyAndBurn.addActionListener(e -> accept(true));
            cancel.addActionListener(e -> onDecision.accept(null));
            buttons.add(cancel);
            buttons.add(apply);
            if (connected && !restoring) {
                buttons.add(applyAndBurn);
            }
            actions.add(buttons, BorderLayout.EAST);
            content.add(actions, BorderLayout.SOUTH);
            updateApply();
        }

        private void accept(boolean burn) {
            if (changeTable.isEditing() && !changeTable.getCellEditor().stopCellEditing()) {
                return;
            }
            Set<TuneLoadPlan.Change> picked = new LinkedHashSet<>();
            for (int i = 0; i < selected.length; i++) {
                if (selected[i]) {
                    picked.add(changes.get(i));
                }
            }
            if (!picked.isEmpty()) {
                onDecision.accept(new Decision(picked, burn));
            }
        }

        private void showPreview(TuneLoadPlan.Change change) {
            preview.removeAll();
            previewTitle.setText(change.label());
            preview.add(previewTitle, BorderLayout.NORTH);
            if (change.isLua() && change.tables.isEmpty() && change.settings().isEmpty()) {
                preview.add(luaPreview(change), BorderLayout.CENTER);
            } else if (!change.tables.isEmpty() || change.isLua()) {
                preview.add(tablePreview(change), BorderLayout.CENTER);
            } else {
                preview.add(settingPreview(change.details()), BorderLayout.CENTER);
            }
            preview.revalidate();
            preview.repaint();
        }

        private static Component settingPreview(List<TuneLoadPlan.Detail> changed) {
            JTable table = new JTable(new AbstractTableModel() {
                private final String[] columns = {"Setting", "Previous", "New"};

                @Override
                public int getRowCount() {
                    return changed.size();
                }

                @Override
                public int getColumnCount() {
                    return columns.length;
                }

                @Override
                public String getColumnName(int column) {
                    return columns[column];
                }

                @Override
                public Object getValueAt(int row, int column) {
                    TuneLoadPlan.Detail detail = changed.get(row);
                    if (column == 0) {
                        return detail.label();
                    }
                    return column == 1 ? detail.current() : detail.incoming();
                }
            });
            table.setAutoCreateRowSorter(true);
            table.setFillsViewportHeight(true);
            return new JScrollPane(table);
        }

        private static Component tablePreview(TuneLoadPlan.Change change) {
            List<TuneLoadPlan.Detail> settings = change.settings();
            if (change.tables.size() == 1 && settings.isEmpty() && !change.isLua()) {
                return comparisonPanel(change.tables.get(0));
            }
            JTabbedPane tables = new JTabbedPane();
            change.tables.forEach(comparison -> tables.addTab(comparison.title, comparisonPanel(comparison)));
            if (change.isLua()) {
                tables.addTab("Lua script", luaPreview(change));
            }
            if (!settings.isEmpty()) {
                tables.addTab("Other changed settings", settingPreview(settings));
            }
            return tables;
        }

        private static Component comparisonPanel(TuneLoadPlan.TableComparison comparison) {
            JTable previous = valueGrid(comparison, comparison.previous);
            JTable incoming = valueGrid(comparison, comparison.incoming);
            FontMetrics metrics = previous.getFontMetrics(previous.getFont().deriveFont(Font.BOLD));
            for (int col = 0; col < previous.getColumnCount(); col++) {
                int width = 64;
                for (int row = 0; row < comparison.previous.length; row++) {
                    width = Math.max(width, Math.max(
                        metrics.stringWidth(comparison.previous[row][col]),
                        metrics.stringWidth(comparison.incoming[row][col])) + 18);
                }
                previous.getColumnModel().getColumn(col).setPreferredWidth(width);
                incoming.getColumnModel().getColumn(col).setPreferredWidth(width);
            }
            JScrollPane previousScroll = new JScrollPane(previous);
            JScrollPane incomingScroll = new JScrollPane(incoming);
            syncScrollbars(previousScroll.getHorizontalScrollBar(), incomingScroll.getHorizontalScrollBar());
            syncScrollbars(previousScroll.getVerticalScrollBar(), incomingScroll.getVerticalScrollBar());
            JSplitPane tables = new JSplitPane(JSplitPane.HORIZONTAL_SPLIT,
                titled(withUnits("Previous", comparison.previousUnits), previousScroll),
                titled(withUnits("New", comparison.incomingUnits), incomingScroll));
            tables.setResizeWeight(0.5);
            JPanel tablePanel = new JPanel(new BorderLayout(0, 4));
            if (comparison.omittedSeries) {
                tablePanel.add(new JLabel("Only tune-backed or configured series are shown; calculated analyzer data is unavailable."),
                    BorderLayout.NORTH);
            } else if (!comparison.hasVisibleDifference()) {
                tablePanel.add(new JLabel("No visible differences in this view."), BorderLayout.NORTH);
            }
            tablePanel.add(tables, BorderLayout.CENTER);
            if (comparison.previousPoints == null) {
                return tablePanel;
            }
            JComponent plot;
            if (comparison.hasSharedCurveAxes()) {
                plot = new CurveComparisonPlot(comparison, true, true);
            } else {
                JSplitPane separate = new JSplitPane(JSplitPane.HORIZONTAL_SPLIT,
                    new CurveComparisonPlot(comparison, true, false), new CurveComparisonPlot(comparison, false, true));
                separate.setResizeWeight(0.5);
                plot = separate;
            }
            JSplitPane curve = new JSplitPane(JSplitPane.VERTICAL_SPLIT, plot, tablePanel);
            curve.setResizeWeight(0.6);
            return curve;
        }

        private static String withUnits(String label, String units) {
            return units.isEmpty() ? label : label + " (" + units + ")";
        }

        private static JTable valueGrid(TuneLoadPlan.TableComparison comparison, String[][] cells) {
            JTable grid = new JTable(new AbstractTableModel() {
                @Override
                public int getRowCount() {
                    return cells.length - 1;
                }

                @Override
                public int getColumnCount() {
                    return cells[0].length;
                }

                @Override
                public String getColumnName(int column) {
                    return cells[0][column];
                }

                @Override
                public Object getValueAt(int row, int column) {
                    return cells[row + 1][column];
                }
            });
            grid.setAutoResizeMode(JTable.AUTO_RESIZE_OFF);
            grid.setFillsViewportHeight(true);
            grid.getTableHeader().setReorderingAllowed(false);
            grid.setDefaultRenderer(Object.class, new DefaultTableCellRenderer() {
                @Override
                public Component getTableCellRendererComponent(JTable table, Object value, boolean selected,
                                                                boolean focus, int row, int col) {
                    super.getTableCellRendererComponent(table, value, selected, focus, row, col);
                    boolean changed = comparison.isChanged(row + 1, col);
                    setHorizontalAlignment(SwingConstants.RIGHT);
                    setFont(table.getFont().deriveFont(changed ? Font.BOLD : Font.PLAIN));
                    if (!selected) {
                        setBackground(changed ? TuningStyle.color("Console.Tuning.unburned.background", table.getBackground())
                            : table.getBackground());
                    }
                    return this;
                }
            });
            javax.swing.table.TableCellRenderer header = grid.getTableHeader().getDefaultRenderer();
            grid.getTableHeader().setDefaultRenderer((table, value, selected, focus, row, col) -> {
                Component component = header.getTableCellRendererComponent(table, value, selected, focus, row, col);
                boolean changed = comparison.isChanged(0, col);
                component.setFont(grid.getTableHeader().getFont().deriveFont(changed ? Font.BOLD : Font.PLAIN));
                component.setForeground(changed ? TuningStyle.dirty() : grid.getTableHeader().getForeground());
                return component;
            });
            return grid;
        }

        private static Component luaPreview(TuneLoadPlan.Change change) {
            JTextArea current = luaText(change.current());
            JTextArea incoming = luaText(change.incoming());
            JScrollPane currentScroll = numbered(current);
            JScrollPane incomingScroll = numbered(incoming);
            syncScrollbars(currentScroll.getVerticalScrollBar(), incomingScroll.getVerticalScrollBar());
            JSplitPane panes = new JSplitPane(JSplitPane.HORIZONTAL_SPLIT,
                    titled("Previous", currentScroll), titled("New", incomingScroll));
            panes.setResizeWeight(0.5);
            return panes;
        }

        private static void syncScrollbars(JScrollBar first, JScrollBar second) {
            boolean[] syncing = {false};
            first.addAdjustmentListener(e -> syncScroll(second, e.getValue(), syncing));
            second.addAdjustmentListener(e -> syncScroll(first, e.getValue(), syncing));
        }

        private static void syncScroll(JScrollBar other, int position, boolean[] syncing) {
            if (syncing[0]) {
                return;
            }
            syncing[0] = true;
            try {
                other.setValue(position);
            } finally {
                syncing[0] = false;
            }
        }

        private static JTextArea luaText(String text) {
            JTextArea area = new JTextArea(text);
            area.setEditable(false);
            area.setFont(ConsoleFonts.mono(Font.PLAIN, area.getFont().getSize()));
            area.setCaretPosition(0);
            return area;
        }

        private static JScrollPane numbered(JTextArea area) {
            JScrollPane scroll = new JScrollPane(area);
            JTextArea lines = new JTextArea();
            lines.setEditable(false);
            lines.setFocusable(false);
            lines.setFont(area.getFont());
            int count = Math.max(1, area.getLineCount());
            StringBuilder numbers = new StringBuilder();
            for (int line = 1; line <= count; line++) {
                numbers.append(line).append('\n');
            }
            lines.setText(numbers.toString());
            lines.setBackground(scroll.getBackground());
            scroll.setRowHeaderView(lines);
            return scroll;
        }

        private static JPanel titled(String title, Component component) {
            JPanel panel = new JPanel(new BorderLayout());
            panel.setBorder(BorderFactory.createTitledBorder(title));
            panel.add(component, BorderLayout.CENTER);
            return panel;
        }

        private void updateApply() {
            boolean allSelected = true;
            for (boolean include : selected) {
                allSelected &= include;
            }
            if (selectAll != null) {
                selectAll.setSelected(allSelected && selected.length > 0);
            }
            boolean anySelected = false;
            for (boolean include : selected) {
                anySelected |= include;
            }
            partialSelection.setVisible(anySelected && !allSelected);
            apply.setEnabled(anySelected);
            applyAndBurn.setEnabled(anySelected);
        }

        private static final class CurveComparisonPlot extends JComponent {
            private static final int TICK_COUNT = 5;
            private final TuneLoadPlan.TableComparison comparison;
            private final boolean showPrevious, showIncoming;
            private final double[] min = {Double.POSITIVE_INFINITY, Double.POSITIVE_INFINITY};
            private final double[] max = {Double.NEGATIVE_INFINITY, Double.NEGATIVE_INFINITY};

            CurveComparisonPlot(TuneLoadPlan.TableComparison comparison, boolean showPrevious, boolean showIncoming) {
                this.comparison = comparison;
                this.showPrevious = showPrevious;
                this.showIncoming = showIncoming;
                setPreferredSize(new Dimension(450, 270));
                setMinimumSize(new Dimension(180, 150));
                boolean trimmed = showPrevious && comparison.previous.length - 1 > comparison.previousPoints.length
                    || showIncoming && comparison.incoming.length - 1 > comparison.incomingPoints.length;
                String title = showPrevious && showIncoming ? "Curve comparison" : showPrevious ? "Previous curve" : "New curve";
                setBorder(BorderFactory.createTitledBorder(title + (trimmed ? " (firmware padding omitted)" : "")));
                setToolTipText(showPrevious && showIncoming ? "Previous is dashed; New is solid. Both curves use the same axes."
                    : "The axis labels differ between tunes, so each curve uses its own axes.");
                for (double[][] points : new double[][][]{showPrevious ? comparison.previousPoints : new double[0][],
                        showIncoming ? comparison.incomingPoints : new double[0][]}) {
                    for (double[] point : points) {
                        if (Double.isFinite(point[0]) && Double.isFinite(point[1])) {
                            for (int axis = 0; axis < 2; axis++) {
                                min[axis] = Math.min(min[axis], point[axis]);
                                max[axis] = Math.max(max[axis], point[axis]);
                            }
                        }
                    }
                }
                for (int axis = 0; axis < 2; axis++) {
                    if (min[axis] == max[axis]) {
                        double padding = Math.max(1, Math.abs(min[axis]) * 0.05);
                        min[axis] -= padding;
                        max[axis] += padding;
                    }
                }
            }

            @Override
            protected void paintComponent(Graphics graphics) {
                super.paintComponent(graphics);
                if (!Double.isFinite(min[0])) {
                    Insets border = getInsets();
                    graphics.setColor(TuningStyle.text());
                    graphics.drawString("No calibration points to plot.", border.left + 10,
                        border.top + graphics.getFontMetrics().getHeight());
                    return;
                }
                Graphics2D g = (Graphics2D) graphics.create();
                try {
                    g.setRenderingHint(RenderingHints.KEY_ANTIALIASING, RenderingHints.VALUE_ANTIALIAS_ON);
                    FontMetrics metrics = g.getFontMetrics();
                    String[] xTicks = ticks(0);
                    String[] yTicks = ticks(1);
                    Insets border = getInsets();
                    Rectangle plot = curvePlotArea(getWidth(), getHeight(), border, metrics, xTicks, yTicks);
                    int left = plot.x, top = plot.y, right = left + plot.width, bottom = top + plot.height;
                    int line = metrics.getHeight();
                    int legendBaseline = border.top + 6 + metrics.getAscent();
                    int legendX = border.left + 10;
                    if (showPrevious) {
                        drawLegend(g, legendX, legendBaseline, true, TuningStyle.text());
                    }
                    if (showIncoming) {
                        int newLegendX = showPrevious ? legendX + 52 + metrics.stringWidth("Previous") : legendX;
                        drawLegend(g, newLegendX, legendBaseline, false, TuningStyle.dirty());
                    }
                    if (plot.width < 1 || plot.height < 1) {
                        return;
                    }
                    int center = left + plot.width / 2;
                    g.setColor(TuningStyle.separator());
                    g.drawRect(left, top, plot.width, plot.height);
                    g.setColor(TuningStyle.text());
                    String[] labels = showPrevious ? comparison.previous[0] : comparison.incoming[0];
                    drawAxisTitle(g, labels[1], center, legendBaseline + line + 4,
                        getWidth() - border.right);
                    drawAxisTitle(g, labels[0], center,
                        bottom + metrics.getAscent() + line + 9, getWidth() - border.right);
                    int tickInterval = TICK_COUNT - 1;
                    for (int i = 0; i < TICK_COUNT; i++) {
                        int x = left + plot.width * i / tickInterval;
                        int y = bottom - plot.height * i / tickInterval;
                        // Endpoints still fit when there isn't room for all the intermediate labels.
                        if (i == 0 || i == tickInterval || plot.width / tickInterval > metrics.stringWidth(xTicks[i]) + 8) {
                            g.drawString(xTicks[i], x - metrics.stringWidth(xTicks[i]) / 2, bottom + metrics.getAscent() + 5);
                        }
                        if (i == 0 || i == tickInterval || plot.height / tickInterval >= line) {
                            g.drawString(yTicks[i], left - metrics.stringWidth(yTicks[i]) - 6, y + metrics.getAscent() / 2);
                        }
                    }
                    if (showIncoming) {
                        drawSeries(g, comparison.incomingPoints, left, top, right, bottom, false, TuningStyle.dirty());
                    }
                    if (showPrevious) {
                        drawSeries(g, comparison.previousPoints, left, top, right, bottom, true, TuningStyle.text());
                    }
                } finally {
                    g.dispose();
                }
            }

            private String[] ticks(int axis) {
                String[] ticks = new String[TICK_COUNT];
                for (int i = 0; i < TICK_COUNT; i++) {
                    double value = min[axis] + (max[axis] - min[axis]) * i / (TICK_COUNT - 1);
                    ticks[i] = com.rusefi.config.StringFormatter.niceToString(value, 3);
                }
                return ticks;
            }

            private static void drawAxisTitle(Graphics2D g, String text, int center, int baseline, int rightEdge) {
                int width = g.getFontMetrics().stringWidth(text);
                g.drawString(text, Math.max(4, Math.min(center - width / 2, rightEdge - width - 4)), baseline);
            }

            private static void drawLegend(Graphics2D g, int x, int baseline, boolean previous, Color color) {
                styleSeries(g, previous, color);
                int y = baseline - g.getFontMetrics().getAscent() / 3;
                g.drawLine(x, y, x + 24, y);
                g.drawString(previous ? "Previous" : "New", x + 30, baseline);
            }

            private static void styleSeries(Graphics2D g, boolean previous, Color color) {
                g.setColor(color);
                g.setStroke(previous ? new BasicStroke(2, BasicStroke.CAP_BUTT, BasicStroke.JOIN_ROUND, 10,
                    new float[]{6, 5}, 0) : new BasicStroke(3));
            }

            private void drawSeries(Graphics2D g, double[][] points, int left, int top, int right, int bottom,
                                    boolean previous, Color color) {
                styleSeries(g, previous, color);
                Path2D path = new Path2D.Double();
                boolean penDown = false;
                for (double[] point : points) {
                    if (!Double.isFinite(point[0]) || !Double.isFinite(point[1])) {
                        penDown = false;
                        continue;
                    }
                    double x = left + (point[0] - min[0]) / (max[0] - min[0]) * (right - left);
                    double y = bottom - (point[1] - min[1]) / (max[1] - min[1]) * (bottom - top);
                    if (penDown) {
                        path.lineTo(x, y);
                    } else {
                        path.moveTo(x, y);
                        penDown = true;
                    }
                    g.fillOval((int) x - 2, (int) y - 2, 5, 5);
                }
                g.draw(path);
            }
        }

        private final class ChangeTableModel extends AbstractTableModel {
            private final String[] columns = {"Apply", "Setting"};

            @Override
            public int getRowCount() {
                return changes.size();
            }

            @Override
            public int getColumnCount() {
                return columns.length;
            }

            @Override
            public String getColumnName(int column) {
                return columns[column];
            }

            @Override
            public Class<?> getColumnClass(int column) {
                return column == 0 ? Boolean.class : String.class;
            }

            @Override
            public boolean isCellEditable(int row, int column) {
                return column == 0;
            }

            @Override
            public Object getValueAt(int row, int column) {
                return column == 0 ? selected[row] : changes.get(row).label();
            }

            @Override
            public void setValueAt(Object value, int row, int column) {
                if (column == 0) {
                    selected[row] = Boolean.TRUE.equals(value);
                    fireTableCellUpdated(row, column);
                    updateApply();
                }
            }
        }
    }
}
