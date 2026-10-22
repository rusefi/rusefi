package com.rusefi.ui.logview;

import com.rusefi.logview.CsvLogReader;
import com.rusefi.ui.widgets.ConsoleFonts;

import javax.swing.BorderFactory;
import javax.swing.Icon;
import javax.swing.JButton;
import javax.swing.JComponent;
import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.JScrollPane;
import javax.swing.JSplitPane;
import javax.swing.JTable;
import javax.swing.SwingConstants;
import javax.swing.table.AbstractTableModel;
import javax.swing.table.DefaultTableCellRenderer;
import java.awt.BorderLayout;
import java.awt.Color;
import java.awt.Component;
import java.awt.Dimension;
import java.awt.Font;
import java.awt.FontMetrics;
import java.awt.Graphics;
import java.awt.Graphics2D;
import java.awt.RenderingHints;
import java.awt.event.MouseAdapter;
import java.awt.event.MouseEvent;
import java.awt.event.MouseMotionAdapter;
import java.util.ArrayList;
import java.util.List;
import java.util.Locale;

import static com.rusefi.ui.logview.ThemedStrip.themed;

/**
 * Table of every plotted signal with its value at the cursor and the minimum, maximum and average of the
 * visible time window, grouped by plot when several are stacked. Opened from the toolbar's Stats button.
 * [tag:console_theme]
 */
final class StatsPanel extends JPanel {
    static final int DEFAULT_WIDTH = 400;
    /** Room for the A, B and Δ columns. */
    static final int MEASURE_WIDTH = 480;
    static final String[] COLUMNS = {"Signal", "Cursor", "Min", "Max", "Avg"};
    /** While both measurement cursors are set: their values and the change instead of the cursor column. */
    static final String[] MEASURE_COLUMNS = {"Signal", "A", "B", "Δ", "Min", "Max", "Avg"};

    private final LogChart chart;
    private final StatsModel model = new StatsModel();
    final JTable table = new JTable(model);
    final JLabel footer = ChartToolbar.muted(new JLabel());
    final JButton closeButton = new JButton("×");

    /** A plot heading (field -1) or a signal row. */
    static final class Row {
        final int plot;
        final int field;

        Row(int plot, int field) {
            this.plot = plot;
            this.field = field;
        }

        boolean isHeading() {
            return field < 0;
        }
    }

    StatsPanel(LogChart chart, Runnable onClose) {
        super(new BorderLayout());
        this.chart = chart;
        Color background = themed("Console.LogViewer.sidebar.background", Color.WHITE);
        Color line = themed("Separator.foreground", Color.LIGHT_GRAY);
        setBackground(background);
        setBorder(BorderFactory.createMatteBorder(0, 1, 0, 0, line));
        setPreferredSize(new Dimension(DEFAULT_WIDTH, 100));

        JLabel title = new JLabel("Statistics");
        title.setFont(title.getFont().deriveFont(Font.BOLD));
        closeButton.setToolTipText("Close the statistics");
        closeButton.setBorderPainted(false);
        closeButton.setContentAreaFilled(false);
        closeButton.setFocusPainted(false);
        closeButton.setForeground(themed("Console.LogViewer.mutedForeground", Color.GRAY));
        closeButton.addActionListener(e -> onClose.run());
        JPanel heading = new JPanel(new BorderLayout());
        heading.setOpaque(false);
        heading.setBorder(BorderFactory.createEmptyBorder(8, 12, 6, 4));
        heading.add(title, BorderLayout.WEST);
        heading.add(closeButton, BorderLayout.EAST);

        table.setRowHeight(28);
        table.setShowGrid(false);
        table.setIntercellSpacing(new Dimension(0, 0));
        table.setFillsViewportHeight(true);
        table.setFocusable(false);
        table.setRowSelectionAllowed(false);
        table.setBackground(background);
        table.getTableHeader().setReorderingAllowed(false);
        setUpColumns();
        table.setDefaultRenderer(Object.class, new CellRenderer());
        chart.cursors().addListener(this::cursorsChanged);
        // hovering a row highlights its trace on the plots, a click keeps it highlighted
        table.addMouseMotionListener(new MouseMotionAdapter() {
            @Override
            public void mouseMoved(MouseEvent event) {
                int field = fieldAt(table.rowAtPoint(event.getPoint()));
                if (field >= 0) {
                    chart.focus().hover(field);
                } else {
                    chart.focus().clearHover();
                }
            }
        });
        table.addMouseListener(new MouseAdapter() {
            @Override
            public void mouseExited(MouseEvent event) {
                chart.focus().clearHover();
            }

            @Override
            public void mouseClicked(MouseEvent event) {
                int field = fieldAt(table.rowAtPoint(event.getPoint()));
                if (field >= 0) {
                    chart.focus().togglePin(field);
                }
            }
        });
        chart.focus().addListener(table::repaint);
        JScrollPane scroll = new JScrollPane(table);
        // a scroll pane only adopts the table header once it is in a window; attach it now
        scroll.setColumnHeaderView(table.getTableHeader());
        scroll.setBorder(BorderFactory.createMatteBorder(1, 0, 1, 0, line));
        scroll.getViewport().setBackground(background);

        footer.setBorder(BorderFactory.createEmptyBorder(8, 12, 10, 12));
        add(heading, BorderLayout.NORTH);
        add(scroll, BorderLayout.CENTER);
        add(footer, BorderLayout.SOUTH);
        refresh();
    }

    /** The columns shown: with A and B set, their values and the change replace the cursor column. */
    String[] columns() {
        // the table asks while it is built, before the chart is set
        return chart != null && chart.cursors().isRange() ? MEASURE_COLUMNS : COLUMNS;
    }

    private void setUpColumns() {
        boolean measuring = table.getColumnCount() > COLUMNS.length;
        table.getColumnModel().getColumn(0).setPreferredWidth(measuring ? 120 : 150);
        for (int column = 1; column < table.getColumnCount(); column++) {
            table.getColumnModel().getColumn(column).setPreferredWidth(measuring ? 52 : 60);
        }
        HeaderRenderer header = new HeaderRenderer();
        for (int column = 0; column < table.getColumnCount(); column++) {
            table.getColumnModel().getColumn(column).setHeaderRenderer(header);
        }
    }

    private void cursorsChanged() {
        if (table.getColumnCount() != columns().length) {
            model.fireTableStructureChanged();
            setUpColumns();
            widenForMeasureColumns();
        }
        refreshValues();
    }

    /** Makes room for the A, B and Δ columns when the panel is narrower; never narrows it. */
    private void widenForMeasureColumns() {
        if (!chart.cursors().isRange()) {
            return;
        }
        setPreferredSize(new Dimension(Math.max(getPreferredSize().width, MEASURE_WIDTH), 100));
        if (getParent() instanceof JSplitPane && getWidth() > 0 && getWidth() < MEASURE_WIDTH) {
            JSplitPane split = (JSplitPane) getParent();
            split.setDividerLocation(Math.max(0, split.getWidth() - split.getDividerSize() - MEASURE_WIDTH));
        }
    }

    /** Right padding of a value cell: tighter while the seven measure columns share the width. */
    private int cellRightPadding() {
        return table.getColumnCount() > COLUMNS.length ? 6 : 12;
    }

    /** Rebuilds the rows after the plots change. */
    void refresh() {
        model.rows = rows(chart.plotLayout());
        model.fireTableDataChanged();
        refreshFooter();
    }

    /** Updates the values after zoom, pan or a cursor move; the rows stay the same. */
    void refreshValues() {
        if (!model.rows.isEmpty()) {
            model.fireTableRowsUpdated(0, model.rows.size() - 1);
        }
        refreshFooter();
    }

    private void refreshFooter() {
        VisibleStats stats = chart.visibleStats();
        footer.setText(model.rows.isEmpty()
                ? "Plot signals to see their statistics"
                : String.format(Locale.ROOT, "<html>Min, max and avg %s %.2f–%.2f s<br>"
                + "Hover a row to highlight its trace; click to keep it highlighted</html>",
                stats.isBetweenCursors() ? "between A and B," : "over the visible",
                stats.rangeStart(), stats.rangeEnd()));
    }

    /** Plot headings only when several plots are stacked. */
    static List<Row> rows(PlotLayout layout) {
        List<Row> rows = new ArrayList<>();
        boolean headings = layout.plotCount() > 1;
        for (int plot = 0; plot < layout.plotCount(); plot++) {
            if (headings) {
                rows.add(new Row(plot, -1));
            }
            for (int field : layout.fields(plot)) {
                rows.add(new Row(plot, field));
            }
        }
        return rows;
    }

    List<Row> currentRows() {
        return model.rows;
    }

    /** The signal of a table row, or -1 for a plot heading or no row. */
    int fieldAt(int row) {
        return row >= 0 && row < model.rows.size() ? model.rows.get(row).field : -1;
    }

    /** Text shown in a cell, as the table renders it. */
    String text(int row, int column) {
        Object value = model.getValueAt(row, column);
        return value instanceof Row ? chart.log().name(((Row) value).field) : String.valueOf(value);
    }

    private final class StatsModel extends AbstractTableModel {
        private List<Row> rows = new ArrayList<>();

        @Override
        public int getRowCount() {
            return rows.size();
        }

        @Override
        public int getColumnCount() {
            return columns().length;
        }

        @Override
        public String getColumnName(int column) {
            return columns()[column];
        }

        @Override
        public Object getValueAt(int rowIndex, int column) {
            Row row = rows.get(rowIndex);
            if (row.isHeading()) {
                return column == 0 ? "PLOT " + (row.plot + 1) : "";
            }
            if (column == 0) {
                return row;
            }
            CsvLogReader log = chart.log();
            String name = getColumnName(column);
            switch (name) {
                case "Cursor": {
                    int sample = chart.cursorSample();
                    return sample < 0 ? ValueFormat.NONE : ValueFormat.format(log, row.field, log.value(sample, row.field));
                }
                case "A":
                case "B":
                    return ValueFormat.format(log, row.field,
                            chart.valueAt(MeasureCursors.Which.valueOf(name), row.field));
                case "Δ":
                    return chart.delta(row.field);
                default:
                    break;
            }
            TraceStats stats = chart.visibleStats().get(row.field);
            if (stats.isEmpty()) {
                return ValueFormat.NONE;
            }
            double value = name.equals("Min") ? stats.min : name.equals("Max") ? stats.max : stats.average;
            return ValueFormat.format(log, row.field, value);
        }
    }

    /**
     * A signal cell on one line: trace dot, name, then the unit in the muted colour. When the column is too narrow
     * the name is shortened with "…" first, then the unit is left out; the tooltip has the whole name.
     */
    static final class SignalCell extends JComponent {
        private static final int LEFT = 12;
        private static final int RIGHT = 12;
        private static final int GAP = 9;
        /** Name width kept before the unit is left out. */
        private static final int MIN_NAME = 40;
        private Icon dot;
        private String name = "";
        private String unit = "";
        private Color ink = Color.BLACK;
        private Color muted = Color.GRAY;
        private Color background = Color.WHITE;

        void set(Icon dot, String name, String unit, Font font, Color ink, Color muted, Color background) {
            this.dot = dot;
            this.name = name;
            this.unit = unit;
            this.ink = ink;
            this.muted = muted;
            this.background = background;
            setFont(font);
            setToolTipText(unit.isEmpty() ? name : name + " (" + unit + ")");
        }

        /** The name and unit as drawn in a cell {@code width} wide: {name, unit}, unit empty when left out. */
        String[] shown(int width) {
            FontMetrics metrics = getFontMetrics(getFont());
            int available = width - LEFT - (dot == null ? 0 : dot.getIconWidth() + GAP) - RIGHT;
            String unitText = unit.isEmpty() ? "" : "  " + unit;
            int unitWidth = metrics.stringWidth(unitText);
            if (metrics.stringWidth(name) + unitWidth <= available) {
                return new String[]{name, unit};
            }
            if (!unit.isEmpty() && available - unitWidth >= MIN_NAME) {
                return new String[]{shorten(name, metrics, available - unitWidth), unit};
            }
            return new String[]{shorten(name, metrics, available), ""};
        }

        static String shorten(String text, FontMetrics metrics, int width) {
            if (metrics.stringWidth(text) <= width) {
                return text;
            }
            String ellipsis = "…";
            int end = text.length();
            while (end > 0 && metrics.stringWidth(text.substring(0, end).trim() + ellipsis) > width) {
                end--;
            }
            return text.substring(0, end).trim() + ellipsis;
        }

        @Override
        protected void paintComponent(Graphics g) {
            Graphics2D g2 = (Graphics2D) g.create();
            try {
                g2.setRenderingHint(RenderingHints.KEY_TEXT_ANTIALIASING, RenderingHints.VALUE_TEXT_ANTIALIAS_ON);
                g2.setColor(background);
                g2.fillRect(0, 0, getWidth(), getHeight());
                int x = LEFT;
                if (dot != null) {
                    dot.paintIcon(this, g2, x, (getHeight() - dot.getIconHeight()) / 2);
                    x += dot.getIconWidth() + GAP;
                }
                g2.setFont(getFont());
                FontMetrics metrics = g2.getFontMetrics();
                int baseline = (getHeight() + metrics.getAscent() - metrics.getDescent()) / 2;
                String[] shown = shown(getWidth());
                g2.setColor(ink);
                g2.drawString(shown[0], x, baseline);
                if (!shown[1].isEmpty()) {
                    g2.setColor(muted);
                    g2.drawString("  " + shown[1], x + metrics.stringWidth(shown[0]), baseline);
                }
            } finally {
                g2.dispose();
            }
        }
    }

    private static final class HeaderRenderer extends DefaultTableCellRenderer {
        @Override
        public Component getTableCellRendererComponent(JTable table, Object value, boolean selected, boolean focused,
                                                       int row, int column) {
            super.getTableCellRendererComponent(table, value, false, false, row, column);
            setFont(table.getFont().deriveFont(Font.BOLD, table.getFont().getSize2D() - 1));
            setForeground(themed("Console.LogViewer.mutedForeground", Color.GRAY));
            setBackground(table.getBackground());
            setHorizontalAlignment(column == 0 ? SwingConstants.LEFT : SwingConstants.RIGHT);
            setBorder(BorderFactory.createCompoundBorder(
                    BorderFactory.createMatteBorder(0, 0, 1, 0, themed("Separator.foreground", Color.LIGHT_GRAY)),
                    BorderFactory.createEmptyBorder(6, column == 0 ? 12 : 4, 6,
                            column == 0 || table.getColumnCount() <= COLUMNS.length ? 12 : 6)));
            return this;
        }
    }

    private final class CellRenderer extends DefaultTableCellRenderer {
        private final TraceDot dot = new TraceDot();
        private final SignalCell signalCell = new SignalCell();

        @Override
        public Component getTableCellRendererComponent(JTable table, Object value, boolean selected, boolean focused,
                                                       int rowIndex, int column) {
            super.getTableCellRendererComponent(table, value, false, false, rowIndex, column);
            Row row = model.rows.get(rowIndex);
            Color muted = themed("Console.LogViewer.mutedForeground", Color.GRAY);
            setIcon(null);
            setBorder(BorderFactory.createEmptyBorder(0, column == 0 ? 12 : 4, 0, column == 0 ? 12 : cellRightPadding()));
            boolean rowFocused = !row.isHeading() && row.field == chart.focus().focused();
            boolean pinned = !row.isHeading() && row.field == chart.focus().pinned();
            setBackground(rowFocused ? themed("Console.LogViewer.chipBackground", table.getBackground()) : table.getBackground());
            setForeground(table.getForeground());
            setFont(pinned ? table.getFont().deriveFont(Font.BOLD) : table.getFont());
            setHorizontalAlignment(column == 0 ? SwingConstants.LEFT : SwingConstants.RIGHT);
            if (row.isHeading()) {
                setFont(table.getFont().deriveFont(Font.BOLD, table.getFont().getSize2D() - 1));
                setForeground(muted);
                setBackground(themed("Console.LogViewer.chipBackground", table.getBackground()));
            } else if (column == 0) {
                CsvLogReader log = chart.log();
                Color trace = chart.colorOf(row.field, chart.palette());
                // a darker outline keeps pale dark-plot colours visible on the light panel
                dot.set(trace, trace.darker());
                signalCell.set(dot, log.name(row.field), log.unit(row.field), getFont(), table.getForeground(), muted,
                        getBackground());
                return signalCell;
            } else {
                setFont(ConsoleFonts.mono(pinned ? Font.BOLD : Font.PLAIN, table.getFont().getSize()));
            }
            return this;
        }
    }
}
