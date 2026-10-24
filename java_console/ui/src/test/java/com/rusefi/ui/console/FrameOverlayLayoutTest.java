package com.rusefi.ui.console;

import org.junit.jupiter.api.Test;

import javax.swing.JTextArea;
import javax.swing.SwingUtilities;
import javax.swing.text.BadLocationException;
import java.awt.Color;
import java.awt.Component;
import java.awt.Container;
import java.awt.event.KeyEvent;
import java.awt.geom.Rectangle2D;

import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertTrue;

class FrameOverlayLayoutTest {
    @Test
    void duplicateInjectorPinErrorIsClippedInNarrowWindow() throws Exception {
        SwingUtilities.invokeAndWait(() -> {
            // Issue #10490: duplicate injector outputs produce this critical-error format. Capture
            // the current bad layout first; the fix can then invert these visibility assertions.
            String error = "Config Error\n\nPin \"PA1\" (Injector 1) required by \"Injector 2\" but is used by \"Injector 1\"";
            MainFrame.FrameOverlay overlay = new MainFrame.FrameOverlay(error, new Color(255, 96, 96),
                new MainFrame.OverlayAction("Close", KeyEvent.VK_C, () -> {})).asDimmedOverlay();

            overlay.setSize(800, 600);
            layoutRecursively(overlay);
            Container card = (Container) overlay.getComponent(0);
            JTextArea message = (JTextArea) card.getComponent(0);
            assertEquals(error, message.getText());
            assertTrue(message.getWidth() < overlay.getWidth() / 4,
                "The card currently collapses to a very narrow text column");
            try {
                Rectangle2D lastCharacter = message.modelToView2D(message.getDocument().getLength());
                assertTrue(lastCharacter.getMaxY() > message.getHeight(),
                    "The end of the critical error currently falls below the visible text area");
            } catch (BadLocationException e) {
                throw new AssertionError(e);
            }
        });
    }

    private static void layoutRecursively(Container container) {
        container.doLayout();
        for (Component child : container.getComponents()) {
            if (child instanceof Container) {
                layoutRecursively((Container) child);
            }
        }
    }
}
