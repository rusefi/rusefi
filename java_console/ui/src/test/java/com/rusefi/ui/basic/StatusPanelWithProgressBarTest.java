package com.rusefi.ui.basic;

import com.rusefi.io.UpdateOperationCallbacks;
import com.rusefi.maintenance.jobs.AsyncJob;
import org.junit.jupiter.api.Test;

import javax.swing.SwingUtilities;
import java.util.concurrent.CountDownLatch;
import java.util.concurrent.TimeUnit;
import java.util.concurrent.atomic.AtomicInteger;
import java.util.concurrent.atomic.AtomicReference;

import static org.junit.jupiter.api.Assertions.*;
import static org.mockito.Mockito.*;

class StatusPanelWithProgressBarTest {
    @Test
    void rejectedJobDisplaysExactReasonOnEdtAndRetainsLog() throws Exception {
        AtomicReference<StatusPanelWithProgressBar> panel = new AtomicReference<>();
        AtomicReference<String> displayed = new AtomicReference<>();
        AtomicInteger displays = new AtomicInteger();
        SwingUtilities.invokeAndWait(() -> panel.set(spy(new StatusPanelWithProgressBar(reason -> {
            assertTrue(SwingUtilities.isEventDispatchThread());
            displayed.set(reason);
            displays.incrementAndGet();
        }))));
        String reason = "Install an intermediate version first.\nDetails: <current> & <destination>";
        SingleAsyncJobExecutor executor = new SingleAsyncJobExecutor(panel.get());
        CountDownLatch finished = new CountDownLatch(1);
        executor.addOnJobInProgressFinishedListener(finished::countDown);

        assertTrue(executor.startJob(new AsyncJob("blocked firmware") {
            @Override
            public void doJob(UpdateOperationCallbacks callbacks, Runnable onJobFinished) {
                callbacks.firmwareUpdateBlocked(reason);
                callbacks.error();
                onJobFinished.run();
            }
        }, null));
        assertTrue(finished.await(5, TimeUnit.SECONDS));
        SwingUtilities.invokeAndWait(() -> {});

        assertEquals(reason, displayed.get());
        assertEquals(1, displays.get());
        verify(panel.get()).log(reason, true, true);
        assertEquals(UpdateFirmwareResult.FAILURE, executor.getLastResult());
        assertTrue(executor.isNotInProgress());
    }

    @Test
    void ordinaryErrorsDoNotOpenBlockedUpdateScreen() throws Exception {
        AtomicInteger displays = new AtomicInteger();
        SwingUtilities.invokeAndWait(() -> {
            StatusPanelWithProgressBar panel = new StatusPanelWithProgressBar(reason -> displays.incrementAndGet());
            panel.error();
            panel.done();
        });
        SwingUtilities.invokeAndWait(() -> {});
        assertEquals(0, displays.get());
    }

    @Test
    void nonUiCallbackDefaultsToLoggingTheReason() {
        UpdateOperationCallbacks callbacks = mock(UpdateOperationCallbacks.class, CALLS_REAL_METHODS);
        callbacks.firmwareUpdateBlocked("Intermediate update required");
        verify(callbacks).log("Intermediate update required", true, true);
    }
}
