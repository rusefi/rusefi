package com.rusefi.maintenance.jobs;

import com.rusefi.core.io.ConnectedEcuTarget;
import com.rusefi.io.UpdateOperationCallbacks;
import com.rusefi.maintenance.DfuFlasher;

public class DfuManualJob extends AsyncJob {
    private final ConnectedEcuTarget connectedEcuTarget;

    public DfuManualJob(final ConnectedEcuTarget connectedEcuTarget) {
        super("DFU update");
        this.connectedEcuTarget = connectedEcuTarget;
    }

    @Override
    public void doJob(final UpdateOperationCallbacks callbacks, final Runnable onJobFinished) {
        DfuFlasher.runDfuProgramming(callbacks, onJobFinished, connectedEcuTarget);
    }
}
