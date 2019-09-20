package com.rusefi;

import com.rusefi.io.tcp.TcpConnector;
import jssc.SerialPortList;
import org.jetbrains.annotations.NotNull;

import javax.swing.*;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;
import java.util.concurrent.CopyOnWriteArrayList;

/**
 * @author Andrey Belomutskiy
 */
public enum SerialPortScanner {
    INSTANCE;

    static final String AUTO_SERIAL = "Auto Serial";
    @NotNull
    private List<String> knownPorts = new ArrayList<>();

    public List<Listener> listeners = new CopyOnWriteArrayList<>();

    private final Timer scanPortsTimer = new Timer(1000, e -> findAllAvailablePorts());


    /**
     * Find all available serial ports and checks if simulator local TCP port is available
     */
    @NotNull
    void findAllAvailablePorts() {
        List<String> ports = new ArrayList<>();
        String[] serialPorts = SerialPortList.getPortNames();
        if (serialPorts.length > 0 || serialPorts.length < 15)
            ports.add(AUTO_SERIAL);
        ports.addAll(Arrays.asList(serialPorts));
        ports.addAll(TcpConnector.getAvailablePorts());

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
        scanPortsTimer.start();
    }

    public void stopTimer() {
        scanPortsTimer.stop();
    }

    interface Listener {
        void onChange();
    }
}
