package com.rusefi.ui.basic;

import com.rusefi.autoupdate.Autoupdate;
import org.junit.jupiter.api.Test;

import javax.swing.JButton;
import javax.swing.JLabel;
import javax.swing.SwingUtilities;
import java.util.concurrent.CompletableFuture;
import java.util.concurrent.atomic.AtomicBoolean;

import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertFalse;
import static org.junit.jupiter.api.Assertions.assertTrue;

class StartupUpdateActionsTest {
    @Test
    void completedFailureEnablesManualRetry() throws Exception {
        JButton button = new JButton();
        button.setVisible(false);
        button.setEnabled(false);
        JLabel status = new JLabel();
        status.setVisible(false);
        AtomicBoolean failureObserved = new AtomicBoolean();

        StartupUpdateActions.bindSoftwareUpdateOutcome(
            CompletableFuture.completedFuture(Autoupdate.UpdateOutcome.FAILED),
            button, status, () -> failureObserved.set(true));
        SwingUtilities.invokeAndWait(() -> {});

        assertTrue(failureObserved.get());
        assertTrue(button.isVisible());
        assertTrue(button.isEnabled());
        assertTrue(status.isVisible());
        assertEquals("Automatic software update failed. You can retry.", status.getText());
    }

    @Test
    void completedNoOpDoesNotOfferRetry() throws Exception {
        JButton button = new JButton();
        button.setVisible(false);
        button.setEnabled(false);
        JLabel status = new JLabel();
        status.setVisible(false);

        StartupUpdateActions.bindSoftwareUpdateOutcome(
            CompletableFuture.completedFuture(Autoupdate.UpdateOutcome.UP_TO_DATE),
            button, status, () -> {});
        SwingUtilities.invokeAndWait(() -> {});

        assertFalse(button.isVisible());
        assertFalse(button.isEnabled());
        assertFalse(status.isVisible());
    }
}
