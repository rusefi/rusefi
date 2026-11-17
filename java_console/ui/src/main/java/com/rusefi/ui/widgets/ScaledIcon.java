package com.rusefi.ui.widgets;

import javax.swing.*;
import java.awt.*;

/** Draws a small line icon larger, e.g. a 16 px {@link CommandIcon} at 40 px on a message card. */
public final class ScaledIcon implements Icon {
    private final Icon icon;
    private final double scale;

    public ScaledIcon(Icon icon, double scale) {
        this.icon = icon;
        this.scale = scale;
    }

    @Override
    public void paintIcon(Component c, Graphics g, int x, int y) {
        Graphics2D g2 = (Graphics2D) g.create();
        try {
            g2.translate(x, y);
            g2.scale(scale, scale);
            icon.paintIcon(c, g2, 0, 0);
        } finally {
            g2.dispose();
        }
    }

    @Override
    public int getIconWidth() {
        return (int) Math.round(icon.getIconWidth() * scale);
    }

    @Override
    public int getIconHeight() {
        return (int) Math.round(icon.getIconHeight() * scale);
    }
}
