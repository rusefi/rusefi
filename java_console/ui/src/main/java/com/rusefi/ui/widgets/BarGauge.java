package com.rusefi.ui.widgets;

import com.opensr5.ConfigurationImage;
import com.opensr5.ini.GaugeModel;
import com.opensr5.ini.IniFileModel;

import java.awt.*;
import java.awt.geom.RoundRectangle2D;

/**
 * Bar presentation: a rounded track with the danger zone tinted and a fill that turns red
 * inside it. Horizontal bars put the value beside the title; vertical bars put it below.
 */
public final class BarGauge extends CardGauge {
    private final boolean vertical;

    public BarGauge(GaugeModel model, IniFileModel ini, ConfigurationImage configuration, boolean vertical) {
        this(GaugeScale.fromIni(model, ini, configuration), model, vertical);
    }

    public BarGauge(GaugeScale scale, GaugeModel model, boolean vertical) {
        super(scale, model);
        this.vertical = vertical;
        setPreferredSize(vertical ? new Dimension(120, 240) : new Dimension(260, 110));
        setMinimumSize(new Dimension(50, 50));
    }

    boolean isVertical() {
        return vertical;
    }

    @Override
    void paintContent(Graphics2D g, RoundRectangle2D.Double face, double value) {
        if (vertical) {
            paintVertical(g, face, value);
        } else {
            paintHorizontal(g, face, value);
        }
    }

    private void paintHorizontal(Graphics2D g, RoundRectangle2D.Double face, double value) {
        double pad = Math.max(8, 0.1 * Math.min(face.width, face.height));
        double left = face.x + pad;
        double width = face.width - 2 * pad;
        double bottom = face.y + face.height - pad;

        Font valueFont = GaugeStyle.font(Font.BOLD, (float) clamp(face.height * 0.26, 12, 34));
        Font unitsFont = GaugeStyle.font(Font.PLAIN, valueFont.getSize2D() * 0.45f);
        FontMetrics valueMetrics = g.getFontMetrics(valueFont);
        double headerBaseline = face.y + pad + valueMetrics.getAscent() * 0.9;
        double valueRight = left + width;
        double valueLeft = paintValue(g, value, valueFont, unitsFont, valueRight, headerBaseline);

        g.setFont(GaugeStyle.font(Font.PLAIN, (float) clamp(face.height * 0.14, 10, 16)));
        g.setColor(GaugeStyle.SCALE);
        GaugeStyle.drawText(g, GaugeStyle.fit(g, getTitle(), valueLeft - left - pad), left, headerBaseline, -1);

        Font labelFont = GaugeStyle.font(Font.PLAIN, (float) clamp(face.height * 0.11, 9, 13));
        FontMetrics labelMetrics = g.getFontMetrics(labelFont);
        double thickness = clamp(face.height * 0.18, 6, 26);
        double labelsHeight = labelMetrics.getHeight();
        double trackTop = Math.min(headerBaseline + valueMetrics.getDescent() + Math.max(6, face.height * 0.08),
            bottom - labelsHeight - 4 - thickness);
        GaugeStyle.paintBar(g, left, trackTop, width, thickness, false,
            fraction(value), dangerFraction(), isDanger(value));

        g.setFont(labelFont);
        g.setColor(GaugeStyle.TICK);
        double labelBaseline = trackTop + thickness + 4 + labelMetrics.getAscent();
        GaugeStyle.drawText(g, label(minValue), left, labelBaseline, -1);
        GaugeStyle.drawText(g, label(maxValue), left + width, labelBaseline, 1);
    }

    private void paintVertical(Graphics2D g, RoundRectangle2D.Double face, double value) {
        double pad = Math.max(8, 0.1 * Math.min(face.width, face.height));
        double centre = face.x + face.width / 2;
        double width = face.width - 2 * pad;
        double bottom = face.y + face.height - pad;

        g.setFont(GaugeStyle.font(Font.PLAIN, (float) clamp(face.width * 0.11, 10, 16)));
        g.setColor(GaugeStyle.SCALE);
        FontMetrics titleMetrics = g.getFontMetrics();
        double titleBaseline = face.y + pad + titleMetrics.getAscent();
        GaugeStyle.drawText(g, GaugeStyle.fit(g, getTitle(), width), centre, titleBaseline, 0);

        Font valueFont = GaugeStyle.font(Font.BOLD,
            (float) clamp(Math.min(face.width * 0.24, face.height * 0.13), 12, 34));
        Font unitsFont = GaugeStyle.font(Font.PLAIN, valueFont.getSize2D() * 0.45f);
        String text = text(value);
        String units = units();
        double gap = units.isEmpty() ? 0 : valueFont.getSize2D() * 0.15;
        double total = g.getFontMetrics(valueFont).stringWidth(text) + gap + g.getFontMetrics(unitsFont).stringWidth(units);
        if (total > width && total > 0) {
            float scale = (float) (width / total);
            valueFont = GaugeStyle.font(Font.BOLD, valueFont.getSize2D() * scale);
            unitsFont = GaugeStyle.font(Font.PLAIN, unitsFont.getSize2D() * scale);
            total = width;
        }
        FontMetrics valueMetrics = g.getFontMetrics(valueFont);
        double valueBaseline = titleBaseline + titleMetrics.getDescent() + valueMetrics.getAscent();
        paintValue(g, value, valueFont, unitsFont, centre + total / 2, valueBaseline);

        Font labelFont = GaugeStyle.font(Font.PLAIN, (float) clamp(face.width * 0.1, 9, 13));
        FontMetrics labelMetrics = g.getFontMetrics(labelFont);
        double labelWidth = Math.max(labelMetrics.stringWidth(label(minValue)), labelMetrics.stringWidth(label(maxValue)));
        double thickness = clamp(face.width * 0.2, 8, 28);
        double trackTop = valueBaseline + valueMetrics.getDescent() + Math.max(6, face.height * 0.04);
        double trackHeight = bottom - trackTop;
        if (trackHeight < thickness) {
            return;
        }
        // Centre the track and its labels together when there is room for the labels.
        boolean labels = thickness + 6 + labelWidth <= width;
        double trackLeft = labels ? centre - (thickness + 6 + labelWidth) / 2 : centre - thickness / 2;
        GaugeStyle.paintBar(g, trackLeft, trackTop, thickness, trackHeight, true,
            fraction(value), dangerFraction(), isDanger(value));
        if (labels) {
            g.setFont(labelFont);
            g.setColor(GaugeStyle.TICK);
            double labelX = trackLeft + thickness + 6;
            GaugeStyle.drawText(g, label(maxValue), labelX, trackTop + labelMetrics.getAscent(), -1);
            GaugeStyle.drawText(g, label(minValue), labelX, trackTop + trackHeight, -1);
        }
    }

    /** Draws the value and units ending at {@code right}; returns where the value starts. */
    private double paintValue(Graphics2D g, double value, Font valueFont, Font unitsFont, double right, double baseline) {
        String units = units();
        g.setFont(unitsFont);
        g.setColor(GaugeStyle.SCALE);
        double unitsWidth = g.getFontMetrics().stringWidth(units);
        GaugeStyle.drawText(g, units, right, baseline, 1);
        double gap = units.isEmpty() ? 0 : valueFont.getSize2D() * 0.15;
        g.setFont(valueFont);
        g.setColor(!Double.isFinite(value) ? GaugeStyle.TICK : isDanger(value) ? GaugeStyle.DANGER : GaugeStyle.TEXT);
        String text = text(value);
        double valueRight = right - unitsWidth - gap;
        GaugeStyle.drawText(g, text, valueRight, baseline, 1);
        return valueRight - g.getFontMetrics().stringWidth(text);
    }

    private String units() {
        return getUnitString() == null ? "" : getUnitString();
    }

    private static double clamp(double value, double min, double max) {
        return Math.max(min, Math.min(max, value));
    }
}
