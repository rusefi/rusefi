package com.rusefi.ui.widgets;

import com.rusefi.core.MessagesCentral;
import com.rusefi.core.Sensor;
import com.rusefi.core.SensorCentral;
import com.rusefi.io.CommandQueue;

import javax.swing.*;

import java.util.Random;

import static com.rusefi.Timeouts.SECOND;
import static com.rusefi.ui.widgets.EtbTestSequence.*;

/**
 * 3/2/2019
 * (c) Andrey Belomutskiy
 */
public class EtbMonteCarloSequence {
    public static final int LIMIT = 100;
    private final JButton button = new JButton("ETB I feel lucky!");
    private final static Random r = new Random();
    private int counter;

    public EtbMonteCarloSequence() {
        button.addActionListener(e -> {
            counter = 0;

            // 3000 data points at 10Hz should be 300 seconds worth of data
            metric.start(/* buffer size: */3000, /*period, ms: */ 100);

            executor.execute(this::runRandomCycle);
        });
    }

    private void runRandomCycle() {
        final int offset = r.nextInt(100);
        final double pFactor = 1 + (r.nextInt(300) / 100.0);
        final double iFactor = r.nextInt(30) / 100.0;
        final double dFactor = r.nextInt(30) / 100.0;
        String stats = "mcstats:offset:" + offset +
                ":pFactor:" + pFactor +
                ":iFactor:" + iFactor +
                ":dFactor:" + dFactor;
        MessagesCentral.getInstance().postMessage(EtbMonteCarloSequence.class, stats);
        CommandQueue.getInstance().write("set etb_o " + offset);
        CommandQueue.getInstance().write("set etb_p " + pFactor);
        CommandQueue.getInstance().write("set etb_i " + iFactor);
        CommandQueue.getInstance().write("set etb_d " + dFactor);

        SequenceStep firstStep = new EtbTarget(10 * SECOND, 4 /*position*/);
        SequenceStep last = addSequence(firstStep);
        last.addNext(new SequenceStep(5 * SECOND) {
            @Override
            protected void doJob() {
                double result = SensorCentral.getInstance().getValue(Sensor.ETB_CONTROL_QUALITY);
                MessagesCentral.getInstance().postMessage(EtbMonteCarloSequence.class, stats + ":result:" + result);
                if (counter == LIMIT) {
                    MessagesCentral.getInstance().postMessage(EtbTestSequence.class, "ETB MC sequence done!");
                    return;
                }
                counter++;
                MessagesCentral.getInstance().postMessage(EtbTestSequence.class, "Starting " + counter + " of " + LIMIT);
                runRandomCycle();
            }
        });
        firstStep.execute(executor);
    }

    public JButton getButton() {
        return button;
    }
}
