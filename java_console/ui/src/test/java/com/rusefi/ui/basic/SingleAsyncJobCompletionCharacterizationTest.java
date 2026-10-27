package com.rusefi.ui.basic;

import com.rusefi.ConnectivityContext;
import com.rusefi.DeviceSessionManager;
import com.rusefi.FakePortScanner;
import com.rusefi.io.ConnectionStatusLogic;
import com.rusefi.io.ConnectionStatusValue;
import com.rusefi.io.UpdateOperationCallbacks;
import com.rusefi.maintenance.jobs.AsyncJob;
import com.rusefi.maintenance.jobs.JobHelper;
import org.junit.jupiter.api.Test;

import javax.swing.*;
import java.util.ArrayList;
import java.util.List;
import java.util.concurrent.CountDownLatch;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;
import java.util.concurrent.Future;
import java.util.concurrent.TimeUnit;
import java.util.concurrent.atomic.AtomicBoolean;
import java.util.concurrent.atomic.AtomicInteger;
import java.util.concurrent.atomic.AtomicReference;
import java.util.concurrent.locks.LockSupport;

import static org.junit.jupiter.api.Assertions.*;
import static org.mockito.Mockito.*;

/** Regression tests for exclusive cleanup ownership and correctly attributed job outcomes. */
class SingleAsyncJobCompletionCharacterizationTest {
    @Test
    void executorRejectsBUntilACleanupReleasesScannerAndWatchdog() throws Exception {
        ConnectionStatusValue previous = ConnectionStatusLogic.INSTANCE.getValue();
        ConnectionStatusLogic.INSTANCE.setValue(ConnectionStatusValue.NOT_CONNECTED);
        SingleAsyncJobExecutor executor = new SingleAsyncJobExecutor(UpdateOperationCallbacks.DUMMY);
        CountDownLatch aStarted = new CountDownLatch(1);
        CountDownLatch bStarted = new CountDownLatch(1);
        CountDownLatch cleanupEntered = new CountDownLatch(1);
        CountDownLatch releaseCleanup = new CountDownLatch(1);
        AtomicReference<Runnable> completeA = new AtomicReference<>();
        AtomicReference<Runnable> completeB = new AtomicReference<>();
        AtomicInteger cleanupCalls = new AtomicInteger();
        AtomicInteger watchdogResumes = new AtomicInteger();
        AtomicBoolean availableDuringCleanup = new AtomicBoolean();
        AtomicBoolean acceptedDuringCleanup = new AtomicBoolean();
        AtomicBoolean acceptedAfterCleanup = new AtomicBoolean();
        AtomicBoolean cleanupAObservedBOwnership = new AtomicBoolean();
        AtomicInteger cleanupObservations = new AtomicInteger();
        List<String> rejected = new ArrayList<>();
        FakePortScanner scanner = new FakePortScanner();
        AsyncJob a = heldJob("A", completeA, aStarted);
        AsyncJob b = heldJob("B", completeB, bStarted);
        executor.addOnJobInProgressFinishedListener(() -> {
            if (cleanupCalls.incrementAndGet() == 1) {
                cleanupEntered.countDown();
                await(releaseCleanup);
            }
        });
        // These are the ACTUAL registered scanner/watchdog cleanup hooks, not a copied simulation.
        DeviceSessionManager session = new DeviceSessionManager(new ConnectivityContext(scanner), null,
            () -> {}, watchdogResumes::incrementAndGet);
        session.setJobExecutor(executor);
        executor.addOnJobInProgressFinishedListener(() -> {
            if (cleanupObservations.incrementAndGet() == 1) {
                cleanupAObservedBOwnership.set(executor.getJobInProgress().orElse(null) == b);
            }
        });
        ExecutorService threads = Executors.newFixedThreadPool(2);
        try {
            assertTrue(executor.startJob(a, null));
            assertTrue(aStarted.await(5, TimeUnit.SECONDS));
            Future<?> aCompletion = threads.submit(() -> completeA.get().run());
            assertTrue(cleanupEntered.await(5, TimeUnit.SECONDS));
            try {
                // A rejecting start must remain non-blocking on EDT; a future ownership fix cannot hold
                // the executor monitor across cleanup and deadlock this attempted start.
                Future<?> attempt = threads.submit(() -> {
                    try {
                        SwingUtilities.invokeAndWait(() -> {
                            availableDuringCleanup.set(executor.isNotInProgress());
                            acceptedDuringCleanup.set(executor.startJob(b, null, rejected::add));
                        });
                    } catch (Exception e) {
                        throw new IllegalStateException(e);
                    }
                });
                attempt.get(5, TimeUnit.SECONDS);
                if (acceptedDuringCleanup.get()) {
                    assertTrue(bStarted.await(5, TimeUnit.SECONDS));
                }
            } finally {
                releaseCleanup.countDown();
            }
            aCompletion.get(5, TimeUnit.SECONDS);
            SwingUtilities.invokeAndWait(() -> acceptedAfterCleanup.set(executor.startJob(b, null, rejected::add)));
            assertTrue(bStarted.await(5, TimeUnit.SECONDS));

            assertFalse(availableDuringCleanup.get(), "A must reserve the slot until cleanup returns");
            assertFalse(acceptedDuringCleanup.get(), "B must not start while A cleanup owns shared resources");
            assertTrue(acceptedAfterCleanup.get(), "B can start as soon as A cleanup has returned");
            assertFalse(cleanupAObservedBOwnership.get(), "A cleanup must not observe B owning its slot");
            assertEquals(1, scanner.resumeCount);
            assertEquals(1, watchdogResumes.get());
            assertSame(b, executor.getJobInProgress().orElse(null));
            completeB.get().run();
            awaitNoLongerOwnedBy(executor, b);
            assertTrue(executor.isNotInProgress());
            assertEquals(2, cleanupCalls.get());
            assertEquals(2, watchdogResumes.get());
            assertEquals(2, scanner.resumeCount, "each accepted job must resume its own scanner suspension");
        } finally {
            releaseCleanup.countDown();
            if (completeB.get() != null && !executor.isNotInProgress()) {
                completeB.get().run();
            }
            threads.shutdownNow();
            ConnectionStatusLogic.INSTANCE.setValue(previous);
        }
    }

    @Test
    void synchronousJobFailureIsRecordedBeforeCompletionAndDoesNotAffectNextJob() throws Exception {
        AtomicReference<Runnable> completeB = new AtomicReference<>();
        AtomicReference<UpdateOperationCallbacks> bCallbacks = new AtomicReference<>();
        CountDownLatch bStarted = new CountDownLatch(1);
        CountDownLatch aErrorReported = new CountDownLatch(1);
        CountDownLatch firstObserverReturned = new CountDownLatch(1);
        AtomicInteger finishes = new AtomicInteger();
        AtomicReference<UpdateFirmwareResult> resultAtAFinish = new AtomicReference<>();
        AtomicBoolean acceptedDuringAFinish = new AtomicBoolean();
        AtomicBoolean aErrorSawBOwnership = new AtomicBoolean();
        UpdateOperationCallbacks aUi = mock(UpdateOperationCallbacks.class);
        UpdateOperationCallbacks bUi = mock(UpdateOperationCallbacks.class);
        AsyncJob b = new AsyncJob("B") {
            @Override
            public void doJob(UpdateOperationCallbacks callbacks, Runnable complete) {
                bCallbacks.set(callbacks);
                completeB.set(complete);
                bStarted.countDown();
            }
        };
        SingleAsyncJobExecutor executor = new SingleAsyncJobExecutor(job -> job == b ? bUi : aUi);
        List<String> rejected = new ArrayList<>();
        doAnswer(invocation -> {
            aErrorSawBOwnership.set(executor.getJobInProgress().orElse(null) == b);
            aErrorReported.countDown();
            return null;
        }).when(aUi).error();
        executor.addOnJobInProgressFinishedListener(() -> {
            if (finishes.incrementAndGet() == 1) {
                resultAtAFinish.set(executor.getLastResult());
                acceptedDuringAFinish.set(executor.startJob(b, null, rejected::add));
                if (acceptedDuringAFinish.get()) {
                    await(bStarted);
                }
                firstObserverReturned.countDown();
            }
        });
        AsyncJob a = new AsyncJob("A synchronous JobHelper failure") {
            @Override
            public void doJob(UpdateOperationCallbacks callbacks, Runnable complete) {
                // Production JobHelper invokes complete in finally, BEFORE this exception reaches AsyncJobExecutor.
                JobHelper.doJob(() -> { throw new IllegalStateException("characterized synchronous failure"); }, complete);
            }
        };
        try {
            assertTrue(executor.startJob(a, null));
            assertTrue(aErrorReported.await(5, TimeUnit.SECONDS));
            assertTrue(firstObserverReturned.await(5, TimeUnit.SECONDS));
            awaitNoLongerOwnedBy(executor, a);
            boolean acceptedAfterAFinish = executor.startJob(b, null, rejected::add);
            assertTrue(bStarted.await(5, TimeUnit.SECONDS));

            assertEquals(UpdateFirmwareResult.FAILURE, resultAtAFinish.get(), "A's synchronous failure precedes cleanup");
            assertFalse(acceptedDuringAFinish.get(), "B cannot start from A's cleanup callback");
            assertTrue(acceptedAfterAFinish, "B can start after A has released ownership");
            assertFalse(aErrorSawBOwnership.get(), "A's failure callback must not be attributed to B");
            assertEquals(UpdateFirmwareResult.NONE, executor.getLastResult(), "B starts with a fresh result slot");
            bCallbacks.get().done();
            completeB.get().run();
            awaitNoLongerOwnedBy(executor, b);
            assertEquals(2, finishes.get());
            assertEquals(UpdateFirmwareResult.SUCCESS, executor.getLastResult());
            verify(aUi).error();
            verify(bUi).done();
            verify(bUi, never()).error();
        } finally {
            if (completeB.get() != null && !executor.isNotInProgress()) {
                completeB.get().run();
            }
        }
    }

    private static AsyncJob heldJob(String name, AtomicReference<Runnable> completion, CountDownLatch started) {
        return new AsyncJob(name) {
            @Override
            public void doJob(UpdateOperationCallbacks callbacks, Runnable complete) {
                completion.set(complete);
                started.countDown();
            }
        };
    }

    private static void await(CountDownLatch latch) {
        try {
            assertTrue(latch.await(5, TimeUnit.SECONDS));
        } catch (InterruptedException e) {
            Thread.currentThread().interrupt();
            throw new IllegalStateException(e);
        }
    }

    private static void awaitNoLongerOwnedBy(SingleAsyncJobExecutor executor, AsyncJob job) {
        long deadline = System.nanoTime() + TimeUnit.SECONDS.toNanos(5);
        while (executor.getJobInProgress().orElse(null) == job) {
            assertTrue(System.nanoTime() < deadline, "completed A never released ownership");
            LockSupport.parkNanos(TimeUnit.MILLISECONDS.toNanos(1));
        }
    }
}
