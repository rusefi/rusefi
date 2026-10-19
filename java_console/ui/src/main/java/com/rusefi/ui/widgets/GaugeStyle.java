package com.rusefi.ui.widgets;

import javax.swing.UIManager;
import java.awt.*;
import java.awt.geom.Rectangle2D;
import java.awt.geom.RoundRectangle2D;

/** Shared light palette and drawing helpers, so every gauge presentation looks like one family. */
final class GaugeStyle {
    static final Color PROGRESS = new Color(0xFCE38A);
    static final Color PROGRESS_STRONG = new Color(0xF6C445);
    static final Color DANGER_FILL = new Color(0xFFCDD2);
    static final Color DANGER = new Color(0xE53935);
    static final Color SCALE = new Color(0x5F656D);
    static final Color TICK = new Color(0x9AA0A6);
    static final Color TRACK = new Color(0xE3E5E8);
    static final Color CARD_TOP = Color.WHITE;
    static final Color CARD_BOTTOM = new Color(0xF6F7F9);
    static final Color CARD_BORDER = new Color(0xE1E4E8);
    static final Color TEXT = new Color(0x111111);
    /** Ellipsis written as an escape: source encoding must not change the rendered text. */
    static final String ELLIPSIS = "…";
    private static final int SHADOW = 3;

    private GaugeStyle() {
    }

    /** The look and feel's UI font family, so gauges match the rest of the console. */
    static Font font(int style, float size) {
        Font base = UIManager.getFont("Label.font");
        if (base == null) {
            base = new Font(Font.SANS_SERIF, Font.PLAIN, 12);
        }
        return base.deriveFont(style, size);
    }

    /**
     * Paints a rounded white card with a soft shadow, entirely inside {@code width} x
     * {@code height}, and returns its face. Corners stay transparent for overlapping gauges.
     */
    static RoundRectangle2D.Double paintCard(Graphics2D g, int width, int height) {
        double arc = Math.min(18, 0.14 * Math.min(width, height));
        for (int i = SHADOW; i >= 1; i--) {
            g.setColor(new Color(0, 0, 0, 7));
            g.fill(new RoundRectangle2D.Double(SHADOW - i + 1, SHADOW - i + 2, width - 2 * (SHADOW - i + 1),
                height - 2 * (SHADOW - i + 1) - 1, arc + i, arc + i));
        }
        RoundRectangle2D.Double face = new RoundRectangle2D.Double(SHADOW, SHADOW - 1,
            width - 2 * SHADOW, height - 2 * SHADOW, arc, arc);
        g.setPaint(new GradientPaint(0, (float) face.y, CARD_TOP, 0, (float) (face.y + face.height), CARD_BOTTOM));
        g.fill(face);
        g.setColor(CARD_BORDER);
        g.setStroke(new BasicStroke(1f));
        g.draw(face);
        return face;
    }

    /** Rounded track, optional danger tint from {@code dangerStart}, and the filled share {@code fill}. */
    static void paintBar(Graphics2D g, double x, double y, double width, double height, boolean vertical,
                         double fill, double dangerStart, boolean danger) {
        double arc = vertical ? width : height;
        RoundRectangle2D.Double track = new RoundRectangle2D.Double(x, y, width, height, arc, arc);
        g.setColor(TRACK);
        g.fill(track);
        Shape oldClip = g.getClip();
        g.clip(track);
        if (dangerStart < 1) {
            g.setColor(DANGER_FILL);
            if (vertical) {
                g.fill(new Rectangle2D.Double(x, y, width, height * (1 - dangerStart)));
            } else {
                g.fill(new Rectangle2D.Double(x + width * dangerStart, y, width * (1 - dangerStart), height));
            }
        }
        if (fill > 0) {
            if (danger) {
                g.setColor(DANGER);
            } else if (vertical) {
                g.setPaint(new GradientPaint((float) x, 0, PROGRESS, (float) (x + width), 0, PROGRESS_STRONG));
            } else {
                g.setPaint(new GradientPaint(0, (float) y, PROGRESS, 0, (float) (y + height), PROGRESS_STRONG));
            }
            double length = (vertical ? height : width) * fill;
            // Round the leading end too, unless the bar is full.
            RoundRectangle2D.Double bar = vertical
                ? new RoundRectangle2D.Double(x, y + height - length, width, length, arc, arc)
                : new RoundRectangle2D.Double(x, y, length, height, arc, arc);
            g.fill(bar);
        }
        g.setClip(oldClip);
    }

    /** Draws {@code text} with its left edge, centre or right edge at {@code x} (align -1, 0, 1). */
    static void drawText(Graphics2D g, String text, double x, double baseline, int align) {
        if (text == null || text.isEmpty()) {
            return;
        }
        int width = g.getFontMetrics().stringWidth(text);
        double left = align < 0 ? x : align == 0 ? x - width / 2.0 : x - width;
        g.drawString(text, (float) left, (float) baseline);
    }

    /** {@code text} shortened with an ellipsis to fit {@code maxWidth} in the current font. */
    static String fit(Graphics2D g, String text, double maxWidth) {
        if (text == null) {
            return "";
        }
        FontMetrics metrics = g.getFontMetrics();
        if (metrics.stringWidth(text) <= maxWidth) {
            return text;
        }
        for (int length = text.length() - 1; length > 0; length--) {
            String shortened = text.substring(0, length).trim() + ELLIPSIS;
            if (metrics.stringWidth(shortened) <= maxWidth) {
                return shortened;
            }
        }
        return "";
    }

    /** Largest font of {@code style} up to {@code maxSize} whose rendering of {@code text} fits {@code maxWidth}. */
    static Font fitFont(Graphics2D g, String text, int style, float maxSize, double maxWidth) {
        Font font = font(style, maxSize);
        int width = g.getFontMetrics(font).stringWidth(text);
        if (width > maxWidth && width > 0) {
            font = font(style, (float) Math.max(6, maxSize * maxWidth / width));
        }
        return font;
    }
}
