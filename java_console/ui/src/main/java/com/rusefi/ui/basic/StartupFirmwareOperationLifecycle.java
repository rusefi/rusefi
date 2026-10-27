package com.rusefi.ui.basic;

import com.rusefi.maintenance.jobs.AsyncJob;
import com.rusefi.maintenance.jobs.ExportTuneJob;
import com.rusefi.maintenance.jobs.ImportTuneJob;

import javax.swing.SwingUtilities;
import java.util.concurrent.CountDownLatch;
import java.util.concurrent.atomic.AtomicInteger;
import java.util.concurrent.atomic.AtomicReference;
import java.util.function.Consumer;

/** Shared, headless-testable registration for the startup frame's firmware-job hooks. */
public final class StartupFirmwareOperationLifecycle {
    private StartupFirmwareOperationLifecycle() {
    }

    public static void registerAboutToStart(SingleAsyncJobExecutor executor, Consumer<Boolean> busyChanged,
                                            Consumer<AsyncJob> prepareSplash) {
        executor.addOnJobAboutToStartListener(() ->
            SwingUtilities.invokeLater(() -> busyChanged.accept(hasFirmwareOperationReservation(executor))));
        executor.addOnJobWorkerAboutToStartListener(() -> {
            AsyncJob job = executor.getJobInProgress()
                .orElseThrow(() -> new IllegalStateException("Startup job reservation was released before preparation."));
            runOnEdtAndWait(() -> prepareSplash.accept(job));
        });
    }

    public static void registerReleased(SingleAsyncJobExecutor executor, Consumer<Boolean> busyChanged) {
        executor.addOnJobReleasedListener(() -> SwingUtilities.invokeLater(
            () -> busyChanged.accept(hasFirmwareOperationReservation(executor))));
    }

    public static boolean isFirmwareOperationInProgress(SingleAsyncJobExecutor executor) {
        return executor.isUpdating() && hasFirmwareOperationReservation(executor);
    }

    private static boolean hasFirmwareOperationReservation(SingleAsyncJobExecutor executor) {
        return executor.getJobInProgress()
            .map(job -> !(job instanceof ImportTuneJob) && !(job instanceof ExportTuneJob))
            .orElse(false);
    }

    private static void runOnEdtAndWait(Runnable action) {
        if (SwingUtilities.isEventDispatchThread()) {
            action.run();
            return;
        }
        EdtAction edtAction = new EdtAction(action);
        SwingUtilities.invokeLater(edtAction);
        try {
            edtAction.awaitCompletion();
        } catch (InterruptedException e) {
            if (!edtAction.cancelIfQueued()) {
                edtAction.awaitCompletionUninterruptibly();
            }
            Thread.currentThread().interrupt();
            throw new IllegalStateException("Startup connection preparation interrupted", e);
        }
        if (Thread.interrupted()) {
            Thread.currentThread().interrupt();
            throw new IllegalStateException("Startup connection preparation interrupted");
        }
        edtAction.rethrowFailure();
    }

    /** An interrupted caller can cancel queued EDT work, but must await work that already claimed execution. */
    private static final class EdtAction implements Runnable {
        private static final int QUEUED = 0;
        private static final int STARTED = 1;
        private static final int CANCELLED = 2;

        private final Runnable action;
        private final AtomicInteger state = new AtomicInteger(QUEUED);
        private final CountDownLatch completed = new CountDownLatch(1);
        private final AtomicReference<Throwable> failure = new AtomicReference<>();

        private EdtAction(Runnable action) {
            this.action = action;
        }

        @Override
        public void run() {
            if (!state.compareAndSet(QUEUED, STARTED)) {
                completed.countDown();
                return;
            }
            try {
                action.run();
            } catch (Throwable e) {
                failure.set(e);
            } finally {
                completed.countDown();
            }
        }

        private void awaitCompletion() throws InterruptedException {
            completed.await();
        }

        private boolean cancelIfQueued() {
            if (state.compareAndSet(QUEUED, CANCELLED)) {
                completed.countDown();
                return true;
            }
            return false;
        }

        private void awaitCompletionUninterruptibly() {
            boolean interrupted = false;
            while (true) {
                try {
                    completed.await();
                    break;
                } catch (InterruptedException e) {
                    interrupted = true;
                }
            }
            if (interrupted) {
                Thread.currentThread().interrupt();
            }
        }

        private void rethrowFailure() {
            Throwable thrown = failure.get();
            if (thrown instanceof RuntimeException) {
                throw (RuntimeException) thrown;
            }
            if (thrown != null) {
                throw new IllegalStateException("Startup connection preparation failed", thrown);
            }
        }
    }
}
