package com.rusefi.maintenance.jobs;

import com.rusefi.core.io.ConnectedEcuTarget;
import com.rusefi.core.io.BundleUtil;
import com.rusefi.io.UpdateOperationCallbacks;
import com.rusefi.maintenance.DfuFlasher;
import com.rusefi.maintenance.ManualDfuRecovery;
import org.jetbrains.annotations.Nullable;
import java.util.Objects;
import java.util.function.BooleanSupplier;

public class DfuManualJob extends AsyncJob {
    private final ConnectedEcuTarget connectedEcuTarget;
    @Nullable
    private final String firmwareBinFile;
    private final ManualDfuRecovery.BoardPicker boardPicker;
    private final BooleanSupplier stillEligible;
    @Nullable
    private ConnectedEcuTarget selectedRecoveryTarget;

    public DfuManualJob(final ConnectedEcuTarget connectedEcuTarget) {
        this(connectedEcuTarget, null);
    }

    public DfuManualJob(final ConnectedEcuTarget connectedEcuTarget, final @Nullable String firmwareBinFile) {
        this(connectedEcuTarget, firmwareBinFile, null);
    }

    public DfuManualJob(final ConnectedEcuTarget connectedEcuTarget, final @Nullable String firmwareBinFile,
                         final @Nullable ManualDfuRecovery.BoardPicker boardPicker) {
        this(connectedEcuTarget, firmwareBinFile, boardPicker, () -> true);
    }

    public DfuManualJob(ConnectedEcuTarget connectedEcuTarget, @Nullable String firmwareBinFile,
                        @Nullable ManualDfuRecovery.BoardPicker boardPicker, BooleanSupplier stillEligible) {
        super("Recover ECU");
        this.connectedEcuTarget = connectedEcuTarget;
        this.firmwareBinFile = firmwareBinFile;
        this.boardPicker = boardPicker;
        this.stillEligible = Objects.requireNonNull(stillEligible);
    }

    @Override
    public boolean prepare(UpdateOperationCallbacks callbacks) {
        selectedRecoveryTarget = null;
        if (!validateEligibility(callbacks)) {
            return false;
        }
        if (firmwareBinFile == null && ManualDfuRecovery.isUniversalBundle(BundleUtil.getBundleTarget())) {
            selectedRecoveryTarget = ManualDfuRecovery.selectBoard(connectedEcuTarget, callbacks, boardPicker);
            return selectedRecoveryTarget != null && validateEligibility(callbacks);
        }
        return true;
    }

    private boolean validateEligibility(UpdateOperationCallbacks callbacks) {
        if (!stillEligible.getAsBoolean()) {
            callbacks.logLine("Device or programming eligibility changed; recovery cancelled before programming.");
            return false;
        }
        return true;
    }

    @Override
    public void doJob(final UpdateOperationCallbacks callbacks, final Runnable onJobFinished) {
        // Active hooks can drain the scanner or perform a handoff after prepare() accepted consent.
        // Recheck their latest published state before entering any download/programming backend.
        if (!validateEligibility(callbacks)) {
            onJobFinished.run();
            return;
        }
        if (selectedRecoveryTarget != null) {
            DfuFlasher.runPreparedManualRecovery(callbacks, onJobFinished, selectedRecoveryTarget);
        } else {
            DfuFlasher.runDfuProgramming(callbacks, onJobFinished, connectedEcuTarget, firmwareBinFile, boardPicker);
        }
    }
}
