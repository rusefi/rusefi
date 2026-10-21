package com.rusefi.ui.logview;

import com.rusefi.logview.CsvLogReader;
import com.rusefi.ui.widgets.ConsoleFonts;

import javax.swing.JComponent;
import javax.swing.SwingUtilities;
import java.awt.BasicStroke;
import java.awt.Font;
import java.awt.FontMetrics;
import java.awt.Graphics;
import java.awt.Graphics2D;
import java.awt.RenderingHints;
import java.awt.event.MouseAdapter;
import java.awt.event.MouseEvent;
import java.awt.event.MouseWheelEvent;
import java.awt.geom.Path2D;
import java.util.ArrayList;
import java.util.List;
import java.util.Locale;

import static com.rusefi.config.generated.VariableRegistryValues.GAUGE_NAME_TIME;

/**
 * Time plot for the fields plotted in the companion list, each in its {@link TraceColors} slot colour.
 * Wheel zooms around the pointer, drag selects a range to zoom into, right-drag or Shift+wheel pans,
 * double-click shows the whole log.
 */
final class LogChart extends JComponent {
    /** Each wheel notch scales the visible span by this factor. */
    static final double WHEEL_ZOOM = 1.25;
    /** Fraction of the visible span that one Shift+wheel notch pans. */
    static final double WHEEL_PAN = 0.1;
    /** A drag shorter than this many pixels is a click, not a range selection. */
    static final int MIN_SELECTION_PIXELS = 5;

    private final CsvLogReader log;
    private final TraceColors colors;
    private final TimeWindow window;
    private final ChartSettings grid;
    private int cursorSample = -1;
    private int cursorX = -1;
    // Plot rectangle from the last paint, used to map mouse positions to time.
    private int plotLeft = 54;
    private int plotRight = 55;
    private int plotTop = 68;
    private int plotBottom = 69;
    private int selectionFrom = -1;
    private int selectionTo = -1;
    private int panFromX = -1;
    private double panFromStart;
    private Runnable viewListener = () -> {
    };

    LogChart(CsvLogReader log, boolean[] displayed) {
        this(log, displayed, new ChartSettings());
    }

    LogChart(CsvLogReader log, boolean[] displayed, ChartSettings grid) {
        this(log, TraceColors.plotted(log.fieldCount(), displayed), grid);
    }

    LogChart(CsvLogReader log, TraceColors colors, ChartSettings grid) {
        this.log = log;
        this.colors = colors;
        this.grid = grid;
        double fullStart = log.minimum(0);
        double fullEnd = log.maximum(0);
        double interval = log.minimumSampleInterval();
        // Allow zooming in until only a few samples are visible.
        window = new TimeWindow(fullStart, fullEnd, interval > 0 ? interval * 4 : (fullEnd - fullStart) / 1000);
        setOpaque(true);
        MouseAdapter mouse = new MouseHandler();
        addMouseListener(mouse);
        addMouseMotionListener(mouse);
        addMouseWheelListener(mouse);
    }

    TimeWindow timeWindow() {
        return window;
    }

    PlotPalette palette() {
        return PlotPalette.of(grid.isDarkPlot());
    }

    /** Called after the visible time range or the cursor changes, e.g. to update a status line. */
    void setViewListener(Runnable viewListener) {
        this.viewListener = viewListener;
    }

    /** Time under the cursor line, or NaN when the pointer is outside the plot. */
    double cursorTime() {
        if (cursorSample < 0) {
            return Double.NaN;
        }
        return grid.isSnapCursor() ? log.value(cursorSample, 0) : timeAt(cursorX);
    }

    private double timeAt(int x) {
        double fraction = (x - plotLeft) / (double) Math.max(1, plotRight - plotLeft);
        return window.start() + Math.min(1, Math.max(0, fraction)) * window.span();
    }

    private int xOf(double time) {
        return plotLeft + (int) Math.round((time - window.start()) * (plotRight - plotLeft) / window.span());
    }

    private boolean insidePlot(int x) {
        return x >= plotLeft && x <= plotRight;
    }

    private final class MouseHandler extends MouseAdapter {
        @Override
        public void mouseMoved(MouseEvent event) {
            boolean inside = insidePlot(event.getX());
            cursorX = inside ? event.getX() : -1;
            cursorSample = inside ? log.nearestSample(timeAt(event.getX())) : -1;
            repaint();
            viewListener.run();
        }

        @Override
        public void mouseExited(MouseEvent event) {
            if (selectionFrom < 0 && panFromX < 0) {
                cursorX = cursorSample = -1;
                repaint();
                viewListener.run();
            }
        }

        @Override
        public void mousePressed(MouseEvent event) {
            if (SwingUtilities.isLeftMouseButton(event) && insidePlot(event.getX())) {
                selectionFrom = selectionTo = event.getX();
            } else if (SwingUtilities.isRightMouseButton(event) || SwingUtilities.isMiddleMouseButton(event)) {
                panFromX = event.getX();
                panFromStart = window.start();
            }
        }

        @Override
        public void mouseDragged(MouseEvent event) {
            if (selectionFrom >= 0) {
                selectionTo = Math.max(plotLeft, Math.min(plotRight, event.getX()));
            } else if (panFromX >= 0) {
                double secondsPerPixel = window.span() / Math.max(1, plotRight - plotLeft);
                window.pan(panFromStart - (event.getX() - panFromX) * secondsPerPixel - window.start());
            }
            mouseMoved(event);
        }

        @Override
        public void mouseReleased(MouseEvent event) {
            if (selectionFrom >= 0 && Math.abs(selectionTo - selectionFrom) >= MIN_SELECTION_PIXELS) {
                window.show(timeAt(selectionFrom), timeAt(selectionTo));
            }
            selectionFrom = selectionTo = -1;
            panFromX = -1;
            repaint();
            viewListener.run();
        }

        @Override
        public void mouseClicked(MouseEvent event) {
            if (SwingUtilities.isLeftMouseButton(event) && event.getClickCount() == 2) {
                window.reset();
                repaint();
                viewListener.run();
            }
        }

        @Override
        public void mouseWheelMoved(MouseWheelEvent event) {
            double notches = event.getPreciseWheelRotation();
            // Touchpads deliver horizontal scrolling as Shift+wheel.
            if (event.isShiftDown()) {
                window.pan(notches * WHEEL_PAN * window.span());
            } else {
                window.zoom(timeAt(event.getX()), Math.pow(WHEEL_ZOOM, notches));
            }
            mouseMoved(event);
        }
    }

    @Override
    protected void paintComponent(Graphics graphics) {
        super.paintComponent(graphics);
        Graphics2D g = (Graphics2D) graphics.create();
        try {
            PlotPalette palette = palette();
            g.setColor(palette.background);
            g.fillRect(0, 0, getWidth(), getHeight());
            g.setRenderingHint(RenderingHints.KEY_ANTIALIASING, RenderingHints.VALUE_ANTIALIAS_ON);
            int left = 54;
            int right = Math.max(left + 1, getWidth() - 22);
            FontMetrics metrics = g.getFontMetrics();
            // Fixed-width digits so legend values do not jitter as the cursor moves.
            Font valueFont = ConsoleFonts.mono(Font.PLAIN, g.getFont().getSize());
            FontMetrics valueMetrics = g.getFontMetrics(valueFont);

            List<Integer> signals = selectedSignals();
            int legendX = left;
            int legendY = 26;
            for (int index = 0; index < signals.size(); index++) {
                int field = signals.get(index);
                String name = log.name(field) + " ";
                String value = legendValue(field, cursorSample);
                String unit = log.unit(field).isEmpty() ? "" : " " + log.unit(field);
                int nameWidth = metrics.stringWidth(name);
                int valueWidth = valueMetrics.stringWidth(value);
                int width = 19 + nameWidth + valueWidth + metrics.stringWidth(unit) + 13;
                if (legendX + width > right && legendX > left) {
                    legendX = left;
                    legendY += 19;
                }
                g.setColor(palette.trace(colors.slot(field)));
                g.fillRoundRect(legendX, legendY - 10, 13, 5, 4, 4);
                g.setColor(palette.label);
                g.drawString(name, legendX + 19, legendY);
                g.setFont(valueFont);
                g.drawString(value, legendX + 19 + nameWidth, legendY);
                g.setFont(metrics.getFont());
                g.drawString(unit, legendX + 19 + nameWidth + valueWidth, legendY);
                legendX += width;
            }
            int top = Math.max(46, legendY + 20);
            int bottom = Math.max(top + 1, getHeight() - 46);
            plotLeft = left;
            plotRight = right;
            plotTop = top;
            plotBottom = bottom;

            if (grid.isHorizontal()) {
                // Traces are scaled independently, so these lines mark fractions of each trace's range.
                g.setColor(palette.grid);
                int divisions = grid.getHorizontalDivisions();
                for (int i = 0; i <= divisions; i++) {
                    int y = top + (bottom - top) * i / divisions;
                    g.drawLine(left, y, right, y);
                }
            }
            drawTimeAxis(g, metrics, palette);
            if (signals.isEmpty()) {
                g.setColor(palette.hint);
                g.drawString("Click a signal on the left to plot it", left + 20, top + 30);
                return;
            }

            Graphics2D plot = (Graphics2D) g.create();
            try {
                plot.clipRect(left, top - 1, right - left + 1, bottom - top + 3);
                plot.setStroke(new BasicStroke(palette.strokeWidth));
                for (int field : signals) {
                    plot.setColor(palette.trace(colors.slot(field)));
                    plot.draw(tracePath(field));
                }
                if (selectionFrom >= 0 && selectionTo != selectionFrom) {
                    plot.setColor(palette.selection);
                    plot.fillRect(Math.min(selectionFrom, selectionTo), top, Math.abs(selectionTo - selectionFrom), bottom - top);
                }
                int cursorLine = cursorLineX();
                if (cursorLine >= 0) {
                    plot.setColor(palette.cursor);
                    plot.setStroke(new BasicStroke(1));
                    plot.drawLine(cursorLine, top, cursorLine, bottom);
                }
                if (cursorLine >= 0 && !grid.isSnapCursor()) {
                    // The line no longer sits on the sample being read, so mark that sample on each trace.
                    int sampleX = xOf(log.value(cursorSample, 0));
                    for (int field : signals) {
                        double value = log.value(cursorSample, field);
                        if (Double.isFinite(value)) {
                            int y = (int) Math.round(yOf(field, value));
                            plot.setColor(palette.trace(colors.slot(field)));
                            plot.fillOval(sampleX - 3, y - 3, 7, 7);
                        }
                    }
                }
            } finally {
                plot.dispose();
            }
        } finally {
            g.dispose();
        }
    }

    private void drawTimeAxis(Graphics2D g, FontMetrics metrics, PlotPalette palette) {
        boolean tickLines = grid.isVertical() && grid.getVerticalMode() == ChartSettings.VerticalMode.TIME_TICKS;
        boolean sampleLines = grid.isVertical() && grid.getVerticalMode() == ChartSettings.VerticalMode.SAMPLES;
        List<Double> sampleTimes = sampleLines ? sampleGridTimes() : null;
        if (sampleTimes != null) {
            g.setColor(palette.grid);
            for (double time : sampleTimes) {
                int x = xOf(time);
                g.drawLine(x, plotTop, x, plotBottom);
            }
        }
        double step = timeTickStep();
        int decimals = Math.max(0, Math.min(6, (int) -Math.floor(Math.log10(step))));
        long lastTick = (long) Math.floor(window.end() / step + 1e-6);
        for (long tick = (long) Math.ceil(window.start() / step - 1e-6); tick <= lastTick; tick++) {
            double time = tick * step;
            int x = xOf(time);
            if (tickLines) {
                g.setColor(palette.grid);
                g.drawLine(x, plotTop, x, plotBottom);
            }
            String text = String.format(Locale.ROOT, "%." + decimals + "f", time);
            g.setColor(palette.label);
            g.drawString(text, x - metrics.stringWidth(text) / 2, plotBottom + 19);
        }
        String timeLabel = GAUGE_NAME_TIME + " (s)";
        if (sampleLines && sampleTimes == null) {
            timeLabel += "  ·  zoom in to see sample lines";
        }
        g.setColor(palette.label);
        g.drawString(timeLabel, (plotLeft + plotRight - metrics.stringWidth(timeLabel)) / 2, plotBottom + 37);
    }

    /** Screen x of the cursor line, or -1 when the pointer is outside the plot. */
    int cursorLineX() {
        if (cursorSample < 0) {
            return -1;
        }
        return grid.isSnapCursor() ? xOf(log.value(cursorSample, 0)) : cursorX;
    }

    /** Screen y of {@code value}; each field is scaled to its own whole-log range. */
    private double yOf(int field, double value) {
        double min = log.minimum(field);
        double range = log.maximum(field) - min;
        return plotTop + (1 - (range == 0 ? 0.5 : (value - min) / range)) * (plotBottom - plotTop);
    }

    /** Seconds between time-axis ticks, chosen so ticks are roughly the configured spacing apart. */
    double timeTickStep() {
        return niceStep(window.span() / Math.max(2, (plotRight - plotLeft) / (double) grid.getTimeSpacing()));
    }

    /**
     * Distinct sample times inside the visible window, or {@code null} when they are too many to tell apart:
     * more than one per {@link ChartSettings#MIN_SAMPLE_LINE_SPACING} pixels on average. Samples sharing a
     * timestamp (common in real logs) are one line; uneven logging may still put a few lines close together.
     */
    List<Double> sampleGridTimes() {
        int first = log.firstSampleAtOrAfter(window.start());
        int end = log.firstSampleAtOrAfter(Math.nextUp(window.end()));
        int maxLines = (plotRight - plotLeft) / ChartSettings.MIN_SAMPLE_LINE_SPACING + 1;
        List<Double> times = new ArrayList<>();
        for (int sample = first; sample < end; sample++) {
            double time = log.value(sample, 0);
            if (times.isEmpty() || time != times.get(times.size() - 1)) {
                if (times.size() == maxLines) {
                    return null;
                }
                times.add(time);
            }
        }
        return times;
    }

    /** Smallest 1, 2 or 5 times a power of ten that is at least {@code rough}. */
    static double niceStep(double rough) {
        if (!(rough > 0) || !Double.isFinite(rough)) {
            return 1;
        }
        double magnitude = Math.pow(10, Math.floor(Math.log10(rough)));
        double fraction = rough / magnitude;
        double nice = fraction <= 1 ? 1 : fraction <= 2 ? 2 : fraction <= 5 ? 5 : 10;
        return nice * magnitude;
    }

    /**
     * Line through the visible samples of {@code field}, plus one sample beyond each edge.
     * Samples landing in the same pixel column collapse to a vertical min/max stroke, so the
     * path size depends on the plot width rather than on the number of samples.
     */
    Path2D tracePath(int field) {
        Path2D path = new Path2D.Double();
        double min = log.minimum(field);
        if (!Double.isFinite(min)) {
            return path;
        }
        int first = Math.max(0, log.firstSampleAtOrAfter(window.start()) - 1);
        int last = Math.min(log.sampleCount() - 1, log.firstSampleAtOrAfter(window.end()));
        double scale = (plotRight - plotLeft) / window.span();
        ColumnPath column = new ColumnPath(path);
        for (int sample = first; sample <= last; sample++) {
            double value = log.value(sample, field);
            if (!Double.isFinite(value)) {
                column.breakLine();
                continue;
            }
            double x = plotLeft + (log.value(sample, 0) - window.start()) * scale;
            column.add(x, yOf(field, value));
        }
        column.flush();
        return path;
    }

    /** Accumulates points per pixel column and emits first, min, max and last of each column. */
    private static final class ColumnPath {
        private final Path2D path;
        private boolean drawing;
        private long column = Long.MIN_VALUE;
        private double x;
        private double first;
        private double min;
        private double max;
        private double last;

        ColumnPath(Path2D path) {
            this.path = path;
        }

        void add(double pointX, double pointY) {
            long pointColumn = (long) Math.floor(pointX);
            if (pointColumn == column) {
                min = Math.min(min, pointY);
                max = Math.max(max, pointY);
                last = pointY;
                return;
            }
            flush();
            column = pointColumn;
            x = pointX;
            first = min = max = last = pointY;
        }

        void breakLine() {
            flush();
            drawing = false;
        }

        void flush() {
            if (column == Long.MIN_VALUE) {
                return;
            }
            if (drawing) {
                path.lineTo(x, first);
            } else {
                path.moveTo(x, first);
                drawing = true;
            }
            if (min < max) {
                path.lineTo(x, min);
                path.lineTo(x, max);
                path.lineTo(x, last);
            }
            column = Long.MIN_VALUE;
        }
    }

    private List<Integer> selectedSignals() {
        List<Integer> result = new ArrayList<>();
        for (int field = 1; field < colors.fieldCount(); field++) {
            if (colors.isPlotted(field)) {
                result.add(field);
            }
        }
        return result;
    }

    /** Value at the cursor sample, or the whole-log range when there is no cursor. */
    private String legendValue(int field, int sample) {
        if (sample >= 0 && Double.isFinite(log.value(sample, field))) {
            return String.format(Locale.ROOT, "%.2f", log.value(sample, field));
        }
        return String.format(Locale.ROOT, "%.1f–%.1f", log.minimum(field), log.maximum(field));
    }
}
