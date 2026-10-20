package com.rusefi.ui.widgets;

import com.formdev.flatlaf.FlatClientProperties;

import javax.swing.*;
import java.awt.*;
import java.awt.event.ComponentAdapter;
import java.awt.event.ComponentEvent;

/**
 * Buttons drawn as one outlined control with dividers, e.g. Save / Load / Reset for gauge
 * layouts; a single button gets the same outline so every control in a toolbar row matches.
 * All controls are {@link #HEIGHT} tall, like {@link SegmentedControl}. The buttons keep their
 * own listeners, tooltips and text; the group hides itself while all of its buttons are hidden.
 * Colours follow the theme's component border [tag:console_theme].
 */
public class JoinedButtons extends JPanel {
    /** Shared height of the toolbar controls, so a row of them lines up. */
    public static final int HEIGHT = 36;
    /** Gap between controls in a toolbar row. */
    public static final int ROW_GAP = 8;
    /** Distance from a toolbar row's last control to the window edge. */
    public static final int ROW_END_INSET = 12;
    private static final int ARC = 10;
    private static final int BORDER = 1;

    public JoinedButtons(AbstractButton... buttons) {
        setLayout(new RowLayout());
        setOpaque(false);
        ComponentAdapter visibilityTracker = new ComponentAdapter() {
            @Override
            public void componentShown(ComponentEvent e) {
                updateVisibility();
            }

            @Override
            public void componentHidden(ComponentEvent e) {
                updateVisibility();
            }
        };
        for (AbstractButton button : buttons) {
            // flat button: hover/pressed/selected backgrounds only, the outline is drawn by this panel
            button.putClientProperty(FlatClientProperties.BUTTON_TYPE, FlatClientProperties.BUTTON_TYPE_TOOLBAR_BUTTON);
            button.setFocusPainted(false);
            button.setMargin(new Insets(0, 14, 0, 14));
            button.addComponentListener(visibilityTracker);
            add(button);
        }
        updateVisibility();
    }

    private void updateVisibility() {
        boolean any = false;
        for (Component child : getComponents()) {
            any |= child.isVisible();
        }
        setVisible(any);
        revalidate();
        repaint();
    }

    @Override
    protected void paintComponent(Graphics g) {
        Graphics2D g2 = (Graphics2D) g.create();
        try {
            g2.setRenderingHint(RenderingHints.KEY_ANTIALIASING, RenderingHints.VALUE_ANTIALIAS_ON);
            Color background = UIManager.getColor("Button.background");
            if (background != null) {
                g2.setColor(background);
                g2.fillRoundRect(0, 0, getWidth() - 1, getHeight() - 1, ARC, ARC);
            }
        } finally {
            g2.dispose();
        }
    }

    @Override
    protected void paintChildren(Graphics g) {
        Graphics2D clipped = (Graphics2D) g.create();
        try {
            // keep a hovered button's square background inside the rounded outline
            clipped.clip(new java.awt.geom.RoundRectangle2D.Float(0, 0, getWidth(), getHeight(), ARC, ARC));
            super.paintChildren(clipped);
        } finally {
            clipped.dispose();
        }
        Graphics2D g2 = (Graphics2D) g.create();
        try {
            g2.setRenderingHint(RenderingHints.KEY_ANTIALIASING, RenderingHints.VALUE_ANTIALIAS_ON);
            g2.setColor(SegmentedControl.themed("Component.borderColor", Color.LIGHT_GRAY));
            g2.drawRoundRect(0, 0, getWidth() - 1, getHeight() - 1, ARC, ARC);
            boolean first = true;
            for (Component child : getComponents()) {
                if (!child.isVisible()) {
                    continue;
                }
                if (!first) {
                    g2.drawLine(child.getX() - 1, BORDER, child.getX() - 1, getHeight() - 1 - BORDER);
                }
                first = false;
            }
        } finally {
            g2.dispose();
        }
    }

    /** Visible buttons side by side at their preferred widths, 1 px dividers between them. */
    private static class RowLayout implements LayoutManager {
        @Override
        public void addLayoutComponent(String name, Component comp) {
        }

        @Override
        public void removeLayoutComponent(Component comp) {
        }

        @Override
        public Dimension preferredLayoutSize(Container parent) {
            int width = BORDER;
            int visible = 0;
            for (Component child : parent.getComponents()) {
                if (child.isVisible()) {
                    width += child.getPreferredSize().width;
                    visible++;
                }
            }
            width += Math.max(visible - 1, 0) + BORDER;
            return new Dimension(width, HEIGHT);
        }

        @Override
        public Dimension minimumLayoutSize(Container parent) {
            return preferredLayoutSize(parent);
        }

        @Override
        public void layoutContainer(Container parent) {
            int x = BORDER;
            for (Component child : parent.getComponents()) {
                if (!child.isVisible()) {
                    continue;
                }
                int w = child.getPreferredSize().width;
                child.setBounds(x, BORDER, w, HEIGHT - 2 * BORDER);
                x += w + 1;
            }
        }
    }
}
