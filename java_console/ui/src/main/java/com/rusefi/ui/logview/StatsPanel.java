package com.rusefi.ui.logview;

import com.rusefi.logview.CsvLogReader;
import com.rusefi.ui.widgets.ConsoleFonts;

import javax.swing.BorderFactory;
import javax.swing.JButton;
import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.JScrollPane;
import javax.swing.JTable;
import javax.swing.SwingConstants;
import javax.swing.table.AbstractTableModel;
import javax.swing.table.DefaultTableCellRenderer;
import java.awt.BorderLayout;
import java.awt.Color;
import java.awt.Component;
import java.awt.Dimension;
import java.awt.Font;
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
    static final String[] COLUMNS = {"Signal", "Cursor", "Min", "Max", "Avg"};

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
        table.getColumnModel().getColumn(0).setPreferredWidth(150);
        for (int column = 1; column < COLUMNS.length; column++) {
            table.getColumnModel().getColumn(column).setPreferredWidth(60);
        }
        HeaderRenderer header = new HeaderRenderer();
        for (int column = 0; column < COLUMNS.length; column++) {
            table.getColumnModel().getColumn(column).setHeaderRenderer(header);
        }
        table.setDefaultRenderer(Object.class, new CellRenderer());
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
        TimeWindow window = chart.timeWindow();
        footer.setText(model.rows.isEmpty()
                ? "Plot signals to see their statistics"
                : String.format(Locale.ROOT, "<html>Min, max and avg over the visible %.2f–%.2f s<br>"
                + "Hover a row to highlight its trace; click to keep it highlighted</html>",
                window.start(), window.end()));
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
            return COLUMNS.length;
        }

        @Override
        public String getColumnName(int column) {
            return COLUMNS[column];
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
            if (column == 1) {
                int sample = chart.cursorSample();
                return sample < 0 ? ValueFormat.NONE : ValueFormat.format(log, row.field, log.value(sample, row.field));
            }
            TraceStats stats = chart.visibleStats().get(row.field);
            if (stats.isEmpty()) {
                return ValueFormat.NONE;
            }
            double value = column == 2 ? stats.min : column == 3 ? stats.max : stats.average;
            return ValueFormat.format(log, row.field, value);
        }
    }

    private static String escape(String text) {
        return text.replace("&", "&amp;").replace("<", "&lt;").replace(">", "&gt;");
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
                    BorderFactory.createEmptyBorder(6, column == 0 ? 12 : 4, 6, 12)));
            return this;
        }
    }

    private final class CellRenderer extends DefaultTableCellRenderer {
        private final TraceDot dot = new TraceDot();

        @Override
        public Component getTableCellRendererComponent(JTable table, Object value, boolean selected, boolean focused,
                                                       int rowIndex, int column) {
            super.getTableCellRendererComponent(table, value, false, false, rowIndex, column);
            Row row = model.rows.get(rowIndex);
            Color muted = themed("Console.LogViewer.mutedForeground", Color.GRAY);
            setIcon(null);
            setBorder(BorderFactory.createEmptyBorder(0, column == 0 ? 12 : 4, 0, 12));
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
                setIcon(dot);
                setIconTextGap(9);
                String unit = log.unit(row.field);
                // the unit in the muted colour, after the name
                setText(unit.isEmpty() ? log.name(row.field) : String.format("<html>%s&nbsp;&nbsp;<font color='#%06x'>%s</font></html>",
                        escape(log.name(row.field)), muted.getRGB() & 0xffffff, escape(unit)));
                setToolTipText(unit.isEmpty() ? log.name(row.field) : log.name(row.field) + " (" + unit + ")");
            } else {
                setFont(ConsoleFonts.mono(pinned ? Font.BOLD : Font.PLAIN, table.getFont().getSize()));
            }
            return this;
        }
    }
}
