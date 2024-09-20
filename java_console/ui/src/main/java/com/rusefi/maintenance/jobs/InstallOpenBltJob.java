package com.rusefi.maintenance.jobs;

import com.rusefi.io.UpdateOperationCallbacks;
import com.rusefi.maintenance.DfuFlasher;

public class InstallOpenBltJob extends AsyncJob {
    public InstallOpenBltJob() {
        super("OpenBLT Initial Programming");
    }

    @Override
    public void doJob(final UpdateOperationCallbacks callbacks) {
        DfuFlasher.runOpenBltInitialProgramming(callbacks);
    }
}
