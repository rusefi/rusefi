package com.rusefi.ui.widgets.tune;

import org.junit.jupiter.api.Test;

import javax.swing.*;
import java.awt.*;
import java.awt.event.MouseEvent;
import java.util.concurrent.atomic.AtomicInteger;

import static org.junit.jupiter.api.Assertions.*;

class DetachPanelLayerTest {
    @Test
    void hoverOverNestedContentRevealsSquareButtonAndLeavesEditorsClickable() throws Exception {
        SwingUtilities.invokeAndWait(() -> {
            JTextField editor = new JTextField("Editable content");
            JPanel panel = new JPanel(new BorderLayout());
            panel.add(editor);
            AtomicInteger detached = new AtomicInteger();
            DetachPanelLayer ui = new DetachPanelLayer(new JScrollPane(panel), detached::incrementAndGet);
            JLayer<JScrollPane> layer = ui.getContent();
            layer.setSize(500, 300);
            layer.doLayout();
            layer.getGlassPane().doLayout();
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
