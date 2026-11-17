package com.rusefi.maintenance.jobs;

import com.rusefi.AvailableHardware;
import com.rusefi.ConnectivityContext;
import com.rusefi.FakePortScanner;
import com.rusefi.PortResult;
import com.rusefi.SerialPortType;
import com.rusefi.binaryprotocol.BinaryProtocol;
import com.rusefi.io.CommandQueue;
import com.rusefi.io.LinkManager;
import com.rusefi.io.UpdateOperationCallbacks;
import org.junit.jupiter.api.AfterEach;
import org.junit.jupiter.api.Test;

import java.util.ArrayDeque;
import java.util.Collections;
import java.util.Deque;
import java.util.function.Consumer;

import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertFalse;
import static org.junit.jupiter.api.Assertions.assertNull;
import static org.junit.jupiter.api.Assertions.assertThrows;
import static org.junit.jupiter.api.Assertions.assertTrue;
import static org.mockito.ArgumentMatchers.anyString;
import static org.mockito.Mockito.inOrder;
import static org.mockito.Mockito.mock;
import static org.mockito.Mockito.never;
import static org.mockito.Mockito.times;
import static org.mockito.Mockito.verify;
import static org.mockito.Mockito.when;

/**
 * Tests the post-flash reconnect wait {@link AbstractAutoFlashJob#awaitEcuPort} — the
 * follow-the-renumbered-port / bootloader-grace logic behind #9771 — driven through the injected
 * {@link AbstractAutoFlashJob.Clock} and a {@link FakePortScanner}, so no real time passes and the
 * hardware snapshot changes between polls on demand. [tag:better_ux_for_flashing]
 */
public class AbstractAutoFlashJobAwaitEcuPortTest {
    private static final long GRACE_MS = 5_000;

    private final FakePortScanner scanner = new FakePortScanner();
    private final AbstractAutoFlashJob job = new AbstractAutoFlashJob(
        "test", new PortResult("COM_OLD", SerialPortType.Ecu), null,
        new ConnectivityContext(scanner), null) {
        @Override
        protected boolean flash(final LinkManager lm, final BinaryProtocol bp, final UpdateOperationCallbacks cb) {
            throw new UnsupportedOperationException("not exercised");
        }
    };

    /**
     * Virtual clock: each {@link #sleep} advances time by the requested amount and runs the next
     * scripted hardware mutation (if any), so a test lays out what the scanner reports poll-by-poll.
     */
    private static class FakeClock implements AbstractAutoFlashJob.Clock {
        long now;
        int sleepCalls;
        boolean interruptOnSleep;
        final Deque<Runnable> perSleepActions = new ArrayDeque<>();

        @Override
        public long millis() {
            return now;
        }

        @Override
        public void sleep(final long ms) throws InterruptedException {
            sleepCalls++;
            if (interruptOnSleep) {
                throw new InterruptedException("test interrupt");
            }
            now += ms;
            if (!perSleepActions.isEmpty()) {
                perSleepActions.removeFirst().run();
            }
        }
    }

    private final FakeClock clock = new FakeClock();

    private static AvailableHardware hw(final PortResult... ports) {
        return new AvailableHardware(java.util.Arrays.asList(ports), false, false, false);
    }

    private static AvailableHardware dfu() {
        return new AvailableHardware(Collections.emptyList(), true, false, false);
    }

    private static AvailableHardware empty() {
        return new AvailableHardware(Collections.emptyList(), false, false, false);
    }

    private static PortResult ecu(final String port) {
        return new PortResult(port, SerialPortType.Ecu);
    }

    private static PortResult openblt(final String port) {
        return new PortResult(port, SerialPortType.OpenBlt);
    }

    @AfterEach
    public void clearInterruptFlag() {
        Thread.interrupted();
    }

    @Test
    public void ecuAlreadyPresentReturnsImmediatelyWithoutWaiting() {
        scanner.fireHardwareChange(hw(ecu("COM5")));

        assertEquals("COM5", job.awaitEcuPort(60_000, clock));
        assertEquals(0, clock.sleepCalls, "an already-present ECU must not cost a poll cycle");
    }

    @Test
    public void followsEcuOntoRenumberedPortThatAppearsAfterSeveralPolls() {
        // Board gone from the bus mid-reboot (empty, no bootloader), then re-enumerates on a NEW port.
        scanner.fireHardwareChange(empty());
        clock.perSleepActions.add(() -> {}); // still gone
        clock.perSleepActions.add(() -> {}); // still gone
        clock.perSleepActions.add(() -> scanner.fireHardwareChange(hw(ecu("COM_NEW"))));

        assertEquals("COM_NEW", job.awaitEcuPort(60_000, clock),
            "must reconnect to the renumbered port, never the ghost pre-flash port");
    }

    @Test
    public void bootloaderFlickerWithinGraceThenEcuIsFollowedNotAbandoned() {
        // Normal successful flash: brief OpenBLT flicker while re-enumerating, then firmware comes up.
        scanner.fireHardwareChange(hw(openblt("COM_BL")));
        clock.perSleepActions.add(() -> {}); // still bootloader, but within grace
        clock.perSleepActions.add(() -> scanner.fireHardwareChange(hw(ecu("COM7"))));

        assertEquals("COM7", job.awaitEcuPort(60_000, clock));
    }

    @Test
    public void persistentOpenbltBeyondGraceGivesUpBeforeTimeout() {
        scanner.fireHardwareChange(hw(openblt("COM_BL")));

        assertNull(job.awaitEcuPort(60_000, clock), "an interrupted flash must not hold the job for the full timeout");
        // grace is 5s and each poll advances 500ms, so it bails right after crossing the grace line
        assertTrue(clock.now > GRACE_MS, "should not bail before the bootloader grace period");
        assertTrue(clock.now < 60_000, "should bail well before the overall timeout");
    }

    @Test
    public void persistentDfuBeyondGraceGivesUp() {
        scanner.fireHardwareChange(dfu());

        assertNull(job.awaitEcuPort(60_000, clock));
        assertTrue(clock.now > GRACE_MS);
    }

    @Test
    public void nothingAppearsReturnsNullAtTimeout() {
        // No bootloader present, so the grace shortcut never fires — it must run out the full timeout.
        scanner.fireHardwareChange(empty());

        assertNull(job.awaitEcuPort(2_000, clock));
        assertTrue(clock.now >= 2_000, "must wait out the timeout when nothing is on the bus");
    }

    @Test
    public void interruptDuringWaitStopsAndRestoresInterruptFlag() {
        scanner.fireHardwareChange(empty());
        clock.interruptOnSleep = true;

        assertNull(job.awaitEcuPort(60_000, clock));
        assertEquals(1, clock.sleepCalls, "interrupt must break out of the wait immediately");
        assertTrue(Thread.interrupted(), "the interrupt flag must be restored for the caller");
    }

    @Test
    public void ecuWithOpenbltCountsAsConnectable() {
        scanner.fireHardwareChange(hw(new PortResult("COM9", SerialPortType.EcuWithOpenblt)));

        assertEquals("COM9", job.awaitEcuPort(60_000, clock));
    }

    @Test
    public void socketCanEcuCountsAsConnectable() {
        scanner.fireHardwareChange(hw(ecu(LinkManager.SOCKET_CAN)));

        assertEquals(LinkManager.SOCKET_CAN, job.awaitEcuPort(60_000, clock));
        assertEquals(0, clock.sleepCalls);
    }

    @Test
    public void originalSocketCanTransportIsPreferredOverAnotherEcu() {
        AbstractAutoFlashJob socketCanJob = new AbstractAutoFlashJob(
            "test", ecu(LinkManager.SOCKET_CAN), null,
            new ConnectivityContext(scanner), null) {
            @Override
            protected boolean flash(LinkManager lm, BinaryProtocol bp, UpdateOperationCallbacks cb) {
                throw new UnsupportedOperationException("not exercised");
            }
        };
        scanner.fireHardwareChange(hw(ecu("COM5"), ecu(LinkManager.SOCKET_CAN)));

        assertEquals(LinkManager.SOCKET_CAN, socketCanJob.awaitEcuPort(60_000, clock));
    }

    @Test
    public void socketCanRecoveryDoesNotFallBackToASerialEcu() {
        AbstractAutoFlashJob socketCanJob = new AbstractAutoFlashJob(
            "test", ecu(LinkManager.SOCKET_CAN), null,
            new ConnectivityContext(scanner), null) {
            @Override
            protected boolean flash(LinkManager lm, BinaryProtocol bp, UpdateOperationCallbacks cb) {
                throw new UnsupportedOperationException("not exercised");
            }
        };
        scanner.fireHardwareChange(hw(ecu("COM5")));

        assertNull(socketCanJob.awaitEcuPort(1_000, clock));
    }

    @Test
    public void pcanRecoveryDoesNotFallBackToASerialEcu() {
        AbstractAutoFlashJob pcanJob = new AbstractAutoFlashJob(
            "test", ecu(LinkManager.PCAN), null,
            new ConnectivityContext(scanner), null) {
            @Override
            protected boolean flash(LinkManager lm, BinaryProtocol bp, UpdateOperationCallbacks cb) {
                throw new UnsupportedOperationException("not exercised");
            }
        };
        scanner.fireHardwareChange(hw(ecu("COM5")));

        assertNull(pcanJob.awaitEcuPort(1_000, clock));
    }

    @Test
    public void completionRunsAfterReconnectRecovery() {
        LinkManager linkManager = mock(LinkManager.class);
        BinaryProtocol binaryProtocol = mock(BinaryProtocol.class);
        CommandQueue commandQueue = mock(CommandQueue.class);
        UpdateOperationCallbacks callbacks = mock(UpdateOperationCallbacks.class);
        Runnable completion = mock(Runnable.class);
        when(linkManager.getBinaryProtocol()).thenReturn(binaryProtocol);
        when(linkManager.getCommandQueue()).thenReturn(commandQueue);

        AbstractAutoFlashJob reconnectingJob = new AbstractAutoFlashJob(
            "test", new PortResult("COM_OLD", SerialPortType.Ecu), null,
            new ConnectivityContext(scanner), linkManager) {
            @Override
            protected boolean flash(LinkManager lm, BinaryProtocol bp, UpdateOperationCallbacks cb) {
                return true;
            }

            @Override
            String awaitEcuPort(long timeoutMs, Clock clock) {
                return "COM_NEW";
            }
        };

        reconnectingJob.doJob(callbacks, completion);

        org.mockito.InOrder order = inOrder(linkManager, completion);
        order.verify(linkManager).reconnect("COM_NEW");
        order.verify(completion).run();
    }

    /**
     * Regression for the completion-boundary bug: done() is terminal to the UI and must follow the
     * reconnect wait/attempt. This retains the original characterization's successful flash setup.
     */
    @Test
    public void characterizesSuccessReportedBeforeReconnectWait() {
        LinkManager linkManager = mock(LinkManager.class);
        UpdateOperationCallbacks callbacks = mock(UpdateOperationCallbacks.class);
        Runnable reconnectWait = mock(Runnable.class);
        Runnable completion = mock(Runnable.class);
        AbstractAutoFlashJob reconnectingJob = successfulFlashJob(linkManager, reconnectWait, "COM_NEW");

        reconnectingJob.doJob(callbacks, completion);

        org.mockito.InOrder order = inOrder(callbacks, reconnectWait, linkManager, completion);
        order.verify(reconnectWait).run();
        order.verify(linkManager).reconnect("COM_NEW");
        order.verify(callbacks).done();
        order.verify(linkManager).allowAutomaticReconnect();
        order.verify(completion).run();
        verify(callbacks).done();
        verify(callbacks, never()).error();
    }

    /**
     * A successful programming/settings step followed by no detected ECU is not a complete update.
     * Retains the characterization's timeout/flash setup, but requires a non-success final outcome.
     */
    @Test
    public void characterizesSuccessDespiteReconnectTimeout() {
        LinkManager linkManager = mock(LinkManager.class);
        UpdateOperationCallbacks callbacks = mock(UpdateOperationCallbacks.class);
        Runnable reconnectWait = mock(Runnable.class);
        Runnable completion = mock(Runnable.class);
        AbstractAutoFlashJob reconnectingJob = successfulFlashJob(linkManager, reconnectWait, null);

        reconnectingJob.doJob(callbacks, completion);

        org.mockito.InOrder order = inOrder(callbacks, reconnectWait, linkManager, completion);
        order.verify(reconnectWait).run();
        order.verify(callbacks).logLine(
            "ECU did not re-appear after flashing — reconnect manually once it enumerates.");
        order.verify(callbacks).error();
        order.verify(linkManager).allowAutomaticReconnect();
        order.verify(completion).run();
        verify(callbacks, never()).done();
        verify(linkManager, never()).reconnect(anyString());
    }

    /**
     * Direct callbacks (such as MassUpdater's StatusWindow) must retain a warning after reconnect,
     * without SingleAsyncJobExecutor to protect them. Uses the original characterization stimulus.
     */
    @Test
    public void characterizesDirectCallbacksWarningOverwrittenByCompletion() {
        LinkManager linkManager = mock(LinkManager.class);
        when(linkManager.getBinaryProtocol()).thenReturn(mock(BinaryProtocol.class));
        when(linkManager.getCommandQueue()).thenReturn(mock(CommandQueue.class));
        UpdateOperationCallbacks callbacks = mock(UpdateOperationCallbacks.class);
        Runnable reconnectWait = mock(Runnable.class);
        Runnable completion = mock(Runnable.class);
        AbstractAutoFlashJob warningJob = new AbstractAutoFlashJob("test", ecu("COM_OLD"), null,
            new ConnectivityContext(scanner), linkManager) {
            @Override
            protected boolean flash(LinkManager lm, BinaryProtocol bp, UpdateOperationCallbacks cb) {
                cb.warning();
                return true;
            }

            @Override
            String awaitEcuPort(long timeoutMs, Clock clock) {
                reconnectWait.run();
                return "COM_NEW";
            }
        };

        warningJob.doJob(callbacks, completion);

        org.mockito.InOrder order = inOrder(callbacks, reconnectWait, linkManager, completion);
        order.verify(reconnectWait).run();
        order.verify(linkManager).reconnect("COM_NEW");
        order.verify(callbacks).warning();
        order.verify(completion).run();
        verify(callbacks).warning();
        verify(callbacks, never()).done();
        verify(callbacks, never()).error();
    }

    @Test
    public void nestedDoneDoesNotPublishSuccessBeforeReconnect() {
        UpdateOperationCallbacks callbacks = mock(UpdateOperationCallbacks.class);

        LinkManager lm = runFlashAction(callbacks, nested -> {
            nested.done();
            verify(callbacks, never()).done();
        });

        org.mockito.InOrder order = inOrder(lm, callbacks);
        order.verify(lm).reconnect("COM_NEW");
        order.verify(callbacks).done();
        verify(callbacks).done();
        verify(callbacks, never()).warning();
        verify(callbacks, never()).error();
    }

    @Test
    public void nestedFailureSurvivesWarningDoneAndPresentationClear() {
        UpdateOperationCallbacks callbacks = mock(UpdateOperationCallbacks.class);

        LinkManager lm = runFlashAction(callbacks, nested -> {
            nested.error();
            nested.warning();
            nested.clear();
            nested.done();
            verify(callbacks, never()).error();
            verify(callbacks, never()).warning();
            verify(callbacks, never()).done();
        });

        org.mockito.InOrder order = inOrder(callbacks, lm);
        order.verify(callbacks).clear();
        order.verify(lm).reconnect("COM_NEW");
        order.verify(callbacks).error();
        verify(callbacks).error();
        verify(callbacks, never()).warning();
        verify(callbacks, never()).done();
    }

    @Test
    public void flashCallbacksForwardNonterminalEventsAndInteractivePolicy() {
        UpdateOperationCallbacks callbacks = mock(UpdateOperationCallbacks.class);
        when(callbacks.isInteractive()).thenReturn(true);

        runFlashAction(callbacks, nested -> {
            assertTrue(nested.isInteractive());
            nested.log("step details", false, true);
            nested.logLine("step status");
            nested.updateProgress(42);
            nested.firmwareUpdateBlocked("step rejection details");
            nested.firmwareHandoffStarted();
        });

        verify(callbacks).log("step details", false, true);
        verify(callbacks).logLine("step status");
        verify(callbacks).updateProgress(42);
        verify(callbacks).firmwareUpdateBlocked("step rejection details");
        // The job's own pre-flash handoff and the explicitly forwarded nested event.
        verify(callbacks, times(2)).firmwareHandoffStarted();
    }

    @Test
    public void flashCallbacksPreserveHeadlessPolicyForLoggerDummyAndCustomCallbacks() {
        UpdateOperationCallbacks customHeadless = mock(UpdateOperationCallbacks.class);
        when(customHeadless.isInteractive()).thenReturn(false);
        for (UpdateOperationCallbacks callbacks : new UpdateOperationCallbacks[]{
            UpdateOperationCallbacks.LOGGER, UpdateOperationCallbacks.DUMMY, customHeadless}) {
            runFlashAction(callbacks, nested -> assertFalse(nested.isInteractive()));
        }
    }

    private LinkManager runFlashAction(UpdateOperationCallbacks callbacks,
                                       Consumer<UpdateOperationCallbacks> flashAction) {
        LinkManager lm = mock(LinkManager.class);
        when(lm.getBinaryProtocol()).thenReturn(mock(BinaryProtocol.class));
        when(lm.getCommandQueue()).thenReturn(mock(CommandQueue.class));
        AbstractAutoFlashJob nestedStepJob = new AbstractAutoFlashJob("test", ecu("COM_OLD"), null,
            new ConnectivityContext(scanner), lm) {
            @Override
            protected boolean flash(LinkManager link, BinaryProtocol bp, UpdateOperationCallbacks nested) {
                flashAction.accept(nested);
                return true;
            }

            @Override
            String awaitEcuPort(long timeoutMs, Clock clock) {
                return "COM_NEW";
            }
        };
        nestedStepJob.doJob(callbacks, () -> {});
        return lm;
    }

    private AbstractAutoFlashJob successfulFlashJob(final LinkManager linkManager, final Runnable reconnectWait,
                                                   final String detectedPort) {
        when(linkManager.getBinaryProtocol()).thenReturn(mock(BinaryProtocol.class));
        when(linkManager.getCommandQueue()).thenReturn(mock(CommandQueue.class));
        return new AbstractAutoFlashJob("test", ecu("COM_OLD"), null,
            new ConnectivityContext(scanner), linkManager) {
            @Override
            protected boolean flash(LinkManager lm, BinaryProtocol bp, UpdateOperationCallbacks cb) {
                return true;
            }

            @Override
            String awaitEcuPort(long timeoutMs, Clock clock) {
                reconnectWait.run();
                return detectedPort;
            }
        };
    }

    /** An eligibility rejection must prevent handoff and recovery. */
    @Test
    public void flashingEligibilityRejectionStopsBeforeHandoff() {
        LinkManager linkManager = mock(LinkManager.class);
        BinaryProtocol binaryProtocol = mock(BinaryProtocol.class);
        CommandQueue commandQueue = mock(CommandQueue.class);
        UpdateOperationCallbacks callbacks = mock(UpdateOperationCallbacks.class);
        Runnable completion = mock(Runnable.class);
        Runnable programming = mock(Runnable.class);
        when(linkManager.getBinaryProtocol()).thenReturn(binaryProtocol);
        when(linkManager.getCommandQueue()).thenReturn(commandQueue);

        AbstractAutoFlashJob rejectedJob = new AbstractAutoFlashJob(
            "test", new PortResult("COM_OLD", SerialPortType.Ecu), null,
            new ConnectivityContext(scanner), linkManager) {
            // Before the fix the job has no eligibility hook and ignores this decision.
            protected boolean isFlashAllowed(BinaryProtocol bp, UpdateOperationCallbacks cb) {
                cb.logLine("This update requires an intermediate firmware version");
                return false;
            }

            @Override
            protected boolean flash(LinkManager lm, BinaryProtocol bp, UpdateOperationCallbacks cb) {
                lm.disconnect();
                programming.run();
                return true;
            }

            @Override
            String awaitEcuPort(long timeoutMs, Clock clock) {
                return "COM_NEW";
            }
        };

        rejectedJob.doJob(callbacks, completion);

        verify(programming, never()).run();
        verify(linkManager, never()).disconnect();
        verify(linkManager, never()).reconnect(anyString());
        verify(linkManager, never()).allowAutomaticReconnect();
        verify(commandQueue, never()).clearPendingCommands();
        verify(callbacks, never()).done();
        verify(callbacks).error();
        verify(callbacks).logLine("This update requires an intermediate firmware version");
        verify(completion).run();
    }

    @Test
    public void completionRunsOnceWhenFlashThrows() {
        LinkManager linkManager = mock(LinkManager.class);
        BinaryProtocol binaryProtocol = mock(BinaryProtocol.class);
        UpdateOperationCallbacks callbacks = mock(UpdateOperationCallbacks.class);
        Runnable completion = mock(Runnable.class);
        when(linkManager.getBinaryProtocol()).thenReturn(binaryProtocol);

        AbstractAutoFlashJob failingJob = new AbstractAutoFlashJob(
            "test", new PortResult("COM_OLD", SerialPortType.Ecu), null,
            new ConnectivityContext(scanner), linkManager) {
            @Override
            protected boolean flash(LinkManager lm, BinaryProtocol bp, UpdateOperationCallbacks cb) {
                throw new IllegalStateException("flash failed");
            }
        };

        assertThrows(IllegalStateException.class, () -> failingJob.doJob(callbacks, completion));
        verify(completion).run();
        verify(linkManager, never()).reconnect(anyString());
    }

    @Test
    public void failedHandoffPreparationReportsErrorAndCompletesWithoutFlashing() {
        LinkManager lm = mock(LinkManager.class);
        when(lm.getBinaryProtocol()).thenReturn(mock(BinaryProtocol.class));
        UpdateOperationCallbacks callbacks = mock(UpdateOperationCallbacks.class);
        org.mockito.Mockito.doThrow(new IllegalStateException("UI preparation failed"))
            .when(callbacks).firmwareHandoffStarted();
        Runnable completion = mock(Runnable.class);
        AbstractAutoFlashJob job = new AbstractAutoFlashJob("test", ecu("COM_OLD"), null,
            new ConnectivityContext(scanner), lm) {
            @Override
            protected boolean flash(LinkManager link, BinaryProtocol bp, UpdateOperationCallbacks cb) {
                throw new AssertionError("Must not flash after failed handoff preparation");
            }
        };

        job.doJob(callbacks, completion);

        verify(callbacks).logLine("Unable to prepare firmware handoff: UI preparation failed");
        verify(callbacks).error();
        verify(callbacks, never()).done();
        verify(completion).run();
        verify(lm, never()).disconnect();
        verify(lm, never()).reconnect(anyString());
        verify(lm, never()).allowAutomaticReconnect();
    }

    @Test
    public void failedRecoveryReenablesAutomaticReconnectBeforeCompletion() {
        LinkManager linkManager = new LinkManager().setNotifyGlobalStatusOnClose(false);
        linkManager.setBinaryProtocolForTests(mock(BinaryProtocol.class));

        AbstractAutoFlashJob failingJob = new AbstractAutoFlashJob(
            "test", new PortResult("COM_OLD", SerialPortType.Ecu), null,
            new ConnectivityContext(scanner), linkManager) {
            @Override
            protected boolean flash(LinkManager lm, BinaryProtocol bp, UpdateOperationCallbacks cb) {
                lm.disconnect();
                return false;
            }

            @Override
            String awaitEcuPort(long timeoutMs, Clock clock) {
                return null;
            }
        };

        failingJob.doJob(mock(UpdateOperationCallbacks.class), () ->
            assertFalse(linkManager.isDisconnectedByUser(),
                "automatic reconnect must be enabled before completion listeners run"));

        assertFalse(linkManager.isDisconnectedByUser(),
            "failed recovery must not leave a permanent user-disconnect marker");
    }

    @Test
    public void rejectedBeforeFirmwareHandoffPreservesUserDisconnect() {
        LinkManager linkManager = new LinkManager().setNotifyGlobalStatusOnClose(false);
        linkManager.disconnect();

        AbstractAutoFlashJob rejectedJob = new AbstractAutoFlashJob(
            "test", new PortResult("COM_OLD", SerialPortType.Ecu), null,
            new ConnectivityContext(scanner), linkManager) {
            @Override
            protected boolean flash(LinkManager lm, BinaryProtocol bp, UpdateOperationCallbacks cb) {
                throw new AssertionError("firmware handoff must not start without a binary protocol");
            }
        };

        rejectedJob.doJob(mock(UpdateOperationCallbacks.class), () -> {});

        assertTrue(linkManager.isDisconnectedByUser(),
            "a job rejected before firmware handoff must preserve explicit user intent");
    }
}
