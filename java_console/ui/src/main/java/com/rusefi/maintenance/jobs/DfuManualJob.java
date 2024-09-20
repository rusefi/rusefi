package com.rusefi.maintenance.jobs;

import com.rusefi.io.UpdateOperationCallbacks;
import com.rusefi.maintenance.DfuFlasher;

public class DfuManualJob extends AsyncJob {
    public DfuManualJob() {
        super("DFU update");
    }

    @Override
    public void doJob(final UpdateOperationCallbacks callbacks) {
        DfuFlasher.runDfuProgramming(callbacks);
    }
}
