package com.rusefi.maintenance.jobs;

import com.rusefi.core.io.ConnectedEcuTarget;
import com.rusefi.io.UpdateOperationCallbacks;
import com.rusefi.maintenance.DfuFlasher;

public class InstallOpenBltJob extends AsyncJob {
    private final ConnectedEcuTarget connectedEcuTarget;

    public InstallOpenBltJob(final ConnectedEcuTarget connectedEcuTarget) {
        super("OpenBLT Initial Programming");
        this.connectedEcuTarget = connectedEcuTarget;
    }

    @Override
    public void doJob(final UpdateOperationCallbacks callbacks, final Runnable onJobFinished) {
        DfuFlasher.runOpenBltInitialProgramming(callbacks, onJobFinished, connectedEcuTarget);
    }
}
