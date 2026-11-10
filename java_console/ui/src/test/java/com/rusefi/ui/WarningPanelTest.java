package com.rusefi.ui;

import com.rusefi.core.MessagesCentral;
import com.rusefi.config.generated.Integration;
import com.rusefi.core.preferences.storage.Node;
import org.junit.jupiter.api.Test;

import javax.swing.*;
import java.awt.*;
import java.awt.image.BufferedImage;
import java.util.LinkedHashMap;
import java.util.Map;

import static org.junit.jupiter.api.Assertions.*;

public class WarningPanelTest {
    @Test
    public void criticalBlinkDoesNotMoveStatusControls() throws Exception {
        WarningPanel[] warning = new WarningPanel[1];
        SwingUtilities.invokeAndWait(() -> {
            UIContext context = new UIContext();
            context.recordButtonFactory = () -> new JButton("Record");
            warning[0] = new WarningPanel(new Node(), context);
        });
        MessagesCentral.getInstance().postMessage(WarningPanelTest.class,
            Integration.CRITICAL_PREFIX + " bad isr priority at fastAdcStartTrigger expected 30 got 70");
        SwingUtilities.invokeAndWait(() -> {
            WarningPanel status = warning[0];
            JPanel strip = status.getPanel(null);
            JButton clear = findButton(strip, "Clear warning");
            try {
                strip.setSize(1200, 52);
                layoutAll(strip);
                Map<Component, Rectangle> before = controlBounds(strip, status.getWarningLabel());
                JLabel label = status.getWarningLabel();
                Rectangle warningBounds = label.getBounds();
                Dimension preferred = strip.getPreferredSize();
                assertTrue(hasPaintedPixels(label));
                assertEquals(label.getText(), label.getToolTipText());
                status.blinkWarning();
                layoutAll(strip);
                assertEquals(before, controlBounds(strip, label));
                assertEquals(warningBounds, label.getBounds());
                assertEquals(preferred, strip.getPreferredSize());
                assertFalse(hasPaintedPixels(label), "The text and icon disappear during the off phase");
                status.blinkWarning();
                assertTrue(hasPaintedPixels(label));
                assertEquals(SwingConstants.CENTER, label.getHorizontalAlignment());
                assertSame(label, ((BorderLayout) strip.getLayout()).getLayoutComponent(BorderLayout.CENTER));
                status.blinkWarning();
                clear.doClick(0);
                layoutAll(strip);
                assertTrue(hasPaintedPixels(label), "Clear restores the idle message even during the off phase");
                assertEquals(before, controlBounds(strip, label));
            } finally {
                clear.doClick(0);
            }
        });
    }

    private static boolean hasPaintedPixels(JLabel label) {
        BufferedImage image = new BufferedImage(label.getWidth(), label.getHeight(), BufferedImage.TYPE_INT_ARGB);
        Graphics graphics = image.getGraphics();
        try {
            label.paint(graphics);
        } finally {
            graphics.dispose();
        }
        for (int y = 0; y < image.getHeight(); y++) {
            for (int x = 0; x < image.getWidth(); x++) {
                if ((image.getRGB(x, y) >>> 24) != 0) {
                    return true;
                }
            }
        }
        return false;
    }

    private static Map<Component, Rectangle> controlBounds(Container container, JLabel warning) {
        Map<Component, Rectangle> result = new LinkedHashMap<>();
        for (Component child : container.getComponents()) {
            if (child == warning) {
                continue;
            }
            result.put(child, child.getBounds());
            if (child instanceof Container) {
                result.putAll(controlBounds((Container) child, warning));
            }
        }
        return result;
    }

    @Test
    public void showsIdleTextUntilAWarningArrivesAndClearRestoresIt() throws Exception {
        WarningPanel[] panel = new WarningPanel[1];
        SwingUtilities.invokeAndWait(() -> {
            panel[0] = new WarningPanel(new Node());
            assertEquals(WarningPanel.NO_WARNINGS, panel[0].getWarningLabel().getText());
        });

        // delivered on the EDT
        MessagesCentral.getInstance().postMessage(WarningPanelTest.class, "WARNING: low battery");
        SwingUtilities.invokeAndWait(() -> {
            assertEquals("WARNING: low battery", panel[0].getWarningLabel().getText());
            JButton clear = findButton(panel[0].getPanel(null), "Clear warning");
            assertNotNull(clear);
            assertTrue(clear.isEnabled());
            clear.doClick(0);
            assertEquals(WarningPanel.NO_WARNINGS, panel[0].getWarningLabel().getText());
            assertFalse(clear.isEnabled());
        });
    }

    /** The console's logging button sits at the right end of the strip, after the warnings and log controls. */
    @Test
    public void recordButtonFromTheConsoleSitsAtTheRight() throws Exception {
        SwingUtilities.invokeAndWait(() -> {
            UIContext uiContext = new UIContext();
            uiContext.recordButtonFactory = () -> new JButton("Record");
            JPanel strip = new WarningPanel(new Node(), uiContext).getPanel(null);
            JButton record = findButton(strip, "Record");
            assertNotNull(record);
            JButton clear = findButton(strip, "Clear warning");
            strip.setSize(1200, 50);
            layoutAll(strip);
            int recordX = javax.swing.SwingUtilities.convertPoint(record, 0, 0, strip).x;
            int clearX = javax.swing.SwingUtilities.convertPoint(clear, 0, 0, strip).x;
            assertTrue(recordX > 1000, "right end: " + recordX);
            assertTrue(recordX > clearX);

            assertNull(findButton(new WarningPanel(new Node()).getPanel(null), "Record"), "no console, no button");
        });
    }

    private static void layoutAll(java.awt.Container container) {
        container.doLayout();
        for (java.awt.Component child : container.getComponents()) {
            if (child instanceof java.awt.Container) {
                layoutAll((java.awt.Container) child);
            }
        }
    }

    private static JButton findButton(java.awt.Container container, String text) {
        for (java.awt.Component child : container.getComponents()) {
            if (child instanceof JButton && text.equals(((JButton) child).getText())) {
                return (JButton) child;
            }
            if (child instanceof java.awt.Container) {
                JButton found = findButton((java.awt.Container) child, text);
                if (found != null) {
                    return found;
                }
            }
        }
        return null;
    }
}
