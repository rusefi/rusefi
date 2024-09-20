package com.rusefi.maintenance.jobs;

import com.rusefi.io.UpdateOperationCallbacks;
import com.rusefi.maintenance.DfuFlasher;

import static com.rusefi.maintenance.JobType.INSTALL_OPENBLT;

public class InstallOpenBltJob extends AsyncJob {
    public InstallOpenBltJob() {
        super(INSTALL_OPENBLT.jobName);
    }

    @Override
    public void doJob(final UpdateOperationCallbacks callbacks) {
        DfuFlasher.runOpenBltInitialProgramming(callbacks);
    }
}
