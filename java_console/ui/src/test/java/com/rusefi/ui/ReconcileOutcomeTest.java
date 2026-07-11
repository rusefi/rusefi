package com.rusefi.ui;

import org.junit.jupiter.api.Test;

import javax.swing.JOptionPane;

import static com.rusefi.ui.TuningPane.ReconcileOutcome.*;
import static com.rusefi.ui.TuningPane.reconcileOutcome;
import static org.junit.jupiter.api.Assertions.assertEquals;

/**
 * Reconnect-dialog choice → action mapping (#9730). The critical property: dismissing the dialog
 * (window X / Esc) never discards local edits.
 */
public class ReconcileOutcomeTest {

    // ---- same signature: option 0 = Keep Local and Burn, option 1 = Use ECU Tune ----

    @Test
    void sameSignatureKeepAndBurn() {
        assertEquals(WRITE_LOCAL, reconcileOutcome(true, 0, false));
    }

    @Test
    void sameSignatureUseEcu() {
        assertEquals(USE_ECU, reconcileOutcome(true, 1, false));
    }

    @Test
    void sameSignatureDismissKeepsLocal() {
        assertEquals(KEEP_LOCAL, reconcileOutcome(true, JOptionPane.CLOSED_OPTION, false));
    }

    // ---- different signature: option 0 = Migrate, option 1 = Use ECU Tune ----

    @Test
    void differentSignatureMigrate() {
        assertEquals(MIGRATE, reconcileOutcome(false, 0, false));
    }

    @Test
    void differentSignatureMigrateWithNothingMigratableUsesEcu() {
        // "Migrate" with an empty plan has nothing to apply — adopt the ECU tune rather than pretend.
        assertEquals(USE_ECU, reconcileOutcome(false, 0, true));
    }

    @Test
    void differentSignatureUseEcu() {
        assertEquals(USE_ECU, reconcileOutcome(false, 1, false));
    }

    @Test
    void differentSignatureDismissKeepsLocal() {
        assertEquals(KEEP_LOCAL, reconcileOutcome(false, JOptionPane.CLOSED_OPTION, false));
    }
}
