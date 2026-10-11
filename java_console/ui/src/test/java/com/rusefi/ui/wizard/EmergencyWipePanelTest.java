package com.rusefi.ui.wizard;

import org.junit.jupiter.api.Test;

import javax.swing.*;
import java.awt.event.ActionEvent;
import java.util.concurrent.atomic.AtomicInteger;

import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertNotNull;

class EmergencyWipePanelTest {
    @Test
    void requiresExplicitEraseAndCannotSubmitTwice() throws Exception {
        AtomicInteger eraseCount = new AtomicInteger();
        AtomicInteger cancelCount = new AtomicInteger();
        EmergencyWipePanel[] panel = new EmergencyWipePanel[1];
        SwingUtilities.invokeAndWait(() -> panel[0] = new EmergencyWipePanel(
            "Target details", eraseCount::incrementAndGet, cancelCount::incrementAndGet));

        SwingUtilities.invokeAndWait(panel[0].cancelButtonForTests()::doClick);
        assertEquals(0, eraseCount.get());
        assertEquals(1, cancelCount.get());

        SwingUtilities.invokeAndWait(() -> {
            panel[0].eraseButtonForTests().doClick();
            panel[0].eraseButtonForTests().doClick();
        });
        assertEquals(1, eraseCount.get());
    }

    @Test
    void escapeCancels() throws Exception {
        AtomicInteger cancelCount = new AtomicInteger();
        EmergencyWipePanel[] panel = new EmergencyWipePanel[1];
        SwingUtilities.invokeAndWait(() -> panel[0] = new EmergencyWipePanel(
            "Target details", () -> { }, cancelCount::incrementAndGet));

        SwingUtilities.invokeAndWait(() -> {
            KeyStroke escape = KeyStroke.getKeyStroke("ESCAPE");
            Object actionKey = panel[0].getInputMap(JComponent.WHEN_IN_FOCUSED_WINDOW).get(escape);
            assertNotNull(actionKey);
            panel[0].getActionMap().get(actionKey).actionPerformed(
                new ActionEvent(panel[0], ActionEvent.ACTION_PERFORMED, "escape"));
        });

        assertEquals(1, cancelCount.get());
    }
}
