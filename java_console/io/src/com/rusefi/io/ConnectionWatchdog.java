package com.rusefi.io;

import com.rusefi.FileLog;
import com.rusefi.Timeouts;

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
        HeartBeatListeners.INSTANCE.addListener(ConnectionWatchdog::onDataArrived);
        onDataArrived();
    }

    private static void onDataArrived() {
        /**
         * this timer will reconnect
         */
        reconnectTimer.restart();
    }
}