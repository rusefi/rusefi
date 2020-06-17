package com.rusefi.maintenance;

import com.rusefi.ui.StatusWindow;

import javax.swing.*;
import java.awt.event.ActionEvent;

/**
 * This class is in charge of status bar animation with dots running from empty to DOTS_COUNT
 */
public class StatusAnimation {
    private static final String WORKING = "Working";
    private static final int DOTS_COUNT = 5;

    private final Timer timer;
    private volatile boolean isStopped;
    private int counter;

    public StatusAnimation(StatusWindow wnd) {
        timer = new Timer(200 /*ms*/, new AbstractAction() {
            @Override
            public void actionPerformed(ActionEvent e) {
                if (isStopped)
                    return;
                counter++;
                StringBuilder status = new StringBuilder(WORKING);
                for (int i = 0; i < counter % DOTS_COUNT; i++) {
                    status.append(".");
                }
                wnd.setStatus(status.toString());
            }
        });
        timer.start();
    }

    public void stop() {
        timer.stop();
        isStopped = true;
    }
}
