package com.rusefi;

import org.junit.jupiter.api.Test;

import java.util.Collections;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.concurrent.CopyOnWriteArrayList;
import java.util.concurrent.CountDownLatch;
import java.util.concurrent.FutureTask;
import java.util.concurrent.TimeUnit;
import java.util.concurrent.atomic.AtomicInteger;
import java.util.concurrent.atomic.AtomicBoolean;
import java.util.function.Function;

import static java.util.Arrays.asList;
import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertFalse;
import static org.junit.jupiter.api.Assertions.assertTrue;

/**
 * Tests for the parallel port-probe fan-out with an injected inspector — no real serial ports.
 * Covers the result-classification rules: dead ports (null) are dropped, a crashing probe is
 * reported as Unknown rather than killing the scan. Also exercises the real timeout with an
 * inspector that ignores interruption, like a blocked native serial open. [tag:better_ux_for_flashing]
 */
public class SerialPortScannerInspectPortsTest {

    private static List<PortResult> inspect(List<String> ports, Function<String, PortResult> inspector) {
        try {
            return SerialPortScanner.inspectPorts(ports, new HashMap<>(), inspector);
        } finally {
            Thread.interrupted();
        }
    }

    @Test
    public void emptyPortListReturnsImmediately() {
        long before = System.currentTimeMillis();
        assertTrue(inspect(Collections.emptyList(), port -> new PortResult(port, SerialPortType.Ecu)).isEmpty());
        assertTrue(System.currentTimeMillis() - before < 4000, "empty input must not wait out the probe timeout");
    }

    @Test
    public void allInspectedPortsAreCollected() {
        List<PortResult> results = inspect(asList("COM1", "COM2"),
            port -> new PortResult(port, "COM1".equals(port) ? SerialPortType.Ecu : SerialPortType.Unknown));
        assertEquals(2, results.size());
        assertTrue(results.contains(new PortResult("COM1", SerialPortType.Ecu)));
        assertTrue(results.contains(new PortResult("COM2", SerialPortType.Unknown)));
    }

    @Test
    public void deadPortIsDroppedFromResults() {
        // the inspector returns null for a stale OS node (device disappeared mid-probe)
        List<PortResult> results = inspect(asList("COM1", "COM_DEAD"),
            port -> "COM_DEAD".equals(port) ? null : new PortResult(port, SerialPortType.Ecu));
        assertEquals(Collections.singletonList(new PortResult("COM1", SerialPortType.Ecu)), results);
    }

    @Test
    public void crashingInspectorYieldsUnknownInsteadOfKillingTheScan() {
        List<PortResult> results = inspect(asList("COM1", "COM_BOOM"), port -> {
            if ("COM_BOOM".equals(port)) {
                throw new RuntimeException("probe blew up");
            }
            return new PortResult(port, SerialPortType.Ecu);
        });
        assertEquals(2, results.size());
        assertTrue(results.contains(new PortResult("COM_BOOM", SerialPortType.Unknown)),
            "a probe exception must degrade to Unknown, not lose the port");
        assertTrue(results.contains(new PortResult("COM1", SerialPortType.Ecu)));
    }

    @Test
    public void identifiedIniResolutionGetsBoundedExtraWaitWithoutWaitingForUnknownSibling() throws Exception {
        CountDownLatch iniResolutionEntered = new CountDownLatch(1);
        CountDownLatch releaseIniResolution = new CountDownLatch(1);
        CountDownLatch unknownProbeEntered = new CountDownLatch(1);
        CountDownLatch unknownProbeInterrupted = new CountDownLatch(1);
        CountDownLatch releaseUnknownProbe = new CountDownLatch(1);
        CountDownLatch extensionWaitStarted = new CountDownLatch(1);
        AtomicBoolean iniResolutionInProgress = new AtomicBoolean();
        Map<String, Thread> tracked = new HashMap<>();
        FutureTask<List<PortResult>> scan = new FutureTask<>(() ->
            SerialPortScanner.inspectPorts(asList("INI_TARGET", "UNKNOWN_SIBLING"), tracked, port -> {
                if ("INI_TARGET".equals(port)) {
                    iniResolutionInProgress.set(true);
                    iniResolutionEntered.countDown();
                    awaitIgnoringInterrupts(releaseIniResolution);
                    iniResolutionInProgress.set(false);
                    return new PortResult(port, SerialPortType.Ecu);
                }
                unknownProbeEntered.countDown();
                try {
                    releaseUnknownProbe.await();
                } catch (InterruptedException e) {
                    unknownProbeInterrupted.countDown();
                    awaitIgnoringInterrupts(releaseUnknownProbe);
                }
                return new PortResult(port, SerialPortType.Ecu);
            }, port -> {
                boolean pending = "INI_TARGET".equals(port) && iniResolutionInProgress.get();
                if (pending) {
                    extensionWaitStarted.countDown();
                }
                return pending;
            }, 50, 1_000));
        Thread scanThread = new Thread(scan, "test INI-aware scanner deadline");
        scanThread.setDaemon(true);
        scanThread.start();

        try {
            assertTrue(iniResolutionEntered.await(1, TimeUnit.SECONDS));
            assertTrue(unknownProbeEntered.await(1, TimeUnit.SECONDS));
            assertTrue(extensionWaitStarted.await(1, TimeUnit.SECONDS),
                "the scanner should extend only after identifying the active INI resolution");
            assertTrue(unknownProbeInterrupted.await(1, TimeUnit.SECONDS),
                "unfinished non-INI probes must be interrupted at the initial deadline");
            releaseIniResolution.countDown();

            List<PortResult> results = scan.get(1, TimeUnit.SECONDS);
            assertTrue(results.contains(new PortResult("INI_TARGET", SerialPortType.Ecu)),
                "the identified ECU must finish INI resolution before its probe is interrupted");
            assertTrue(results.contains(new PortResult("UNKNOWN_SIBLING", SerialPortType.Unknown)),
                "an interrupted sibling remains Unknown and a late result cannot overwrite it");
        } finally {
            releaseIniResolution.countDown();
            releaseUnknownProbe.countDown();
        }
    }

    @Test
    public void identifiedIniResolutionStillStopsAtTheTotalScanDeadline() throws Exception {
        CountDownLatch iniResolutionEntered = new CountDownLatch(1);
        CountDownLatch releaseIniResolution = new CountDownLatch(1);
        CountDownLatch extensionWaitStarted = new CountDownLatch(1);
        AtomicBoolean iniResolutionInProgress = new AtomicBoolean();
        FutureTask<List<PortResult>> scan = new FutureTask<>(() ->
            SerialPortScanner.inspectPorts(Collections.singletonList("INI_TARGET"), new HashMap<>(), port -> {
                iniResolutionInProgress.set(true);
                iniResolutionEntered.countDown();
                awaitIgnoringInterrupts(releaseIniResolution);
                return new PortResult(port, SerialPortType.Ecu);
            }, port -> {
                boolean pending = iniResolutionInProgress.get();
                if (pending) {
                    extensionWaitStarted.countDown();
                }
                return pending;
            }, 50, 250));
        Thread scanThread = new Thread(scan, "test bounded INI scan deadline");
        scanThread.setDaemon(true);
        scanThread.start();

        try {
            assertTrue(iniResolutionEntered.await(1, TimeUnit.SECONDS));
            assertTrue(extensionWaitStarted.await(1, TimeUnit.SECONDS));
            assertEquals(SerialPortType.Unknown, scan.get(1, TimeUnit.SECONDS).get(0).type,
                "an INI probe that exceeds its total deadline remains bounded");
        } finally {
            releaseIniResolution.countDown();
        }
    }

    private static void awaitIgnoringInterrupts(CountDownLatch latch) {
        boolean released = false;
        while (!released) {
            try {
                released = latch.await(5, TimeUnit.SECONDS);
            } catch (InterruptedException ignored) {
                // Model an underlying protocol/native operation which finishes only on its own.
            }
        }
    }

    @Test
    public void stuckProbeIsNotRepeatedUntilItsEarlierThreadExits() throws InterruptedException {
        Map<String, Thread> tracked = new HashMap<>();
        List<Thread> blockedThreads = new CopyOnWriteArrayList<>();
        CountDownLatch release = new CountDownLatch(1);
        AtomicInteger stuckCalls = new AtomicInteger();
        AtomicInteger healthyCalls = new AtomicInteger();
        Function<String, PortResult> inspector = port -> {
            if ("STUCK".equals(port)) {
                blockedThreads.add(Thread.currentThread());
                stuckCalls.incrementAndGet();
                boolean interrupted = false;
                while (true) {
                    try {
                        release.await();
                        break;
                    } catch (InterruptedException e) {
                        // Model a native open that cannot be cancelled by Thread.interrupt().
                        interrupted = true;
                    }
                }
                if (interrupted) {
                    Thread.currentThread().interrupt();
                }
            } else {
                healthyCalls.incrementAndGet();
            }
            return new PortResult(port, SerialPortType.Ecu);
        };

        try {
            assertEquals(Collections.singletonList(new PortResult("STUCK", SerialPortType.Unknown)),
                SerialPortScanner.inspectPorts(Collections.singletonList("STUCK"), tracked, inspector));
            Thread.interrupted();

            List<PortResult> second = SerialPortScanner.inspectPorts(asList("STUCK", "HEALTHY"), tracked, inspector);
            Thread.interrupted();
            assertTrue(second.contains(new PortResult("STUCK", SerialPortType.Unknown)));
            assertTrue(second.contains(new PortResult("HEALTHY", SerialPortType.Ecu)));
            assertEquals(1, healthyCalls.get(), "a blocked port must not prevent detection of other ports");
            assertEquals(1, stuckCalls.get(), "a timed-out probe must not be repeated while its thread is alive");

            // An OS node disappearing and reappearing must not bypass the running probe.
            assertTrue(SerialPortScanner.inspectPorts(Collections.emptyList(), tracked, inspector).isEmpty());
            long before = System.nanoTime();
            assertEquals(Collections.singletonList(new PortResult("STUCK", SerialPortType.Unknown)),
                SerialPortScanner.inspectPorts(Collections.singletonList("STUCK"), tracked, inspector));
            assertTrue(System.nanoTime() - before < 4_000_000_000L,
                "a scan with only an already-running probe must not wait for another timeout");
            assertEquals(1, stuckCalls.get(), "reappearing ports must still wait for their earlier probe");
        } finally {
            release.countDown();
            Thread.interrupted();
            for (Thread thread : blockedThreads) {
                thread.join(2000);
                assertFalse(thread.isAlive(), "test must release every blocked probe");
            }
            Thread.interrupted();
        }

        assertEquals(Collections.singletonList(new PortResult("STUCK", SerialPortType.Ecu)),
            SerialPortScanner.inspectPorts(Collections.singletonList("STUCK"), tracked, inspector));
        Thread.interrupted();
        assertEquals(2, stuckCalls.get(), "the port can be detected after its blocked probe exits");
    }
}
