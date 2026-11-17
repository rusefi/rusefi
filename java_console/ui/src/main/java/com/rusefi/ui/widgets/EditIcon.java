package com.rusefi.ui.widgets;

import javax.swing.*;
import java.awt.*;
import java.awt.geom.Path2D;

/** Outline pencil, drawn in the owning component's text colour, e.g. on "Edit dashboard". */
public class EditIcon implements Icon {
    private static final int SIZE = 16;

    @Override
    public void paintIcon(Component c, Graphics g, int x, int y) {
        Graphics2D g2 = (Graphics2D) g.create();
        try {
            g2.setRenderingHint(RenderingHints.KEY_ANTIALIASING, RenderingHints.VALUE_ANTIALIAS_ON);
            g2.setColor(c != null ? c.getForeground() : Color.DARK_GRAY);
            g2.setStroke(new BasicStroke(1.5f, BasicStroke.CAP_ROUND, BasicStroke.JOIN_ROUND));
            Path2D pencil = new Path2D.Float();
            pencil.moveTo(x + 2.5, y + 13.5);
            pencil.lineTo(x + 3.2, y + 10.2);
            pencil.lineTo(x + 11, y + 2.4);
            pencil.lineTo(x + 13.6, y + 5);
            pencil.lineTo(x + 5.8, y + 12.8);
            pencil.closePath();
            g2.draw(pencil);
            g2.drawLine(x + 9, y + 4, x + 12, y + 7);
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
