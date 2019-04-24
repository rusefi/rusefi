package com.rusefi.ui.etb;

import com.rusefi.core.MessagesCentral;
import com.rusefi.core.Sensor;
import com.rusefi.core.SensorCentral;
import com.rusefi.io.CommandQueue;
import com.rusefi.io.InvocationConfirmationListener;

import javax.swing.*;
import java.awt.event.ActionEvent;
import java.util.concurrent.Executors;
import java.util.concurrent.ScheduledExecutorService;

import static com.rusefi.Timeouts.SECOND;

/**
 * This tool finds interesting ETB duty cycles like the value when in starts to open or the value
 * when it's completely open
 * <p>
 * (c) Andrey Belomutskiy
 * 10/21/2018
 */
public class MagicSpotsFinder {
    private static final long SLEEP = SECOND;
    // how much do we want to change duty cycle on each iteration?
    private static final double DUTY_CYCLE_STEP = 0.5;
    private final JButton button = new JButton("ETB Research");
    private final ScheduledExecutorService executor = Executors.newSingleThreadScheduledExecutor();
    // close to zero but not really zero so that we can validate this value
    private final static double LOW_VALUE = 0.5;
//    private boolean isStarted;


    private State state;
    private double currentValue;
    /**
     * here we record at which duty cycle ETB would start moving
     */
    private double startedToOpenValue;
    private double reached100Value;
    private double startedToCloseValue;
    private double backToZeroValue;

    private InvocationConfirmationListener goingDown = new InvocationConfirmationListener() {
        @Override
        public void onCommandConfirmation() {
            state = State.GOING_DOWN;

            executor.execute(new Runnable() {
                @Override
                public void run() {
                    sleep();

                    double tpsPosition = SensorCentral.getInstance().getValue(Sensor.TPS);
                    MessagesCentral.getInstance().postMessage(getClass(), "ETB duty " + currentValue + ": tps=" + tpsPosition);

                    if (tpsPosition >= 100) {
                        currentValue -= DUTY_CYCLE_STEP;
                        CommandQueue.getInstance().write("set_etb " + currentValue, goingDown);
                    } else if (tpsPosition > 0) {

                        if (startedToCloseValue == 0) {
                            // if that's the first we've moved let's remember duty cycle value
                            startedToCloseValue = currentValue;
                            MessagesCentral.getInstance().postMessage(getClass(), "Started to close at " + startedToCloseValue);
                        }

                        currentValue -= DUTY_CYCLE_STEP;
                        CommandQueue.getInstance().write("set_etb " + currentValue, goingDown);


                    } else {
                        backToZeroValue = currentValue;
                        MessagesCentral.getInstance().postMessage(getClass(), "Back closed to close at " + backToZeroValue);

                        MessagesCentral.getInstance().postMessage(getClass(), "startedToOpenValue = " + startedToOpenValue + ", reached100Value = " + reached100Value);
                        MessagesCentral.getInstance().postMessage(getClass(), "startedToCloseValue = " + startedToCloseValue + ", backToZeroValue = " + backToZeroValue);
                    }

                }
            });
        }
    };

    private InvocationConfirmationListener goingUp = new InvocationConfirmationListener() {
        @Override
        public void onCommandConfirmation() {
            state = State.GOING_UP;

            executor.execute(new Runnable() {
                @Override
                public void run() {
                    sleep();

                    double tpsPosition = SensorCentral.getInstance().getValue(Sensor.TPS);
                    MessagesCentral.getInstance().postMessage(getClass(), "ETB duty " + currentValue + ": tps=" + tpsPosition);

                    if (tpsPosition == 0) {
                        // ETB has not moved yet, keep going up
                        currentValue += DUTY_CYCLE_STEP;
                        CommandQueue.getInstance().write("set_etb " + currentValue, goingUp);
                    } else if (tpsPosition < 100) {

                        if (startedToOpenValue == 0) {
                            // if that's the first we've moved let's remember duty cycle value
                            startedToOpenValue = currentValue;
                            MessagesCentral.getInstance().postMessage(getClass(), "Started to open at " + startedToOpenValue);
                        }


                        // ETB has not reached 100%, keep going up
                        currentValue += DUTY_CYCLE_STEP;
                        CommandQueue.getInstance().write("set_etb " + currentValue, goingUp);

                    } else {
                        // looks like we have reached 100%, cool!
                        reached100Value = currentValue;
                        MessagesCentral.getInstance().postMessage(getClass(), "startedToOpenValue = " + startedToOpenValue + ", reached100Value = " + reached100Value);

                        currentValue -= DUTY_CYCLE_STEP;
                        CommandQueue.getInstance().write("set_etb " + currentValue, goingDown);
                    }
                }

            });

        }
    };

    // settings are confirmed, let's start going up from zero
    private InvocationConfirmationListener setDebugModeConfiguration = new InvocationConfirmationListener() {
        @Override
        public void onCommandConfirmation() {
            executor.execute(new Runnable() {
                @Override
                public void run() {
                    state = State.START;

                    MessagesCentral.getInstance().postMessage(getClass(), "Start!");
                    resetValues();

                    CommandQueue.getInstance().write("set_etb " + currentValue, goingUp);
                }
            });
        }
    };

    private void resetValues() {
        currentValue = LOW_VALUE;
        startedToOpenValue = 0;
        reached100Value = 0;
        startedToCloseValue = 0;
        backToZeroValue = 0;
    }

    public MagicSpotsFinder() {
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

    private void sleep() {
        try {
            MessagesCentral.getInstance().postMessage(getClass(), "Sleeping " + SLEEP + "ms");
            Thread.sleep(SLEEP);
        } catch (InterruptedException unexpected) {
            unexpected.printStackTrace();
            throw new IllegalStateException(unexpected);
        }
    }

    private enum State {
        /**
         * To begin we need to adjust some settings
         */
        DEBUG_MODE,
        /**
         * Once settings change is confirmed we are here
         */
        START,
        GOING_UP,
        GOING_DOWN,
    }
}
