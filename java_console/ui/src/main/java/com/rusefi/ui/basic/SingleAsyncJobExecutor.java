package com.rusefi.ui.basic;

import com.rusefi.io.UpdateOperationCallbacks;
import com.rusefi.maintenance.jobs.AsyncJob;
import com.rusefi.maintenance.jobs.AsyncJobExecutor;

import javax.swing.*;
import java.awt.*;
import java.util.Optional;

public class SingleAsyncJobExecutor {
    private final UpdateOperationCallbacks updateOperationCallbacks;

    private final Runnable onJobInProgressFinished;

    private volatile Optional<AsyncJob> jobInProgress = Optional.empty();

    SingleAsyncJobExecutor(
        final UpdateOperationCallbacks updateOperationCallbacks,
        final Runnable onJobInProgressFinished
    ) {
        this.updateOperationCallbacks = updateOperationCallbacks;
        this.onJobInProgressFinished = onJobInProgressFinished;
    }

    void startJob(final AsyncJob job, final Component parent) {
        final Optional<AsyncJob> prevJobInProgress = setJobInProgressIfEmpty(job);
        if (!prevJobInProgress.isPresent()) {
            updateOperationCallbacks.clear();
            AsyncJobExecutor.INSTANCE.executeJobWithStatusWindow(
                job,
                updateOperationCallbacks,
                this::handleJobInProgressFinished
            );
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
        onJobInProgressFinished.run();
    }
}
