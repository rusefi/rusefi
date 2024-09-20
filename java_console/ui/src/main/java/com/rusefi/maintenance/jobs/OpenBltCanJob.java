package com.rusefi.maintenance.jobs;

import com.rusefi.io.UpdateOperationCallbacks;
import com.rusefi.maintenance.ProgramSelector;

import javax.swing.*;

import static com.rusefi.maintenance.JobType.OPENBLT_CAN;

public class OpenBltCanJob extends AsyncJobWithContext<ParentComponentContext> {
    public OpenBltCanJob(final JComponent parent) {
        super(OPENBLT_CAN.jobName, new ParentComponentContext(parent));
    }

    @Override
    public void doJob(final UpdateOperationCallbacks callbacks) {
        ProgramSelector.flashOpenBltCan(context.getParent(), callbacks);
    }
}
