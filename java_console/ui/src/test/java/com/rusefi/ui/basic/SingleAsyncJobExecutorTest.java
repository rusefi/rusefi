package com.rusefi.ui.basic;

import com.rusefi.io.UpdateOperationCallbacks;
import com.rusefi.maintenance.jobs.AsyncJob;
import com.rusefi.AvailableHardware;
import com.rusefi.ConnectivityContext;
import com.rusefi.DeviceSessionManager;
import com.rusefi.FakePortScanner;
import com.rusefi.SessionState;
import com.rusefi.io.ConnectionStatusLogic;
import com.rusefi.io.ConnectionStatusValue;
import org.junit.jupiter.api.Test;

import java.util.concurrent.CountDownLatch;
import java.util.concurrent.TimeUnit;
import java.util.Collections;
import java.util.concurrent.atomic.AtomicInteger;

import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertTrue;
import static org.junit.jupiter.api.Assertions.assertFalse;
import static org.mockito.Mockito.*;

class SingleAsyncJobExecutorTest {
    /** Preserve the specialized event so the UI can prominently display the rejection. */
    @Test
    void blockedUpdateReasonReachesUiCallback() throws Exception {
        UpdateOperationCallbacks ui = mock(UpdateOperationCallbacks.class);
        SingleAsyncJobExecutor executor = new SingleAsyncJobExecutor(ui);
        CountDownLatch released = new CountDownLatch(1);
        executor.addOnJobReleasedListener(released::countDown);
        String reason = "Please install an intermediate firmware version";

        assertTrue(executor.startJob(new AsyncJob("blocked firmware") {
            @Override
            public void doJob(UpdateOperationCallbacks callbacks, Runnable onJobFinished) {
                callbacks.firmwareUpdateBlocked(reason);
                callbacks.error();
                onJobFinished.run();
            }
        }, null));
        assertTrue(released.await(5, TimeUnit.SECONDS));

        verify(ui).firmwareUpdateBlocked(reason);
        verify(ui, never()).log(reason, true, true);
        verify(ui).error();
        assertEquals(UpdateFirmwareResult.FAILURE, executor.getLastResult());
        assertTrue(executor.isNotInProgress());
    }

    @Test
    void unrecordedJobDoesNotOverwriteRecordedResult() throws Exception {
        final SingleAsyncJobExecutor executor = new SingleAsyncJobExecutor(
            job -> UpdateOperationCallbacks.DUMMY,
            job -> "firmware".equals(job.getName()));

        CountDownLatch firmwareFinished = new CountDownLatch(1);
        CountDownLatch firmwareReleased = new CountDownLatch(1);
        executor.addOnJobInProgressFinishedListener(firmwareFinished::countDown);
        executor.addOnJobReleasedListener(firmwareReleased::countDown);
        assertTrue(executor.startJob(completedJob("firmware", true), null));
        assertTrue(firmwareFinished.await(5, TimeUnit.SECONDS));
        assertTrue(firmwareReleased.await(5, TimeUnit.SECONDS));
        assertEquals(UpdateFirmwareResult.SUCCESS, executor.getLastResult());

        CountDownLatch tuneFinished = new CountDownLatch(1);
        CountDownLatch tuneReleased = new CountDownLatch(1);
        executor.addOnJobInProgressFinishedListener(tuneFinished::countDown);
        executor.addOnJobReleasedListener(tuneReleased::countDown);
        assertTrue(executor.startJob(completedJob("tune", false), null));
        assertTrue(tuneFinished.await(5, TimeUnit.SECONDS));
        assertTrue(tuneReleased.await(5, TimeUnit.SECONDS));
        assertEquals(UpdateFirmwareResult.SUCCESS, executor.getLastResult());
    }

    @Test
    void preparingRetainsExclusiveSlotButOnlyAcceptedPreflightSuspendsAndUpdates() throws Exception {
        ConnectionStatusValue previous = ConnectionStatusLogic.INSTANCE.getValue();
        ConnectionStatusLogic.INSTANCE.setValue(ConnectionStatusValue.NOT_CONNECTED);
        try {
            for (boolean accepted : new boolean[]{false, true}) {
                FakePortScanner scanner = new FakePortScanner();
                scanner.fireHardwareChange(new AvailableHardware(Collections.emptyList(), true, false, false));
                AtomicInteger paused = new AtomicInteger();
                AtomicInteger resumed = new AtomicInteger();
                DeviceSessionManager session = new DeviceSessionManager(new ConnectivityContext(scanner), null,
                    paused::incrementAndGet, resumed::incrementAndGet);
                SingleAsyncJobExecutor executor = new SingleAsyncJobExecutor(UpdateOperationCallbacks.DUMMY);
                session.setJobExecutor(executor);
                CountDownLatch preparing = new CountDownLatch(1);
                CountDownLatch consent = new CountDownLatch(1);
                CountDownLatch active = new CountDownLatch(1);
                CountDownLatch releaseActive = new CountDownLatch(1);
                CountDownLatch finished = new CountDownLatch(1);
                CountDownLatch released = new CountDownLatch(1);
                AtomicInteger completions = new AtomicInteger();
                executor.addOnJobInProgressFinishedListener(() -> {
                    completions.incrementAndGet();
                    finished.countDown();
                });
                executor.addOnJobReleasedListener(released::countDown);
                AsyncJob job = new AsyncJob("recovery preflight") {
                    @Override
                    public boolean prepare(UpdateOperationCallbacks callbacks) {
                        preparing.countDown();
                        try {
                            return consent.await(5, TimeUnit.SECONDS) && accepted;
                        } catch (InterruptedException e) {
                            Thread.currentThread().interrupt();
                            return false;
                        }
                    }

                    @Override
                    public void doJob(UpdateOperationCallbacks callbacks, Runnable complete) {
                        active.countDown();
                        try {
                            releaseActive.await(5, TimeUnit.SECONDS);
                        } catch (InterruptedException e) {
                            Thread.currentThread().interrupt();
                        } finally {
                            complete.run();
                            complete.run();
                        }
                    }
                };
                assertTrue(executor.startJob(job, null));
                try {
                    assertTrue(preparing.await(5, TimeUnit.SECONDS));
                    assertFalse(executor.isNotInProgress());
                    assertFalse(executor.isUpdating());
                    assertEquals(SessionState.DEVICE_IN_DFU, session.getState());
                    assertEquals(0, scanner.suspendCount);
                    assertEquals(1, paused.get());
                    consent.countDown();
                    if (accepted) {
                        assertTrue(active.await(5, TimeUnit.SECONDS));
                        assertTrue(executor.isUpdating());
                        assertEquals(SessionState.FLASHING, session.getState());
                        assertEquals(1, scanner.suspendCount);
                    }
                } finally {
                    consent.countDown();
                    releaseActive.countDown();
                }
                assertTrue(finished.await(5, TimeUnit.SECONDS));
                assertTrue(released.await(5, TimeUnit.SECONDS));
                assertEquals(1, completions.get());
                assertEquals(accepted ? 1 : 0, scanner.resumeCount);
                assertEquals(1, resumed.get());
                assertTrue(executor.isNotInProgress());
                assertFalse(executor.isUpdating());
                assertEquals(UpdateFirmwareResult.NONE, executor.getLastResult());
            }
        } finally {
            ConnectionStatusLogic.INSTANCE.setValue(previous);
        }
    }

    @Test
    void preflightExceptionReportsFailureAndReleasesWithoutActiveHooks() throws Exception {
        UpdateOperationCallbacks callbacks = mock(UpdateOperationCallbacks.class);
        SingleAsyncJobExecutor executor = new SingleAsyncJobExecutor(callbacks);
        AtomicInteger active = new AtomicInteger();
        CountDownLatch released = new CountDownLatch(1);
        executor.addOnJobWorkerAboutToStartListener(active::incrementAndGet);
        executor.addOnJobReleasedListener(released::countDown);
        assertTrue(executor.startJob(new AsyncJob("invalid preparation") {
            @Override
            public boolean prepare(UpdateOperationCallbacks ignored) {
                throw new IllegalStateException("preflight unavailable");
            }

            @Override
            public void doJob(UpdateOperationCallbacks ignored, Runnable complete) {
                throw new AssertionError("rejected job must not run");
            }
        }, null));
        assertTrue(released.await(5, TimeUnit.SECONDS));
        assertEquals(0, active.get());
        assertEquals(UpdateFirmwareResult.FAILURE, executor.getLastResult());
        assertTrue(executor.isNotInProgress());
        assertFalse(executor.isUpdating());
        verify(callbacks).error();
        verify(callbacks, never()).done();
    }

    private static AsyncJob completedJob(String name, boolean success) {
        return new AsyncJob(name) {
            @Override
            public void doJob(UpdateOperationCallbacks callbacks, Runnable onJobFinished) {
                if (success) {
                    callbacks.done();
                } else {
                    callbacks.error();
                }
                onJobFinished.run();
            }
        };
    }

    @Test
    void laterCompletionDoesNotHideSettingsWarnings() throws Exception {
        UpdateOperationCallbacks ui = mock(UpdateOperationCallbacks.class);
        SingleAsyncJobExecutor executor = new SingleAsyncJobExecutor(ui);
        CountDownLatch released = new CountDownLatch(1);
        executor.addOnJobReleasedListener(released::countDown);

        assertTrue(executor.startJob(new AsyncJob("firmware with settings defaults") {
            @Override
            public void doJob(UpdateOperationCallbacks callbacks, Runnable onJobFinished) {
                callbacks.warning();
                callbacks.done();
                onJobFinished.run();
            }
        }, null));
        assertTrue(released.await(5, TimeUnit.SECONDS));

        assertEquals(UpdateFirmwareResult.WARNING, executor.getLastResult());
        verify(ui, never()).done();
        verify(ui, times(2)).warning();
        assertEquals(UpdateFirmwareResult.WARNING, executor.consumeLastResult());
        assertEquals(UpdateFirmwareResult.NONE, executor.getLastResult());
    }

    @Test
    void warningsAndLaterCompletionDoNotHideFailure() throws Exception {
        UpdateOperationCallbacks ui = mock(UpdateOperationCallbacks.class);
        SingleAsyncJobExecutor executor = new SingleAsyncJobExecutor(ui);
        CountDownLatch finished = new CountDownLatch(1);
        executor.addOnJobInProgressFinishedListener(finished::countDown);

        assertTrue(executor.startJob(new AsyncJob("failed firmware settings") {
            @Override
            public void doJob(UpdateOperationCallbacks callbacks, Runnable onJobFinished) {
                callbacks.error();
                callbacks.warning();
                callbacks.done();
                onJobFinished.run();
            }
        }, null));
        assertTrue(finished.await(5, TimeUnit.SECONDS));

        assertEquals(UpdateFirmwareResult.FAILURE, executor.getLastResult());
        verify(ui, never()).done();
    }

    /**
     * A warning after an error must not repaint the status panel yellow, even when there is no
     * later done() callback. Retains the error -> warning -> finish characterization stimulus.
     */
    @Test
    void warningAfterErrorWithoutLaterDoneKeepsFailureVisible() throws Exception {
        UpdateOperationCallbacks ui = mock(UpdateOperationCallbacks.class);
        SingleAsyncJobExecutor executor = new SingleAsyncJobExecutor(ui);
        CountDownLatch released = new CountDownLatch(1);
        executor.addOnJobReleasedListener(released::countDown);

        assertTrue(executor.startJob(new AsyncJob("failure followed by warning") {
            @Override
            public void doJob(UpdateOperationCallbacks callbacks, Runnable onJobFinished) {
                callbacks.error();
                callbacks.warning();
                onJobFinished.run();
            }
        }, null));
        assertTrue(released.await(5, TimeUnit.SECONDS));

        assertEquals(UpdateFirmwareResult.FAILURE, executor.getLastResult());
        verify(ui).error();
        verify(ui, never()).warning();
        verify(ui, never()).done();
        assertTrue(executor.isNotInProgress());
    }

    @Test
    void blockingPreparationDoesNotBlockCallerAndFinishesBeforeJobStarts() throws Exception {
        final SingleAsyncJobExecutor executor = new SingleAsyncJobExecutor(UpdateOperationCallbacks.DUMMY);
        final CountDownLatch preparationStarted = new CountDownLatch(1);
        final CountDownLatch releasePreparation = new CountDownLatch(1);
        final CountDownLatch callerReturned = new CountDownLatch(1);
        final CountDownLatch jobStarted = new CountDownLatch(1);

        executor.addOnJobWorkerAboutToStartListener(() -> {
            preparationStarted.countDown();
            try {
                releasePreparation.await();
            } catch (InterruptedException e) {
                Thread.currentThread().interrupt();
            }
        });

        final AsyncJob job = new AsyncJob("test") {
            @Override
            public void doJob(final UpdateOperationCallbacks callbacks, final Runnable onJobFinished) {
                jobStarted.countDown();
                onJobFinished.run();
            }
        };

        final Thread caller = new Thread(() -> {
            executor.startJob(job, null);
            callerReturned.countDown();
        });
        caller.setDaemon(true);
        caller.start();

        assertTrue(preparationStarted.await(5, TimeUnit.SECONDS));
        final boolean returnedBeforePreparationFinished = callerReturned.await(1, TimeUnit.SECONDS);
        assertEquals(1, jobStarted.getCount());
        releasePreparation.countDown();

        assertTrue(returnedBeforePreparationFinished, "startJob blocked its caller during preparation");
        assertTrue(jobStarted.await(5, TimeUnit.SECONDS));
        caller.join(5000);
    }
}
