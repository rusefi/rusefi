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
