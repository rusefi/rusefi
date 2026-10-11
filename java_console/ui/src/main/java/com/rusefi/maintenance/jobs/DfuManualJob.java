package com.rusefi.maintenance.jobs;

import com.rusefi.core.io.ConnectedEcuTarget;
import com.rusefi.io.UpdateOperationCallbacks;
import com.rusefi.maintenance.DfuFlasher;
import com.rusefi.maintenance.ManualDfuRecovery;
import org.jetbrains.annotations.Nullable;

public class DfuManualJob extends AsyncJob {
    private final ConnectedEcuTarget connectedEcuTarget;
    @Nullable
    private final String firmwareBinFile;
    private final ManualDfuRecovery.BoardPicker boardPicker;

    public DfuManualJob(final ConnectedEcuTarget connectedEcuTarget) {
        this(connectedEcuTarget, null);
    }

    public DfuManualJob(final ConnectedEcuTarget connectedEcuTarget, final @Nullable String firmwareBinFile) {
        this(connectedEcuTarget, firmwareBinFile, null);
    }

    public DfuManualJob(final ConnectedEcuTarget connectedEcuTarget, final @Nullable String firmwareBinFile,
                        final @Nullable ManualDfuRecovery.BoardPicker boardPicker) {
        super("DFU update");
        this.connectedEcuTarget = connectedEcuTarget;
        this.firmwareBinFile = firmwareBinFile;
        this.boardPicker = boardPicker;
    }

    @Override
    public void doJob(final UpdateOperationCallbacks callbacks, final Runnable onJobFinished) {
        DfuFlasher.runDfuProgramming(callbacks, onJobFinished, connectedEcuTarget, firmwareBinFile, boardPicker);
    }
}
