package com.rusefi;

import com.rusefi.io.LinkManager;
import com.rusefi.io.tcp.TcpConnector;
import com.rusefi.maintenance.DfuFlasher;
import com.rusefi.io.UpdateOperationCallbacks;
import org.jetbrains.annotations.NotNull;

import java.util.*;
import java.util.concurrent.CopyOnWriteArrayList;

/**
 * @author Andrey Belomutskiy
 */
public enum SerialPortScanner {
    INSTANCE;

    public enum SerialPortType {
        FomeEcu,
        OpenBlt,
        Unknown,
    }

    public static class PortResult {
        public final String port;
        public final SerialPortType type;

        public PortResult(String port, SerialPortType type) {
            this.port = port;
            this.type = type;
        }
    }

    private volatile boolean isRunning = true;

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
        List<PortResult> ports = new ArrayList<>();
        boolean dfuConnected;

        String[] serialPorts = LinkManager.getCommPorts();
        if (serialPorts.length > 0)
            ports.add(new PortResult(AUTO_SERIAL, SerialPortType.FomeEcu));

        boolean hasAnyEcu = false;
        boolean hasAnyOpenblt = false;

        for (String serialPort : serialPorts) {
            // TODO: removeme once we can query each port's type
            hasAnyEcu = true;
            hasAnyOpenblt = true;


            // See if this looks like an ECU
            boolean isEcu = true;   // TODO
            if (isEcu) {
                ports.add(new PortResult(serialPort, SerialPortType.FomeEcu));
                hasAnyEcu = true;
            } else {
                boolean isOpenblt = true;   // TODO
                if (isOpenblt) {
                    ports.add(new PortResult(serialPort, SerialPortType.OpenBlt));
                    hasAnyOpenblt = true;
                } else {
                    // Dunno what this is, leave it in the list anyway
                    ports.add(new PortResult(serialPort, SerialPortType.Unknown));
                }
            }
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
}
