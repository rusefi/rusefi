package com.rusefi;

import com.devexperts.logging.Logging;
import com.rusefi.autodetect.SerialAutoChecker;
import com.rusefi.binaryprotocol.IncomingDataBuffer;
import com.rusefi.binaryprotocol.IoHelper;
import com.rusefi.config.generated.Fields;
import com.rusefi.core.RusEfiSignature;
import com.rusefi.core.SignatureHelper;
import com.rusefi.io.IoStream;
import com.rusefi.io.LinkManager;
import com.rusefi.io.UpdateOperationCallbacks;
import com.rusefi.io.serial.BufferedSerialIoStream;
import com.rusefi.io.tcp.TcpConnector;
import com.rusefi.maintenance.DfuFlasher;
import org.jetbrains.annotations.NotNull;

import java.io.IOException;
import java.util.*;
import java.util.concurrent.CopyOnWriteArrayList;
import java.util.stream.Collectors;

/**
 * @author Andrey Belomutskiy
 */
public enum SerialPortScanner {
    INSTANCE;

    private final static Logging log = Logging.getLogging(SerialPortScanner.class);

    public enum SerialPortType {
        FomeEcu("FOME ECU", 20),
        FomeEcuWithOpenblt("FOME ECU w/ BL", 20),
        OpenBlt("OpenBLT Bootloader", 10),
        Unknown("Unknown", 100),
        ;

        public final String friendlyString;
        public final int sortOrder;
        SerialPortType(String friendlyString, int sortOrder) {
            this.friendlyString = friendlyString;
            this.sortOrder = sortOrder;
        }
    }

    public static class PortResult {
        public final String port;
        public final SerialPortType type;
        public final RusEfiSignature signature;

        public PortResult(String port, SerialPortType type, String signature) {
            this.port = port;
            this.type = type;
            this.signature = SignatureHelper.parse(signature);
        }

        public PortResult(String port, SerialPortType type) {
            this(port, type, null);
        }

        @Override
        public String toString() {
            if (type.friendlyString == null) {
                return this.port;
            } else {
                return this.port + " (" + type.friendlyString + ")";
            }
        }

        @Override
        public boolean equals(Object o) {
            if (o == this) {
                return true;
            }

            if (o == null) {
                return false;
            }

            if (getClass() != o.getClass()) {
                return false;
            }

            PortResult other = (PortResult) o;

            return this.port.equals(other.port) && this.type.equals(other.type);
        }

        public boolean isEcu() {
            return type == SerialPortType.FomeEcu || type == SerialPortType.FomeEcuWithOpenblt;
        }
    }

    private volatile boolean isRunning = false;

    private final Object lock = new Object();
    private AvailableHardware knownHardware = null;

    private final List<Listener> listeners = new CopyOnWriteArrayList<>();

    public void addListener(Listener listener) {
        boolean shouldStart = listeners.isEmpty();
        listeners.add(listener);
        if (shouldStart) {
            startTimer();
        }
    }

    private static PortResult inspectPort(String serialPort) {
        log.info("Determining type of serial port: " + serialPort);

        boolean isOpenblt = isPortOpenblt(serialPort);
        log.info("Port " + serialPort + (isOpenblt ? " looks like" : " does not look like") + " an OpenBLT bootloader");
        if (isOpenblt) {
            return new PortResult(serialPort, SerialPortType.OpenBlt);
        } else {
            // See if this looks like an ECU
            String signature = getEcuSignature(serialPort);
            boolean isEcu = signature != null && signature.contains("FOME");
            log.info("Port " + serialPort + (isEcu ? " looks like" : " does not look like") + " a FOME ECU");
            if (isEcu) {
                boolean ecuHasOpenblt = fomeEcuHasOpenblt(serialPort);
                log.info("FOME ECU at " + serialPort + (ecuHasOpenblt ? " has" : " does not have") + " an OpenBLT bootloader");
                return new PortResult(serialPort, ecuHasOpenblt ? SerialPortType.FomeEcuWithOpenblt : SerialPortType.FomeEcu, signature);
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
            Thread t = new Thread(() -> {
                PortResult r = inspectPort(p);

                // Record the result under lock
                synchronized (resultsLock) {
                    if (Thread.currentThread().isInterrupted()) {
                        // If interrupted, don't try to write our result
                        return;
                    }

                    results.put(p, r);

                    if (results.size() == ports.size()) {
                        // We now have all the results - interrupt the calling thread
                        callingThread.interrupt();
                    }
                }
            });

            t.setName("SerialPortScanner inspectPort " + p);
            t.setDaemon(true);
            t.start();

            return t;
        }).collect(Collectors.toList());

        // Give everyone a chance to finish
        try {
            Thread.sleep(5000);
        } catch (InterruptedException e) {
            // We got interrupted because the last port got found, nothing to do
        }

        // Interrupt all threads under lock to ensure no more objects are added to results
        synchronized (resultsLock) {
            for (Thread t : threads) {
                t.interrupt();
            }
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

    private final static Map<String, PortResult> portCache = new HashMap<>();

    /**
     * Find all available serial ports and checks if simulator local TCP port is available
     */
    private void findAllAvailablePorts(boolean includeSlowLookup) {
        List<PortResult> ports = new ArrayList<>();

        String[] serialPorts = LinkManager.getCommPorts();

        List<String> portsToInspect = new ArrayList<>();

        for (String serialPort : serialPorts) {
            // First, check the port cache
            if (portCache.containsKey(serialPort)) {
                // We've already probed this port - don't re-probe it again
                PortResult cached = portCache.get(serialPort);

                ports.add(cached);
            } else {
                portsToInspect.add(serialPort);
            }
        }

        for (PortResult p : inspectPorts(portsToInspect)) {
            log.info("Port " + p.port + " detected as: " + p.type.friendlyString);

            ports.add(p);
            portCache.put(p.port, p);
        }

        {
            // Clean the port cache of any entries that no longer exist
            // If the same port appears later, we want to re-probe it at that time
            // In any other scenario, auto could have unexpected behavior for the user
            List<String> toRemove = new ArrayList<>();
            for (String x : portCache.keySet()) {
                if (Arrays.stream(serialPorts).noneMatch(x::equals)) {
                    toRemove.add(x);
                }
            }

            // two steps to avoid ConcurrentModificationException
            toRemove.forEach(p -> {
                portCache.remove(p);
                log.info("Removing port " + p);
            });
        }

        // Sort ports by their type to put your ECU at the top
        ports.sort(Comparator.comparingInt(a -> a.type.sortOrder));

        if (includeSlowLookup) {
            for (String tcpPort : TcpConnector.getAvailablePorts()) {
                ports.add(new PortResult(tcpPort, SerialPortType.FomeEcu));
            }
        }

        boolean dfuConnected = DfuFlasher.detectSTM32BootloaderDriverState(UpdateOperationCallbacks.DUMMY);

        boolean isListUpdated;
        AvailableHardware currentHardware = new AvailableHardware(ports, dfuConnected);

        synchronized (lock) {
            isListUpdated = !currentHardware.equals(knownHardware);
            knownHardware = currentHardware;
        }

        if (isListUpdated) {
            for (Listener listener : listeners) {
                listener.onChange(currentHardware);
            }
        }
    }

    public void startTimer() {
        isRunning = true;
        Thread portsScanner = new Thread(() -> {
            boolean isFirstTime = true;
            while (isRunning) {
                findAllAvailablePorts(!isFirstTime);
                isFirstTime = false;
                try {
                    //noinspection BusyWait
                    Thread.sleep(300);
                } catch (InterruptedException e) {
                    throw new IllegalStateException(e);
                }
            }

        }, "Ports Scanner");
        portsScanner.setDaemon(true);
        portsScanner.start();
    }

    public void stopTimer() {
        isRunning = false;
    }

    interface Listener {
        void onChange(AvailableHardware currentHardware);
    }

    public static class AvailableHardware {
        private final List<PortResult> ports;
        public final boolean dfuFound;

        public <T> AvailableHardware(List<PortResult> ports, boolean dfuFound) {
            this.ports = ports;
            this.dfuFound = dfuFound;
        }

        @NotNull
        public List<PortResult> getKnownPorts() {return new ArrayList<>(ports);}

        @Override
        public boolean equals(Object o) {
            if (this == o) return true;
            if (o == null || getClass() != o.getClass()) return false;
            AvailableHardware that = (AvailableHardware) o;
            return dfuFound == that.dfuFound && ports.equals(that.ports);
        }

        public boolean isEmpty() {
            return !dfuFound && ports.isEmpty();
        }
    }

    public static String getEcuSignature(String port) {
        try (IoStream stream = BufferedSerialIoStream.openPort(port)) {
            return SerialAutoChecker.checkResponse(stream);
        } catch (Exception e) {
            return null;
        }
    }

    public static boolean fomeEcuHasOpenblt(String port) {
        try (IoStream stream = BufferedSerialIoStream.openPort(port)) {
            if (stream == null) {
                return false;
            }

            stream.sendPacket(new byte[]{(byte) Fields.TS_QUERY_BOOTLOADER});

            byte[] response = stream.getDataBuffer().getPacket(500, "fomeEcuHasOpenblt");
            if (!IoHelper.checkResponseCode(response, (byte) Fields.TS_RESPONSE_OK)) {
                // ECU didn't understand request, bootloader certainly not supported
                return false;
            }

            // Data byte indicates bootloader type
            return response[1] == Fields.TS_QUERY_BOOTLOADER_OPENBLT;
        } catch (Exception e) {
            return false;
        }
    }

    public static boolean isPortOpenblt(String port) {
        try (IoStream stream = BufferedSerialIoStream.openPort(port)) {
            if (stream == null) {
                return false;
            }

            byte[] request = new byte[3];
            request[0] = 2; // packet length
            request[1] = (byte) 0xff; // XCPLOADER_CMD_CONNECT
            request[2] = 0; // connectMode

            stream.write(request);

            IncomingDataBuffer idb = stream.getDataBuffer();

            byte responseLength = idb.readByte(250);

            // Invalid length, ignore
            if (responseLength != 8) {
                return false;
            }

            // Read length worth of bytes
            byte[] response = new byte[responseLength];
            idb.waitForBytes(100, "isPortOpenblt", System.currentTimeMillis(), responseLength);
            idb.read(response);

            // Response packet should start with FF
            // Not much else to check, as the rest of the response is protocol settings from the device.
            return response[0] == (byte) 0xFF;
        } catch (IOException e) {
            return false;
        }
    }
}
