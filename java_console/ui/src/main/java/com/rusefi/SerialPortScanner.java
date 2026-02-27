package com.rusefi;

import com.devexperts.logging.Logging;
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
public enum SerialPortScanner {
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

    private final List<Listener> listeners = new CopyOnWriteArrayList<>();

    public AvailableHardware getCurrentHardware() {
        synchronized (lock) {
            return knownHardware;
        }
    }

    public void addListener(Listener listener) {
        boolean shouldStart = listeners.isEmpty();
        listeners.add(listener);
        if (shouldStart)
            startTimer();
    }

    private static PortResult inspectPort(String serialPort) {
        log.info("Determining type of serial port: " + serialPort);

        boolean isOpenblt = isPortOpenblt(serialPort);
        log.info("Port " + serialPort + (isOpenblt ? " looks like" : " does not look like") + " an OpenBLT bootloader");
        if (isOpenblt) {
            return new PortResult(serialPort, SerialPortType.OpenBlt);
        } else {
            // See if this looks like an ECU
            final Optional<CalibrationsInfo> ecuCalibrations = getEcuCalibrations(serialPort);
            final boolean isEcu = ecuCalibrations.isPresent();
            log.info("Port " + serialPort + (isEcu ? " looks like" : " does not look like") + " an ECU");
            if (isEcu) {
                final boolean ecuHasOpenblt = ecuHasOpenblt(serialPort);
                log.info("ECU at " + serialPort + (ecuHasOpenblt ? " has" : " does not have") + " an OpenBLT bootloader");
                return new PortResult(
                    serialPort,
                    ecuHasOpenblt ? SerialPortType.EcuWithOpenblt : SerialPortType.Ecu,
                    ecuCalibrations.get()
                );
            } else {
                // Dunno what this is, leave it in the list anyway
                return new PortResult(serialPort, SerialPortType.Unknown);
            }
        }
    }

    private static List<PortResult> inspectPorts(final List<String> ports) {
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
                PortResult r = inspectPort(p);

                // Record the result under lock
                synchronized (resultsLock) {
                    if (Thread.currentThread().isInterrupted()) {
                        log.trace(String.format("Thread `%s` is interrupted.", threadName));
                        // If interrupted, don't try to write our result
                        return;
                    }

                    results.put(p, r);

                    if (results.size() == ports.size()) {
                        // We now have all the results - interrupt the calling thread
                        callingThread.interrupt();
                    }
                }
                log.trace(String.format("Thread `%s` has finished.", threadName));
            });

            t.setName(threadName);
            t.setDaemon(true);
            t.start();

            return t;
        }).collect(Collectors.toList());

        // Give everyone a chance to finish
        try {
            // todo: see if everyone has already finished - make this sleep conditional!
            // todo: lower this timeout?
            Thread.sleep(5000);
        } catch (InterruptedException e) {
            // We got interrupted because the last port got found, nothing to do
        }

        // Interrupt all threads under lock to ensure no more objects are added to results
        synchronized (resultsLock) {
            ScannerHelper.interruptThreads(threads);
        }

        // Now check that we got everything - if any timed out, register them as unknown
        for (String port : ports) {
            if (!results.containsKey(port)) {
                log.info("Port " + port + " timed out, adding as Unknown.");
                results.put(port, new PortResult(port, SerialPortType.Unknown));
            }
        }

        return new ArrayList<>(results.values());
    }

    private final SerialPortCache portCache = new SerialPortCache();

    /**
     * Find all available serial ports and checks if simulator local TCP port is available
     */
    private void findAllAvailablePorts(boolean includeSlowLookup) {
        List<PortResult> ports = new ArrayList<>();
        boolean dfuConnected;
        boolean stLinkConnected;
        boolean PCANConnected;

        final Set<String> serialPorts = LinkManager.getCommPorts();
        log.info("getCommPorts: " + serialPorts);

        List<String> portsToInspect = new ArrayList<>();

        for (String serialPort : serialPorts) {
            // First, check the port cache
            final Optional<PortResult> cachedPort = portCache.get(serialPort);
            CompatibilityOptional.ifPresentOrElse(cachedPort, ports::add, () -> portsToInspect.add(serialPort));
        }

        for (PortResult p : inspectPorts(portsToInspect)) {
            log.info("Port " + p.port + " detected as: " + p.type.friendlyString);

            ports.add(p);
            portCache.put(p);
        }

        portCache.retainAll(serialPorts);

        // Sort ports by their type to put your ECU at the top
        ports.sort(Comparator.comparingInt(a -> a.type.sortOrder));

        if (includeSlowLookup) {
            for (String tcpPort : TcpConnector.getAvailablePorts()) {
                ports.add(new PortResult(tcpPort, SerialPortType.Ecu));
            }
            dfuConnected = DfuFlasher.detectSTM32BootloaderDriverState(UpdateOperationCallbacks.DUMMY);
            stLinkConnected = StLinkFlasher.detectStLink(UpdateOperationCallbacks.DUMMY);
            PCANConnected = MaintenanceUtil.detectPcan(UpdateOperationCallbacks.DUMMY);
        } else {
            dfuConnected = false;
            stLinkConnected = false;
            PCANConnected = false;
        }
        if (PCANConnected)
            ports.add(new PortResult(LinkManager.PCAN, SerialPortType.CAN));
/*
        if (SHOW_SOCKETCAN)
            ports.add(new PortResult(LinkManager.SOCKET_CAN, SerialPortType.CAN));
*/
        boolean isListUpdated;
        AvailableHardware currentHardware = new AvailableHardware(ports, dfuConnected, stLinkConnected, PCANConnected);
        synchronized (lock) {
            isListUpdated = !knownHardware.equals(currentHardware);
            knownHardware = currentHardware;
        }
        if (isListUpdated) {
            for (Listener listener : listeners)
                listener.onChange(currentHardware);
        }
    }

    private void startTimer() {
        portsScanner.start();
    }

    public void stopTimer() {
        portsScanner.stop();
    }

    public interface Listener {
        void onChange(AvailableHardware currentHardware);
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
        } catch (IOException e) {
            return false;
        }
    }

    public void resume() {
        portsScanner.resume();
    }

    public CountDownLatch suspend() {
        return portsScanner.suspend();
    }
}
