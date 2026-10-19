package com.rusefi.ui.widgets;

import com.opensr5.ConfigurationImage;
import com.opensr5.ini.ExpressionEvaluator;
import com.opensr5.ini.GaugeModel;
import com.opensr5.ini.IniFileModel;
import com.opensr5.ini.IniValue;

import javax.swing.JComponent;
import java.awt.*;
import java.awt.geom.Arc2D;
import java.awt.geom.Point2D;
import java.awt.geom.Ellipse2D;
import java.awt.geom.Line2D;
import java.util.Locale;
import java.util.Objects;

/**
 * Swing rendering of lvgl-linux-dash's classic circular gauge, restyled with a
 * light dial to match the console's light theme. Geometry follows
 * ui/components/ui_comp_gaugecontainer.c; display rounding and the shared
 * needle/arc position follow ui/gauge_display_value.c.
 * Draw in a 400-unit square, scaled to fit the component without stretching.
 */
public final class CircularGauge extends JComponent {
    private static final int START_DEGREES = 135;
    private static final int SWEEP_DEGREES = 270;
    // Light dial matching the console's FlatLaf light theme.
    static final Color PROGRESS = new Color(0xFCE38A);
    static final Color DANGER_FILL = new Color(0xFFCDD2);
    private static final Color DANGER = new Color(0xE53935);
    static final Color NEEDLE = new Color(0xE53935);
    private static final Color SCALE = new Color(0x5F656D);
    private static final Color TICK = new Color(0x9AA0A6);
    private static final Color TRACK = new Color(0xE3E5E8);
    private static final Color BEZEL_TOP = new Color(0xFBFBFC);
    private static final Color BEZEL_BOTTOM = new Color(0xDADDE1);
    private static final Color FACE_CENTER = Color.WHITE;
    private static final Color FACE_EDGE = new Color(0xF3F4F6);
    static final Color HUB = Color.WHITE;
    private static final Color HUB_RIM = new Color(0xE1E4E8);
    private static final Color TEXT = new Color(0x111111);
    private static final Color LABEL_TEXT = new Color(0x2B2F33);
    private static final Font FONT = new Font(Font.SANS_SERIF, Font.PLAIN, 16);
    // Clockwise label centers from ui_gauge_resize(), relative to the dial center.
    private static final int[] LABEL_X = {-127, -169, -165, -139, -83, -1, 83, 139, 165, 163, 129};
    private static final int[] LABEL_Y = {112, 55, -26, -98, -146, -174, -146, -100, -25, 54, 105};

    private final double minValue;
    private final double maxValue;
    private final double displayStep;
    private final int valueDecimals;
    private final String[] scaleLabels = new String[11];
    private double dangerThreshold;
    private String title;
    private String units;
    // Sensor callbacks can run off the EDT. Publishing one immutable reading
    // keeps the text, color, arc and needle consistent; repaint() is thread-safe.
    private volatile Reading reading;

    public CircularGauge(GaugeModel model, IniFileModel ini, ConfigurationImage configuration) {
        this(resolve(model.getLowValueValue(), ini, configuration, 0),
            resolve(model.getHighValueValue(), ini, configuration, 0), model,
            resolve(model.getHighDangerValueValue(), ini, configuration, Double.NaN),
            (int) resolve(model.getValueDecimalPlacesValue(), ini, configuration, 0),
            (int) resolve(model.getLabelDecimalPlacesValue(), ini, configuration, 0));
    }

    public CircularGauge(double minValue, double maxValue, GaugeModel model) {
        this(minValue, maxValue, model,
            model.getHighDangerValueValue().isExpression() ? Double.NaN : model.getHighDangerValue(),
            model.getValueDecimalPlaces(), model.getLabelDecimalPlaces());
    }

    private CircularGauge(double minValue, double maxValue, GaugeModel model,
                          double highDangerValue, int valueDecimals, int labelDecimals) {
        if (!Double.isFinite(minValue)) {
            minValue = 0;
        }
        if (!Double.isFinite(maxValue) || maxValue <= minValue || !Double.isFinite(maxValue - minValue)) {
            maxValue = minValue + 10;
            if (!Double.isFinite(maxValue) || maxValue <= minValue) {
                minValue = 0;
                maxValue = 10;
            }
        }
        this.minValue = minValue;
        this.maxValue = maxValue;
        double maxAbs = Math.max(Math.abs(minValue), Math.abs(maxValue));
        displayStep = maxAbs > 150 ? 4 : maxAbs >= 75 ? 2 : maxAbs >= 40 ? 1 : 0.25;
        this.valueDecimals = boundedDecimals(valueDecimals);
        title = model.getTitle();
        units = model.getUnits();
        for (int i = 0; i < scaleLabels.length; i++) {
            scaleLabels[i] = format(minValue + (maxValue - minValue) * i / 10,
                boundedDecimals(labelDecimals));
        }
        reading = new Reading(Double.NaN);
        setHighDangerValue(highDangerValue);
        setPreferredSize(new Dimension(200, 200));
        setMinimumSize(new Dimension(80, 80));
        setOpaque(false);
    }

    private static double resolve(IniValue value, IniFileModel ini, ConfigurationImage configuration, double fallback) {
        if (!value.isExpression()) {
            return value.getNumericValue();
        }
        Double evaluated = ini == null ? null : ExpressionEvaluator.evaluateNumericExpression(value.getRawString(), ini, configuration);
        return evaluated != null && Double.isFinite(evaluated) ? evaluated : fallback;
    }

    /** Configuration/label setters are called on the EDT. */
    public void setHighDangerValue(double highDangerValue) {
        if (!Double.isFinite(highDangerValue)) {
            highDangerValue = maxValue;
        }
        // The dashboard stores its end-of-scale danger zone in whole percent.
        int dangerPercent = (int) Math.max(0, Math.min(100,
            (maxValue - highDangerValue) / (maxValue - minValue) * 100));
        dangerThreshold = maxValue - dangerPercent / 100.0 * (maxValue - minValue);
        repaint();
    }

    public void setTitle(String title) {
        if (!Objects.equals(this.title, title)) {
            this.title = title;
            repaint();
        }
    }

    public String getTitle() {
        return title;
    }

    public void setUnitString(String units) {
        if (!Objects.equals(this.units, units)) {
            this.units = units;
            repaint();
        }
    }

    public String getUnitString() {
        return units;
    }

    public void setValue(double value) {
        double displayValue = Double.NaN;
        if (Double.isFinite(value)) {
            double steps = (value - minValue) / displayStep;
            // C roundf rounds ties away from zero, unlike Math.round for negatives.
            displayValue = minValue + Math.copySign(Math.floor(Math.abs(steps) + 0.5), steps) * displayStep;
        }
        if (Double.compare(reading.value, displayValue) != 0) {
            reading = new Reading(displayValue);
            repaint();
        }
    }

    public double getValue() {
        return reading.value;
    }

    String getDisplayText() {
        return reading.text;
    }

    int getProgressDegrees() {
        return reading.degrees;
    }

    private final class Reading {
        final double value;
        final String text;
        final int degrees;

        Reading(double value) {
            this.value = value;
            text = Double.isFinite(value) ? format(value, valueDecimals) : "---";
            double visualValue = Math.max(minValue, Math.min(maxValue, value));
            degrees = Double.isFinite(value)
                ? (int) Math.round((visualValue - minValue) / (maxValue - minValue) * SWEEP_DEGREES) : 0;
        }
    }

    private static int boundedDecimals(int decimals) {
        return Math.max(0, Math.min(6, decimals));
    }

    private static String format(double value, int decimals) {
        return String.format(Locale.ROOT, "%." + decimals + "f", value == 0 ? 0 : value);
    }

    @Override
    protected void paintComponent(Graphics graphics) {
        super.paintComponent(graphics);
        Insets insets = getInsets();
        int width = getWidth() - insets.left - insets.right;
        int height = getHeight() - insets.top - insets.bottom;
        int size = Math.min(width, height);
        if (size <= 0) {
            return;
        }
        Graphics2D g = (Graphics2D) graphics.create();
        try {
            g.translate(insets.left + (width - size) / 2.0, insets.top + (height - size) / 2.0);
            g.scale(size / 400.0, size / 400.0);
            g.setRenderingHint(RenderingHints.KEY_ANTIALIASING, RenderingHints.VALUE_ANTIALIAS_ON);
            g.setRenderingHint(RenderingHints.KEY_TEXT_ANTIALIASING, RenderingHints.VALUE_TEXT_ANTIALIAS_ON);
            Reading current = reading;
            boolean danger = current.value >= dangerThreshold;
            Color fill = danger ? DANGER_FILL : PROGRESS;

            // Everything stays inside the dial circle: detached windows clip to it.
            g.setPaint(new GradientPaint(0, 0, BEZEL_TOP, 0, 400, BEZEL_BOTTOM));
            g.fill(new Ellipse2D.Double(1.5, 1.5, 397, 397));
            g.setColor(HUB_RIM);
            g.setStroke(new BasicStroke(1.5f));
            g.draw(new Ellipse2D.Double(1.5, 1.5, 397, 397));
            g.setPaint(new RadialGradientPaint(new Point2D.Double(200, 170), 200,
                new float[] {0.4f, 1f}, new Color[] {FACE_CENTER, FACE_EDGE}));
            g.fill(new Ellipse2D.Double(11, 11, 378, 378));

            drawArc(g, 140, 80, 0, current.degrees, fill);
            drawArc(g, 184, 4, 0, SWEEP_DEGREES, TRACK);
            double dangerStart = (dangerThreshold - minValue) / (maxValue - minValue) * SWEEP_DEGREES;
            drawArc(g, 184, 6, dangerStart, SWEEP_DEGREES - dangerStart, DANGER);

            for (int i = 0; i <= 10; i++) {
                double value = minValue + (maxValue - minValue) * i / 10;
                boolean dangerLabel = value >= dangerThreshold - 0.001;
                Color color = dangerLabel ? DANGER : SCALE;
                double angle = Math.toRadians(START_DEGREES + i * SWEEP_DEGREES / 10.0);
                g.setColor(dangerLabel ? DANGER : TICK);
                g.setStroke(new BasicStroke(2, BasicStroke.CAP_ROUND, BasicStroke.JOIN_MITER));
                double inner = i % 2 == 0 ? 168 : 174;
                g.draw(new Line2D.Double(200 + inner * Math.cos(angle), 200 + inner * Math.sin(angle),
                    200 + 181 * Math.cos(angle), 200 + 181 * Math.sin(angle)));
                drawText(g, scaleLabels[i], 200 + LABEL_X[i] * 0.84, 200 + LABEL_Y[i] * 0.84,
                    i % 2 == 0 ? 18 : 16, 62, color);
            }

            // The needle and fill share the same rounded angle. The hub covers
            // the needle's inner end; labels and the value stay above the needle.
            double angle = Math.toRadians(START_DEGREES + current.degrees);
            g.setColor(NEEDLE);
            g.setStroke(new BasicStroke(4, BasicStroke.CAP_ROUND, BasicStroke.JOIN_MITER));
            g.draw(new Line2D.Double(200 + 90 * Math.cos(angle), 200 + 90 * Math.sin(angle),
                200 + 178 * Math.cos(angle), 200 + 178 * Math.sin(angle)));
            // Soft drop shadow lifting the hub off the face.
            for (int i = 8; i >= 1; i--) {
                g.setColor(new Color(0, 0, 0, 5));
                g.fill(new Ellipse2D.Double(100 - i, 104 - i, 200 + 2 * i, 200 + 2 * i));
            }
            g.setPaint(new GradientPaint(0, 100, HUB, 0, 300, FACE_EDGE));
            g.fill(new Ellipse2D.Double(100, 100, 200, 200));
            g.setColor(HUB_RIM);
            g.setStroke(new BasicStroke(1.5f));
            g.draw(new Ellipse2D.Double(100, 100, 200, 200));
            drawText(g, current.text, 200, 200, 60, 165, danger ? DANGER : TEXT);
            drawText(g, units, 202, 246, 16, 160, LABEL_TEXT);
            drawText(g, title, 200, 331, 16, 230, LABEL_TEXT);
        } finally {
            g.dispose();
        }
    }

    private static void drawArc(Graphics2D g, double radius, float width, double start, double sweep, Color color) {
        if (sweep <= 0) {
            return;
        }
        g.setColor(color);
        g.setStroke(new BasicStroke(width, BasicStroke.CAP_BUTT, BasicStroke.JOIN_MITER));
        // Java2D angles increase counterclockwise; LVGL angles increase clockwise.
        g.draw(new Arc2D.Double(200 - radius, 200 - radius, radius * 2, radius * 2,
            -START_DEGREES - start, -sweep, Arc2D.OPEN));
    }

    private static void drawText(Graphics2D g, String text, double x, double y, float size, int maxWidth, Color color) {
        if (text == null || text.isEmpty()) {
            return;
        }
        g.setFont(FONT.deriveFont(size));
        int width = g.getFontMetrics().stringWidth(text);
        if (width > maxWidth) {
            g.setFont(FONT.deriveFont(size * maxWidth / width));
        }
        FontMetrics metrics = g.getFontMetrics();
        g.setColor(color);
        g.drawString(text, (float) (x - metrics.stringWidth(text) / 2.0),
            (float) (y + (metrics.getAscent() - metrics.getDescent()) / 2.0));
    }
}
