package com.rusefi.ui.widgets.tune;

import com.formdev.flatlaf.FlatClientProperties;

import javax.swing.*;
import javax.swing.event.ChangeListener;
import javax.swing.plaf.LayerUI;
import java.awt.*;
import java.awt.event.ComponentAdapter;
import java.awt.event.ComponentEvent;
import java.awt.event.MouseEvent;
import java.awt.geom.Line2D;
import java.awt.geom.Path2D;

/** Hover affordance in the form's corner; only the button intercepts mouse input. */
final class DetachPanelLayer extends LayerUI<JScrollPane> {
    private static final int BUTTON_SIZE = 28;
    private final JButton button = new JButton(new DetachIcon());
    private final JLayer<JScrollPane> layer;
    private final JScrollPane scrollPane;
    private final JComponent panel;
    private boolean available;
    private Point mousePosition;
    private final ChangeListener viewportListener = event -> layoutButton();
    private final ComponentAdapter panelListener = new ComponentAdapter() {
        @Override
        public void componentMoved(ComponentEvent event) {
            layoutButton();
        }

        @Override
        public void componentResized(ComponentEvent event) {
            layoutButton();
        }
    };

    DetachPanelLayer(JScrollPane scrollPane, JComponent panel, Runnable detach) {
        this.scrollPane = scrollPane;
        this.panel = panel;
        button.setToolTipText("Detach this panel into a separate window");
        button.getAccessibleContext().setAccessibleName("Detach panel");
        button.setMargin(new Insets(0, 0, 0, 0));
        // A small rounded tile in the theme's button colours, without the focus ring of a form button.
        button.putClientProperty(FlatClientProperties.STYLE, "arc: 8; focusWidth: 0; innerFocusWidth: 0");
        button.setFocusable(false);
        button.setVisible(false);
        button.addActionListener(event -> detach.run());
        JPanel glass = new JPanel(null) {
            @Override
            public boolean contains(int x, int y) {
                return button.isVisible() && button.getBounds().contains(x, y);
            }

            @Override
            public void doLayout() {
                layoutButton();
            }
        };
        glass.setOpaque(false);
        glass.add(button);
        layer = new JLayer<>(scrollPane, this);
        layer.setGlassPane(glass);
        glass.setVisible(true);
        scrollPane.getViewport().addChangeListener(viewportListener);
        panel.addComponentListener(panelListener);
    }

    JLayer<JScrollPane> getContent() {
        return layer;
    }

    void setAvailable(boolean available) {
        this.available = available;
        layoutButton();
    }

    @Override
    public void installUI(JComponent component) {
        super.installUI(component);
        ((JLayer<?>) component).setLayerEventMask(AWTEvent.MOUSE_EVENT_MASK | AWTEvent.MOUSE_MOTION_EVENT_MASK);
    }

    @Override
    public void uninstallUI(JComponent component) {
        ((JLayer<?>) component).setLayerEventMask(0);
        scrollPane.getViewport().removeChangeListener(viewportListener);
        panel.removeComponentListener(panelListener);
        super.uninstallUI(component);
    }

    @Override
    protected void processMouseEvent(MouseEvent event, JLayer<? extends JScrollPane> layer) {
        updateHover(event, layer);
    }

    @Override
    protected void processMouseMotionEvent(MouseEvent event, JLayer<? extends JScrollPane> layer) {
        updateHover(event, layer);
    }

    private void updateHover(MouseEvent event, JLayer<? extends JScrollPane> layer) {
        mousePosition = SwingUtilities.convertPoint(event.getComponent(), event.getPoint(), layer);
        layoutButton();
    }

    private void layoutButton() {
        Rectangle bounds = SwingUtilities.convertRectangle(panel, new Rectangle(panel.getSize()), layer);
        Rectangle viewport = SwingUtilities.convertRectangle(scrollPane.getViewport(),
            new Rectangle(scrollPane.getViewport().getSize()), layer);
        button.setBounds(bounds.x + bounds.width - BUTTON_SIZE - 6, bounds.y + 6, BUTTON_SIZE, BUTTON_SIZE);
        button.setVisible(available && mousePosition != null && bounds.contains(mousePosition)
            && viewport.contains(mousePosition) && viewport.contains(button.getBounds()));
    }

    /** "Open in new window": a rounded window open at its top-right corner, an arrow leaving through it. */
    private static final class DetachIcon implements Icon {
        @Override
        public int getIconWidth() {
            return 16;
        }

        @Override
        public int getIconHeight() {
            return 16;
        }

        @Override
        public void paintIcon(Component component, Graphics graphics, int x, int y) {
            Graphics2D g2 = (Graphics2D) graphics.create();
            try {
                g2.setRenderingHint(RenderingHints.KEY_ANTIALIASING, RenderingHints.VALUE_ANTIALIAS_ON);
                g2.setRenderingHint(RenderingHints.KEY_STROKE_CONTROL, RenderingHints.VALUE_STROKE_PURE);
                g2.setColor(component.getForeground());
                g2.setStroke(new BasicStroke(1.5f, BasicStroke.CAP_ROUND, BasicStroke.JOIN_ROUND));
                g2.translate(x, y);
                // window: left and bottom sides plus the start of the top and right ones, rounded corners
                Path2D window = new Path2D.Float();
                window.moveTo(7, 2.75);
                window.lineTo(4.5, 2.75);
                window.quadTo(2.75, 2.75, 2.75, 4.5);
                window.lineTo(2.75, 11.5);
                window.quadTo(2.75, 13.25, 4.5, 13.25);
                window.lineTo(11.5, 13.25);
                window.quadTo(13.25, 13.25, 13.25, 11.5);
                window.lineTo(13.25, 9);
                g2.draw(window);
                // arrow out of the open corner
                g2.draw(new Line2D.Float(7.5f, 8.5f, 13.25f, 2.75f));
                Path2D head = new Path2D.Float();
                head.moveTo(9.5, 2.75);
                head.lineTo(13.25, 2.75);
                head.lineTo(13.25, 6.5);
                g2.draw(head);
            } finally {
                g2.dispose();
            }
        }
    }
}
