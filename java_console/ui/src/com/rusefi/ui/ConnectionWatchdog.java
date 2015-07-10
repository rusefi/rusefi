package com.rusefi.ui;

import com.rusefi.Timeouts;
import com.rusefi.core.EngineTimeListener;
import com.rusefi.io.LinkManager;

import javax.swing.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

public class ConnectionWatchdog {
    static final Timer reconnectTimer = new Timer(Timeouts.CONNECTION_RESTART_DELAY, new ActionListener() {
        @Override
        public void actionPerformed(ActionEvent e) {
            LinkManager.restart();
            reconnectTimer.restart();
        }
    });

    private ConnectionWatchdog() {
    }

    public static void start() {
        reconnectTimer.restart();

        LinkManager.engineState.timeListeners.add(new EngineTimeListener() {
            @Override
            public void onTime(double time) {
                /**
                 * this timer will reconnect
                 */
                reconnectTimer.restart();
            }
        });
    }
}