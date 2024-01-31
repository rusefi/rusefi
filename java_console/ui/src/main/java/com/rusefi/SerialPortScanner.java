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
import com.rusefi.io.serial.BufferedSerialIoStream;
import com.rusefi.io.tcp.TcpConnector;
import com.rusefi.maintenance.DfuFlasher;
import com.rusefi.io.UpdateOperationCallbacks;
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
        Ecu("ECU", 20),
        EcuWithOpenblt("ECU w/ BL", 20),
        OpenBlt("OpenBLT Bootloader", 10),
        CAN("CAN", 30),
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
            return type == SerialPortType.Ecu || type == SerialPortType.EcuWithOpenblt;
        }
    }

    private volatile boolean isRunning = true;

    private static final boolean SHOW_SOCKETCAN = FileLog.isLinux();

    static final String AUTO_SERIAL = "Auto Serial";

    private final Object lock = new Object();
    @NotNull
    private AvailableHardware knownHardware = new AvailableHardware(Collections.emptyList(), false, false, false);

    private final List<Listener> listeners = new CopyOnWriteArrayList<>();

    public void addListener(Listener listener) {
        boolean shouldStart = listeners.isEmpty();
        listeners.add(listener);
        if (shouldStart)
            startTimer();
    }

    /**
     * Find all available serial ports and checks if simulator local TCP port is available
     */
    private void findAllAvailablePorts(boolean includeSlowLookup) {
        List<String> ports = new ArrayList<>();
        boolean dfuConnected;
        boolean stLinkConnected;
        boolean PCANConnected;

        String[] serialPorts = LinkManager.getCommPorts();
        if (serialPorts.length > 0)
            ports.add(AUTO_SERIAL);
        for (String serialPort : serialPorts) {
            // Filter out some macOS trash
            if (serialPort.contains("wlan-debug") ||
                    serialPort.contains("Bluetooth-Incoming-Port") ||
                    serialPort.startsWith("cu.")) {
                continue;
            }
            ports.add(serialPort);
        }

        if (includeSlowLookup) {
            ports.addAll(TcpConnector.getAvailablePorts());
            dfuConnected = DfuFlasher.detectSTM32BootloaderDriverState(UpdateOperationCallbacks.DUMMY);
            stLinkConnected = DfuFlasher.detectStLink(UpdateOperationCallbacks.DUMMY);
            PCANConnected = DfuFlasher.detectPcan(UpdateOperationCallbacks.DUMMY);
        } else {
            dfuConnected = false;
            stLinkConnected = false;
            PCANConnected = false;
        }
        if (PCANConnected)
            ports.add(LinkManager.PCAN);
        if (SHOW_SOCKETCAN)
            ports.add(LinkManager.SOCKET_CAN);

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
        Thread portsScanner = new Thread(() -> {
            boolean isFirstTime = true;
            while (isRunning) {
                findAllAvailablePorts(!isFirstTime);
                isFirstTime = false;
                try {
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

        private final List<String> ports;
        private final boolean dfuFound;
        private final boolean stLinkConnected;
        private final boolean PCANConnected;

        public <T> AvailableHardware(List<String> ports, boolean dfuFound, boolean stLinkConnected, boolean PCANConnected) {
            this.ports = ports;
            this.dfuFound = dfuFound;
            this.stLinkConnected = stLinkConnected;
            this.PCANConnected = PCANConnected;
        }

        @NotNull
        public List<String> getKnownPorts() {return new ArrayList<>(ports);}

        public boolean isDfuFound() {
            return dfuFound;
        }

        public boolean isStLinkConnected() {return stLinkConnected;}
        public boolean isPCANConnected(){return PCANConnected;}

        @Override
        public boolean equals(Object o) {
            if (this == o) return true;
            if (o == null || getClass() != o.getClass()) return false;
            AvailableHardware that = (AvailableHardware) o;
            return dfuFound == that.dfuFound && stLinkConnected == that.stLinkConnected && PCANConnected == that.PCANConnected && ports.equals(that.ports);
        }

        public boolean isEmpty() {
            return !dfuFound && !stLinkConnected && !PCANConnected && ports.isEmpty();
        }
    }

    public static String getEcuSignature(String port) {
        try (IoStream stream = BufferedSerialIoStream.openPort(port)) {
            return SerialAutoChecker.checkResponse(stream, callbackContext -> null);
        } catch (Exception e) {
            return null;
        }
    }

    public static boolean ecuHasOpenblt(String port) {
        try (IoStream stream = BufferedSerialIoStream.openPort(port)) {
            if (stream == null) {
                return false;
            }

            stream.sendPacket(new byte[]{(byte) Fields.TS_QUERY_BOOTLOADER});

            byte[] response = stream.getDataBuffer().getPacket(500, "ecuHasOpenblt");
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
