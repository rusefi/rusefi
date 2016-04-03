package com.rusefi.io;

import com.rusefi.FileLog;
import com.rusefi.Timeouts;
import com.rusefi.core.EngineTimeListener;
import com.rusefi.io.LinkManager;

import javax.swing.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

public class ConnectionWatchdog {
    private static final Timer reconnectTimer = new Timer(Timeouts.CONNECTION_RESTART_DELAY, new ActionListener() {
        @Override
        public void actionPerformed(ActionEvent e) {
            FileLog.MAIN.logLine("ConnectionWatchdog.reconnectTimer restarting");
            LinkManager.restart();
            onDataArrived();
        }
    });

    private ConnectionWatchdog() {
    }

    public static void start() {
        onDataArrived();

        LinkManager.engineState.timeListeners.add(new EngineTimeListener() {
            @Override
            public void onTime(double time) {
                onDataArrived();
            }
        });
    }

    public static void onDataArrived() {
        /**
         * this timer will reconnect
         */
        reconnectTimer.restart();
    }
}