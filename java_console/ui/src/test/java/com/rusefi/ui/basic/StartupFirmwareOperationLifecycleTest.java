package com.rusefi.ui.basic;

import com.rusefi.io.UpdateOperationCallbacks;
import com.rusefi.maintenance.jobs.AsyncJob;
import org.junit.jupiter.api.Test;

import javax.swing.SwingUtilities;
import java.util.concurrent.CountDownLatch;
import java.util.concurrent.TimeUnit;
import java.util.concurrent.atomic.AtomicBoolean;
import java.util.concurrent.atomic.AtomicInteger;
import java.util.concurrent.atomic.AtomicReference;
import java.util.concurrent.locks.LockSupport;

import static org.junit.jupiter.api.Assertions.*;
import static org.mockito.Mockito.*;

class StartupFirmwareOperationLifecycleTest {
    @Test
    void startupKeepsReservationBusyButWaitsForConsentBeforeSplashTeardown() throws Exception {
        SingleAsyncJobExecutor executor = new SingleAsyncJobExecutor(UpdateOperationCallbacks.DUMMY);
        AtomicInteger splashTeardowns = new AtomicInteger();
        AtomicBoolean splashTeardownOnEdt = new AtomicBoolean();
        AtomicBoolean splashReleasedBeforeJob = new AtomicBoolean();
        AtomicBoolean startupBusy = new AtomicBoolean();
        CountDownLatch busyStarted = new CountDownLatch(1);
        CountDownLatch busyReleased = new CountDownLatch(1);
        CountDownLatch preparing = new CountDownLatch(1);
        CountDownLatch consent = new CountDownLatch(1);
        CountDownLatch active = new CountDownLatch(1);
        CountDownLatch releaseActive = new CountDownLatch(1);
        CountDownLatch cleanupEntered = new CountDownLatch(1);
        CountDownLatch releaseCleanup = new CountDownLatch(1);
        CountDownLatch released = new CountDownLatch(1);

        StartupFirmwareOperationLifecycle.registerAboutToStart(executor, inProgress -> {
            startupBusy.set(inProgress);
            (inProgress ? busyStarted : busyReleased).countDown();
        }, ignored -> {
            splashTeardowns.incrementAndGet();
            splashTeardownOnEdt.set(SwingUtilities.isEventDispatchThread());
        });
        StartupFirmwareOperationLifecycle.registerReleased(executor, inProgress -> {
            startupBusy.set(inProgress);
            (inProgress ? busyStarted : busyReleased).countDown();
        });
        executor.addOnJobReleasedListener(released::countDown);
        executor.addOnJobInProgressFinishedListener(() -> {
            cleanupEntered.countDown();
            try {
                releaseCleanup.await(5, TimeUnit.SECONDS);
            } catch (InterruptedException e) {
                Thread.currentThread().interrupt();
            }
        });

        AsyncJob recovery = new AsyncJob("recovery awaiting board consent") {
            @Override
            public boolean prepare(UpdateOperationCallbacks callbacks) {
                preparing.countDown();
                try {
                    return consent.await(5, TimeUnit.SECONDS);
                } catch (InterruptedException e) {
                    Thread.currentThread().interrupt();
                    return false;
                }
            }

            @Override
            public void doJob(UpdateOperationCallbacks callbacks, Runnable complete) {
                splashReleasedBeforeJob.set(splashTeardowns.get() == 1);
                active.countDown();
                try {
                    releaseActive.await(5, TimeUnit.SECONDS);
                } catch (InterruptedException e) {
                    Thread.currentThread().interrupt();
                } finally {
                    callbacks.done();
                    complete.run();
                }
            }
        };

        try {
            assertTrue(executor.startJob(recovery, null));
            assertTrue(preparing.await(5, TimeUnit.SECONDS));
            assertTrue(busyStarted.await(5, TimeUnit.SECONDS));
            assertTrue(startupBusy.get(), "startup navigation remains gated by the reservation");
            assertFalse(StartupFirmwareOperationLifecycle.isFirmwareOperationInProgress(executor),
                "the close warning must distinguish board-consent preflight from active programming");
            assertFalse(executor.isUpdating(), "board consent is still pending; active programming has not begun");
            assertEquals(0, splashTeardowns.get(), "splash connection remains intact until consent is accepted");

            consent.countDown();
            assertTrue(active.await(5, TimeUnit.SECONDS));
            assertEquals(1, splashTeardowns.get());
            assertTrue(splashTeardownOnEdt.get(), "startup connection teardown is marshalled to the EDT");
            assertTrue(splashReleasedBeforeJob.get(), "teardown completes before active job work begins");
            assertTrue(StartupFirmwareOperationLifecycle.isFirmwareOperationInProgress(executor));
            assertTrue(executor.isUpdating());
        } finally {
            consent.countDown();
            releaseActive.countDown();
        }

        try {
            assertTrue(cleanupEntered.await(5, TimeUnit.SECONDS));
            assertFalse(executor.isNotInProgress(), "cleanup still owns the reservation");
            assertTrue(startupBusy.get(), "startup controls remain gated until cleanup and release finish");
            assertFalse(StartupFirmwareOperationLifecycle.isFirmwareOperationInProgress(executor),
                "the close warning ends with active work, not cleanup ownership");
        } finally {
            releaseCleanup.countDown();
        }
        assertTrue(released.await(5, TimeUnit.SECONDS));
        assertTrue(busyReleased.await(5, TimeUnit.SECONDS));
        assertFalse(startupBusy.get());
    }

    @Test
    void cancelledPreparationKeepsSplashAndClearsBusyStateAfterRelease() throws Exception {
        SingleAsyncJobExecutor executor = new SingleAsyncJobExecutor(UpdateOperationCallbacks.DUMMY);
        AtomicInteger splashTeardowns = new AtomicInteger();
        AtomicBoolean startupBusy = new AtomicBoolean();
        CountDownLatch busyStarted = new CountDownLatch(1);
        CountDownLatch busyReleased = new CountDownLatch(1);
        CountDownLatch preparing = new CountDownLatch(1);
        CountDownLatch cancel = new CountDownLatch(1);
        CountDownLatch released = new CountDownLatch(1);
        StartupFirmwareOperationLifecycle.registerAboutToStart(executor, inProgress -> {
            startupBusy.set(inProgress);
            (inProgress ? busyStarted : busyReleased).countDown();
        }, ignored -> splashTeardowns.incrementAndGet());
        StartupFirmwareOperationLifecycle.registerReleased(executor, inProgress -> {
            startupBusy.set(inProgress);
            (inProgress ? busyStarted : busyReleased).countDown();
        });
        executor.addOnJobReleasedListener(released::countDown);

        AsyncJob recovery = new AsyncJob("cancelled recovery preparation") {
            @Override
            public boolean prepare(UpdateOperationCallbacks callbacks) {
                preparing.countDown();
                try {
                    cancel.await(5, TimeUnit.SECONDS);
                    return false;
                } catch (InterruptedException e) {
                    Thread.currentThread().interrupt();
                    return false;
                }
            }

            @Override
            public void doJob(UpdateOperationCallbacks callbacks, Runnable complete) {
                fail("cancelled preparation must not enter active programming");
            }
        };

        try {
            assertTrue(executor.startJob(recovery, null));
            assertTrue(preparing.await(5, TimeUnit.SECONDS));
            assertTrue(busyStarted.await(5, TimeUnit.SECONDS));
            assertTrue(startupBusy.get(), "reservation continues to gate startup controls during consent");
            assertFalse(executor.isUpdating());
            assertFalse(StartupFirmwareOperationLifecycle.isFirmwareOperationInProgress(executor));
            assertEquals(0, splashTeardowns.get());
        } finally {
            cancel.countDown();
        }

        assertTrue(released.await(5, TimeUnit.SECONDS));
        assertTrue(busyReleased.await(5, TimeUnit.SECONDS));
        assertFalse(startupBusy.get());
        assertEquals(0, splashTeardowns.get(), "cancelled recovery leaves the splash connection intact");
        assertFalse(StartupFirmwareOperationLifecycle.isFirmwareOperationInProgress(executor));
    }

    @Test
    void failedStartupConnectionPreparationAbortsBeforeJobAndReleasesReservation() throws Exception {
        for (boolean interruptWorker : new boolean[]{false, true}) {
            UpdateOperationCallbacks callbacks = mock(UpdateOperationCallbacks.class);
            SingleAsyncJobExecutor executor = new SingleAsyncJobExecutor(callbacks);
            CountDownLatch released = new CountDownLatch(1);
            AtomicInteger jobStarted = new AtomicInteger();
            StartupFirmwareOperationLifecycle.registerAboutToStart(executor, ignored -> {}, ignored -> {
                if (!interruptWorker) {
                    throw new IllegalStateException("startup connection teardown failed");
                }
            });
            executor.addOnJobReleasedListener(released::countDown);

            AsyncJob recovery = new AsyncJob("startup preparation failure") {
                @Override
                public boolean prepare(UpdateOperationCallbacks ignored) {
                    if (interruptWorker) {
                        Thread.currentThread().interrupt();
                    }
                    return true;
                }

                @Override
                public void doJob(UpdateOperationCallbacks ignored, Runnable complete) {
                    jobStarted.incrementAndGet();
                    complete.run();
                }
            };

            assertTrue(executor.startJob(recovery, null));
            assertTrue(released.await(5, TimeUnit.SECONDS));
            assertEquals(0, jobStarted.get(), "failed EDT preparation must not enter active programming");
            assertTrue(executor.isNotInProgress(), "failed preparation still releases the reservation");
            assertEquals(UpdateFirmwareResult.FAILURE, executor.getLastResult());
            verify(callbacks).error();
        }
    }

    @Test
    void interruptedQueuedSplashTeardownIsCancelledBeforeTheNextReservation() throws Exception {
        SingleAsyncJobExecutor executor = new SingleAsyncJobExecutor(UpdateOperationCallbacks.DUMMY);
        CountDownLatch edtEntered = new CountDownLatch(1);
        CountDownLatch releaseEdt = new CountDownLatch(1);
        CountDownLatch aPreparing = new CountDownLatch(1);
        CountDownLatch aReleased = new CountDownLatch(1);
        CountDownLatch bPreparing = new CountDownLatch(1);
        CountDownLatch releaseBPreparation = new CountDownLatch(1);
        CountDownLatch bReleased = new CountDownLatch(1);
        AtomicReference<Thread> aWorker = new AtomicReference<>();
        AtomicReference<AsyncJob> ownerAtATeardown = new AtomicReference<>();
        AtomicInteger aTeardowns = new AtomicInteger();
        AtomicInteger releaseCount = new AtomicInteger();

        StartupFirmwareOperationLifecycle.registerAboutToStart(executor, ignored -> {}, ignored -> {
            aTeardowns.incrementAndGet();
            ownerAtATeardown.set(executor.getJobInProgress().orElse(null));
        });
        StartupFirmwareOperationLifecycle.registerReleased(executor, ignored -> {});
        executor.addOnJobReleasedListener(() -> {
            if (releaseCount.incrementAndGet() == 1) {
                aReleased.countDown();
            } else {
                bReleased.countDown();
            }
        });

        AsyncJob a = new AsyncJob("interrupted startup job A") {
            @Override
            public boolean prepare(UpdateOperationCallbacks callbacks) {
                aWorker.set(Thread.currentThread());
                aPreparing.countDown();
                return true;
            }

            @Override
            public void doJob(UpdateOperationCallbacks callbacks, Runnable complete) {
                fail("interrupted EDT preparation must abort job A");
            }
        };
        AsyncJob b = new AsyncJob("held startup job B") {
            @Override
            public boolean prepare(UpdateOperationCallbacks callbacks) {
                bPreparing.countDown();
                try {
                    releaseBPreparation.await(5, TimeUnit.SECONDS);
                    return false;
                } catch (InterruptedException e) {
                    Thread.currentThread().interrupt();
                    return false;
                }
            }

            @Override
            public void doJob(UpdateOperationCallbacks callbacks, Runnable complete) {
                fail("held job B is cancelled before active work");
            }
        };

        SwingUtilities.invokeLater(() -> {
            edtEntered.countDown();
            try {
                releaseEdt.await(30, TimeUnit.SECONDS);
            } catch (InterruptedException e) {
                Thread.currentThread().interrupt();
            }
        });
        assertTrue(edtEntered.await(5, TimeUnit.SECONDS));
        try {
            assertTrue(executor.startJob(a, null));
            assertTrue(aPreparing.await(5, TimeUnit.SECONDS));
            awaitQueuedEdtAction(aWorker.get());
            aWorker.get().interrupt();
            assertTrue(aReleased.await(5, TimeUnit.SECONDS));

            assertTrue(executor.startJob(b, null), "A's reservation is released after the interrupted hook");
            assertTrue(bPreparing.await(5, TimeUnit.SECONDS));
            releaseEdt.countDown();
            SwingUtilities.invokeAndWait(() -> {});

            assertEquals(0, aTeardowns.get(), "A's cancelled queued teardown must not run against a later job");
            assertNull(ownerAtATeardown.get());
        } finally {
            releaseEdt.countDown();
            releaseBPreparation.countDown();
            Thread worker = aWorker.get();
            if (worker != null && worker.isAlive()) {
                worker.interrupt();
            }
        }
        assertTrue(bReleased.await(5, TimeUnit.SECONDS));
    }

    @Test
    void releaseNotificationRecomputesBusyForTheCurrentReservation() throws Exception {
        SingleAsyncJobExecutor executor = new SingleAsyncJobExecutor(UpdateOperationCallbacks.DUMMY);
        CountDownLatch aReleaseListenerEntered = new CountDownLatch(1);
        CountDownLatch releaseAListener = new CountDownLatch(1);
        CountDownLatch aReleaseListenersFinished = new CountDownLatch(1);
        CountDownLatch bPreparing = new CountDownLatch(1);
        CountDownLatch releaseBPreparation = new CountDownLatch(1);
        CountDownLatch bReleased = new CountDownLatch(1);
        AtomicInteger releaseCount = new AtomicInteger();
        AtomicInteger busyNotifications = new AtomicInteger();
        AtomicBoolean startupBusy = new AtomicBoolean();
        AtomicBoolean busyAtAReleaseNotification = new AtomicBoolean();
        AtomicReference<AsyncJob> ownerAtAReleaseNotification = new AtomicReference<>();

        executor.addOnJobReleasedListener(() -> {
            if (releaseCount.incrementAndGet() == 1) {
                aReleaseListenerEntered.countDown();
                try {
                    releaseAListener.await(5, TimeUnit.SECONDS);
                } catch (InterruptedException e) {
                    Thread.currentThread().interrupt();
                }
            }
        });
        StartupFirmwareOperationLifecycle.registerAboutToStart(executor, startupBusy::set, ignored -> {});
        StartupFirmwareOperationLifecycle.registerReleased(executor, inProgress -> {
            startupBusy.set(inProgress);
            if (busyNotifications.incrementAndGet() == 1) {
                busyAtAReleaseNotification.set(inProgress);
                ownerAtAReleaseNotification.set(executor.getJobInProgress().orElse(null));
            }
        });
        executor.addOnJobReleasedListener(() -> {
            if (releaseCount.get() == 1) {
                aReleaseListenersFinished.countDown();
            } else {
                bReleased.countDown();
            }
        });

        AsyncJob a = new AsyncJob("fast startup job A") {
            @Override
            public void doJob(UpdateOperationCallbacks callbacks, Runnable complete) {
                complete.run();
            }
        };
        AsyncJob b = new AsyncJob("held startup job B") {
            @Override
            public boolean prepare(UpdateOperationCallbacks callbacks) {
                bPreparing.countDown();
                try {
                    releaseBPreparation.await(5, TimeUnit.SECONDS);
                    return false;
                } catch (InterruptedException e) {
                    Thread.currentThread().interrupt();
                    return false;
                }
            }

            @Override
            public void doJob(UpdateOperationCallbacks callbacks, Runnable complete) {
                fail("held job B is cancelled before active work");
            }
        };

        try {
            assertTrue(executor.startJob(a, null));
            assertTrue(aReleaseListenerEntered.await(5, TimeUnit.SECONDS));
            assertTrue(executor.isNotInProgress(), "A is released before release observers are notified");
            assertTrue(executor.startJob(b, null));
            assertTrue(bPreparing.await(5, TimeUnit.SECONDS));
            SwingUtilities.invokeAndWait(() -> {});
            assertTrue(startupBusy.get(), "B's reservation initially marks startup busy");

            releaseAListener.countDown();
            assertTrue(aReleaseListenersFinished.await(5, TimeUnit.SECONDS));
            SwingUtilities.invokeAndWait(() -> {});

            assertTrue(startupBusy.get(), "A's delayed release notification must preserve B's busy reservation");
            assertTrue(busyAtAReleaseNotification.get());
            assertSame(b, ownerAtAReleaseNotification.get());
        } finally {
            releaseAListener.countDown();
            releaseBPreparation.countDown();
        }
        assertTrue(bReleased.await(5, TimeUnit.SECONDS));
    }

    @Test
    void interruptionWaitsForAnAlreadyRunningSplashTeardownBeforeReleasingReservation() throws Exception {
        SingleAsyncJobExecutor executor = new SingleAsyncJobExecutor(UpdateOperationCallbacks.DUMMY);
        CountDownLatch callbackEntered = new CountDownLatch(1);
        CountDownLatch releaseCallback = new CountDownLatch(1);
        CountDownLatch callbackFinished = new CountDownLatch(1);
        CountDownLatch preparing = new CountDownLatch(1);
        CountDownLatch released = new CountDownLatch(1);
        AtomicReference<Thread> worker = new AtomicReference<>();
        AtomicReference<AsyncJob> ownerAtCallback = new AtomicReference<>();
        AtomicBoolean jobStarted = new AtomicBoolean();

        StartupFirmwareOperationLifecycle.registerAboutToStart(executor, ignored -> {}, ignored -> {
            ownerAtCallback.set(executor.getJobInProgress().orElse(null));
            callbackEntered.countDown();
            try {
                releaseCallback.await(5, TimeUnit.SECONDS);
            } catch (InterruptedException e) {
                Thread.currentThread().interrupt();
            } finally {
                callbackFinished.countDown();
            }
        });
        executor.addOnJobReleasedListener(released::countDown);

        AsyncJob a = new AsyncJob("callback already running") {
            @Override
            public boolean prepare(UpdateOperationCallbacks callbacks) {
                worker.set(Thread.currentThread());
                preparing.countDown();
                return true;
            }

            @Override
            public void doJob(UpdateOperationCallbacks callbacks, Runnable complete) {
                jobStarted.set(true);
                fail("interrupted teardown must abort before active work");
            }
        };
        AsyncJob b = new AsyncJob("job rejected while teardown is running") {
            @Override
            public void doJob(UpdateOperationCallbacks callbacks, Runnable complete) {
                fail("B cannot acquire A's reservation while the EDT callback is running");
            }
        };

        try {
            assertTrue(executor.startJob(a, null));
            assertTrue(preparing.await(5, TimeUnit.SECONDS));
            assertTrue(callbackEntered.await(5, TimeUnit.SECONDS));
            assertSame(a, ownerAtCallback.get());
            worker.get().interrupt();
            awaitWorkerMethod(worker.get(), "awaitCompletionUninterruptibly");

            assertSame(a, executor.getJobInProgress().orElse(null),
                "A retains ownership until its already-running EDT callback completes");
            assertFalse(executor.startJob(b, null, ignored -> {}), "B must be rejected during A's callback");
            assertEquals(1L, released.getCount());
            assertEquals(1L, callbackFinished.getCount());
        } finally {
            releaseCallback.countDown();
            Thread thread = worker.get();
            if (thread != null && thread.isAlive()) {
                thread.interrupt();
            }
        }

        assertTrue(callbackFinished.await(5, TimeUnit.SECONDS));
        assertTrue(released.await(5, TimeUnit.SECONDS));
        assertFalse(jobStarted.get());
        assertTrue(executor.isNotInProgress());
    }

    private static void awaitQueuedEdtAction(Thread worker) throws InterruptedException {
        assertNotNull(worker);
        long deadline = System.nanoTime() + TimeUnit.SECONDS.toNanos(5);
        while (System.nanoTime() < deadline) {
            for (StackTraceElement frame : worker.getStackTrace()) {
                if ("awaitCompletion".equals(frame.getMethodName())) {
                    return;
                }
            }
            LockSupport.parkNanos(TimeUnit.MILLISECONDS.toNanos(1));
        }
        fail("worker did not queue and wait for the EDT teardown callback");
    }

    private static void awaitWorkerMethod(Thread worker, String method) throws InterruptedException {
        long deadline = System.nanoTime() + TimeUnit.SECONDS.toNanos(5);
        while (System.nanoTime() < deadline) {
            for (StackTraceElement frame : worker.getStackTrace()) {
                if (method.equals(frame.getMethodName())) {
                    return;
                }
            }
            LockSupport.parkNanos(TimeUnit.MILLISECONDS.toNanos(1));
        }
        fail("worker did not reach " + method);
    }
}
