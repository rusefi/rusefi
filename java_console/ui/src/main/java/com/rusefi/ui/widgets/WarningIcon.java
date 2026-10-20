package com.rusefi.ui.widgets;

import javax.swing.*;
import java.awt.*;
import java.awt.geom.Path2D;

/** Outline warning triangle drawn in the owning label's text colour (grey when idle, red on a warning). */
public class WarningIcon implements Icon {
    private static final int SIZE = 18;

    @Override
    public void paintIcon(Component c, Graphics g, int x, int y) {
        Graphics2D g2 = (Graphics2D) g.create();
        try {
            g2.setRenderingHint(RenderingHints.KEY_ANTIALIASING, RenderingHints.VALUE_ANTIALIAS_ON);
            g2.setColor(c != null ? c.getForeground() : Color.GRAY);
            g2.setStroke(new BasicStroke(1.6f, BasicStroke.CAP_ROUND, BasicStroke.JOIN_ROUND));
            Path2D triangle = new Path2D.Float();
            triangle.moveTo(x + SIZE / 2.0, y + 2);
            triangle.lineTo(x + SIZE - 1.5, y + SIZE - 2);
            triangle.lineTo(x + 1.5, y + SIZE - 2);
            triangle.closePath();
            g2.draw(triangle);
            g2.drawLine(x + SIZE / 2, y + 7, x + SIZE / 2, y + 10);
            g2.fillOval(x + SIZE / 2 - 1, y + 12, 2, 2);
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
