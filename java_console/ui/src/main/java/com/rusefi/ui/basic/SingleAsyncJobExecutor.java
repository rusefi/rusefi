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

    private static final UpdateOperationCallbacks BACKGROUND_CALLBACKS = new UpdateOperationCallbacks() {
        @Override
        public void log(final String message, final boolean breakLineOnTextArea, final boolean sendToLogger) {
            log.info(message);
        }

        @Override
        public void done() {
            log.info("Background job is done");
        }

        @Override
        public void error() {
            log.info("Background job failed");
        }

        @Override
        public void clear() {
        }
    };

    SingleAsyncJobExecutor(
        final UpdateOperationCallbacks updateOperationCallbacks,
        final Runnable onJobInProgressFinished
    ) {
        this.updateOperationCallbacks = updateOperationCallbacks;
        this.onJobInProgressFinished = onJobInProgressFinished;
    }

    void startJob(final AsyncJob job, final Component parent) {
        startJob(job, parent, updateOperationCallbacks);
    }

    void startBackgroundJob(final AsyncJob job, final Component parent) {
        startJob(job, parent, BACKGROUND_CALLBACKS);
    }

    void startJob(final AsyncJob job, final Component parent, final UpdateOperationCallbacks callbacksToUse) {
        final Optional<AsyncJob> prevJobInProgress = setJobInProgressIfEmpty(job);
        if (!prevJobInProgress.isPresent()) {
            updateOperationCallbacks.clear();
            AsyncJobExecutor.INSTANCE.executeJob(job, callbacksToUse, this::handleJobInProgressFinished);
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
