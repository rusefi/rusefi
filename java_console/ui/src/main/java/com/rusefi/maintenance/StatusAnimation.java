package com.rusefi.maintenance;

import javax.swing.*;
import java.awt.event.ActionEvent;

/**
 * This class is in charge of status bar animation with dots running from empty to DOTS_COUNT
 */
public class StatusAnimation {
    private static final int DOTS_COUNT = 5;

    private final Timer timer;
    private volatile boolean isStopped;
    private int counter;

    public StatusAnimation(StatusConsumer consumer, String message) {
        timer = new Timer(200 /*ms*/, new AbstractAction() {
            @Override
            public void actionPerformed(ActionEvent e) {
                if (isStopped)
                    return;
                counter++;
                StringBuilder status = new StringBuilder(message);
                for (int i = 0; i < counter % DOTS_COUNT; i++) {
                    status.append(".");
                }
                consumer.onStatus(status.toString());
            }
        });
        timer.start();
    }

    public void stop() {
        timer.stop();
        isStopped = true;
    }

    public interface StatusConsumer {
        void onStatus(String niceStatus);
    }
}
