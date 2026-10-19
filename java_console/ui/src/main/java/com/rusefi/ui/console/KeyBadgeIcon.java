package com.rusefi.ui.console;

import javax.swing.*;
import java.awt.*;
import java.awt.font.FontRenderContext;
import java.awt.geom.Rectangle2D;

/**
 * Keyboard key drawn as a small outlined badge, for example the F1 beside "Shortcuts" in the
 * title bar. Colours follow the active theme [tag:console_theme].
 */
class KeyBadgeIcon implements Icon {
    private static final int PAD_X = 5;
    private static final int PAD_Y = 1;
    private static final FontRenderContext FRC = new FontRenderContext(null, true, true);

    private final String key;

    KeyBadgeIcon(String key) {
        this.key = key;
    }

    private static Font font() {
        Font base = UIManager.getFont("Label.font");
        int size = base != null ? Math.max(base.getSize() - 2, 9) : 11;
        return new Font(Font.MONOSPACED, Font.PLAIN, size);
    }

    private Rectangle2D textBounds() {
        return font().getStringBounds(key, FRC);
    }

    @Override
    public int getIconWidth() {
        return (int) Math.ceil(textBounds().getWidth()) + 2 * PAD_X;
    }

    @Override
    public int getIconHeight() {
        return (int) Math.ceil(textBounds().getHeight()) + 2 * PAD_Y;
    }

    @Override
    public void paintIcon(Component c, Graphics g, int x, int y) {
        Graphics2D g2 = (Graphics2D) g.create();
        try {
            g2.setRenderingHint(RenderingHints.KEY_ANTIALIASING, RenderingHints.VALUE_ANTIALIAS_ON);
            g2.setRenderingHint(RenderingHints.KEY_TEXT_ANTIALIASING, RenderingHints.VALUE_TEXT_ANTIALIAS_ON);
            int w = getIconWidth();
            int h = getIconHeight();
            Color border = UIManager.getColor("Component.borderColor");
            g2.setColor(border != null ? border : Color.GRAY);
            g2.drawRoundRect(x, y, w - 1, h - 1, 6, 6);
            g2.setFont(font());
            g2.setColor(TitleBarPanel.mutedForeground());
            FontMetrics fm = g2.getFontMetrics();
            g2.drawString(key, x + PAD_X, y + PAD_Y + fm.getAscent());
        } finally {
            g2.dispose();
        }
    }
}
