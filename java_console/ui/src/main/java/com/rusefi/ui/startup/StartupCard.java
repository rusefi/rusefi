package com.rusefi.ui.startup;

import javax.swing.*;
import java.awt.*;

/**
 * White rounded card holding the startup Connect controls, centred on the grey page.
 * Colours: Console.Card.* theme keys [tag:console_theme].
 */
public class StartupCard extends JPanel {
    /** Width of the card's content column; banners and rows fill it. */
    public static final int CONTENT_WIDTH = 560;
    private static final int ARC = 16;
    private static final int PAD_X = 40;
    private static final int PAD_Y = 32;

    public StartupCard() {
        setLayout(new BoxLayout(this, BoxLayout.Y_AXIS));
        setOpaque(false);
        setBorder(BorderFactory.createEmptyBorder(PAD_Y, PAD_X, PAD_Y, PAD_X));
    }

    /** Adds a full-width row, centred, with vertical space above it. */
    public void addRow(JComponent row, int gapAbove) {
        if (gapAbove > 0 && getComponentCount() > 0) {
            add(Box.createVerticalStrut(gapAbove));
        }
        row.setAlignmentX(CENTER_ALIGNMENT);
        add(row);
    }

    /** A thin horizontal divider across the content column. */
    public static JComponent divider() {
        JComponent line = new JComponent() {
            @Override
            protected void paintComponent(Graphics g) {
                g.setColor(themed("Separator.foreground", new Color(0xdfe3e9)));
                g.drawLine(0, getHeight() / 2, getWidth(), getHeight() / 2);
            }
        };
        Dimension size = new Dimension(CONTENT_WIDTH, 1);
        line.setPreferredSize(size);
        line.setMaximumSize(new Dimension(Integer.MAX_VALUE, 1));
        return line;
    }

    @Override
    public Dimension getMaximumSize() {
        return getPreferredSize();
    }

    @Override
    protected void paintComponent(Graphics g) {
        Graphics2D g2 = (Graphics2D) g.create();
        try {
            g2.setRenderingHint(RenderingHints.KEY_ANTIALIASING, RenderingHints.VALUE_ANTIALIAS_ON);
            g2.setColor(themed("Console.Card.background", Color.WHITE));
            g2.fillRoundRect(0, 0, getWidth() - 1, getHeight() - 1, ARC, ARC);
            g2.setColor(themed("Console.Card.borderColor", new Color(0xdfe3e9)));
            g2.drawRoundRect(0, 0, getWidth() - 1, getHeight() - 1, ARC, ARC);
        } finally {
            g2.dispose();
        }
    }

    public static Color themed(String key, Color fallback) {
        Color c = UIManager.getColor(key);
        return c != null ? c : fallback;
    }

    /** Wraps a row so it is exactly the content width (BoxLayout otherwise stretches or shrinks it). */
    public static JPanel fullWidth(JComponent content) {
        JPanel row = new JPanel(new BorderLayout()) {
            @Override
            public Dimension getPreferredSize() {
                return new Dimension(CONTENT_WIDTH, super.getPreferredSize().height);
            }

            @Override
            public Dimension getMaximumSize() {
                return getPreferredSize();
            }
        };
        row.setOpaque(false);
        row.add(content, BorderLayout.CENTER);
        return row;
    }
}
