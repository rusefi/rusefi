package com.rusefi;

import com.rusefi.binaryprotocol.BinaryProtocol;
import com.rusefi.io.LinkManager;
import com.rusefi.io.CommandQueue;
import com.rusefi.io.UpdateOperationCallbacks;
import com.rusefi.maintenance.jobs.AsyncJob;
import com.rusefi.maintenance.jobs.OpenBltAutoJob;
import com.rusefi.maintenance.jobs.DfuAutoJob;
import com.rusefi.ui.basic.SingleAsyncJobExecutor;
import com.rusefi.ui.basic.StatusPanelWithProgressBar;
import com.rusefi.ui.basic.UpdateFirmwareResult;
import org.junit.jupiter.api.Test;

import java.util.concurrent.CountDownLatch;
import java.util.concurrent.TimeUnit;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Collections;
import java.util.List;
import java.util.concurrent.atomic.AtomicReference;
import javax.swing.SwingUtilities;

import static org.junit.jupiter.api.Assertions.*;
import static org.mockito.Mockito.*;

class StartupFirmwareHandoffTest {
    /** An eligibility rejection keeps the original splash listeners and live connection intact. */
    @Test
    void rejectedUpdatePreservesOriginalSplashConnection() throws Exception {
        LinkManager lm = mock(LinkManager.class);
        when(lm.getBinaryProtocol()).thenReturn(mock(BinaryProtocol.class));
        Runnable releaseSplash = mock(Runnable.class);
        Runnable flash = mock(Runnable.class);
        UpdateOperationCallbacks callbacks = mock(UpdateOperationCallbacks.class);
        SingleAsyncJobExecutor executor = new SingleAsyncJobExecutor(callbacks);
        executor.addOnJobAboutToStartListener(() -> executor.getJobInProgress()
            .ifPresent(job -> StartupFrame.prepareSplashForJob(job, releaseSplash)));
        CountDownLatch finished = new CountDownLatch(1);
        executor.addOnJobInProgressFinishedListener(finished::countDown);

        OpenBltAutoJob job = new OpenBltAutoJob(new PortResult("TEST_PORT", SerialPortType.EcuWithOpenblt),
            null, new ConnectivityContext(new FakePortScanner()), lm) {
            @Override
            protected boolean isFlashAllowed(BinaryProtocol bp, UpdateOperationCallbacks cb) {
                cb.firmwareUpdateBlocked("Test rejection");
                return false;
            }

            @Override
            protected boolean flash(LinkManager link, BinaryProtocol bp, UpdateOperationCallbacks cb) {
                flash.run();
                return true;
            }
        };

        assertTrue(executor.startJob(job, null));
        assertTrue(finished.await(5, TimeUnit.SECONDS));

        verifyNoInteractions(releaseSplash);
        verifyNoInteractions(flash);
        verify(lm, never()).disconnect();
        verify(callbacks).firmwareUpdateBlocked("Test rejection");
        verify(callbacks).error();
        verify(callbacks, never()).firmwareHandoffStarted();
        assertTrue(executor.isNotInProgress());
    }

    @Test
    void allowedUpdatePreparesSplashOnEdtAfterEligibilityBeforeFlash() throws Exception {
        List<String> events = Collections.synchronizedList(new ArrayList<>());
        LinkManager lm = mock(LinkManager.class);
        when(lm.getBinaryProtocol()).thenReturn(mock(BinaryProtocol.class));
        when(lm.getCommandQueue()).thenReturn(mock(CommandQueue.class));
        FakePortScanner scanner = new FakePortScanner();
        PortResult port = new PortResult("TEST_PORT", SerialPortType.EcuWithOpenblt);
        scanner.fireHardwareChange(new AvailableHardware(Collections.singletonList(port), false, false, false));
        Runnable releaseSplash = () -> {
            assertTrue(SwingUtilities.isEventDispatchThread());
            events.add("prepare");
        };
        AtomicReference<StatusPanelWithProgressBar> panel = new AtomicReference<>();
        SwingUtilities.invokeAndWait(() -> panel.set(new StatusPanelWithProgressBar(reason -> fail(reason), releaseSplash)));
        SingleAsyncJobExecutor executor = new SingleAsyncJobExecutor(panel.get());
        executor.addOnJobAboutToStartListener(() -> executor.getJobInProgress()
            .ifPresent(job -> StartupFrame.prepareSplashForJob(job, releaseSplash)));
        CountDownLatch finished = new CountDownLatch(1);
        executor.addOnJobInProgressFinishedListener(finished::countDown);
        OpenBltAutoJob job = new OpenBltAutoJob(port, null, new ConnectivityContext(scanner), lm) {
            @Override
            protected boolean isFlashAllowed(BinaryProtocol bp, UpdateOperationCallbacks cb) {
                events.add("eligible");
                return true;
            }

            @Override
            protected boolean flash(LinkManager link, BinaryProtocol bp, UpdateOperationCallbacks cb) {
                events.add("flash");
                return true;
            }
        };

        assertTrue(executor.startJob(job, null));
        assertTrue(finished.await(5, TimeUnit.SECONDS));
        assertEquals(Arrays.asList("eligible", "prepare", "flash"), events);
        assertEquals(UpdateFirmwareResult.SUCCESS, executor.getLastResult());
        verify(lm).reconnect("TEST_PORT");
    }

    @Test
    void automaticDfuAlsoDefersSplashHandoff() {
        Runnable releaseSplash = mock(Runnable.class);
        StartupFrame.prepareSplashForJob(new DfuAutoJob(new PortResult("TEST_PORT", SerialPortType.Ecu),
            null, new ConnectivityContext(new FakePortScanner()), mock(LinkManager.class)), releaseSplash);
        verifyNoInteractions(releaseSplash);
    }

    @Test
    void otherJobsStillPrepareSplashAtJobStart() {
        Runnable releaseSplash = mock(Runnable.class);
        StartupFrame.prepareSplashForJob(new AsyncJob("manual job") {
            @Override
            public void doJob(UpdateOperationCallbacks callbacks, Runnable finished) {
                throw new AssertionError("Not executed by preparation");
            }
        }, releaseSplash);
        verify(releaseSplash).run();
    }
}
