package com.rusefi.ui.startup;

import javax.swing.*;
import java.awt.*;
import java.awt.event.ComponentAdapter;
import java.awt.event.ComponentEvent;
import java.awt.geom.Ellipse2D;

/**
 * Tinted message strip in the startup card: a round icon, the message component, and an optional
 * action on the right. The banner shows and hides together with its message component, so code that
 * toggles the message's visibility keeps working unchanged.
 * Colours: Console.Banner.&lt;kind&gt;.* theme keys [tag:console_theme].
 */
public class Banner extends JPanel {
    public enum Kind {
        INFO("info", "#eef4ff", "#c9dafb", "#1d4ed8", 'i'),
        WARNING("warning", "#fff8e6", "#f3dca7", "#b7791f", '!'),
        ERROR("error", "#fdecec", "#f5c2c2", "#b91c1c", '!');

        private final String key;
        private final Color background;
        private final Color border;
        private final Color accent;
        private final char symbol;

        Kind(String key, String background, String border, String accent, char symbol) {
            this.key = key;
            this.background = Color.decode(background);
            this.border = Color.decode(border);
            this.accent = Color.decode(accent);
            this.symbol = symbol;
        }

        Color background() {
            return StartupCard.themed("Console.Banner." + key + ".background", background);
        }

        Color border() {
            return StartupCard.themed("Console.Banner." + key + ".borderColor", border);
        }

        Color accent() {
            return StartupCard.themed("Console.Banner." + key + ".accent", accent);
        }
    }

    private static final int ARC = 10;
    private final Kind kind;

    public Banner(Kind kind, JComponent message, JComponent action) {
        super(new BorderLayout(12, 0));
        this.kind = kind;
        setOpaque(false);
        setBorder(BorderFactory.createEmptyBorder(10, 14, 10, 14));
        add(new JLabel(new SymbolIcon(kind)), BorderLayout.WEST);
        message.setOpaque(false);
        add(message, BorderLayout.CENTER);
        if (action != null) {
            add(action, BorderLayout.EAST);
        }
        setVisible(message.isVisible());
        message.addComponentListener(new ComponentAdapter() {
            @Override
            public void componentShown(ComponentEvent e) {
                setVisible(true);
            }

            @Override
            public void componentHidden(ComponentEvent e) {
                setVisible(false);
            }
        });
    }

    @Override
    public Dimension getPreferredSize() {
        return new Dimension(StartupCard.CONTENT_WIDTH, super.getPreferredSize().height);
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
            g2.setColor(kind.background());
            g2.fillRoundRect(0, 0, getWidth() - 1, getHeight() - 1, ARC, ARC);
            g2.setColor(kind.border());
            g2.drawRoundRect(0, 0, getWidth() - 1, getHeight() - 1, ARC, ARC);
        } finally {
            g2.dispose();
        }
    }

    /** Filled circle in the banner colour with a white "i" or "!". */
    private static class SymbolIcon implements Icon {
        private static final int SIZE = 20;
        private final Kind kind;

        SymbolIcon(Kind kind) {
            this.kind = kind;
        }

        @Override
        public void paintIcon(Component c, Graphics g, int x, int y) {
            Graphics2D g2 = (Graphics2D) g.create();
            try {
                g2.setRenderingHint(RenderingHints.KEY_ANTIALIASING, RenderingHints.VALUE_ANTIALIAS_ON);
                g2.setRenderingHint(RenderingHints.KEY_TEXT_ANTIALIASING, RenderingHints.VALUE_TEXT_ANTIALIAS_ON);
                g2.setColor(kind.accent());
                g2.fill(new Ellipse2D.Float(x, y, SIZE, SIZE));
                g2.setColor(Color.WHITE);
                g2.setFont(c.getFont().deriveFont(Font.BOLD, 13f));
                FontMetrics fm = g2.getFontMetrics();
                String s = String.valueOf(kind.symbol);
                g2.drawString(s, x + (SIZE - fm.stringWidth(s)) / 2f, y + (SIZE + fm.getAscent() - fm.getDescent()) / 2f);
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
}
