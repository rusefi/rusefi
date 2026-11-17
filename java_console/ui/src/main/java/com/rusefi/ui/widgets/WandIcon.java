package com.rusefi.ui.widgets;

import javax.swing.*;
import java.awt.*;
import java.awt.geom.Path2D;

/** Outline magic wand with a sparkle, drawn in the owning button's text colour, e.g. on "Launch Wizard". */
public class WandIcon implements Icon {
    private static final int SIZE = 16;

    @Override
    public void paintIcon(Component c, Graphics g, int x, int y) {
        Graphics2D g2 = (Graphics2D) g.create();
        try {
            g2.setRenderingHint(RenderingHints.KEY_ANTIALIASING, RenderingHints.VALUE_ANTIALIAS_ON);
            g2.setColor(c != null ? c.getForeground() : Color.DARK_GRAY);
            g2.setStroke(new BasicStroke(1.5f, BasicStroke.CAP_ROUND, BasicStroke.JOIN_ROUND));
            // wand
            g2.drawLine(x + 2, y + 14, x + 10, y + 6);
            // sparkle at the tip
            Path2D sparkle = new Path2D.Float();
            sparkle.moveTo(x + 12, y + 1.5);
            sparkle.lineTo(x + 12, y + 6.5);
            sparkle.moveTo(x + 9.5, y + 4);
            sparkle.lineTo(x + 14.5, y + 4);
            g2.draw(sparkle);
            g2.fillOval(x + 14, y + 8, 2, 2);
            g2.fillOval(x + 6, y + 1, 2, 2);
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
