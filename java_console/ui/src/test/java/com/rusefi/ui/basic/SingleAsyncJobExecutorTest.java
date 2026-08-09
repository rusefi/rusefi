package com.rusefi.ui.basic;

import com.rusefi.io.UpdateOperationCallbacks;
import com.rusefi.maintenance.jobs.AsyncJob;
import org.junit.jupiter.api.Test;

import java.util.concurrent.CountDownLatch;
import java.util.concurrent.TimeUnit;

import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertTrue;

class SingleAsyncJobExecutorTest {
    @Test
    void unrecordedJobDoesNotOverwriteRecordedResult() throws Exception {
        final SingleAsyncJobExecutor executor = new SingleAsyncJobExecutor(
            job -> UpdateOperationCallbacks.DUMMY,
            job -> "firmware".equals(job.getName()));

        CountDownLatch firmwareFinished = new CountDownLatch(1);
        executor.addOnJobInProgressFinishedListener(firmwareFinished::countDown);
        assertTrue(executor.startJob(completedJob("firmware", true), null));
        assertTrue(firmwareFinished.await(5, TimeUnit.SECONDS));
        assertEquals(UpdateFirmwareResult.SUCCESS, executor.getLastResult());

        CountDownLatch tuneFinished = new CountDownLatch(1);
        executor.addOnJobInProgressFinishedListener(tuneFinished::countDown);
        assertTrue(executor.startJob(completedJob("tune", false), null));
        assertTrue(tuneFinished.await(5, TimeUnit.SECONDS));
        assertEquals(UpdateFirmwareResult.SUCCESS, executor.getLastResult());
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
