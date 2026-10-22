package com.rusefi.ui;

import com.rusefi.core.MessagesCentral;
import com.rusefi.core.preferences.storage.Node;
import org.junit.jupiter.api.Test;

import javax.swing.*;

import static org.junit.jupiter.api.Assertions.*;

public class WarningPanelTest {
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
