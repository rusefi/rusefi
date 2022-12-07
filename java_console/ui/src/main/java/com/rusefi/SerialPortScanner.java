package com.rusefi;

import com.rusefi.io.LinkManager;
import com.rusefi.io.tcp.TcpConnector;
import com.rusefi.maintenance.DfuFlasher;
import com.rusefi.ui.StatusConsumer;
import org.jetbrains.annotations.NotNull;

import java.util.*;
import java.util.concurrent.CopyOnWriteArrayList;

import static com.rusefi.FileLog.isLinux;

/**
 * @author Andrey Belomutskiy
 */
public enum SerialPortScanner {
    INSTANCE;

    private volatile boolean isRunning = true;

    private static final boolean SHOW_PCAN = false;// todo: reimplement with wmic Boolean.parseBoolean(System.getenv().get("RUSEFI_PCAN"));
    private static final boolean SHOW_SOCKETCAN = isLinux();

    static final String AUTO_SERIAL = "Auto Serial";

    private final Object lock = new Object();
    @NotNull
    private AvailableHardware knownHardware = new AvailableHardware(Collections.emptyList(), false, false);

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
    private void findAllAvailablePorts(boolean includeSlowTcpLookup) {
        List<String> ports = new ArrayList<>();
        boolean dfuConnected;
        boolean stLinkConnected;

        String[] serialPorts = LinkManager.getCommPorts();
        if (serialPorts.length > 0)
            ports.add(AUTO_SERIAL);
        ports.addAll(Arrays.asList(serialPorts));

        if (includeSlowTcpLookup) {
            ports.addAll(TcpConnector.getAvailablePorts());
            dfuConnected = DfuFlasher.detectSTM32BootloaderDriverState(StatusConsumer.VOID);
            stLinkConnected = DfuFlasher.detectStLink(StatusConsumer.VOID);
        } else {
            dfuConnected = false;
            stLinkConnected = false;
        }
        if (SHOW_PCAN)
            ports.add(LinkManager.PCAN);
        if (SHOW_SOCKETCAN)
            ports.add(LinkManager.SOCKET_CAN);

        boolean isListUpdated;
        AvailableHardware currentHardware = new AvailableHardware(ports, dfuConnected, stLinkConnected);
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

        public <T> AvailableHardware(List<String> ports, boolean dfuFound, boolean stLinkConnected) {
            this.ports = ports;
            this.dfuFound = dfuFound;
            this.stLinkConnected = stLinkConnected;
        }

        @NotNull
        public List<String> getKnownPorts() {
            return new ArrayList<>(ports);
        }

        public boolean isDfuFound() {
            return dfuFound;
        }

        public boolean isStLinkConnected() {
            return stLinkConnected;
        }

        @Override
        public boolean equals(Object o) {
            if (this == o) return true;
            if (o == null || getClass() != o.getClass()) return false;
            AvailableHardware that = (AvailableHardware) o;
            return dfuFound == that.dfuFound && stLinkConnected == that.stLinkConnected && ports.equals(that.ports);
        }

        public boolean isEmpty() {
            return !dfuFound && !stLinkConnected && ports.isEmpty();
        }
    }
}
