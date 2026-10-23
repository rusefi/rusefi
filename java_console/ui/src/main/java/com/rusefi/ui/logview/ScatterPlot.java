package com.rusefi.ui.logview;

import com.rusefi.core.preferences.storage.Node;

import javax.swing.AbstractAction;
import javax.swing.JComponent;
import javax.swing.JPopupMenu;
import javax.swing.KeyStroke;
import javax.swing.SwingUtilities;
import javax.swing.ToolTipManager;
import java.awt.BasicStroke;
import java.awt.Color;
import java.awt.Cursor;
import java.awt.Dimension;
import java.awt.Font;
import java.awt.FontMetrics;
import java.awt.Graphics;
import java.awt.Graphics2D;
import java.awt.Point;
import java.awt.Rectangle;
import java.awt.RenderingHints;
import java.awt.Shape;
import java.awt.event.ActionEvent;
import java.awt.event.KeyEvent;
import java.awt.event.MouseAdapter;
import java.awt.event.MouseEvent;
import java.awt.event.MouseWheelEvent;
import java.awt.geom.AffineTransform;
import java.awt.image.BufferedImage;
import java.awt.image.DataBufferInt;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Collections;
import java.util.List;
import java.util.Locale;
import java.util.function.Consumer;

import static com.rusefi.ui.logview.ThemedStrip.themed;

/**
 * Every shown record as a point at its X and Y, coloured by how much time the engine spent there (Density) or by
 * its Z value. Points are drawn once into an image, redrawn only when the data, colours, zoom or size change, so a
 * log of hundreds of thousands of records stays responsive. Coloured by Z, higher values are drawn last (on top),
 * so rare events such as knock are not buried, and points below a fade level are drawn pale first.
 * <p>
 * Mouse: hover a point for its values, click to pick it, drag a box to select points, double-click a point to show
 * it on the timeline (an empty spot: whole plot), wheel to zoom (Shift: X only, Ctrl: Y only), right-drag to pan,
 * right-click for the menu, a click on the legend for the colours. Keys: Enter shows the selection on the timeline,
 * Esc clears it, 0 the whole plot. [tag:console_theme]
 */
final class ScatterPlot extends JComponent implements ColourScale {
    enum ColourBy {
        DENSITY("Density", "How much time the engine spent at each spot"),
        Z("Z signal", "Each point by its Z value; higher values on top"),
        TIME("Time", "Each point by when it was logged, later on top: drift over the session, e.g. heat soak");

        final String text;
        final String tooltip;

        ColourBy(String text, String tooltip) {
            this.text = text;
            this.tooltip = tooltip;
        }
    }

    /** A guide line: X = Y (e.g. measured against target), or a level on one axis (e.g. 85 % duty). */
    static final class Reference {
        enum Kind {
            DIAGONAL, HORIZONTAL, VERTICAL
        }

        final Kind kind;
        final double value;
        final String label;

        Reference(Kind kind, double value, String label) {
            this.kind = kind;
            this.value = value;
            this.label = label;
        }
    }

    private static final int PAD = 12;
    private static final int LEGEND_HEIGHT = 30;
    private static final int LEGEND_BAR_WIDTH = 160;
    /** Density is gathered over squares this many pixels wide. */
    private static final int DENSITY_SPOT = 2;
    /** Fewer points than this get larger density spots. */
    private static final int SPARSE_POINTS = 100_000;
    private static final int POINT = 3;
    /** Hover and click find points within this many pixels. */
    private static final int PICK_RADIUS = 8;
    private static final int BUCKET = 8;
    private static final double WHEEL_ZOOM = 1.15;

    private ScatterData data;
    private ColourBy colourBy = ColourBy.DENSITY;
    private HistogramColors colors = new HistogramColors(new Node());
    private HistogramTable.Mixture mixture = HistogramTable.Mixture.NONE;
    private boolean dark;
    /** Points with a lower Z (logged unit) are drawn pale, under the others; NaN: none. */
    private double fadeBelow = Double.NaN;
    private List<Reference> references = new ArrayList<>();

    /** The visible X and Y ranges. */
    private double xFrom;
    private double xTo;
    private double yFrom;
    private double yTo;
    private boolean zoomed;

    private BufferedImage image;
    private boolean dirty = true;
    private int[] bucketHead = new int[0];
    private int[] bucketNext = new int[0];
    private int bucketColumns;
    private int bucketRows;
    private HistogramColors.Scale scale = new HistogramColors.Scale(0, 1, Double.NaN, false, false);

    /** The selected box in X and Y values ({x1, x2, y1, y2}), or null; and the points in it. */
    private double[] box;
    private List<Integer> selected = Collections.emptyList();
    /** The clicked point, or -1. */
    private int picked = -1;
    private int hovered = -1;
    private Point dragFrom;
    private Point dragTo;
    private Point panFrom;
    private double[] panView;
    private boolean panned;
    private Rectangle legendBounds = new Rectangle();

    private Runnable selectionListener = () -> {
    };
    private Runnable hoverListener = () -> {
    };
    private Runnable activateListener = () -> {
    };
    private Consumer<Rectangle> legendListener = bounds -> {
    };
    private JPopupMenu menu;

    ScatterPlot() {
        setOpaque(true);
        setFocusable(true);
        ToolTipManager.sharedInstance().registerComponent(this);
        MouseAdapter mouse = new Mouse();
        addMouseListener(mouse);
        addMouseMotionListener(mouse);
        addMouseWheelListener(mouse);
        bind(KeyEvent.VK_ENTER, "activate", () -> activateListener.run());
        bind(KeyEvent.VK_ESCAPE, "clearSelection", this::clearSelection);
        bind(KeyEvent.VK_0, "resetZoom", this::resetZoom);
        bind(KeyEvent.VK_NUMPAD0, "resetZoomPad", this::resetZoom);
    }

    private void bind(int key, String name, Runnable action) {
        getInputMap(WHEN_FOCUSED).put(KeyStroke.getKeyStroke(key, 0), name);
        getActionMap().put(name, new AbstractAction() {
            @Override
            public void actionPerformed(ActionEvent event) {
                if (data != null) {
                    action.run();
                }
            }
        });
    }

    /**
     * Shows new points or colours. The zoom stays while X and Y stay the same signals; the selection is cleared,
     * since the points behind it changed.
     */
    void set(ScatterData data, ColourBy colourBy, HistogramColors colors, HistogramTable.Mixture mixture,
             double fadeBelow, List<Reference> references) {
        boolean sameAxes = this.data != null && this.data.xField == data.xField && this.data.yField == data.yField;
        this.data = data;
        this.colourBy = colourBy;
        this.colors = colors;
        this.mixture = mixture;
        this.fadeBelow = fadeBelow;
        this.references = new ArrayList<>(references);
        if (!sameAxes || !zoomed) {
            fitView();
        }
        box = null;
        selected = Collections.emptyList();
        picked = -1;
        hovered = -1;
        updateZScale();
        dirty = true;
        repaint();
        selectionListener.run();
    }

    void setDark(boolean dark) {
        if (this.dark != dark) {
            this.dark = dark;
            dirty = true;
            repaint();
        }
    }

    ScatterData data() {
        return data;
    }

    ColourBy colourBy() {
        return colourBy;
    }

    void setSelectionListener(Runnable selectionListener) {
        this.selectionListener = selectionListener;
    }

    void setHoverListener(Runnable hoverListener) {
        this.hoverListener = hoverListener;
    }

    void setActivateListener(Runnable activateListener) {
        this.activateListener = activateListener;
    }

    void setLegendListener(Consumer<Rectangle> legendListener) {
        this.legendListener = legendListener;
    }

    void setMenu(JPopupMenu menu) {
        this.menu = menu;
    }

    // ---- view ----

    /** The whole data, with a little room around it. */
    private void fitView() {
        double xPad = Math.max((data.xMax - data.xMin) * 0.03, 1e-6);
        double yPad = Math.max((data.yMax - data.yMin) * 0.03, 1e-6);
        xFrom = data.xMin - xPad;
        xTo = data.xMax + xPad;
        yFrom = data.yMin - yPad;
        yTo = data.yMax + yPad;
        zoomed = false;
    }

    void resetZoom() {
        fitView();
        dirty = true;
        repaint();
    }

    boolean isZoomed() {
        return zoomed;
    }

    /** {x from, x to, y from, y to} on screen. */
    double[] view() {
        return new double[]{xFrom, xTo, yFrom, yTo};
    }

    void setView(double x1, double x2, double y1, double y2) {
        if (!(Math.abs(x2 - x1) > 1e-12) || !(Math.abs(y2 - y1) > 1e-12)) {
            return;
        }
        xFrom = Math.min(x1, x2);
        xTo = Math.max(x1, x2);
        yFrom = Math.min(y1, y2);
        yTo = Math.max(y1, y2);
        zoomed = true;
        dirty = true;
        repaint();
    }

    void zoomToSelection() {
        if (box != null) {
            setView(box[0], box[1], box[2], box[3]);
        }
    }

    // ---- selection ----

    /** Selects the points in a box of X and Y values. */
    void selectBox(double x1, double x2, double y1, double y2) {
        box = new double[]{Math.min(x1, x2), Math.max(x1, x2), Math.min(y1, y2), Math.max(y1, y2)};
        selected = data.inBox(x1, x2, y1, y2);
        picked = -1;
        repaint();
        selectionListener.run();
    }

    void pick(int point) {
        box = null;
        selected = Collections.emptyList();
        picked = point;
        repaint();
        selectionListener.run();
    }

    void clearSelection() {
        box = null;
        selected = Collections.emptyList();
        picked = -1;
        repaint();
        selectionListener.run();
    }

    double[] box() {
        return box == null ? null : box.clone();
    }

    List<Integer> selected() {
        return selected;
    }

    int picked() {
        return picked;
    }

    int hovered() {
        return hovered;
    }

    // ---- geometry ----

    /** Where the points go: room on the left for the Y labels and title, below for the X ones and the legend. */
    Rectangle plotArea() {
        FontMetrics metrics = getFontMetrics(labelFont());
        int yLabels = metrics.stringWidth("-00000.0");
        int left = PAD + metrics.getHeight() + 4 + yLabels + 6;
        int bottom = metrics.getHeight() * 2 + 12 + LEGEND_HEIGHT + PAD;
        return new Rectangle(left, PAD, Math.max(1, getWidth() - left - PAD - 4),
                Math.max(1, getHeight() - PAD - bottom));
    }

    private Font labelFont() {
        return getFont().deriveFont(getFont().getSize2D() - 1);
    }

    double xValue(int px, Rectangle area) {
        return xFrom + (double) (px - area.x) / area.width * (xTo - xFrom);
    }

    double yValue(int py, Rectangle area) {
        return yTo - (double) (py - area.y) / area.height * (yTo - yFrom);
    }

    private int xPixel(double x, Rectangle area) {
        return area.x + (int) Math.floor((x - xFrom) / (xTo - xFrom) * area.width);
    }

    private int yPixel(double y, Rectangle area) {
        return area.y + area.height - 1 - (int) Math.floor((y - yFrom) / (yTo - yFrom) * area.height);
    }

    /** The point nearest to a spot on screen, within a few pixels, or -1. */
    int pointAt(Point point) {
        if (data == null || bucketHead.length == 0) {
            return -1;
        }
        Rectangle area = plotArea();
        int bx = (point.x - area.x) / BUCKET;
        int by = (point.y - area.y) / BUCKET;
        int best = -1;
        double bestDistance = PICK_RADIUS * PICK_RADIUS;
        for (int dy = -1; dy <= 1; dy++) {
            for (int dx = -1; dx <= 1; dx++) {
                int cx = bx + dx;
                int cy = by + dy;
                if (cx < 0 || cy < 0 || cx >= bucketColumns || cy >= bucketRows) {
                    continue;
                }
                for (int i = bucketHead[cy * bucketColumns + cx]; i >= 0; i = bucketNext[i]) {
                    double ddx = xPixel(data.x[i], area) - point.x;
                    double ddy = yPixel(data.y[i], area) - point.y;
                    double distance = ddx * ddx + ddy * ddy;
                    // buckets list the last drawn (topmost) point first: a tie keeps it
                    if (distance < bestDistance || best < 0 && distance <= bestDistance) {
                        bestDistance = distance;
                        best = i;
                    }
                }
            }
        }
        return best;
    }

    // ---- colours ----

    private void updateZScale() {
        if (colourBy == ColourBy.TIME) {
            // from the first point shown to the last
            double first = data.count > 0 ? data.time(0) : 0;
            double last = data.count > 0 ? data.time(data.count - 1) : 1;
            scale = new HistogramColors.Scale(first, last, Double.NaN, false, false);
            return;
        }
        if (colourBy != ColourBy.Z) {
            return;
        }
        // the scale needs the spread, not every point: an even sample keeps it quick
        int step = Math.max(1, data.count / 50_000);
        double[] values = new double[data.count / step + 1];
        int n = 0;
        for (int i = 0; i < data.count; i += step) {
            // pale points (under the fade level) are not on the scale: it spreads over the ones in colour
            if (!Float.isNaN(data.z[i]) && !(data.z[i] < fadeBelow)) {
                values[n++] = data.z[i] * mixture.factor;
            }
        }
        scale = colors.scale(values, n, true, mixture.factor, mixture.stoich);
    }

    /** Where a value falls on the palette, 0..1, after Reverse; for density the value is seconds per spot. */
    double position(double value) {
        double t = scale.high > scale.low ? Math.max(0, Math.min(1, (value - scale.low) / (scale.high - scale.low))) : 0.5;
        if (colourBy == ColourBy.DENSITY) {
            // a few busy spots (idle) dwarf the rest: a square root keeps the quieter ones visible
            t = Math.sqrt(t);
        }
        return colors.reversed() ? 1 - t : t;
    }

    @Override
    public boolean showsValues() {
        return colourBy == ColourBy.Z;
    }

    @Override
    public HistogramTable.Mixture mixture() {
        return mixture;
    }

    @Override
    public String zUnit() {
        return mixture != HistogramTable.Mixture.NONE ? mixture.unit : data.log.unit(data.zField);
    }

    @Override
    public double scaleLow() {
        return scale.low;
    }

    @Override
    public double scaleHigh() {
        return scale.high;
    }

    @Override
    public double scaleCentre() {
        return scale.centre;
    }

    // ---- drawing ----

    /** Draws the points into the image (and the hover index) for the current data, colours, zoom and size. */
    private void render(Rectangle area, PlotPalette palette) {
        int w = area.width;
        int h = area.height;
        image = new BufferedImage(w, h, BufferedImage.TYPE_INT_ARGB);
        int[] pixels = ((DataBufferInt) image.getRaster().getDataBuffer()).getData();
        bucketColumns = w / BUCKET + 1;
        bucketRows = h / BUCKET + 1;
        bucketHead = new int[bucketColumns * bucketRows];
        Arrays.fill(bucketHead, -1);
        bucketNext = new int[data.count];
        double sx = w / (xTo - xFrom);
        double sy = h / (yTo - yFrom);
        int[] px = new int[data.count];
        int[] py = new int[data.count];
        for (int i = 0; i < data.count; i++) {
            px[i] = (int) Math.floor((data.x[i] - xFrom) * sx);
            py[i] = h - 1 - (int) Math.floor((data.y[i] - yFrom) * sy);
            if (px[i] < 0 || py[i] < 0 || px[i] >= w || py[i] >= h) {
                // off screen: not drawn, not pickable
                px[i] = -1;
            }
        }
        if (colourBy == ColourBy.DENSITY) {
            renderDensity(pixels, w, h, px, py);
        } else {
            renderByZ(pixels, w, h, px, py, palette);
        }
    }

    private void addToIndex(int point, int x, int y) {
        int bucket = (y / BUCKET) * bucketColumns + x / BUCKET;
        bucketNext[point] = bucketHead[bucket];
        bucketHead[bucket] = point;
    }

    /** Spots of 2 px, or 3 px for a plot of few points, so that sparse points (outside idle) stay visible. */
    private int densitySpot() {
        return data.count < SPARSE_POINTS ? DENSITY_SPOT + 1 : DENSITY_SPOT;
    }

    private void renderDensity(int[] pixels, int w, int h, int[] px, int[] py) {
        int spotSize = densitySpot();
        int sw = (w + spotSize - 1) / spotSize;
        int sh = (h + spotSize - 1) / spotSize;
        float[] seconds = new float[sw * sh];
        for (int i = 0; i < data.count; i++) {
            if (px[i] >= 0) {
                seconds[(py[i] / spotSize) * sw + px[i] / spotSize] += Math.max(data.weight[i], 1e-6f);
                addToIndex(i, px[i], py[i]);
            }
        }
        double[] busy = new double[seconds.length];
        int n = 0;
        for (float s : seconds) {
            if (s > 0) {
                busy[n++] = s;
            }
        }
        scale = colors.scale(busy, n, false, 1, Double.NaN);
        for (int spot = 0; spot < seconds.length; spot++) {
            if (seconds[spot] <= 0) {
                continue;
            }
            // the faintest spots stay visible on a light background
            double t = position(seconds[spot]);
            t = colors.reversed() ? Math.min(0.88, t) : Math.max(0.12, t);
            int rgb = 0xFF000000 | HistogramColors.color(colors.palette(), t).getRGB();
            int sx = (spot % sw) * spotSize;
            int sy = (spot / sw) * spotSize;
            for (int dy = 0; dy < spotSize && sy + dy < h; dy++) {
                for (int dx = 0; dx < spotSize && sx + dx < w; dx++) {
                    pixels[(sy + dy) * w + sx + dx] = rgb;
                }
            }
        }
    }

    private void renderByZ(int[] pixels, int w, int h, int[] px, int[] py, PlotPalette palette) {
        boolean byTime = colourBy == ColourBy.TIME;
        // by Z: lowest first, so the highest end up on top, points without Z first of all; by time: in time order
        long[] order = new long[data.count];
        for (int i = 0; i < data.count; i++) {
            float z = data.z[i];
            int key = byTime ? 0 : Float.isNaN(z) ? Integer.MIN_VALUE : sortable(z);
            order[i] = ((long) key << 32) | (i & 0xFFFFFFFFL);
        }
        if (!byTime) {
            Arrays.sort(order);
        }
        // pale points: light and small, so the ones in colour stand out
        Color muted = HistogramTable.mix(palette.background, palette.hint, 0.28);
        int mutedRgb = 0xFF000000 | muted.getRGB();
        for (long entry : order) {
            int i = (int) entry;
            if (px[i] < 0) {
                continue;
            }
            float z = data.z[i];
            int rgb;
            int size = POINT;
            if (byTime) {
                rgb = 0xFF000000 | HistogramColors.color(colors.palette(), position(data.time(i))).getRGB();
            } else if (Float.isNaN(z) || z < fadeBelow) {
                rgb = mutedRgb;
                size = POINT - 1;
            } else {
                rgb = 0xFF000000 | HistogramColors.color(colors.palette(), position(z * mixture.factor)).getRGB();
            }
            for (int dy = -1; dy < size - 1; dy++) {
                int y = py[i] + dy;
                if (y < 0 || y >= h) {
                    continue;
                }
                for (int dx = -1; dx < size - 1; dx++) {
                    int x = px[i] + dx;
                    if (x >= 0 && x < w) {
                        pixels[y * w + x] = rgb;
                    }
                }
            }
            addToIndex(i, px[i], py[i]);
        }
    }

    /** A float as an int that sorts the same way. */
    private static int sortable(float value) {
        int bits = Float.floatToIntBits(value);
        return bits >= 0 ? bits : bits ^ 0x7FFFFFFF;
    }

    @Override
    public Dimension getPreferredSize() {
        return new Dimension(800, 520);
    }

    @Override
    protected void paintComponent(Graphics graphics) {
        Graphics2D g = (Graphics2D) graphics.create();
        try {
            g.setRenderingHint(RenderingHints.KEY_TEXT_ANTIALIASING, RenderingHints.VALUE_TEXT_ANTIALIAS_ON);
            PlotPalette palette = PlotPalette.of(dark);
            g.setColor(themed("Console.LogViewer.sidebar.background", Color.WHITE));
            g.fillRect(0, 0, getWidth(), getHeight());
            legendBounds = new Rectangle();
            if (data == null) {
                return;
            }
            Rectangle area = plotArea();
            g.setColor(palette.background);
            g.fillRect(area.x, area.y, area.width, area.height);
            if (data.count == 0) {
                g.setColor(palette.hint);
                String message = "No records to show: check the filters, the time range or the signals";
                g.drawString(message, area.x + (area.width - g.getFontMetrics().stringWidth(message)) / 2,
                        area.y + area.height / 2);
                paintAxes(g, area, palette);
                return;
            }
            if (dirty || image == null || image.getWidth() != area.width || image.getHeight() != area.height) {
                render(area, palette);
                dirty = false;
            }
            paintGrid(g, area, palette);
            g.drawImage(image, area.x, area.y, null);
            g.setRenderingHint(RenderingHints.KEY_ANTIALIASING, RenderingHints.VALUE_ANTIALIAS_ON);
            paintReferences(g, area, palette);
            paintSelection(g, area, palette);
            paintAxes(g, area, palette);
            paintLegend(g, area);
        } finally {
            g.dispose();
        }
    }

    /** Tick values for a range drawn over {@code pixels}: about one every 90 px. */
    static double[] ticks(double from, double to, int pixels) {
        double step = LogChart.niceStep((to - from) / Math.max(2, pixels / 90.0));
        List<Double> ticks = new ArrayList<>();
        for (double tick = Math.ceil(from / step) * step; tick <= to + step * 1e-9; tick += step) {
            ticks.add(Math.abs(tick) < step * 1e-9 ? 0 : tick);
        }
        double[] result = new double[ticks.size()];
        for (int i = 0; i < result.length; i++) {
            result[i] = ticks.get(i);
        }
        return result;
    }

    private static String tickLabel(double value, double[] ticks) {
        double step = ticks.length > 1 ? ticks[1] - ticks[0] : 1;
        int decimals = Math.max(0, Math.min(4, (int) Math.ceil(-Math.log10(step) - 1e-9)));
        return String.format(Locale.ROOT, "%." + decimals + "f", value);
    }

    private void paintGrid(Graphics2D g, Rectangle area, PlotPalette palette) {
        g.setColor(palette.grid);
        for (double x : ticks(xFrom, xTo, area.width)) {
            int px = xPixel(x, area);
            g.drawLine(px, area.y, px, area.y + area.height);
        }
        for (double y : ticks(yFrom, yTo, area.height)) {
            int py = yPixel(y, area);
            g.drawLine(area.x, py, area.x + area.width, py);
        }
    }

    private void paintReferences(Graphics2D g, Rectangle area, PlotPalette palette) {
        Shape clip = g.getClip();
        g.clipRect(area.x, area.y, area.width, area.height);
        g.setStroke(new BasicStroke(1.5f, BasicStroke.CAP_BUTT, BasicStroke.JOIN_MITER, 10, new float[]{6, 4}, 0));
        g.setFont(labelFont());
        for (Reference reference : references) {
            g.setColor(palette.label);
            int x1;
            int y1;
            int x2;
            int y2;
            switch (reference.kind) {
                case DIAGONAL: {
                    double from = Math.min(xFrom, yFrom);
                    double to = Math.max(xTo, yTo);
                    x1 = xPixel(from, area);
                    y1 = yPixel(from, area);
                    x2 = xPixel(to, area);
                    y2 = yPixel(to, area);
                    break;
                }
                case HORIZONTAL:
                    x1 = area.x;
                    x2 = area.x + area.width;
                    y1 = y2 = yPixel(reference.value, area);
                    break;
                default:
                    x1 = x2 = xPixel(reference.value, area);
                    y1 = area.y;
                    y2 = area.y + area.height;
                    break;
            }
            g.drawLine(x1, y1, x2, y2);
            int labelX = reference.kind == Reference.Kind.VERTICAL ? x1 + 6 : area.x + area.width - 6
                    - g.getFontMetrics().stringWidth(reference.label);
            int labelY = reference.kind == Reference.Kind.HORIZONTAL ? y1 - 5
                    : reference.kind == Reference.Kind.VERTICAL ? area.y + 14
                    : Math.max(area.y + 14, Math.min(area.y + area.height - 6, yPixel(xValue(labelX, area), area) - 6));
            g.drawString(reference.label, labelX, labelY);
        }
        g.setStroke(new BasicStroke(1f));
        g.setClip(clip);
    }

    private void paintSelection(Graphics2D g, Rectangle area, PlotPalette palette) {
        Shape clip = g.getClip();
        g.clipRect(area.x, area.y, area.width, area.height);
        Rectangle drawn = null;
        if (dragFrom != null && dragTo != null) {
            drawn = new Rectangle(Math.min(dragFrom.x, dragTo.x), Math.min(dragFrom.y, dragTo.y),
                    Math.abs(dragTo.x - dragFrom.x), Math.abs(dragTo.y - dragFrom.y));
        } else if (box != null) {
            int x1 = xPixel(box[0], area);
            int x2 = xPixel(box[1], area);
            int y1 = yPixel(box[3], area);
            int y2 = yPixel(box[2], area);
            drawn = new Rectangle(x1, y1, x2 - x1, y2 - y1);
        }
        if (drawn != null) {
            g.setColor(palette.selection);
            g.fillRect(drawn.x, drawn.y, drawn.width, drawn.height);
            g.setColor(palette.accent);
            g.setStroke(new BasicStroke(1.5f));
            g.drawRect(drawn.x, drawn.y, drawn.width, drawn.height);
        }
        for (int point : new int[]{picked, hovered}) {
            if (point >= 0) {
                int x = xPixel(data.x[point], area);
                int y = yPixel(data.y[point], area);
                g.setStroke(new BasicStroke(point == picked ? 2f : 1.5f));
                g.setColor(point == picked ? palette.label : palette.cursor);
                g.drawOval(x - 6, y - 6, 12, 12);
            }
        }
        g.setStroke(new BasicStroke(1f));
        g.setClip(clip);
    }

    private void paintAxes(Graphics2D g, Rectangle area, PlotPalette palette) {
        Color muted = themed("Console.LogViewer.mutedForeground", Color.GRAY);
        g.setColor(themed("Separator.foreground", Color.LIGHT_GRAY));
        g.drawRect(area.x, area.y, area.width - 1, area.height - 1);
        Font font = labelFont();
        g.setFont(font);
        FontMetrics metrics = g.getFontMetrics();
        g.setColor(muted);
        double[] xTicks = ticks(xFrom, xTo, area.width);
        int labelTop = area.y + area.height + 4 + metrics.getAscent();
        for (double x : xTicks) {
            String label = tickLabel(x, xTicks);
            int px = xPixel(x, area);
            g.drawString(label, px - metrics.stringWidth(label) / 2, labelTop);
        }
        double[] yTicks = ticks(yFrom, yTo, area.height);
        for (double y : yTicks) {
            String label = tickLabel(y, yTicks);
            int py = yPixel(y, area);
            g.drawString(label, area.x - 6 - metrics.stringWidth(label), py + metrics.getAscent() / 2 - 1);
        }
        g.setFont(font.deriveFont(Font.BOLD));
        String xTitle = title(data.xField);
        g.drawString(xTitle, area.x + (area.width - g.getFontMetrics().stringWidth(xTitle)) / 2,
                labelTop + metrics.getHeight() + 2);
        String yTitle = title(data.yField);
        AffineTransform transform = g.getTransform();
        g.translate(PAD + g.getFontMetrics().getAscent(), area.y + (area.height + g.getFontMetrics().stringWidth(yTitle)) / 2);
        g.rotate(-Math.PI / 2);
        g.drawString(yTitle, 0, 0);
        g.setTransform(transform);
    }

    private String title(int field) {
        String unit = data.log.unit(field);
        String name = data.log.name(field);
        return unit.isEmpty() || unit.equalsIgnoreCase(name) ? name : name + " · " + unit;
    }

    /** The low and high labels of the legend. */
    String[] legendLabels() {
        if (colourBy == ColourBy.DENSITY) {
            return new String[]{"Less time", "More time"};
        }
        if (colourBy == ColourBy.TIME) {
            return new String[]{String.format(Locale.ROOT, "%.1f s", scale.low),
                    String.format(Locale.ROOT, "%.1f s", scale.high)};
        }
        int digits = mixture == HistogramTable.Mixture.NONE ? data.log.digits(data.zField) : mixture.digits();
        String low = (scale.clippedLow ? "≤ " : "") + String.format(Locale.ROOT, "%." + digits + "f", scale.low);
        String high = (scale.clippedHigh ? "≥ " : "") + String.format(Locale.ROOT, "%." + digits + "f", scale.high);
        return new String[]{low, high};
    }

    private void paintLegend(Graphics2D g, Rectangle area) {
        Color muted = themed("Console.LogViewer.mutedForeground", Color.GRAY);
        g.setFont(labelFont());
        FontMetrics metrics = g.getFontMetrics();
        int baseline = getHeight() - PAD - (LEGEND_HEIGHT - metrics.getAscent()) / 2;
        int barTop = baseline - metrics.getAscent() + 1;
        int barHeight = Math.max(8, metrics.getAscent() - 2);
        int start = area.x;
        int x = start;
        String[] labels = legendLabels();
        String name = colourBy == ColourBy.DENSITY ? "Time spent" : colourBy == ColourBy.TIME ? "Logged at"
                : title(data.zField);
        g.setColor(muted);
        g.drawString(name, x, baseline);
        x += metrics.stringWidth(name) + 12;
        g.drawString(labels[0], x, baseline);
        x += metrics.stringWidth(labels[0]) + 8;
        for (int i = 0; i < LEGEND_BAR_WIDTH; i++) {
            double t = (i + 0.5) / LEGEND_BAR_WIDTH;
            g.setColor(HistogramColors.color(colors.palette(), colors.reversed() ? 1 - t : t));
            g.fillRect(x + i, barTop, 1, barHeight);
        }
        x += LEGEND_BAR_WIDTH + 8;
        g.setColor(muted);
        g.drawString(labels[1], x, baseline);
        x += metrics.stringWidth(labels[1]) + 10;
        String settings = colors.palette().text + "  ▾";
        g.setColor(themed("Console.LogViewer.histogram.accent", new Color(0x1d4ed8)));
        g.drawString(settings, x, baseline);
        x += metrics.stringWidth(settings);
        legendBounds = new Rectangle(start - 4, barTop - 6, x - start + 8, barHeight + 12);
        if (colourBy == ColourBy.Z && Double.isFinite(fadeBelow)) {
            g.setColor(muted);
            String fade = String.format(Locale.ROOT, "pale: under %s", HistogramFilter.number(fadeBelow * mixture.factor));
            g.drawString(fade, x + 24, baseline);
        }
    }

    Rectangle legendBounds() {
        return new Rectangle(legendBounds);
    }

    @Override
    public String getToolTipText(MouseEvent event) {
        if (legendBounds.contains(event.getPoint())) {
            return "Click to change the colours: palette, range and centre";
        }
        return null;
    }

    private final class Mouse extends MouseAdapter {
        @Override
        public void mousePressed(MouseEvent event) {
            requestFocusInWindow();
            if (data == null) {
                return;
            }
            if (SwingUtilities.isLeftMouseButton(event)) {
                if (legendBounds.contains(event.getPoint())) {
                    legendListener.accept(new Rectangle(legendBounds));
                    return;
                }
                if (plotArea().contains(event.getPoint())) {
                    dragFrom = event.getPoint();
                    dragTo = null;
                }
            } else {
                panFrom = event.getPoint();
                panView = view();
                panned = false;
            }
        }

        @Override
        public void mouseDragged(MouseEvent event) {
            if (dragFrom != null) {
                dragTo = event.getPoint();
                repaint();
            } else if (panFrom != null) {
                Rectangle area = plotArea();
                double dx = (double) (event.getX() - panFrom.x) / area.width * (panView[1] - panView[0]);
                double dy = (double) (event.getY() - panFrom.y) / area.height * (panView[3] - panView[2]);
                panned |= Math.abs(event.getX() - panFrom.x) + Math.abs(event.getY() - panFrom.y) > 3;
                setView(panView[0] - dx, panView[1] - dx, panView[2] + dy, panView[3] + dy);
            }
        }

        @Override
        public void mouseReleased(MouseEvent event) {
            if (dragFrom != null && SwingUtilities.isLeftMouseButton(event)) {
                Point from = dragFrom;
                Point to = dragTo;
                dragFrom = null;
                dragTo = null;
                Rectangle area = plotArea();
                if (to != null && Math.abs(to.x - from.x) + Math.abs(to.y - from.y) > 6) {
                    selectBox(xValue(from.x, area), xValue(to.x, area), yValue(from.y, area), yValue(to.y, area));
                } else if (event.getClickCount() == 2) {
                    int point = pointAt(event.getPoint());
                    if (point >= 0) {
                        pick(point);
                        activateListener.run();
                    } else {
                        resetZoom();
                    }
                } else {
                    int point = pointAt(event.getPoint());
                    if (point >= 0) {
                        pick(point);
                    } else {
                        clearSelection();
                    }
                }
                repaint();
            } else if (panFrom != null) {
                boolean wasPan = panned;
                panFrom = null;
                if (!wasPan && menu != null) {
                    // a right-click without moving: the menu, acting on the point under the pointer if nothing else
                    if (box == null && picked < 0) {
                        int point = pointAt(event.getPoint());
                        if (point >= 0) {
                            pick(point);
                        }
                    }
                    menu.show(ScatterPlot.this, event.getX(), event.getY());
                }
            }
        }

        @Override
        public void mouseMoved(MouseEvent event) {
            boolean overPlot = data != null && plotArea().contains(event.getPoint());
            setCursor(legendBounds.contains(event.getPoint()) ? Cursor.getPredefinedCursor(Cursor.HAND_CURSOR)
                    : overPlot ? Cursor.getPredefinedCursor(Cursor.CROSSHAIR_CURSOR) : Cursor.getDefaultCursor());
            int point = overPlot ? pointAt(event.getPoint()) : -1;
            if (point != hovered) {
                hovered = point;
                repaint();
                hoverListener.run();
            }
        }

        @Override
        public void mouseExited(MouseEvent event) {
            if (hovered >= 0) {
                hovered = -1;
                repaint();
                hoverListener.run();
            }
        }

        @Override
        public void mouseWheelMoved(MouseWheelEvent event) {
            if (data == null) {
                return;
            }
            Rectangle area = plotArea();
            double factor = Math.pow(WHEEL_ZOOM, event.getPreciseWheelRotation());
            double x = xValue(event.getX(), area);
            double y = yValue(event.getY(), area);
            boolean zoomX = !event.isControlDown();
            boolean zoomY = !event.isShiftDown();
            double x1 = zoomX ? x - (x - xFrom) * factor : xFrom;
            double x2 = zoomX ? x + (xTo - x) * factor : xTo;
            double y1 = zoomY ? y - (y - yFrom) * factor : yFrom;
            double y2 = zoomY ? y + (yTo - y) * factor : yTo;
            setView(x1, x2, y1, y2);
        }
    }
}
