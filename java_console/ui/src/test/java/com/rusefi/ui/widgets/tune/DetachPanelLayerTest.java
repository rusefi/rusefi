package com.rusefi.ui.widgets.tune;

import org.junit.jupiter.api.Test;

import javax.swing.*;
import java.awt.*;
import java.awt.event.MouseEvent;
import java.util.concurrent.atomic.AtomicInteger;

import static org.junit.jupiter.api.Assertions.*;

class DetachPanelLayerTest {
    @Test
    void compactPanelHoverUsesItsCorner() throws Exception {
        SwingUtilities.invokeAndWait(() -> {
            JPanel compact = new JPanel();
            compact.setBounds(200, 20, 300, 140);
            JPanel tile = new JPanel(null);
            tile.setPreferredSize(new Dimension(900, 600));
            tile.add(compact);
            JScrollPane scroll = new JScrollPane(tile);
            DetachPanelLayer ui = new DetachPanelLayer(scroll, compact, () -> { });
            JLayer<JScrollPane> layer = ui.getContent();
            layer.setSize(1000, 700);
            layout(layer);
            JButton button = CalibrationPanelWorkspaceTest.find(layer.getGlassPane(), JButton.class);
            ui.setAvailable(true);
            ui.eventDispatched(new MouseEvent(compact, MouseEvent.MOUSE_ENTERED, 0, 0, 20, 20, 0, false), layer);
            assertTrue(button.isVisible());
            Rectangle bounds = SwingUtilities.convertRectangle(compact, new Rectangle(compact.getSize()), layer);
            assertEquals(bounds.x + bounds.width - 34, button.getX());
            assertEquals(bounds.y + 6, button.getY());
            ui.eventDispatched(new MouseEvent(tile, MouseEvent.MOUSE_MOVED, 0, 0, 10, 200, 0, false), layer);
            assertFalse(button.isVisible(), "Empty tile background must not reveal the icon");
            compact.setBounds(240, 40, 350, 180);
            layer.getGlassPane().doLayout();
            ui.eventDispatched(new MouseEvent(compact, MouseEvent.MOUSE_ENTERED, 0, 0, 20, 20, 0, false), layer);
            bounds = SwingUtilities.convertRectangle(compact, new Rectangle(compact.getSize()), layer);
            assertEquals(bounds.x + bounds.width - 34, button.getX(), "Follow resized and recentered forms");
            assertEquals(bounds.y + 6, button.getY());
            scroll.getViewport().setViewPosition(new Point(0, 100));
            assertFalse(button.isVisible(), "Do not paint a scrolled-out corner over the surrounding UI");
            layer.setUI(null);
        });
    }

    private static void layout(Container container) {
        container.doLayout();
        for (Component child : container.getComponents()) {
            if (child instanceof Container) {
                layout((Container) child);
            }
        }
    }

    @Test
    void hoverOverNestedContentRevealsSquareButtonAndLeavesEditorsClickable() throws Exception {
        SwingUtilities.invokeAndWait(() -> {
            JTextField editor = new JTextField("Editable content");
            JPanel panel = new JPanel(new BorderLayout());
            panel.add(editor);
            AtomicInteger detached = new AtomicInteger();
            DetachPanelLayer ui = new DetachPanelLayer(new JScrollPane(panel), panel, detached::incrementAndGet);
            JLayer<JScrollPane> layer = ui.getContent();
            layer.setSize(500, 300);
            layout(layer);
            JButton button = CalibrationPanelWorkspaceTest.find(layer.getGlassPane(), JButton.class);
            assertFalse(button.isVisible());
            ui.setAvailable(true);
            ui.eventDispatched(new MouseEvent(editor, MouseEvent.MOUSE_ENTERED, 0, 0, 10, 10, 0, false), layer);
            assertTrue(button.isVisible());
            assertEquals(button.getWidth(), button.getHeight());
            assertTrue(button.getX() > 450);
            assertFalse(layer.getGlassPane().contains(20, 20), "The overlay must not block editors");
            assertTrue(layer.getGlassPane().contains(button.getX() + 2, button.getY() + 2));
            button.doClick();
            assertEquals(1, detached.get());
            ui.eventDispatched(new MouseEvent(layer, MouseEvent.MOUSE_EXITED, 0, 0, 501, 50, 0, false), layer);
            assertFalse(button.isVisible());
            ui.setAvailable(false);
            ui.eventDispatched(new MouseEvent(editor, MouseEvent.MOUSE_ENTERED, 0, 0, 10, 10, 0, false), layer);
            assertFalse(button.isVisible(), "No detach affordance without a loaded page");
            layer.setUI(null);
        });
    }
}
