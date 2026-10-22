package com.rusefi.ui.logview;

import javax.swing.BorderFactory;
import javax.swing.JComponent;
import javax.swing.JPanel;
import javax.swing.JPopupMenu;
import javax.swing.MenuSelectionManager;
import javax.swing.SwingUtilities;
import java.awt.BasicStroke;
import java.awt.Color;
import java.awt.Cursor;
import java.awt.Dimension;
import java.awt.Graphics;
import java.awt.Graphics2D;
import java.awt.GridLayout;
import java.awt.RenderingHints;
import java.awt.event.MouseAdapter;
import java.awt.event.MouseEvent;
import java.util.function.IntConsumer;

/**
 * The colour picker's swatches: the plot palette's {@link PlotPalette#SWATCH_COUNT} colours in two rows, the
 * current one ringed with a check, the one under the pointer ringed lightly. A click picks a swatch and closes
 * the menu holding the grid (a popup or a submenu). [tag:console_theme]
 */
final class SwatchGrid extends JPanel {
    private static final int COLUMNS = 6;
    private static final int SIZE = 28;

    private final PlotPalette palette;
    private final int current;
    private final IntConsumer onPick;
    private int hovered = -1;

    /** @param current the swatch showing the signal's colour now, or -1 */
    SwatchGrid(PlotPalette palette, int current, IntConsumer onPick) {
        super(new GridLayout(0, COLUMNS, 6, 6));
        this.palette = palette;
        this.current = current;
        this.onPick = onPick;
        setOpaque(false);
        setBorder(BorderFactory.createEmptyBorder(8, 10, 6, 10));
        for (int swatch = 0; swatch < PlotPalette.SWATCH_COUNT; swatch++) {
            add(new Swatch(swatch));
        }
    }

    /** Picks a swatch as a click on it would. */
    void pick(int swatch) {
        onPick.accept(swatch);
        // a submenu closes through the menu selection, a popup shown on its own by hiding it
        MenuSelectionManager.defaultManager().clearSelectedPath();
        JPopupMenu popup = (JPopupMenu) SwingUtilities.getAncestorOfClass(JPopupMenu.class, this);
        if (popup != null) {
            popup.setVisible(false);
        }
    }

    private final class Swatch extends JComponent {
        private final int swatch;

        Swatch(int swatch) {
            this.swatch = swatch;
            setPreferredSize(new Dimension(SIZE, SIZE));
            setCursor(Cursor.getPredefinedCursor(Cursor.HAND_CURSOR));
            MouseAdapter mouse = new MouseAdapter() {
                @Override
                public void mouseEntered(MouseEvent event) {
                    hovered = swatch;
                    SwatchGrid.this.repaint();
                }

                @Override
                public void mouseExited(MouseEvent event) {
                    hovered = -1;
                    SwatchGrid.this.repaint();
                }

                @Override
                public void mouseReleased(MouseEvent event) {
                    if (contains(event.getPoint())) {
                        pick(swatch);
                    }
                }
            };
            addMouseListener(mouse);
        }

        @Override
        protected void paintComponent(Graphics g) {
            Graphics2D g2 = (Graphics2D) g.create();
            try {
                g2.setRenderingHint(RenderingHints.KEY_ANTIALIASING, RenderingHints.VALUE_ANTIALIAS_ON);
                int w = getWidth();
                int h = getHeight();
                if (swatch == current || swatch == hovered) {
                    Color label = ThemedStrip.themed("Label.foreground", Color.DARK_GRAY);
                    g2.setColor(swatch == current ? label : ThemedStrip.themed("Console.LogViewer.mutedForeground", Color.GRAY));
                    g2.setStroke(new BasicStroke(2f));
                    g2.drawRoundRect(1, 1, w - 3, h - 3, 8, 8);
                }
                Color color = palette.trace(swatch);
                g2.setColor(color);
                g2.fillRoundRect(4, 4, w - 8, h - 8, 6, 6);
                if (swatch == current) {
                    // a check that stays visible on light (dark-plot) and dark swatches alike
                    boolean light = (color.getRed() * 299 + color.getGreen() * 587 + color.getBlue() * 114) / 1000 > 150;
                    g2.setColor(light ? Color.BLACK : Color.WHITE);
                    g2.setStroke(new BasicStroke(2f, BasicStroke.CAP_ROUND, BasicStroke.JOIN_ROUND));
                    g2.drawPolyline(new int[]{w / 2 - 5, w / 2 - 1, w / 2 + 5}, new int[]{h / 2, h / 2 + 4, h / 2 - 4}, 3);
                }
            } finally {
                g2.dispose();
            }
        }
    }
}
