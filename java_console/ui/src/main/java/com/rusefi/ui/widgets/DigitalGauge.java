package com.rusefi.ui.widgets;

import com.opensr5.ConfigurationImage;
import com.opensr5.ini.GaugeModel;
import com.opensr5.ini.IniFileModel;

import java.awt.*;
import java.awt.geom.RoundRectangle2D;

/** Text presentation: title, a large value with its units, and a thin progress line. */
public final class DigitalGauge extends CardGauge {
    public DigitalGauge(GaugeModel model, IniFileModel ini, ConfigurationImage configuration) {
        this(GaugeScale.fromIni(model, ini, configuration), model);
    }

    public DigitalGauge(GaugeScale scale, GaugeModel model) {
        super(scale, model);
        setPreferredSize(new Dimension(220, 120));
        setMinimumSize(new Dimension(80, 50));
    }

    @Override
    void paintContent(Graphics2D g, RoundRectangle2D.Double face, double value) {
        double pad = Math.max(8, 0.08 * Math.min(face.width, face.height));
        double left = face.x + pad;
        double width = face.width - 2 * pad;
        double bottom = face.y + face.height - pad;

        g.setFont(GaugeStyle.font(Font.PLAIN, (float) clamp(face.height * 0.12, 10, 16)));
        g.setColor(GaugeStyle.SCALE);
        FontMetrics titleMetrics = g.getFontMetrics();
        double titleBaseline = face.y + pad + titleMetrics.getAscent();
        GaugeStyle.drawText(g, GaugeStyle.fit(g, getTitle(), width), left, titleBaseline, -1);

        double line = Math.max(3, face.height * 0.035);
        GaugeStyle.paintBar(g, left, bottom - line, width, line, false,
            fraction(value), dangerFraction(), isDanger(value));

        // The value and its units share one centred line between the title and the progress line.
        String text = text(value);
        String units = getUnitString() == null ? "" : getUnitString();
        double top = titleBaseline + titleMetrics.getDescent();
        double available = bottom - line - top;
        float valueSize = (float) Math.max(8, available * 0.78);
        Font valueFont = GaugeStyle.font(Font.BOLD, valueSize);
        Font unitsFont = GaugeStyle.font(Font.PLAIN, valueSize * 0.38f);
        double gap = units.isEmpty() ? 0 : valueSize * 0.15;
        double total = g.getFontMetrics(valueFont).stringWidth(text) + gap + g.getFontMetrics(unitsFont).stringWidth(units);
        if (total > width && total > 0) {
            float scale = (float) (width / total);
            valueSize *= scale;
            valueFont = GaugeStyle.font(Font.BOLD, valueSize);
            unitsFont = GaugeStyle.font(Font.PLAIN, valueSize * 0.38f);
            gap *= scale;
            total = width;
        }
        FontMetrics valueMetrics = g.getFontMetrics(valueFont);
        double baseline = top + available / 2 + (valueMetrics.getAscent() - valueMetrics.getDescent()) / 2.0;
        double x = left + (width - total) / 2;
        g.setFont(valueFont);
        g.setColor(!Double.isFinite(value) ? GaugeStyle.TICK : isDanger(value) ? GaugeStyle.DANGER : GaugeStyle.TEXT);
        GaugeStyle.drawText(g, text, x, baseline, -1);
        g.setFont(unitsFont);
        g.setColor(GaugeStyle.SCALE);
        GaugeStyle.drawText(g, units, x + valueMetrics.stringWidth(text) + gap, baseline, -1);
    }

    private static double clamp(double value, double min, double max) {
        return Math.max(min, Math.min(max, value));
    }
}
