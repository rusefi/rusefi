package com.rusefi.ui.logview;

import javax.swing.Icon;
import java.awt.BasicStroke;
import java.awt.Color;
import java.awt.Component;
import java.awt.Graphics;
import java.awt.Graphics2D;
import java.awt.RenderingHints;

/** A filled dot in the trace colour for plotted signals, an empty ring for the rest. */
final class TraceDot implements Icon {
    private static final int SIZE = 10;
    private Color fill;
    private Color ring;

    void set(Color fill, Color ring) {
        this.fill = fill;
        this.ring = ring;
    }

    @Override
    public void paintIcon(Component c, Graphics g, int x, int y) {
        Graphics2D g2 = (Graphics2D) g.create();
        try {
            g2.setRenderingHint(RenderingHints.KEY_ANTIALIASING, RenderingHints.VALUE_ANTIALIAS_ON);
            if (fill != null) {
                g2.setColor(fill);
                g2.fillOval(x, y, SIZE, SIZE);
            }
            g2.setColor(ring);
            g2.setStroke(new BasicStroke(1.2f));
            g2.drawOval(x, y, SIZE - 1, SIZE - 1);
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
