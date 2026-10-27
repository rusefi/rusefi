package com.rusefi.maintenance;

import com.rusefi.core.io.ConnectedEcuTarget;
import com.rusefi.io.UpdateOperationCallbacks;

import java.io.IOException;

/** Universal-console recovery. Invoked on the existing DFU job worker, never on the EDT. */
public final class ManualDfuRecovery {
    private ManualDfuRecovery() {
    }

    public interface BoardPicker {
        String pick(String suggestedTarget) throws IOException;
    }

    interface FirmwarePreparation {
        String prepare(String target) throws IOException;
    }

    interface FlashAction {
        void flash(String file, ConnectedEcuTarget target);
    }

    public static boolean isUniversalBundle(String bundleTarget) {
        return "universal".equalsIgnoreCase(bundleTarget);
    }

    static void run(ConnectedEcuTarget previousTarget, UpdateOperationCallbacks callbacks,
                    BoardPicker picker, FlashAction flash) {
        run(previousTarget, callbacks, picker,
            target -> DfuRecoveryFirmware.prepare(target, callbacks), flash);
    }

    static void run(ConnectedEcuTarget previousTarget, UpdateOperationCallbacks callbacks,
                     BoardPicker picker, FirmwarePreparation preparation, FlashAction flash) {
        ConnectedEcuTarget selected = selectBoard(previousTarget, callbacks, picker);
        if (selected != null) {
            runSelected(selected, callbacks, preparation, flash);
        }
    }

    /** Consent only: no firmware preparation, download or device access. Null is cancellation or a reported error. */
    public static ConnectedEcuTarget selectBoard(ConnectedEcuTarget previousTarget, UpdateOperationCallbacks callbacks,
                                                BoardPicker picker) {
        try {
            if (picker == null) {
                throw new IOException("Open Manual DFU Update in the console to select a board.");
            }
            String selected = picker.pick(previousTarget.effectiveTarget());
            if (selected == null) {
                callbacks.logLine("Manual DFU update cancelled; no firmware downloaded or flashed.");
                return null;
            }
            return ConnectedEcuTarget.forManualRecovery(selected);
        } catch (IOException | IllegalArgumentException e) {
            callbacks.logLine("Manual DFU update unavailable: " + e.getMessage());
            callbacks.error();
            return null;
        }
    }

    static void runSelected(ConnectedEcuTarget recoveryTarget, UpdateOperationCallbacks callbacks, FlashAction flash) {
        runSelected(recoveryTarget, callbacks, target -> DfuRecoveryFirmware.prepare(target, callbacks), flash);
    }

    private static void runSelected(ConnectedEcuTarget recoveryTarget, UpdateOperationCallbacks callbacks,
                                    FirmwarePreparation preparation, FlashAction flash) {
        try {
            String selected = recoveryTarget.effectiveTarget();
            callbacks.logLine("Manual DFU recovery board: " + selected);
            String firmware = preparation.prepare(selected);
            if (firmware == null) {
                throw new IOException("No full DFU binary is available for " + selected + ". Use OpenBLT for this firmware.");
            }
            flash.flash(firmware, recoveryTarget);
        } catch (IOException | IllegalArgumentException e) {
            callbacks.logLine("Manual DFU update unavailable: " + e.getMessage());
            callbacks.error();
        }
    }
}
