package com.rusefi.ui.logview;

import com.rusefi.logview.CsvLogReader;
import com.rusefi.ui.widgets.ConsoleFonts;

import javax.swing.JComponent;
import javax.swing.JMenuItem;
import javax.swing.JPopupMenu;
import javax.swing.SwingUtilities;
import javax.swing.TransferHandler;
import java.awt.BasicStroke;
import java.awt.Cursor;
import java.awt.Font;
import java.awt.FontMetrics;
import java.awt.Graphics;
import java.awt.Graphics2D;
import java.awt.Point;
import java.awt.Rectangle;
import java.awt.RenderingHints;
import java.awt.datatransfer.Transferable;
import java.awt.dnd.DropTargetAdapter;
import java.awt.dnd.DropTargetDropEvent;
import java.awt.dnd.DropTargetEvent;
import java.awt.event.MouseAdapter;
import java.awt.event.MouseEvent;
import java.awt.event.MouseWheelEvent;
import java.awt.geom.Path2D;
import java.util.ArrayList;
import java.util.List;
import java.util.Locale;
import java.util.TooManyListenersException;

import static com.rusefi.config.generated.VariableRegistryValues.GAUGE_NAME_TIME;

/**
 * Stacked time plots from a {@link PlotLayout}, sharing one time axis, zoom, range selection and cursor.
 * Each plot has a header with its legend (hover a legend item for x to remove it, drag it to another plot)
 * and a ⋯ menu; the row below the time axis adds a plot, and signals can be dropped on a plot or on that
 * row. Dragging the gap between two plots resizes them, double-clicking it makes all plots equal.
 * Clicking a plot makes it active, so new signals go there.
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

    private static final int LEFT = 54;
    private static final int RIGHT_MARGIN = 22;
    private static final int TOP = 8;
    private static final int AXIS_HEIGHT = 46;
    private static final int ADD_ROW_HEIGHT = 30;
    private static final int PLOT_GAP = 10;
    private static final int LEGEND_LINE = 19;
    private static final int HEADER_PADDING = 8;
    private static final int MENU_WIDTH = 24;
    private static final int CLOSE_WIDTH = 14;
    private static final int MIN_TRACE_HEIGHT = 20;
    /** Drag distance before a pressed legend item starts a drag and drop. */
    private static final int DRAG_START_PIXELS = 5;
    static final int NO_TARGET = -1;
    static final int ADD_ROW_TARGET = -2;

    private final CsvLogReader log;
    private final PlotLayout layout;
    private final TimeWindow window;
    private final ChartSettings grid;
    private int cursorSample = -1;
    private int cursorX = -1;
    // Geometry from the last paint, used to map mouse positions to time, plots and controls.
    private int plotLeft = LEFT;
    private int plotRight = LEFT + 1;
    private List<Band> bands = new ArrayList<>();
    private Rectangle addPlotRow = new Rectangle();
    /** Gaps between neighbouring plots; dragging one resizes the two plots. */
    private List<Rectangle> dividers = new ArrayList<>();
    private int resizing = -1;
    private int resizeFromY;
    private int[] resizeHeights;
    private int dragField = -1;
    private int dragFromX;
    private int dragFromY;
    private int exportingField = -1;
    /** Plot under a signal dragged over the chart, {@link #ADD_ROW_TARGET}, or {@link #NO_TARGET}. */
    private int dropTarget = NO_TARGET;
    private int selectionFrom = -1;
    private int selectionTo = -1;
    private int panFromX = -1;
    private double panFromStart;
    private int hoveredField = -1;
    private int hoveredMenu = -1;
    private Runnable viewListener = () -> {
    };

    /** One plot's area: legend header on top, traces below. */
    static final class Band {
        final int plot;
        final Rectangle header;
        final Rectangle traces;
        final Rectangle menu;
        final List<LegendItem> legend = new ArrayList<>();

        Band(int plot, Rectangle header, Rectangle traces, Rectangle menu) {
            this.plot = plot;
            this.header = header;
            this.traces = traces;
            this.menu = menu;
        }
    }

    static final class LegendItem {
        final int field;
        final Rectangle bounds;
        final Rectangle close;
        final int nameWidth;
        final int valueWidth;

        LegendItem(int field, Rectangle bounds, Rectangle close, int nameWidth, int valueWidth) {
            this.field = field;
            this.bounds = bounds;
            this.close = close;
            this.nameWidth = nameWidth;
            this.valueWidth = valueWidth;
        }
    }

    LogChart(CsvLogReader log, boolean[] displayed) {
        this(log, displayed, new ChartSettings());
    }

    LogChart(CsvLogReader log, boolean[] displayed, ChartSettings grid) {
        this(log, PlotLayout.single(log.fieldCount(), displayed), grid);
    }

    LogChart(CsvLogReader log, PlotLayout layout, ChartSettings grid) {
        this.log = log;
        this.layout = layout;
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
        layout.addListener(this::repaint);
        setTransferHandler(new ChartTransferHandler());
        // Headless (CI, no display) Swing installs no drop target, and there is no drag and drop to highlight.
        if (getDropTarget() == null) {
            return;
        }
        try {
            getDropTarget().addDropTargetListener(new DropTargetAdapter() {
                @Override
                public void dragExit(DropTargetEvent event) {
                    setDropTarget(NO_TARGET);
                }

                @Override
                public void drop(DropTargetDropEvent event) {
                    setDropTarget(NO_TARGET);
                }
            });
        } catch (TooManyListenersException e) {
            // the highlight then clears on the next drop instead of on exit
        }
    }

    TimeWindow timeWindow() {
        return window;
    }

    PlotLayout plotLayout() {
        return layout;
    }

    PlotPalette palette() {
        return PlotPalette.of(grid.isDarkPlot());
    }

    /** Called after the visible time range or the cursor changes, e.g. to update a status line. */
    void setViewListener(Runnable viewListener) {
        this.viewListener = viewListener;
    }

    /** Time under the cursor line, or NaN when the pointer is outside the plots. */
    double cursorTime() {
        if (cursorSample < 0) {
            return Double.NaN;
        }
        return grid.isSnapCursor() ? log.value(cursorSample, 0) : timeAt(cursorX);
    }

    /** Plot areas from the last paint, top to bottom. */
    List<Band> bands() {
        return bands;
    }

    Rectangle addPlotRow() {
        return addPlotRow;
    }

    List<Rectangle> dividers() {
        return dividers;
    }

    int dropTarget() {
        return dropTarget;
    }

    private void setDropTarget(int target) {
        if (target != dropTarget) {
            dropTarget = target;
            repaint();
        }
    }

    /** Where a signal dropped at {@code point} goes: a plot index, {@link #ADD_ROW_TARGET} or {@link #NO_TARGET}. */
    int dropTargetAt(Point point) {
        if (point == null) {
            return NO_TARGET;
        }
        Rectangle row = new Rectangle(addPlotRow);
        row.grow(0, 6);
        if (row.contains(point) && layout.canAddPlot()) {
            return ADD_ROW_TARGET;
        }
        for (Band band : bands) {
            Rectangle area = band.header.union(band.traces);
            if (point.y >= area.y && point.y < area.y + area.height) {
                return band.plot;
            }
        }
        return NO_TARGET;
    }

    /** Puts a dropped signal into the plot under {@code point}, or into a new plot on the add row. */
    boolean dropSignal(int field, Point point) {
        int target = dropTargetAt(point);
        setDropTarget(NO_TARGET);
        if (target == ADD_ROW_TARGET) {
            layout.moveToNewPlot(field);
            return true;
        }
        if (target >= 0) {
            layout.moveTo(field, target);
            layout.setActive(target);
            return true;
        }
        return false;
    }

    private int dividerAt(int x, int y) {
        for (int i = 0; i < dividers.size(); i++) {
            if (dividers.get(i).contains(x, y)) {
                return i;
            }
        }
        return -1;
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

    private Band bandAt(int x, int y) {
        for (Band band : bands) {
            if (band.header.contains(x, y) || band.traces.contains(x, y)) {
                return band;
            }
        }
        return null;
    }

    private LegendItem legendAt(int x, int y) {
        for (Band band : bands) {
            for (LegendItem item : band.legend) {
                if (item.bounds.contains(x, y)) {
                    return item;
                }
            }
        }
        return null;
    }

    private boolean onControl(int x, int y) {
        Band band = bandAt(x, y);
        return addPlotRow.contains(x, y) || legendAt(x, y) != null || dividerAt(x, y) >= 0
                || (band != null && band.menu.contains(x, y));
    }

    /** Menu behind a plot's ⋯ button. */
    JPopupMenu plotMenu(int plot) {
        JPopupMenu menu = new JPopupMenu();
        JMenuItem up = new JMenuItem("Move up");
        up.setEnabled(plot > 0);
        up.addActionListener(e -> layout.move(plot, -1));
        JMenuItem down = new JMenuItem("Move down");
        down.setEnabled(plot < layout.plotCount() - 1);
        down.addActionListener(e -> layout.move(plot, 1));
        JMenuItem remove = new JMenuItem("Remove plot");
        remove.setEnabled(layout.canRemovePlot());
        remove.addActionListener(e -> layout.removePlot(plot));
        menu.add(up);
        menu.add(down);
        menu.addSeparator();
        menu.add(remove);
        return menu;
    }

    private final class MouseHandler extends MouseAdapter {
        @Override
        public void mouseMoved(MouseEvent event) {
            boolean inside = insidePlot(event.getX());
            cursorX = inside ? event.getX() : -1;
            cursorSample = inside ? log.nearestSample(timeAt(event.getX())) : -1;
            LegendItem item = legendAt(event.getX(), event.getY());
            hoveredField = item != null ? item.field : -1;
            Band band = bandAt(event.getX(), event.getY());
            hoveredMenu = band != null && band.menu.contains(event.getX(), event.getY()) ? band.plot : -1;
            if (dividerAt(event.getX(), event.getY()) >= 0) {
                setCursor(Cursor.getPredefinedCursor(Cursor.N_RESIZE_CURSOR));
            } else {
                setCursor(onControl(event.getX(), event.getY())
                        ? Cursor.getPredefinedCursor(Cursor.HAND_CURSOR) : Cursor.getDefaultCursor());
            }
            repaint();
            viewListener.run();
        }

        @Override
        public void mouseExited(MouseEvent event) {
            if (selectionFrom < 0 && panFromX < 0) {
                cursorX = cursorSample = -1;
                hoveredField = hoveredMenu = -1;
                repaint();
                viewListener.run();
            }
        }

        @Override
        public void mousePressed(MouseEvent event) {
            int x = event.getX();
            int y = event.getY();
            if (SwingUtilities.isLeftMouseButton(event)) {
                if (addPlotRow.contains(x, y)) {
                    layout.addPlot();
                    return;
                }
                LegendItem item = legendAt(x, y);
                if (item != null && item.close.contains(x, y)) {
                    layout.remove(item.field);
                    hoveredField = -1;
                    return;
                }
                int divider = dividerAt(x, y);
                if (divider >= 0) {
                    resizing = divider;
                    resizeFromY = y;
                    resizeHeights = new int[bands.size()];
                    for (int i = 0; i < bands.size(); i++) {
                        resizeHeights[i] = bands.get(i).header.height + bands.get(i).traces.height;
                    }
                    return;
                }
                if (item != null) {
                    dragField = item.field;
                    dragFromX = x;
                    dragFromY = y;
                }
                Band band = bandAt(x, y);
                if (band == null) {
                    return;
                }
                layout.setActive(band.plot);
                if (band.menu.contains(x, y)) {
                    plotMenu(band.plot).show(LogChart.this, band.menu.x, band.menu.y + band.menu.height);
                } else if (band.traces.contains(x, y) && insidePlot(x)) {
                    selectionFrom = selectionTo = x;
                }
            } else if (SwingUtilities.isRightMouseButton(event) || SwingUtilities.isMiddleMouseButton(event)) {
                panFromX = x;
                panFromStart = window.start();
            }
        }

        @Override
        public void mouseDragged(MouseEvent event) {
            if (resizing >= 0) {
                resize(event.getY() - resizeFromY);
                return;
            }
            if (dragField >= 0) {
                if (Math.abs(event.getX() - dragFromX) + Math.abs(event.getY() - dragFromY) >= DRAG_START_PIXELS) {
                    exportingField = dragField;
                    dragField = -1;
                    getTransferHandler().exportAsDrag(LogChart.this, event, TransferHandler.MOVE);
                }
                return;
            }
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
            resizing = -1;
            dragField = -1;
            repaint();
            viewListener.run();
        }

        @Override
        public void mouseClicked(MouseEvent event) {
            if (!SwingUtilities.isLeftMouseButton(event) || event.getClickCount() != 2) {
                return;
            }
            if (dividerAt(event.getX(), event.getY()) >= 0) {
                layout.resetWeights();
            } else if (!onControl(event.getX(), event.getY())) {
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

    /** Moves divider {@link #resizing} by {@code delta} pixels, keeping both neighbouring plots usable. */
    private void resize(int delta) {
        int upper = resizing;
        int lower = resizing + 1;
        if (lower >= resizeHeights.length) {
            return;
        }
        int pair = resizeHeights[upper] + resizeHeights[lower];
        int minUpper = bands.get(upper).header.height + MIN_TRACE_HEIGHT;
        int minLower = bands.get(lower).header.height + MIN_TRACE_HEIGHT;
        int newUpper = Math.max(minUpper, Math.min(pair - minLower, resizeHeights[upper] + delta));
        double[] heights = new double[resizeHeights.length];
        for (int i = 0; i < heights.length; i++) {
            heights[i] = resizeHeights[i];
        }
        heights[upper] = newUpper;
        heights[lower] = pair - newUpper;
        layout.setWeights(heights);
    }

    /** Exports legend drags; accepts signals on a plot or the add row, and passes log files to the host. */
    private final class ChartTransferHandler extends SignalTransfer.Handler {
        @Override
        protected Transferable createTransferable(JComponent component) {
            return exportingField >= 0 ? SignalTransfer.of(exportingField) : null;
        }

        @Override
        protected void exportDone(JComponent source, Transferable data, int action) {
            exportingField = -1;
            setDropTarget(NO_TARGET);
        }

        @Override
        boolean canImportSignal(TransferSupport support) {
            int target = support.isDrop() ? dropTargetAt(support.getDropLocation().getDropPoint()) : NO_TARGET;
            setDropTarget(target);
            return target != NO_TARGET;
        }

        @Override
        boolean importSignal(TransferSupport support, int field) {
            return support.isDrop() && dropSignal(field, support.getDropLocation().getDropPoint());
        }
    }

    /** Lays out each plot's legend, then splits the height between the plots by their weights. */
    private List<Band> layoutBands(FontMetrics metrics, FontMetrics valueMetrics) {
        int plots = layout.plotCount();
        int legendRight = plotRight - MENU_WIDTH - 6;
        List<List<LegendItem>> legends = new ArrayList<>();
        List<Integer> lineCounts = new ArrayList<>();
        for (int plot = 0; plot < plots; plot++) {
            List<LegendItem> items = new ArrayList<>();
            int x = plotLeft;
            int line = 0;
            for (int field : layout.fields(plot)) {
                int nameWidth = metrics.stringWidth(log.name(field) + " ");
                int valueWidth = valueMetrics.stringWidth(legendValue(field, cursorSample));
                int width = 19 + nameWidth + valueWidth + metrics.stringWidth(unitSuffix(field)) + CLOSE_WIDTH + 10;
                if (x + width > legendRight && x > plotLeft) {
                    x = plotLeft;
                    line++;
                }
                // y is relative to the header top until the bands are placed
                items.add(new LegendItem(field, new Rectangle(x, line * LEGEND_LINE, width, LEGEND_LINE),
                        new Rectangle(x + width - CLOSE_WIDTH - 8, line * LEGEND_LINE, CLOSE_WIDTH, LEGEND_LINE),
                        nameWidth, valueWidth));
                x += width;
            }
            legends.add(items);
            lineCounts.add(line + 1);
        }

        int bottom = getHeight() - ADD_ROW_HEIGHT - AXIS_HEIGHT;
        int available = Math.max(plots, bottom - TOP - PLOT_GAP * (plots - 1));
        double totalWeight = 0;
        for (int plot = 0; plot < plots; plot++) {
            totalWeight += layout.weight(plot);
        }
        List<Band> result = new ArrayList<>();
        int top = TOP;
        for (int plot = 0; plot < plots; plot++) {
            int bandHeight = (int) Math.round(available * layout.weight(plot) / totalWeight);
            int headerHeight = HEADER_PADDING + lineCounts.get(plot) * LEGEND_LINE;
            int traceHeight = Math.max(MIN_TRACE_HEIGHT, bandHeight - headerHeight);
            Rectangle header = new Rectangle(plotLeft, top, plotRight - plotLeft, headerHeight);
            Rectangle traces = new Rectangle(plotLeft, top + headerHeight, plotRight - plotLeft, traceHeight);
            Rectangle menu = new Rectangle(plotRight - MENU_WIDTH, top + 2, MENU_WIDTH, LEGEND_LINE);
            Band band = new Band(plot, header, traces, menu);
            for (LegendItem item : legends.get(plot)) {
                item.bounds.translate(0, top + HEADER_PADDING / 2);
                item.close.translate(0, top + HEADER_PADDING / 2);
                band.legend.add(item);
            }
            result.add(band);
            top += headerHeight + traceHeight + PLOT_GAP;
        }
        return result;
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
            FontMetrics metrics = g.getFontMetrics();
            // Fixed-width digits so legend values do not jitter as the cursor moves.
            Font valueFont = ConsoleFonts.mono(Font.PLAIN, g.getFont().getSize());
            FontMetrics valueMetrics = g.getFontMetrics(valueFont);
            plotLeft = LEFT;
            plotRight = Math.max(LEFT + 1, getWidth() - RIGHT_MARGIN);
            bands = layoutBands(metrics, valueMetrics);
            dividers = new ArrayList<>();
            for (int i = 0; i + 1 < bands.size(); i++) {
                Rectangle upper = bands.get(i).traces;
                int y = upper.y + upper.height;
                dividers.add(new Rectangle(plotLeft - 14, y, plotRight - plotLeft + 14, bands.get(i + 1).header.y - y));
            }
            Band last = bands.get(bands.size() - 1);
            int axisTop = last.traces.y + last.traces.height;
            addPlotRow = new Rectangle(plotLeft, axisTop + AXIS_HEIGHT, plotRight - plotLeft, ADD_ROW_HEIGHT - 6);

            List<Double> sampleTimes = grid.isVertical() && grid.getVerticalMode() == ChartSettings.VerticalMode.SAMPLES
                    ? sampleGridTimes() : null;
            for (Band band : bands) {
                paintBand(g, band, palette, metrics, valueFont, sampleTimes);
            }
            drawTimeAxis(g, metrics, palette, axisTop, sampleTimes);
            drawAddPlotRow(g, metrics, palette);
        } finally {
            g.dispose();
        }
    }

    private void paintBand(Graphics2D g, Band band, PlotPalette palette, FontMetrics metrics, Font valueFont,
                           List<Double> sampleTimes) {
        Rectangle area = band.traces;
        boolean stacked = layout.plotCount() > 1;
        if (band.plot > 0) {
            g.setColor(palette.grid);
            int y = band.header.y - PLOT_GAP / 2;
            g.drawLine(plotLeft, y, plotRight, y);
        }
        if (stacked && band.plot == layout.activePlot()) {
            g.setColor(palette.accent);
            g.fillRoundRect(plotLeft - 12, band.header.y, 3, area.y + area.height - band.header.y, 3, 3);
        }
        if (band.plot == dropTarget) {
            Rectangle target = band.header.union(area);
            g.setColor(palette.selection);
            g.fillRoundRect(target.x - 6, target.y - 2, target.width + 12, target.height + 4, 8, 8);
            g.setColor(palette.accent);
            g.drawRoundRect(target.x - 6, target.y - 2, target.width + 12, target.height + 4, 8, 8);
        }

        for (LegendItem item : band.legend) {
            int baseline = item.bounds.y + 14;
            int x = item.bounds.x;
            g.setColor(palette.trace(layout.colors().slot(item.field)));
            g.fillRoundRect(x, baseline - 10, 13, 5, 4, 4);
            g.setColor(palette.label);
            g.drawString(log.name(item.field) + " ", x + 19, baseline);
            g.setFont(valueFont);
            g.drawString(legendValue(item.field, cursorSample), x + 19 + item.nameWidth, baseline);
            g.setFont(metrics.getFont());
            g.drawString(unitSuffix(item.field), x + 19 + item.nameWidth + item.valueWidth, baseline);
            if (item.field == hoveredField) {
                g.setColor(palette.hint);
                g.drawString("×", item.close.x + (item.close.width - metrics.stringWidth("×")) / 2, baseline);
            }
        }
        // ⋯ menu button: three dots, darker while hovered
        g.setColor(band.plot == hoveredMenu ? palette.label : palette.hint);
        int dotsX = band.menu.x + band.menu.width / 2 - 7;
        int dotsY = band.menu.y + band.menu.height / 2 - 1;
        for (int dot = 0; dot < 3; dot++) {
            g.fillOval(dotsX + dot * 6, dotsY, 3, 3);
        }

        if (grid.isHorizontal()) {
            // Traces are scaled independently, so these lines mark fractions of each trace's range.
            g.setColor(palette.grid);
            int divisions = grid.getHorizontalDivisions();
            for (int i = 0; i <= divisions; i++) {
                int y = area.y + area.height * i / divisions;
                g.drawLine(plotLeft, y, plotRight, y);
            }
        }
        g.setColor(palette.grid);
        if (sampleTimes != null) {
            for (double time : sampleTimes) {
                int x = xOf(time);
                g.drawLine(x, area.y, x, area.y + area.height);
            }
        } else if (grid.isVertical() && grid.getVerticalMode() == ChartSettings.VerticalMode.TIME_TICKS) {
            for (double time : timeTicks()) {
                int x = xOf(time);
                g.drawLine(x, area.y, x, area.y + area.height);
            }
        }

        List<Integer> fields = layout.fields(band.plot);
        if (fields.isEmpty()) {
            g.setColor(palette.hint);
            String hint = band.plot == layout.activePlot() || !stacked
                    ? "Click a signal on the left to add it here"
                    : "Click this plot, then a signal on the left";
            g.drawString(hint, plotLeft + 20, area.y + Math.min(30, area.height - 4));
        }

        Graphics2D plot = (Graphics2D) g.create();
        try {
            plot.clipRect(area.x, area.y - 1, area.width + 1, area.height + 3);
            plot.setStroke(new BasicStroke(palette.strokeWidth));
            for (int field : fields) {
                plot.setColor(palette.trace(layout.colors().slot(field)));
                plot.draw(tracePath(field));
            }
            if (selectionFrom >= 0 && selectionTo != selectionFrom) {
                plot.setColor(palette.selection);
                plot.fillRect(Math.min(selectionFrom, selectionTo), area.y, Math.abs(selectionTo - selectionFrom), area.height);
            }
            int cursorLine = cursorLineX();
            if (cursorLine >= 0) {
                plot.setColor(palette.cursor);
                plot.setStroke(new BasicStroke(1));
                plot.drawLine(cursorLine, area.y, cursorLine, area.y + area.height);
            }
            if (cursorLine >= 0 && !grid.isSnapCursor()) {
                // The line no longer sits on the sample being read, so mark that sample on each trace.
                int sampleX = xOf(log.value(cursorSample, 0));
                for (int field : fields) {
                    double value = log.value(cursorSample, field);
                    if (Double.isFinite(value)) {
                        int y = (int) Math.round(yOf(field, value, area));
                        plot.setColor(palette.trace(layout.colors().slot(field)));
                        plot.fillOval(sampleX - 3, y - 3, 7, 7);
                    }
                }
            }
        } finally {
            plot.dispose();
        }
    }

    private void drawTimeAxis(Graphics2D g, FontMetrics metrics, PlotPalette palette, int axisTop, List<Double> sampleTimes) {
        double step = timeTickStep();
        int decimals = Math.max(0, Math.min(6, (int) -Math.floor(Math.log10(step))));
        g.setColor(palette.label);
        for (double time : timeTicks()) {
            String text = String.format(Locale.ROOT, "%." + decimals + "f", time);
            g.drawString(text, xOf(time) - metrics.stringWidth(text) / 2, axisTop + 19);
        }
        String timeLabel = GAUGE_NAME_TIME + " (s)";
        if (grid.isVertical() && grid.getVerticalMode() == ChartSettings.VerticalMode.SAMPLES && sampleTimes == null) {
            timeLabel += "  ·  zoom in to see sample lines";
        }
        g.drawString(timeLabel, (plotLeft + plotRight - metrics.stringWidth(timeLabel)) / 2, axisTop + 37);
    }

    private void drawAddPlotRow(Graphics2D g, FontMetrics metrics, PlotPalette palette) {
        Rectangle row = addPlotRow;
        boolean target = dropTarget == ADD_ROW_TARGET;
        if (target) {
            g.setColor(palette.selection);
            g.fillRoundRect(row.x, row.y, row.width, row.height, 8, 8);
        }
        g.setColor(target ? palette.accent : palette.grid);
        g.setStroke(new BasicStroke(1, BasicStroke.CAP_BUTT, BasicStroke.JOIN_ROUND, 1, new float[]{4, 4}, 0));
        g.drawRoundRect(row.x, row.y, row.width, row.height, 8, 8);
        g.setStroke(new BasicStroke(1));
        String text = !layout.canAddPlot() ? "Maximum of " + PlotLayout.MAX_PLOTS + " plots"
                : target ? "Drop to show it in a new plot" : "+  Add plot  (or drop a signal here)";
        g.setColor(target ? palette.label : palette.hint);
        g.drawString(text, row.x + (row.width - metrics.stringWidth(text)) / 2,
                row.y + (row.height + metrics.getAscent() - metrics.getDescent()) / 2);
    }

    /** Screen x of the cursor line, or -1 when the pointer is outside the plots. */
    int cursorLineX() {
        if (cursorSample < 0) {
            return -1;
        }
        return grid.isSnapCursor() ? xOf(log.value(cursorSample, 0)) : cursorX;
    }

    /** Screen y of {@code value} inside {@code area}; each field is scaled to its own whole-log range. */
    private double yOf(int field, double value, Rectangle area) {
        double min = log.minimum(field);
        double range = log.maximum(field) - min;
        return area.y + (1 - (range == 0 ? 0.5 : (value - min) / range)) * area.height;
    }

    /** Seconds between time-axis ticks, chosen so ticks are roughly the configured spacing apart. */
    double timeTickStep() {
        return niceStep(window.span() / Math.max(2, (plotRight - plotLeft) / (double) grid.getTimeSpacing()));
    }

    private List<Double> timeTicks() {
        double step = timeTickStep();
        List<Double> ticks = new ArrayList<>();
        long lastTick = (long) Math.floor(window.end() / step + 1e-6);
        for (long tick = (long) Math.ceil(window.start() / step - 1e-6); tick <= lastTick; tick++) {
            ticks.add(tick * step);
        }
        return ticks;
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
     * Line through the visible samples of {@code field} in its plot, plus one sample beyond each edge.
     * Samples landing in the same pixel column collapse to a vertical min/max stroke, so the
     * path size depends on the plot width rather than on the number of samples.
     */
    Path2D tracePath(int field) {
        Path2D path = new Path2D.Double();
        int plot = layout.plotOf(field);
        if (!Double.isFinite(log.minimum(field)) || plot < 0 || plot >= bands.size()) {
            return path;
        }
        Rectangle area = bands.get(plot).traces;
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
            column.add(x, yOf(field, value, area));
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

    private String unitSuffix(int field) {
        return log.unit(field).isEmpty() ? "" : " " + log.unit(field);
    }

    /** Value at the cursor sample, or the whole-log range when there is no cursor. */
    private String legendValue(int field, int sample) {
        if (sample >= 0 && Double.isFinite(log.value(sample, field))) {
            return String.format(Locale.ROOT, "%.2f", log.value(sample, field));
        }
        return String.format(Locale.ROOT, "%.1f–%.1f", log.minimum(field), log.maximum(field));
    }
}
