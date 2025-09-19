package com.rusefi.maintenance.jobs;

import com.rusefi.Launcher;
import com.rusefi.io.DoubleCallbacks;
import com.rusefi.io.UpdateOperationCallbacks;
import com.rusefi.maintenance.ExecHelper;
import com.rusefi.ui.StatusWindow;

import java.util.concurrent.atomic.AtomicInteger;

import static com.rusefi.core.ui.FrameHelper.appendBundleName;

public enum AsyncJobExecutor {
    INSTANCE;

    private final AtomicInteger threadNameIndex = new AtomicInteger();

    public void executeJobWithStatusWindow(final AsyncJob job) {
        executeJobWithStatusWindow(job, UpdateOperationCallbacks.DUMMY, () -> {});
    }

    public void executeJobWithStatusWindow(
        final AsyncJob job,
        final UpdateOperationCallbacks secondary,
        final Runnable onJobFinished
    ) {
        final UpdateOperationCallbacks callbacks = StatusWindow.createAndShowFrame(appendBundleName(job.getName() + " " + Launcher.CONSOLE_VERSION));
        final UpdateOperationCallbacks doubleCallbacks = new DoubleCallbacks(callbacks, secondary);
        executeJob(job, doubleCallbacks, onJobFinished);
    }

    public void executeJob(final AsyncJob job, final UpdateOperationCallbacks callbacks, final Runnable onJobFinished) {
        final Runnable jobWithSuspendedPortScanning = () -> job.doJob(callbacks, onJobFinished);
        ExecHelper.submitAction(jobWithSuspendedPortScanning, "mx-" + threadNameIndex.incrementAndGet());
    }
}
