package com.rusefi.io.can.slcan;

import com.devexperts.logging.Logging;
import com.rusefi.autodetect.SerialAutoChecker;
import com.rusefi.io.IoStream;
import com.rusefi.io.LinkManager;
import com.rusefi.io.serial.BufferedSerialIoStream;

import java.io.EOFException;
import java.io.IOException;
import java.nio.charset.StandardCharsets;
import java.util.ArrayList;
import java.util.Collections;
import java.util.Comparator;
import java.util.Date;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.Objects;
import java.util.Set;
import java.util.TreeSet;
import java.util.concurrent.CountDownLatch;
import java.util.concurrent.TimeUnit;
import java.util.concurrent.atomic.AtomicBoolean;
import java.util.function.Consumer;
import java.util.function.Function;
import java.util.stream.Collectors;

/**
 * Stand-alone scanner which continuously classifies serial ports as SLCAN CAN sniffer VCPs
 * (see firmware/controllers/can/can_sniffer.md), rusEFI TunerStudio console VCPs, or neither.
 * Doubles as a manual sandbox via {@link #main}.
 * <p>
 * Follows the general idea of {@code com.rusefi.SerialPortScanner} (connectivity module) but is
 * intentionally independent of it — the io module cannot see connectivity, and the SLCAN use case
 * needs none of the DFU/ST-Link/flashing choreography. The two ideas borrowed from it:
 * <ul>
 * <li>parallel per-port probe fan-out with a shared timeout: a port whose earlier probe is still
 * alive (a blocked native serial open) is reported as {@link Type#UNKNOWN} without starting
 * another thread, a crashing probe degrades to UNKNOWN instead of killing the scan, and a dead
 * OS node (probe returns null) is dropped entirely;</li>
 * <li>scan policy: positively classified ports are cached and not reopened every cycle, UNKNOWN
 * is retried automatically, cache entries for disappeared ports are forgotten, and the listener
 * only fires when the port list actually changes.</li>
 * </ul>
 * The probe itself is passive: TunerStudio HELLO first (both to skip the primary console VCP and
 * to identify it), then the SLCAN 'V' version command — it never sends 'S'/'O', so it does not
 * open the streaming channel on the sniffer port it finds. A port which is already streaming
 * frames from a stale session is recognized as SLCAN by its frame lines.
 *
 * @see SlcanClient
 * @see com.rusefi.binaryprotocol.test.SlcanSandbox
 */
public class SlcanPortScanner {
    private static final Logging log = Logging.getLogging(SlcanPortScanner.class);

    private static final int SCAN_PERIOD_MS = 1000;
    private static final int PROBE_FANOUT_TIMEOUT_MS = 5000;
    private static final int SLCAN_RESPONSE_TIMEOUT_MS = 700;
    private static final char CR = '\r';
    private static final char BELL = 7;

    public enum Type {
        SLCAN,
        TS_CONSOLE,
        NOT_SLCAN,
        /** Failed/timed-out/in-flight probe: not cached, retried on the next cycle. */
        UNKNOWN,
    }

    /** One classified port. {@code detail} is the SLCAN version response / TS signature / diagnostic. */
    public static class Result {
        public final String port;
        public final Type type;
        public final String detail;

        public Result(String port, Type type, String detail) {
            this.port = port;
            this.type = type;
            this.detail = detail;
        }

        @Override
        public boolean equals(Object o) {
            if (!(o instanceof Result)) {
                return false;
            }
            Result other = (Result) o;
            return port.equals(other.port) && type == other.type && Objects.equals(detail, other.detail);
        }

        @Override
        public int hashCode() {
            return Objects.hash(port, type, detail);
        }

        @Override
        public String toString() {
            return port + ": " + type + (detail == null ? "" : " [" + detail + "]");
        }
    }

    /**
     * The hardware probes the scan loop performs, separated from the scan policy so the policy
     * can be unit tested with scripted results — same seam idea as
     * {@code SerialPortScanner.HardwareProbes}.
     */
    public interface Probes {
        Set<String> listSerialPorts();

        /** @return classification, or null for a dead/stale OS node that must be dropped entirely */
        Result inspectPort(String serialPort);

        Probes REAL = new Probes() {
            @Override
            public Set<String> listSerialPorts() {
                return LinkManager.getCommPorts();
            }

            @Override
            public Result inspectPort(String serialPort) {
                return realInspect(serialPort);
            }
        };
    }

    private final Probes probes;
    private final Consumer<List<Result>> listener;
    // Keep timed-out threads keyed by port until they exit: native serial opens may ignore
    // interruption, and we must not pile a second probe onto the same stuck port.
    private final Map<String, Thread> probeThreads = new HashMap<>();
    // Positively classified ports are not reopened every cycle; only touched on the scan thread.
    private final Map<String, Result> cache = new HashMap<>();

    private final Object lock = new Object();
    private List<Result> knownPorts = Collections.emptyList();

    public SlcanPortScanner(Probes probes, Consumer<List<Result>> listener) {
        this.probes = probes;
        this.listener = listener;
    }

    public List<Result> getKnownPorts() {
        synchronized (lock) {
            return knownPorts;
        }
    }

    /** Starts the background scan loop; tests drive {@link #scanCycle()} directly instead. */
    public void start() {
        Thread thread = new Thread(() -> {
            while (!Thread.currentThread().isInterrupted()) {
                try {
                    scanCycle();
                } catch (Throwable e) {
                    log.warn("scan cycle failed: " + e);
                }
                try {
                    Thread.sleep(SCAN_PERIOD_MS);
                } catch (InterruptedException e) {
                    return;
                }
            }
        }, "SLCAN Ports Scanner");
        thread.setDaemon(true);
        thread.start();
    }

    void scanCycle() {
        // ttyS* are legacy motherboard UARTs on Linux — never a rusEFI VCP and they stall probes.
        Set<String> serialPorts = probes.listSerialPorts().stream()
            .filter(name -> !name.startsWith("ttyS"))
            .collect(Collectors.toCollection(TreeSet::new));

        List<Result> ports = new ArrayList<>();
        List<String> portsToInspect = new ArrayList<>();
        for (String serialPort : serialPorts) {
            Result cached = cache.get(serialPort);
            if (cached != null) {
                ports.add(cached);
            } else {
                portsToInspect.add(serialPort);
            }
        }

        for (Result result : inspectPorts(portsToInspect, probeThreads, probes::inspectPort)) {
            log.info("Port " + result);
            ports.add(result);
            // Do not cache UNKNOWN — keep the port uninspected so the next cycle retries
            // detection automatically without waiting for the port to disappear and reappear.
            if (result.type != Type.UNKNOWN) {
                cache.put(result.port, result);
            }
        }
        cache.keySet().retainAll(serialPorts);

        ports.sort(Comparator.comparing(result -> result.port));
        boolean isListUpdated;
        synchronized (lock) {
            isListUpdated = !ports.equals(knownPorts);
            knownPorts = ports;
        }
        if (isListUpdated) {
            listener.accept(ports);
        }
    }

    /**
     * Probe ports concurrently with a shared timeout. A port with an earlier probe still alive
     * is reported as UNKNOWN without starting another thread. Dead ports (null) are dropped.
     */
    static List<Result> inspectPorts(List<String> ports, Map<String, Thread> probeThreadsRef,
                                     Function<String, Result> inspector) {
        synchronized (probeThreadsRef) {
            probeThreadsRef.values().removeIf(t -> !t.isAlive());
        }
        if (ports.isEmpty()) {
            return new ArrayList<>();
        }

        Object resultsLock = new Object();
        Map<String, Result> results = new HashMap<>();
        AtomicBoolean acceptingResults = new AtomicBoolean(true);
        CountDownLatch completed = new CountDownLatch(ports.size());
        List<Thread> threads = new ArrayList<>();

        for (String port : ports) {
            // Checking, registering and starting must be atomic with respect to other scans.
            synchronized (probeThreadsRef) {
                Thread previous = probeThreadsRef.get(port);
                if (previous != null && previous.isAlive()) {
                    synchronized (resultsLock) {
                        results.put(port, new Result(port, Type.UNKNOWN, "probe still in flight"));
                    }
                    completed.countDown();
                    continue;
                }

                Thread thread = new Thread(() -> {
                    try {
                        Result result;
                        try {
                            result = inspector.apply(port);
                        } catch (Throwable e) {
                            // A failed probe must not kill the scan.
                            log.warn("inspectPort crashed for " + port + ", treating as UNKNOWN: " + e);
                            result = new Result(port, Type.UNKNOWN, "probe crashed: " + e);
                        }
                        synchronized (resultsLock) {
                            // A native call may consume the interrupt before returning. Explicitly
                            // reject late results as well as results from an interrupted probe.
                            if (acceptingResults.get() && !Thread.currentThread().isInterrupted()) {
                                results.put(port, result);
                            }
                        }
                    } finally {
                        synchronized (probeThreadsRef) {
                            probeThreadsRef.remove(port, Thread.currentThread());
                        }
                        completed.countDown();
                    }
                }, "SlcanPortScanner inspectPort " + port);
                thread.setDaemon(true);
                probeThreadsRef.put(port, thread);
                thread.start();
                threads.add(thread);
            }
        }

        try {
            completed.await(PROBE_FANOUT_TIMEOUT_MS, TimeUnit.MILLISECONDS);
        } catch (InterruptedException e) {
            Thread.currentThread().interrupt();
        }

        List<Result> scanResults;
        synchronized (resultsLock) {
            acceptingResults.set(false);
            for (Thread thread : threads) {
                if (thread.isAlive()) {
                    thread.interrupt();
                }
            }
            for (String port : ports) {
                if (!results.containsKey(port)) {
                    log.info("Port " + port + " timed out, adding as UNKNOWN.");
                    results.put(port, new Result(port, Type.UNKNOWN, "probe timed out"));
                }
            }
            scanResults = results.values().stream()
                .filter(Objects::nonNull)
                .collect(Collectors.toList());
        }

        synchronized (probeThreadsRef) {
            probeThreadsRef.values().removeIf(t -> !t.isAlive());
        }
        return scanResults;
    }

    /**
     * Passive classification of one port: TunerStudio HELLO first (the primary console VCP answers
     * with a rusEFI signature), then the SLCAN 'V' version command. Never opens the SLCAN channel.
     */
    static Result realInspect(String port) {
        IoStream stream = BufferedSerialIoStream.openPort(port);
        if (stream == null) {
            // dead/busy OS node — same policy as SerialPortScanner: drop it entirely
            return null;
        }
        try {
            String signature = SerialAutoChecker.checkResponse(stream, null);
            if (signature != null) {
                return new Result(port, Type.TS_CONSOLE, signature);
            }
            // the TS HELLO probe above may have confused the SLCAN parser, drain leftovers
            stream.getDataBuffer().dropPending();
            stream.write(("V" + CR).getBytes(StandardCharsets.US_ASCII));
            stream.flush();
            String response = readLine(stream, SLCAN_RESPONSE_TIMEOUT_MS);
            if (response == null) {
                return new Result(port, Type.UNKNOWN, "no response to V probe");
            }
            // A stale open session may stream frame lines instead of the V response — still SLCAN.
            if ((!response.isEmpty() && response.charAt(0) == 'V') || SlcanClient.Frame.parse(response) != null) {
                return new Result(port, Type.SLCAN, response);
            }
            return new Result(port, Type.NOT_SLCAN, SlcanClient.printable(response));
        } catch (IOException e) {
            return new Result(port, Type.UNKNOWN, "IO error: " + e);
        } finally {
            stream.close();
        }
    }

    private static String readLine(IoStream stream, int timeoutMs) throws IOException {
        StringBuilder line = new StringBuilder();
        while (true) {
            byte b;
            try {
                b = stream.getDataBuffer().readByte(timeoutMs);
            } catch (EOFException timeout) {
                return null;
            }
            if (b == BELL) {
                return String.valueOf(BELL);
            }
            if (b == CR) {
                return line.toString();
            }
            line.append((char) (b & 0xFF));
        }
    }

    public static void main(String[] args) throws InterruptedException {
        SlcanPortScanner scanner = new SlcanPortScanner(Probes.REAL, ports -> {
            System.out.println("=== " + new Date());
            if (ports.isEmpty()) {
                System.out.println("  (no serial ports)");
            }
            for (Result result : ports) {
                System.out.println("  " + result);
            }
        });
        scanner.start();
        Thread.sleep(Long.MAX_VALUE);
    }
}
