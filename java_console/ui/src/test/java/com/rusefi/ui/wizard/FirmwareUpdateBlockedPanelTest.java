package com.rusefi.ui.wizard;

import org.junit.jupiter.api.Test;

import javax.swing.*;
import java.awt.event.ActionEvent;
import java.util.concurrent.atomic.AtomicInteger;

import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertNotNull;

class FirmwareUpdateBlockedPanelTest {
    @Test
    void rendersProviderMessageAsExactPlainTextAndClosesOnlyOnce() throws Exception {
        String providerMessage = "Build <unsupported> & must not be rendered as HTML\nUse a compatible version.";
        AtomicInteger closeCount = new AtomicInteger();
        FirmwareUpdateBlockedPanel[] panel = new FirmwareUpdateBlockedPanel[1];
        SwingUtilities.invokeAndWait(() -> panel[0] = new FirmwareUpdateBlockedPanel(
            providerMessage, closeCount::incrementAndGet));

        SwingUtilities.invokeAndWait(() -> {
            assertEquals(providerMessage, panel[0].messageDetailsForTests().getText());
            panel[0].backButtonForTests().doClick();
            panel[0].backButtonForTests().doClick();
        });

        assertEquals(1, closeCount.get());
    }

    @Test
    void escapeClosesPanel() throws Exception {
        AtomicInteger closeCount = new AtomicInteger();
        FirmwareUpdateBlockedPanel[] panel = new FirmwareUpdateBlockedPanel[1];
        SwingUtilities.invokeAndWait(() -> panel[0] = new FirmwareUpdateBlockedPanel(
            "Target details", closeCount::incrementAndGet));

        SwingUtilities.invokeAndWait(() -> {
            KeyStroke escape = KeyStroke.getKeyStroke("ESCAPE");
            Object actionKey = panel[0].getInputMap(JComponent.WHEN_IN_FOCUSED_WINDOW).get(escape);
            assertNotNull(actionKey);
            panel[0].getActionMap().get(actionKey).actionPerformed(
                new ActionEvent(panel[0], ActionEvent.ACTION_PERFORMED, "escape"));
            panel[0].backButtonForTests().doClick();
        });

        assertEquals(1, closeCount.get());
    }
}
