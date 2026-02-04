package com.rusefi.ui.laf;

import javax.swing.border.AbstractBorder;
import java.awt.*;

public class GradientTitleBorder extends AbstractBorder {
    private final String title;
    private static final int TITLE_HEIGHT = 25;
    private static final int HORIZONTAL_PADDING = 5;

    public GradientTitleBorder(String title) {
        this.title = title;
    }

    public String getTitle() {
        return title;
    }

    @Override
    public void paintBorder(Component c, Graphics g, int x, int y, int width, int height) {
        Graphics2D g2 = (Graphics2D) g.create();
        g2.setRenderingHint(RenderingHints.KEY_ANTIALIASING, RenderingHints.VALUE_ANTIALIAS_ON);

        // Draw title background gradient
        GradientPaint gp = new GradientPaint(x, y, Color.BLACK, x + width, y, Color.ORANGE);
        g2.setPaint(gp);
        g2.fillRect(x, y, width, TITLE_HEIGHT);

        // Draw title text
        g2.setColor(Color.WHITE);
        FontMetrics fm = g2.getFontMetrics();
        int textY = y + ((TITLE_HEIGHT - fm.getHeight()) / 2) + fm.getAscent();
        g2.drawString(title, x + HORIZONTAL_PADDING, textY);

        // Draw border lines (left, right, bottom)
        g2.setColor(Color.GRAY);
        // Left
        g2.drawLine(x, y + TITLE_HEIGHT, x, y + height - 1);
        // Right
        g2.drawLine(x + width - 1, y + TITLE_HEIGHT, x + width - 1, y + height - 1);
        // Bottom
        g2.drawLine(x, y + height - 1, x + width - 1, y + height - 1);

        g2.dispose();
    }

    @Override
    public Insets getBorderInsets(Component c) {
        return new Insets(TITLE_HEIGHT + 2, 2, 2, 2);
    }

    @Override
    public Insets getBorderInsets(Component c, Insets insets) {
        insets.set(TITLE_HEIGHT + 2, 2, 2, 2);
        return insets;
    }
}
