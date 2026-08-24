package com.rusefi.maintenance.jobs;

import com.rusefi.core.io.ConnectedEcuTarget;
import com.rusefi.io.UpdateOperationCallbacks;
import com.rusefi.maintenance.DfuFlasher;
import org.jetbrains.annotations.Nullable;

public class DfuManualJob extends AsyncJob {
    private final ConnectedEcuTarget connectedEcuTarget;
    @Nullable
    private final String firmwareBinFile;

    public DfuManualJob(final ConnectedEcuTarget connectedEcuTarget) {
        this(connectedEcuTarget, null);
    }

    public DfuManualJob(final ConnectedEcuTarget connectedEcuTarget, final @Nullable String firmwareBinFile) {
        super("DFU update");
        this.connectedEcuTarget = connectedEcuTarget;
        this.firmwareBinFile = firmwareBinFile;
    }

    @Override
    public void doJob(final UpdateOperationCallbacks callbacks, final Runnable onJobFinished) {
        DfuFlasher.runDfuProgramming(callbacks, onJobFinished, connectedEcuTarget, firmwareBinFile);
    }
}
