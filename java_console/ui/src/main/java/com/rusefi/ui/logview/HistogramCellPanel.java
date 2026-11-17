package com.rusefi.ui.logview;

import com.formdev.flatlaf.FlatClientProperties;
import com.rusefi.logview.CsvLogReader;
import com.rusefi.ui.util.WrapLayout;
import com.rusefi.ui.widgets.ConsoleFonts;

import javax.swing.BorderFactory;
import javax.swing.Box;
import javax.swing.BoxLayout;
import javax.swing.JButton;
import javax.swing.JComponent;
import javax.swing.JLabel;
import javax.swing.JPanel;
import java.awt.BasicStroke;
import java.awt.BorderLayout;
import java.awt.Color;
import java.awt.Component;
import java.awt.Dimension;
import java.awt.FlowLayout;
import java.awt.Font;
import java.awt.FontMetrics;
import java.awt.Graphics;
import java.awt.Graphics2D;
import java.awt.GridLayout;
import java.awt.RenderingHints;
import java.awt.event.MouseEvent;
import java.util.Locale;
import java.util.function.DoubleFunction;

import static com.rusefi.ui.logview.ThemedStrip.themed;

/**
 * The selected histogram cell in detail, to the right of the table: where it is, its mean, spread, min, max,
 * time and records, how its readings spread, and how often the engine passed through. Opened and closed with
 * the toolbar's Cell button, like the timeline's Stats panel. [tag:console_theme]
 */
final class HistogramCellPanel extends JPanel {
    static final int DEFAULT_WIDTH = 300;
    static final int DISTRIBUTION_BINS = 12;
    static final String[] STATS = {"Mean", "Spread (σ)", "Min", "Max", "Time in cell", "Records"};

    final JButton closeButton = new JButton("×");
    final JLabel titleLabel = new JLabel();
    final JLabel rangeLabel = ChartToolbar.muted(new JLabel());
    final JLabel[] statValues = new JLabel[STATS.length];
    private final JLabel[] statNames = new JLabel[STATS.length];
    final JLabel visitsLabel = ChartToolbar.muted(new JLabel());
    static final String HINT = "<html>Click a cell to see its readings.<br>Hover a cell for its ranges; "
            + "Shift+click or Shift+arrows select a block to copy.</html>";
    static final String EMPTY = "<html>No records in this cell.<br>Hover it for its ranges.</html>";
    final JLabel emptyLabel = ChartToolbar.muted(new JLabel(HINT));
    final JButton timelineButton = new JButton("Show on timeline");
    final JButton copyButton = new JButton("Copy");
    final Distribution distribution = new Distribution();
    final JLabel spreadLabel = new JLabel();
    private final JPanel details = new JPanel();
    private final JPanel detailsHolder = new JPanel(new BorderLayout());

    HistogramCellPanel(Runnable onClose) {
        super(new BorderLayout());
        Color background = themed("Console.LogViewer.sidebar.background", Color.WHITE);
        setBackground(background);
        setBorder(BorderFactory.createMatteBorder(0, 1, 0, 0, themed("Separator.foreground", Color.LIGHT_GRAY)));
        setPreferredSize(new Dimension(DEFAULT_WIDTH, 100));

        JLabel heading = new JLabel("Cell");
        heading.setFont(heading.getFont().deriveFont(Font.BOLD));
        closeButton.setToolTipText("Close the cell details");
        closeButton.setBorderPainted(false);
        closeButton.setContentAreaFilled(false);
        closeButton.setFocusPainted(false);
        closeButton.setForeground(themed("Console.LogViewer.mutedForeground", Color.GRAY));
        closeButton.addActionListener(e -> onClose.run());
        JPanel top = new JPanel(new BorderLayout());
        top.setOpaque(false);
        top.setBorder(BorderFactory.createEmptyBorder(8, 12, 6, 4));
        top.add(heading, BorderLayout.WEST);
        top.add(closeButton, BorderLayout.EAST);

        titleLabel.setFont(titleLabel.getFont().deriveFont(Font.BOLD, titleLabel.getFont().getSize2D() + 4));
        JPanel stats = new JPanel(new GridLayout(0, 2, 8, 8));
        stats.setOpaque(false);
        for (int i = 0; i < STATS.length; i++) {
            statNames[i] = ChartToolbar.muted(new JLabel(STATS[i]));
            statNames[i].setFont(statNames[i].getFont().deriveFont(statNames[i].getFont().getSize2D() - 1));
            statValues[i] = new JLabel();
            statValues[i].setFont(ConsoleFonts.mono(Font.BOLD, ConsoleFonts.baseSize() + 2));
            stats.add(new Tile(statNames[i], statValues[i]));
        }
        spreadLabel.setFont(spreadLabel.getFont().deriveFont(spreadLabel.getFont().getSize2D() - 1));

        timelineButton.putClientProperty(FlatClientProperties.STYLE_CLASS, "primary");
        timelineButton.setToolTipText("Shade every stay in this cell on the timeline and step through them (Enter, "
                + "or double-click the cell)");
        copyButton.setToolTipText("Copy the selected cells (Ctrl+C; Ctrl+Shift+C with their axes)");
        // full labels at any panel width: a narrow panel puts Copy on a second line rather than cutting a label
        JPanel actions = new JPanel(new WrapLayout(FlowLayout.LEFT, 0, 4)) {
            @Override
            public Dimension getMaximumSize() {
                return new Dimension(Integer.MAX_VALUE, getPreferredSize().height);
            }
        };
        actions.setOpaque(false);
        actions.add(timelineButton);
        actions.add(Box.createHorizontalStrut(8));
        actions.add(copyButton);

        details.setLayout(new BoxLayout(details, BoxLayout.Y_AXIS));
        details.setOpaque(false);
        details.setBorder(BorderFactory.createEmptyBorder(4, 12, 12, 12));
        add(details, titleLabel, Box.createVerticalStrut(2), rangeLabel, Box.createVerticalStrut(12), stats,
                Box.createVerticalStrut(14), ChartToolbar.muted(new JLabel("Readings in this cell")),
                Box.createVerticalStrut(6), distribution, Box.createVerticalStrut(6), spreadLabel,
                Box.createVerticalStrut(12), visitsLabel, Box.createVerticalStrut(14), actions);
        emptyLabel.setBorder(BorderFactory.createEmptyBorder(4, 12, 12, 12));
        emptyLabel.setVerticalAlignment(JLabel.TOP);

        // details keep their preferred height at the top instead of spreading down the panel
        detailsHolder.setOpaque(false);
        detailsHolder.add(emptyLabel, BorderLayout.NORTH);
        add(top, BorderLayout.NORTH);
        add(detailsHolder, BorderLayout.CENTER);
    }

    private static void add(JPanel panel, Component... components) {
        for (Component component : components) {
            if (component instanceof JComponent) {
                ((JComponent) component).setAlignmentX(Component.LEFT_ALIGNMENT);
            }
            panel.add(component);
        }
    }

    boolean isShowingCell() {
        return details.getParent() == detailsHolder;
    }

    /** Shows the table's selected cell, or the hint when none is selected. */
    void show(HistogramTable table) {
        Histogram histogram = table.histogram();
        int x = table.selectedX();
        int y = table.selectedY();
        Histogram.Cell cell = histogram == null || x < 0 ? null : histogram.cell(x, y);
        detailsHolder.removeAll();
        detailsHolder.add(cell == null ? emptyLabel : details, BorderLayout.NORTH);
        emptyLabel.setText(x >= 0 && histogram != null ? EMPTY : HINT);
        if (cell != null) {
            CsvLogReader log = histogram.log;
            HistogramTable.Mixture mixture = table.mixture();
            titleLabel.setText(axisValue(histogram, histogram.xField, histogram.xAxis.center(x)) + "  ·  "
                    + axisValue(histogram, histogram.yField, histogram.yAxis.center(y)));
            rangeLabel.setText(String.format(Locale.ROOT, "%s %s–%s  ·  %s %s–%s", log.name(histogram.xField),
                    ValueFormat.format(log, histogram.xField, histogram.xAxis.lower(x)),
                    ValueFormat.format(log, histogram.xField, histogram.xAxis.upper(x)), log.name(histogram.yField),
                    ValueFormat.format(log, histogram.yField, histogram.yAxis.lower(y)),
                    ValueFormat.format(log, histogram.yField, histogram.yAxis.upper(y))));
            String unit = mixture != HistogramTable.Mixture.NONE ? mixture.unit : log.unit(histogram.zField);
            statNames[0].setText(unit.isEmpty() ? "Mean" : "Mean " + unit);
            statValues[0].setText(table.formatZ(cell.mean() * mixture.factor));
            statValues[1].setText("± " + table.formatZ(cell.sigma() * mixture.factor));
            statValues[2].setText(table.formatZ(cell.min * mixture.factor));
            statValues[3].setText(table.formatZ(cell.max * mixture.factor));
            statValues[4].setText(String.format(Locale.ROOT, "%.1f s", cell.time));
            statValues[5].setText(String.format(Locale.ROOT, "%,d", cell.count));
            Histogram.Detail detail = histogram.detail(x, y, DISTRIBUTION_BINS);
            distribution.set(detail, mixture.factor, table::formatZ);
            spreadLabel.setText(spread(detail, mixture.factor, table));
            visitsLabel.setText(String.format(Locale.ROOT, "Visited %,d time%s  ·  longest stay %.1f s", detail.visits,
                    detail.visits == 1 ? "" : "s", detail.longestVisit));
        }
        revalidate();
        repaint();
    }

    /** "90% of readings between 12.8 and 13.4 · 3 below, 2 above", or all of them for a few readings. */
    static String spread(Histogram.Detail detail, double factor, HistogramTable table) {
        String low = table.formatZ(detail.low * factor);
        String high = table.formatZ(detail.high * factor);
        if (detail.total() < Histogram.DISTRIBUTION_MIN_RECORDS) {
            return String.format(Locale.ROOT, "All %d readings between %s and %s", detail.total(), low, high);
        }
        String text = String.format(Locale.ROOT, "90%% of readings between %s and %s", low, high);
        if (detail.below + detail.above > 0) {
            // on a second line, so the panel's default width shows it all
            text = String.format(Locale.ROOT, "<html>%s<br>%,d below, %,d above (grey bars)</html>", text,
                    detail.below, detail.above);
        }
        return text;
    }

    private static String axisValue(Histogram histogram, int field, double value) {
        String unit = histogram.log.unit(field);
        return ValueFormat.format(histogram.log, field, value) + (unit.isEmpty() ? "" : " " + unit);
    }

    /** A rounded tile with a stat's name above its value. */
    private static final class Tile extends JPanel {
        Tile(JLabel name, JLabel value) {
            super(new BorderLayout(0, 2));
            setOpaque(false);
            setBorder(BorderFactory.createEmptyBorder(7, 10, 7, 10));
            add(name, BorderLayout.NORTH);
            add(value, BorderLayout.CENTER);
        }

        @Override
        protected void paintComponent(Graphics g) {
            Graphics2D g2 = (Graphics2D) g.create();
            try {
                g2.setRenderingHint(RenderingHints.KEY_ANTIALIASING, RenderingHints.VALUE_ANTIALIAS_ON);
                g2.setColor(themed("Console.Card.sectionBackground", new Color(0xf7f8fa)));
                g2.fillRoundRect(0, 0, getWidth(), getHeight(), 10, 10);
            } finally {
                g2.dispose();
            }
        }
    }

    /**
     * Bars of how many readings fall in each part of the cell's 5th–95th percentile range, with the readings
     * beyond it as separate grey bars at either end and a dashed line at the mean. Hover a bar for its range
     * and share of the readings.
     */
    static final class Distribution extends JComponent {
        static final int BARS_HEIGHT = 56;
        static final int OUTLIER_WIDTH = 30;
        static final int OUTLIER_GAP = 8;
        private Histogram.Detail detail;
        /** Turns a logged value into the shown text (unit conversion and precision). */
        private DoubleFunction<String> format = value -> String.valueOf(value);
        private double factor = 1;

        Distribution() {
            setToolTipText("");
            int height = BARS_HEIGHT + 18;
            setPreferredSize(new Dimension(200, height));
            setMaximumSize(new Dimension(Integer.MAX_VALUE, height));
        }

        void set(Histogram.Detail detail, double factor, DoubleFunction<String> format) {
            this.detail = detail;
            this.factor = factor;
            this.format = format;
            repaint();
        }

        /** {left, right} of the main bars: the width less the outlier bars that are shown. */
        private int[] mainArea() {
            int left = detail.below > 0 ? OUTLIER_WIDTH + OUTLIER_GAP : 0;
            int right = getWidth() - (detail.above > 0 ? OUTLIER_WIDTH + OUTLIER_GAP : 0);
            return new int[]{left, Math.max(left + 1, right)};
        }

        private int peak() {
            int peak = Math.max(1, Math.max(detail.below, detail.above));
            for (int count : detail.distribution) {
                peak = Math.max(peak, count);
            }
            return peak;
        }

        private String share(int count) {
            return String.format(Locale.ROOT, "%,d reading%s (%.0f%%)", count, count == 1 ? "" : "s",
                    100.0 * count / Math.max(1, detail.total()));
        }

        /** What the bar under the pointer stands for. */
        @Override
        public String getToolTipText(MouseEvent event) {
            if (detail == null) {
                return null;
            }
            int x = event.getX();
            String low = format.apply(detail.low * factor);
            String high = format.apply(detail.high * factor);
            if (detail.below > 0 && x < OUTLIER_WIDTH) {
                return share(detail.below) + " below " + low;
            }
            if (detail.above > 0 && x >= getWidth() - OUTLIER_WIDTH) {
                return share(detail.above) + " above " + high;
            }
            int[] area = mainArea();
            if (x < area[0] || x >= area[1]) {
                return null;
            }
            int bins = detail.distribution.length;
            int bin = Math.min(bins - 1, (x - area[0]) * bins / (area[1] - area[0]));
            double from = detail.low + (detail.high - detail.low) * bin / bins;
            double to = detail.low + (detail.high - detail.low) * (bin + 1) / bins;
            return format.apply(from * factor) + "–" + format.apply(to * factor) + ": "
                    + share(detail.distribution[bin]);
        }

        @Override
        protected void paintComponent(Graphics g) {
            if (detail == null) {
                return;
            }
            Graphics2D g2 = (Graphics2D) g.create();
            try {
                g2.setRenderingHint(RenderingHints.KEY_ANTIALIASING, RenderingHints.VALUE_ANTIALIAS_ON);
                g2.setRenderingHint(RenderingHints.KEY_TEXT_ANTIALIASING, RenderingHints.VALUE_TEXT_ANTIALIAS_ON);
                int peak = peak();
                int base = BARS_HEIGHT - 1;
                int[] area = mainArea();
                Color accent = themed("Console.LogChart.light.accent", new Color(0x2563eb));
                Color line = themed("Separator.foreground", Color.LIGHT_GRAY);
                Color muted = themed("Console.LogViewer.mutedForeground", Color.GRAY);
                // the tails are not necessarily bad readings: neutral, not a warning colour
                Color outlier = muted;
                int bins = detail.distribution.length;
                double width = (double) (area[1] - area[0]) / bins;
                g2.setColor(accent);
                for (int i = 0; i < bins; i++) {
                    int left = area[0] + (int) Math.round(i * width) + 1;
                    int right = area[0] + (int) Math.round((i + 1) * width) - 1;
                    bar(g2, left, Math.max(1, right - left), detail.distribution[i], peak, base);
                }
                g2.setColor(line);
                g2.drawLine(area[0], base, area[1], base);
                // the panel's font once it is in a window; the graphics' own before that
                java.awt.Font font = getFont() != null ? getFont() : g2.getFont();
                g2.setFont(font.deriveFont(font.getSize2D() - 1));
                FontMetrics metrics = g2.getFontMetrics();
                int labels = base + 3 + metrics.getAscent();
                if (detail.below > 0) {
                    g2.setColor(outlier);
                    bar(g2, 0, OUTLIER_WIDTH, detail.below, peak, base);
                    g2.setColor(line);
                    g2.drawLine(0, base, OUTLIER_WIDTH, base);
                    centred(g2, metrics, "‹ " + detail.below, OUTLIER_WIDTH / 2, labels, outlier);
                }
                if (detail.above > 0) {
                    int left = getWidth() - OUTLIER_WIDTH;
                    g2.setColor(outlier);
                    bar(g2, left, OUTLIER_WIDTH, detail.above, peak, base);
                    g2.setColor(line);
                    g2.drawLine(left, base, getWidth(), base);
                    centred(g2, metrics, detail.above + " ›", left + OUTLIER_WIDTH / 2, labels, outlier);
                }
                if (detail.high > detail.low && detail.mean >= detail.low && detail.mean <= detail.high) {
                    int x = area[0] + (int) Math.round((detail.mean - detail.low) / (detail.high - detail.low)
                            * (area[1] - area[0]));
                    g2.setColor(themed("Console.LogViewer.histogram.selection", new Color(0x151a21)));
                    g2.setStroke(new BasicStroke(1.5f, BasicStroke.CAP_BUTT, BasicStroke.JOIN_MITER, 10,
                            new float[]{3, 3}, 0));
                    g2.drawLine(x, 0, x, base);
                }
                g2.setColor(muted);
                g2.drawString(format.apply(detail.low * factor), area[0], labels);
                String high = format.apply(detail.high * factor);
                g2.drawString(high, area[1] - metrics.stringWidth(high), labels);
            } finally {
                g2.dispose();
            }
        }

        private static void bar(Graphics2D g2, int left, int width, int count, int peak, int base) {
            int height = count == 0 ? 0 : Math.max(2, (int) Math.round((double) count / peak * (base - 2)));
            g2.fillRoundRect(left, base - height, width, height, 3, 3);
        }

        private static void centred(Graphics2D g2, FontMetrics metrics, String text, int x, int y, Color color) {
            g2.setColor(color);
            g2.drawString(text, x - metrics.stringWidth(text) / 2, y);
        }
    }
}
