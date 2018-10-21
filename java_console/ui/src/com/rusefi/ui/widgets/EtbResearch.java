package com.rusefi.ui.widgets;

import com.opensr5.Logger;
import com.rusefi.FileLog;
import com.rusefi.core.Sensor;
import com.rusefi.core.SensorCentral;
import com.rusefi.io.CommandQueue;
import com.rusefi.io.InvocationConfirmationListener;

import javax.swing.*;
import java.awt.event.ActionEvent;
import java.util.concurrent.Executors;
import java.util.concurrent.ScheduledExecutorService;

/**
 * (c) Andrey Belomutskiy
 * 10/21/2018
 */
public class EtbResearch {
    private static final long SLEEP = 1000;
    private final JButton button = new JButton("ETB R");
    private final ScheduledExecutorService executor = Executors.newSingleThreadScheduledExecutor();
    // close to zero but not really zero so that we can validate this value
    private final static double LOW_VALUE = 0.5;
//    private boolean isStarted;


    State state;
    private double currentValue;

    InvocationConfirmationListener goingUp = new InvocationConfirmationListener() {
        @Override
        public void onCommandConfirmation() {
            state = State.GOING_UP;


            executor.execute(new Runnable() {
                @Override
                public void run() {
                    try {
                        Thread.sleep(SLEEP);
                    } catch (InterruptedException unexpected) {
                        unexpected.printStackTrace();
                        return;
                    }

                    double tpsPosition = SensorCentral.getInstance().getValue(Sensor.TPS);
                    FileLog.MAIN.logLine("ETB duty " + currentValue + ": tps=" + tpsPosition);

                    if (tpsPosition == 0) {
                        currentValue += 1.0;
                        CommandQueue.getInstance().write("set_etb " + currentValue, goingUp);
                    }
                }
            });

        }
    };

    InvocationConfirmationListener setDebugModeConfiguration = new InvocationConfirmationListener() {
        @Override
        public void onCommandConfirmation() {
            executor.execute(new Runnable() {
                @Override
                public void run() {
                    state = State.START;

                    currentValue = LOW_VALUE;
                    CommandQueue.getInstance().write("set_etb " + currentValue, goingUp);
                }
            });
        }
    };

    public EtbResearch() {
        button.addActionListener(new AbstractAction() {
            @Override
            public void actionPerformed(ActionEvent e) {
//                if (!isStarted) {
////                    metric.start();
//                    isStarted = true;
//                }
                executor.execute(new Runnable() {
                    @Override
                    public void run() {
                        // magic constant for DBG_ELECTRONIC_THROTTLE_EXTRA
                        state = State.DEBUG_MODE;
                        CommandQueue.getInstance().write("set debug_mode " + 29, setDebugModeConfiguration);
                    }
                });

            }
        });
    }

    public JComponent getButton() {
        return button;
    }

    enum State {
        DEBUG_MODE,
        START,
        GOING_UP,
    }
}
