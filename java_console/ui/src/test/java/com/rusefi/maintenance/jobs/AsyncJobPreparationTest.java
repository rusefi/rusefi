package com.rusefi.maintenance.jobs;

import com.rusefi.io.UpdateOperationCallbacks;
import org.junit.jupiter.api.Test;

import javax.swing.*;
import java.util.concurrent.CountDownLatch;
import java.util.concurrent.TimeUnit;
import java.util.concurrent.atomic.AtomicInteger;

import static org.junit.jupiter.api.Assertions.*;
import static org.mockito.Mockito.*;

class AsyncJobPreparationTest {
    @Test
    void directStatusWindowWaitsForConsentAndCancellationNeverCreatesIt() throws Exception {
        for (String result : new String[]{"accepted", "cancelled", "error"}) {
            CountDownLatch preparing = new CountDownLatch(1);
            CountDownLatch consent = new CountDownLatch(1);
            CountDownLatch finished = new CountDownLatch(1);
            AtomicInteger windows = new AtomicInteger();
            AtomicInteger jobs = new AtomicInteger();
            AtomicInteger completions = new AtomicInteger();
            UpdateOperationCallbacks secondary = mock(UpdateOperationCallbacks.class);
            UpdateOperationCallbacks window = mock(UpdateOperationCallbacks.class);
            AsyncJob job = new AsyncJob("preparing recovery") {
                @Override
                public boolean prepare(UpdateOperationCallbacks callbacks) {
                    preparing.countDown();
                    try {
                        assertTrue(consent.await(5, TimeUnit.SECONDS));
                    } catch (InterruptedException e) {
                        Thread.currentThread().interrupt();
                        return false;
                    }
                    if (result.equals("error")) {
                        callbacks.logLine("Preflight unavailable");
                        callbacks.error();
                    }
                    return result.equals("accepted");
                }

                @Override
                public void doJob(UpdateOperationCallbacks callbacks, Runnable complete) {
                    jobs.incrementAndGet();
                    complete.run();
                    complete.run(); // The executor releases ownership exactly once even with a duplicate completion.
                }
            };
            AsyncJobExecutor.INSTANCE.executeJobWithStatusWindow(job, secondary, () -> {
                completions.incrementAndGet();
                finished.countDown();
            }, () -> {
                assertTrue(SwingUtilities.isEventDispatchThread());
                windows.incrementAndGet();
                return window;
            });
            try {
                assertTrue(preparing.await(5, TimeUnit.SECONDS));
                assertEquals(0, windows.get());
                assertEquals(0, jobs.get());
            } finally {
                consent.countDown();
            }
            assertTrue(finished.await(5, TimeUnit.SECONDS));
            assertEquals(result.equals("cancelled") ? 0 : 1, windows.get());
            assertEquals(result.equals("accepted") ? 1 : 0, jobs.get());
            assertEquals(1, completions.get());
            verify(secondary, never()).done();
            if (result.equals("error")) {
                verify(secondary).error();
                verify(window).log("Preflight unavailable", true, true);
                verify(window).error();
            } else {
                verify(secondary, never()).error();
            }
        }
    }
}
