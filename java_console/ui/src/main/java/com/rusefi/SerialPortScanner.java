package com.rusefi;

import com.devexperts.logging.Logging;
import com.rusefi.io.ConnectionStatusLogic;
import com.rusefi.io.IoStream;
import com.rusefi.io.LinkManager;
import com.rusefi.io.serial.BufferedSerialIoStream;
import com.rusefi.io.tcp.TcpConnector;
import com.rusefi.maintenance.*;
import com.rusefi.io.UpdateOperationCallbacks;
import com.rusefi.updater.OpenbltDetectorStrategy;
import com.rusefi.util.CompatibilityOptional;
import org.jetbrains.annotations.NotNull;

import java.io.IOException;
import java.util.*;
import java.util.concurrent.*;
import java.util.stream.Collectors;

/**
 * We have too many programming / ECU possible states
 * 1) the oldest one: discovery board has on-board St-link debug interface (discovery board second microcontrollers on board which is used to debug the primary one), same if we have a modern ECU with external st-link hooked up. Open question if it's time to remove support of st-link programming since all nicer ECUs do not on-board debug interface. Orthogonal to USB port.
 * 2) stm32 build-in bootloader, aka "DFU", regardless if PROG/BOOT0 button or software reboot into DFU
 * 3) live ECU with software reboot into DFU, aka "Auto DFU", not suitable for obfuscated firmware
 * 4) OpenBLT USB device, ala "Manual OpenBLT", regardless if primary firmware has not been uploaded yet or software reboot into OpenBLT
 * 5) live ECU compiled with OpenBLT support
 *
 * technical debt: this class has some *serial* use-cases scanning and some not serial scanning: DFU and st-link
 *
 * @author Andrey Belomutskiy
 */
public enum SerialPortScanner implements PortScanner {
    INSTANCE;

    private final static Logging log = Logging.getLogging(SerialPortScanner.class);

    private static final boolean SHOW_SOCKETCAN = FileLog.isLinux();

    private final RecurringStep portsScanner;

    SerialPortScanner() {
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
        if (shouldStart)
            startTimer();
    }

    // Number of ECU-detection attempts before giving up and labelling a port Unknown.
    // A freshly rebooted or reconnected ECU may not respond on the first try.
    private static final int DETECT_MAX_ATTEMPTS = 3;

    /**
     * Determines the type of a serial port: OpenBLT bootloader, ECU (with or without OpenBLT), or Unknown.
     * Unlike {@link com.rusefi.autodetect.PortDetector#autoDetectSerial} which only finds ECUs,
     * this method also recognises OpenBLT bootloaders via the XCP protocol probe.
     */
    static PortResult inspectPort(String serialPort) {
        log.info("Determining type of serial port: " + serialPort);

        boolean isOpenblt;
        try {
            isOpenblt = isPortOpenblt(serialPort);
        } catch (com.fazecast.jSerialComm.SerialPortInvalidPortException e) {
            // Return null so inspectPorts filters them out entirely. [tag:better_ux_for_flashing]
            log.info("Port " + serialPort + " is not actually available (stale OS node): " + e.getMessage());
            return null;
        }
        log.info("Port " + serialPort + (isOpenblt ? " looks like" : " does not look like") + " an OpenBLT bootloader");
        if (isOpenblt) {
            return new PortResult(serialPort, SerialPortType.OpenBlt);
        }

        for (int attempt = 1; attempt <= DETECT_MAX_ATTEMPTS; attempt++) {
            final Optional<CalibrationsInfo> ecuCalibrations;
            try {
                ecuCalibrations = getEcuCalibrations(serialPort);
            } catch (com.fazecast.jSerialComm.SerialPortInvalidPortException e) {
                log.info("Port " + serialPort + " vanished during ECU detection: " + e.getMessage());
                return null;
            }
            final boolean isEcu = ecuCalibrations.isPresent();
            log.info("Port " + serialPort + " ECU detection attempt " + attempt + "/" + DETECT_MAX_ATTEMPTS
                + ": " + (isEcu ? "found" : "not found"));
            if (isEcu) {
                final boolean hasOpenblt = ecuHasOpenblt(serialPort);
                log.info("ECU at " + serialPort + (hasOpenblt ? " has" : " does not have") + " an OpenBLT bootloader");
                return new PortResult(
                    serialPort,
                    hasOpenblt ? SerialPortType.EcuWithOpenblt : SerialPortType.Ecu,
                    ecuCalibrations.get()
                );
            }
            if (attempt < DETECT_MAX_ATTEMPTS) {
                try {
                    Thread.sleep(200);
                } catch (InterruptedException e) {
                    Thread.currentThread().interrupt();
                    break;
                }
            }
        }

        log.info("Port " + serialPort + " does not look like an ECU after " + DETECT_MAX_ATTEMPTS + " attempts");
        return new PortResult(serialPort, SerialPortType.Unknown);
    }

    // Track in-flight probe threads so suspend() can interrupt them and wait
    // for them to release the port before a flash job proceeds.
    // [tag:better_ux_for_flashing]
    private final List<Thread> probeThreads = Collections.synchronizedList(new ArrayList<>());

    static List<PortResult> inspectPorts(final List<String> ports, final List<Thread> probeThreadsRef) {
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
                    r = inspectPort(p);
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
     * Find all available serial ports and checks if simulator local TCP port is available
     */
    private void findAllAvailablePorts(boolean includeSlowLookup) {
        List<PortResult> ports = new ArrayList<>();
        boolean dfuConnected;
        boolean stLinkConnected;
        boolean PCANConnected;

        // ttyS* are legacy motherboard UARTs on Linux — never a rusEFI ECU and they stall
        // the binary protocol handshake for seconds.  Drop them before the scan pipeline.
        final Set<String> serialPorts = LinkManager.getCommPorts().stream()
            .filter(name -> !name.startsWith("ttyS"))
            .collect(Collectors.toCollection(TreeSet::new));
        log.info("getCommPorts (filtered): " + serialPorts);

        List<String> portsToInspect = new ArrayList<>();

        for (String serialPort : serialPorts) {
            // First, check the port cache
            final Optional<PortResult> cachedPort = portCache.get(serialPort);
            CompatibilityOptional.ifPresentOrElse(cachedPort, ports::add, () -> portsToInspect.add(serialPort));
        }

        for (PortResult p : inspectPorts(portsToInspect, probeThreads)) {
            log.info("Port " + p.port + " detected as: " + p.type.friendlyString);
            ports.add(p);
            // Do not cache Unknown — keep the port uninspected so the next scan cycle retries
            // detection automatically without waiting for the port to disappear and reappear.
            if (p.type != SerialPortType.Unknown) {
                portCache.put(p);
            }
        }

        final Collection<String> tcpPorts = includeSlowLookup
            ? TcpConnector.getAvailablePorts()
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
                    final Optional<CalibrationsInfo> tcpCalibrations = getEcuCalibrations(tcpPort);
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
            final boolean liveEcuConnected = ConnectionStatusLogic.INSTANCE.isConnected();
            final long now = System.currentTimeMillis();
            if (!liveEcuConnected && (now - lastDeviceProbeMs) >= DEVICE_PROBE_INTERVAL_MS) {
                lastDfuConnected = DfuFlasher.detectSTM32BootloaderDriverState(UpdateOperationCallbacks.DUMMY);
                lastStLinkConnected = StLinkFlasher.detectStLink(UpdateOperationCallbacks.DUMMY);
                lastPcanConnected = MaintenanceUtil.detectPcan(UpdateOperationCallbacks.DUMMY);
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

    private static Optional<CalibrationsInfo> getEcuCalibrations(final String port) {
        log.info("getEcuCalibrations " + port);
        return CalibrationsHelper.readCurrentCalibrationsWithoutSuspendingPortScanner(
            port,
            UpdateOperationCallbacks.LOGGER
        );
    }

    private static boolean ecuHasOpenblt(String port) {
        try (IoStream stream = BufferedSerialIoStream.openPort(port)) {
            if (stream == null) {
                return false;
            }

            return OpenbltDetectorStrategy.streamHasOpenBlt(stream);
        } catch (Exception e) {
            return false;
        }
    }

    private static boolean isPortOpenblt(String port) {
        try (IoStream stream = BufferedSerialIoStream.openPort(port)) {
            return OpenbltDetectorStrategy.isPortOpenblt(stream);
        } catch (com.fazecast.jSerialComm.SerialPortInvalidPortException e) {
            // Port is truly dead — let it propagate so inspectPort returns null.
            throw e;
        } catch (Exception e) {
            log.info("isPortOpenblt probe failed for " + port + ": " + e);
            return false;
        }
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
