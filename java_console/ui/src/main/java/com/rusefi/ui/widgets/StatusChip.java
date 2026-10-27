package com.rusefi.ui.widgets;

import javax.swing.*;
import java.awt.*;

/**
 * Read-only status badge: a coloured dot and a short text on a rounded background, like the
 * connection badge in the tab row. Colours come from the Console.Status.* theme keys
 * [tag:console_theme], with the light theme's values as fallbacks.
 */
public class StatusChip extends JLabel {
    private static final int DOT = 8;

    public enum Tone {
        /** Running or healthy, e.g. "Capturing". */
        GOOD("connected", "#16a34a", "#e7f6ec", "#15803d"),
        /** Needs attention, e.g. "Not supported". */
        WARNING("pending", "#d97706", "#fdf3e3", "#b45309"),
        /** Unavailable, e.g. "Not connected". */
        BAD("disconnected", "#dc2626", "#fdecec", "#b91c1c"),
        /** Idle, e.g. "Stopped". */
        NEUTRAL(null, "#9aa3af", "#eef0f3", "#586273"),
        /** Points at what the user is inspecting, e.g. "At cursor". */
        ACCENT(null, "#2563eb", "#dbeafe", "#1d4ed8");

        private final String key;
        private final Color dot;
        private final Color background;
        private final Color foreground;

        Tone(String key, String dot, String background, String foreground) {
            this.key = key;
            this.dot = Color.decode(dot);
            this.background = Color.decode(background);
            this.foreground = Color.decode(foreground);
        }

        private Color themed(String part, Color fallback) {
            Color c = key == null ? null : UIManager.getColor("Console.Status." + key + "." + part);
            return c != null ? c : fallback;
        }
    }

    private Tone tone = Tone.NEUTRAL;

    public StatusChip() {
        setIconTextGap(8);
        setBorder(BorderFactory.createEmptyBorder(5, 12, 5, 12));
        setIcon(new Icon() {
            @Override
            public void paintIcon(Component c, Graphics g, int x, int y) {
                Graphics2D g2 = (Graphics2D) g.create();
                try {
                    g2.setRenderingHint(RenderingHints.KEY_ANTIALIASING, RenderingHints.VALUE_ANTIALIAS_ON);
                    g2.setColor(tone.themed("dot", tone.dot));
                    g2.fillOval(x, y, DOT, DOT);
                } finally {
                    g2.dispose();
                }
            }

            @Override
            public int getIconWidth() {
                return DOT;
            }

            @Override
            public int getIconHeight() {
                return DOT;
            }
        });
        updateFont();
    }

    public void show(String text, Tone tone) {
        this.tone = tone;
        setText(text);
        setForeground(tone.themed("foreground", tone.foreground));
        repaint();
    }

    public Tone getTone() {
        return tone;
    }

    @Override
    public void updateUI() {
        super.updateUI();
        updateFont();
    }

    private void updateFont() {
        Font base = UIManager.getFont("Label.font");
        if (base != null) {
            setFont(base.deriveFont(Font.BOLD));
        }
    }

    @Override
    protected void paintComponent(Graphics g) {
        Graphics2D g2 = (Graphics2D) g.create();
        try {
            g2.setRenderingHint(RenderingHints.KEY_ANTIALIASING, RenderingHints.VALUE_ANTIALIAS_ON);
            g2.setColor(tone.themed("background", tone.background));
            g2.fillRoundRect(0, 0, getWidth(), getHeight(), getHeight(), getHeight());
        } finally {
            g2.dispose();
        }
        super.paintComponent(g);
    }
}
