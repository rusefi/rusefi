package com.rusefi.ui.basic;

import com.rusefi.io.UpdateOperationCallbacks;
import com.rusefi.maintenance.jobs.AsyncJob;
import com.rusefi.maintenance.jobs.AsyncJobExecutor;

import javax.swing.*;
import java.awt.*;
import java.util.ArrayList;
import java.util.Optional;

public class SingleAsyncJobExecutor {
    private final UpdateOperationCallbacks updateOperationCallbacks;

    private final java.util.List<Runnable> onJobInProgressFinished = new ArrayList<>();

    private volatile Optional<AsyncJob> jobInProgress = Optional.empty();

    SingleAsyncJobExecutor(
        final UpdateOperationCallbacks updateOperationCallbacks
    ) {
        this.updateOperationCallbacks = updateOperationCallbacks;
    }

    public void addOnJobInProgressFinishedListener(Runnable listener) {
        onJobInProgressFinished.add(listener);
    }

    public void startJob(final AsyncJob job, final Component parent) {
        final Optional<AsyncJob> prevJobInProgress = setJobInProgressIfEmpty(job);
        if (!prevJobInProgress.isPresent()) {
            updateOperationCallbacks.clear();
            AsyncJobExecutor.INSTANCE.executeJob(job, updateOperationCallbacks, this::handleJobInProgressFinished);
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
        for (Runnable listener : onJobInProgressFinished)
            listener.run();
    }
}
