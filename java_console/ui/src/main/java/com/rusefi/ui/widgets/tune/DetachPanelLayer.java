package com.rusefi.ui.widgets.tune;

import javax.swing.*;
import javax.swing.event.ChangeListener;
import javax.swing.plaf.LayerUI;
import java.awt.*;
import java.awt.event.ComponentAdapter;
import java.awt.event.ComponentEvent;
import java.awt.event.MouseEvent;

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
            graphics.setColor(component.getForeground());
            graphics.drawRect(x + 1, y + 5, 9, 9);
            graphics.drawLine(x + 6, y + 9, x + 14, y + 1);
            graphics.drawLine(x + 9, y + 1, x + 14, y + 1);
            graphics.drawLine(x + 14, y + 1, x + 14, y + 6);
        }
    }
}
