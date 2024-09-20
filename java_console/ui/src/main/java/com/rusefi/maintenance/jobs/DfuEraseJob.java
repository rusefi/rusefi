package com.rusefi.maintenance.jobs;

import com.rusefi.io.UpdateOperationCallbacks;
import com.rusefi.maintenance.DfuFlasher;

import static com.rusefi.maintenance.JobType.DFU_ERASE;

public class DfuEraseJob extends AsyncJob {
    public DfuEraseJob() {
        super(DFU_ERASE.jobName);
    }

    @Override
    public void doJob(UpdateOperationCallbacks callbacks) {
        DfuFlasher.runDfuEraseAsync(callbacks);
    }
}
