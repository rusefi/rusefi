package com.rusefi.ui.laf;

import javax.swing.*;
import javax.swing.border.AbstractBorder;
import java.awt.*;
import java.awt.event.MouseAdapter;
import java.awt.event.MouseEvent;

public class GradientTitleBorder extends AbstractBorder {
    private final String title;
    private boolean mouseOver = false;
    private static final int TITLE_HEIGHT = 25;
    private static final int HORIZONTAL_PADDING = 5;

    public GradientTitleBorder(String title) {
        this.title = title;
    }

    public static void installBorder(String uiName, JPanel panelWidget) {
        GradientTitleBorder border = new GradientTitleBorder(uiName);
        panelWidget.setBorder(border);
        border.installMouseListener(panelWidget);
    }

    public void installMouseListener(Component c) {
        c.addMouseListener(new MouseAdapter() {
            @Override
            public void mouseEntered(MouseEvent e) {
                mouseOver = true;
                c.repaint();
            }

            @Override
            public void mouseExited(MouseEvent e) {
                mouseOver = false;
                c.repaint();
            }
        });
    }

    public String getTitle() {
        return title;
    }

    @Override
    public void paintBorder(Component c, Graphics g, int x, int y, int width, int height) {
        Graphics2D g2 = (Graphics2D) g.create();
        g2.setRenderingHint(RenderingHints.KEY_ANTIALIASING, RenderingHints.VALUE_ANTIALIAS_ON);

        Color borderColor = mouseOver ? new Color(180, 120, 0) : Color.GRAY;
        g2.setColor(borderColor);

        // Draw outer rectangle (TitledBorder style)
        g2.drawRect(x, y, width - 1, height - 1);

        // Measure title text so we can blank the top-border line behind it
        Font titleFont = g2.getFont().deriveFont(Font.PLAIN, 12f);
        g2.setFont(titleFont);
        FontMetrics fm = g2.getFontMetrics();
        int textX = x + HORIZONTAL_PADDING + 2;
        int textY = y + fm.getAscent();

        // White-fill gap behind title string to "cut" through the top border line
        int textWidth = fm.stringWidth(title);
        g2.setColor(c.getBackground() != null ? c.getBackground() : Color.WHITE);
        g2.fillRect(textX - 2, y, textWidth + 4, 1);

        // Draw title text in dark gray
        g2.setColor(new Color(60, 60, 60));
        g2.drawString(title, textX, textY);

        g2.dispose();
    }

    @Override
    public Insets getBorderInsets(Component c) {
        return new Insets(18, 4, 4, 4);
    }

    @Override
    public Insets getBorderInsets(Component c, Insets insets) {
        insets.set(18, 4, 4, 4);
        return insets;
    }
}
