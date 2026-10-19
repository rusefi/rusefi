package com.rusefi.ui.widgets;

import javax.swing.*;
import java.awt.*;
import java.awt.event.ContainerAdapter;
import java.awt.event.ContainerEvent;
import java.awt.event.MouseAdapter;
import java.awt.event.MouseEvent;
import java.awt.geom.Area;
import java.awt.geom.Ellipse2D;
import java.util.function.Consumer;

/** Transparent detached-window content, with the close button outside the dial. */
final class DetachedGaugePanel extends JPanel {
    private static final int CLOSE_SIZE = 28;
    private static final int CLOSE_MARGIN = 4;
    private static final int ANTIALIAS_MARGIN = 2;
    private final JPanelWithListener gaugeWrapper;
    private final JButton closeButton = new JButton(new CloseIcon());
    private final DetachedGaugeHover hover;
    private final Consumer<Boolean> hoverTarget = this::setCloseVisible;
    private final Consumer<Shape> shapeChanged;
    private boolean disposed;
    private Point pressLocation;
    private Point windowLocation;
    private Window dragWindow;

    private final MouseAdapter mouse = new MouseAdapter() {
        @Override
        public void mouseEntered(MouseEvent event) {
            hovered();
        }

        @Override
        public void mouseMoved(MouseEvent event) {
            hovered();
        }

        @Override
        public void mousePressed(MouseEvent event) {
            if (SwingUtilities.isLeftMouseButton(event) && event.getComponent() != closeButton) {
                dragWindow = SwingUtilities.getWindowAncestor(DetachedGaugePanel.this);
                if (dragWindow != null) {
                    pressLocation = event.getLocationOnScreen();
                    windowLocation = dragWindow.getLocation();
                }
            }
        }

        @Override
        public void mouseDragged(MouseEvent event) {
            hovered();
            if (dragWindow != null) {
                Point current = event.getLocationOnScreen();
                dragWindow.setLocation(windowLocation.x + current.x - pressLocation.x,
                    windowLocation.y + current.y - pressLocation.y);
            }
        }

        @Override
        public void mouseReleased(MouseEvent event) {
            dragWindow = null;
        }
    };

    private final ContainerAdapter gaugeChanges = new ContainerAdapter() {
        @Override
        public void componentAdded(ContainerEvent event) {
            track(event.getChild());
        }

        @Override
        public void componentRemoved(ContainerEvent event) {
            untrack(event.getChild());
        }
    };

    DetachedGaugePanel(JPanelWithListener gaugeWrapper, Runnable closeAction, Consumer<Shape> shapeChanged) {
        this(gaugeWrapper, closeAction, shapeChanged, DetachedGaugeHover.SHARED);
    }

    DetachedGaugePanel(JPanelWithListener gaugeWrapper, Runnable closeAction, Consumer<Shape> shapeChanged,
                       DetachedGaugeHover hover) {
        super(null);
        this.gaugeWrapper = gaugeWrapper;
        this.shapeChanged = shapeChanged;
        this.hover = hover;
        setOpaque(false);
        gaugeWrapper.setOpaque(false);
        closeButton.setBorder(BorderFactory.createEmptyBorder());
        closeButton.setContentAreaFilled(false);
        closeButton.setFocusPainted(false);
        closeButton.setToolTipText("Close gauge");
        closeButton.getAccessibleContext().setAccessibleName("Close gauge");
        closeButton.addActionListener(event -> closeAction.run());
        closeButton.setVisible(false);
        add(closeButton);
        add(gaugeWrapper);
        track(this);
        track(closeButton);
        // Gauge replacement removes the wrapper's own mouse listeners. Follow
        // its children instead, so hover/drag survives a right-click selection.
        gaugeWrapper.addContainerListener(gaugeChanges);
        for (Component child : gaugeWrapper.getComponents()) {
            track(child);
        }
    }

    private void track(Component component) {
        component.addMouseListener(mouse);
        component.addMouseMotionListener(mouse);
    }

    private void untrack(Component component) {
        component.removeMouseListener(mouse);
        component.removeMouseMotionListener(mouse);
    }

    private void hovered() {
        if (!disposed) {
            hover.hover(hoverTarget);
        }
    }

    private void setCloseVisible(boolean visible) {
        if (closeButton.isVisible() != visible) {
            closeButton.setVisible(visible);
            shapeChanged.accept(windowShape());
            repaint();
        }
    }

    @Override
    protected void paintComponent(Graphics graphics) {
        // Swing may reuse a buffer that still contains an opaque background or
        // the previous antialiased edge. Replace those pixels before painting.
        Window window = SwingUtilities.getWindowAncestor(this);
        Graphics2D g = (Graphics2D) graphics.create();
        try {
            g.setComposite(AlphaComposite.Src);
            g.setColor(window == null ? new Color(0, true) : window.getBackground());
            g.fillRect(0, 0, getWidth(), getHeight());
        } finally {
            g.dispose();
        }
        super.paintComponent(graphics);
    }

    @Override
    public void doLayout() {
        int size = Math.min(getWidth(), getHeight());
        int x = (getWidth() - size) / 2;
        int y = (getHeight() - size) / 2;
        gaugeWrapper.setBounds(x, y, size, size);
        closeButton.setBounds(x + size - CLOSE_SIZE - CLOSE_MARGIN, y + CLOSE_MARGIN, CLOSE_SIZE, CLOSE_SIZE);
        shapeChanged.accept(windowShape());
    }

    Shape windowShape() {
        Rectangle dial = gaugeWrapper.getBounds();
        // Native shaping is a binary clip. Keep it outside the painted fringe
        // so the gauge's per-pixel alpha controls the smooth visible edge.
        Area shape = new Area(new Ellipse2D.Double(dial.x - ANTIALIAS_MARGIN, dial.y - ANTIALIAS_MARGIN,
            dial.width + 2 * ANTIALIAS_MARGIN, dial.height + 2 * ANTIALIAS_MARGIN));
        if (closeButton.isVisible()) {
            shape.add(new Area(closeButton.getBounds()));
        }
        return shape;
    }

    void dispose() {
        disposed = true;
        dragWindow = null;
        hover.remove(hoverTarget);
        gaugeWrapper.removeContainerListener(gaugeChanges);
        for (Component child : gaugeWrapper.getComponents()) {
            untrack(child);
        }
        untrack(this);
        untrack(closeButton);
    }

    private static final class CloseIcon implements Icon {
        @Override
        public int getIconWidth() {
            return CLOSE_SIZE;
        }

        @Override
        public int getIconHeight() {
            return CLOSE_SIZE;
        }

        @Override
        public void paintIcon(Component component, Graphics graphics, int x, int y) {
            Graphics2D g = (Graphics2D) graphics.create();
            try {
                g.translate(x, y);
                g.setColor(new Color(0xF3F4F6));
                g.fillRect(0, 0, CLOSE_SIZE, CLOSE_SIZE);
                g.setRenderingHint(RenderingHints.KEY_ANTIALIASING, RenderingHints.VALUE_ANTIALIAS_ON);
                g.setStroke(new BasicStroke(2));
                g.setColor(Color.RED);
                g.drawRect(1, 1, CLOSE_SIZE - 3, CLOSE_SIZE - 3);
                g.drawLine(8, 8, 20, 20);
                g.drawLine(20, 8, 8, 20);
            } finally {
                g.dispose();
            }
        }
    }
}
