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
    private static ConnectionWatchdog instance;

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
        instance = new ConnectionWatchdog(Timeouts.CONNECTION_RESTART_DELAY, () -> {
            if (isRestartPending.compareAndSet(false, true)) {
                linkManager.execute(() -> {
                    log.info("ConnectionWatchdog.reconnectTimer restarting: " + Timeouts.CONNECTION_RESTART_DELAY);
                    linkManager.restart();
                    isRestartPending.set(false);
                });
            } else {
                log.info("restart already pending...");
            }
        });
        instance.start();
    }

    /**
     * Temporarily suppress the watchdog reconnect timer. Use this when a firmware flash
     * or other operation owns the port exclusively so the watchdog does not race it. (#9771)
     */
    public synchronized static void pause() {
        if (instance != null) {
            instance.reconnectTimer.stop();
            log.info("ConnectionWatchdog paused");
        }
    }

    /**
     * Resume the watchdog after {@link #pause()}. The timer restarts its full delay.
     */
    public synchronized static void resume() {
        if (instance != null) {
            instance.reconnectTimer.restart();
            log.info("ConnectionWatchdog resumed");
        }
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