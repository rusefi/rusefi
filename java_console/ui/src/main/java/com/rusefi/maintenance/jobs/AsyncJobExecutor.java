package com.rusefi.maintenance.jobs;

import com.rusefi.Launcher;
import com.rusefi.io.UpdateOperationCallbacks;
import com.rusefi.maintenance.ExecHelper;
import com.rusefi.ui.StatusWindow;

import javax.swing.*;
import java.util.ArrayList;
import java.util.List;
import java.util.concurrent.atomic.AtomicInteger;
import java.util.concurrent.atomic.AtomicReference;
import java.util.function.Consumer;
import java.util.function.Supplier;

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
        executeJobWithStatusWindow(job, secondary, onJobFinished, () -> StatusWindow.createAndShowFrame(
            appendBundleName(job.getName() + " " + Launcher.CONSOLE_VERSION)));
    }

    void executeJobWithStatusWindow(AsyncJob job, UpdateOperationCallbacks secondary, Runnable onJobFinished,
                                    Supplier<UpdateOperationCallbacks> createWindow) {
        final DeferredStatusCallbacks callbacks = new DeferredStatusCallbacks(createWindow, secondary);
        executeJob(job, callbacks, callbacks::showWindow, onJobFinished);
    }

    public void executeJob(final AsyncJob job, final UpdateOperationCallbacks callbacks, final Runnable onJobFinished) {
        executeJob(job, callbacks, () -> {}, onJobFinished);
    }

    public void executeJob(final AsyncJob job, final UpdateOperationCallbacks callbacks,
                           final Runnable onJobAboutToStart, final Runnable onJobFinished) {
        final Completion completion = new Completion(onJobFinished);
        final Runnable jobWithPreparation = () -> {
            try {
                if (!job.prepare(callbacks)) {
                    completion.request();
                    return;
                }
                onJobAboutToStart.run();
                job.doJob(callbacks, completion::request);
            } catch (RuntimeException e) {
                try {
                    callbacks.logLine("Job could not start: " + e.getMessage());
                    callbacks.error();
                } finally {
                    completion.request();
                }
            } finally {
                // JobHelper may request completion in finally before propagating a synchronous error.
                // Publish that error first; asynchronous completion after return still delivers directly.
                completion.invocationReturned();
            }
        };
        ExecHelper.submitAction(jobWithPreparation, "mx-" + threadNameIndex.incrementAndGet());
    }

    private static final class Completion {
        private final Runnable finished;
        private boolean invoking = true;
        private boolean requested;
        private boolean delivered;

        Completion(Runnable finished) {
            this.finished = finished;
        }

        void request() {
            boolean deliver;
            synchronized (this) {
                requested = true;
                deliver = claimDelivery();
            }
            if (deliver) {
                finished.run();
            }
        }

        void invocationReturned() {
            boolean deliver;
            synchronized (this) {
                invoking = false;
                deliver = claimDelivery();
            }
            if (deliver) {
                finished.run();
            }
        }

        private boolean claimDelivery() {
            if (invoking || !requested || delivered) {
                return false;
            }
            delivered = true;
            return true;
        }
    }

    /** Preflight logging is retained without creating a window over a board picker. Actual errors remain visible. */
    private static final class DeferredStatusCallbacks implements UpdateOperationCallbacks {
        private final Supplier<UpdateOperationCallbacks> createWindow;
        private final UpdateOperationCallbacks secondary;
        private final List<Consumer<UpdateOperationCallbacks>> pending = new ArrayList<>();
        private UpdateOperationCallbacks window;

        DeferredStatusCallbacks(Supplier<UpdateOperationCallbacks> createWindow, UpdateOperationCallbacks secondary) {
            this.createWindow = createWindow;
            this.secondary = secondary;
        }

        synchronized void showWindow() {
            if (window != null) {
                return;
            }
            AtomicReference<UpdateOperationCallbacks> created = new AtomicReference<>();
            Runnable show = () -> created.set(createWindow.get());
            try {
                if (SwingUtilities.isEventDispatchThread()) {
                    show.run();
                } else {
                    SwingUtilities.invokeAndWait(show);
                }
            } catch (InterruptedException e) {
                Thread.currentThread().interrupt();
                throw new IllegalStateException("Status window interrupted", e);
            } catch (java.lang.reflect.InvocationTargetException e) {
                throw new IllegalStateException("Status window unavailable", e.getCause());
            }
            window = created.get();
            pending.forEach(event -> event.accept(window));
            pending.clear();
        }

        private synchronized void emit(Consumer<UpdateOperationCallbacks> event) {
            event.accept(secondary);
            if (window == null) {
                pending.add(event);
            } else {
                event.accept(window);
            }
        }

        @Override
        public void log(String message, boolean breakLine, boolean logger) {
            emit(callbacks -> callbacks.log(message, breakLine, logger));
        }

        @Override
        public void done() { emit(UpdateOperationCallbacks::done); }

        @Override
        public void warning() { emit(UpdateOperationCallbacks::warning); }

        @Override
        public void error() {
            emit(UpdateOperationCallbacks::error);
            showWindow();
        }

        @Override
        public void clear() { emit(UpdateOperationCallbacks::clear); }

        @Override
        public void updateProgress(int percent) { emit(callbacks -> callbacks.updateProgress(percent)); }

        @Override
        public void firmwareUpdateBlocked(String reason) { emit(callbacks -> callbacks.firmwareUpdateBlocked(reason)); }

        @Override
        public void firmwareHandoffStarted() { emit(UpdateOperationCallbacks::firmwareHandoffStarted); }
    }
}
