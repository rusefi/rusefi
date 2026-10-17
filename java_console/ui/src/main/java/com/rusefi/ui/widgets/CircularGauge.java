package com.rusefi.ui.widgets;

import com.opensr5.ConfigurationImage;
import com.opensr5.ini.ExpressionEvaluator;
import com.opensr5.ini.GaugeModel;
import com.opensr5.ini.IniFileModel;
import com.opensr5.ini.IniValue;

import javax.swing.JComponent;
import java.awt.*;
import java.awt.geom.Arc2D;
import java.awt.geom.Ellipse2D;
import java.awt.geom.Line2D;
import java.util.Locale;
import java.util.Objects;

/**
 * Swing rendering of lvgl-linux-dash's classic circular gauge.
 * Geometry and colors follow ui/components/ui_comp_gaugecontainer.c; display
 * rounding and the shared needle/arc position follow ui/gauge_display_value.c.
 * Draw in a 400-unit square, scaled to fit the component without stretching.
 */
public final class CircularGauge extends JComponent {
    private static final int START_DEGREES = 135;
    private static final int SWEEP_DEGREES = 270;
    private static final Color PROGRESS = new Color(0xD0E128);
    private static final Color DANGER = new Color(0xFF0000);
    private static final Color NEEDLE = new Color(0xC60000);
    private static final Color SCALE = new Color(0x717171);
    private static final Color RING = new Color(0x394041);
    private static final Color HUB = new Color(0x202020);
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
            Color fill = danger ? DANGER : PROGRESS;

            g.setPaint(new GradientPaint(0, 0, new Color(0x1D1D1D), 0, 314, new Color(0x151515)));
            g.fill(new Ellipse2D.Double(1.5, 1.5, 397, 397));
            g.setColor(RING);
            g.setStroke(new BasicStroke(3));
            g.draw(new Ellipse2D.Double(1.5, 1.5, 397, 397));

            drawArc(g, 141, 100, 0, current.degrees, fill);
            drawArc(g, 194.5, 7, 0, SWEEP_DEGREES, RING);
            double dangerStart = (dangerThreshold - minValue) / (maxValue - minValue) * SWEEP_DEGREES;
            drawArc(g, 194.5, 7, dangerStart, SWEEP_DEGREES - dangerStart, DANGER);

            for (int i = 0; i <= 10; i++) {
                double value = minValue + (maxValue - minValue) * i / 10;
                Color color = value >= dangerThreshold - 0.001 ? new Color(0xB70000) : SCALE;
                if (current.degrees > i * SWEEP_DEGREES / 10 && contrast(color, fill) < 3) {
                    color = new Color(fill.getRed() * 90 / 255, fill.getGreen() * 90 / 255, fill.getBlue() * 90 / 255);
                }
                double angle = Math.toRadians(START_DEGREES + i * SWEEP_DEGREES / 10.0);
                g.setColor(color);
                g.setStroke(new BasicStroke(2));
                double inner = i % 2 == 0 ? 180 : 183;
                g.draw(new Line2D.Double(200 + inner * Math.cos(angle), 200 + inner * Math.sin(angle),
                    200 + 191 * Math.cos(angle), 200 + 191 * Math.sin(angle)));
                drawText(g, scaleLabels[i], 200 + LABEL_X[i], 200 + LABEL_Y[i], i % 2 == 0 ? 18 : 16, 62, color);
            }

            // The needle and fill share the same rounded angle. The hub covers
            // the needle's inner end; labels and the value stay above the needle.
            double angle = Math.toRadians(START_DEGREES + current.degrees);
            g.setColor(NEEDLE);
            g.setStroke(new BasicStroke(4, BasicStroke.CAP_BUTT, BasicStroke.JOIN_MITER));
            g.draw(new Line2D.Double(200 + 90 * Math.cos(angle), 200 + 90 * Math.sin(angle),
                200 + 180 * Math.cos(angle), 200 + 180 * Math.sin(angle)));
            g.setColor(HUB);
            g.fill(new Ellipse2D.Double(100, 100, 200, 200));
            drawArc(g, 96.5, 7, 0, SWEEP_DEGREES, new Color(0x384244));
            drawArc(g, 99.5, 1, 0, SWEEP_DEGREES, new Color(0, 0, 0, 100));
            drawText(g, current.text, 200, 200, 60, 165, danger ? DANGER : Color.WHITE);
            drawText(g, units, 202, 246, 16, 160, Color.WHITE);
            drawText(g, title, 200, 331, 16, 230, Color.WHITE);
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

    private static double contrast(Color a, Color b) {
        double first = luminance(a) + 0.05;
        double second = luminance(b) + 0.05;
        return Math.max(first, second) / Math.min(first, second);
    }

    private static double luminance(Color color) {
        return 0.2126 * linear(color.getRed()) + 0.7152 * linear(color.getGreen()) + 0.0722 * linear(color.getBlue());
    }

    private static double linear(int channel) {
        double c = channel / 255.0;
        return c <= 0.04045 ? c / 12.92 : Math.pow((c + 0.055) / 1.055, 2.4);
    }
}
