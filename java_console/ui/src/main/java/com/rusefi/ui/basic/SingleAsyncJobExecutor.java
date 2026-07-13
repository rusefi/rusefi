package com.rusefi.ui.basic;

import com.rusefi.io.UpdateOperationCallbacks;
import com.rusefi.maintenance.jobs.AsyncJob;
import com.rusefi.maintenance.jobs.AsyncJobExecutor;

import javax.swing.*;
import java.awt.*;
import java.util.ArrayList;
import java.util.Optional;

public class SingleAsyncJobExecutor implements com.rusefi.DeviceSessionManager.JobExecutor {
    private final UpdateOperationCallbacks updateOperationCallbacks;

    private final java.util.List<Runnable> onJobInProgressFinished = new ArrayList<>();
    private final java.util.List<Runnable> onJobAboutToStart = new ArrayList<>();

    private volatile Optional<AsyncJob> jobInProgress = Optional.empty();

    /**
     * Terminal outcome of the last job, captured by wrapping the real callbacks — so the post-flash
     * reconnect can show a persistent success/failure indication that outlives the transient status
     * panel (issue #9832).
     */
    private volatile UpdateFirmwareResult lastResult = UpdateFirmwareResult.NONE;

    /** Delegating callbacks that record the terminal outcome before forwarding to the real UI. */
    private final UpdateOperationCallbacks recordingCallbacks;

    public SingleAsyncJobExecutor(
        final UpdateOperationCallbacks updateOperationCallbacks
    ) {
        this.updateOperationCallbacks = updateOperationCallbacks;
        this.recordingCallbacks = new UpdateOperationCallbacks() {
            @Override
            public void log(final String message, final boolean breakLineOnTextArea, final boolean sendToLogger) {
                updateOperationCallbacks.log(message, breakLineOnTextArea, sendToLogger);
            }

            @Override
            public void done() {
                lastResult = UpdateFirmwareResult.SUCCESS;
                updateOperationCallbacks.done();
            }

            @Override
            public void error() {
                lastResult = UpdateFirmwareResult.FAILURE;
                updateOperationCallbacks.error();
            }

            @Override
            public void warning() {
                updateOperationCallbacks.warning();
            }

            @Override
            public void clear() {
                lastResult = UpdateFirmwareResult.NONE;
                updateOperationCallbacks.clear();
            }

            @Override
            public void updateProgress(final int percent) {
                updateOperationCallbacks.updateProgress(percent);
            }
        };
    }

    /** Outcome of the most recent job (reset to {@link UpdateFirmwareResult#NONE} at each job start). */
    public UpdateFirmwareResult getLastResult() {
        return lastResult;
    }

    /**
     * Return the last outcome and reset it to {@link UpdateFirmwareResult#NONE}, so a one-shot consumer
     * (e.g. the post-flash reconnect handler) shows the success/failure exactly once and a later,
     * unrelated reconnect does not re-show a stale report (#9832).
     */
    public synchronized UpdateFirmwareResult consumeLastResult() {
        final UpdateFirmwareResult result = lastResult;
        lastResult = UpdateFirmwareResult.NONE;
        return result;
    }

    public void addOnJobInProgressFinishedListener(Runnable listener) {
        onJobInProgressFinished.add(listener);
    }

    /**
     * Runs before a new job is handed off to the executor. Use this to release resources the job
     * will need exclusive access to (e.g. close a live serial connection so the firmware updater
     * can open the same port).
     */
    public void addOnJobAboutToStartListener(Runnable listener) {
        onJobAboutToStart.add(listener);
    }

    public void startJob(final AsyncJob job, final Component parent) {
        final Optional<AsyncJob> prevJobInProgress = setJobInProgressIfEmpty(job);
        if (!prevJobInProgress.isPresent()) {
            for (Runnable listener : onJobAboutToStart)
                listener.run();
            recordingCallbacks.clear(); // resets lastResult to NONE and clears the status panel
            AsyncJobExecutor.INSTANCE.executeJob(job, recordingCallbacks, this::handleJobInProgressFinished);
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

    public synchronized Optional<AsyncJob> getJobInProgress() {
        return jobInProgress;
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
