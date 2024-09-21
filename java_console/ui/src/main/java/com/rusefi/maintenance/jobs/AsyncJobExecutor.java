package com.rusefi.maintenance.jobs;

import com.rusefi.Launcher;
import com.rusefi.io.UpdateOperationCallbacks;
import com.rusefi.maintenance.ExecHelper;
import com.rusefi.maintenance.UpdateStatusWindow;

import static com.rusefi.core.ui.FrameHelper.appendBundleName;

public enum AsyncJobExecutor {
    INSTANCE;

    public void executeJob(final AsyncJob job) {
        final UpdateOperationCallbacks callbacks = new UpdateStatusWindow(appendBundleName(job.getName() + " " + Launcher.CONSOLE_VERSION));
        final Runnable jobWithSuspendedPortScanning = () -> job.doJob(callbacks);
        ExecHelper.submitAction(jobWithSuspendedPortScanning, "mx");
    }
}
