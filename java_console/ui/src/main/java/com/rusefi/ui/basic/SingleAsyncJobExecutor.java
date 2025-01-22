package com.rusefi.ui.basic;

import com.rusefi.io.UpdateOperationCallbacks;
import com.rusefi.maintenance.jobs.AsyncJob;
import com.rusefi.maintenance.jobs.AsyncJobExecutor;

import javax.swing.*;
import java.awt.*;
import java.util.Optional;

public class SingleAsyncJobExecutor {
    private final UpdateOperationCallbacks updateOperationCallbacks;

    // Temporary feature flag for testing #7199
    // TODO: get rid of this feature flag after #7199 is completed
    private final boolean doNotUseStatusWindow;

    private volatile Optional<AsyncJob> jobInProgress = Optional.empty();

    SingleAsyncJobExecutor(
        final UpdateOperationCallbacks updateOperationCallbacks,
        final boolean doNotUseStatusWindow,
        final Runnable onJobInProgressFinished
    ) {
        this.updateOperationCallbacks = updateOperationCallbacks;
        this.doNotUseStatusWindow = doNotUseStatusWindow;
    }

    void startJob(final AsyncJob job, final Component parent) {
        final Optional<AsyncJob> prevJobInProgress = setJobInProgressIfEmpty(job);
        if (!prevJobInProgress.isPresent()) {
            updateOperationCallbacks.clear();
            if (doNotUseStatusWindow) {
                AsyncJobExecutor.INSTANCE.executeJob(job, updateOperationCallbacks, this::handleJobInProgressFinished);
            } else {
                AsyncJobExecutor.INSTANCE.executeJobWithStatusWindow(
                    job,
                    updateOperationCallbacks,
                    this::handleJobInProgressFinished
                );
            }
        } else {
            JOptionPane.showMessageDialog(
                parent,
                String.format("Job `%s` is already in progress!", prevJobInProgress.get().getName()),
                "Error",
                JOptionPane.ERROR_MESSAGE
            );
        }
    }

    public synchronized boolean isNotInProgress() {
        return !jobInProgress.isPresent();
    }

    private synchronized Optional<AsyncJob> setJobInProgressIfEmpty(final AsyncJob job) {
        final Optional<AsyncJob> prevJobInProgress = jobInProgress;
        if (!prevJobInProgress.isPresent()) {
            jobInProgress = Optional.of(job);
        }
        return prevJobInProgress;
    }

    private synchronized void resetJobInProgress() {
        jobInProgress = Optional.empty();
    }

    private void handleJobInProgressFinished() {
        resetJobInProgress();
    }
}
