package com.rusefi.maintenance.jobs;

import com.rusefi.io.UpdateOperationCallbacks;
import com.rusefi.maintenance.DfuFlasher;

import static com.rusefi.maintenance.JobType.DFU_MANUAL;

public class DfuManualJob extends AsyncJob {
    public DfuManualJob() {
        super(DFU_MANUAL.jobName);
    }

    @Override
    public void doJob(final UpdateOperationCallbacks callbacks) {
        DfuFlasher.runDfuProgramming(callbacks);
    }
}
