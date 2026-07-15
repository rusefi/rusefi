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

import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertNull;
import static org.junit.jupiter.api.Assertions.assertThrows;
import static org.junit.jupiter.api.Assertions.assertTrue;
import static org.mockito.ArgumentMatchers.anyString;
import static org.mockito.Mockito.inOrder;
import static org.mockito.Mockito.mock;
import static org.mockito.Mockito.never;
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
}
