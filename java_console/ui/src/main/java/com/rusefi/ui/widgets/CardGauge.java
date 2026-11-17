package com.rusefi.ui.widgets;

import com.opensr5.ini.GaugeModel;

import javax.swing.JComponent;
import java.awt.*;
import java.awt.geom.RoundRectangle2D;
import java.util.Locale;
import java.util.Objects;

/**
 * Base of the card-style gauges (text and bars): range, danger threshold and value
 * formatting from a {@link GaugeScale}, painted on a {@link GaugeStyle} card.
 */
abstract class CardGauge extends JComponent implements GaugeDisplay {
    final double minValue;
    final double maxValue;
    final double dangerThreshold;
    private final int valueDecimals;
    private final int labelDecimals;
    private String title;
    private String units;
    // Sensor callbacks can run off the EDT; repaint() is thread-safe.
    private volatile double value = Double.NaN;

    CardGauge(GaugeScale scale, GaugeModel model) {
        minValue = scale.getMin();
        maxValue = scale.getMax();
        double danger = scale.getHighDanger();
        // Same rule as the round gauge: without a danger value the zone starts at the maximum.
        dangerThreshold = Double.isFinite(danger) ? Math.max(minValue, Math.min(maxValue, danger)) : maxValue;
        valueDecimals = scale.getValueDecimals();
        labelDecimals = scale.getLabelDecimals();
        title = model.getTitle();
        units = model.getUnits();
        setOpaque(false);
    }

    @Override
    public void setValue(double value) {
        if (Double.compare(this.value, value) != 0) {
            this.value = value;
            repaint();
        }
    }

    public double getValue() {
        return value;
    }

    @Override
    public void setTitle(String title) {
        if (!Objects.equals(this.title, title)) {
            this.title = title;
            repaint();
        }
    }

    public String getTitle() {
        return title;
    }

    @Override
    public void setUnitString(String units) {
        if (!Objects.equals(this.units, units)) {
            this.units = units;
            repaint();
        }
    }

    public String getUnitString() {
        return units;
    }

    String getDisplayText() {
        return text(value);
    }

    String text(double value) {
        return Double.isFinite(value) ? format(value, valueDecimals) : "---";
    }

    String label(double value) {
        return format(value, labelDecimals);
    }

    boolean isDanger(double value) {
        return Double.isFinite(value) && value >= dangerThreshold;
    }

    /** Share of the range covered by {@code value}, 0 when there is no reading. */
    double fraction(double value) {
        if (!Double.isFinite(value)) {
            return 0;
        }
        return Math.max(0, Math.min(1, (value - minValue) / (maxValue - minValue)));
    }

    double dangerFraction() {
        return (dangerThreshold - minValue) / (maxValue - minValue);
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
        if (width < 20 || height < 20) {
            return;
        }
        Graphics2D g = (Graphics2D) graphics.create();
        try {
            g.translate(insets.left, insets.top);
            g.setRenderingHint(RenderingHints.KEY_ANTIALIASING, RenderingHints.VALUE_ANTIALIAS_ON);
            g.setRenderingHint(RenderingHints.KEY_TEXT_ANTIALIASING, RenderingHints.VALUE_TEXT_ANTIALIAS_ON);
            g.setRenderingHint(RenderingHints.KEY_FRACTIONALMETRICS, RenderingHints.VALUE_FRACTIONALMETRICS_ON);
            RoundRectangle2D.Double face = GaugeStyle.paintCard(g, width, height);
            paintContent(g, face, value);
        } finally {
            g.dispose();
        }
    }

    /** Paints the presentation inside the card {@code face} for one consistent {@code value}. */
    abstract void paintContent(Graphics2D g, RoundRectangle2D.Double face, double value);
}
