package com.rusefi.ui.laf;

import javax.swing.*;
import javax.swing.border.AbstractBorder;
import java.awt.*;
import java.awt.event.AWTEventListener;
import java.awt.event.HierarchyEvent;
import java.awt.event.MouseAdapter;
import java.awt.event.MouseEvent;

/**
 * A titled group inside a Tuning dialog: a rounded outline with the bold title inside its top
 * edge. The outline turns accent-tinted under the mouse to show which group a field belongs to.
 * Colours: Console.Tuning.section.* theme keys [tag:console_theme].
 */
public class GradientTitleBorder extends AbstractBorder {
    private static final int ARC = 10;
    private static final int TITLE_HEIGHT = 24;
    private static final int PADDING = 8;
    private static final int VERTICAL_MARGIN = 3;
    private final String title;
    private boolean mouseOver = false;

    public GradientTitleBorder(String title) {
        this.title = title;
    }

    public static void installBorder(String uiName, JPanel panelWidget) {
        GradientTitleBorder border = new GradientTitleBorder(uiName);
        panelWidget.setBorder(border);
        border.installMouseListener(panelWidget);
    }

    /**
     * Keeps the outline highlighted while the pointer is anywhere inside the group, including over
     * its fields. Moving onto a child sends the group a mouseExited, so the group's own events alone
     * would make the outline flicker; while highlighted, a toolkit-wide listener watches every mouse
     * event and turns the highlight off once the pointer is outside the group's bounds.
     */
    public void installMouseListener(Component c) {
        AWTEventListener watcher = new AWTEventListener() {
            @Override
            public void eventDispatched(AWTEvent event) {
                if (!(event instanceof MouseEvent)) {
                    return;
                }
                MouseEvent e = (MouseEvent) event;
                Component source = e.getComponent();
                boolean inside = source != null && c.isShowing() && source.isShowing()
                    && c.contains(SwingUtilities.convertPoint(source, e.getPoint(), c));
                if (!inside) {
                    setMouseOver(c, false, this);
                }
            }
        };
        c.addMouseListener(new MouseAdapter() {
            @Override
            public void mouseEntered(MouseEvent e) {
                setMouseOver(c, true, watcher);
            }

            @Override
            public void mouseExited(MouseEvent e) {
                if (!c.contains(e.getPoint())) {
                    setMouseOver(c, false, watcher);
                }
            }
        });
        c.addHierarchyListener(e -> {
            if ((e.getChangeFlags() & HierarchyEvent.SHOWING_CHANGED) != 0 && !c.isShowing()) {
                setMouseOver(c, false, watcher);
            }
        });
    }

    private void setMouseOver(Component c, boolean over, AWTEventListener watcher) {
        if (mouseOver == over) {
            return;
        }
        mouseOver = over;
        Toolkit toolkit = Toolkit.getDefaultToolkit();
        if (over) {
            toolkit.addAWTEventListener(watcher, AWTEvent.MOUSE_EVENT_MASK | AWTEvent.MOUSE_MOTION_EVENT_MASK);
        } else {
            toolkit.removeAWTEventListener(watcher);
        }
        c.repaint();
    }

    public String getTitle() {
        return title;
    }

    private static Color color(String key, Color fallback) {
        Color color = UIManager.getColor(key);
        return color != null ? color : fallback;
    }

    @Override
    public void paintBorder(Component c, Graphics g, int x, int y, int width, int height) {
        Graphics2D g2 = (Graphics2D) g.create();
        try {
            g2.setRenderingHint(RenderingHints.KEY_ANTIALIASING, RenderingHints.VALUE_ANTIALIAS_ON);
            g2.setRenderingHint(RenderingHints.KEY_TEXT_ANTIALIASING, RenderingHints.VALUE_TEXT_ANTIALIAS_ON);
            g2.setColor(mouseOver
                ? color("Console.Tuning.section.hoverBorderColor", new Color(0x92b1f5))
                : color("Console.Tuning.section.borderColor", new Color(0xdfe3e9)));
            g2.drawRoundRect(x, y + VERTICAL_MARGIN, width - 1, height - 2 * VERTICAL_MARGIN - 1, ARC, ARC);

            Font base = c.getFont() != null ? c.getFont() : g2.getFont();
            g2.setFont(base.deriveFont(Font.BOLD));
            FontMetrics fm = g2.getFontMetrics();
            g2.setColor(color("Console.Tuning.section.title", color("Label.foreground", Color.DARK_GRAY)));
            g2.drawString(title, x + PADDING + 2, y + VERTICAL_MARGIN + (TITLE_HEIGHT - fm.getHeight()) / 2 + fm.getAscent() + 1);
        } finally {
            g2.dispose();
        }
    }

    @Override
    public Insets getBorderInsets(Component c) {
        return getBorderInsets(c, new Insets(0, 0, 0, 0));
    }

    @Override
    public Insets getBorderInsets(Component c, Insets insets) {
        insets.set(TITLE_HEIGHT + VERTICAL_MARGIN, PADDING, PADDING - 2 + VERTICAL_MARGIN, PADDING);
        return insets;
    }
}
