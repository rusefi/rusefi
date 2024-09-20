package com.rusefi.maintenance.jobs;

import com.rusefi.io.UpdateOperationCallbacks;
import com.rusefi.maintenance.DfuFlasher;

public class DfuEraseJob extends AsyncJob {
    public DfuEraseJob() {
        super("DFU erase");
    }

    @Override
    public void doJob(UpdateOperationCallbacks callbacks) {
        DfuFlasher.runDfuEraseAsync(callbacks);
    }
}
