package com.rusefi.io;

import com.devexperts.logging.Logging;
import com.rusefi.Timeouts;

import javax.swing.*;

import java.util.concurrent.atomic.AtomicBoolean;

import static com.devexperts.logging.Logging.getLogging;

/**
 * todo: open question if it's OK to use AWT timer in headless cases?
 */
public class ConnectionWatchdog {
    private static final Logging log = getLogging(ConnectionWatchdog.class);

    private final Timer reconnectTimer;
    private static boolean isCreated;

    private ConnectionWatchdog(int timeoutMs, Runnable restartAction) {
        reconnectTimer = new Timer(timeoutMs, e -> {
            restartAction.run();
            // mark alive right after restart attempt
            onDataArrived();
        });
    }

    public synchronized static void init(LinkManager linkManager) {
        final AtomicBoolean isRestartPending = new AtomicBoolean();

        if (isCreated)
            return; // only one instance is needed
        isCreated = true;
        new ConnectionWatchdog(Timeouts.CONNECTION_RESTART_DELAY, () -> {
            if (isRestartPending.compareAndSet(false, true)) {
                linkManager.execute(() -> {
                    log.info("ConnectionWatchdog.reconnectTimer restarting: " + Timeouts.CONNECTION_RESTART_DELAY);
                    linkManager.restart();
                    isRestartPending.set(false);
                });
            } else {
                log.info("restart already pending...");
            }
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