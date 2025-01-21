package com.rusefi.maintenance.jobs;

import com.rusefi.Launcher;
import com.rusefi.io.DoubleCallbacks;
import com.rusefi.io.UpdateOperationCallbacks;
import com.rusefi.maintenance.ExecHelper;
import com.rusefi.ui.StatusWindow;

import static com.rusefi.core.ui.FrameHelper.appendBundleName;

public enum AsyncJobExecutor {
    INSTANCE;

    public void executeJobWithStatusWindow(final AsyncJob job) {
        executeJobWithStatusWindow(job, UpdateOperationCallbacks.DUMMY);
    }

    public void executeJobWithStatusWindow(final AsyncJob job, UpdateOperationCallbacks secondary) {
        final UpdateOperationCallbacks callbacks = StatusWindow.createAndShowFrame(appendBundleName(job.getName() + " " + Launcher.CONSOLE_VERSION));
        final UpdateOperationCallbacks doubleCallbacks = new DoubleCallbacks(callbacks, secondary);
        final Runnable jobWithSuspendedPortScanning = () -> job.doJob(doubleCallbacks);
        ExecHelper.submitAction(jobWithSuspendedPortScanning, "mx");
    }
}
