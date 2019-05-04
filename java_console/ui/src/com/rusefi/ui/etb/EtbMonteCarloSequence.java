package com.rusefi.ui.etb;

import com.rusefi.core.MessagesCentral;
import com.rusefi.core.Sensor;
import com.rusefi.core.SensorCentral;
import com.rusefi.etb.EtbTarget;
import com.rusefi.etb.StandardTestSequence;
import com.rusefi.etb.TestSequenceStep;
import com.rusefi.io.CommandQueue;

import javax.swing.*;

import java.util.Random;

import static com.rusefi.SensorLogger.getSecondsSinceFileStart;
import static com.rusefi.Timeouts.SECOND;
import static com.rusefi.ui.etb.EtbTestSequence.*;

/**
 * 3/2/2019
 * (c) Andrey Belomutskiy
 */
public class EtbMonteCarloSequence {
    public static final int LIMIT = 300;
    private static final double DEFAULT_POSITION = 7;
    private final JButton button = new JButton("ETB I feel lucky!");
    private final static Random r = new Random();
    private int counter;

    private double bestResultSoFar = 75;

    public EtbMonteCarloSequence() {
        button.addActionListener(e -> {
            counter = 0;

            // 3000 data points at 10Hz should be 300 seconds worth of data
            StandardTestSequence.metric.start(/* buffer size: */3000, /*period, ms: */ 100);

            // start first cycle. At the end of the run it would decide if it wants to start from beginning again
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
        CommandQueue.getInstance().write("etbreset");
        CommandQueue.getInstance().write("set etb_o " + offset);
        CommandQueue.getInstance().write("set etb_p " + pFactor);
        CommandQueue.getInstance().write("set etb_i " + iFactor);
        CommandQueue.getInstance().write("set etb_d " + dFactor);

        TestSequenceStep firstStep = new EtbTarget(10 * SECOND, DEFAULT_POSITION, null, TestSequenceStep.Condition.YES);
        TestSequenceStep.Condition condition = new TestSequenceStep.Condition() {
            @Override
            public boolean shouldContinue() {
                double currentValue = StandardTestSequence.metric.getStandardDeviation();
                boolean shouldContinue = currentValue < bestResultSoFar;
                if (!shouldContinue) {
                    MessagesCentral.getInstance().postMessage(EtbMonteCarloSequence.class,
                            "Two much error accumulated, aborting! " + currentValue + " > " + bestResultSoFar);

                }
                return shouldContinue;
            }
        };
        TestSequenceStep last = StandardTestSequence.addSequence(firstStep, null, condition);
        last.addNext(new TestSequenceStep(5 * SECOND, EtbTarget.Condition.YES) {
            @Override
            protected void doJob() {
                double cycleResult = SensorCentral.getInstance().getValue(Sensor.ETB_CONTROL_QUALITY);
                if (cycleResult < bestResultSoFar) {
                    bestResultSoFar = cycleResult;
                    MessagesCentral.getInstance().postMessage(EtbMonteCarloSequence.class,
                            getSecondsSinceFileStart() + ":" + stats + ":new_record:" + bestResultSoFar);
                }
                MessagesCentral.getInstance().postMessage(EtbMonteCarloSequence.class,
                        getSecondsSinceFileStart() + ":" + stats + ":result:" + cycleResult);
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
