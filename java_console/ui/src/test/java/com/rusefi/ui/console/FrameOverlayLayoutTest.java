package com.rusefi.ui.console;

import org.junit.jupiter.api.Test;

import javax.swing.JButton;
import javax.swing.JTextArea;
import javax.swing.JViewport;
import javax.swing.SwingUtilities;
import javax.swing.text.BadLocationException;
import java.awt.Color;
import java.awt.Component;
import java.awt.Container;
import java.awt.Rectangle;
import java.awt.event.KeyEvent;
import java.awt.geom.Rectangle2D;

import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertNotNull;
import static org.junit.jupiter.api.Assertions.assertTrue;

class FrameOverlayLayoutTest {
    @Test
    void duplicateInjectorPinErrorFitsInNarrowWindow() throws Exception {
        SwingUtilities.invokeAndWait(() -> {
            // Issue #10490: duplicate injector outputs produce this critical-error format.
            String error = "Config Error\n\nPin \"PA1\" (Injector 1) required by \"Injector 2\" but is used by \"Injector 1\"";
            MainFrame.FrameOverlay overlay = new MainFrame.FrameOverlay(error, new Color(255, 96, 96),
                new MainFrame.OverlayAction("Close", KeyEvent.VK_C, () -> {})).asDimmedOverlay();

            for (int[] size : new int[][]{{800, 600}, {1280, 800}}) {
                overlay.setSize(size[0], size[1]);
                layoutRecursively(overlay);
                Container card = (Container) overlay.getComponent(0);
                JTextArea message = findMessage(card);
                assertNotNull(message);
                assertEquals(error, message.getText());
                assertTrue(card.getX() >= 0 && card.getX() + card.getWidth() <= overlay.getWidth());
                assertTrue(card.getY() >= 0 && card.getY() + card.getHeight() <= overlay.getHeight());
                assertTrue(message.getWidth() >= overlay.getWidth() / 2,
                    "The card should leave enough width to read the error");
                try {
                    Rectangle2D lastCharacter = message.modelToView2D(message.getDocument().getLength());
                    JViewport viewport = (JViewport) message.getParent();
                    assertTrue(lastCharacter.getMaxY() <= viewport.getExtentSize().height,
                        "The end of the critical error should be visible");
                } catch (BadLocationException e) {
                    throw new AssertionError(e);
                }
            }
        });
    }

    @Test
    void longConfigErrorCanBeScrolledWhileCloseStaysVisible() throws Exception {
        SwingUtilities.invokeAndWait(() -> {
            StringBuilder error = new StringBuilder("Config Error\n\n");
            for (int i = 0; i < 12; i++) {
                error.append("Pin \"PA1\" required by \"Injector 2\" but is used by \"Injector 1\"\n");
            }
            MainFrame.FrameOverlay overlay = new MainFrame.FrameOverlay(error.toString(), Color.RED,
                new MainFrame.OverlayAction("Close", KeyEvent.VK_C, () -> {})).asDimmedOverlay();

            overlay.setSize(640, 480);
            layoutRecursively(overlay);
            JTextArea message = findMessage(overlay);
            assertNotNull(message);
            JViewport viewport = (JViewport) message.getParent();
            try {
                Rectangle2D lastCharacter = message.modelToView2D(message.getDocument().getLength());
                assertTrue(lastCharacter.getMaxY() > viewport.getExtentSize().height);
                message.scrollRectToVisible(lastCharacter.getBounds());
                assertTrue(message.getVisibleRect().contains(lastCharacter.getBounds()),
                    "The end of a longer error should be reachable by scrolling");
            } catch (BadLocationException e) {
                throw new AssertionError(e);
            }

            Container card = (Container) overlay.getComponent(0);
            assertTrue(card.getY() >= 0 && card.getY() + card.getHeight() <= overlay.getHeight());
            JButton close = findButton(card);
            assertNotNull(close);
            Rectangle closeBounds = SwingUtilities.convertRectangle(close.getParent(), close.getBounds(), overlay);
            assertTrue(closeBounds.y >= 0 && closeBounds.y + closeBounds.height <= overlay.getHeight());
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

    private static JTextArea findMessage(Container container) {
        for (Component child : container.getComponents()) {
            if (child instanceof JTextArea) {
                return (JTextArea) child;
            }
            if (child instanceof Container) {
                JTextArea found = findMessage((Container) child);
                if (found != null) {
                    return found;
                }
            }
        }
        return null;
    }

    private static JButton findButton(Container container) {
        for (Component child : container.getComponents()) {
            if (child instanceof JButton) {
                return (JButton) child;
            }
            if (child instanceof Container) {
                JButton found = findButton((Container) child);
                if (found != null) {
                    return found;
                }
            }
        }
        return null;
    }
}
