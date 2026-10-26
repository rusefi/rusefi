package com.rusefi.ui.widgets;

import javax.swing.*;
import java.awt.*;

/**
 * White strip with a divider along its bottom edge, like the Gauges toolbar and the status bar
 * [tag:console_theme]. Children should be transparent so the strip's background shows.
 */
public class ToolbarStrip extends JPanel {
    public ToolbarStrip(LayoutManager layout) {
        super(layout);
    }

    @Override
    protected void paintComponent(Graphics g) {
        Color background = UIManager.getColor("Console.Toolbar.background");
        g.setColor(background != null ? background : getBackground());
        g.fillRect(0, 0, getWidth(), getHeight());
        Color line = UIManager.getColor("Separator.foreground");
        g.setColor(line != null ? line : Color.LIGHT_GRAY);
        g.drawLine(0, getHeight() - 1, getWidth(), getHeight() - 1);
    }
}
