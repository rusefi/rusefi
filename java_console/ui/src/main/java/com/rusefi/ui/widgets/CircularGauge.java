package com.rusefi.ui.widgets;

import com.opensr5.ConfigurationImage;
import com.opensr5.ini.ExpressionEvaluator;
import com.opensr5.ini.GaugeModel;
import com.opensr5.ini.IniFileModel;
import com.opensr5.ini.IniValue;

import javax.swing.JComponent;
import java.awt.*;
import java.awt.geom.AffineTransform;
import java.awt.geom.Arc2D;
import java.awt.geom.Ellipse2D;
import java.awt.geom.Line2D;
import java.awt.geom.Point2D;
import java.awt.image.BufferedImage;
import java.awt.print.PrinterGraphics;
import java.util.Locale;
import java.util.Objects;

/**
 * Swing rendering of lvgl-linux-dash's classic circular gauge, restyled with a
 * light dial to match the console's light theme. Geometry follows
 * ui/components/ui_comp_gaugecontainer.c; the shared needle/arc position follows
 * ui/gauge_display_value.c. Readings round to the .ini value decimals, as in TS,
 * not to the dash's range-based step, which showed e.g. a 0.45 V nernst as 0.550.
 * Draw in a 400-unit square, scaled to fit the component without stretching.
 */
public final class CircularGauge extends JComponent implements GaugeDisplay {
    private static final int START_DEGREES = 135;
    private static final int SWEEP_DEGREES = 270;
    // Light dial matching the console's FlatLaf light theme.
    static final Color PROGRESS = new Color(0xFCE38A);
    static final Color DANGER_FILL = new Color(0xFFCDD2);
    static final Color DANGER = new Color(0xE53935);
    // Amber rather than yellow: the normal progress fill is already yellow.
    static final Color WARNING = new Color(0xFFA000);
    static final Color WARNING_FILL = new Color(0xFFCC80);
    static final Color WARNING_TEXT = new Color(0xE65100);
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
    // Raw .ini limits (NaN when absent); updateZones() derives the drawn, ordered thresholds.
    private final double lowDangerValue;
    private final double lowWarningValue;
    private final double highWarningValue;
    private double lowDangerThreshold;
    private double lowWarningThreshold;
    private double highWarningThreshold;
    private String title;
    private String units;
    // Sensor callbacks can run off the EDT. Publishing one immutable reading
    // keeps the text, color, arc and needle consistent; repaint() is thread-safe.
    private volatile Reading reading;
    private int artworkRenderCountForTests;
    private BufferedImage artworkCache;
    private int cacheWidth;
    private int cacheHeight;
    private double cacheScaleX;
    private double cacheScaleY;
    private double cachePhaseX;
    private double cachePhaseY;
    private Reading cacheReading;
    private String cacheTitle;
    private String cacheUnits;
    private double cacheDangerThreshold;
    private double cacheLowDangerThreshold;
    private double cacheLowWarningThreshold;
    private double cacheHighWarningThreshold;

    public CircularGauge(GaugeModel model, IniFileModel ini, ConfigurationImage configuration) {
        this(GaugeScale.fromIni(model, ini, configuration), model);
    }

    public CircularGauge(double minValue, double maxValue, GaugeModel model) {
        this(GaugeScale.of(minValue, maxValue,
            numeric(model.getLowDangerValueValue()), numeric(model.getLowWarningValueValue()),
            numeric(model.getHighWarningValueValue()), numeric(model.getHighDangerValueValue()),
            model.getValueDecimalPlaces(), model.getLabelDecimalPlaces()), model);
    }

    public CircularGauge(GaugeScale scale, GaugeModel model) {
        minValue = scale.getMin();
        maxValue = scale.getMax();
        valueDecimals = scale.getValueDecimals();
        displayStep = Math.pow(10, -valueDecimals);
        title = model.getTitle();
        units = model.getUnits();
        for (int i = 0; i < scaleLabels.length; i++) {
            scaleLabels[i] = format(minValue + (maxValue - minValue) * i / 10, scale.getLabelDecimals());
        }
        lowDangerValue = scale.getLowDanger();
        lowWarningValue = scale.getLowWarning();
        highWarningValue = scale.getHighWarning();
        reading = new Reading(Double.NaN, Double.NaN);
        setHighDangerValue(scale.getHighDanger());
        setPreferredSize(new Dimension(200, 200));
        setMinimumSize(new Dimension(80, 80));
        setOpaque(false);
    }

    static double resolve(IniValue value, IniFileModel ini, ConfigurationImage configuration, double fallback) {
        if (!value.isExpression()) {
            return value.getNumericValue();
        }
        Double evaluated = ini == null ? null : ExpressionEvaluator.evaluateNumericExpression(value.getRawString(), ini, configuration);
        return evaluated != null && Double.isFinite(evaluated) ? evaluated : fallback;
    }

    private static double numeric(IniValue value) {
        return value.isExpression() ? Double.NaN : value.getNumericValue();
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
        updateZones();
        repaint();
    }

    /**
     * Orders the TS limits min <= lowDanger <= lowWarning <= highWarning <= highDanger <= max,
     * so a malformed definition still draws non-overlapping zones. A missing limit leaves its
     * zone empty.
     */
    private void updateZones() {
        highWarningThreshold = Double.isFinite(highWarningValue)
            ? Math.max(minValue, Math.min(dangerThreshold, highWarningValue)) : dangerThreshold;
        lowDangerThreshold = Double.isFinite(lowDangerValue)
            ? Math.max(minValue, Math.min(highWarningThreshold, lowDangerValue)) : minValue;
        lowWarningThreshold = Double.isFinite(lowWarningValue)
            ? Math.max(lowDangerThreshold, Math.min(highWarningThreshold, lowWarningValue)) : lowDangerThreshold;
    }

    enum Zone {NORMAL, WARNING, DANGER}

    /**
     * High zones include their threshold, as the end-of-scale danger zone always has; low zones
     * exclude it, so a limit equal to the minimum (common in the .ini) adds no zone. Readings
     * below the minimum are not flagged: TS shows a disconnected 0 AFR or 0 C sensor
     * temperature, both below their scales, on a normal dial.
     */
    Zone zoneOf(double value, double tolerance) {
        if (!Double.isFinite(value)) {
            return Zone.NORMAL;
        }
        if (value >= dangerThreshold - tolerance) {
            return Zone.DANGER;
        }
        if (value < minValue) {
            return Zone.NORMAL;
        }
        if (value < lowDangerThreshold - tolerance) {
            return Zone.DANGER;
        }
        if (value >= highWarningThreshold - tolerance || value < lowWarningThreshold - tolerance) {
            return Zone.WARNING;
        }
        return Zone.NORMAL;
    }

    Zone getZone() {
        return zoneOf(reading.rawValue, 0);
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
            double steps = value / displayStep;
            // Ties round away from zero, so negative readings mirror positive ones.
            displayValue = Math.copySign(Math.floor(Math.abs(steps) + 0.5), steps) * displayStep;
        }
        Reading previous = reading;
        if (Double.compare(previous.value, displayValue) != 0
            || zoneOf(previous.rawValue, 0) != zoneOf(value, 0)) {
            reading = new Reading(displayValue, value);
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
        /**
         * Unrounded reading, which selects the zone: rounding to the displayed decimals could
         * otherwise move a reading just below a limit across it.
         */
        final double rawValue;
        final String text;
        final int degrees;

        Reading(double value, double rawValue) {
            this.value = value;
            this.rawValue = rawValue;
            text = Double.isFinite(value) ? format(value, valueDecimals) : "---";
            double visualValue = Math.max(minValue, Math.min(maxValue, value));
            degrees = Double.isFinite(value)
                ? (int) Math.round((visualValue - minValue) / (maxValue - minValue) * SWEEP_DEGREES) : 0;
        }
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
        Shape incomingClip = graphics.getClip();
        boolean hasPartialClip = incomingClip != null && !incomingClip.contains(0, 0, getWidth(), getHeight());
        Graphics2D g = (Graphics2D) graphics.create();
        try {
            g.translate(insets.left + (width - size) / 2.0, insets.top + (height - size) / 2.0);
            g.scale(size / 400.0, size / 400.0);
            g.setRenderingHint(RenderingHints.KEY_ANTIALIASING, RenderingHints.VALUE_ANTIALIAS_ON);
            g.setRenderingHint(RenderingHints.KEY_TEXT_ANTIALIASING, RenderingHints.VALUE_TEXT_ANTIALIAS_ON);
            // Clip-edge antialiasing composes differently when the whole dial is rasterized
            // first and then clipped as an image, so preserve vector rendering for partial paints.
            if (hasPartialClip) {
                paintVectorArtwork(g, reading);
                return;
            }
            AffineTransform artworkTransform = g.getTransform();
            double scaleX = artworkTransform.getScaleX();
            double scaleY = artworkTransform.getScaleY();
            double originX = artworkTransform.getTranslateX();
            double originY = artworkTransform.getTranslateY();
            double phaseX = originX - Math.floor(originX);
            double phaseY = originY - Math.floor(originY);
            double physicalWidth = 400 * scaleX + phaseX;
            double physicalHeight = 400 * scaleY + phaseY;
            if (graphics instanceof PrinterGraphics || !Double.isFinite(scaleX) || !Double.isFinite(scaleY)
                || !Double.isFinite(originX) || !Double.isFinite(originY) || scaleX <= 0 || scaleY <= 0
                || artworkTransform.getShearX() != 0 || artworkTransform.getShearY() != 0
                || !Double.isFinite(physicalWidth) || !Double.isFinite(physicalHeight)
                || physicalWidth <= 0 || physicalHeight <= 0 || physicalWidth > 4096 || physicalHeight > 4096) {
                paintVectorArtwork(g, reading);
                return;
            }
            int rasterWidth = (int) Math.ceil(physicalWidth);
            int rasterHeight = (int) Math.ceil(physicalHeight);
            if ((long) rasterWidth * rasterHeight > 16_777_216) {
                paintVectorArtwork(g, reading);
                return;
            }
            Reading current = reading;
            // XRender makes these gradients and antialiased shapes expensive on every repaint.
            // Rasterize once per visible state, retaining device-pixel phase for sharp HiDPI text.
            if (!cacheMatches(rasterWidth, rasterHeight, scaleX, scaleY, phaseX, phaseY, current)) {
                artworkCache = new BufferedImage(rasterWidth, rasterHeight, BufferedImage.TYPE_INT_ARGB_PRE);
                Graphics2D raster = artworkCache.createGraphics();
                try {
                    raster.setRenderingHint(RenderingHints.KEY_ANTIALIASING, RenderingHints.VALUE_ANTIALIAS_ON);
                    raster.setRenderingHint(RenderingHints.KEY_TEXT_ANTIALIASING, RenderingHints.VALUE_TEXT_ANTIALIAS_ON);
                    raster.setTransform(new AffineTransform(scaleX, 0, 0, scaleY, phaseX, phaseY));
                    paintVectorArtwork(raster, current);
                } finally {
                    raster.dispose();
                }
                cacheWidth = rasterWidth;
                cacheHeight = rasterHeight;
                cacheScaleX = scaleX;
                cacheScaleY = scaleY;
                cachePhaseX = phaseX;
                cachePhaseY = phaseY;
                cacheReading = current;
                cacheTitle = title;
                cacheUnits = units;
                cacheDangerThreshold = dangerThreshold;
                cacheLowDangerThreshold = lowDangerThreshold;
                cacheLowWarningThreshold = lowWarningThreshold;
                cacheHighWarningThreshold = highWarningThreshold;
            }
            try {
                AffineTransform alignToDevicePixels = artworkTransform.createInverse();
                alignToDevicePixels.translate(Math.floor(originX), Math.floor(originY));
                g.transform(alignToDevicePixels);
                g.drawImage(artworkCache, 0, 0, null);
            } catch (java.awt.geom.NoninvertibleTransformException impossibleForPositiveScale) {
                paintVectorArtwork(g, current);
            }
        } finally {
            g.dispose();
        }
    }

    private boolean cacheMatches(int width, int height, double scaleX, double scaleY,
                                 double phaseX, double phaseY, Reading current) {
        return artworkCache != null && cacheWidth == width && cacheHeight == height
            && Double.compare(cacheScaleX, scaleX) == 0 && Double.compare(cacheScaleY, scaleY) == 0
            && Double.compare(cachePhaseX, phaseX) == 0 && Double.compare(cachePhaseY, phaseY) == 0
            && cacheReading == current
            && Objects.equals(cacheTitle, title) && Objects.equals(cacheUnits, units)
            && Double.compare(cacheDangerThreshold, dangerThreshold) == 0
            && Double.compare(cacheLowDangerThreshold, lowDangerThreshold) == 0
            && Double.compare(cacheLowWarningThreshold, lowWarningThreshold) == 0
            && Double.compare(cacheHighWarningThreshold, highWarningThreshold) == 0;
    }

    private void paintVectorArtwork(Graphics2D g, Reading current) {
        artworkRenderCountForTests++;
        Zone zone = zoneOf(current.rawValue, 0);
        Color fill = zone == Zone.DANGER ? DANGER_FILL : zone == Zone.WARNING ? WARNING_FILL : PROGRESS;

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
        double lowDangerEnd = degreesOf(lowDangerThreshold);
        double lowWarningEnd = degreesOf(lowWarningThreshold);
        double highWarningStart = degreesOf(highWarningThreshold);
        double dangerStart = degreesOf(dangerThreshold);
        drawArc(g, 184, 6, 0, lowDangerEnd, DANGER);
        drawArc(g, 184, 6, lowDangerEnd, lowWarningEnd - lowDangerEnd, WARNING);
        drawArc(g, 184, 6, highWarningStart, dangerStart - highWarningStart, WARNING);
        drawArc(g, 184, 6, dangerStart, SWEEP_DEGREES - dangerStart, DANGER);

        for (int i = 0; i <= 10; i++) {
            double value = minValue + (maxValue - minValue) * i / 10;
            Zone labelZone = zoneOf(value, 0.001);
            Color color = labelZone == Zone.DANGER ? DANGER : labelZone == Zone.WARNING ? WARNING_TEXT : SCALE;
            double angle = Math.toRadians(START_DEGREES + i * SWEEP_DEGREES / 10.0);
            g.setColor(labelZone == Zone.DANGER ? DANGER : labelZone == Zone.WARNING ? WARNING : TICK);
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
        drawText(g, current.text, 200, 200, 60, 165,
            zone == Zone.DANGER ? DANGER : zone == Zone.WARNING ? WARNING_TEXT : TEXT);
        drawText(g, units, 202, 246, 16, 160, LABEL_TEXT);
        drawText(g, title, 200, 331, 16, 230, LABEL_TEXT);
    }

    int getArtworkRenderCountForTests() {
        return artworkRenderCountForTests;
    }

    void paintVectorForTests(Graphics2D graphics) {
        int width = getWidth();
        int height = getHeight();
        int size = Math.min(width, height);
        graphics.translate((width - size) / 2.0, (height - size) / 2.0);
        graphics.scale(size / 400.0, size / 400.0);
        graphics.setRenderingHint(RenderingHints.KEY_ANTIALIASING, RenderingHints.VALUE_ANTIALIAS_ON);
        graphics.setRenderingHint(RenderingHints.KEY_TEXT_ANTIALIASING, RenderingHints.VALUE_TEXT_ANTIALIAS_ON);
        paintVectorArtwork(graphics, reading);
    }

    private double degreesOf(double value) {
        return (value - minValue) / (maxValue - minValue) * SWEEP_DEGREES;
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
