package com.rusefi.ui.startup;

import javax.swing.*;
import java.awt.*;
import java.awt.geom.Ellipse2D;

/** Round status dot drawn in the owning label's text colour, so it follows every status change. */
public class StatusDotIcon implements Icon {
    private static final int SIZE = 12;

    @Override
    public void paintIcon(Component c, Graphics g, int x, int y) {
        Graphics2D g2 = (Graphics2D) g.create();
        try {
            g2.setRenderingHint(RenderingHints.KEY_ANTIALIASING, RenderingHints.VALUE_ANTIALIAS_ON);
            g2.setColor(c.getForeground());
            g2.fill(new Ellipse2D.Float(x, y, SIZE, SIZE));
        } finally {
            g2.dispose();
        }
    }

    @Override
    public int getIconWidth() {
        return SIZE;
    }

    @Override
    public int getIconHeight() {
        return SIZE;
    }
}
