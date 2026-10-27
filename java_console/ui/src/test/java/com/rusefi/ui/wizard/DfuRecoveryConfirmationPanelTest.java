package com.rusefi.ui.wizard;

import org.junit.jupiter.api.Test;

import javax.swing.*;
import java.awt.event.ActionEvent;
import java.util.concurrent.atomic.AtomicInteger;

import static org.junit.jupiter.api.Assertions.*;

class DfuRecoveryConfirmationPanelTest {
    @Test
    void rendersPlainTextAndDefaultsFocusToCancelWithoutAnEnterDefault() throws Exception {
        SwingUtilities.invokeAndWait(() -> {
            String message = "Recovery <board> & settings\nKeep only the intended ECU connected.";
            DfuRecoveryConfirmationPanel panel = new DfuRecoveryConfirmationPanel(message, () -> {}, () -> {});
            assertEquals(message, panel.messageDetailsForTests().getText());
            assertFalse(panel.messageDetailsForTests().isEditable());
            assertSame(panel.cancelButtonForTests(), panel.getFocusTraversalPolicy().getDefaultComponent(panel));
            JRootPane root = new JRootPane();
            root.setContentPane(panel);
            assertNull(root.getDefaultButton(), "Enter must not implicitly consent to recovery");
        });
    }

    @Test
    void confirmIsOneShotEvenWithQueuedOrStaleActions() throws Exception {
        SwingUtilities.invokeAndWait(() -> {
            AtomicInteger confirmations = new AtomicInteger();
            AtomicInteger cancellations = new AtomicInteger();
            DfuRecoveryConfirmationPanel panel = new DfuRecoveryConfirmationPanel(
                "Recovery details", confirmations::incrementAndGet, cancellations::incrementAndGet);

            panel.confirmButtonForTests().doClick();
            assertFalse(panel.confirmButtonForTests().isEnabled());
            assertFalse(panel.cancelButtonForTests().isEnabled());
            panel.confirmButtonForTests().setEnabled(true);
            panel.confirmButtonForTests().doClick();
            panel.cancelButtonForTests().setEnabled(true);
            panel.cancelButtonForTests().doClick();
            escape(panel);

            assertEquals(1, confirmations.get());
            assertEquals(0, cancellations.get());
        });
    }

    @Test
    void cancelAndEscapeInvalidateAllSubsequentConfirmationActions() throws Exception {
        SwingUtilities.invokeAndWait(() -> {
            for (boolean useEscape : new boolean[]{false, true}) {
                AtomicInteger confirmations = new AtomicInteger();
                AtomicInteger cancellations = new AtomicInteger();
                DfuRecoveryConfirmationPanel panel = new DfuRecoveryConfirmationPanel(
                    "Recovery details", confirmations::incrementAndGet, cancellations::incrementAndGet);
                if (useEscape) {
                    escape(panel);
                } else {
                    panel.cancelButtonForTests().doClick();
                }
                assertFalse(panel.confirmButtonForTests().isEnabled());
                panel.confirmButtonForTests().setEnabled(true);
                panel.confirmButtonForTests().doClick();
                escape(panel);
                assertEquals(0, confirmations.get());
                assertEquals(1, cancellations.get());
            }
        });
    }

    @Test
    void hidingOrRemovingPanelInvalidatesItWithoutClosingAnotherScreen() throws Exception {
        SwingUtilities.invokeAndWait(() -> {
            for (boolean remove : new boolean[]{false, true}) {
                AtomicInteger confirmations = new AtomicInteger();
                AtomicInteger cancellations = new AtomicInteger();
                DfuRecoveryConfirmationPanel panel = new DfuRecoveryConfirmationPanel(
                    "Recovery details", confirmations::incrementAndGet, cancellations::incrementAndGet);
                if (remove) {
                    panel.removeNotify();
                } else {
                    panel.setVisible(false);
                }
                panel.confirmButtonForTests().setEnabled(true);
                panel.confirmButtonForTests().doClick();
                escape(panel);
                assertEquals(0, confirmations.get());
                assertEquals(0, cancellations.get());
            }
        });
    }

    private static void escape(JComponent panel) {
        Object key = panel.getInputMap(JComponent.WHEN_IN_FOCUSED_WINDOW).get(KeyStroke.getKeyStroke("ESCAPE"));
        assertNotNull(key);
        panel.getActionMap().get(key).actionPerformed(new ActionEvent(panel, ActionEvent.ACTION_PERFORMED, "escape"));
    }
}
