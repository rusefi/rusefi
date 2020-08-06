package com.rusefi.io;

import javax.swing.*;

/**
 * todo: open question if it's OK to use AWT timer in headless cases?
 */
public class ConnectionWatchdog {
    private final Timer reconnectTimer;

    public ConnectionWatchdog(int timeoutMs, Runnable action) {
        reconnectTimer = new Timer(timeoutMs, e -> {
            action.run();
            onDataArrived();
        });
    }

    public void start() {
        HeartBeatListeners.INSTANCE.addListener(this::onDataArrived);
        onDataArrived();
    }

    private void onDataArrived() {
        /**
         * this timer will reconnect
         */
        reconnectTimer.restart();
    }
}