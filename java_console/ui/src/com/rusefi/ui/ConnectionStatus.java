package com.rusefi.ui;

import com.rusefi.core.EngineTimeListener;
import com.rusefi.io.LinkManager;

import javax.swing.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.util.List;
import java.util.concurrent.CopyOnWriteArrayList;

public class ConnectionStatus {
    private boolean isConnected;

    public static ConnectionStatus INSTANCE = new ConnectionStatus();
    private List<Listener> listeners = new CopyOnWriteArrayList<>();

    private ConnectionStatus() {
        final Timer timer1 = new Timer(2000, new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                setConnected(false);
            }
        });

        LinkManager.engineState.timeListeners.add(new EngineTimeListener() {
            @Override
            public void onTime(double time) {
                setConnected(true);
                /**
                 * this timer will catch engine inactivity and display a warning
                 */
                timer1.restart();
            }
        });
    }

    private void setConnected(boolean isConnected) {
        if (isConnected == this.isConnected)
            return;
        this.isConnected = isConnected;
        for (Listener listener : listeners)
            listener.onConnectionStatus(isConnected);
    }

    public boolean isConnected() {
        return isConnected;
    }

    public void addListener(Listener listener) {
        listeners.add(listener);
    }

    public interface Listener {
        void onConnectionStatus(boolean isConnected);
    }
}