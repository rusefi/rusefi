package com.rusefi.ui.widgets;

import javax.swing.UIManager;
import java.awt.*;
import java.awt.geom.Rectangle2D;
import java.awt.geom.RoundRectangle2D;
import java.util.Collections;
import java.util.LinkedHashMap;
import java.util.Map;

/**
 * Shared palette and drawing helpers, so every gauge presentation looks like one family. Colours come
 * from the Console.Gauge.* theme keys [tag:console_theme], light and dark; {@link #FALLBACKS} holds the
 * light values for when no theme is installed, and GaugeStyleTest keeps them equal to the light theme.
 */
public final class GaugeStyle {
    /** Console.Gauge.* key suffix -> light colour as 0xAARRGGBB, used when the theme has no value. */
    static final Map<String, Integer> FALLBACKS;

    static {
        Map<String, Integer> m = new LinkedHashMap<>();
        // card behind bar, digital and graph gauges
        m.put("card.top", 0xFFFFFFFF);
        m.put("card.bottom", 0xFFF6F7F9);
        m.put("card.border", 0xFFE1E4E8);
        // value, captions, scale numbers and ticks
        m.put("text", 0xFF111111);
        m.put("label", 0xFF2B2F33);
        m.put("scale", 0xFF5F656D);
        m.put("tick", 0xFF9AA0A6);
        // value bars and arcs: empty track, normal fill, warning and danger zones
        m.put("track", 0xFFE3E5E8);
        m.put("progress", 0xFFFCE38A);
        m.put("progressStrong", 0xFFF6C445);
        // normal-zone sector of a round dial: the scale numbers are drawn over it, unlike a bar's fill
        m.put("dialFill", 0xFFFCE38A);
        // amber rather than yellow: the normal fill is already yellow
        m.put("warning", 0xFFFFA000);
        m.put("warningFill", 0xFFFFCC80);
        m.put("warningText", 0xFFE65100);
        m.put("danger", 0xFFE53935);
        m.put("dangerFill", 0xFFFFCDD2);
        // round gauge: needle, bezel ring, dial face and the hub over the needle
        m.put("needle", 0xFFE53935);
        m.put("bezel.top", 0xFFFBFBFC);
        m.put("bezel.bottom", 0xFFDADDE1);
        m.put("face.center", 0xFFFFFFFF);
        m.put("face.edge", 0xFFF3F4F6);
        m.put("hub", 0xFFFFFFFF);
        m.put("hub.rim", 0xFFE1E4E8);
        // live graph: line, grid, danger band and the area under the line
        m.put("graph.line", 0xFFE0A100);
        m.put("graph.grid", 0xFFEDEFF2);
        m.put("graph.dangerBand", 0x6EFFCDD2);
        m.put("graph.warningBand", 0x5CFFCC80);
        // the graph line is amber already, so its warning part is a deeper orange
        m.put("graph.warningLine", 0xFFEF6C00);
        m.put("graph.areaTop", 0x78F6C445);
        m.put("graph.areaBottom", 0x00F6C445);
        // free layout editing: selection, item outlines, remove button and snap grid
        m.put("edit.accent", 0xFF1E88E5);
        m.put("edit.outline", 0x8C1E88E5);
        m.put("edit.close", 0xFFE53935);
        m.put("edit.grid", 0x12000000);
        // detached gauge windows without transparency, and their close box
        m.put("detached.background", 0xFFF3F4F6);
        m.put("detached.close", 0xFFFF0000);
        // gauge options button under the mouse
        m.put("hover", 0x18000000);
        FALLBACKS = Collections.unmodifiableMap(m);
    }

    /** Ellipsis written as an escape: source encoding must not change the rendered text. */
    public static final String ELLIPSIS = "\u2026";
    private static final int SHADOW = 3;

    private GaugeStyle() {
    }

    /** The theme's Console.Gauge.{@code name} colour, or its light fallback. */
    static Color color(String name) {
        Color themed = UIManager.getColor("Console.Gauge." + name);
        return themed != null ? themed : new Color(FALLBACKS.get(name), true);
    }

    public static Color cardTop() { return color("card.top"); }
    public static Color cardBottom() { return color("card.bottom"); }
    public static Color cardBorder() { return color("card.border"); }
    public static Color text() { return color("text"); }
    public static Color label() { return color("label"); }
    public static Color scale() { return color("scale"); }
    public static Color tick() { return color("tick"); }
    public static Color track() { return color("track"); }
    public static Color progress() { return color("progress"); }
    public static Color progressStrong() { return color("progressStrong"); }
    public static Color dialFill() { return color("dialFill"); }
    public static Color warning() { return color("warning"); }
    public static Color warningFill() { return color("warningFill"); }
    public static Color warningText() { return color("warningText"); }
    public static Color danger() { return color("danger"); }
    public static Color dangerFill() { return color("dangerFill"); }
    public static Color needle() { return color("needle"); }
    public static Color bezelTop() { return color("bezel.top"); }
    public static Color bezelBottom() { return color("bezel.bottom"); }
    public static Color faceCenter() { return color("face.center"); }
    public static Color faceEdge() { return color("face.edge"); }
    public static Color hub() { return color("hub"); }
    public static Color hubRim() { return color("hub.rim"); }
    public static Color graphLine() { return color("graph.line"); }
    public static Color graphGrid() { return color("graph.grid"); }
    public static Color graphDangerBand() { return color("graph.dangerBand"); }
    public static Color graphWarningBand() { return color("graph.warningBand"); }
    public static Color graphWarningLine() { return color("graph.warningLine"); }
    public static Color graphAreaTop() { return color("graph.areaTop"); }
    public static Color graphAreaBottom() { return color("graph.areaBottom"); }
    public static Color editAccent() { return color("edit.accent"); }
    public static Color editOutline() { return color("edit.outline"); }
    public static Color editClose() { return color("edit.close"); }
    public static Color editGrid() { return color("edit.grid"); }
    public static Color detachedBackground() { return color("detached.background"); }
    public static Color detachedClose() { return color("detached.close"); }
    public static Color hover() { return color("hover"); }

    /** The look and feel's UI font family, so gauges match the rest of the console. */
    public static Font font(int style, float size) {
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
    public static RoundRectangle2D.Double paintCard(Graphics2D g, int width, int height) {
        double arc = Math.min(18, 0.14 * Math.min(width, height));
        for (int i = SHADOW; i >= 1; i--) {
            g.setColor(new Color(0, 0, 0, 7)); // [tag:fixed_color] a faint shadow is black in every theme
            g.fill(new RoundRectangle2D.Double(SHADOW - i + 1, SHADOW - i + 2, width - 2 * (SHADOW - i + 1),
                height - 2 * (SHADOW - i + 1) - 1, arc + i, arc + i));
        }
        RoundRectangle2D.Double face = new RoundRectangle2D.Double(SHADOW, SHADOW - 1,
            width - 2 * SHADOW, height - 2 * SHADOW, arc, arc);
        g.setPaint(new GradientPaint(0, (float) face.y, cardTop(), 0, (float) (face.y + face.height), cardBottom()));
        g.fill(face);
        g.setColor(cardBorder());
        g.setStroke(new BasicStroke(1f));
        g.draw(face);
        return face;
    }

    /** Rounded track, optional danger tint from {@code dangerStart}, and the filled share {@code fill}. */
    public static void paintBar(Graphics2D g, double x, double y, double width, double height, boolean vertical,
                         double fill, double dangerStart, boolean danger) {
        double arc = vertical ? width : height;
        RoundRectangle2D.Double track = new RoundRectangle2D.Double(x, y, width, height, arc, arc);
        g.setColor(track());
        g.fill(track);
        Shape oldClip = g.getClip();
        g.clip(track);
        if (dangerStart < 1) {
            g.setColor(dangerFill());
            if (vertical) {
                g.fill(new Rectangle2D.Double(x, y, width, height * (1 - dangerStart)));
            } else {
                g.fill(new Rectangle2D.Double(x + width * dangerStart, y, width * (1 - dangerStart), height));
            }
        }
        if (fill > 0) {
            if (danger) {
                g.setColor(danger());
            } else if (vertical) {
                g.setPaint(new GradientPaint((float) x, 0, progress(), (float) (x + width), 0, progressStrong()));
            } else {
                g.setPaint(new GradientPaint(0, (float) y, progress(), 0, (float) (y + height), progressStrong()));
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
    public static void drawText(Graphics2D g, String text, double x, double baseline, int align) {
        if (text == null || text.isEmpty()) {
            return;
        }
        int width = g.getFontMetrics().stringWidth(text);
        double left = align < 0 ? x : align == 0 ? x - width / 2.0 : x - width;
        g.drawString(text, (float) left, (float) baseline);
    }

    /** {@code text} shortened with an ellipsis to fit {@code maxWidth} in the current font. */
    public static String fit(Graphics2D g, String text, double maxWidth) {
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
    public static Font fitFont(Graphics2D g, String text, int style, float maxSize, double maxWidth) {
        Font font = font(style, maxSize);
        int width = g.getFontMetrics(font).stringWidth(text);
        if (width > maxWidth && width > 0) {
            font = font(style, (float) Math.max(6, maxSize * maxWidth / width));
        }
        return font;
    }
}
