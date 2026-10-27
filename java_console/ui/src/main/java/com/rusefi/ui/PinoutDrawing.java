package com.rusefi.ui;

import com.rusefi.ui.util.PinColors;

import javax.swing.JPanel;
import javax.swing.UIManager;
import java.awt.BasicStroke;
import java.awt.Color;
import java.awt.Cursor;
import java.awt.Font;
import java.awt.FontMetrics;
import java.awt.Graphics;
import java.awt.Graphics2D;
import java.awt.Rectangle;
import java.awt.RenderingHints;
import java.awt.Shape;
import java.awt.event.MouseAdapter;
import java.awt.event.MouseEvent;
import java.awt.geom.Ellipse2D;
import java.awt.image.BufferedImage;
import java.util.ArrayList;
import java.util.Collections;
import java.util.List;
import java.util.function.Consumer;
import java.util.function.Predicate;

/**
 * A connector's image with a marker on each pin the YAML places, on a light card [tag:console_theme].
 * Markers are sized from the spacing between pins, so they fill the cavities at any size, and coloured by
 * {@link ColorMode}. The selected pin gets an accent ring, pins outside a filter fade, and hovering a pin
 * shows a card with what it is for. The image is downscaled in steps and cached, which keeps the line art's
 * thin lines intact when a large image is drawn small.
 */
final class PinoutDrawing extends JPanel {
    /** What the marker colours show. */
    enum ColorMode {
        /** What each pin is for ({@link PinColors.PinType}). */
        TYPE,
        /** The pigtail wire colour. */
        PIGTAIL,
        /** Used by the tune, free or fixed ({@link PinoutPin.Use}). */
        TUNE_USE
    }

    private static final int PAD = 16;
    private static final int MIN_RADIUS = 9;
    private static final int MAX_RADIUS = 24;
    /** Marker radius as a share of the distance between neighbouring pins. */
    private static final double RADIUS_OF_PITCH = 0.34;
    private static final float FADED = 0.22f;
    private static final int CARD_WIDTH = 280;

    private BufferedImage image;
    private List<PinoutPin> pins = Collections.emptyList();
    /** Smallest distance between two placed pins, in image pixels; 0 with fewer than two. */
    private double pitch;
    private String selected;
    private String hovered;
    private ColorMode mode = ColorMode.TYPE;
    private Predicate<PinoutPin> shown = pin -> true;
    private Consumer<String> pinClickListener;

    private BufferedImage scaled;
    private double scale = 1;
    private int offsetX;
    private int offsetY;

    PinoutDrawing() {
        setOpaque(false);
        MouseAdapter mouse = new MouseAdapter() {
            @Override
            public void mouseClicked(MouseEvent e) {
                String pin = pinAt(e.getX(), e.getY());
                if (pin != null) {
                    setSelectedPin(pin);
                    if (pinClickListener != null) {
                        pinClickListener.accept(pin);
                    }
                }
            }

            @Override
            public void mouseMoved(MouseEvent e) {
                setHovered(pinAt(e.getX(), e.getY()));
            }

            @Override
            public void mouseExited(MouseEvent e) {
                setHovered(null);
            }
        };
        addMouseListener(mouse);
        addMouseMotionListener(mouse);
    }

    void setData(BufferedImage image, List<PinoutPin> pins) {
        this.image = image;
        this.pins = new ArrayList<>();
        for (PinoutPin pin : pins) {
            if (pin.isPlaced()) {
                this.pins.add(pin);
            }
        }
        pitch = 0;
        for (int i = 0; i < this.pins.size(); i++) {
            for (int j = i + 1; j < this.pins.size(); j++) {
                double d = Math.hypot(this.pins.get(i).x - this.pins.get(j).x, this.pins.get(i).y - this.pins.get(j).y);
                if (d > 0 && (pitch == 0 || d < pitch)) {
                    pitch = d;
                }
            }
        }
        scaled = null;
        selected = null;
        repaint();
    }

    void setPinClickListener(Consumer<String> listener) {
        pinClickListener = listener;
    }

    void setSelectedPin(String pin) {
        selected = pin;
        repaint();
    }

    String selectedPin() {
        return selected;
    }

    String hoveredPin() {
        return hovered;
    }

    void setColorMode(ColorMode mode) {
        this.mode = mode;
        repaint();
    }

    /** Pins the predicate rejects fade, e.g. those not matching a search. */
    void setShown(Predicate<PinoutPin> shown) {
        this.shown = shown;
        repaint();
    }

    private void setHovered(String pin) {
        if (pin == null ? hovered != null : !pin.equals(hovered)) {
            hovered = pin;
            setCursor(Cursor.getPredefinedCursor(pin != null ? Cursor.HAND_CURSOR : Cursor.DEFAULT_CURSOR));
            repaint();
        }
    }

    // ---- geometry

    private void layoutImage() {
        if (image == null || getWidth() <= 2 * PAD || getHeight() <= 2 * PAD) {
            scale = 1;
            offsetX = offsetY = 0;
            return;
        }
        double sx = (double) (getWidth() - 2 * PAD) / image.getWidth();
        double sy = (double) (getHeight() - 2 * PAD) / image.getHeight();
        scale = Math.min(sx, sy);
        offsetX = (int) Math.round((getWidth() - image.getWidth() * scale) / 2);
        offsetY = (int) Math.round((getHeight() - image.getHeight() * scale) / 2);
    }

    // the geometry follows the current size, so these lay out first; it is a few multiplications

    int screenX(PinoutPin pin) {
        layoutImage();
        return (int) Math.round(pin.x * scale) + offsetX;
    }

    int screenY(PinoutPin pin) {
        layoutImage();
        return (int) Math.round(pin.y * scale) + offsetY;
    }

    /** Marker radius in screen pixels: a share of the pin spacing, within limits. */
    int markerRadius() {
        layoutImage();
        double fromPitch = pitch > 0 ? pitch * scale * RADIUS_OF_PITCH : 14;
        return (int) Math.round(Math.max(MIN_RADIUS, Math.min(MAX_RADIUS, fromPitch)));
    }

    /** The pin under a point, within its marker or a few pixels around it; the nearest wins. */
    String pinAt(int mx, int my) {
        layoutImage();
        double reach = markerRadius() + 3;
        String nearest = null;
        double best = reach * reach;
        for (PinoutPin pin : pins) {
            double dx = mx - screenX(pin);
            double dy = my - screenY(pin);
            double d2 = dx * dx + dy * dy;
            if (d2 <= best) {
                best = d2;
                nearest = pin.name;
            }
        }
        return nearest;
    }

    // ---- colours, shared with the legend

    /** One fill, or two for a two-colour wire; never empty. */
    static Color[] fills(PinoutPin pin, ColorMode mode) {
        switch (mode) {
            case PIGTAIL: {
                Color[] wire = PinColors.pigtailToColors(pin.wire);
                return wire != null ? wire : new Color[]{themed("Console.Card.background", Color.WHITE)};
            }
            case TUNE_USE:
                switch (pin.use()) {
                    case USED:
                        return new Color[]{themed("Console.Pinout.used", new Color(0x2563eb))};
                    case FREE:
                        return new Color[]{themed("Console.Card.background", Color.WHITE)};
                    default:
                        return new Color[]{themed("Console.Pinout.fixed", new Color(0xe5e7eb))};
                }
            default:
                return new Color[]{pin.type.type.color()};
        }
    }

    /** Pins drawn with a dashed outline: free for the tune, or with no wire colour listed. */
    static boolean dashed(PinoutPin pin, ColorMode mode) {
        return mode == ColorMode.TUNE_USE && pin.use() == PinoutPin.Use.FREE
                || mode == ColorMode.PIGTAIL && PinColors.pigtailToColors(pin.wire) == null
                || mode == ColorMode.TYPE && pin.type.type == PinColors.PinType.UNTYPED;
    }

    private static Color labelColor(PinoutPin pin, ColorMode mode, Color fill) {
        if (mode == ColorMode.TUNE_USE && pin.use() == PinoutPin.Use.FREE) {
            return themed("Console.Pinout.used", new Color(0x2563eb));
        }
        return PinColors.isLight(fill) ? new Color(0x1f2937) : Color.WHITE;
    }

    static Color themed(String key, Color fallback) {
        Color color = UIManager.getColor(key);
        return color != null ? color : fallback;
    }

    // ---- painting

    @Override
    protected void paintComponent(Graphics g) {
        super.paintComponent(g);
        if (image == null) {
            return;
        }
        layoutImage();
        Graphics2D g2 = (Graphics2D) g.create();
        try {
            g2.setRenderingHint(RenderingHints.KEY_ANTIALIASING, RenderingHints.VALUE_ANTIALIAS_ON);
            g2.setRenderingHint(RenderingHints.KEY_TEXT_ANTIALIASING, RenderingHints.VALUE_TEXT_ANTIALIAS_ON);
            g2.setRenderingHint(RenderingHints.KEY_STROKE_CONTROL, RenderingHints.VALUE_STROKE_PURE);
            int w = (int) Math.round(image.getWidth() * scale);
            int h = (int) Math.round(image.getHeight() * scale);
            if (w > 0 && h > 0) {
                g2.drawImage(scaledImage(w, h), offsetX, offsetY, null);
            }
            int r = markerRadius();
            PinoutPin hover = null;
            for (PinoutPin pin : pins) {
                paintMarker(g2, pin, r);
                if (pin.name.equals(hovered)) {
                    hover = pin;
                }
            }
            if (hover != null) {
                paintCard(g2, hover, r);
            }
        } finally {
            g2.dispose();
        }
    }

    /** The image at the drawn size, halved step by step from full size so thin lines survive. */
    private BufferedImage scaledImage(int w, int h) {
        if (scaled != null && scaled.getWidth() == w && scaled.getHeight() == h) {
            return scaled;
        }
        BufferedImage current = image;
        int cw = image.getWidth();
        int ch = image.getHeight();
        do {
            cw = Math.max(w, cw / 2);
            ch = Math.max(h, ch / 2);
            if (w > image.getWidth()) {
                cw = w;
                ch = h;
            }
            BufferedImage next = new BufferedImage(cw, ch, BufferedImage.TYPE_INT_ARGB);
            Graphics2D g = next.createGraphics();
            g.setRenderingHint(RenderingHints.KEY_INTERPOLATION, RenderingHints.VALUE_INTERPOLATION_BILINEAR);
            g.setRenderingHint(RenderingHints.KEY_RENDERING, RenderingHints.VALUE_RENDER_QUALITY);
            g.drawImage(current, 0, 0, cw, ch, null);
            g.dispose();
            current = next;
        } while (cw != w || ch != h);
        scaled = current;
        return scaled;
    }

    private void paintMarker(Graphics2D g2, PinoutPin pin, int r) {
        int cx = screenX(pin);
        int cy = screenY(pin);
        boolean faded = !shown.test(pin);
        Graphics2D g = (Graphics2D) g2.create();
        try {
            Ellipse2D circle = new Ellipse2D.Double(cx - r, cy - r, 2 * r, 2 * r);
            if (!faded) {
                // a soft shadow lifts the marker off the line art
                g.setColor(new Color(21, 26, 33, 50));
                g.fill(new Ellipse2D.Double(cx - r, cy - r + 1.5, 2 * r, 2 * r));
            }
            Color[] fills = fills(pin, mode);
            if (faded) {
                // still solid, so the cavity behind does not show through; only washed out
                Color background = themed("Console.Card.background", Color.WHITE);
                for (int i = 0; i < fills.length; i++) {
                    fills[i] = mix(fills[i], background, 1 - FADED);
                }
            }
            if (fills.length == 2) {
                Shape clip = g.getClip();
                g.clip(new Rectangle(cx - r, cy - r, r, 2 * r));
                g.setColor(fills[0]);
                g.fill(circle);
                g.setClip(clip);
                g.clip(new Rectangle(cx, cy - r, r + 1, 2 * r));
                g.setColor(fills[1]);
                g.fill(circle);
                g.setClip(clip);
            } else {
                g.setColor(fills[0]);
                g.fill(circle);
            }
            if (dashed(pin, mode)) {
                g.setColor(mode == ColorMode.TUNE_USE ? themed("Console.Pinout.used", new Color(0x2563eb))
                        : new Color(0x9ca3af));
                g.setStroke(new BasicStroke(2f, BasicStroke.CAP_BUTT, BasicStroke.JOIN_ROUND, 1f,
                        new float[]{3f, 2.5f}, 0f));
            } else {
                boolean whiteFill = PinColors.isLight(fills[0]) && fills[0].getRed() > 240;
                g.setColor(whiteFill ? new Color(0x9ca3af) : Color.WHITE);
                g.setStroke(new BasicStroke(2f));
            }
            g.draw(circle);
            if (pin.name.equals(selected)) {
                g.setColor(Color.WHITE);
                g.setStroke(new BasicStroke(3f));
                g.draw(new Ellipse2D.Double(cx - r - 2.5, cy - r - 2.5, 2 * r + 5, 2 * r + 5));
                g.setColor(themed("Console.Pinout.selection", new Color(0x2563eb)));
                g.setStroke(new BasicStroke(3f));
                g.draw(new Ellipse2D.Double(cx - r - 5.5, cy - r - 5.5, 2 * r + 11, 2 * r + 11));
            }
            Color label = labelColor(pin, mode, fills[0]);
            paintLabel(g, pin, cx, cy, r, faded ? mix(label, fills[0], 0.55f) : label);
        } finally {
            g.dispose();
        }
    }

    /** {@code share} of {@code to} mixed into {@code from}. */
    private static Color mix(Color from, Color to, float share) {
        return new Color(
                Math.round(from.getRed() + (to.getRed() - from.getRed()) * share),
                Math.round(from.getGreen() + (to.getGreen() - from.getGreen()) * share),
                Math.round(from.getBlue() + (to.getBlue() - from.getBlue()) * share));
    }

    /** The pin name, as large as fits inside the marker. */
    private void paintLabel(Graphics2D g, PinoutPin pin, int cx, int cy, int r, Color color) {
        float size = Math.max(8f, Math.min(14f, r * 0.75f));
        Font font = getFont().deriveFont(Font.BOLD, size);
        FontMetrics fm = g.getFontMetrics(font);
        while (fm.stringWidth(pin.name) > 2 * r - 4 && size > 7f) {
            size -= 0.5f;
            font = font.deriveFont(size);
            fm = g.getFontMetrics(font);
        }
        g.setFont(font);
        g.setColor(color);
        g.drawString(pin.name, cx - fm.stringWidth(pin.name) / 2f, cy + (fm.getAscent() - fm.getDescent()) / 2f);
    }

    /** What a hovered pin is for, in a card above it (below when there is no room above). */
    private void paintCard(Graphics2D g2, PinoutPin pin, int r) {
        Font base = getFont();
        Font bold = base.deriveFont(Font.BOLD);
        Font small = base.deriveFont(base.getSize2D() - 1);
        FontMetrics fm = g2.getFontMetrics(base);
        FontMetrics bm = g2.getFontMetrics(bold);
        FontMetrics sm = g2.getFontMetrics(small);
        int inner = CARD_WIDTH - 24;
        List<String> lines = wrap(pin.function.isEmpty() ? "No function listed" : pin.function, fm, inner);
        String use;
        Color useColor;
        switch (pin.use()) {
            case USED:
                use = "Tune: " + pin.tuneUse;
                useColor = themed("Console.Pinout.used", new Color(0x2563eb));
                break;
            case FREE:
                use = "Free: not used by the tune";
                useColor = themed("Label.disabledForeground", Color.GRAY);
                break;
            default:
                use = "Fixed function: not set in the tune";
                useColor = themed("Label.disabledForeground", Color.GRAY);
        }
        List<String> useLines = wrap(use, bm, inner);
        int lineH = fm.getHeight();
        int height = 12 + bm.getHeight() + 6 + lines.size() * lineH + 6 + useLines.size() * bm.getHeight() + 10;
        int x = screenX(pin) - CARD_WIDTH / 2;
        int y = screenY(pin) - r - 12 - height;
        if (y < 4) {
            y = screenY(pin) + r + 12;
        }
        x = Math.max(4, Math.min(getWidth() - CARD_WIDTH - 4, x));
        y = Math.max(4, Math.min(getHeight() - height - 4, y));

        g2.setColor(new Color(21, 26, 33, 40));
        g2.fillRoundRect(x + 1, y + 3, CARD_WIDTH, height, 12, 12);
        g2.setColor(themed("Console.Card.background", Color.WHITE));
        g2.fillRoundRect(x, y, CARD_WIDTH, height, 12, 12);
        g2.setColor(themed("Console.Card.borderColor", new Color(0xdfe3e9)));
        g2.setStroke(new BasicStroke(1f));
        g2.drawRoundRect(x, y, CARD_WIDTH, height, 12, 12);

        int tx = x + 12;
        int ty = y + 12 + bm.getAscent();
        g2.setFont(bold);
        g2.setColor(themed("Label.foreground", Color.BLACK));
        g2.drawString(pin.name, tx, ty);
        // type chip after the name, the wire swatch at the right
        PinColors.PinType type = pin.type.type;
        String chip = type.label;
        int chipX = tx + bm.stringWidth(pin.name) + 10;
        int chipW = sm.stringWidth(chip) + 14;
        int chipH = sm.getHeight() + 2;
        int chipY = ty - bm.getAscent() + (bm.getHeight() - chipH) / 2;
        g2.setColor(type.color());
        g2.fillRoundRect(chipX, chipY, chipW, chipH, chipH, chipH);
        g2.setFont(small);
        g2.setColor(type.textColor());
        g2.drawString(chip, chipX + 7, chipY + 1 + sm.getAscent());
        Color[] wire = PinColors.pigtailToColors(pin.wire);
        if (wire != null) {
            int sw = 20;
            int sh = 10;
            int sx = x + CARD_WIDTH - 12 - sw;
            int sy = ty - bm.getAscent() + (bm.getHeight() - sh) / 2;
            g2.setColor(wire[0]);
            g2.fillRoundRect(sx, sy, wire.length == 2 ? sw / 2 + 2 : sw, sh, 3, 3);
            if (wire.length == 2) {
                g2.setColor(wire[1]);
                g2.fillRoundRect(sx + sw / 2, sy, sw / 2, sh, 3, 3);
            }
            g2.setColor(new Color(0xc7ccd4));
            g2.drawRoundRect(sx, sy, sw, sh, 3, 3);
        }

        ty += bm.getDescent() + 6;
        g2.setFont(base);
        g2.setColor(themed("Label.foreground", Color.BLACK));
        for (String line : lines) {
            ty += fm.getAscent();
            g2.drawString(line, tx, ty);
            ty += fm.getDescent() + fm.getLeading();
        }
        ty += 6;
        g2.setFont(bold);
        g2.setColor(useColor);
        for (String line : useLines) {
            ty += bm.getAscent();
            g2.drawString(line, tx, ty);
            ty += bm.getDescent() + bm.getLeading();
        }
    }

    private static List<String> wrap(String text, FontMetrics fm, int width) {
        List<String> lines = new ArrayList<>();
        StringBuilder line = new StringBuilder();
        for (String word : text.split("\\s+")) {
            String candidate = line.length() == 0 ? word : line + " " + word;
            if (fm.stringWidth(candidate) > width && line.length() > 0) {
                lines.add(line.toString());
                line = new StringBuilder(word);
            } else {
                line = new StringBuilder(candidate);
            }
        }
        if (line.length() > 0) {
            lines.add(line.toString());
        }
        return lines;
    }
}
