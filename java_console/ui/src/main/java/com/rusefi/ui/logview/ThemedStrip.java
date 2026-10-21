package com.rusefi.ui.logview;

import javax.swing.JPanel;
import javax.swing.UIManager;
import java.awt.Color;
import java.awt.Graphics;
import java.awt.LayoutManager;

/**
 * A full-width bar filled with a theme colour and a divider on one edge, like the Gauges toolbar
 * (divider below) and status strip (divider above). [tag:console_theme]
 */
class ThemedStrip extends JPanel {
    private final String backgroundKey;
    private final boolean dividerOnTop;

    ThemedStrip(LayoutManager layout, String backgroundKey, boolean dividerOnTop) {
        super(layout);
        this.backgroundKey = backgroundKey;
        this.dividerOnTop = dividerOnTop;
    }

    static Color themed(String key, Color fallback) {
        Color color = UIManager.getColor(key);
        return color != null ? color : fallback;
    }

    @Override
    protected void paintComponent(Graphics g) {
        g.setColor(themed(backgroundKey, getBackground()));
        g.fillRect(0, 0, getWidth(), getHeight());
        g.setColor(themed("Separator.foreground", Color.LIGHT_GRAY));
        int y = dividerOnTop ? 0 : getHeight() - 1;
        g.drawLine(0, y, getWidth(), y);
    }
}
