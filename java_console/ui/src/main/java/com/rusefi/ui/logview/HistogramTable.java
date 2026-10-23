package com.rusefi.ui.logview;

import com.rusefi.core.preferences.storage.Node;
import com.rusefi.logview.CsvLogReader;

import javax.swing.AbstractAction;
import javax.swing.JComponent;
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
import java.awt.HeadlessException;
import java.awt.Point;
import java.awt.Rectangle;
import java.awt.RenderingHints;
import java.awt.Shape;
import java.awt.Toolkit;
import java.awt.datatransfer.StringSelection;
import java.awt.event.ActionEvent;
import java.awt.event.InputEvent;
import java.awt.event.KeyEvent;
import java.awt.event.MouseAdapter;
import java.awt.event.MouseEvent;
import java.awt.geom.AffineTransform;
import java.util.ArrayList;
import java.util.List;
import java.util.Locale;
import java.util.function.Consumer;

import static com.rusefi.ui.logview.ThemedStrip.themed;

/**
 * The histogram as a coloured table: X bins left to right, Y bins with the highest at the top, like a tune table.
 * Colours follow the Z signal's {@link HistogramColors}: a palette over a range of values, by default the heatmap
 * over the cells without outliers; the diverging palette centres on stoichiometric for lambda or AFR. Time and
 * Count go from nothing to the busiest cells, on a square root so quieter cells stay visible. Cells with less time
 * than the thin-data limit are hatched, or left empty when thin cells are hidden. A click selects a cell; a click
 * on the legend opens the colour settings. [tag:console_theme]
 */
final class HistogramTable extends JComponent implements ColourScale {
    private static final int PAD = 12;
    private static final int GAP = 2;
    private static final int MIN_CELL_WIDTH = 30;
    private static final int MIN_CELL_HEIGHT = 18;
    private static final int LEGEND_HEIGHT = 30;
    private static final int LEGEND_BAR_WIDTH = 160;

    private Histogram histogram;
    private HistogramSettings.Mode mode = HistogramSettings.Mode.MEAN;
    private Mixture mixture = Mixture.NONE;
    private double minTime = HistogramSettings.DEFAULT_MIN_TIME;
    private boolean hideThin;
    private HistogramColors colors = new HistogramColors(new Node());
    /** The selected cell (the block's moving corner), or -1. */
    private int selectedX = -1;
    private int selectedY = -1;
    /** The block's fixed corner: the selected cell itself unless Shift extended the selection. */
    private int anchorX = -1;
    private int anchorY = -1;
    private final List<Runnable> selectionListeners = new ArrayList<>();
    /** Enter or a double-click: show the selected cell on the timeline. */
    private Runnable activateListener = () -> {
    };
    /** Receives copied text; the system clipboard unless replaced, e.g. in tests. */
    Consumer<String> clipboard = HistogramTable::toSystemClipboard;
    /** Told what was copied, e.g. for the status strip. */
    private Consumer<String> copyListener = text -> {
    };
    /** Called with the legend's bounds when it is clicked, to open the colour settings there. */
    private Consumer<Rectangle> legendListener = bounds -> {
    };
    private Rectangle legendBounds = new Rectangle();

    /** Values in the shown unit that get the low and high end colours, and the diverging palette's centre. */
    private double scaleLow;
    private double scaleHigh;
    private double scaleCentre = Double.NaN;
    /** Some shown cells are beyond the low or high end (No outliers, Manual). */
    private boolean clippedLow;
    private boolean clippedHigh;

    /** How a mixture Z is shown: in its own unit, converted, or not a mixture at all. */
    enum Mixture {
        NONE(1, Double.NaN, ""),
        LAMBDA(1, 1, "λ"),
        LAMBDA_AS_AFR(HistogramSignals.STOICH_AFR, HistogramSignals.STOICH_AFR, "AFR"),
        AFR(1, HistogramSignals.STOICH_AFR, "AFR"),
        AFR_AS_LAMBDA(1 / HistogramSignals.STOICH_AFR, 1, "λ");

        /** Multiplies the logged value into the shown unit. */
        final double factor;
        /** Stoichiometric in the shown unit. */
        final double stoich;
        final String unit;

        Mixture(double factor, double stoich, String unit) {
            this.factor = factor;
            this.stoich = stoich;
            this.unit = unit;
        }

        static Mixture of(CsvLogReader log, int field, boolean showAfr) {
            if (HistogramSignals.isAfr(log, field)) {
                return showAfr ? AFR : AFR_AS_LAMBDA;
            }
            if (HistogramSignals.isLambda(log, field)) {
                return showAfr ? LAMBDA_AS_AFR : LAMBDA;
            }
            return NONE;
        }

        int digits() {
            return unit.equals("λ") ? 2 : 1;
        }
    }

    HistogramTable() {
        setOpaque(true);
        setFocusable(true);
        ToolTipManager.sharedInstance().registerComponent(this);
        MouseAdapter mouse = new MouseAdapter() {
            @Override
            public void mousePressed(MouseEvent event) {
                requestFocusInWindow();
                if (legendBounds.contains(event.getPoint())) {
                    legendListener.accept(new Rectangle(legendBounds));
                    return;
                }
                int[] cell = cellAt(event.getPoint());
                if (cell == null) {
                    return;
                }
                if (SwingUtilities.isRightMouseButton(event)) {
                    // the menu acts on the block under the pointer, or on the cell there
                    if (!isInBlock(cell[0], cell[1])) {
                        select(cell[0], cell[1]);
                    }
                } else if (event.isShiftDown()) {
                    extendTo(cell[0], cell[1]);
                } else if (event.getClickCount() == 2) {
                    select(cell[0], cell[1]);
                    activateListener.run();
                } else {
                    select(cell[0], cell[1]);
                }
            }

            @Override
            public void mouseMoved(MouseEvent event) {
                setCursor(legendBounds.contains(event.getPoint()) ? Cursor.getPredefinedCursor(Cursor.HAND_CURSOR)
                        : Cursor.getDefaultCursor());
            }
        };
        addMouseListener(mouse);
        addMouseMotionListener(mouse);
        bindKeys();
    }

    /** Arrows move (Shift: extend the block), Enter shows on the timeline, Ctrl+C copies, Esc clears. */
    private void bindKeys() {
        int[][] arrows = {{KeyEvent.VK_LEFT, -1, 0}, {KeyEvent.VK_RIGHT, 1, 0}, {KeyEvent.VK_UP, 0, 1},
                {KeyEvent.VK_DOWN, 0, -1}};
        for (int[] arrow : arrows) {
            bind(KeyStroke.getKeyStroke(arrow[0], 0), "move" + arrow[0], () -> move(arrow[1], arrow[2], false));
            bind(KeyStroke.getKeyStroke(arrow[0], InputEvent.SHIFT_DOWN_MASK), "extend" + arrow[0],
                    () -> move(arrow[1], arrow[2], true));
        }
        bind(KeyStroke.getKeyStroke(KeyEvent.VK_ENTER, 0), "activate", () -> {
            if (selectedX >= 0) {
                activateListener.run();
            }
        });
        bind(KeyStroke.getKeyStroke(KeyEvent.VK_ESCAPE, 0), "clearSelection", this::clearSelection);
        bind(KeyStroke.getKeyStroke(KeyEvent.VK_A, InputEvent.CTRL_DOWN_MASK), "selectAll", this::selectAll);
        bind(KeyStroke.getKeyStroke(KeyEvent.VK_C, InputEvent.CTRL_DOWN_MASK), "copy", () -> copy(false));
        bind(KeyStroke.getKeyStroke(KeyEvent.VK_C, InputEvent.CTRL_DOWN_MASK | InputEvent.SHIFT_DOWN_MASK),
                "copyWithAxes", () -> copy(true));
    }

    private void bind(KeyStroke key, String name, Runnable action) {
        getInputMap(WHEN_FOCUSED).put(key, name);
        getActionMap().put(name, new AbstractAction() {
            @Override
            public void actionPerformed(ActionEvent event) {
                if (histogram != null) {
                    action.run();
                }
            }
        });
    }

    /** Moves the selected cell (or with {@code extend} the block's moving corner); nothing selected: the top left. */
    void move(int dx, int dy, boolean extend) {
        if (selectedX < 0) {
            select(0, histogram.yAxis.size() - 1);
            return;
        }
        int x = Math.max(0, Math.min(histogram.xAxis.size() - 1, selectedX + dx));
        int y = Math.max(0, Math.min(histogram.yAxis.size() - 1, selectedY + dy));
        if (extend) {
            extendTo(x, y);
        } else {
            select(x, y);
        }
    }

    void setActivateListener(Runnable activateListener) {
        this.activateListener = activateListener;
    }

    void setCopyListener(Consumer<String> copyListener) {
        this.copyListener = copyListener;
    }

    /** {minX, minY, maxX, maxY} of the selected block, or null with nothing selected. */
    int[] block() {
        if (selectedX < 0) {
            return null;
        }
        return new int[]{Math.min(anchorX, selectedX), Math.min(anchorY, selectedY), Math.max(anchorX, selectedX),
                Math.max(anchorY, selectedY)};
    }

    private boolean isInBlock(int x, int y) {
        int[] block = block();
        return block != null && x >= block[0] && x <= block[2] && y >= block[1] && y <= block[3];
    }

    /** Extends the selection from its fixed corner to a cell, or selects the cell with nothing selected. */
    void extendTo(int x, int y) {
        if (selectedX < 0) {
            select(x, y);
            return;
        }
        if (x == selectedX && y == selectedY) {
            return;
        }
        selectedX = x;
        selectedY = y;
        repaint();
        fireSelection();
    }

    void selectAll() {
        anchorX = 0;
        anchorY = 0;
        selectedX = histogram.xAxis.size() - 1;
        selectedY = histogram.yAxis.size() - 1;
        repaint();
        fireSelection();
    }

    /**
     * The selected block (or the whole table) as tab-separated lines, top row first as on screen, the cells'
     * text as shown and empty cells blank: pastes into a TunerStudio table or a spreadsheet. With {@code axes}
     * the first line holds the X values and each line starts with its Y value.
     */
    String copyText(boolean axes) {
        int[] block = block();
        if (block == null) {
            block = new int[]{0, 0, histogram.xAxis.size() - 1, histogram.yAxis.size() - 1};
        }
        StringBuilder text = new StringBuilder();
        if (axes) {
            for (int x = block[0]; x <= block[2]; x++) {
                text.append('\t').append(axisLabel(histogram.xField, histogram.xAxis.center(x)));
            }
            text.append('\n');
        }
        for (int y = block[3]; y >= block[1]; y--) {
            if (axes) {
                text.append(axisLabel(histogram.yField, histogram.yAxis.center(y))).append('\t');
            }
            for (int x = block[0]; x <= block[2]; x++) {
                text.append(x > block[0] ? "\t" : "").append(text(x, y));
            }
            text.append('\n');
        }
        return text.toString();
    }

    /** Copies the block (or the whole table) and says what was copied. */
    void copy(boolean axes) {
        int[] block = block();
        int columns = block == null ? histogram.xAxis.size() : block[2] - block[0] + 1;
        int rows = block == null ? histogram.yAxis.size() : block[3] - block[1] + 1;
        clipboard.accept(copyText(axes));
        copyListener.accept(String.format(Locale.ROOT, "Copied %d × %d cell%s%s", columns, rows,
                columns * rows == 1 ? "" : "s", axes ? " with their axes" : ""));
    }

    private static void toSystemClipboard(String text) {
        try {
            Toolkit.getDefaultToolkit().getSystemClipboard().setContents(new StringSelection(text), null);
        } catch (HeadlessException | IllegalStateException e) {
            // no clipboard (headless) or it is busy: nothing to copy to
        }
    }

    void set(Histogram histogram, HistogramSettings.Mode mode, Mixture mixture, double minTime, boolean hideThin,
             HistogramColors colors) {
        this.histogram = histogram;
        this.mode = mode;
        this.mixture = mixture;
        this.minTime = minTime;
        this.hideThin = hideThin;
        this.colors = colors;
        updateScale();
        // the selection stays where it was while the bins stay (an empty cell can be selected too)
        if (selectedX >= 0 && (Math.max(selectedX, anchorX) >= histogram.xAxis.size()
                || Math.max(selectedY, anchorY) >= histogram.yAxis.size())) {
            selectedX = selectedY = anchorX = anchorY = -1;
        }
        fireSelection();
        repaint();
    }

    Histogram histogram() {
        return histogram;
    }

    @Override
    public Mixture mixture() {
        return mixture;
    }

    HistogramSettings.Mode mode() {
        return mode;
    }

    @Override
    public boolean showsValues() {
        return mode.isValue();
    }

    @Override
    public String zUnit() {
        return mixture != Mixture.NONE ? mixture.unit : histogram.log.unit(histogram.zField);
    }

    void setLegendListener(Consumer<Rectangle> legendListener) {
        this.legendListener = legendListener;
    }

    void addSelectionListener(Runnable listener) {
        selectionListeners.add(listener);
    }

    private void fireSelection() {
        for (Runnable listener : selectionListeners) {
            listener.run();
        }
    }

    /** Selects one cell, ending any block; -1 selects nothing. */
    void select(int x, int y) {
        if (x == selectedX && y == selectedY && x == anchorX && y == anchorY) {
            return;
        }
        selectedX = anchorX = x;
        selectedY = anchorY = y;
        repaint();
        fireSelection();
    }

    void clearSelection() {
        select(-1, -1);
    }

    int selectedX() {
        return selectedX;
    }

    int selectedY() {
        return selectedY;
    }

    /** A cell's records span less time than the thin-data limit. */
    boolean isThin(Histogram.Cell cell) {
        return cell.time < minTime;
    }

    /** False for a thin cell while thin cells are hidden. */
    boolean isShown(Histogram.Cell cell) {
        return !(hideThin && isThin(cell));
    }

    /** The number a cell shows in the current mode, in the shown unit. */
    double value(Histogram.Cell cell) {
        switch (mode) {
            case MIN:
                return cell.min * mixture.factor;
            case MAX:
                return cell.max * mixture.factor;
            case TIME:
                return cell.time;
            case COUNT:
                return cell.count;
            default:
                return cell.mean() * mixture.factor;
        }
    }

    /** A Z value in the shown unit, with the precision of that unit. */
    String formatZ(double shown) {
        if (!Double.isFinite(shown)) {
            return ValueFormat.NONE;
        }
        int digits = mixture == Mixture.NONE ? histogram.log.digits(histogram.zField) : mixture.digits();
        return String.format(Locale.ROOT, "%." + digits + "f", shown);
    }

    /** A value as the cells and legend show it in the current mode. */
    private String formatValue(double value) {
        switch (mode) {
            case TIME:
                return String.format(Locale.ROOT, value < 1 ? "%.2f" : value < 10 ? "%.1f" : "%.0f", value);
            case COUNT:
                return String.format(Locale.ROOT, "%d", Math.round(value));
            default:
                return formatZ(value);
        }
    }

    /** The text of a cell, empty for a cell without (shown) records. */
    String text(int x, int y) {
        Histogram.Cell cell = histogram.cell(x, y);
        return cell == null || !isShown(cell) ? "" : formatValue(value(cell));
    }

    // ---- colours ----

    private boolean isDiverging() {
        return colors.palette() == HistogramColors.Palette.DIVERGING;
    }

    private void updateScale() {
        int columns = histogram.xAxis.size();
        double[] values = new double[columns * histogram.yAxis.size()];
        int count = 0;
        for (int y = 0; y < histogram.yAxis.size(); y++) {
            for (int x = 0; x < columns; x++) {
                Histogram.Cell cell = histogram.cell(x, y);
                if (cell != null && isShown(cell)) {
                    values[count++] = value(cell);
                }
            }
        }
        // occupancy (Time, Count) starts from nothing; a manual range is for values, not seconds or records
        HistogramColors.Scale scale = colors.scale(values, count, mode.isValue(), mixture.factor, mixture.stoich);
        scaleLow = scale.low;
        scaleHigh = scale.high;
        scaleCentre = scale.centre;
        clippedLow = scale.clippedLow;
        clippedHigh = scale.clippedHigh;
    }

    @Override
    public double scaleLow() {
        return scaleLow;
    }

    @Override
    public double scaleHigh() {
        return scaleHigh;
    }

    @Override
    public double scaleCentre() {
        return scaleCentre;
    }

    /** Where a value falls on the palette, 0 (low end) to 1 (high end), after Reverse. */
    double position(double value) {
        double t;
        if (!(scaleHigh > scaleLow)) {
            t = 0.5;
        } else {
            t = Math.max(0, Math.min(1, (value - scaleLow) / (scaleHigh - scaleLow)));
            if (!mode.isValue()) {
                // occupancy is uneven (idle dwarfs the rest): a square root keeps the quieter cells visible
                t = Math.sqrt(t);
            }
        }
        return colors.reversed() ? 1 - t : t;
    }

    double position(Histogram.Cell cell) {
        return position(value(cell));
    }

    Color cellColor(Histogram.Cell cell) {
        return HistogramColors.color(colors.palette(), position(cell));
    }

    Color textColor(Histogram.Cell cell) {
        if (isThin(cell)) {
            return themed("Console.LogViewer.mutedForeground", Color.GRAY);
        }
        return HistogramColors.wantsLightText(cellColor(cell))
                ? themed("Console.LogViewer.histogram.lightText", Color.WHITE)
                : themed("Console.LogViewer.histogram.darkText", new Color(0x151a21));
    }

    static Color mix(Color from, Color to, double amount) {
        double a = Math.max(0, Math.min(1, amount));
        return new Color(
                (int) Math.round(from.getRed() + (to.getRed() - from.getRed()) * a),
                (int) Math.round(from.getGreen() + (to.getGreen() - from.getGreen()) * a),
                (int) Math.round(from.getBlue() + (to.getBlue() - from.getBlue()) * a));
    }

    // ---- layout ----

    /** Where the cells are drawn: left and top edge, cell width and height. */
    private static final class Grid {
        int left;
        int top;
        double cellWidth;
        double cellHeight;
    }

    private Grid grid() {
        FontMetrics metrics = getFontMetrics(labelFont());
        Grid grid = new Grid();
        int yLabels = 0;
        for (int y = 0; y < histogram.yAxis.size(); y++) {
            yLabels = Math.max(yLabels, metrics.stringWidth(axisLabel(histogram.yField, histogram.yAxis.center(y))));
        }
        int titleWidth = metrics.getHeight() + 4;
        grid.left = PAD + titleWidth + yLabels + 8;
        grid.top = PAD;
        int bottom = metrics.getHeight() * 2 + 10 + LEGEND_HEIGHT + PAD;
        int width = Math.max(1, getWidth() - grid.left - PAD);
        int height = Math.max(1, getHeight() - grid.top - bottom);
        grid.cellWidth = (double) width / histogram.xAxis.size();
        grid.cellHeight = (double) height / histogram.yAxis.size();
        return grid;
    }

    /** The cell rectangle of column x and row y (row 0 at the bottom). */
    Rectangle cellBounds(int x, int y) {
        Grid grid = grid();
        int row = histogram.yAxis.size() - 1 - y;
        int left = grid.left + (int) Math.round(x * grid.cellWidth);
        int right = grid.left + (int) Math.round((x + 1) * grid.cellWidth) - GAP;
        int top = grid.top + (int) Math.round(row * grid.cellHeight);
        int bottom = grid.top + (int) Math.round((row + 1) * grid.cellHeight) - GAP;
        return new Rectangle(left, top, Math.max(1, right - left), Math.max(1, bottom - top));
    }

    /** {x, y} of the cell under a point, or null. */
    int[] cellAt(Point point) {
        if (histogram == null) {
            return null;
        }
        Grid grid = grid();
        int x = (int) Math.floor((point.x - grid.left) / grid.cellWidth);
        int row = (int) Math.floor((point.y - grid.top) / grid.cellHeight);
        if (x < 0 || x >= histogram.xAxis.size() || row < 0 || row >= histogram.yAxis.size()) {
            return null;
        }
        return new int[]{x, histogram.yAxis.size() - 1 - row};
    }

    /** Where the legend was last drawn; a click there opens the colour settings. */
    Rectangle legendBounds() {
        return new Rectangle(legendBounds);
    }

    private String axisLabel(int field, double value) {
        return ValueFormat.format(histogram.log, field, value);
    }

    private Font labelFont() {
        return getFont().deriveFont(getFont().getSize2D() - 1);
    }

    @Override
    public Dimension getPreferredSize() {
        if (histogram == null) {
            return new Dimension(400, 300);
        }
        return new Dimension(histogram.xAxis.size() * 52 + 120, histogram.yAxis.size() * 26 + 120);
    }

    @Override
    public Dimension getMinimumSize() {
        if (histogram == null) {
            return new Dimension(200, 150);
        }
        return new Dimension(histogram.xAxis.size() * MIN_CELL_WIDTH + 100,
                histogram.yAxis.size() * MIN_CELL_HEIGHT + 110);
    }

    @Override
    public String getToolTipText(MouseEvent event) {
        if (legendBounds.contains(event.getPoint())) {
            return "Click to change the colours: palette, range and centre, kept for "
                    + histogram.log.name(histogram.zField);
        }
        int[] at = cellAt(event.getPoint());
        if (at == null) {
            return null;
        }
        CsvLogReader log = histogram.log;
        HistogramAxis xAxis = histogram.xAxis;
        HistogramAxis yAxis = histogram.yAxis;
        String where = String.format(Locale.ROOT, "%s %s–%s · %s %s–%s", log.name(histogram.xField),
                axisLabel(histogram.xField, xAxis.lower(at[0])), axisLabel(histogram.xField, xAxis.upper(at[0])),
                log.name(histogram.yField),
                axisLabel(histogram.yField, yAxis.lower(at[1])), axisLabel(histogram.yField, yAxis.upper(at[1])));
        Histogram.Cell cell = histogram.cell(at[0], at[1]);
        if (cell == null) {
            return "<html>" + where + "<br>No records</html>";
        }
        String values = String.format(Locale.ROOT, "mean %s · min %s · max %s", formatZ(cell.mean() * mixture.factor),
                formatZ(cell.min * mixture.factor), formatZ(cell.max * mixture.factor));
        String time = String.format(Locale.ROOT, "%.1f s · %,d records%s", cell.time, cell.count,
                isThin(cell) ? " · under the thin-data limit" : "");
        return "<html>" + where + "<br>" + values + "<br>" + time + "</html>";
    }

    // ---- painting ----

    @Override
    protected void paintComponent(Graphics graphics) {
        Graphics2D g = (Graphics2D) graphics.create();
        try {
            g.setRenderingHint(RenderingHints.KEY_ANTIALIASING, RenderingHints.VALUE_ANTIALIAS_ON);
            g.setRenderingHint(RenderingHints.KEY_TEXT_ANTIALIASING, RenderingHints.VALUE_TEXT_ANTIALIAS_ON);
            g.setColor(themed("Console.LogViewer.sidebar.background", Color.WHITE));
            g.fillRect(0, 0, getWidth(), getHeight());
            legendBounds = new Rectangle();
            if (histogram == null) {
                return;
            }
            Color muted = themed("Console.LogViewer.mutedForeground", Color.GRAY);
            if (histogram.usedRecords == 0) {
                g.setColor(muted);
                g.setFont(getFont());
                String message = "No records to show: check the filters, the time range or the signals";
                g.drawString(message, (getWidth() - g.getFontMetrics().stringWidth(message)) / 2, getHeight() / 2);
                return;
            }
            Grid grid = grid();
            paintCells(g);
            paintAxes(g, grid, muted);
            paintLegend(g, grid, muted);
        } finally {
            g.dispose();
        }
    }

    private void paintCells(Graphics2D g) {
        Font cellFont = getFont().deriveFont(Font.BOLD, getFont().getSize2D() - 1);
        g.setFont(cellFont);
        FontMetrics metrics = g.getFontMetrics();
        Color empty = themed("Console.LogViewer.histogram.empty", Color.WHITE);
        Color emptyBorder = themed("Console.LogViewer.histogram.emptyBorder", new Color(0xeef0f3));
        Color hatch = themed("Console.LogViewer.histogram.hatch", new Color(88, 98, 115, 58));
        for (int y = 0; y < histogram.yAxis.size(); y++) {
            for (int x = 0; x < histogram.xAxis.size(); x++) {
                Rectangle bounds = cellBounds(x, y);
                Histogram.Cell cell = histogram.cell(x, y);
                if (cell == null || !isShown(cell)) {
                    g.setColor(empty);
                    g.fillRoundRect(bounds.x, bounds.y, bounds.width, bounds.height, 4, 4);
                    g.setColor(emptyBorder);
                    g.drawRoundRect(bounds.x, bounds.y, bounds.width - 1, bounds.height - 1, 4, 4);
                    continue;
                }
                Color fill = cellColor(cell);
                boolean thin = isThin(cell);
                g.setColor(thin ? mix(fill, Color.WHITE, 0.6) : fill);
                g.fillRoundRect(bounds.x, bounds.y, bounds.width, bounds.height, 4, 4);
                if (thin) {
                    paintHatch(g, bounds, hatch);
                }
                String text = text(x, y);
                if (metrics.stringWidth(text) + 4 <= bounds.width && metrics.getAscent() <= bounds.height) {
                    g.setColor(textColor(cell));
                    g.drawString(text, bounds.x + (bounds.width - metrics.stringWidth(text)) / 2,
                            bounds.y + (bounds.height + metrics.getAscent() - metrics.getDescent()) / 2);
                }
            }
        }
        int[] block = block();
        if (block != null && (block[0] != block[2] || block[1] != block[3])) {
            // a block: tinted and outlined in the accent, its moving corner outlined dark below
            Rectangle low = cellBounds(block[0], block[1]);
            Rectangle high = cellBounds(block[2], block[3]);
            Rectangle area = low.union(high);
            Color accent = themed("Console.LogViewer.histogram.accent", new Color(0x1d4ed8));
            g.setColor(new Color(accent.getRed(), accent.getGreen(), accent.getBlue(), 40));
            g.fillRoundRect(area.x - 1, area.y - 1, area.width + 2, area.height + 2, 6, 6);
            g.setStroke(new BasicStroke(2f));
            g.setColor(accent);
            g.drawRoundRect(area.x - 2, area.y - 2, area.width + 3, area.height + 3, 6, 6);
            g.setStroke(new BasicStroke(1f));
        }
        if (selectedX >= 0) {
            Rectangle bounds = cellBounds(selectedX, selectedY);
            g.setStroke(new BasicStroke(2f));
            g.setColor(themed("Console.LogViewer.histogram.selection", new Color(0x151a21)));
            g.drawRoundRect(bounds.x - 1, bounds.y - 1, bounds.width + 1, bounds.height + 1, 6, 6);
            g.setStroke(new BasicStroke(1f));
        }
    }

    private static void paintHatch(Graphics2D g, Rectangle bounds, Color hatch) {
        Shape clip = g.getClip();
        g.clipRect(bounds.x, bounds.y, bounds.width, bounds.height);
        g.setColor(hatch);
        for (int offset = -bounds.height; offset < bounds.width; offset += 5) {
            g.drawLine(bounds.x + offset, bounds.y + bounds.height, bounds.x + offset + bounds.height, bounds.y);
        }
        g.setClip(clip);
    }

    private void paintAxes(Graphics2D g, Grid grid, Color muted) {
        Font font = labelFont();
        g.setFont(font);
        FontMetrics metrics = g.getFontMetrics();
        Color accent = themed("Console.LogViewer.histogram.accent", new Color(0x1d4ed8));
        Font bold = font.deriveFont(Font.BOLD);
        // row labels, right-aligned left of the cells
        for (int y = 0; y < histogram.yAxis.size(); y++) {
            Rectangle bounds = cellBounds(0, y);
            String label = axisLabel(histogram.yField, histogram.yAxis.center(y));
            boolean selected = y == selectedY;
            g.setFont(selected ? bold : font);
            g.setColor(selected ? accent : muted);
            FontMetrics m = g.getFontMetrics();
            if (m.getAscent() <= bounds.height + GAP || y % 2 == 0) {
                g.drawString(label, grid.left - 8 - m.stringWidth(label),
                        bounds.y + (bounds.height + m.getAscent() - m.getDescent()) / 2);
            }
        }
        // column labels under the cells, skipping those that would overlap
        int labelsTop = cellBounds(0, 0).y + cellBounds(0, 0).height + 6;
        int lastRight = Integer.MIN_VALUE;
        for (int x = 0; x < histogram.xAxis.size(); x++) {
            Rectangle bounds = cellBounds(x, 0);
            String label = axisLabel(histogram.xField, histogram.xAxis.center(x));
            boolean selected = x == selectedX;
            g.setFont(selected ? bold : font);
            FontMetrics m = g.getFontMetrics();
            int left = bounds.x + (bounds.width - m.stringWidth(label)) / 2;
            if (left > lastRight + 4 || selected) {
                g.setColor(selected ? accent : muted);
                g.drawString(label, left, labelsTop + m.getAscent());
                lastRight = left + m.stringWidth(label);
            }
        }
        // axis titles
        g.setFont(font.deriveFont(Font.BOLD));
        g.setColor(muted);
        String xTitle = title(histogram.xField);
        int cellsWidth = (int) Math.round(grid.cellWidth * histogram.xAxis.size());
        g.drawString(xTitle, grid.left + (cellsWidth - g.getFontMetrics().stringWidth(xTitle)) / 2,
                labelsTop + metrics.getHeight() + g.getFontMetrics().getAscent() + 2);
        String yTitle = title(histogram.yField);
        int cellsHeight = (int) Math.round(grid.cellHeight * histogram.yAxis.size());
        AffineTransform transform = g.getTransform();
        g.translate(PAD + g.getFontMetrics().getAscent(), grid.top + (cellsHeight + g.getFontMetrics().stringWidth(yTitle)) / 2);
        g.rotate(-Math.PI / 2);
        g.drawString(yTitle, 0, 0);
        g.setTransform(transform);
    }

    private String title(int field) {
        String unit = histogram.log.unit(field);
        String name = histogram.log.name(field);
        return unit.isEmpty() || unit.equalsIgnoreCase(name) ? name : name + " · " + unit;
    }

    /** The low and high labels of the legend, "≤" / "≥" where cells lie beyond them. */
    String[] legendLabels() {
        String low = (clippedLow ? "≤ " : "") + formatValue(scaleLow);
        String high = (clippedHigh ? "≥ " : "") + formatValue(scaleHigh);
        if (mode == HistogramSettings.Mode.TIME) {
            low += " s";
            high += " s";
        }
        if (isDiverging() && mode.isValue() && mixture != Mixture.NONE) {
            boolean reversed = colors.reversed();
            low += reversed ? " lean" : " rich";
            high += reversed ? " rich" : " lean";
        }
        return new String[]{low, high};
    }

    private void paintLegend(Graphics2D g, Grid grid, Color muted) {
        g.setFont(labelFont());
        FontMetrics metrics = g.getFontMetrics();
        int baseline = getHeight() - PAD - (LEGEND_HEIGHT - metrics.getAscent()) / 2;
        int barTop = baseline - metrics.getAscent() + 1;
        int barHeight = Math.max(8, metrics.getAscent() - 2);
        int start = grid.left;
        int x = start;
        String[] labels = legendLabels();
        g.setColor(muted);
        g.drawString(labels[0], x, baseline);
        x += metrics.stringWidth(labels[0]) + 8;
        for (int i = 0; i < LEGEND_BAR_WIDTH; i++) {
            double t = (i + 0.5) / LEGEND_BAR_WIDTH;
            if (!mode.isValue()) {
                t = Math.sqrt(t);
            }
            g.setColor(HistogramColors.color(colors.palette(), colors.reversed() ? 1 - t : t));
            g.fillRect(x + i, barTop, 1, barHeight);
        }
        x += LEGEND_BAR_WIDTH + 8;
        g.setColor(muted);
        g.drawString(labels[1], x, baseline);
        x += metrics.stringWidth(labels[1]) + 10;
        String settings = (Double.isFinite(scaleCentre) ? "centre " + formatValue(scaleCentre) + "  ·  " : "")
                + colors.palette().text + "  ▾";
        g.setColor(themed("Console.LogViewer.histogram.accent", new Color(0x1d4ed8)));
        g.drawString(settings, x, baseline);
        x += metrics.stringWidth(settings);
        legendBounds = new Rectangle(start - 4, barTop - 6, x - start + 8, barHeight + 12);
        x += 24;

        if (!hideThin) {
            Rectangle swatch = new Rectangle(x, barTop - 1, 22, barHeight + 2);
            g.setColor(mix(themed("Console.LogViewer.histogram.neutral", new Color(0xf4f5f7)), Color.WHITE, 0.6));
            g.fillRect(swatch.x, swatch.y, swatch.width, swatch.height);
            paintHatch(g, swatch, themed("Console.LogViewer.histogram.hatch", new Color(88, 98, 115, 58)));
            x += swatch.width + 6;
            g.setColor(muted);
            String thin = String.format(Locale.ROOT, "under %s s of data", HistogramFilter.number(minTime));
            g.drawString(thin, x, baseline);
            x += metrics.stringWidth(thin) + 24;
        }
        g.setColor(themed("Console.LogViewer.histogram.empty", Color.WHITE));
        g.fillRect(x, barTop - 1, 22, barHeight + 2);
        g.setColor(themed("Console.LogViewer.histogram.emptyBorder", new Color(0xeef0f3)));
        g.drawRect(x, barTop - 1, 21, barHeight + 1);
        x += 28;
        g.setColor(muted);
        g.drawString(hideThin ? "no data, or under the thin-data limit" : "no data", x, baseline);
        String weighted = mode == HistogramSettings.Mode.MEAN ? "Means are time-weighted" : "";
        g.drawString(weighted, getWidth() - PAD - metrics.stringWidth(weighted), baseline);
    }
}
