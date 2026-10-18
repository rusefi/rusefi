package com.rusefi.ui.widgets.tune;

import javax.swing.*;
import javax.swing.plaf.LayerUI;
import java.awt.*;
import java.awt.event.MouseEvent;

/** Hover affordance above the scrolling content; only the button intercepts mouse input. */
final class DetachPanelLayer extends LayerUI<JScrollPane> {
    private static final int BUTTON_SIZE = 28;
    private final JButton button = new JButton(new DetachIcon());
    private final JLayer<JScrollPane> layer;
    private boolean available;
    private boolean hovered;

    DetachPanelLayer(JScrollPane scrollPane, Runnable detach) {
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
                JScrollBar scrollbar = scrollPane.getVerticalScrollBar();
                int scrollbarWidth = scrollbar.isVisible() ? scrollbar.getWidth() : 0;
                button.setBounds(Math.max(0, getWidth() - scrollbarWidth - BUTTON_SIZE - 6),
                    6, BUTTON_SIZE, BUTTON_SIZE);
            }
        };
        glass.setOpaque(false);
        glass.add(button);
        layer = new JLayer<>(scrollPane, this);
        layer.setGlassPane(glass);
        glass.setVisible(true);
    }

    JLayer<JScrollPane> getContent() {
        return layer;
    }

    void setAvailable(boolean available) {
        this.available = available;
        button.setVisible(available && hovered);
    }

    @Override
    public void installUI(JComponent component) {
        super.installUI(component);
        ((JLayer<?>) component).setLayerEventMask(AWTEvent.MOUSE_EVENT_MASK | AWTEvent.MOUSE_MOTION_EVENT_MASK);
    }

    @Override
    public void uninstallUI(JComponent component) {
        ((JLayer<?>) component).setLayerEventMask(0);
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
        Point point = SwingUtilities.convertPoint(event.getComponent(), event.getPoint(), layer);
        hovered = layer.contains(point);
        button.setVisible(available && hovered);
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
