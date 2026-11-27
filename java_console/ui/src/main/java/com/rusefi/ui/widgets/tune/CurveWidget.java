package com.rusefi.ui.widgets.tune;

import com.opensr5.ConfigurationImage;
import com.opensr5.ConfigurationImageGetterSetter;
import com.opensr5.ini.AxisModel;
import com.opensr5.ini.CurveModel;
import com.opensr5.ini.ExpressionEvaluator;
import com.opensr5.ini.IniFileModel;
import com.opensr5.ini.TsStringFunction;
import com.opensr5.ini.field.ArrayIniField;
import com.opensr5.ini.field.IniField;
import com.opensr5.ini.field.PcVariableArrayIniField;

import javax.swing.*;
import javax.swing.table.AbstractTableModel;
import javax.swing.table.DefaultTableCellRenderer;
import java.awt.*;
import java.awt.event.MouseAdapter;
import java.awt.event.MouseEvent;
import java.awt.event.KeyEvent;
import java.awt.geom.AffineTransform;
import java.util.Locale;
import java.util.HashMap;
import java.util.Map;
import java.util.Objects;
import java.util.Optional;
import java.util.EventObject;
import java.util.function.Supplier;

/**
 * curve panel consists of two parts
 *
 * on the left we have black canvas representing an X by Y table, with a grid of step steps per AxisModel
 * y axis is displayed in descending order
 * grid scale is displayed as red text labels showing from min to max according to AxisModel
 *
 * actual values are charged with green dots and a bright orange line connecting the value dots. User can drag and drop
 * by green dots, the line should render as per dot movement. it should not be possible to move dot too far left part neighboring line
 * or too far right, enforcing X-coordinate to stay in order
 *
 * on the right we need a vertical jtable showing axis values, with X-asix as first column and Y-asix as second column.
 * table cells should follow the gradient effect as in the TrimsTab
 */
public class CurveWidget {
    private final JPanel content = new JPanel(new BorderLayout());
    private final CurveCanvas canvas = new CurveCanvas();
    private final JTable table = new JTable();
    /** Live value of the X axis channel, from {@code xBins = bins, channel} (issue #10670). */
    private final LiveChannelFeed live = new LiveChannelFeed(canvas, () -> {
        canvas.repaint();
        table.repaint();
    });

    private CurveModel curveModel;
    private AxisModel xAxis;
    private AxisModel yAxis;
    private AxisModel configuredXAxis;
    private AxisModel configuredYAxis;
    private double xMin;
    private double xMax;
    private double yMin;
    private double yMax;
    private Double[] xValues;
    private Double[] yValues;
    private String xUnits;
    private String yUnits;
    private String xLabel;
    private String yLabel;
    private String curveTitle;
    private final boolean readOnly;
    private int xDigits;
    private int yDigits;
    private ConfigurationImage imageTarget;
    private ArrayIniField xBinsField;
    private boolean xBinsFixed;
    private ArrayIniField yBinsField;
    private Runnable onEdit;
    /** [tag:unburned_marks] The last burned tune; null marks nothing. */
    private Supplier<ConfigurationImage> burnedImage = () -> null;
    private Supplier<UnburnedWording> unburnedWording = () -> UnburnedWording.BURNED;
    private final JLabel unburnedCountLabel = UnburnedCellMark.createCountLabel();

    public CurveWidget(CurveModel curveModel, IniFileModel iniFileModel, ConfigurationImage ci) {
        this(curveModel, iniFileModel, ci, ci, false);
    }

    public CurveWidget(CurveModel curveModel, IniFileModel iniFileModel, ConfigurationImage valuesImage,
                       ConfigurationImage configImage, boolean readOnly) {
        this.readOnly = readOnly;
        table.getTableHeader().setReorderingAllowed(false);
        table.setDefaultRenderer(Object.class, new GradientRenderer());
        table.setDefaultEditor(Object.class, new DefaultCellEditor(new JTextField()) {
            private boolean keyboardEditing;

            @Override
            public boolean isCellEditable(EventObject event) {
                keyboardEditing = event instanceof KeyEvent;
                return super.isCellEditable(event);
            }

            @Override
            public Component getTableCellEditorComponent(JTable table, Object value, boolean isSelected, int row, int column) {
                JTextField editor = (JTextField) super.getTableCellEditorComponent(table, value, isSelected, row, column);
                if (keyboardEditing) {
                    editor.setText("");
                }
                return editor;
            }
        });
        table.setCellSelectionEnabled(true);
        table.setSelectionMode(ListSelectionModel.MULTIPLE_INTERVAL_SELECTION);
        table.setToolTipText(readOnly ? "ECU page values are read-only" :
            "Type to replace a value; Enter to accept; Esc to cancel; Shift+arrows to select");
        table.getSelectionModel().addListSelectionListener(e -> canvas.repaint());
        table.getColumnModel().getSelectionModel().addListSelectionListener(e -> canvas.repaint());
        // JTable defaults to a 450x400 preferredScrollableViewportSize; left unchanged the table would
        // claim ~450px of fixed preferred width and starve the canvas (which only receives the GridBag
        // remainder) on narrow windows. Size the table to its actual content so the 0.8/0.2 weights hold.
        table.setPreferredScrollableViewportSize(new Dimension(180, 300));

        JPanel rightPanel = new JPanel(new BorderLayout());
        // The table header is reparented into the JScrollPane's column header by configureEnclosingScrollPane(),
        // so it does not need (and must not get) a separate NORTH slot here.
        rightPanel.add(new JScrollPane(table), BorderLayout.CENTER);
        unburnedCountLabel.setBorder(BorderFactory.createEmptyBorder(4, 0, 0, 0));
        // The count's line is reserved while it is hidden: the plot shares this row, and appearing on
        // the first edit made the window grow the row and stretch the curve.
        JPanel countLine = new JPanel(new BorderLayout()) {
            @Override
            public Dimension getPreferredSize() {
                Dimension size = super.getPreferredSize();
                Insets insets = unburnedCountLabel.getInsets();
                size.height = Math.max(size.height, unburnedCountLabel.getFontMetrics(unburnedCountLabel.getFont()).getHeight()
                    + insets.top + insets.bottom);
                return size;
            }
        };
        countLine.setOpaque(false);
        countLine.add(unburnedCountLabel, BorderLayout.CENTER);
        rightPanel.add(countLine, BorderLayout.SOUTH);

        content.setLayout(new GridBagLayout());
        GridBagConstraints gbc = new GridBagConstraints();
        gbc.fill = GridBagConstraints.BOTH;
        gbc.weighty = 1.0;

        gbc.gridx = 0;
        gbc.weightx = 0.8;
        content.add(canvas, gbc);

        gbc.gridx = 1;
        gbc.weightx = 0.2;
        gbc.insets = new Insets(0, 8, 0, 0);
        content.add(rightPanel, gbc);

        update(curveModel, iniFileModel, valuesImage, configImage);
    }

    private void update(CurveModel curveModel, IniFileModel iniFile, ConfigurationImage valuesImage,
                        ConfigurationImage configImage) {
        this.curveModel = Objects.requireNonNull(curveModel);
        this.imageTarget = valuesImage;
        this.configuredXAxis = curveModel.getxAxis().resolve(iniFile, configImage);
        this.configuredYAxis = curveModel.getyAxis().resolve(iniFile, configImage);
        IniField xField = iniFile.findIniField(curveModel.getxBins()).get();
        this.curveTitle = resolveCurveLabel(curveModel.getTitle(), iniFile, valuesImage, configImage, xField.getPageIndex());
        this.xLabel = resolveCurveLabel(curveModel.getXLabel(), iniFile, valuesImage, configImage, xField.getPageIndex());
        this.yLabel = resolveCurveLabel(curveModel.getYLabel(), iniFile, valuesImage, configImage, xField.getPageIndex());
        this.xUnits = resolveUnits(xField.getUnits(), iniFile, configImage);
        this.xDigits = parseDigits(xField.getDigits());
        this.xBinsField = xField instanceof ArrayIniField ? (ArrayIniField) xField : null;
        // PC variable bins (gear numbers, #10626) are fixed by the INI and have no tune storage
        this.xBinsFixed = xField instanceof PcVariableArrayIniField;

        IniField yField = iniFile.findIniField(curveModel.getyBins()).get();
        this.yUnits = resolveUnits(yField.getUnits(), iniFile, configImage);
        this.yDigits = parseDigits(yField.getDigits());
        this.yBinsField = yField instanceof ArrayIniField ? (ArrayIniField) yField : null;

        // Field limits are engineering values, independent of the INI's suggested plot range (#9191).
        xMin = resolveLimit(xBinsField == null ? null : xBinsField.getMin(), iniFile, configImage, Double.NEGATIVE_INFINITY);
        xMax = resolveLimit(xBinsField == null ? null : xBinsField.getMax(), iniFile, configImage, Double.POSITIVE_INFINITY);
        yMin = resolveLimit(yBinsField == null ? null : yBinsField.getMin(), iniFile, configImage, Double.NEGATIVE_INFINITY);
        yMax = resolveLimit(yBinsField == null ? null : yBinsField.getMax(), iniFile, configImage, Double.POSITIVE_INFINITY);

        this.xValues = readArray(curveModel.getxBins(), iniFile, valuesImage);
        this.yValues = readArray(curveModel.getyBins(), iniFile, valuesImage);
        xAxis = fitAxis(configuredXAxis, xValues);
        yAxis = fitAxis(configuredYAxis, yValues);

        canvas.setCurve(curveModel, xValues, yValues);
        table.setModel(new CurveTableModel());
        live.setChannels(curveModel.getXBinsChannel());
        refreshUnburnedMarks();
        content.revalidate();
        content.repaint();
    }

    /** [tag:unburned_marks] Supplies the last burned tune, so changed points are marked in the table. */
    public void setBurnedImage(Supplier<ConfigurationImage> burnedImage) {
        this.burnedImage = burnedImage == null ? () -> null : burnedImage;
        refreshUnburnedMarks();
    }

    /** Names the reference tune in the cell tooltips: burned, or loaded while offline. */
    public void setUnburnedWording(Supplier<UnburnedWording> wording) {
        unburnedWording = wording == null ? () -> UnburnedWording.BURNED : wording;
    }

    /** Re-marks the table and recounts, after an edit or when the burned tune changes. */
    public void refreshUnburnedMarks() {
        UnburnedChanges changes = UnburnedChanges.of(imageTarget, burnedImage.get());
        UnburnedCellMark.updateCountLabel(unburnedCountLabel,
            changes.countChangedCells(xBinsField) + changes.countChangedCells(yBinsField));
        table.repaint();
    }

    JLabel getUnburnedCountLabelForTests() {
        return unburnedCountLabel;
    }

    /** The burned value shown in table cell ({@code row}, {@code column}) when it differs, else null. */
    String burnedCellText(int row, int column) {
        ConfigurationImage burned = burnedImage.get();
        ArrayIniField field = column == 0 ? xBinsField : yBinsField;
        if (burned == null || field == null || field.getCols() <= 0) {
            return null;
        }
        // curve points are the field's cells in row-major order, as readArray reads them
        int storedRow = row / field.getCols();
        int storedColumn = row % field.getCols();
        if (!UnburnedChanges.of(imageTarget, burned).isCellChanged(field, storedRow, storedColumn)) {
            return null;
        }
        return formatValue(UnburnedCellMark.burnedValue(field, storedRow, storedColumn, burned),
            column == 0 ? xDigits : yDigits);
    }

    LiveChannelFeed liveFeedForUnitTest() {
        return live;
    }

    /** The live X value as a fractional index into the curve points, or null when there is none. */
    private Double livePosition() {
        if (xValues == null || yValues == null) {
            return null;
        }
        return LiveOperatingPoint.axisPosition(xValues, Math.min(xValues.length, yValues.length), live.value(0));
    }

    /** The table row nearest the live X value, or -1. */
    int liveRow() {
        Double position = livePosition();
        return position == null ? -1 : (int) Math.round(position);
    }

    private static String resolveCurveLabel(String raw, IniFileModel ini, ConfigurationImage valuesImage,
                                            ConfigurationImage configImage, int dataPage) {
        Map<String, Double> values = new HashMap<>();
        if (dataPage != 0 && valuesImage != null) {
            for (String variable : ExpressionEvaluator.extractVariables(raw)) {
                Optional<IniField> field = ini.findIniField(variable);
                if (field.isPresent() && field.get().getPageIndex() == dataPage
                    && field.get().getOffset() + Math.max(4, field.get().getSize()) <= valuesImage.getSize()) {
                    Double value = valuesImage.readNumericValue(field.get());
                    if (value != null) {
                        values.put(variable, value);
                    }
                }
            }
        }
        return TsStringFunction.resolveDisplayLabel(raw, ini, configImage, values);
    }

    public void setOnEdit(Runnable onEdit) {
        this.onEdit = onEdit;
    }

    private static double resolveLimit(String raw, IniFileModel ini, ConfigurationImage image, double fallback) {
        Double value = ExpressionEvaluator.evaluateNumericExpression(raw, ini, image);
        if (value == null) {
            value = ExpressionEvaluator.tryEvaluate(raw);
        }
        return value != null && Double.isFinite(value) ? value : fallback;
    }

    /**
     * After an edit the plot only grows, when a value leaves it: a scale that shrank or moved with
     * every edit made the points that were not edited jump on screen. Opening the curve fits it tightly.
     */
    private void updatePlotAxes() {
        xAxis = growAxis(xAxis, configuredXAxis, xValues);
        yAxis = growAxis(yAxis, configuredYAxis, yValues);
    }

    private static AxisModel growAxis(AxisModel current, AxisModel configured, Double[] values) {
        for (Double value : values) {
            if (value != null && Double.isFinite(value) && (value < current.getMin() || value > current.getMax())) {
                // Refit around the values and everything shown now, with the INI's tick count.
                return fitAxis(new AxisModel(current.getMin(), current.getMax(), configured.getStep()), values);
            }
        }
        return current;
    }

    private static AxisModel fitAxis(AxisModel configured, Double[] values) {
        double min = configured.getMin();
        double max = configured.getMax();
        for (Double value : values) {
            if (value != null && Double.isFinite(value)) {
                min = Math.min(min, value);
                max = Math.max(max, value);
            }
        }
        if (min == configured.getMin() && max == configured.getMax() && min < max && configured.getStep() >= 2) {
            return configured;
        }
        if (min == max) {
            // Avoid a zero-sized transform, including unresolved or constant INI axes.
            min -= 1;
            max += 1;
        }
        // Expand to readable 1/2/5 * 10^n ticks, preserving at least the suggested INI range.
        double step = (max - min) / Math.max(1, configured.getStep() - 1);
        double magnitude = Math.pow(10, Math.floor(Math.log10(step)));
        double fraction = step / magnitude;
        double roundedStep = (fraction <= 1 ? 1 : fraction <= 2 ? 2 : fraction <= 5 ? 5 : 10) * magnitude;
        min = Math.floor(min / roundedStep) * roundedStep;
        max = Math.ceil(max / roundedStep) * roundedStep;
        return new AxisModel(min, max, (int) Math.round((max - min) / roundedStep) + 1);
    }

    private void setXValue(int index, double value) {
        if (xBinsFixed) {
            return;
        }
        double min = index == 0 ? xMin : Math.max(xMin, xValues[index - 1]);
        double max = index == xValues.length - 1 ? xMax : Math.min(xMax, xValues[index + 1]);
        if (min <= max) {
            xValues[index] = Math.max(min, Math.min(max, value));
        }
    }

    private void setYValue(int index, double value) {
        yValues[index] = Math.max(yMin, Math.min(yMax, value));
    }

    private void writeBackToImage() {
        if (imageTarget == null) return;
        if (xBinsField != null) ConfigurationImageGetterSetter.setArrayValues(xBinsField, imageTarget, xValues);
        if (yBinsField != null) ConfigurationImageGetterSetter.setArrayValues(yBinsField, imageTarget, yValues);
    }

    /**
     * The units token may be a TunerStudio expression such as {@code {bitStringValue(unitsLabels,
     * useMetricOnInterface)}}, which picks a °C/°F label depending on a config setting. Resolve it so the
     * canvas shows the actual unit instead of the raw expression (issue #9635). {@code useMetricOnInterface}
     * lives in the configuration image, so {@code ci} is passed as the config source (not output channels).
     */
    public static String resolveUnits(String raw, IniFileModel ini, ConfigurationImage ci) {
        if (raw == null) {
            return null;
        }
        if (TsStringFunction.containsStringFunction(raw)) {
            String resolved = TsStringFunction.resolve(raw, ini, ci, null);
            // On failure (e.g. no config image) show nothing rather than the raw expression text.
            return resolved != null ? resolved : "";
        }
        String t = raw.trim();
        if (t.startsWith("{")) {
            t = t.replaceAll("^\\{\\s*", "").replaceAll("\\s*}$", "").trim();
        }
        return t;
    }

    /** A curve value with the INI's digits every time, like table values, so a column's decimals line up. */
    static String formatValue(double value, int digits) {
        if (digits <= 0) {
            return Long.toString(Math.round(value));
        }
        return String.format(Locale.ROOT, "%." + digits + "f", value);
    }

    private int parseDigits(String digits) {
        if (digits == null || digits.isEmpty()) {
            return 1;
        }
        try {
            return Integer.parseInt(digits);
        } catch (NumberFormatException e) {
            return 1;
        }
    }

    private Double[] readArray(String key, IniFileModel iniFile, ConfigurationImage ci) {
        if (iniFile == null)
            return new Double[0];
        Optional<IniField> field = iniFile.findIniField(key);
        if (field.isPresent() && field.get() instanceof ArrayIniField) {
            ArrayIniField arrayField = (ArrayIniField) field.get();
            if (ci != null) {
                Double[][] arrayValues = ConfigurationImageGetterSetter.getArrayValues(arrayField, ci);
                Double[] result = new Double[arrayField.getRows() * arrayField.getCols()];
                int index = 0;
                for (int r = 0; r < arrayField.getRows(); r++) {
                    for (int c = 0; c < arrayField.getCols(); c++) {
                        result[index++] = arrayValues[r][c];
                    }
                }
                return result;
            }

            // Fallback for when ci is null
            int size = arrayField.getRows() * arrayField.getCols();
            Double[] values = new Double[size];
            for (int i = 0; i < size; i++) {
                values[i] = 0.0; // dummy
            }
            return values;
        }
        return new Double[0];
    }

    public JPanel getContentPane() {
        return content;
    }

    static final Color LIVE_POINT_COLOR = Surface3DView.LIVE_POINT_COLOR;

    /** Grid lines of the curve plot. */
    public static Color gridColor() {
        return TuningStyle.color("Console.Tuning.curve.grid", new Color(0xdfe3e9));
    }

    public class CurveCanvas extends JPanel {
        private CurveModel curve;
        private Double[] x;
        private Double[] y;

        private Integer draggingIndex = null;
        private boolean dragged = false;
        /** Where the drag started, and from the mouse to the grabbed point's centre. */
        private Point pressPoint;
        private Point grabOffset;

        public CurveCanvas() {
            // An empty JPanel reports a near-zero preferred width, so GridBagLayout has no canvas baseline
            // to defend while shrinking and the plot collapses to a sliver on small windows (issue #9635).
            // A real preferred size lets the 0.8 weight keep the canvas usefully wide as the window resizes.
            // (No minimumSize on purpose: with the no-horizontal-scroll container a minimum could push the
            // table off the right edge on a very narrow window; the preferred size already carries the fix.)
            setPreferredSize(new Dimension(400, 300));
            MouseAdapter mouseAdapter = new MouseAdapter() {
                @Override
                public void mousePressed(MouseEvent e) {
                    draggingIndex = readOnly ? null : findIndex(e.getPoint());
                    dragged = false;
                    if (draggingIndex != null) {
                        // The point moves by as much as the mouse does: snapping its centre to the
                        // mouse changed its X on a straight vertical drag.
                        pressPoint = e.getPoint();
                        Point centre = worldToCanvas(x[draggingIndex], y[draggingIndex]);
                        grabOffset = new Point(centre.x - pressPoint.x, centre.y - pressPoint.y);
                    }
                }

                @Override
                public void mouseReleased(MouseEvent e) {
                    if (dragged) {
                        writeBackToImage();
                        // Keep the transform stable for the whole gesture; fit only after release.
                        updatePlotAxes();
                        repaint();
                        table.repaint();
                        if (onEdit != null) onEdit.run();
                    }
                    draggingIndex = null;
                    dragged = false;
                }

                @Override
                public void mouseDragged(MouseEvent e) {
                    if (draggingIndex != null) {
                        updatePoint(draggingIndex, e.getPoint(), pressPoint, grabOffset);
                        dragged = true;
                        repaint();
                        table.repaint();
                    }
                }
            };
            addMouseListener(mouseAdapter);
            addMouseMotionListener(mouseAdapter);
        }

        /** The plot follows the theme, on the same surface as the 3D table view [tag:console_theme]. */
        @Override
        public Color getBackground() {
            return TuningStyle.color("Console.Tuning.surface.background", Color.WHITE);
        }

        private Color labelColor() {
            return TuningStyle.color("Console.Tuning.curve.label", new Color(0x586273));
        }

        private Color lineColor() {
            return TuningStyle.color("Console.Tuning.curve.line", new Color(0xc2410c));
        }

        public void setCurve(CurveModel curve, Double[] x, Double[] y) {
            this.curve = curve;
            this.x = x;
            this.y = y;
            repaint();
        }

        private Integer findIndex(Point p) {
            if (x == null || y == null) return null;
            for (int i = 0; i < x.length; i++) {
                Point pt = worldToCanvas(x[i], y[i]);
                if (pt.distance(p) < 10) {
                    return i;
                }
            }
            return null;
        }

        /** Moves a point by the drag; an axis the mouse has not moved along keeps its exact value. */
        private void updatePoint(int index, Point mouse, Point press, Point offset) {
            Point2D world = canvasToWorld(new Point(mouse.x + offset.x, mouse.y + offset.y));
            if (mouse.x != press.x) {
                setXValue(index, world.x);
            }
            if (mouse.y != press.y) {
                setYValue(index, world.y);
            }
        }

        @Override
        public void paintComponent(Graphics g) {
            super.paintComponent(g);
            if (curve == null) return;

            Graphics2D g2 = (Graphics2D) g;
            g2.setRenderingHint(RenderingHints.KEY_ANTIALIASING, RenderingHints.VALUE_ANTIALIAS_ON);

            drawGrid(g2);
            Graphics2D plotGraphics = (Graphics2D) g2.create();
            try {
                Rectangle plot = getPlotBounds();
                plotGraphics.clipRect(plot.x, plot.y, plot.width + 1, plot.height + 1);
                drawCurve(plotGraphics);
            } finally {
                plotGraphics.dispose();
            }

            Font font = g2.getFont();
            g2.setFont(font.deriveFont(Font.BOLD));
            g2.setColor(TuningStyle.color("Console.Tuning.curve.title", Color.DARK_GRAY));
            String title = curveTitle;
            int titleWidth = g2.getFontMetrics().stringWidth(title);
            g2.drawString(title, (getWidth() - titleWidth) / 2, 15);
            g2.setFont(font);

            g2.setColor(labelColor());
            if (xUnits != null && !xUnits.isEmpty()) {
                int unitsWidth = g2.getFontMetrics().stringWidth(xUnits);
                g2.drawString(xUnits, (getWidth() - unitsWidth) / 2, getHeight() - 5);
            }

            if (yUnits != null && !yUnits.isEmpty()) {
                int unitsWidth = g2.getFontMetrics().stringWidth(yUnits);
                AffineTransform transform = g2.getTransform();
                g2.rotate(-Math.PI / 2);
                g2.drawString(yUnits, -(getHeight() + unitsWidth) / 2, 12);
                g2.setTransform(transform);
            }
        }

        public void drawGrid(Graphics2D g2) {
            Color gridColor = gridColor();
            Color labelColor = labelColor();
            g2.setStroke(new BasicStroke(1f));

            FontMetrics fm = g2.getFontMetrics();

            // Draw vertical grid lines
            // Track the right edge of the last drawn x-axis label so that, on narrow canvases, we skip
            // labels that would overlap their neighbour (the grid lines themselves are always drawn).
            int lastXLabelRight = Integer.MIN_VALUE;
            for (int i = 0; i < xAxis.getStep(); i++) {
                double val = xAxis.getMin() + (xAxis.getMax() - xAxis.getMin()) * i / (xAxis.getStep() - 1);
                Point p1 = worldToCanvas(val, yAxis.getMin());
                Point p2 = worldToCanvas(val, yAxis.getMax());

                g2.setColor(gridColor);
                g2.drawLine(p1.x, p1.y, p2.x, p2.y);

                // Locale.ROOT so the decimal separator is always a dot (e.g. "0.00", not "0,00" on es_* systems).
                String label = String.format(Locale.ROOT, "%." + xDigits + "f", val);
                int labelWidth = fm.stringWidth(label);
                int labelLeft = p1.x - labelWidth / 2;
                if (labelLeft > lastXLabelRight) {
                    g2.setColor(labelColor);
                    g2.drawString(label, labelLeft, p1.y + fm.getAscent() + 2);
                    lastXLabelRight = labelLeft + labelWidth + 4; // 4px minimum gap between labels
                }
            }

            // Draw horizontal grid lines
            for (int i = 0; i < yAxis.getStep(); i++) {
                double val = yAxis.getMin() + (yAxis.getMax() - yAxis.getMin()) * i / (yAxis.getStep() - 1);
                Point p1 = worldToCanvas(xAxis.getMin(), val);
                Point p2 = worldToCanvas(xAxis.getMax(), val);

                g2.setColor(gridColor);
                g2.drawLine(p1.x, p1.y, p2.x, p2.y);

                g2.setColor(labelColor);
                String label = String.format(Locale.ROOT, "%." + yDigits + "f", val);
                int labelWidth = fm.stringWidth(label);
                g2.drawString(label, p1.x - labelWidth - 5, p1.y + fm.getAscent() / 2);
            }
            g2.setStroke(new BasicStroke(1.0f));
        }

        private void drawCurve(Graphics2D g2) {
            if (x == null || y == null || x.length == 0) return;

            Color line = lineColor();
            Color background = getBackground();
            Stroke stroke = g2.getStroke();
            g2.setStroke(new BasicStroke(2f, BasicStroke.CAP_ROUND, BasicStroke.JOIN_ROUND));
            g2.setColor(line);
            for (int i = 0; i < x.length - 1; i++) {
                Point p1 = worldToCanvas(x[i], y[i]);
                Point p2 = worldToCanvas(x[i + 1], y[i + 1]);
                g2.drawLine(p1.x, p1.y, p2.x, p2.y);
            }

            // Points are hollow rings on the line, the handles to drag.
            for (int i = 0; i < x.length; i++) {
                Point p = worldToCanvas(x[i], y[i]);
                g2.setColor(background);
                g2.fillOval(p.x - 4, p.y - 4, 8, 8);
                g2.setColor(line);
                g2.drawOval(p.x - 4, p.y - 4, 8, 8);
            }
            g2.setStroke(stroke);

            // Selected points in the selection colour, as in tables, ringed in the plot colour to stand off the line.
            Color selection = CalibrationColorPalette.selection();
            for (int row : table.getSelectedRows()) {
                Point p = worldToCanvas(x[row], y[row]);
                g2.setColor(background);
                g2.fillOval(p.x - 7, p.y - 7, 14, 14);
                g2.setColor(selection);
                g2.fillOval(p.x - 5, p.y - 5, 10, 10);
            }
            drawLivePoint(g2);
        }

        /** A vertical line at the live X value and a dot where the firmware interpolates the curve. */
        private void drawLivePoint(Graphics2D g2) {
            Double position = livePosition();
            if (position == null) {
                return;
            }
            int from = (int) Math.floor(position);
            int to = Math.min(from + 1, Math.min(x.length, y.length) - 1);
            double fraction = position - from;
            double liveX = x[from] + fraction * (x[to] - x[from]);
            double liveY = y[from] + fraction * (y[to] - y[from]);
            Point p = worldToCanvas(liveX, liveY);
            Rectangle plot = getPlotBounds();
            Stroke stroke = g2.getStroke();
            g2.setColor(labelColor());
            g2.setStroke(new BasicStroke(1f, BasicStroke.CAP_BUTT, BasicStroke.JOIN_MITER, 10f, new float[]{4f, 4f}, 0f));
            g2.drawLine(p.x, plot.y, p.x, plot.y + plot.height);
            // The live point looks as in the 3D view: white ringed in black, visible in every theme.
            g2.setStroke(new BasicStroke(2f));
            g2.setColor(LIVE_POINT_COLOR);
            g2.fillOval(p.x - 6, p.y - 6, 12, 12);
            g2.setColor(Color.BLACK); // [tag:fixed_color] live point outline, as in the 3D view
            g2.drawOval(p.x - 6, p.y - 6, 12, 12);
            g2.setStroke(stroke);
        }

        private Rectangle getPlotBounds() {
            return new Rectangle(50, 20, Math.max(1, getWidth() - 70), Math.max(1, getHeight() - 60));
        }

        // Package-private for rendering and mouse-interaction tests.
        Point worldToCanvas(double wx, double wy) {
            double xMin = xAxis.getMin();
            double xMax = xAxis.getMax();
            double yMin = yAxis.getMin();
            double yMax = yAxis.getMax();

            Rectangle plot = getPlotBounds();
            int cx = plot.x + (int) ((wx - xMin) / (xMax - xMin) * plot.width);
            // y axis is displayed in descending order
            int cy = plot.y + plot.height - (int) ((wy - yMin) / (yMax - yMin) * plot.height);

            return new Point(cx, cy);
        }

        private Point2D canvasToWorld(Point p) {
            double xMin = xAxis.getMin();
            double xMax = xAxis.getMax();
            double yMin = yAxis.getMin();
            double yMax = yAxis.getMax();

            Rectangle plot = getPlotBounds();
            double wx = xMin + (double) (p.x - plot.x) / plot.width * (xMax - xMin);
            double wy = yMin + (double) (plot.y + plot.height - p.y) / plot.height * (yMax - yMin);

            return new Point2D(wx, wy);
        }

        private class Point2D {
            double x, y;
            Point2D(double x, double y) { this.x = x; this.y = y; }
        }
    }

    private class CurveTableModel extends AbstractTableModel {
        @Override
        public int getRowCount() {
            return xValues == null ? 0 : xValues.length;
        }

        @Override
        public int getColumnCount() {
            return 2;
        }

        @Override
        public String getColumnName(int column) {
            String label = column == 0 ? xLabel : yLabel;
            return label != null && !label.isEmpty() ? label : (column == 0 ? "X Axis" : "Y Axis");
        }

        @Override
        public Object getValueAt(int rowIndex, int columnIndex) {
            double val = columnIndex == 0 ? xValues[rowIndex] : yValues[rowIndex];
            int digits = columnIndex == 0 ? xDigits : yDigits;
            return formatValue(val, digits);
        }

        @Override
        public boolean isCellEditable(int rowIndex, int columnIndex) {
            return !readOnly && !(columnIndex == 0 && xBinsFixed);
        }

        @Override
        public void setValueAt(Object aValue, int rowIndex, int columnIndex) {
            if (readOnly) {
                return;
            }
            try {
                double val = DecimalInput.parse(aValue.toString());
                if (!Double.isFinite(val)) {
                    return;
                }
                for (int row : table.getSelectedRows()) {
                    for (int column : table.getSelectedColumns()) {
                        setValue(row, column, val);
                    }
                }
                updatePlotAxes();
                fireTableDataChanged();
                canvas.repaint();
                writeBackToImage();
                if (onEdit != null) onEdit.run();
            } catch (NumberFormatException ignored) {}
        }

        private void setValue(int row, int column, double value) {
            if (column == 0) {
                setXValue(row, value);
            } else {
                setYValue(row, value);
            }
        }
    }

    private class GradientRenderer extends DefaultTableCellRenderer {
        GradientRenderer() {
            // centred, like table values
            setHorizontalAlignment(SwingConstants.CENTER);
        }

        @Override
        public Component getTableCellRendererComponent(JTable table, Object value, boolean isSelected, boolean hasFocus, int row, int column) {
            Component c = renderCell(table, value, isSelected, hasFocus, row, column);
            // [tag:unburned_marks] after the live-row outline, which replaces the border
            UnburnedCellMark.apply(c, String.valueOf(value), burnedCellText(row, column), unburnedWording.get());
            return c;
        }

        private Component renderCell(JTable table, Object value, boolean isSelected, boolean hasFocus, int row, int column) {
            Component c = super.getTableCellRendererComponent(table, value, isSelected, hasFocus, row, column);
            if (row == liveRow()) {
                // Black stays visible on the light and the dark value gradient alike.
                setBorder(BorderFactory.createLineBorder(Color.BLACK, 2)); // [tag:fixed_color] live row outline
            }
            double doubleValue;
            if (value instanceof Double) {
                doubleValue = (Double) value;
            } else {
                try {
                    doubleValue = Double.parseDouble(value.toString());
                } catch (NumberFormatException e) {
                    return c;
                }
            }

            double min, max;
            if (column == 0) {
                min = xAxis.getMin();
                max = xAxis.getMax();
            } else {
                min = yAxis.getMin();
                max = yAxis.getMax();
            }
            Color background = CalibrationColorPalette.background(doubleValue, min, max);
            if (isSelected) {
                // As in tables: the value colour under the selection tint, an outline around the block.
                background = CalibrationColorPalette.selected(background);
                setBorder(BorderFactory.createCompoundBorder(
                    CalibrationColorPalette.selectionEdges(table, row, column, 0), getBorder()));
            }
            c.setBackground(background);
            c.setForeground(CalibrationColorPalette.foreground(background));
            return c;
        }
    }
}
