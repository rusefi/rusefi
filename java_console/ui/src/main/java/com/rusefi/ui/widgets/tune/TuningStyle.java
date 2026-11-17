package com.rusefi.ui.widgets.tune;

import com.formdev.flatlaf.ui.FlatLineBorder;

import javax.swing.*;
import java.awt.*;

/**
 * Shared look of the Tuning tab: a white toolbar strip, soft indicator chips, white rounded cards
 * for live values and the open dialog, matching the startup card, setup wizard and Log Viewer.
 * Colours: Console.Tuning.*, Console.Card.* and Console.Toolbar.* theme keys [tag:console_theme].
 */
public final class TuningStyle {
    public static final int ARC = 10;

    public static Color color(String key, Color fallback) {
        Color color = UIManager.getColor(key);
        return color != null ? color : fallback;
    }

    public static Color barBackground() {
        return color("Console.Toolbar.background", Color.WHITE);
    }

    public static Color sidebarBackground() {
        return color("Console.Tuning.sidebar.background", Color.WHITE);
    }

    public static Color separator() {
        return color("Separator.foreground", new Color(0xdfe3e9));
    }

    public static Color text() {
        return color("Label.foreground", new Color(0x151a21));
    }

    public static Color muted() {
        return color("Console.Tuning.mutedForeground", new Color(0x586273));
    }

    public static Color dirty() {
        return color("Console.Tuning.dirtyForeground", new Color(0xb45309));
    }

    public static Color cardBackground() {
        return color("Console.Card.background", Color.WHITE);
    }

    public static Color cardBorder() {
        return color("Console.Card.borderColor", new Color(0xdfe3e9));
    }

    public static Color sectionBackground() {
        return color("Console.Card.sectionBackground", new Color(0xf7f8fa));
    }

    public static Color sectionBorder() {
        return color("Console.Tuning.section.borderColor", new Color(0xdfe3e9));
    }

    public static Color sectionHoverBorder() {
        return color("Console.Tuning.section.hoverBorderColor", new Color(0x92b1f5));
    }

    public static Color sectionTitle() {
        return color("Console.Tuning.section.title", text());
    }

    /** A white rounded card with the given padding, e.g. a live value or the open dialog. */
    public static void styleCard(JComponent card, int vertical, int horizontal) {
        styleRounded(card, cardBackground(), vertical, horizontal);
    }

    /** A live value in the Tuning strip: a mildly tinted rounded card, set apart from settings. */
    public static void styleLiveValue(JComponent card, int vertical, int horizontal) {
        Color background = color("Console.Tuning.liveValue.background", new Color(0xf3f7ff));
        Color border = color("Console.Tuning.liveValue.borderColor", new Color(0xd6e2fb));
        card.setBorder(new FlatLineBorder(new Insets(vertical, horizontal, vertical, horizontal), border, 1, ARC));
        card.setBackground(new Color(background.getRGB(), true));
        card.setOpaque(true);
    }

    /** A tinted rounded block inside a card, e.g. a dialog readout. */
    public static void styleSection(JComponent section, int vertical, int horizontal) {
        styleRounded(section, sectionBackground(), vertical, horizontal);
    }

    private static void styleRounded(JComponent component, Color background, int vertical, int horizontal) {
        // FlatPanelUI paints a rounded background when the border is a FlatLineBorder with an arc
        component.setBorder(new FlatLineBorder(new Insets(vertical, horizontal, vertical, horizontal), cardBorder(), 1, ARC));
        // a plain Color, not the theme's UIResource, so TransparentPanels keeps this block tinted
        component.setBackground(new Color(background.getRGB(), true));
        component.setOpaque(true);
    }

    /** A strip along the top of the tab: toolbar colour with a separator line below it. */
    public static void styleBar(JComponent bar) {
        bar.setBackground(new Color(barBackground().getRGB(), true));
        bar.setOpaque(true);
        bar.setBorder(BorderFactory.createMatteBorder(0, 0, 1, 0, separator()));
    }

    /**
     * Chip colours for an INI indicator colour name. The INI asks for saturated colours such as red
     * or yellow; the chip shows the same meaning as a soft background with matching text.
     *
     * @return {background, foreground}
     */
    static Color[] indicatorColors(String iniBackground, String iniForeground) {
        String tone = indicatorTone(iniBackground, iniForeground);
        return new Color[]{
            color("Console.Tuning.indicator." + tone + ".background", IndicatorPanel.parseColor(iniBackground)),
            color("Console.Tuning.indicator." + tone + ".foreground", IndicatorPanel.parseColor(iniForeground)),
        };
    }

    static String indicatorTone(String iniBackground, String iniForeground) {
        String background = iniBackground == null ? "" : iniBackground.toLowerCase().trim();
        switch (background) {
            case "red":
            case "yellow":
            case "green":
            case "blue":
                return background;
            case "black":
                return "dark";
            default:
                return "neutral";
        }
    }

    /** Red, yellow and green indicators are shown as solid bold chips that catch the eye. */
    static boolean isSolidTone(String tone) {
        return "red".equals(tone) || "yellow".equals(tone) || "green".equals(tone);
    }

    /** A label painted as a rounded chip in its background colour. */
    static final class Chip extends JLabel {
        Chip() {
            super(" ");
            setOpaque(false);
            setBorder(BorderFactory.createEmptyBorder(4, 10, 4, 10));
        }

        void setSolid(boolean solid) {
            Font font = getFont();
            int style = solid ? Font.BOLD : Font.PLAIN;
            if (font != null && font.getStyle() != style) {
                setFont(font.deriveFont(style));
            }
        }

        @Override
        protected void paintComponent(Graphics g) {
            Graphics2D g2 = (Graphics2D) g.create();
            try {
                g2.setRenderingHint(RenderingHints.KEY_ANTIALIASING, RenderingHints.VALUE_ANTIALIAS_ON);
                g2.setColor(getBackground());
                g2.fillRoundRect(0, 0, getWidth(), getHeight(), ARC, ARC);
            } finally {
                g2.dispose();
            }
            super.paintComponent(g);
        }
    }

    /** Expand and collapse all for the menu tree: chevrons pointing apart or together. */
    enum FoldIcon implements Icon {
        EXPAND_ALL, COLLAPSE_ALL;

        @Override
        public void paintIcon(Component component, Graphics graphics, int x, int y) {
            Graphics2D g = (Graphics2D) graphics.create();
            try {
                g.translate(x, y);
                g.setRenderingHint(RenderingHints.KEY_ANTIALIASING, RenderingHints.VALUE_ANTIALIAS_ON);
                g.setRenderingHint(RenderingHints.KEY_STROKE_CONTROL, RenderingHints.VALUE_STROKE_PURE);
                g.setColor(component != null && !component.isEnabled() ? separator() : muted());
                g.setStroke(new BasicStroke(1.6f, BasicStroke.CAP_ROUND, BasicStroke.JOIN_ROUND));
                // two chevrons on a 16 px grid: apart for expand, meeting in the middle for collapse
                if (this == EXPAND_ALL) {
                    chevron(g, 2.5f, false);
                    chevron(g, 13.5f, true);
                } else {
                    chevron(g, 6, true);
                    chevron(g, 10, false);
                }
            } finally {
                g.dispose();
            }
        }

        /** A chevron 7 px wide whose tip is at {@code tipY}, pointing down or up. */
        private static void chevron(Graphics2D g, float tipY, boolean down) {
            float armY = down ? tipY - 3.5f : tipY + 3.5f;
            java.awt.geom.Path2D path = new java.awt.geom.Path2D.Float();
            path.moveTo(4.5f, armY);
            path.lineTo(8, tipY);
            path.lineTo(11.5f, armY);
            g.draw(path);
        }

        @Override
        public int getIconWidth() {
            return 16;
        }

        @Override
        public int getIconHeight() {
            return 16;
        }
    }

    private TuningStyle() {
    }
}
