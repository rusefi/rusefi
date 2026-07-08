package com.rusefi;

import org.junit.jupiter.api.Test;

import java.util.concurrent.CountDownLatch;

import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertNotSame;
import static org.junit.jupiter.api.Assertions.assertSame;

/**
 * Tests for the suspend/resume latch handshake flash jobs rely on to know the scan worker has
 * actually parked before they open the port. Exercised without start() — the latch state machine
 * is synchronous; only the 300ms worker loop needs a real thread. [tag:better_ux_for_flashing]
 */
public class RecurringStepTest {
    private static final Runnable NOOP = () -> {
    };

    private static RecurringStep step() {
        return new RecurringStep(NOOP, NOOP, "RecurringStepTest");
    }

    @Test
    public void suspendReturnsLatchThatWaitsForTheWorkerToPark() {
        CountDownLatch latch = step().suspend();
        assertEquals(1, latch.getCount(), "caller must wait until the worker observes the suspend");
    }

    @Test
    public void repeatedSuspendReturnsTheSameLatch() {
        RecurringStep step = step();
        assertSame(step.suspend(), step.suspend(),
            "a second suspend while already suspended must join the same handshake");
    }

    @Test
    public void suspendAfterResumeStartsAFreshHandshake() {
        RecurringStep step = step();
        CountDownLatch first = step.suspend();
        step.resume();
        CountDownLatch second = step.suspend();
        assertNotSame(first, second);
        assertEquals(1, second.getCount());
    }

    @Test
    public void suspendAfterStopReleasesCallerImmediately() {
        RecurringStep step = step();
        step.stop();
        // The worker thread has exited (or never ran): checkSuspended() will never count the latch
        // down, so suspend() must hand back a released latch instead of blocking the flash job forever.
        CountDownLatch latch = step.suspend();
        assertEquals(0, latch.getCount());
    }
}
