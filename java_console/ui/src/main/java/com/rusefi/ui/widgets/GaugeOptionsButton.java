package com.rusefi.ui.widgets;

import javax.swing.*;
import java.awt.*;
import java.awt.event.MouseAdapter;
import java.awt.event.MouseEvent;
import java.awt.geom.AffineTransform;
import java.awt.geom.Area;
import java.awt.geom.Ellipse2D;
import java.awt.geom.RoundRectangle2D;
import java.util.function.Supplier;

/**
 * Gear in a gauge's top-right corner, shown while the mouse is over the gauge. It opens the
 * gauge's display settings, so the right-click menu can offer only the gauge catalog.
 */
public final class GaugeOptionsButton extends JButton {
    public static final String TOOLTIP = "Gauge options";
    static final int SIZE = 22;
    private static final int MARGIN = 6;
    private static final Color HOVER = new Color(0, 0, 0, 24);

    private GaugeOptionsButton() {
        super(new GearIcon());
        setToolTipText(TOOLTIP);
        getAccessibleContext().setAccessibleName(TOOLTIP);
        setContentAreaFilled(false);
        setBorderPainted(false);
        setFocusPainted(false);
        setFocusable(false);
        setOpaque(false);
        setMargin(new Insets(0, 0, 0, 0));
        setCursor(Cursor.getPredefinedCursor(Cursor.HAND_CURSOR));
        setVisible(false);
    }

    /**
     * Adds the gear to {@code host}, which must not lay out other children.
     *
     * @param menu builds the options menu each time, so it shows the current state
     */
    public static GaugeOptionsButton install(JComponent host, Supplier<JPopupMenu> menu) {
        GaugeOptionsButton gear = new GaugeOptionsButton();
        gear.addActionListener(e -> GaugePopups.show(menu.get(), gear, 0, gear.getHeight()));
        host.setLayout(new CornerLayout(gear));
        host.add(gear);

        MouseAdapter hover = new MouseAdapter() {
            @Override
            public void mouseEntered(MouseEvent e) {
                gear.setVisible(true);
            }

            @Override
            public void mouseExited(MouseEvent e) {
                // Moving onto the gear exits the host; the pointer is then still inside it.
                if (host.getMousePosition(true) == null) {
                    gear.setVisible(false);
                }
            }
        };
        host.addMouseListener(hover);
        gear.addMouseListener(hover);
        return gear;
    }

    @Override
    protected void paintComponent(Graphics g) {
        if (getModel().isRollover() || getModel().isPressed()) {
            Graphics2D g2 = (Graphics2D) g.create();
            try {
                g2.setRenderingHint(RenderingHints.KEY_ANTIALIASING, RenderingHints.VALUE_ANTIALIAS_ON);
                g2.setColor(HOVER);
                g2.fill(new RoundRectangle2D.Double(0, 0, getWidth(), getHeight(), 8, 8));
            } finally {
                g2.dispose();
            }
        }
        super.paintComponent(g);
    }

    /** Keeps the gear in the top-right corner; the host paints everything else itself. */
    private static final class CornerLayout implements LayoutManager {
        private final Component gear;

        CornerLayout(Component gear) {
            this.gear = gear;
        }

        @Override
        public void layoutContainer(Container parent) {
            gear.setBounds(Math.max(0, parent.getWidth() - SIZE - MARGIN), MARGIN, SIZE, SIZE);
        }

        @Override
        public void addLayoutComponent(String name, Component comp) {
        }

        @Override
        public void removeLayoutComponent(Component comp) {
        }

        /** The current size, as a container without a layout manager reports. */
        @Override
        public Dimension preferredLayoutSize(Container parent) {
            return parent.getSize();
        }

        @Override
        public Dimension minimumLayoutSize(Container parent) {
            return parent.getSize();
        }
    }

    /** Eight-tooth gear in the gauges' scale colour. */
    static final class GearIcon implements Icon {
        private static final int ICON_SIZE = 16;
        private static final int TEETH = 8;

        @Override
        public void paintIcon(Component c, Graphics g, int x, int y) {
            Graphics2D g2 = (Graphics2D) g.create();
            try {
                g2.setRenderingHint(RenderingHints.KEY_ANTIALIASING, RenderingHints.VALUE_ANTIALIAS_ON);
                g2.translate(x + ICON_SIZE / 2.0, y + ICON_SIZE / 2.0);
                Area gear = new Area(new Ellipse2D.Double(-5.2, -5.2, 10.4, 10.4));
                for (int i = 0; i < TEETH; i++) {
                    Area tooth = new Area(new RoundRectangle2D.Double(-1.5, -7.5, 3, 4, 1, 1));
                    tooth.transform(AffineTransform.getRotateInstance(2 * Math.PI * i / TEETH));
                    gear.add(tooth);
                }
                gear.subtract(new Area(new Ellipse2D.Double(-2.2, -2.2, 4.4, 4.4)));
                g2.setColor(c != null && !c.isEnabled() ? GaugeStyle.TICK : GaugeStyle.SCALE);
                g2.fill(gear);
            } finally {
                g2.dispose();
            }
        }

        @Override
        public int getIconWidth() {
            return ICON_SIZE;
        }

        @Override
        public int getIconHeight() {
            return ICON_SIZE;
        }
    }
}
