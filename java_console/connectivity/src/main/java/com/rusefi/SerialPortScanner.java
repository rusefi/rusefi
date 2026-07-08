package com.rusefi;

import com.devexperts.logging.Logging;
import com.rusefi.io.LinkManager;
import com.rusefi.maintenance.CalibrationsInfo;
import org.jetbrains.annotations.NotNull;
import org.jetbrains.annotations.Nullable;

import java.util.*;
import java.util.concurrent.*;
import java.util.function.Function;
import java.util.stream.Collectors;

/**
 * We have too many programming / ECU possible states
 * 1) the oldest one: discovery board has on-board St-link debug interface (discovery board second microcontrollers on board which is used to debug the primary one), same if we have a modern ECU with external st-link hooked up. Open question if it's time to remove support of st-link programming since all nicer ECUs do not on-board debug interface. Orthogonal to USB port.
 * 2) stm32 build-in bootloader, aka "DFU", regardless if PROG/BOOT0 button or software reboot into DFU
 * 3) live ECU with software reboot into DFU, aka "Auto DFU", not suitable for obfuscated firmware
 * 4) OpenBLT USB device, ala "Manual OpenBLT", regardless if primary firmware has not been uploaded yet or software reboot into OpenBLT
 * 5) live ECU compiled with OpenBLT support
 * <p>
 * This class is the scan *policy* — caching, Unknown-retry, listener notification, device-probe
 * throttling, suspend choreography. The actual hardware/OS probing is injected via
 * {@link HardwareProbes}; the production implementation ({@code EcuHardwareProbes} in the ui
 * module) depends on the flashing tools and the binary protocol.
 *
 * @author Andrey Belomutskiy
 */
public class SerialPortScanner implements PortScanner {
    private final static Logging log = Logging.getLogging(SerialPortScanner.class);

    private static final boolean SHOW_SOCKETCAN = FileLog.isLinux();

    /**
     * The hardware/OS probes the scan loop performs, separated from the scan *policy* (caching,
     * Unknown-retry, listener notification, suspend/probe-throttle choreography) so the policy can
     * be unit tested with scripted probe results. {@code EcuHardwareProbes} (ui module) is the real
     * implementation. See docs/java-connectivity-context-review.md.
     */
    public interface HardwareProbes {
        Set<String> listSerialPorts();

        /** @return detected port type, or null for a dead/stale OS node that must be dropped entirely */
        @Nullable
        PortResult inspectPort(String serialPort);

        Collection<String> listTcpPorts();

        Optional<CalibrationsInfo> getEcuCalibrations(String tcpPort);

        boolean isLiveEcuConnected();

        boolean isDfuDeviceConnected();

        boolean isStLinkConnected();

        boolean isPcanConnected();

        /** Clock used for the device-probe throttle; injectable so tests can script time. */
        long now();
    }

    private final HardwareProbes probes;
    // Tests construct a scanner that never starts the background scan thread and drive
    // findAllAvailablePorts directly instead.
    private final boolean startTimerOnFirstListener;
    private final RecurringStep portsScanner;

    public SerialPortScanner(HardwareProbes probes, boolean startTimerOnFirstListener) {
        this.probes = probes;
        this.startTimerOnFirstListener = startTimerOnFirstListener;
        this.portsScanner = new RecurringStep(
            () -> findAllAvailablePorts(false),
            () -> findAllAvailablePorts(true),
            "Ports Scanner"
        );
    }

    private final Object lock = new Object();
    @NotNull
    private AvailableHardware knownHardware = new AvailableHardware(Collections.emptyList(), false, false, false);

    private final List<PortScanner.Listener> listeners = new CopyOnWriteArrayList<>();

    @Override
    public AvailableHardware getCurrentHardware() {
        synchronized (lock) {
            return knownHardware;
        }
    }

    @Override
    public void addListener(PortScanner.Listener listener) {
        boolean shouldStart = listeners.isEmpty();
        listeners.add(listener);
        if (shouldStart && startTimerOnFirstListener)
            startTimer();
    }

    // Track in-flight probe threads so suspend() can interrupt them and wait
    // for them to release the port before a flash job proceeds.
    // [tag:better_ux_for_flashing]
    private final List<Thread> probeThreads = Collections.synchronizedList(new ArrayList<>());

    /**
     * Probe every port concurrently (one thread each, bounded by a shared timeout) and collect the
     * results. Dead ports (null from the inspector) are dropped; ports that time out are reported
     * as Unknown.
     */
    static List<PortResult> inspectPorts(final List<String> ports, final List<Thread> probeThreadsRef,
                                         final Function<String, PortResult> inspector) {
        if (ports.isEmpty()) {
            return new ArrayList<>();
        }

        final Object resultsLock = new Object();
        final Map<String, PortResult> results = new HashMap<>();

        // When the last port is found, we need to cancel the timeout
        final Thread callingThread = Thread.currentThread();

        // One thread per port to check
        final List<Thread> threads = ports.stream().map(p -> {
            final String threadName = "SerialPortScanner inspectPort " + p;

            Thread t = new Thread(() -> {
                log.trace(String.format("Thread `%s` is starting...", threadName));
                PortResult r;
                try {
                    r = inspector.apply(p);
                } catch (Throwable e) {
                    // Never let a probe exception kill the inspect thread.
                    // If inspectPort already returned null (dead port), keep it null.
                    // Otherwise treat as Unknown. [tag:better_ux_for_flashing]
                    log.warn("inspectPort crashed for " + p + ", treating as Unknown: " + e);
                    r = new PortResult(p, SerialPortType.Unknown);
                }

                // Record the result under lock
                synchronized (resultsLock) {
                    if (Thread.currentThread().isInterrupted()) {
                        log.trace(String.format("Thread `%s` is interrupted.", threadName));
                        return;
                    }

                    results.put(p, r);

                    if (results.size() == ports.size()) {
                        callingThread.interrupt();
                    }
                }
                log.trace(String.format("Thread `%s` has finished.", threadName));
            });

            t.setName(threadName);
            t.setDaemon(true);
            t.start();

            if (probeThreadsRef != null) {
                probeThreadsRef.add(t);
            }

            return t;
        }).collect(Collectors.toList());

        try {
            Thread.sleep(5000);
        } catch (InterruptedException e) {
            // We got interrupted because the last port got found, nothing to do
        }

        synchronized (resultsLock) {
            ScannerHelper.interruptThreads(threads);
        }

        // Timed-out ports that don't have a result yet get Unknown;
        // null results (dead ports) are left as-is so they get filtered out.
        for (String port : ports) {
            if (!results.containsKey(port)) {
                log.info("Port " + port + " timed out, adding as Unknown.");
                results.put(port, new PortResult(port, SerialPortType.Unknown));
            }
        }

        // Clean up finished threads from the tracking list
        if (probeThreadsRef != null) {
            probeThreadsRef.removeIf(t -> !t.isAlive());
        }

        return results.values().stream()
            .filter(Objects::nonNull)
            .collect(Collectors.toList());
    }

    private final SerialPortCache portCache = new SerialPortCache();

    // The DFU/STLink/PCAN device probes shell out to PowerShell (~800ms each). Running all three
    // every 300ms scan cycle starves the ECU re-detection this same thread performs after a reboot,
    // which is what makes the reconnect "loading" state drag on. Throttle them to a few seconds and
    // skip them entirely while a live ECU is connected (a board cannot be a connected ECU and a DFU
    // device at the same time, and the scan thread must stay responsive for the reconnect). The
    // last-known results are reused between probes so the ProgramSelector menu stays stable.
    // [tag:better_ux_for_flashing]
    private static final long DEVICE_PROBE_INTERVAL_MS = 3000;
    // Accessed only from the single "Ports Scanner" thread.
    private long lastDeviceProbeMs = 0;
    private boolean lastDfuConnected = false;
    private boolean lastStLinkConnected = false;
    private boolean lastPcanConnected = false;

    /**
     * Find all available serial ports and checks if simulator local TCP port is available.
     * Package-private so scan-policy unit tests can drive scan cycles directly with scripted probes.
     */
    void findAllAvailablePorts(boolean includeSlowLookup) {
        List<PortResult> ports = new ArrayList<>();
        boolean dfuConnected;
        boolean stLinkConnected;
        boolean PCANConnected;

        // ttyS* are legacy motherboard UARTs on Linux — never a rusEFI ECU and they stall
        // the binary protocol handshake for seconds.  Drop them before the scan pipeline.
        final Set<String> serialPorts = probes.listSerialPorts().stream()
            .filter(name -> !name.startsWith("ttyS"))
            .collect(Collectors.toCollection(TreeSet::new));
        log.info("getCommPorts (filtered): " + serialPorts);

        List<String> portsToInspect = new ArrayList<>();

        for (String serialPort : serialPorts) {
            // First, check the port cache
            final Optional<PortResult> cachedPort = portCache.get(serialPort);
            if (cachedPort.isPresent()) {
                ports.add(cachedPort.get());
            } else {
                portsToInspect.add(serialPort);
            }
        }

        for (PortResult p : inspectPorts(portsToInspect, probeThreads, probes::inspectPort)) {
            log.info("Port " + p.port + " detected as: " + p.type.friendlyString);
            ports.add(p);
            // Do not cache Unknown — keep the port uninspected so the next scan cycle retries
            // detection automatically without waiting for the port to disappear and reappear.
            if (p.type != SerialPortType.Unknown) {
                portCache.put(p);
            }
        }

        final Collection<String> tcpPorts = includeSlowLookup
            ? probes.listTcpPorts()
            : Collections.emptyList();

        final Set<String> livePortNames = new HashSet<>(serialPorts);
        livePortNames.addAll(tcpPorts);
        portCache.retainAll(livePortNames);

        // Sort ports by their type to put your ECU at the top
        ports.sort(Comparator.comparingInt(a -> a.type.sortOrder));

        if (includeSlowLookup) {
            for (String tcpPort : tcpPorts) {
                final Optional<PortResult> cachedPort = portCache.get(tcpPort);
                if (cachedPort.isPresent()) {
                    ports.add(cachedPort.get());
                } else {
                    final Optional<CalibrationsInfo> tcpCalibrations = probes.getEcuCalibrations(tcpPort);
                    final PortResult tcpResult = tcpCalibrations
                        .map(c -> new PortResult(tcpPort, SerialPortType.Ecu, c))
                        .orElseGet(() -> new PortResult(tcpPort, SerialPortType.Unknown));
                    ports.add(tcpResult);

                    // cache port + calibrations
                    if (tcpCalibrations.isPresent()) {
                        portCache.put(tcpResult);
                    }
                }
            }
            // Skip the expensive OS device probes while a live ECU is connected, and otherwise run them
            // no more than once per DEVICE_PROBE_INTERVAL_MS. Reuse the last-known values in between so
            // the update menu doesn't flicker. [tag:better_ux_for_flashing]
            final boolean liveEcuConnected = probes.isLiveEcuConnected();
            final long now = probes.now();
            if (!liveEcuConnected && (now - lastDeviceProbeMs) >= DEVICE_PROBE_INTERVAL_MS) {
                lastDfuConnected = probes.isDfuDeviceConnected();
                lastStLinkConnected = probes.isStLinkConnected();
                lastPcanConnected = probes.isPcanConnected();
                lastDeviceProbeMs = now;
            }
            dfuConnected = lastDfuConnected;
            stLinkConnected = lastStLinkConnected;
            PCANConnected = lastPcanConnected;
        } else {
            dfuConnected = false;
            stLinkConnected = false;
            PCANConnected = false;
        }
/*
        if (PCANConnected)
            ports.add(new PortResult(LinkManager.PCAN, SerialPortType.CAN));
 */
/*
        if (SHOW_SOCKETCAN)
            ports.add(new PortResult(LinkManager.SOCKET_CAN, SerialPortType.CAN));
*/
        // Surface a DFU device (STM32 built-in bootloader) as a synthetic, non-connectable port so a
        // running console can offer DFU flashing in-session [tag:better_ux_for_flashing]. dfuConnected stays exposed via
        // AvailableHardware.isDfuFound() for the existing ProgramSelector menu logic.
        if (dfuConnected) {
            ports.add(new PortResult(LinkManager.DFU, SerialPortType.Dfu));
        }
        boolean isListUpdated;
        AvailableHardware currentHardware = new AvailableHardware(ports, dfuConnected, stLinkConnected, PCANConnected);
        synchronized (lock) {
            isListUpdated = !knownHardware.equals(currentHardware);
            knownHardware = currentHardware;
        }
        if (isListUpdated) {
            for (PortScanner.Listener listener : listeners)
                listener.onChange(currentHardware);
        }
    }

    private void startTimer() {
        portsScanner.start();
    }

    public void stopTimer() {
        portsScanner.stop();
    }

    @Override
    public void restartTimer() {
        portsScanner.restart();
    }

    @Override
    public void resume() {
        portsScanner.resume();
    }

    /**
     * Suspend the scanner and wait for all in-flight probe threads to finish,
     * ensuring they release any open port handles before a flash job proceeds.
     * [tag:better_ux_for_flashing]
     */
    @Override
    public CountDownLatch suspend() {
        CountDownLatch latch = portsScanner.suspend();
        // Interrupt all in-flight probe threads so they close their port handles
        synchronized (probeThreads) {
            for (Thread t : probeThreads) {
                if (t.isAlive()) {
                    t.interrupt();
                }
            }
        }
        // Wait for probe threads to finish (max 10 seconds)
        long deadline = System.currentTimeMillis() + 10_000;
        while (System.currentTimeMillis() < deadline) {
            boolean allDone = true;
            synchronized (probeThreads) {
                for (Thread t : probeThreads) {
                    if (t.isAlive()) {
                        allDone = false;
                        break;
                    }
                }
            }
            if (allDone) {
                break;
            }
            try {
                Thread.sleep(100);
            } catch (InterruptedException e) {
                Thread.currentThread().interrupt();
                break;
            }
        }
        return latch;
    }

    /**
     * Pre-populate the port cache with a known result so the scanner does not
     * re-inspect an actively-used port on the next scan cycle.  Call this
     * before resuming the scanner after a reconnect to prevent the scanner from
     * opening the port and competing with an already-established BinaryProtocol
     * connection on the same serial stream.
     */
    @Override
    public void cachePort(PortResult port) {
        portCache.put(port);
    }

    /**
     * Remove a port from the cache so the scanner re-inspects it on the next
     * scan cycle.  Call this before sending a reboot-to-OpenBLT command so that
     * the scanner does not keep reporting the port as {@code EcuWithOpenblt}
     * after the ECU has already transitioned to OpenBLT mode.
     */
    @Override
    public void invalidatePort(String portName) {
        portCache.invalidate(portName);
    }
}
