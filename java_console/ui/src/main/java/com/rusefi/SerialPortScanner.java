package com.rusefi;

import com.rusefi.autodetect.SerialAutoChecker;
import com.rusefi.binaryprotocol.IncomingDataBuffer;
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

/**
 * @author Andrey Belomutskiy
 */
public enum SerialPortScanner {
    INSTANCE;

    public enum SerialPortType {
        None(null),
        FomeEcu("FOME ECU"),
        OpenBlt("OpenBLT Bootloader"),
        Unknown("Unknown"),
        ;

        public final String friendlyString;
        SerialPortType(String friendlyString) {
            this.friendlyString = friendlyString;
        }
    }

    public static class PortResult {
        public final String port;
        public final SerialPortType type;

        public PortResult(String port, SerialPortType type) {
            this.port = port;
            this.type = type;
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
    }

    private volatile boolean isRunning = true;

    static final String AUTO_SERIAL = "Auto Serial";

    private final Object lock = new Object();
    private AvailableHardware knownHardware = null;

    private final List<Listener> listeners = new CopyOnWriteArrayList<>();

    public void addListener(Listener listener) {
        boolean shouldStart = listeners.isEmpty();
        listeners.add(listener);
        if (shouldStart)
            startTimer();
    }

    private final static Map<String, PortResult> portCache = new HashMap<>();

    /**
     * Find all available serial ports and checks if simulator local TCP port is available
     */
    private void findAllAvailablePorts(boolean includeSlowLookup) {
        List<PortResult> ports = new ArrayList<>();
        boolean dfuConnected;

        String[] serialPorts = LinkManager.getCommPorts();

        int ecuCount = 0;
        boolean hasAnyOpenblt = false;

        for (String serialPort : serialPorts) {
            // First, check the port cache
            if (portCache.containsKey(serialPort)) {
                // We've already probed this port - don't re-probe it again
                PortResult cached = portCache.get(serialPort);

                if (cached.type == SerialPortType.FomeEcu) {
                    ecuCount++;
                } else if (cached.type == SerialPortType.OpenBlt) {
                    hasAnyOpenblt = true;
                }

                ports.add(cached);
            } else {
                // This one isn't in the cache, probe it to determine what it is
                PortResult result;

                boolean isOpenblt = isPortOpenblt(serialPort);
                if (isOpenblt) {
                    result = new PortResult(serialPort, SerialPortType.OpenBlt);
                    hasAnyOpenblt = true;
                } else {
                    // See if this looks like an ECU
                    boolean isEcu = isPortFomeEcu(serialPort);
                    if (isEcu) {
                        result = new PortResult(serialPort, SerialPortType.FomeEcu);
                        ecuCount++;
                    } else {
                        // Dunno what this is, leave it in the list anyway
                        result = new PortResult(serialPort, SerialPortType.Unknown);
                    }
                }

                ports.add(result);
                portCache.put(serialPort, result);
            }
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
            toRemove.forEach(portCache::remove);
        }

        boolean hasAnyEcu = ecuCount > 0;

        // Only offer auto if there is more than one option to pick from, AND exactly one option is an ECU
        if (ports.size() > 1 && ecuCount == 1) {
            ports.add(0, new PortResult(AUTO_SERIAL, SerialPortType.None));
        }

        if (includeSlowLookup) {
            for (String tcpPort : TcpConnector.getAvailablePorts()) {
                ports.add(new PortResult(tcpPort, SerialPortType.FomeEcu));
                hasAnyEcu = true;
            }

            dfuConnected = DfuFlasher.detectSTM32BootloaderDriverState(UpdateOperationCallbacks.DUMMY);
        } else {
            dfuConnected = false;
        }

        boolean isListUpdated;
        AvailableHardware currentHardware = new AvailableHardware(ports, dfuConnected, hasAnyEcu, hasAnyOpenblt);
        synchronized (lock) {
            isListUpdated = !currentHardware.equals(knownHardware);
            knownHardware = currentHardware;
        }
        if (isListUpdated) {
            for (Listener listener : listeners)
                listener.onChange(currentHardware);
        }
    }

    private void startTimer() {
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
        public final boolean hasAnyEcu;
        public final boolean hasAnyOpenblt;

        public <T> AvailableHardware(List<PortResult> ports, boolean dfuFound, boolean hasAnyEcu, boolean hasAnyOpenblt) {
            this.ports = ports;
            this.dfuFound = dfuFound;
            this.hasAnyEcu = hasAnyEcu;
            this.hasAnyOpenblt = hasAnyOpenblt;
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

    public static boolean isPortFomeEcu(String port) {
        try (IoStream stream = BufferedSerialIoStream.openPort(port)) {
            String signature = SerialAutoChecker.checkResponse(stream);

            if (signature == null) {
                return false;
            }

            return signature.contains("FOME");
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

            byte responseLength = idb.readByte(50);

            // Invalid length, ignore
            if (responseLength != 8) {
                return false;
            }

            // Read length worth of bytes
            byte[] response = new byte[responseLength];
            idb.waitForBytes(100, "openblt detect", System.currentTimeMillis(), responseLength);
            idb.read(response);

            // Response packet should start with FF
            // Not much else to check, as the rest of the response is protocol settings from the device.
            return response[0] == (byte) 0xFF;
        } catch (IOException e) {
            return false;
        }
    }
}
