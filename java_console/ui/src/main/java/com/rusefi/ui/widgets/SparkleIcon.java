package com.rusefi.ui.widgets;

import javax.swing.*;
import java.awt.*;
import java.awt.geom.Path2D;

/** Two four-point sparkles, the usual "AI" symbol, e.g. on the AI Troubleshooting tab. */
public class SparkleIcon implements Icon {
    private static final Color COLOR = new Color(0x7B5CE6);
    private final int size;

    public SparkleIcon(int size) {
        this.size = size;
    }

    @Override
    public void paintIcon(Component c, Graphics g, int x, int y) {
        Graphics2D g2 = (Graphics2D) g.create();
        try {
            g2.setRenderingHint(RenderingHints.KEY_ANTIALIASING, RenderingHints.VALUE_ANTIALIAS_ON);
            g2.translate(x, y);
            g2.scale(size / 18.0, size / 18.0);
            g2.setColor(c != null && !c.isEnabled() ? UIManager.getColor("Label.disabledForeground") : COLOR);
            g2.fill(sparkle(7.5, 10.5, 7));
            g2.fill(sparkle(14, 4, 3.5));
        } finally {
            g2.dispose();
        }
    }

    /** Four points joined by curves pulled towards the centre. */
    private static Shape sparkle(double cx, double cy, double r) {
        double k = r * 0.18;
        Path2D.Double p = new Path2D.Double();
        p.moveTo(cx, cy - r);
        p.quadTo(cx + k, cy - k, cx + r, cy);
        p.quadTo(cx + k, cy + k, cx, cy + r);
        p.quadTo(cx - k, cy + k, cx - r, cy);
        p.quadTo(cx - k, cy - k, cx, cy - r);
        p.closePath();
        return p;
    }

    @Override
    public int getIconWidth() {
        return size;
    }

    @Override
    public int getIconHeight() {
        return size;
    }
}
