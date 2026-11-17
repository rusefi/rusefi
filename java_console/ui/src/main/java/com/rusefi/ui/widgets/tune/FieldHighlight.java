package com.rusefi.ui.widgets.tune;

import javax.swing.*;
import javax.swing.border.Border;
import java.awt.*;
import java.awt.event.AWTEventListener;
import java.awt.event.MouseEvent;
import java.awt.geom.RoundRectangle2D;

/**
 * Marks one setting row, e.g. the pin setting opened from Pinout's "Open in Tuning", so the eye
 * finds it on a long page. The mark is the Pinout selection blue [tag:console_theme]: a tint behind
 * the row and a bar in its empty left margin, where no editor covers it. It is drawn by a border that
 * keeps the row's own insets, so nothing moves. The next mouse press anywhere in the
 * console removes it, as does showing another page.
 */
final class FieldHighlight {
    private static final int ARC = 10;
    private static final int FILL_ALPHA = 48;
    /** Inside the 10 px strut that starts every setting row. */
    private static final int BAR_WIDTH = 4;

    private JComponent row;
    private Border original;
    private AWTEventListener clearOnPress;

    /** Marks {@code row} and scrolls it into view; a previous mark is removed. */
    void show(JComponent row) {
        clear();
        this.row = row;
        original = row.getBorder();
        row.setBorder(new Mark(original));
        row.repaint();
        clearOnPress = event -> {
            if (event.getID() == MouseEvent.MOUSE_PRESSED) {
                clear();
            }
        };
        Toolkit.getDefaultToolkit().addAWTEventListener(clearOnPress, AWTEvent.MOUSE_EVENT_MASK);
        // after the page's own deferred layout pass, so the row has its final place
        SwingUtilities.invokeLater(() -> SwingUtilities.invokeLater(() -> {
            if (this.row == row) {
                row.scrollRectToVisible(new Rectangle(0, 0, row.getWidth(), row.getHeight()));
            }
        }));
    }

    void clear() {
        if (clearOnPress != null) {
            Toolkit.getDefaultToolkit().removeAWTEventListener(clearOnPress);
            clearOnPress = null;
        }
        if (row != null) {
            row.setBorder(original);
            row.repaint();
            row = null;
            original = null;
        }
    }

    JComponent marked() {
        return row;
    }

    static Color color() {
        return TuningStyle.color("Console.Pinout.selection", new Color(0x2563eb));
    }

    /** A tinted rounded box with an accent bar on the left, around the row's own border. */
    private static final class Mark implements Border {
        private final Border inner;

        Mark(Border inner) {
            this.inner = inner;
        }

        @Override
        public void paintBorder(Component c, Graphics g, int x, int y, int width, int height) {
            if (inner != null) {
                inner.paintBorder(c, g, x, y, width, height);
            }
            Graphics2D g2 = (Graphics2D) g.create();
            try {
                g2.setRenderingHint(RenderingHints.KEY_ANTIALIASING, RenderingHints.VALUE_ANTIALIAS_ON);
                Color accent = color();
                // painted before the row's children, so the fill tints behind the label and editors
                g2.setColor(new Color(accent.getRed(), accent.getGreen(), accent.getBlue(), FILL_ALPHA));
                g2.fill(new RoundRectangle2D.Double(x, y, width, height, ARC, ARC));
                g2.setColor(accent);
                g2.fill(new RoundRectangle2D.Double(x + 2, y + 2, BAR_WIDTH, height - 4, BAR_WIDTH, BAR_WIDTH));
            } finally {
                g2.dispose();
            }
        }

        @Override
        public Insets getBorderInsets(Component c) {
            return inner != null ? inner.getBorderInsets(c) : new Insets(0, 0, 0, 0);
        }

        @Override
        public boolean isBorderOpaque() {
            return false;
        }
    }
}
