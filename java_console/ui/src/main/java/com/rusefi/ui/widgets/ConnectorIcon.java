package com.rusefi.ui.widgets;

import javax.swing.*;
import java.awt.*;
import java.awt.geom.Rectangle2D;
import java.awt.geom.RoundRectangle2D;

/**
 * Connector face as the Pinout drawings show it: a housing with a latch on top and two rows of
 * square cavities, drawn in the owning component's text colour (greyed when disabled), e.g. on
 * the Pinout button beside pin settings.
 */
public class ConnectorIcon implements Icon {
    private static final int WIDTH = 20;
    private static final int HEIGHT = 16;
    private static final int COLUMNS = 4;

    @Override
    public void paintIcon(Component c, Graphics g, int x, int y) {
        Graphics2D g2 = (Graphics2D) g.create();
        try {
            g2.setRenderingHint(RenderingHints.KEY_ANTIALIASING, RenderingHints.VALUE_ANTIALIAS_ON);
            Color color = c == null ? Color.DARK_GRAY // [tag:fixed_color] no component to take a colour from
                : c.isEnabled() ? c.getForeground() : UIManager.getColor("Label.disabledForeground");
            g2.setColor(color != null ? color : c.getForeground());
            g2.setStroke(new BasicStroke(1.4f, BasicStroke.CAP_ROUND, BasicStroke.JOIN_ROUND));
            g2.draw(new RoundRectangle2D.Double(x + 0.7, y + 4.2, 18.6, 11.1, 3, 3));
            g2.draw(new RoundRectangle2D.Double(x + 7, y + 0.7, 6, 3.5, 1.5, 1.5));
            for (int row = 0; row < 2; row++) {
                for (int column = 0; column < COLUMNS; column++) {
                    g2.fill(new Rectangle2D.Double(x + 3.2 + column * 3.75, y + 6.6 + row * 4, 2.4, 2.4));
                }
            }
        } finally {
            g2.dispose();
        }
    }

    @Override
    public int getIconWidth() {
        return WIDTH;
    }

    @Override
    public int getIconHeight() {
        return HEIGHT;
    }
}
