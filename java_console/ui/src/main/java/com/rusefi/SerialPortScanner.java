package com.rusefi;

import com.rusefi.io.LinkManager;
import com.rusefi.io.tcp.TcpConnector;
import org.jetbrains.annotations.NotNull;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;
import java.util.concurrent.CopyOnWriteArrayList;

import static com.rusefi.FileLog.isLinux;

/**
 * @author Andrey Belomutskiy
 */
public enum SerialPortScanner {
    INSTANCE;

    private volatile boolean isRunning = true;

    private static final boolean SHOW_PCAN = Boolean.parseBoolean(System.getenv().get("RUSEFI_PCAN"));
    private static final boolean SHOW_SOCKETCAN = isLinux();

    static final String AUTO_SERIAL = "Auto Serial";
    @NotNull
    private final List<String> knownPorts = new ArrayList<>();

    public List<Listener> listeners = new CopyOnWriteArrayList<>();

    /**
     * Find all available serial ports and checks if simulator local TCP port is available
     */
    void findAllAvailablePorts(boolean includeSlowTcpLookup) {
        List<String> ports = new ArrayList<>();
        String[] serialPorts = LinkManager.getCommPorts();
        if (serialPorts.length > 0 && serialPorts.length < 15)
            ports.add(AUTO_SERIAL);
        ports.addAll(Arrays.asList(serialPorts));
        if (includeSlowTcpLookup)
            ports.addAll(TcpConnector.getAvailablePorts());
        if (SHOW_PCAN)
            ports.add(LinkManager.PCAN);
        if (SHOW_SOCKETCAN)
            ports.add(LinkManager.SOCKET_CAN);

        boolean isListUpdated;
        synchronized (knownPorts) {
            isListUpdated = knownPorts.equals(ports);
            knownPorts.clear();
            knownPorts.addAll(ports);
        }
        if (isListUpdated) {
            for (Listener listener : listeners)
                listener.onChange();
        }
    }

    @NotNull
    public List<String> getKnownPorts() {
        synchronized (knownPorts) {
            return new ArrayList<>(knownPorts);
        }
    }

    public void startTimer() {
        Thread portsScanner = new Thread(() -> {
            while (isRunning) {
                findAllAvailablePorts(true);
                try {
                    Thread.sleep(1000);
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
        void onChange();
    }
}
