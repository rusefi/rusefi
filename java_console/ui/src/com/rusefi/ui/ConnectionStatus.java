package com.rusefi.ui;

import com.rusefi.core.EngineTimeListener;
import com.rusefi.core.MessagesCentral;
import com.rusefi.io.LinkManager;

import javax.swing.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.util.List;
import java.util.concurrent.CopyOnWriteArrayList;

public class ConnectionStatus {
    // todo: react to any message as connected? how to know if message from controller, not internal message?
    private static final String FATAL_MESSAGE_PREFIX = "FATAL";
    private boolean isConnected;

    public static ConnectionStatus INSTANCE = new ConnectionStatus();
    private List<Listener> listeners = new CopyOnWriteArrayList<>();

    private ConnectionStatus() {
        final Timer timer = new Timer(3000, new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                setConnected(false);
            }
        });

        LinkManager.engineState.timeListeners.add(new EngineTimeListener() {
            @Override
            public void onTime(double time) {
                markConnected(timer);
            }
        });

        MessagesCentral.getInstance().addListener(new MessagesCentral.MessageListener() {
            @Override
            public void onMessage(Class clazz, String message) {
                if (message.startsWith(FATAL_MESSAGE_PREFIX))
                    markConnected(timer);
            }
        });
    }

    private void markConnected(Timer timer1) {
        setConnected(true);
        /**
         * this timer will catch engine inactivity and display a warning
         */
        timer1.restart();
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

    /**
     * @see #setConnected(boolean)
     */
    public void addListener(Listener listener) {
        listeners.add(listener);
    }

    public interface Listener {
        void onConnectionStatus(boolean isConnected);
    }
}