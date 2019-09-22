package com.rusefi.ui.etb;

import com.rusefi.core.MessagesCentral;
import com.rusefi.core.Sensor;
import com.rusefi.core.SensorCentral;
import com.rusefi.io.CommandQueue;

import javax.swing.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

import static com.rusefi.Timeouts.SECOND;
import static com.rusefi.config.generated.Fields.CMD_ETB_DUTY;

/**
 * (c) Andrey Belomutskiy
 */
public class EtbReturnToNeutral {
    private static final int ZERO_POSITION = 0;
    private static final int NEUTRAL_POSITION = 3;
    private JButton button = new JButton("ETB Spring Test");

    private final static float SHUT_CLOSED = -30;

    public EtbReturnToNeutral() {
        button.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                Runnable runnable = new Runnable() {
                    @Override
                    public void run() {

                        try {
                            testSequence();
                        } catch (InterruptedException e1) {
                            throw new IllegalStateException(e1);
                        }


                    }
                };
                new Thread(runnable).start();
            }
        });
    }

    private void testSequence() throws InterruptedException {

        CommandQueue.getInstance().write(CMD_ETB_DUTY + " " + 0);
        // longer pause in the beginning just in case
        Thread.sleep(2 * SECOND);
        assertPosition("First neutral position expected", NEUTRAL_POSITION);


        for (int i = 1; i <= 10; i++) {
            CommandQueue.getInstance().write(CMD_ETB_DUTY + " " + 0);
            Thread.sleep(1 * SECOND);

            assertPosition(i + ": Neutral position expected", NEUTRAL_POSITION);


            CommandQueue.getInstance().write(CMD_ETB_DUTY + " " + SHUT_CLOSED);
            Thread.sleep(SECOND);
            assertPosition(i + ": Should be shot closed", 0);
        }


        CommandQueue.getInstance().write(DirectDrivePanel.CANCEL_DIRECT_DRIVE_COMMAND);

    }

    private void assertPosition(String msg, float expectedPosition) {
        double tps = SensorCentral.getInstance().getValue(Sensor.TPS);
        MessagesCentral.getInstance().postMessage(getClass(), msg + tps);
        if (Math.abs(tps - expectedPosition) > 1)
            MessagesCentral.getInstance().postMessage(getClass(), msg +  " NOT GREAT " + tps + " while expected " + expectedPosition);
    }

    public JComponent getContent() {
        return button;
    }
}
