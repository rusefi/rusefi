package com.rusefi.ui.basic;

import com.rusefi.io.UpdateOperationCallbacks;
import com.rusefi.maintenance.jobs.AsyncJob;
import com.rusefi.maintenance.jobs.AsyncJobExecutor;

import javax.swing.*;
import java.awt.*;
import java.util.ArrayList;
import java.util.Optional;
import java.util.function.Consumer;
import java.util.function.Function;

public class SingleAsyncJobExecutor implements com.rusefi.DeviceSessionManager.JobExecutor {
    private final Function<AsyncJob, UpdateOperationCallbacks> callbacksProvider;

    private final java.util.List<Runnable> onJobInProgressFinished = new ArrayList<>();
    private final java.util.List<Runnable> onJobAboutToStart = new ArrayList<>();

    private volatile Optional<AsyncJob> jobInProgress = Optional.empty();

    /**
     * Terminal outcome of the last job, captured by wrapping the real callbacks — so the post-flash
     * reconnect can show a persistent success/failure indication that outlives the transient status
     * panel (issue #9832).
     */
    private volatile UpdateFirmwareResult lastResult = UpdateFirmwareResult.NONE;

    public SingleAsyncJobExecutor(
        final UpdateOperationCallbacks updateOperationCallbacks
    ) {
        this(job -> updateOperationCallbacks);
    }

    public SingleAsyncJobExecutor(Function<AsyncJob, UpdateOperationCallbacks> callbacksProvider) {
        this.callbacksProvider = callbacksProvider;
    }

    /** Delegating callbacks that record the terminal outcome before forwarding to the selected UI. */
    private UpdateOperationCallbacks recordingCallbacks(UpdateOperationCallbacks delegate) {
        return new UpdateOperationCallbacks() {
            @Override
            public void log(final String message, final boolean breakLineOnTextArea, final boolean sendToLogger) {
                delegate.log(message, breakLineOnTextArea, sendToLogger);
            }

            @Override
            public void done() {
                lastResult = UpdateFirmwareResult.SUCCESS;
                delegate.done();
            }

            @Override
            public void error() {
                lastResult = UpdateFirmwareResult.FAILURE;
                delegate.error();
            }

            @Override
            public void warning() {
                delegate.warning();
            }

            @Override
            public void clear() {
                lastResult = UpdateFirmwareResult.NONE;
                delegate.clear();
            }

            @Override
            public void updateProgress(final int percent) {
                delegate.updateProgress(percent);
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

    public boolean startJob(final AsyncJob job, final Component parent) {
        return startJob(job, parent, message -> JOptionPane.showMessageDialog(
            parent,
            message,
            "Error",
            JOptionPane.ERROR_MESSAGE
        ), () -> {});
    }

    public boolean startJob(final AsyncJob job, final Component parent, Consumer<String> errorHandler) {
        return startJob(job, parent, errorHandler, () -> {});
    }

    public boolean startJob(final AsyncJob job, final Component parent, final Runnable onAccepted) {
        return startJob(job, parent, message -> JOptionPane.showMessageDialog(
            parent,
            message,
            "Error",
            JOptionPane.ERROR_MESSAGE
        ), onAccepted);
    }

    private boolean startJob(final AsyncJob job, final Component parent, Consumer<String> errorHandler,
                             final Runnable onAccepted) {
        final Optional<AsyncJob> prevJobInProgress = setJobInProgressIfEmpty(job);
        if (!prevJobInProgress.isPresent()) {
            onAccepted.run();
            for (Runnable listener : onJobAboutToStart) {
                listener.run();
            }
            UpdateOperationCallbacks callbacks = recordingCallbacks(callbacksProvider.apply(job));
            callbacks.clear(); // resets lastResult to NONE and clears the selected status panel
            AsyncJobExecutor.INSTANCE.executeJob(job, callbacks, this::handleJobInProgressFinished);
            return true;
        } else {
            errorHandler.accept(String.format("Job `%s` is already in progress!", prevJobInProgress.get().getName()));
            return false;
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
        for (Runnable listener : onJobInProgressFinished) {
            listener.run();
        }
    }
}
