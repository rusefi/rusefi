package com.rusefi.ui.logview;

import com.rusefi.logview.CsvLogReader;
import com.rusefi.ui.widgets.ConsoleFonts;

import javax.swing.JComponent;
import javax.swing.JMenuItem;
import javax.swing.AbstractAction;
import javax.swing.ButtonGroup;
import javax.swing.JCheckBoxMenuItem;
import javax.swing.KeyStroke;
import javax.swing.JColorChooser;
import javax.swing.JMenu;
import javax.swing.JLabel;
import javax.swing.JOptionPane;
import javax.swing.JPanel;
import javax.swing.JPopupMenu;
import javax.swing.JRadioButtonMenuItem;
import javax.swing.JTextField;
import javax.swing.SwingUtilities;
import javax.swing.TransferHandler;
import java.awt.BasicStroke;
import java.awt.Color;
import java.awt.Cursor;
import java.awt.Font;
import java.awt.FontMetrics;
import java.awt.Graphics;
import java.awt.Graphics2D;
import java.awt.GridLayout;
import java.awt.Point;
import java.awt.Polygon;
import java.awt.Rectangle;
import java.awt.RenderingHints;
import java.awt.Stroke;
import java.awt.datatransfer.Transferable;
import java.awt.dnd.DropTargetAdapter;
import java.awt.dnd.DropTargetDropEvent;
import java.awt.dnd.DropTargetEvent;
import java.awt.event.ActionEvent;
import java.awt.event.InputEvent;
import java.awt.event.KeyEvent;
import java.awt.event.MouseAdapter;
import java.awt.event.MouseEvent;
import java.awt.event.MouseWheelEvent;
import java.awt.geom.Path2D;
import java.util.ArrayList;
import java.util.LinkedHashSet;
import java.util.Set;
import java.util.Arrays;
import java.util.List;
import java.util.function.BiFunction;
import java.util.Locale;
import java.util.TooManyListenersException;

import static com.rusefi.config.generated.VariableRegistryValues.GAUGE_NAME_TIME;

/**
 * Stacked time plots from a {@link PlotLayout}, sharing one time axis, zoom, range selection and cursor.
 * Each plot has a header with its legend (hover a legend item for x to remove it, drag it to another plot)
 * and a ⋯ menu; the row below the time axis adds a plot, and signals can be dropped on a plot or on that
 * row. Dragging the gap between two plots resizes them, double-clicking it makes all plots equal.
 * Clicking a plot makes it active, so new signals go there. A box beside the cursor line lists each plot's
 * values at the cursor ({@link ChartSettings.CursorValues} sets when).
 * Wheel zooms around the pointer, drag selects a range to zoom into, right-drag or Shift+wheel pans,
 * double-click shows the whole log. Right-clicking the traces offers the log edits ({@link EditActions}).
 */
final class LogChart extends JComponent {
    /** Each wheel notch scales the visible span by this factor. */
    static final double WHEEL_ZOOM = 1.25;
    /** Fraction of the visible span that one Shift+wheel notch pans. */
    static final double WHEEL_PAN = 0.1;
    /** A drag shorter than this many pixels is a click, not a range selection. */
    static final int MIN_SELECTION_PIXELS = 5;

    /** Left of the plots: room for the y axis labels. */
    static final int LEFT = 64;
    /** Gap between the y axis labels and the plot (the active-plot bar sits in it). */
    private static final int AXIS_LABEL_GAP = 16;
    private static final int RIGHT_MARGIN = 22;
    /** Right of the plots when a plot has a second unit: room for the right y axis labels. */
    static final int RIGHT_AXIS_MARGIN = 58;
    /** Gap between the plot and the right y axis labels. */
    private static final int RIGHT_AXIS_GAP = 8;
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
    /** Pointer y while it is over the chart, or -1; places the cursor value list boxes. */
    private int pointerY = -1;
    /** {@link ChartSettings#isPinValues()}: the clicked sample and y, or -1. */
    private int clickedSample = -1;
    private int clickedY = -1;
    /** The cursor value labels and list boxes from the last paint. */
    private List<ValueBox> valueBoxes = new ArrayList<>();
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
    private final List<Runnable> viewListeners = new ArrayList<>();
    private final VisibleStats stats;
    private final TraceFocus focus = new TraceFocus();
    /** Legend item pressed and not yet dragged; releasing it toggles the pin. */
    private int legendPressField = -1;
    /** The A and B measurement cursors; see {@link #setMeasuring}. */
    private final MeasureCursors cursors = new MeasureCursors();
    /** Shaded time spans; see {@link #setHighlights}. */
    private List<double[]> highlights = new ArrayList<>();
    private int currentHighlight = -1;
    /** While on, a click on a plot places or moves a measurement cursor. */
    private boolean measuring;
    /** The cursor whose flag is being dragged, or null. */
    private MeasureCursors.Which draggingCursor;
    private final List<Runnable> measuringListeners = new ArrayList<>();
    /** Legend item whose colour mark is under the pointer, or -1. */
    private int hoveredSwatch = -1;
    /** Legend item right-pressed, for its menu on release unless the press became a pan. */
    private LegendItem legendRightPress;
    private int rightPressX;
    private SignalColors signalColors = new SignalColors();
    private SignalScales signalScales = new SignalScales();
    /** Statistics of the visible time only (not A–B), for signals scaled to the visible time. */
    private final VisibleStats windowStats;
    /** Asks for a custom scale (signal name, current {min, max}); null when cancelled. Replaced in tests. */
    BiFunction<String, double[], double[]> customScalePrompt = this::askCustomScale;
    private final List<Runnable> colorListeners = new ArrayList<>();
    /** Asks for a custom colour (signal name, current colour); null when cancelled. Replaced in tests. */
    BiFunction<String, Color, Color> customColorPrompt = (label, initial) ->
            JColorChooser.showDialog(this, "Colour of " + label, initial);
    // markers and card from the last paint, for tests
    private List<Marker> markers = new ArrayList<>();
    private Rectangle card;
    private List<String> cardLines = new ArrayList<>();

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
        stats = new VisibleStats(log, window, cursors);
        windowStats = new VisibleStats(log, window);
        cursors.addListener(() -> {
            repaint();
            fireViewChanged();
        });
        setFocusable(true);
        bindKeys();
        // registers with the tooltip manager; getToolTipText answers only over the y axes
        setToolTipText("");
        setOpaque(true);
        MouseAdapter mouse = new MouseHandler();
        addMouseListener(mouse);
        addMouseMotionListener(mouse);
        addMouseWheelListener(mouse);
        layout.addListener(this::repaint);
        layout.addListener(() -> focus.retain(layout::isPlotted));
        focus.addListener(this::repaint);
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
    void addViewListener(Runnable listener) {
        viewListeners.add(listener);
    }

    private void fireViewChanged() {
        for (Runnable listener : viewListeners) {
            listener.run();
        }
    }

    /** The trace under the pointer in a legend or the Stats table, or the pinned one. */
    TraceFocus focus() {
        return focus;
    }

    List<Marker> markers() {
        return markers;
    }

    /** The detail card of the hovered legend item from the last paint, or null. */
    Rectangle card() {
        return card;
    }

    List<String> cardLines() {
        return cardLines;
    }

    /** A min or max marker of the focused trace: the point on the trace and the value tag next to it. */
    static final class Marker {
        final boolean max;
        final Point point;
        final Rectangle tag;
        final String text;

        Marker(boolean max, Point point, Rectangle tag, String text) {
            this.max = max;
            this.point = point;
            this.tag = tag;
            this.text = text;
        }
    }

    /** Sample under the cursor line, or -1 when the pointer is outside the plots. */
    int cursorSample() {
        return cursorSample;
    }

    /** Statistics of the visible time window, shared with the Stats panel. */
    VisibleStats visibleStats() {
        return stats;
    }

    CsvLogReader log() {
        return log;
    }

    /** Time under the cursor line, or NaN when the pointer is outside the plots. */
    double cursorTime() {
        if (cursorSample < 0) {
            return Double.NaN;
        }
        return grid.isSnapCursor() ? log.value(cursorSample, 0) : timeAt(cursorX);
    }

    MeasureCursors cursors() {
        return cursors;
    }

    /**
     * Shades time spans on every plot, e.g. the stays in a histogram cell, the current one stronger; an empty
     * list clears them.
     *
     * @param spans   {start, end, ...} in seconds
     * @param current the span drawn stronger, or -1
     */
    void setHighlights(List<double[]> spans, int current) {
        highlights = new ArrayList<>(spans);
        currentHighlight = current;
        repaint();
    }

    List<double[]> highlights() {
        return highlights;
    }

    int currentHighlight() {
        return currentHighlight;
    }

    /** Brings a time span into view with as much again on either side, so what led to it and followed shows. */
    void reveal(double from, double to) {
        double pad = Math.max(to - from, window.span() * 0.02);
        window.show(from - pad, to + pad);
        viewChanged();
    }

    boolean isMeasuring() {
        return measuring;
    }

    /** Measure mode (the toolbar's Measure button): clicks place A, then B. Turning it off removes them. */
    void setMeasuring(boolean measuring) {
        boolean changed = this.measuring != measuring;
        this.measuring = measuring;
        if (!measuring) {
            cursors.clear();
        }
        repaint();
        if (changed) {
            for (Runnable listener : measuringListeners) {
                listener.run();
            }
        }
    }

    /** Called when Measure turns on or off, including from the A / B keys, e.g. to update the toolbar. */
    void addMeasuringListener(Runnable listener) {
        measuringListeners.add(listener);
    }

    /** Keys samples move per step with Shift held. */
    static final int FAST_STEP = 10;
    /** Window span factor of one zoom key press: two wheel notches. */
    static final double KEY_ZOOM = WHEEL_ZOOM * WHEEL_ZOOM;

    /**
     * Keys while the plots have focus (a click on them gives it): the arrows step the cursor sample by sample
     * (Shift: {@link #FAST_STEP}), Ctrl+arrows pan half the visible time, + / - zoom around the cursor, Home / End
     * go to the first / last sample, 0 shows the whole log, A / B drop a measurement cursor at the cursor and Esc
     * clears A and B, then the pinned trace. Listed in ShortcutsDialog.
     */
    private void bindKeys() {
        bindKey("stepLeft", () -> stepCursor(-1), KeyEvent.VK_LEFT, 0);
        bindKey("stepRight", () -> stepCursor(1), KeyEvent.VK_RIGHT, 0);
        bindKey("fastLeft", () -> stepCursor(-FAST_STEP), KeyEvent.VK_LEFT, InputEvent.SHIFT_DOWN_MASK);
        bindKey("fastRight", () -> stepCursor(FAST_STEP), KeyEvent.VK_RIGHT, InputEvent.SHIFT_DOWN_MASK);
        bindKey("panLeft", () -> panBy(-0.5), KeyEvent.VK_LEFT, InputEvent.CTRL_DOWN_MASK);
        bindKey("panRight", () -> panBy(0.5), KeyEvent.VK_RIGHT, InputEvent.CTRL_DOWN_MASK);
        bindKey("zoomIn", () -> zoomByKey(1 / KEY_ZOOM), KeyEvent.VK_PLUS, 0);
        bindKey("zoomIn", null, KeyEvent.VK_EQUALS, 0);
        bindKey("zoomIn", null, KeyEvent.VK_EQUALS, InputEvent.SHIFT_DOWN_MASK);
        bindKey("zoomIn", null, KeyEvent.VK_ADD, 0);
        bindKey("zoomOut", () -> zoomByKey(KEY_ZOOM), KeyEvent.VK_MINUS, 0);
        bindKey("zoomOut", null, KeyEvent.VK_SUBTRACT, 0);
        bindKey("first", () -> moveCursorTo(0), KeyEvent.VK_HOME, 0);
        bindKey("last", () -> moveCursorTo(log.sampleCount() - 1), KeyEvent.VK_END, 0);
        bindKey("wholeLog", () -> {
            window.reset();
            viewChanged();
        }, KeyEvent.VK_0, 0);
        bindKey("wholeLog", null, KeyEvent.VK_NUMPAD0, 0);
        bindKey("dropA", () -> dropCursor(MeasureCursors.Which.A), KeyEvent.VK_A, 0);
        bindKey("dropB", () -> dropCursor(MeasureCursors.Which.B), KeyEvent.VK_B, 0);
        bindKey("escape", () -> {
            if (clickedSample >= 0) {
                clickedSample = -1;
                repaint();
            } else if (!cursors.isEmpty()) {
                cursors.clear();
            } else if (focus.pinned() != TraceFocus.NONE) {
                focus.togglePin(focus.pinned());
            }
        }, KeyEvent.VK_ESCAPE, 0);
    }

    /** Binds a key while the plots have focus; a null action reuses the one already named. */
    private void bindKey(String name, Runnable action, int key, int modifiers) {
        getInputMap(WHEN_FOCUSED).put(KeyStroke.getKeyStroke(key, modifiers), name);
        if (action != null) {
            getActionMap().put(name, new AbstractAction() {
                @Override
                public void actionPerformed(ActionEvent event) {
                    action.run();
                }
            });
        }
    }

    /** Runs a key's action, as pressing it with the plots focused does; for tests. */
    void pressKeyForTests(int key, int modifiers) {
        Object name = getInputMap(WHEN_FOCUSED).get(KeyStroke.getKeyStroke(key, modifiers));
        getActionMap().get(name).actionPerformed(new ActionEvent(this, ActionEvent.ACTION_PERFORMED, ""));
    }

    /** The sample a key moves from: the cursor's, else the one in the middle of the visible time. */
    private int keyboardStart() {
        return cursorSample >= 0 ? cursorSample : log.nearestSample((window.start() + window.end()) / 2);
    }

    private void stepCursor(int samples) {
        if (cursorSample < 0) {
            moveCursorTo(keyboardStart());
            return;
        }
        moveCursorTo(cursorSample + samples);
    }

    /**
     * Puts the cursor on a sample, scrolling the view to keep it visible. It stays there until the pointer
     * moves over the plots.
     */
    private void moveCursorTo(int sample) {
        int clamped = Math.max(0, Math.min(log.sampleCount() - 1, sample));
        double time = log.value(clamped, 0);
        if (time < window.start()) {
            window.pan(time - window.start());
        } else if (time > window.end()) {
            window.pan(time - window.end());
        }
        cursorSample = clamped;
        cursorX = xOf(time);
        viewChanged();
    }

    private void panBy(double spans) {
        window.pan(spans * window.span());
        if (cursorSample >= 0) {
            cursorX = xOf(log.value(cursorSample, 0));
        }
        viewChanged();
    }

    private void zoomByKey(double factor) {
        double anchor = cursorSample >= 0 ? log.value(cursorSample, 0) : (window.start() + window.end()) / 2;
        window.zoom(anchor, factor);
        if (cursorSample >= 0) {
            cursorX = xOf(log.value(cursorSample, 0));
        }
        viewChanged();
    }

    /** Drops cursor A or B at the cursor (turning Measure on), or in the middle of the view without one. */
    private void dropCursor(MeasureCursors.Which which) {
        if (!measuring) {
            setMeasuring(true);
        }
        cursors.set(which, log.value(keyboardStart(), 0));
    }

    private void viewChanged() {
        repaint();
        fireViewChanged();
    }

    /** The time a click at {@code x} stands for: the nearest sample's while the cursor snaps. */
    private double cursorTimeAt(int x) {
        double time = timeAt(x);
        return grid.isSnapCursor() ? log.value(log.nearestSample(time), 0) : time;
    }

    /** The draggable flag of a measurement cursor at the top of the plots, or null when not shown. */
    Rectangle cursorFlag(MeasureCursors.Which which) {
        if (!cursors.has(which) || bands.isEmpty()) {
            return null;
        }
        int x = xOf(cursors.time(which));
        if (!insidePlot(x)) {
            return null;
        }
        return new Rectangle(x - FLAG_SIZE / 2, bands.get(0).traces.y, FLAG_SIZE, FLAG_SIZE);
    }

    private MeasureCursors.Which cursorFlagAt(int x, int y) {
        for (MeasureCursors.Which which : MeasureCursors.Which.values()) {
            Rectangle flag = cursorFlag(which);
            if (flag != null && flag.contains(x, y)) {
                return which;
            }
        }
        return null;
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

    /** Shares the user's signal colours, e.g. those of the viewer host, kept from one log to the next. */
    void setSignalColors(SignalColors signalColors) {
        this.signalColors = signalColors;
        repaint();
    }

    SignalColors signalColors() {
        return signalColors;
    }

    /** Called after the user changes a signal's colour, e.g. to repaint the signal list. */
    void addColorListener(Runnable listener) {
        colorListeners.add(listener);
    }

    private void colorsChanged() {
        repaint();
        for (Runnable listener : colorListeners) {
            listener.run();
        }
    }

    /** The colour mark of a legend item (and a little around it), which opens the colour picker. */
    static Rectangle colorMark(LegendItem item) {
        return new Rectangle(item.bounds.x - 4, item.bounds.y, 21, item.bounds.height);
    }

    /** The colour picker, shown under a legend item's colour mark. */
    JPopupMenu colorMenu(int field) {
        JPopupMenu menu = new JPopupMenu();
        fillColorMenu(menu, field);
        return menu;
    }

    /** Right-click menu of a legend item. */
    JPopupMenu legendMenu(int field) {
        JPopupMenu menu = new JPopupMenu();
        JMenuItem remove = new JMenuItem("Remove from plot");
        remove.addActionListener(e -> layout.remove(field));
        menu.add(remove);
        menu.addSeparator();
        menu.add(colorSubmenu(field));
        menu.add(scaleSubmenu(field));
        return menu;
    }

    /** "Colour ▸" with the colour picker, for a signal's right-click menu; its icon shows the colour. */
    JMenu colorSubmenu(int field) {
        JMenu colour = new JMenu("Colour");
        Color shown = shownColor(field);
        if (shown != null) {
            TraceDot dot = new TraceDot();
            dot.set(shown, shown.darker());
            colour.setIcon(dot);
        }
        fillColorMenu(colour, field);
        return colour;
    }

    /** The colour the signal has now: its choice, or its automatic colour while plotted; else null. */
    private Color shownColor(int field) {
        Color chosen = signalColors.resolve(log.name(field), palette());
        if (chosen != null || !layout.isPlotted(field)) {
            return chosen;
        }
        return palette().trace(layout.colors().slot(field));
    }

    /** Swatches, then Automatic and Custom…, in a popup or a submenu. */
    private void fillColorMenu(JComponent menu, int field) {
        String label = log.name(field);
        int chosen = signalColors.swatch(label);
        Color custom = signalColors.custom(label);
        int current = chosen >= 0 ? chosen
                : custom == null && layout.isPlotted(field) ? layout.colors().slot(field) : -1;
        menu.add(new SwatchGrid(palette(), current, swatch -> {
            signalColors.setSwatch(label, swatch);
            colorsChanged();
        }));
        if (menu instanceof JMenu) {
            ((JMenu) menu).addSeparator();
        } else {
            ((JPopupMenu) menu).addSeparator();
        }
        JCheckBoxMenuItem automatic = new JCheckBoxMenuItem("Automatic", chosen < 0 && custom == null);
        automatic.setToolTipText("The colour the viewer gives the signal");
        automatic.addActionListener(e -> {
            signalColors.setAutomatic(label);
            colorsChanged();
        });
        menu.add(automatic);
        JCheckBoxMenuItem customItem = new JCheckBoxMenuItem("Custom…", custom != null);
        customItem.setToolTipText("Any colour; it stays the same on the light and dark plot");
        customItem.addActionListener(e -> {
            Color shown = shownColor(field);
            Color picked = customColorPrompt.apply(label, shown != null ? shown : palette().trace(0));
            if (picked != null) {
                signalColors.setCustom(label, picked);
                colorsChanged();
            }
        });
        menu.add(customItem);
    }

    /**
     * "Left axis ▸" / "Right axis ▸": Automatic, (right only) None, then the plot's units. A unit fixes that side;
     * the unit on the other side cannot be picked. Off while the plot scales every signal separately, when the
     * axis follows the highlighted trace instead.
     */
    JMenu axisMenu(int plot, boolean left) {
        JMenu axis = new JMenu(left ? "Left axis" : "Right axis");
        boolean shared = layout.isSameUnitShared(plot);
        axis.setEnabled(shared);
        axis.setToolTipText(shared ? "Which unit's scale this axis shows"
                : "With separate scales the axis shows the highlighted trace's scale, on the left");
        String chosen = left ? layout.leftAxisUnit(plot) : layout.rightAxisUnit(plot);
        String other = left ? layout.rightAxisUnit(plot) : leftAxisUnit(plot);
        ButtonGroup group = new ButtonGroup();
        List<String> keys = new ArrayList<>();
        keys.add(null);
        if (!left) {
            keys.add(PlotLayout.NO_AXIS);
        }
        keys.addAll(unitsIn(plot));
        for (String key : keys) {
            String text = key == null ? "Automatic" : PlotLayout.NO_AXIS.equals(key) ? "None" : unitLabel(key);
            JRadioButtonMenuItem item = new JRadioButtonMenuItem(text, java.util.Objects.equals(key, chosen));
            item.setEnabled(key == null || PlotLayout.NO_AXIS.equals(key) || !key.equals(other));
            item.addActionListener(e -> {
                if (left) {
                    layout.setLeftAxisUnit(plot, key);
                } else {
                    layout.setRightAxisUnit(plot, key);
                }
            });
            group.add(item);
            axis.add(item);
        }
        return axis;
    }

    /** Menu behind a plot's ⋯ button. */
    /** Edits of the log offered by right-clicking the traces; see {@link #setEditActions}. */
    interface EditActions {
        /** Saves the samples of the visible time to a file of their own. */
        void exportVisible();

        /** Keeps the samples from {@code sample} on. */
        void dropBefore(int sample);

        /** Keeps the samples up to {@code sample}. */
        void dropAfter(int sample);
    }

    private EditActions editActions;

    /** Offers the edits when the traces are right-clicked; null for no menu. */
    void setEditActions(EditActions editActions) {
        this.editActions = editActions;
    }

    private boolean onTraces(int x, int y) {
        for (Band band : bands) {
            if (band.traces.contains(x, y)) {
                return true;
            }
        }
        return false;
    }

    /** Right-click menu of the traces: the edits, at the clicked sample. */
    JPopupMenu editMenu(int sample) {
        JPopupMenu menu = new JPopupMenu();
        String time = String.format(Locale.ROOT, "%.2f s", log.value(sample, 0));
        JMenuItem export = new JMenuItem("Export visible data as…");
        export.setToolTipText("Save every sample of the visible time to a new log file");
        export.addActionListener(e -> editActions.exportVisible());
        JMenuItem before = new JMenuItem("Drop data before cursor (" + time + ")");
        before.setEnabled(sample > 0);
        before.addActionListener(e -> editActions.dropBefore(sample));
        JMenuItem after = new JMenuItem("Drop data after cursor (" + time + ")");
        after.setEnabled(sample < log.sampleCount() - 1);
        after.addActionListener(e -> editActions.dropAfter(sample));
        menu.add(export);
        menu.addSeparator();
        menu.add(before);
        menu.add(after);
        return menu;
    }

    JPopupMenu plotMenu(int plot) {
        JPopupMenu menu = new JPopupMenu();
        JMenuItem up = new JMenuItem("Move up");
        up.setEnabled(plot > 0);
        up.addActionListener(e -> layout.move(plot, -1));
        JMenuItem down = new JMenuItem("Move down");
        down.setEnabled(plot < layout.plotCount() - 1);
        down.addActionListener(e -> layout.move(plot, 1));
        if (measuring || !cursors.isEmpty()) {
            JMenuItem zoom = new JMenuItem("Zoom to A–B");
            zoom.setEnabled(cursors.isRange() && cursors.end() > cursors.start());
            zoom.addActionListener(e -> {
                window.show(cursors.start(), cursors.end());
                repaint();
                fireViewChanged();
            });
            JMenuItem clearCursors = new JMenuItem("Clear A/B");
            clearCursors.setEnabled(!cursors.isEmpty());
            clearCursors.addActionListener(e -> cursors.clear());
            menu.add(zoom);
            menu.add(clearCursors);
            menu.addSeparator();
        }
        JCheckBoxMenuItem sameUnit = new JCheckBoxMenuItem("Same unit, same scale", layout.isSameUnitShared(plot));
        sameUnit.setToolTipText("Signals with the same unit (e.g. CLT and IAT in °C) share one scale, so their "
                + "values can be compared; off scales every signal to its own range");
        sameUnit.addActionListener(e -> layout.setSameUnitShared(plot, sameUnit.isSelected()));
        menu.add(sameUnit);
        menu.add(axisMenu(plot, true));
        menu.add(axisMenu(plot, false));
        menu.addSeparator();
        JMenuItem clear = new JMenuItem("Clear plot");
        clear.setToolTipText("Removes all signals from this plot and keeps the plot");
        clear.setEnabled(!layout.fields(plot).isEmpty());
        clear.addActionListener(e -> layout.clearPlot(plot));
        JMenuItem remove = new JMenuItem("Remove plot");
        remove.setEnabled(layout.canRemovePlot());
        remove.addActionListener(e -> layout.removePlot(plot));
        menu.insert(up, 0);
        menu.insert(down, 1);
        menu.insert(new JPopupMenu.Separator(), 2);
        menu.add(clear);
        menu.add(remove);
        return menu;
    }

    private final class MouseHandler extends MouseAdapter {
        @Override
        public void mouseMoved(MouseEvent event) {
            boolean inside = insidePlot(event.getX());
            cursorX = inside ? event.getX() : -1;
            cursorSample = inside ? log.nearestSample(timeAt(event.getX())) : -1;
            pointerY = event.getY();
            LegendItem item = legendAt(event.getX(), event.getY());
            hoveredField = item != null ? item.field : -1;
            hoveredSwatch = item != null && colorMark(item).contains(event.getX(), event.getY()) ? item.field : -1;
            if (item != null) {
                focus.hover(item.field);
            } else {
                focus.clearHover();
            }
            Band band = bandAt(event.getX(), event.getY());
            hoveredMenu = band != null && band.menu.contains(event.getX(), event.getY()) ? band.plot : -1;
            if (dividerAt(event.getX(), event.getY()) >= 0) {
                setCursor(Cursor.getPredefinedCursor(Cursor.N_RESIZE_CURSOR));
            } else if (cursorFlagAt(event.getX(), event.getY()) != null) {
                setCursor(Cursor.getPredefinedCursor(Cursor.E_RESIZE_CURSOR));
            } else {
                setCursor(onControl(event.getX(), event.getY())
                        ? Cursor.getPredefinedCursor(Cursor.HAND_CURSOR) : Cursor.getDefaultCursor());
            }
            repaint();
            fireViewChanged();
        }

        @Override
        public void mouseExited(MouseEvent event) {
            if (selectionFrom < 0 && panFromX < 0) {
                cursorX = cursorSample = pointerY = -1;
                hoveredField = hoveredMenu = hoveredSwatch = -1;
                focus.clearHover();
                repaint();
                fireViewChanged();
            }
        }

        @Override
        public void mousePressed(MouseEvent event) {
            int x = event.getX();
            int y = event.getY();
            // focus for Esc, which clears the measurement cursors
            requestFocusInWindow();
            if (SwingUtilities.isLeftMouseButton(event)) {
                draggingCursor = cursorFlagAt(x, y);
                if (draggingCursor != null) {
                    return;
                }
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
                if (item != null && colorMark(item).contains(x, y)) {
                    colorMenu(item.field).show(LogChart.this, item.bounds.x - 6, item.bounds.y + item.bounds.height + 2);
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
                    legendPressField = item.field;
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
                legendRightPress = SwingUtilities.isRightMouseButton(event) ? legendAt(x, y) : null;
                rightPressX = x;
            }
        }

        @Override
        public void mouseDragged(MouseEvent event) {
            if (draggingCursor != null) {
                cursors.set(draggingCursor, cursorTimeAt(Math.max(plotLeft, Math.min(plotRight, event.getX()))));
                return;
            }
            if (resizing >= 0) {
                resize(event.getY() - resizeFromY);
                return;
            }
            if (dragField >= 0) {
                if (Math.abs(event.getX() - dragFromX) + Math.abs(event.getY() - dragFromY) >= DRAG_START_PIXELS) {
                    exportingField = dragField;
                    dragField = -1;
                    legendPressField = -1;
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
            LegendItem rightPressed = legendRightPress;
            legendRightPress = null;
            if (rightPressed != null && SwingUtilities.isRightMouseButton(event)
                    && Math.abs(event.getX() - rightPressX) < DRAG_START_PIXELS) {
                // a right-click on a legend item, not a right-drag pan
                legendMenu(rightPressed.field).show(LogChart.this, event.getX(), event.getY());
            } else if (rightPressed == null && editActions != null && SwingUtilities.isRightMouseButton(event)
                    && Math.abs(event.getX() - rightPressX) < DRAG_START_PIXELS && insidePlot(event.getX())
                    && onTraces(event.getX(), event.getY())) {
                // a right-click on the traces, not a right-drag pan
                editMenu(log.nearestSample(timeAt(event.getX()))).show(LogChart.this, event.getX(), event.getY());
            }
            if (draggingCursor != null) {
                draggingCursor = null;
                return;
            }
            if (selectionFrom >= 0 && Math.abs(selectionTo - selectionFrom) >= MIN_SELECTION_PIXELS) {
                window.show(timeAt(selectionFrom), timeAt(selectionTo));
            } else if (selectionFrom >= 0 && measuring) {
                // a click, not a range: place A, then B, then move the nearer one
                int x = selectionFrom;
                cursors.place(cursorTimeAt(x), distanceTo(MeasureCursors.Which.A, x), distanceTo(MeasureCursors.Which.B, x));
            } else if (selectionFrom >= 0 && grid.isPinValues()) {
                // a click, not a range: show the values there, or hide them on a second click at the same sample
                int sample = log.nearestSample(timeAt(selectionFrom));
                clickedSample = sample == clickedSample ? -1 : sample;
                clickedY = event.getY();
            }
            if (legendPressField >= 0) {
                // a click on a legend item, not a drag: keep its trace highlighted, or stop
                focus.togglePin(legendPressField);
                legendPressField = -1;
            }
            selectionFrom = selectionTo = -1;
            panFromX = -1;
            resizing = -1;
            dragField = -1;
            repaint();
            fireViewChanged();
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
                fireViewChanged();
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
            axisMetrics = metrics;
            // Fixed-width digits so legend values do not jitter as the cursor moves.
            Font valueFont = ConsoleFonts.mono(Font.PLAIN, g.getFont().getSize());
            FontMetrics valueMetrics = g.getFontMetrics(valueFont);
            plotLeft = LEFT;
            plotRight = Math.max(LEFT + 1, getWidth() - (anyRightAxis() ? RIGHT_AXIS_MARGIN : RIGHT_MARGIN));
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
            drawTimeBadge(g, metrics, palette, axisTop);
            drawTimeTitle(g, metrics, palette, axisTop);
            drawMeasureCursors(g, metrics, palette, axisTop);
            drawAddPlotRow(g, metrics, palette);
            markers = new ArrayList<>();
            int focused = focus.focused();
            int focusedPlot = layout.plotOf(focused);
            if (focusedPlot >= 0 && focusedPlot < bands.size()) {
                markers = markers(bands.get(focusedPlot).traces, focused, g.getFontMetrics(valueFont));
                drawMarkers(g, palette, valueFont, focused);
            }
            valueBoxes = new ArrayList<>();
            drawValueBoxes(g, metrics, valueFont, palette);
            card = null;
            cardLines = new ArrayList<>();
            LegendItem hovered = hoveredLegendItem();
            if (hovered != null) {
                drawCard(g, hovered, metrics, valueFont, palette);
            }
        } finally {
            g.dispose();
        }
    }

    /** The highlighted spans behind the traces: each at least 2 px wide, the current one edged in the accent. */
    private void paintHighlights(Graphics2D g, Rectangle area, PlotPalette palette) {
        for (int i = 0; i < highlights.size(); i++) {
            double[] span = highlights.get(i);
            if (span[1] < window.start() || span[0] > window.end()) {
                continue;
            }
            int left = xOf(span[0]);
            int width = Math.max(2, xOf(span[1]) - left);
            g.setColor(palette.selection);
            g.fillRect(left, area.y, width, area.height);
            if (i == currentHighlight) {
                g.fillRect(left, area.y, width, area.height);
                g.setColor(palette.accent);
                g.drawLine(left, area.y, left, area.y + area.height);
                g.drawLine(left + width, area.y, left + width, area.y + area.height);
            }
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

        // the colour bar is centred on the capital-letter height of the legend text
        int capHeight = (int) Math.round(metrics.getFont().createGlyphVector(g.getFontRenderContext(), "H")
                .getVisualBounds().getHeight());
        for (LegendItem item : band.legend) {
            int baseline = item.bounds.y + 14;
            int barY = baseline - (capHeight + LEGEND_BAR_HEIGHT + 1) / 2;
            int x = item.bounds.x;
            if (item.field == focus.focused()) {
                Rectangle chip = new Rectangle(item.bounds.x - 6, item.bounds.y, item.bounds.width - 6, item.bounds.height);
                g.setColor(palette.selection);
                g.fillRoundRect(chip.x, chip.y, chip.width, chip.height, 8, 8);
                if (item.field == focus.pinned()) {
                    g.setColor(palette.accent);
                    g.drawRoundRect(chip.x, chip.y, chip.width, chip.height, 8, 8);
                }
            }
            g.setColor(colorOf(item.field, palette));
            g.fillRoundRect(x, barY, 13, LEGEND_BAR_HEIGHT, 4, 4);
            if (item.field == hoveredSwatch) {
                // the colour mark opens the colour picker
                g.setColor(palette.accent);
                g.drawRoundRect(x - 4, barY - 4, 21, LEGEND_BAR_HEIGHT + 8, 6, 6);
            }
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

        List<double[]> ticks = axisTicks(band);
        if (grid.isHorizontal()) {
            g.setColor(palette.grid);
            if (ticks.isEmpty()) {
                int divisions = grid.getHorizontalDivisions();
                for (int i = 0; i <= divisions; i++) {
                    int y = area.y + area.height * i / divisions;
                    g.drawLine(plotLeft, y, plotRight, y);
                }
            } else {
                // the lines mark the axis values of the trace the axis shows
                for (double[] tick : ticks) {
                    g.drawLine(plotLeft, (int) Math.round(tick[1]), plotRight, (int) Math.round(tick[1]));
                }
            }
        }
        drawYAxis(g, band, leftAxisField(band), ticks, true, palette);
        drawYAxis(g, band, rightAxisField(band), rightAxisTicks(band), false, palette);
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
            paintHighlights(plot, area, palette);
            plot.setStroke(new BasicStroke(palette.strokeWidth));
            int focused = focus.focused();
            for (int field : fields) {
                if (field != focused) {
                    plot.setColor(traceColor(field, palette));
                    plot.draw(tracePath(field));
                }
            }
            if (fields.contains(focused)) {
                // on top of the faded traces, slightly thicker
                plot.setStroke(new BasicStroke(palette.strokeWidth + 1));
                plot.setColor(traceColor(focused, palette));
                plot.draw(tracePath(focused));
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
                        plot.setColor(traceColor(field, palette));
                        plot.fillOval(sampleX - 3, y - 3, 7, 7);
                    }
                }
            }
        } finally {
            plot.dispose();
        }
    }

    private static final int LEGEND_BAR_HEIGHT = 5;

    /** Alpha of the traces behind a focused one. */
    static final int FADED_ALPHA = 60;

    /** A plotted signal's colour: the user's choice ({@link SignalColors}), else its automatic palette slot. */
    Color colorOf(int field, PlotPalette palette) {
        Color chosen = signalColors.resolve(log.name(field), palette);
        return chosen != null ? chosen : palette.trace(layout.colors().slot(field));
    }

    /** A trace's colour: faded while another trace is focused. */
    Color traceColor(int field, PlotPalette palette) {
        Color color = colorOf(field, palette);
        int focused = focus.focused();
        if (focused == TraceFocus.NONE || focused == field) {
            return color;
        }
        return new Color(color.getRed(), color.getGreen(), color.getBlue(), FADED_ALPHA);
    }

    /**
     * Values at the cursor from the last paint: a label beside one trace ({@link #field} and the trace's
     * {@link #point}), or a list box of a whole plot ({@link #field} -1, no point). One "value unit" line per trace.
     */
    static final class ValueBox {
        final int plot;
        final int field;
        final Rectangle bounds;
        final Point point;
        final List<String> lines;

        ValueBox(int plot, int field, Rectangle bounds, Point point, List<String> lines) {
            this.plot = plot;
            this.field = field;
            this.bounds = bounds;
            this.point = point;
            this.lines = lines;
        }
    }

    /** The cursor value labels and list boxes from the last paint. */
    List<ValueBox> valueBoxes() {
        return valueBoxes;
    }

    private static final int VALUE_ROW = 18;
    private static final int VALUE_PADDING = 7;
    private static final int VALUE_DOT = 8;
    /** Gap between the cursor line and the value labels or list boxes. */
    private static final int VALUE_GAP = 12;
    private static final int LABEL_HEIGHT = 18;
    /** Space between labels pushed apart. */
    private static final int LABEL_SPACING = 2;

    /**
     * Shows the values at the cursor (or at the pinned time) beside the cursor line, as labels or list boxes
     * ({@link ChartSettings.CursorValues}).
     */
    private void drawValueBoxes(Graphics2D g, FontMetrics metrics, Font valueFont, PlotPalette palette) {
        ChartSettings.CursorValues mode = grid.getCursorValues();
        if (mode == ChartSettings.CursorValues.OFF) {
            return;
        }
        int sample;
        int lineX;
        int anchorY;
        if (grid.isPinValues()) {
            if (clickedSample < 0 || clickedSample >= log.sampleCount()) {
                return;
            }
            sample = clickedSample;
            lineX = xOf(log.value(sample, 0));
            if (!insidePlot(lineX)) {
                return;
            }
            anchorY = clickedY;
            // the pinned time keeps its own line, as the cursor line follows the mouse
            g.setColor(palette.cursor);
            g.setStroke(new BasicStroke(1, BasicStroke.CAP_BUTT, BasicStroke.JOIN_MITER, 10, new float[]{4, 3}, 0));
            for (Band band : bands) {
                g.drawLine(lineX, band.traces.y, lineX, band.traces.y + band.traces.height);
            }
            g.setStroke(new BasicStroke(1));
        } else {
            boolean overTraces = false;
            for (Band band : bands) {
                overTraces |= band.traces.contains(cursorX, pointerY);
            }
            boolean dragging = selectionFrom >= 0 && selectionTo != selectionFrom || panFromX >= 0 || resizing >= 0;
            if (cursorSample < 0 || !overTraces || dragging) {
                return;
            }
            sample = cursorSample;
            lineX = cursorLineX();
            anchorY = pointerY;
        }

        for (Band band : bands) {
            List<Integer> fields = layout.fields(band.plot);
            if (fields.isEmpty()) {
                continue;
            }
            if (mode != ChartSettings.CursorValues.LABELS
                    || !drawLabels(g, band, fields, sample, lineX, metrics, valueFont, palette)) {
                drawValueList(g, band, fields, sample, lineX, anchorY, metrics, valueFont, palette);
            }
        }
    }

    /**
     * Tops of labels {@code height} tall, each as close as possible to being centred on its {@code centers} entry
     * (sorted, top to bottom) without overlapping and within {@code top}..{@code bottom}: labels that would
     * overlap are pushed apart, downwards first, then back up from the bottom edge. Null when they do not fit.
     */
    static int[] spreadLabels(double[] centers, int height, int spacing, int top, int bottom) {
        int count = centers.length;
        if (count * height + (count - 1) * spacing > bottom - top) {
            return null;
        }
        int[] tops = new int[count];
        int below = top;
        for (int i = 0; i < count; i++) {
            tops[i] = Math.max(below, (int) Math.round(centers[i] - height / 2.0));
            below = tops[i] + height + spacing;
        }
        int above = bottom;
        for (int i = count - 1; i >= 0; i--) {
            tops[i] = Math.min(tops[i], above - height);
            above = tops[i] - spacing;
        }
        return tops;
    }

    /**
     * A label beside each trace where it crosses the cursor line: the value with a border and a leader line in the
     * trace's colour, and a dot on the trace. False, drawing nothing, when the labels do not fit in the plot.
     */
    private boolean drawLabels(Graphics2D g, Band band, List<Integer> fields, int sample, int lineX,
                               FontMetrics metrics, Font valueFont, PlotPalette palette) {
        Rectangle area = band.traces;
        int sampleX = xOf(log.value(sample, 0));
        List<Integer> shown = new ArrayList<>();
        for (int field : fields) {
            if (Double.isFinite(log.value(sample, field))) {
                shown.add(field);
            }
        }
        // top to bottom, so pushing apart keeps each label on the same side of its neighbours as its trace
        shown.sort((a, b) -> Double.compare(yOf(a, log.value(sample, a), area), yOf(b, log.value(sample, b), area)));
        double[] centers = new double[shown.size()];
        for (int i = 0; i < centers.length; i++) {
            int field = shown.get(i);
            centers[i] = Math.max(area.y, Math.min(area.y + area.height, yOf(field, log.value(sample, field), area)));
        }
        int[] tops = spreadLabels(centers, LABEL_HEIGHT, LABEL_SPACING, area.y, area.y + area.height);
        if (tops == null) {
            return false;
        }

        FontMetrics valueMetrics = g.getFontMetrics(valueFont);
        String[] values = new String[shown.size()];
        int[] widths = new int[shown.size()];
        int widest = 0;
        for (int i = 0; i < values.length; i++) {
            int field = shown.get(i);
            values[i] = ValueFormat.format(log, field, log.value(sample, field));
            widths[i] = 6 + valueMetrics.stringWidth(values[i]) + metrics.stringWidth(unitSuffix(field)) + 6;
            widest = Math.max(widest, widths[i]);
        }
        // all on one side: right of the line, or left of it near the right edge
        boolean left = lineX + VALUE_GAP + widest > plotRight;
        Color background = ThemedStrip.themed("Console.Card.background", Color.WHITE);
        Color ink = ThemedStrip.themed("Label.foreground", Color.BLACK);
        Color muted = ThemedStrip.themed("Console.LogViewer.mutedForeground", Color.GRAY);
        for (int i = 0; i < values.length; i++) {
            int field = shown.get(i);
            Color color = colorOf(field, palette);
            int x = left ? Math.max(plotLeft, lineX - VALUE_GAP - widths[i]) : lineX + VALUE_GAP;
            Rectangle bounds = new Rectangle(x, tops[i], widths[i], LABEL_HEIGHT);
            Point point = new Point(sampleX, (int) Math.round(centers[i]));

            g.setColor(color);
            int edgeX = left ? bounds.x + bounds.width : bounds.x;
            g.drawLine(point.x, point.y, edgeX, bounds.y + LABEL_HEIGHT / 2);
            g.setColor(palette.background);
            g.fillOval(point.x - 4, point.y - 4, 9, 9);
            g.setColor(color);
            g.fillOval(point.x - 3, point.y - 3, 7, 7);
            // slightly transparent, so the traces underneath stay visible
            g.setColor(new Color(background.getRed(), background.getGreen(), background.getBlue(), 235));
            g.fillRoundRect(bounds.x, bounds.y, bounds.width, bounds.height, 8, 8);
            g.setColor(color);
            g.drawRoundRect(bounds.x, bounds.y, bounds.width, bounds.height, 8, 8);
            int baseline = bounds.y + 13;
            g.setFont(valueFont);
            g.setColor(ink);
            g.drawString(values[i], bounds.x + 6, baseline);
            g.setFont(metrics.getFont());
            g.setColor(muted);
            g.drawString(unitSuffix(field), bounds.x + 6 + valueMetrics.stringWidth(values[i]), baseline);
            List<String> lines = new ArrayList<>();
            lines.add(values[i] + unitSuffix(field));
            valueBoxes.add(new ValueBox(band.plot, field, bounds, point, lines));
        }
        return true;
    }

    /**
     * A box beside the cursor line listing the plot's values in legend order: a dot in the trace's colour, then
     * the value. In the plot under the pointer the box follows the pointer, in the others it sits at the top.
     */
    private void drawValueList(Graphics2D g, Band band, List<Integer> fields, int sample, int lineX, int anchorY,
                               FontMetrics metrics, Font valueFont, PlotPalette palette) {
        FontMetrics valueMetrics = g.getFontMetrics(valueFont);
        Color background = ThemedStrip.themed("Console.Card.background", Color.WHITE);
        Color ink = ThemedStrip.themed("Label.foreground", Color.BLACK);
        Color muted = ThemedStrip.themed("Console.LogViewer.mutedForeground", Color.GRAY);
        List<String> values = new ArrayList<>();
        int valueWidth = 0;
        int unitWidth = 0;
        for (int field : fields) {
            String value = ValueFormat.format(log, field, log.value(sample, field));
            values.add(value);
            valueWidth = Math.max(valueWidth, valueMetrics.stringWidth(value));
            unitWidth = Math.max(unitWidth, metrics.stringWidth(unitSuffix(field)));
        }
        int width = VALUE_PADDING + VALUE_DOT + 6 + valueWidth + unitWidth + VALUE_PADDING;
        int height = VALUE_PADDING * 2 + fields.size() * VALUE_ROW - 4;
        Rectangle area = band.traces;
        // right of the line, or left of it near the right edge
        int x = lineX + VALUE_GAP;
        if (x + width > plotRight) {
            x = Math.max(plotLeft, lineX - VALUE_GAP - width);
        }
        boolean pointerHere = anchorY >= area.y && anchorY < area.y + area.height;
        int y = pointerHere ? anchorY + VALUE_GAP : area.y + 6;
        y = Math.max(area.y, Math.min(area.y + area.height - height, y));
        Rectangle bounds = new Rectangle(x, y, width, height);

        g.setColor(new Color(0, 0, 0, 28));
        g.fillRoundRect(x + 2, y + 3, width, height, 10, 10);
        // slightly transparent, so the traces underneath stay visible
        g.setColor(new Color(background.getRed(), background.getGreen(), background.getBlue(), 235));
        g.fillRoundRect(x, y, width, height, 10, 10);
        g.setColor(ThemedStrip.themed("Console.Card.borderColor", Color.LIGHT_GRAY));
        g.drawRoundRect(x, y, width, height, 10, 10);
        List<String> lines = new ArrayList<>();
        for (int i = 0; i < fields.size(); i++) {
            int field = fields.get(i);
            int rowTop = y + VALUE_PADDING + i * VALUE_ROW - 2;
            int baseline = rowTop + 13;
            g.setColor(colorOf(field, palette));
            g.fillOval(x + VALUE_PADDING, rowTop + 4, VALUE_DOT, VALUE_DOT);
            // right-aligned so the digits line up
            int valueRight = x + VALUE_PADDING + VALUE_DOT + 6 + valueWidth;
            g.setFont(valueFont);
            g.setColor(ink);
            g.drawString(values.get(i), valueRight - valueMetrics.stringWidth(values.get(i)), baseline);
            g.setFont(metrics.getFont());
            g.setColor(muted);
            g.drawString(unitSuffix(field), valueRight, baseline);
            lines.add(values.get(i) + unitSuffix(field));
        }
        valueBoxes.add(new ValueBox(band.plot, -1, bounds, null, lines));
    }

    private LegendItem hoveredLegendItem() {
        for (Band band : bands) {
            for (LegendItem item : band.legend) {
                if (item.field == hoveredField) {
                    return item;
                }
            }
        }
        return null;
    }

    private static final int TAG_HEIGHT = 18;
    private static final int TAG_OFFSET = 14;

    /**
     * Markers at the visible max and min of {@code field}. The max tag sits above its point and the min tag
     * below; a tag that would leave the plot goes to the other side, so it never covers a neighbouring plot.
     */
    List<Marker> markers(Rectangle area, int field, FontMetrics valueMetrics) {
        List<Marker> result = new ArrayList<>();
        TraceStats traceStats = stats.get(field);
        if (traceStats.isEmpty()) {
            return result;
        }
        for (boolean max : new boolean[]{true, false}) {
            int sample = max ? traceStats.maxSample : traceStats.minSample;
            if (!max && sample == traceStats.maxSample) {
                // a flat trace: one marker is enough
                continue;
            }
            double value = log.value(sample, field);
            int x = Math.max(area.x, Math.min(area.x + area.width, xOf(log.value(sample, 0))));
            int y = (int) Math.round(yOf(field, value, area));
            String text = ValueFormat.format(log, field, value);
            int width = valueMetrics.stringWidth(text) + 10;
            int tagX = Math.max(area.x, Math.min(area.x + area.width - width, x - width / 2));
            int above = y - TAG_OFFSET - TAG_HEIGHT;
            int below = y + TAG_OFFSET;
            boolean fitsAbove = above >= area.y;
            boolean fitsBelow = below + TAG_HEIGHT <= area.y + area.height;
            int tagY = max ? (fitsAbove || !fitsBelow ? above : below) : (fitsBelow || !fitsAbove ? below : above);
            tagY = Math.max(area.y, Math.min(area.y + area.height - TAG_HEIGHT, tagY));
            result.add(new Marker(max, new Point(x, y), new Rectangle(tagX, tagY, width, TAG_HEIGHT), text));
        }
        return result;
    }

    private void drawMarkers(Graphics2D g, PlotPalette palette, Font valueFont, int field) {
        Color color = colorOf(field, palette);
        Color outline = palette.background;
        g.setFont(valueFont);
        FontMetrics valueMetrics = g.getFontMetrics();
        for (Marker marker : markers) {
            int x = marker.point.x;
            int y = marker.point.y;
            // the triangle points at the trace from the tag's side
            boolean tagAbove = marker.tag.y < y;
            Polygon triangle = tagAbove
                    ? new Polygon(new int[]{x - 6, x + 6, x}, new int[]{y - 12, y - 12, y - 3}, 3)
                    : new Polygon(new int[]{x - 6, x + 6, x}, new int[]{y + 12, y + 12, y + 3}, 3);
            g.setColor(color);
            g.fillPolygon(triangle);
            g.setStroke(new BasicStroke(1));
            g.setColor(outline);
            g.drawPolygon(triangle);
            g.setColor(color);
            g.fillRoundRect(marker.tag.x, marker.tag.y, marker.tag.width, marker.tag.height, 6, 6);
            g.setColor(outline);
            g.drawString(marker.text, marker.tag.x + 5, marker.tag.y + (marker.tag.height + valueMetrics.getAscent() - valueMetrics.getDescent()) / 2);
        }
    }

    /**
     * Where the card goes: under its legend item as usual, unless that hides a min/max marker; then the first
     * clear spot of: slid right past the markers, the top right, the bottom left or right of the plot. With no
     * clear spot it stays in the usual place.
     */
    private Rectangle placeCard(LegendItem item, int width, int height) {
        int x = Math.max(4, Math.min(getWidth() - width - 4, item.bounds.x - 6));
        int y = item.bounds.y + item.bounds.height + 6;
        if (y + height > getHeight() - 4) {
            y = Math.max(4, item.bounds.y - height - 6);
        }
        Rectangle usual = new Rectangle(x, y, width, height);
        List<Rectangle> obstacles = new ArrayList<>();
        for (Marker marker : markers) {
            Rectangle tag = new Rectangle(marker.tag);
            tag.grow(4, 4);
            obstacles.add(tag);
            // the triangle, on either side of the point
            obstacles.add(new Rectangle(marker.point.x - 8, marker.point.y - 14, 16, 28));
        }
        int pastMarkers = x;
        for (Rectangle obstacle : obstacles) {
            if (obstacle.intersects(usual)) {
                pastMarkers = Math.max(pastMarkers, obstacle.x + obstacle.width + 8);
            }
        }
        int right = getWidth() - width - 4;
        Rectangle traces = item.bounds;
        for (Band band : bands) {
            if (band.legend.contains(item)) {
                traces = band.traces;
            }
        }
        int bottom = traces.y + traces.height - height - 4;
        Rectangle[] candidates = {
                usual,
                new Rectangle(pastMarkers, y, width, height),
                new Rectangle(right, y, width, height),
                new Rectangle(x, bottom, width, height),
                new Rectangle(right, bottom, width, height),
        };
        Rectangle chart = new Rectangle(0, 0, getWidth(), getHeight());
        for (Rectangle candidate : candidates) {
            if (chart.contains(candidate) && obstacles.stream().noneMatch(candidate::intersects)) {
                return candidate;
            }
        }
        return usual;
    }

    /** Max, min (with their times) and average of the hovered legend item over the visible window. */
    private void drawCard(Graphics2D g, LegendItem item, FontMetrics metrics, Font valueFont, PlotPalette palette) {
        int field = item.field;
        TraceStats traceStats = stats.get(field);
        String unit = unitSuffix(field);
        String title = log.name(field);
        String range = stats.isBetweenCursors()
                ? String.format(Locale.ROOT, "A–B %.2f–%.2f s", stats.rangeStart(), stats.rangeEnd())
                : String.format(Locale.ROOT, "visible %.1f–%.1f s", window.start(), window.end());
        String[][] rows = traceStats.isEmpty()
                ? new String[][]{{"", "no values in the visible time", ""}}
                : new String[][]{
                {"max", ValueFormat.format(log, field, traceStats.max) + unit,
                        String.format(Locale.ROOT, "@ %.2f s", log.value(traceStats.maxSample, 0))},
                {"min", ValueFormat.format(log, field, traceStats.min) + unit,
                        String.format(Locale.ROOT, "@ %.2f s", log.value(traceStats.minSample, 0))},
                {"avg", ValueFormat.format(log, field, traceStats.average) + unit, ""}};
        FontMetrics valueMetrics = g.getFontMetrics(valueFont);
        Font bold = metrics.getFont().deriveFont(Font.BOLD);
        int labelWidth = 0;
        int valueWidth = 0;
        int timeWidth = 0;
        for (String[] row : rows) {
            labelWidth = Math.max(labelWidth, metrics.stringWidth(row[0]));
            valueWidth = Math.max(valueWidth, valueMetrics.stringWidth(row[1]));
            timeWidth = Math.max(timeWidth, metrics.stringWidth(row[2]));
        }
        int titleWidth = 32 + g.getFontMetrics(bold).stringWidth(title) + 10 + metrics.stringWidth(range);
        int width = Math.max(titleWidth, 14 + labelWidth + 14 + valueWidth + 18 + timeWidth) + 14;
        int lineHeight = 21;
        int height = 34 + rows.length * lineHeight + 4;
        card = placeCard(item, width, height);
        int x = card.x;
        int y = card.y;

        Color background = ThemedStrip.themed("Console.Card.background", Color.WHITE);
        g.setColor(new Color(0, 0, 0, 28));
        g.fillRoundRect(x + 2, y + 3, width, height, 12, 12);
        // slightly transparent, so the traces underneath stay visible
        g.setColor(new Color(background.getRed(), background.getGreen(), background.getBlue(), 240));
        g.fillRoundRect(x, y, width, height, 12, 12);
        g.setColor(ThemedStrip.themed("Console.Card.borderColor", Color.LIGHT_GRAY));
        g.drawRoundRect(x, y, width, height, 12, 12);
        g.setColor(colorOf(field, palette));
        g.fillRoundRect(x + 12, y + 12, 13, 5, 4, 4);
        Color ink = ThemedStrip.themed("Label.foreground", Color.BLACK);
        Color muted = ThemedStrip.themed("Console.LogViewer.mutedForeground", Color.GRAY);
        g.setFont(bold);
        g.setColor(ink);
        g.drawString(title, x + 32, y + 19);
        g.setFont(metrics.getFont());
        g.setColor(muted);
        g.drawString(range, x + 32 + g.getFontMetrics(bold).stringWidth(title) + 10, y + 19);
        cardLines.add(title + " " + range);
        for (int i = 0; i < rows.length; i++) {
            int baseline = y + 46 + i * lineHeight;
            int column = x + 14;
            g.setFont(metrics.getFont());
            g.setColor(muted);
            g.drawString(rows[i][0], column, baseline);
            column += labelWidth + 14;
            g.setFont(valueFont);
            g.setColor(ink);
            g.drawString(rows[i][1], column, baseline);
            column += valueWidth + 18;
            g.setFont(metrics.getFont());
            g.setColor(muted);
            g.drawString(rows[i][2], column, baseline);
            cardLines.add((rows[i][0] + " " + rows[i][1] + " " + rows[i][2]).trim());
        }
    }

    private void drawTimeAxis(Graphics2D g, FontMetrics metrics, PlotPalette palette, int axisTop, List<Double> sampleTimes) {
        double step = timeTickStep();
        g.setColor(palette.label);
        timeLabelBounds = new ArrayList<>();
        for (double time : timeTicks(step)) {
            String text = timeLabel(time, step);
            int x = xOf(time) - metrics.stringWidth(text) / 2;
            g.drawString(text, x, axisTop + 19);
            timeLabelBounds.add(new Rectangle(x, axisTop + 19 - metrics.getAscent(), metrics.stringWidth(text),
                    metrics.getHeight()));
        }
        String timeLabel = GAUGE_NAME_TIME + " (s)";
        if (grid.isVertical() && grid.getVerticalMode() == ChartSettings.VerticalMode.SAMPLES && sampleTimes == null) {
            timeLabel += "  ·  zoom in to see sample lines";
        }
        timeTitle = timeLabel;
    }

    /** The time axis title, from {@link #drawTimeAxis}, and where {@link #drawTimeTitle} put it. */
    private String timeTitle;
    private Rectangle timeTitleBounds;

    Rectangle timeTitleBounds() {
        return timeTitleBounds;
    }

    /**
     * The time axis title, centred under the plots; it steps aside when the cursor time badge, which grows with the
     * font, would cover it.
     */
    private void drawTimeTitle(Graphics2D g, FontMetrics metrics, PlotPalette palette, int axisTop) {
        int width = metrics.stringWidth(timeTitle);
        int baseline = axisTop + 37;
        Rectangle bounds = new Rectangle((plotLeft + plotRight - width) / 2, baseline - metrics.getAscent(), width,
                metrics.getHeight());
        if (timeBadge != null && bounds.intersects(new Rectangle(timeBadge.x - 8, timeBadge.y, timeBadge.width + 16,
                timeBadge.height))) {
            int right = timeBadge.x + timeBadge.width + 12;
            bounds.x = right + width <= plotRight ? right : timeBadge.x - 12 - width;
        }
        g.setColor(palette.label);
        g.drawString(timeTitle, bounds.x, baseline);
        timeTitleBounds = bounds;
    }

    /** The cursor time badge on the time axis from the last paint, or null. */
    private Rectangle timeBadge;
    private String timeBadgeText;

    Rectangle timeBadge() {
        return timeBadge;
    }

    String timeBadgeText() {
        return timeBadgeText;
    }

    /**
     * The cursor time in a dark badge on the time axis, right under the cursor line and joined to it, covering the
     * tick labels there; the decimals follow the zoom, as in the status bar.
     */
    private void drawTimeBadge(Graphics2D g, FontMetrics metrics, PlotPalette palette, int axisTop) {
        timeBadge = null;
        timeBadgeText = null;
        int x = cursorLineX();
        if (x < 0) {
            return;
        }
        String text = String.format(Locale.ROOT, (window.span() < 10 ? "%.3f" : "%.2f") + " s", cursorTime());
        Font font = g.getFont();
        g.setFont(font.deriveFont(Font.BOLD));
        FontMetrics bold = g.getFontMetrics();
        int width = bold.stringWidth(text) + 16;
        int height = bold.getHeight() + 4;
        int left = Math.max(plotLeft - LEFT / 2, Math.min(plotRight + RIGHT_MARGIN / 2 - width, x - width / 2));
        int top = axisTop + 5;
        // clear the tick labels behind it, with a little room so no half label shows at its sides
        g.setColor(palette.background);
        g.fillRect(left - 6, axisTop + 1, width + 12, height + 6);
        Color ink = palette.label;
        g.setColor(ink);
        g.drawLine(x, axisTop, x, top);
        g.fillRoundRect(left, top, width, height, 6, 6);
        g.setColor(palette.background);
        g.drawString(text, left + 8, top + (height + bold.getAscent() - bold.getDescent()) / 2);
        g.setFont(font);
        timeBadge = new Rectangle(left, top, width, height);
        timeBadgeText = text;
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

    /** Screen y of {@code value} of {@code field} inside {@code area}, on the field's scale ({@link #range}). */
    private double yOf(int field, double value, Rectangle area) {
        return yOf(value, range(field), area);
    }

    /** Screen y of {@code value} on a {min, max} scale inside {@code area}; a flat scale puts it in the middle. */
    private static double yOf(double value, double[] range, Rectangle area) {
        double span = range[1] - range[0];
        return area.y + (1 - (span == 0 ? 0.5 : (value - range[0]) / span)) * area.height;
    }

    /**
     * The {min, max} a field's trace is scaled to: its own range ({@link #ownRange}), or, while its plot shares
     * scales by unit (the default), the range of the signals with the same unit it is comparable with
     * ({@link #sharedClusters}), so they can be compared directly.
     */
    double[] range(int field) {
        int plot = layout.plotOf(field);
        String unit = log.unit(field);
        if (plot < 0 || unit.isEmpty() || !layout.isSameUnitShared(plot) || !sharesScale(field)) {
            return ownRange(field);
        }
        List<Integer> sameUnit = new ArrayList<>();
        for (int other : layout.fields(plot)) {
            if (unit.equals(log.unit(other)) && sharesScale(other)) {
                sameUnit.add(other);
            }
        }
        for (List<Integer> cluster : sharedClusters(sameUnit)) {
            if (cluster.contains(field)) {
                return union(cluster);
            }
        }
        return ownRange(field);
    }

    /** A shared scale must leave every signal on it at least this fraction of the plot height. */
    static final double MIN_SHARED_FRACTION = 0.25;

    /**
     * Splits signals with the same unit into groups that share a scale: a group keeps a signal only while that
     * signal's own span is at least {@link #MIN_SHARED_FRACTION} of the group's combined span, so a shared scale
     * never squashes a trace into a flat line (CLT and IAT share; a 0.04 V and a 0.8 V signal do not). Signals left
     * out form groups of their own the same way.
     */
    List<List<Integer>> sharedClusters(List<Integer> fields) {
        List<List<Integer>> clusters = new ArrayList<>();
        List<Integer> remaining = new ArrayList<>(fields);
        while (!remaining.isEmpty()) {
            List<Integer> cluster = new ArrayList<>(remaining);
            while (cluster.size() > 1) {
                double[] combined = union(cluster);
                double combinedSpan = combined[1] - combined[0];
                int worst = -1;
                double worstFraction = Double.POSITIVE_INFINITY;
                for (int field : cluster) {
                    double[] own = ownRange(field);
                    double fraction = combinedSpan > 0 ? (own[1] - own[0]) / combinedSpan : 1;
                    if (!(fraction >= worstFraction)) {
                        worst = field;
                        worstFraction = fraction;
                    }
                }
                if (worstFraction >= MIN_SHARED_FRACTION) {
                    break;
                }
                cluster.remove(Integer.valueOf(worst));
            }
            clusters.add(cluster);
            remaining.removeAll(cluster);
        }
        return clusters;
    }

    private double[] union(List<Integer> fields) {
        double[] range = {Double.NaN, Double.NaN};
        for (int field : fields) {
            double[] own = ownRange(field);
            range = new double[]{min(range[0], own[0]), max(range[1], own[1])};
        }
        return range;
    }

    /** Whether a field joins a shared scale by unit: not with a custom range, which the user fixed. */
    private boolean sharesScale(int field) {
        return signalScales.mode(log.name(field)) != SignalScales.Mode.CUSTOM;
    }

    /**
     * A field's range by itself, as the user chose ({@link SignalScales}): its whole-log minimum and maximum
     * (default), those of the visible time (empty there: the whole log), or a custom range.
     */
    private double[] ownRange(int field) {
        String label = log.name(field);
        switch (signalScales.mode(label)) {
            case CUSTOM:
                return signalScales.custom(label);
            case VISIBLE: {
                TraceStats visible = windowStats.get(field);
                if (!visible.isEmpty()) {
                    return new double[]{visible.min, visible.max};
                }
                break;
            }
            default:
                break;
        }
        return new double[]{log.minimum(field), log.maximum(field)};
    }

    /** Shares the user's signal scales, e.g. those of the viewer host, kept from one log to the next. */
    void setSignalScales(SignalScales signalScales) {
        this.signalScales = signalScales;
        repaint();
    }

    SignalScales signalScales() {
        return signalScales;
    }

    /** "Scale ▸" with Whole log, Visible time and Custom…, for a plotted signal's right-click menu. */
    JMenu scaleSubmenu(int field) {
        String label = log.name(field);
        SignalScales.Mode mode = signalScales.mode(label);
        JMenu scale = new JMenu("Scale");
        JRadioButtonMenuItem whole = new JRadioButtonMenuItem("Whole log", mode == SignalScales.Mode.WHOLE_LOG);
        whole.setToolTipText("From the lowest to the highest value in the log; stays put while zooming");
        whole.addActionListener(e -> {
            signalScales.setWholeLog(label);
            scalesChanged();
        });
        JRadioButtonMenuItem visible = new JRadioButtonMenuItem("Visible time", mode == SignalScales.Mode.VISIBLE);
        visible.setToolTipText("From the lowest to the highest value on screen; rescales while zooming and panning");
        visible.addActionListener(e -> {
            signalScales.setVisible(label);
            scalesChanged();
        });
        double[] custom = signalScales.custom(label);
        JRadioButtonMenuItem fixed = new JRadioButtonMenuItem(custom == null ? "Custom…"
                : String.format(Locale.ROOT, "Custom (%s–%s)…", ValueFormat.format(log, field, custom[0]),
                ValueFormat.format(log, field, custom[1])), mode == SignalScales.Mode.CUSTOM);
        fixed.setToolTipText("A fixed range you enter, e.g. lambda 0.7–1.3; kept for this signal in every log");
        fixed.addActionListener(e -> {
            double[] picked = customScalePrompt.apply(label, range(field));
            if (picked != null) {
                signalScales.setCustom(label, picked);
            }
            // the radio item toggled itself; rebuild from the stored choice either way
            scalesChanged();
        });
        scale.add(whole);
        scale.add(visible);
        scale.add(fixed);
        return scale;
    }

    private void scalesChanged() {
        repaint();
        fireViewChanged();
    }

    /** Asks for a custom {min, max}: a small dialog with both fields filled in from the current scale. */
    private double[] askCustomScale(String label, double[] current) {
        JTextField min = new JTextField(trim(current[0]), 8);
        JTextField max = new JTextField(trim(current[1]), 8);
        JPanel fields = new JPanel(new GridLayout(2, 2, 8, 6));
        fields.add(new JLabel("Bottom (min)"));
        fields.add(min);
        fields.add(new JLabel("Top (max)"));
        fields.add(max);
        if (JOptionPane.showConfirmDialog(this, fields, "Scale of " + label, JOptionPane.OK_CANCEL_OPTION,
                JOptionPane.PLAIN_MESSAGE) != JOptionPane.OK_OPTION) {
            return null;
        }
        try {
            double bottom = Double.parseDouble(min.getText().trim().replace(',', '.'));
            double top = Double.parseDouble(max.getText().trim().replace(',', '.'));
            if (Double.isFinite(bottom) && Double.isFinite(top) && bottom < top) {
                return new double[]{bottom, top};
            }
        } catch (NumberFormatException ignored) {
            // reported below
        }
        JOptionPane.showMessageDialog(this, "Enter two numbers, the bottom below the top.", "Scale of " + label,
                JOptionPane.WARNING_MESSAGE);
        return null;
    }

    /** A number without needless trailing zeros, for editing. */
    private static String trim(double value) {
        String text = String.format(Locale.ROOT, "%.6f", value);
        return text.contains(".") ? text.replaceAll("0+$", "").replaceAll("\\.$", "") : text;
    }

    /** The smaller value, ignoring a missing (NaN) one. */
    private static double min(double a, double b) {
        return Double.isNaN(a) ? b : Double.isNaN(b) ? a : Math.min(a, b);
    }

    private static double max(double a, double b) {
        return Double.isNaN(a) ? b : Double.isNaN(b) ? a : Math.max(a, b);
    }

    /** Signals of a unit share an axis side; signals without a unit each count as a unit of their own. */
    private String unitKey(int field) {
        String unit = log.unit(field);
        return unit.isEmpty() ? "#" + field : unit;
    }

    /** The highlighted trace (hovered or pinned in the legend, the Stats panel or by keys) if it is in the plot. */
    private int highlightedIn(Band band) {
        int focused = focus.focused();
        return focused != TraceFocus.NONE && layout.plotOf(focused) == band.plot ? focused : -1;
    }

    /** The units (unit keys) of a plot's signals, in plot order. */
    List<String> unitsIn(int plot) {
        Set<String> units = new LinkedHashSet<>();
        for (int field : layout.fields(plot)) {
            units.add(unitKey(field));
        }
        return new ArrayList<>(units);
    }

    /** How a unit key reads in menus: the unit, or the signal's name for a signal without one. */
    String unitLabel(String key) {
        return key.startsWith("#") ? log.name(Integer.parseInt(key.substring(1))) : key;
    }

    /**
     * The unit on a plot's left axis while the plot shares scales by unit: the one chosen in the plot menu when it
     * is plotted, else the first unit (skipping one chosen for the right); null with sharing off or no signals.
     */
    String leftAxisUnit(int plot) {
        List<String> units = unitsIn(plot);
        if (!layout.isSameUnitShared(plot) || units.isEmpty()) {
            return null;
        }
        String chosen = layout.leftAxisUnit(plot);
        if (chosen != null && units.contains(chosen)) {
            return chosen;
        }
        String right = layout.rightAxisUnit(plot);
        for (String unit : units) {
            if (!unit.equals(right)) {
                return unit;
            }
        }
        return null;
    }

    /**
     * The unit on a plot's right axis while the plot shares scales by unit: the one chosen in the plot menu (or
     * none), else with two units the other one, with three or more the highlighted trace's unit when it is not
     * the left one, otherwise the second unit; null with sharing off, a single unit or None chosen.
     */
    String rightAxisUnit(Band band) {
        int plot = band.plot;
        String left = leftAxisUnit(plot);
        String chosen = layout.rightAxisUnit(plot);
        if (left == null || PlotLayout.NO_AXIS.equals(chosen)) {
            return null;
        }
        List<String> others = unitsIn(plot);
        others.remove(left);
        if (chosen != null && others.contains(chosen)) {
            return chosen;
        }
        if (others.isEmpty()) {
            return null;
        }
        int highlighted = highlightedIn(band);
        if (others.size() >= 2 && highlighted >= 0 && others.contains(unitKey(highlighted))) {
            return unitKey(highlighted);
        }
        return others.get(0);
    }

    /** The signal whose scale shows for a unit: the highlighted trace when it has that unit, else the first. */
    private int fieldFor(Band band, String unit) {
        int highlighted = highlightedIn(band);
        if (highlighted >= 0 && unitKey(highlighted).equals(unit)) {
            return highlighted;
        }
        for (int field : layout.fields(band.plot)) {
            if (unitKey(field).equals(unit)) {
                return field;
            }
        }
        return -1;
    }

    /**
     * The signal whose scale the plot's left axis shows: sharing scales by unit, a signal of the left unit
     * ({@link #leftAxisUnit}); with sharing off, every signal has its own scale, so only the highlighted trace's
     * scale is shown, always on the left; -1 for none.
     */
    int leftAxisField(Band band) {
        if (!layout.isSameUnitShared(band.plot)) {
            return highlightedIn(band);
        }
        String unit = leftAxisUnit(band.plot);
        return unit == null ? -1 : fieldFor(band, unit);
    }

    /** The signal whose scale the plot's right axis shows ({@link #rightAxisUnit}), or -1. */
    int rightAxisField(Band band) {
        String unit = rightAxisUnit(band);
        return unit == null ? -1 : fieldFor(band, unit);
    }

    /**
     * The title above an axis: the unit, named after the signal ("VBatt · V") when signals of that unit in the
     * plot are on different scales, so one signal's numbers are not read against another trace.
     */
    String axisTitle(Band band, int field) {
        String unit = log.unit(field);
        if (unit.isEmpty()) {
            return log.name(field);
        }
        double[] scale = range(field);
        for (int other : layout.fields(band.plot)) {
            if (other != field && unit.equals(log.unit(other)) && !Arrays.equals(range(other), scale)) {
                return log.name(field) + " · " + unit;
            }
        }
        return unit;
    }

    /** Over a y axis: its whole title and which scale it is (the axis title may be shortened); else none. */
    @Override
    public String getToolTipText(MouseEvent event) {
        int x = event.getX();
        int y = event.getY();
        for (Band band : bands) {
            if (y < band.header.y || y > band.traces.y + band.traces.height) {
                continue;
            }
            int field = x < plotLeft - 4 ? leftAxisField(band) : x > plotRight + 4 ? rightAxisField(band) : -1;
            if (field >= 0) {
                return "Scale of " + log.name(field) + " (" + axisTitle(band, field) + ")";
            }
        }
        return null;
    }

    /** True when some plot can show a right axis, so the chart keeps room for its labels. */
    private boolean anyRightAxis() {
        for (int plot = 0; plot < layout.plotCount(); plot++) {
            if (layout.isSameUnitShared(plot) && !PlotLayout.NO_AXIS.equals(layout.rightAxisUnit(plot))
                    && unitsIn(plot).size() >= 2) {
                return true;
            }
        }
        return false;
    }

    /** {value, screen y} of the left axis ticks, which the horizontal grid lines follow. */
    List<double[]> axisTicks(Band band) {
        return ticks(band, leftAxisField(band));
    }

    List<double[]> rightAxisTicks(Band band) {
        return ticks(band, rightAxisField(band));
    }

    /**
     * {value, screen y} of the ticks of a signal's scale in a plot: round values about
     * {@code grid.getHorizontalDivisions()} apart; none for no signal or a signal without values.
     */
    private List<double[]> ticks(Band band, int field) {
        List<double[]> ticks = new ArrayList<>();
        if (field < 0) {
            return ticks;
        }
        double[] range = range(field);
        if (!Double.isFinite(range[0]) || !Double.isFinite(range[1])) {
            return ticks;
        }
        if (range[1] == range[0]) {
            ticks.add(new double[]{range[0], yOf(range[0], range, band.traces)});
            return ticks;
        }
        // the configured divisions, fewer in a short plot so labels stay apart
        int intervals = Math.max(1, Math.min(grid.getHorizontalDivisions(), band.traces.height / MIN_TICK_PIXELS));
        double step = nearestNiceStep((range[1] - range[0]) / intervals);
        for (double value = Math.ceil(range[0] / step) * step; value <= range[1] + step * 1e-9; value += step) {
            // round away binary noise such as 0.30000000000000004
            double tick = Math.round(value / step) * step;
            ticks.add(new double[]{tick, yOf(tick, range, band.traces)});
        }
        return ticks;
    }

    /**
     * One y axis of a plot: tick values, then the unit above them, in the colour of the trace they belong to;
     * on the left, right-aligned against the plot, or on the right, left-aligned after it.
     */
    private void drawYAxis(Graphics2D g, Band band, int field, List<double[]> ticks, boolean left,
                           PlotPalette palette) {
        if (field < 0 || ticks.isEmpty()) {
            return;
        }
        Font font = g.getFont();
        g.setFont(font.deriveFont(font.getSize2D() - 1));
        FontMetrics small = g.getFontMetrics();
        double step = ticks.size() > 1 ? ticks.get(1)[0] - ticks.get(0)[0] : 0;
        int decimals = step > 0 ? Math.max(0, (int) -Math.floor(Math.log10(step) + 1e-9)) : log.digits(field);
        int edge = left ? plotLeft - AXIS_LABEL_GAP : plotRight + RIGHT_AXIS_GAP;
        g.setColor(colorOf(field, palette));
        for (double[] tick : ticks) {
            String text = String.format(Locale.ROOT, "%." + decimals + "f", tick[0]);
            int y = (int) Math.round(tick[1]) + (small.getAscent() - small.getDescent()) / 2;
            // keep the top and bottom labels inside the plot
            y = Math.max(band.traces.y + small.getAscent() - small.getDescent(), Math.min(band.traces.y + band.traces.height, y));
            g.drawString(text, left ? edge - small.stringWidth(text) : edge, y);
        }
        String title = axisTitle(band, field);
        int room = left ? edge - 2 : getWidth() - edge - 2;
        String unit = log.unit(field);
        String shown;
        if (title.endsWith(" · " + unit) && !unit.isEmpty()) {
            // shorten the signal name, keep the unit
            String suffix = " · " + unit;
            shown = StatsPanel.SignalCell.shorten(title.substring(0, title.length() - suffix.length()), small,
                    Math.max(small.stringWidth("W…"), room - small.stringWidth(suffix))) + suffix;
        } else {
            shown = StatsPanel.SignalCell.shorten(title, small, room);
        }
        g.drawString(shown, left ? Math.max(2, edge - small.stringWidth(shown)) : edge, band.traces.y - 6);
        g.setFont(font);
    }

    /** Seconds between time-axis ticks, chosen so ticks are roughly the configured spacing apart. */
    /** Least space between two time axis labels, in pixels. */
    static final int TIME_LABEL_GAP = 12;
    /** Font metrics of the time axis labels from the last paint, or null before it. */
    private FontMetrics axisMetrics;
    /** The time axis labels' bounds from the last paint. */
    private List<Rectangle> timeLabelBounds = new ArrayList<>();

    /**
     * About one tick per {@link ChartSettings#getTimeSpacing()} pixels, or fewer when the labels would not fit
     * between them: the widest label plus {@link #TIME_LABEL_GAP}.
     */
    double timeTickStep() {
        double width = plotRight - plotLeft;
        double step = niceStep(window.span() / Math.max(2, width / grid.getTimeSpacing()));
        if (axisMetrics == null) {
            return step;
        }
        for (int i = 0; i < 30 && widestTimeLabel(step) + TIME_LABEL_GAP > step * width / window.span(); i++) {
            step = niceStep(step * 1.01);
        }
        return step;
    }

    private int widestTimeLabel(double step) {
        int widest = 0;
        for (double time : timeTicks(step)) {
            widest = Math.max(widest, axisMetrics.stringWidth(timeLabel(time, step)));
        }
        return widest;
    }

    /** A tick's label: the time in seconds, with the decimals the step needs. */
    String timeLabel(double time, double step) {
        return String.format(Locale.ROOT, "%." + decimalsFor(step) + "f", time);
    }

    /** Decimals that show a multiple of {@code step} exactly, for steps of 1, 2 or 5 times a power of ten. */
    private static int decimalsFor(double step) {
        return Math.max(0, Math.min(6, (int) -Math.floor(Math.log10(step) + 1e-9)));
    }

    List<Rectangle> timeLabelBounds() {
        return timeLabelBounds;
    }

    private List<Double> timeTicks() {
        return timeTicks(timeTickStep());
    }

    private List<Double> timeTicks(double step) {
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
    /** Pixels between y axis labels at least. */
    private static final int MIN_TICK_PIXELS = 28;

    /** The 1, 2 or 5 × 10^n step nearest to {@code rough} (by ratio), so ticks are close to the asked count. */
    static double nearestNiceStep(double rough) {
        if (!(rough > 0) || !Double.isFinite(rough)) {
            return 1;
        }
        double magnitude = Math.pow(10, Math.floor(Math.log10(rough)));
        double best = magnitude;
        for (double nice : new double[]{1, 2, 5, 10}) {
            if (Math.abs(Math.log(nice * magnitude / rough)) < Math.abs(Math.log(best / rough))) {
                best = nice * magnitude;
            }
        }
        return best;
    }

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
        double[] range = range(field);
        ColumnPath column = new ColumnPath(path);
        for (int sample = first; sample <= last; sample++) {
            double value = log.value(sample, field);
            if (!Double.isFinite(value)) {
                column.breakLine();
                continue;
            }
            double x = plotLeft + (log.value(sample, 0) - window.start()) * scale;
            column.add(x, yOf(value, range, area));
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

    /**
     * The value at the cursor while the pointer is over the plots; otherwise the minimum and maximum of the
     * visible time window, so the legend summarises what is on screen.
     */
    String legendValue(int field, int sample) {
        String value = sample >= 0 ? ValueFormat.format(log, field, log.value(sample, field))
                : ValueFormat.range(log, field, stats.get(field));
        return cursors.isRange() ? value + "  Δ" + delta(field) : value;
    }

    /** B minus A of a signal, signed: "+820", "-0.12". */
    String delta(int field) {
        double a = log.value(log.nearestSample(cursors.time(MeasureCursors.Which.A)), field);
        double b = log.value(log.nearestSample(cursors.time(MeasureCursors.Which.B)), field);
        double difference = b - a;
        if (!Double.isFinite(difference)) {
            return ValueFormat.NONE;
        }
        return (difference >= 0 ? "+" : "") + ValueFormat.format(log, field, difference);
    }

    /** A cursor's value of a signal at its nearest sample. */
    double valueAt(MeasureCursors.Which which, int field) {
        return log.value(log.nearestSample(cursors.time(which)), field);
    }

    private double distanceTo(MeasureCursors.Which which, int x) {
        return cursors.has(which) ? Math.abs(xOf(cursors.time(which)) - x) : Double.POSITIVE_INFINITY;
    }

    /** "A 12.40 s   B 13.15 s   Δt 0.75 s", or the one cursor that is set. */
    String measureText() {
        StringBuilder text = new StringBuilder();
        for (MeasureCursors.Which which : MeasureCursors.Which.values()) {
            if (cursors.has(which)) {
                text.append(text.length() == 0 ? "" : "   ")
                        .append(String.format(Locale.ROOT, "%s %.2f s", which, cursors.time(which)));
            }
        }
        if (cursors.isRange()) {
            double span = cursors.end() - cursors.start();
            text.append(String.format(Locale.ROOT, span < 1 ? "   Δt %.3f s" : "   Δt %.2f s", span));
        }
        return text.toString();
    }

    private static final int FLAG_SIZE = 16;

    /**
     * The A and B cursors: dashed lines through every plot, the time between them tinted, a lettered flag on
     * each (dragged to move it) and a pill above the time axis with their times and the time between them.
     */
    private void drawMeasureCursors(Graphics2D g, FontMetrics metrics, PlotPalette palette, int axisTop) {
        if (cursors.isEmpty() || bands.isEmpty()) {
            return;
        }
        if (cursors.isRange()) {
            int from = Math.max(plotLeft, xOf(cursors.start()));
            int to = Math.min(plotRight, xOf(cursors.end()));
            if (to > from) {
                g.setColor(palette.selection);
                for (Band band : bands) {
                    g.fillRect(from, band.traces.y, to - from, band.traces.height);
                }
            }
        }
        Stroke previous = g.getStroke();
        Font font = g.getFont();
        g.setFont(font.deriveFont(Font.BOLD, font.getSize2D() - 1));
        FontMetrics flagMetrics = g.getFontMetrics();
        for (MeasureCursors.Which which : MeasureCursors.Which.values()) {
            Rectangle flag = cursorFlag(which);
            if (flag == null) {
                continue;
            }
            int x = flag.x + flag.width / 2;
            g.setColor(palette.accent);
            g.setStroke(new BasicStroke(1.2f, BasicStroke.CAP_BUTT, BasicStroke.JOIN_ROUND, 1, new float[]{5, 4}, 0));
            for (Band band : bands) {
                g.drawLine(x, band.traces.y, x, band.traces.y + band.traces.height);
            }
            g.setStroke(previous);
            g.fillRoundRect(flag.x, flag.y, flag.width, flag.height, 5, 5);
            g.setColor(palette.background);
            String letter = which.name();
            g.drawString(letter, x - flagMetrics.stringWidth(letter) / 2,
                    flag.y + (flag.height + flagMetrics.getAscent() - flagMetrics.getDescent()) / 2);
        }
        g.setFont(font);

        String text = measureText();
        int width = metrics.stringWidth(text) + 20;
        int height = metrics.getHeight() + 6;
        double middle = cursors.isRange() ? (cursors.start() + cursors.end()) / 2
                : cursors.time(cursors.has(MeasureCursors.Which.A) ? MeasureCursors.Which.A : MeasureCursors.Which.B);
        int x = Math.max(plotLeft, Math.min(plotRight - width, xOf(middle) - width / 2));
        int y = axisTop - height - 4;
        Color background = ThemedStrip.themed("Console.Card.background", Color.WHITE);
        g.setColor(new Color(background.getRed(), background.getGreen(), background.getBlue(), 235));
        g.fillRoundRect(x, y, width, height, height, height);
        g.setColor(palette.accent);
        g.drawRoundRect(x, y, width, height, height, height);
        g.setColor(ThemedStrip.themed("Label.foreground", Color.BLACK));
        g.drawString(text, x + 10, y + (height + metrics.getAscent() - metrics.getDescent()) / 2);
    }
}
