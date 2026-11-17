package com.rusefi.ui.llm;

import javax.swing.Icon;
import javax.swing.UIManager;
import java.awt.BasicStroke;
import java.awt.Color;
import java.awt.Component;
import java.awt.Font;
import java.awt.FontMetrics;
import java.awt.Graphics;
import java.awt.Graphics2D;
import java.awt.RenderingHints;
import java.awt.geom.Path2D;

/** Small painted icons of the troubleshooting tab; colours come from the console theme when painted. */
final class TroubleshootingIcons {
    private TroubleshootingIcons() {
    }

    /** How far the user is with one setup step. */
    enum StepState {
        /** finished: a tick */
        DONE("Console.Status.connected"),
        /** the step to do now: its number, highlighted */
        CURRENT("Console.Status.accent"),
        /** not reachable yet: its number, muted */
        LATER("Console.Status.neutral");

        final String colorKey;

        StepState(String colorKey) {
            this.colorKey = colorKey;
        }
    }

    /** A round badge with the step number, or a tick once done. */
    static final class StepBadge implements Icon {
        private static final int SIZE = 22;
        private final int number;
        StepState state = StepState.LATER;

        StepBadge(int number) {
            this.number = number;
        }

        @Override
        public void paintIcon(Component c, Graphics g, int x, int y) {
            Graphics2D g2 = start(g);
            try {
                g2.setColor(themed(state.colorKey + ".background", Color.LIGHT_GRAY));
                g2.fillOval(x, y, SIZE, SIZE);
                g2.setColor(themed(state.colorKey + ".foreground", Color.DARK_GRAY));
                if (state == StepState.DONE) {
                    g2.setStroke(new BasicStroke(2.2f, BasicStroke.CAP_ROUND, BasicStroke.JOIN_ROUND));
                    Path2D tick = new Path2D.Float();
                    tick.moveTo(x + 6.5, y + 11.5);
                    tick.lineTo(x + 9.8, y + 14.8);
                    tick.lineTo(x + 15.5, y + 8);
                    g2.draw(tick);
                } else {
                    Font font = c.getFont().deriveFont(Font.BOLD, c.getFont().getSize2D() - 1);
                    g2.setFont(font);
                    FontMetrics metrics = g2.getFontMetrics();
                    String text = String.valueOf(number);
                    g2.drawString(text, x + (SIZE - metrics.stringWidth(text)) / 2f,
                            y + (SIZE - metrics.getHeight()) / 2f + metrics.getAscent());
                }
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

    /** A status dot; {@link #colorKey} is one of the console's Console.Status.*.dot colours. */
    static final class Dot implements Icon {
        String colorKey = "Console.Status.neutral.dot";

        @Override
        public void paintIcon(Component c, Graphics g, int x, int y) {
            Graphics2D g2 = start(g);
            try {
                g2.setColor(themed(colorKey, Color.GRAY));
                g2.fillOval(x, y + 1, 7, 7);
            } finally {
                g2.dispose();
            }
        }

        @Override
        public int getIconWidth() {
            return 7;
        }

        @Override
        public int getIconHeight() {
            return 9;
        }
    }

    /** A round avatar with a person outline: highlighted while signed in, muted otherwise. */
    static final class Avatar implements Icon {
        private static final int SIZE = 30;
        boolean signedIn;

        @Override
        public void paintIcon(Component c, Graphics g, int x, int y) {
            Graphics2D g2 = start(g);
            try {
                String tone = signedIn ? "Console.Status.accent" : "Console.Status.neutral";
                g2.setColor(themed(tone + ".background", Color.LIGHT_GRAY));
                g2.fillOval(x, y, SIZE, SIZE);
                g2.setColor(themed(tone + ".foreground", Color.DARK_GRAY));
                g2.setStroke(new BasicStroke(1.6f, BasicStroke.CAP_ROUND, BasicStroke.JOIN_ROUND));
                g2.drawOval(x + 11, y + 7, 8, 8);
                g2.drawArc(x + 7, y + 17, 16, 12, 0, 180);
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

    /** A waste bin outline, drawn in the component's foreground or the muted icon colour. */
    static void paintTrash(Graphics g, int x, int y) {
        Graphics2D g2 = start(g);
        try {
            g2.setColor(themed("Console.Icon.neutral", Color.GRAY));
            g2.setStroke(new BasicStroke(1.5f, BasicStroke.CAP_ROUND, BasicStroke.JOIN_ROUND));
            g2.drawLine(x + 2, y + 4, x + 14, y + 4);
            g2.drawLine(x + 6, y + 4, x + 6, y + 2);
            g2.drawLine(x + 6, y + 2, x + 10, y + 2);
            g2.drawLine(x + 10, y + 2, x + 10, y + 4);
            Path2D bin = new Path2D.Float();
            bin.moveTo(x + 3.5, y + 4);
            bin.lineTo(x + 4.5, y + 15);
            bin.lineTo(x + 11.5, y + 15);
            bin.lineTo(x + 12.5, y + 4);
            g2.draw(bin);
            g2.drawLine(x + 7, y + 7, x + 7, y + 12);
            g2.drawLine(x + 9, y + 7, x + 9, y + 12);
        } finally {
            g2.dispose();
        }
    }

    /** What a tool chip stands for. */
    enum ToolKind {ECU, KNOWLEDGE, CASE, FILE, IMAGE}

    /** A chip's glyph in the chip's text colour: a chip for the ECU, a book for wiki/source, a disk for a saved case. */
    static final class ToolIcon implements Icon {
        private final ToolKind kind;

        ToolIcon(ToolKind kind) {
            this.kind = kind;
        }

        @Override
        public void paintIcon(Component c, Graphics g, int x, int y) {
            Graphics2D g2 = start(g);
            try {
                g2.setColor(c.getForeground());
                g2.setStroke(new BasicStroke(1.3f, BasicStroke.CAP_ROUND, BasicStroke.JOIN_ROUND));
                switch (kind) {
                    case ECU:
                        g2.drawRoundRect(x + 3, y + 3, 6, 6, 2, 2);
                        for (int i = 0; i < 2; i++) {
                            int p = 5 + 2 * i;
                            g2.drawLine(x + p, y + 1, x + p, y + 3);
                            g2.drawLine(x + p, y + 9, x + p, y + 11);
                            g2.drawLine(x + 1, y + p, x + 3, y + p);
                            g2.drawLine(x + 9, y + p, x + 11, y + p);
                        }
                        break;
                    case KNOWLEDGE:
                        g2.drawLine(x + 6, y + 3, x + 6, y + 10);
                        g2.drawPolyline(new int[]{x + 6, x + 4, x + 1, x + 1, x + 4, x + 6},
                                new int[]{y + 3, y + 2, y + 2, y + 9, y + 9, y + 10}, 6);
                        g2.drawPolyline(new int[]{x + 6, x + 8, x + 11, x + 11, x + 8, x + 6},
                                new int[]{y + 3, y + 2, y + 2, y + 9, y + 9, y + 10}, 6);
                        break;
                    case FILE:
                        g2.drawPolyline(new int[]{x + 7, x + 2, x + 2, x + 10, x + 10, x + 7, x + 7, x + 10},
                                new int[]{y + 1, y + 1, y + 11, y + 11, y + 4, y + 1, y + 4, y + 4}, 8);
                        break;
                    case IMAGE:
                        g2.drawRoundRect(x + 1, y + 2, 10, 8, 2, 2);
                        g2.drawPolyline(new int[]{x + 2, x + 5, x + 7, x + 8, x + 10}, new int[]{y + 9, y + 6, y + 8, y + 7, y + 9}, 5);
                        g2.fillOval(x + 7, y + 3, 2, 2);
                        break;
                    default:
                        g2.drawLine(x + 6, y + 1, x + 6, y + 7);
                        g2.drawPolyline(new int[]{x + 3, x + 6, x + 9}, new int[]{y + 5, y + 8, y + 5}, 3);
                        g2.drawLine(x + 2, y + 11, x + 10, y + 11);
                        break;
                }
            } finally {
                g2.dispose();
            }
        }

        @Override
        public int getIconWidth() {
            return 12;
        }

        @Override
        public int getIconHeight() {
            return 12;
        }
    }

    /** A paperclip in the button's text colour, for attaching files. */
    static final class Paperclip implements Icon {
        @Override
        public void paintIcon(Component c, Graphics g, int x, int y) {
            Graphics2D g2 = start(g);
            try {
                g2.setColor(c.isEnabled() ? c.getForeground() : themed("Button.disabledText", Color.GRAY));
                g2.setStroke(new BasicStroke(1.5f, BasicStroke.CAP_ROUND, BasicStroke.JOIN_ROUND));
                Path2D clip = new Path2D.Float();
                clip.moveTo(x + 11, y + 5);
                clip.lineTo(x + 11, y + 12);
                clip.curveTo(x + 11, y + 16.5, x + 4, y + 16.5, x + 4, y + 12);
                clip.lineTo(x + 4, y + 4);
                clip.curveTo(x + 4, y + 0.5, x + 9, y + 0.5, x + 9, y + 4);
                clip.lineTo(x + 9, y + 11.5);
                clip.curveTo(x + 9, y + 13.5, x + 6, y + 13.5, x + 6, y + 11.5);
                clip.lineTo(x + 6, y + 5);
                g2.draw(clip);
            } finally {
                g2.dispose();
            }
        }

        @Override
        public int getIconWidth() {
            return 15;
        }

        @Override
        public int getIconHeight() {
            return 16;
        }
    }

    /** A plus sign in the button's text colour, for "new" actions. */
    static final class Plus implements Icon {
        @Override
        public void paintIcon(Component c, Graphics g, int x, int y) {
            Graphics2D g2 = start(g);
            try {
                g2.setColor(c.isEnabled() ? c.getForeground() : themed("Button.disabledText", Color.GRAY));
                g2.setStroke(new BasicStroke(1.8f, BasicStroke.CAP_ROUND, BasicStroke.JOIN_ROUND));
                g2.drawLine(x + 6, y + 1, x + 6, y + 11);
                g2.drawLine(x + 1, y + 6, x + 11, y + 6);
            } finally {
                g2.dispose();
            }
        }

        @Override
        public int getIconWidth() {
            return 12;
        }

        @Override
        public int getIconHeight() {
            return 12;
        }
    }

    /** A small downward chevron, for a button that opens a menu. */
    static final class Chevron implements Icon {
        @Override
        public void paintIcon(Component c, Graphics g, int x, int y) {
            Graphics2D g2 = start(g);
            try {
                g2.setColor(c.isEnabled() ? c.getForeground() : themed("Label.disabledForeground", Color.GRAY));
                g2.setStroke(new BasicStroke(1.6f, BasicStroke.CAP_ROUND, BasicStroke.JOIN_ROUND));
                Path2D path = new Path2D.Float();
                path.moveTo(x + 2, y + 4);
                path.lineTo(x + 6, y + 8);
                path.lineTo(x + 10, y + 4);
                g2.draw(path);
            } finally {
                g2.dispose();
            }
        }

        @Override
        public int getIconWidth() {
            return 12;
        }

        @Override
        public int getIconHeight() {
            return 12;
        }
    }

    private static Graphics2D start(Graphics g) {
        Graphics2D g2 = (Graphics2D) g.create();
        g2.setRenderingHint(RenderingHints.KEY_ANTIALIASING, RenderingHints.VALUE_ANTIALIAS_ON);
        g2.setRenderingHint(RenderingHints.KEY_TEXT_ANTIALIASING, RenderingHints.VALUE_TEXT_ANTIALIAS_ON);
        g2.setRenderingHint(RenderingHints.KEY_STROKE_CONTROL, RenderingHints.VALUE_STROKE_PURE);
        return g2;
    }

    static Color themed(String key, Color fallback) {
        Color color = UIManager.getColor(key);
        return color != null ? color : fallback;
    }
}
