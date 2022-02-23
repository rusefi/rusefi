package com.rusefi.io;

import com.devexperts.logging.Logging;
import com.rusefi.Timeouts;

import javax.swing.*;

import static com.devexperts.logging.Logging.getLogging;

/**
 * todo: open question if it's OK to use AWT timer in headless cases?
 */
public class ConnectionWatchdog {
    private static final Logging log = getLogging(ConnectionWatchdog.class);

    private final Timer reconnectTimer;
    private static boolean isCreated;

    private ConnectionWatchdog(int timeoutMs, Runnable action) {
        reconnectTimer = new Timer(timeoutMs, e -> {
            action.run();
            onDataArrived();
        });
    }

    public synchronized static void init(LinkManager linkManager) {
        if (isCreated)
            return; // only one instance is needed
        isCreated = true;
        new ConnectionWatchdog(Timeouts.CONNECTION_RESTART_DELAY, () -> {
            linkManager.execute(() -> {
                log.info("ConnectionWatchdog.reconnectTimer restarting: " + Timeouts.CONNECTION_RESTART_DELAY);
                linkManager.restart();
            });
        }).start();
    }

    void start() {
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