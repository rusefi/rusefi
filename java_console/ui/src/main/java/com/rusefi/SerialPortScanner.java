package com.rusefi;

import com.devexperts.logging.Logging;
import com.rusefi.autodetect.SerialAutoChecker;
import com.rusefi.binaryprotocol.IncomingDataBuffer;
import com.rusefi.binaryprotocol.IoHelper;
import com.rusefi.config.generated.Fields;
import com.rusefi.io.IoStream;
import com.rusefi.io.LinkManager;
import com.rusefi.io.UpdateOperationCallbacks;
import com.rusefi.io.commands.HelloCommand;
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

    private final static Logging log = Logging.getLogging(SerialPortScanner.class);

    public enum SerialPortType {
        None(null, 100),
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

                if (cached.isEcu()) {
                    ecuCount++;
                } else if (cached.type == SerialPortType.OpenBlt) {
                    hasAnyOpenblt = true;
                }

                ports.add(cached);
            } else {
                // This one isn't in the cache, probe it to determine what it is
                PortResult result;

                log.info("Determining type of serial port: " + serialPort);

                boolean isOpenblt = isPortOpenblt(serialPort);
                log.info("Port " + serialPort + (isOpenblt ? " looks like" : " does not look like") + " an OpenBLT bootloader");
                if (isOpenblt) {
                    result = new PortResult(serialPort, SerialPortType.OpenBlt);
                    hasAnyOpenblt = true;
                } else {
                    // See if this looks like an ECU
                    boolean isEcu = isPortFomeEcu(serialPort);
                    log.info("Port " + serialPort + (isEcu ? " looks like" : " does not look like") + " a FOME ECU");
                    if (isEcu) {
                        boolean ecuHasOpenblt = fomeEcuHasOpenblt(serialPort);
                        log.info("FOME ECU at " + serialPort + (ecuHasOpenblt ? " has" : " does not have") + " an OpenBLT bootloader");
                        result = new PortResult(serialPort, ecuHasOpenblt ? SerialPortType.FomeEcuWithOpenblt : SerialPortType.FomeEcu);
                        ecuCount++;
                    } else {
                        // Dunno what this is, leave it in the list anyway
                        result = new PortResult(serialPort, SerialPortType.Unknown);
                    }
                }

                log.info("Port " + serialPort + " detected as: " + result.type.friendlyString);

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
            toRemove.forEach(p -> {
                portCache.remove(p);
                log.info("Removing port " + p);
            });
        }

        boolean hasAnyEcu = ecuCount > 0;

        // Sort ports by their type to put your ECU at the top
        ports.sort(Comparator.comparingInt(a -> a.type.sortOrder));

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

    public static boolean fomeEcuHasOpenblt(String port) {
        try (IoStream stream = BufferedSerialIoStream.openPort(port)) {
            if (stream == null) {
                return false;
            }

            IncomingDataBuffer idb = stream.getDataBuffer();
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
