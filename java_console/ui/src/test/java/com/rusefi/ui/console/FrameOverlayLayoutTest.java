package com.rusefi.ui.console;

import org.junit.jupiter.api.Test;

import javax.swing.JButton;
import javax.swing.JTextPane;
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
                JTextPane message = findMessage(card);
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
            JTextPane message = findMessage(overlay);
            assertNotNull(message);
            JViewport viewport = (JViewport) message.getParent();
            try {
                Rectangle2D lastCharacter = message.modelToView2D(message.getDocument().getLength());
                assertTrue(lastCharacter.getMaxY() > viewport.getExtentSize().height);
                message.scrollRectToVisible(lastCharacter.getBounds());
                // the caret box is zero wide, which Rectangle.contains never accepts: compare rows
                Rectangle visible = message.getVisibleRect();
                assertTrue(lastCharacter.getMinY() >= visible.getMinY() && lastCharacter.getMaxY() <= visible.getMaxY(),
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

    @Test
    void messageIsCentredAboveTheCentredButtons() throws Exception {
        SwingUtilities.invokeAndWait(() -> {
            String text = "The tune has changes that have not been burned to the ECU.";
            MainFrame.FrameOverlay plain = new MainFrame.FrameOverlay(text, Color.DARK_GRAY,
                new MainFrame.OverlayAction("Burn to ECU and Exit", KeyEvent.VK_S, () -> {}),
                new MainFrame.OverlayAction("Exit Without Saving", KeyEvent.VK_E, () -> {}),
                new MainFrame.OverlayAction("Cancel", KeyEvent.VK_C, () -> {}));
            MainFrame.FrameOverlay dimmed = new MainFrame.FrameOverlay("Config Error", Color.RED,
                new MainFrame.OverlayAction("Close", KeyEvent.VK_C, () -> {})).asDimmedOverlay();
            for (MainFrame.FrameOverlay overlay : new MainFrame.FrameOverlay[]{plain, dimmed}) {
                overlay.setSize(1905, 990);
                layoutRecursively(overlay);
                JTextPane message = findMessage(overlay);
                assertNotNull(message);
                JButton button = findButton(overlay);
                assertNotNull(button);
                try {
                    Rectangle2D first = message.modelToView2D(0);
                    Rectangle2D last = message.modelToView2D(message.getDocument().getLength());
                    assertEquals(first.getY(), last.getY(), 0.5, "The message should fit on one line");
                    Rectangle textBounds = SwingUtilities.convertRectangle(message,
                        new Rectangle((int) first.getX(), (int) first.getY(), (int) (last.getX() - first.getX()), 1), overlay);
                    Container buttons = button.getParent();
                    Rectangle buttonBounds = SwingUtilities.convertRectangle(buttons.getParent(), buttons.getBounds(), overlay);
                    Component[] row = buttons.getComponents();
                    Rectangle firstButton = SwingUtilities.convertRectangle(buttons, row[0].getBounds(), overlay);
                    Rectangle lastButton = SwingUtilities.convertRectangle(buttons, row[row.length - 1].getBounds(), overlay);
                    double buttonsCentre = (firstButton.getMinX() + lastButton.getMaxX()) / 2;
                    assertEquals(buttonsCentre, textBounds.getCenterX(), 4,
                        "The message and the buttons should share one centre line");
                    assertTrue(buttonBounds.width > 0);
                } catch (BadLocationException e) {
                    throw new AssertionError(e);
                }
            }
        });
    }

    @Test
    void longStatusErrorIsWrappedToTheWindow() {
        java.awt.FontMetrics fm = new JButton().getFontMetrics(new java.awt.Font("Dialog", java.awt.Font.PLAIN, 48));
        String error = "CRITICAL bad isr priority at fastAdcStartTrigger expected 30 got 70";
        java.util.List<String> lines = TabbedPanel.wrapLines(fm, error, 600);
        assertTrue(lines.size() > 1);
        assertEquals(error, String.join(" ", lines));
        for (String line : lines) {
            assertTrue(fm.stringWidth(line) <= 600 || !line.contains(" "), line);
        }
        assertEquals(java.util.Collections.singletonList("Loading"), TabbedPanel.wrapLines(fm, "Loading", 600));
    }

    private static void layoutRecursively(Container container) {
        container.doLayout();
        for (Component child : container.getComponents()) {
            if (child instanceof Container) {
                layoutRecursively((Container) child);
            }
        }
    }

    private static JTextPane findMessage(Container container) {
        for (Component child : container.getComponents()) {
            if (child instanceof JTextPane) {
                return (JTextPane) child;
            }
            if (child instanceof Container) {
                JTextPane found = findMessage((Container) child);
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
