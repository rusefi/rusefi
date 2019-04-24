package com.rusefi.ui.etb;

import com.rusefi.TimeBasedBuffer;
import com.rusefi.core.Sensor;
import com.rusefi.etb.EtbTarget;
import com.rusefi.etb.StandardTestSequence;
import com.rusefi.etb.TestSequenceStep;

import javax.swing.*;
import java.util.concurrent.Executors;
import java.util.concurrent.ScheduledExecutorService;
import java.util.concurrent.atomic.AtomicInteger;

import static com.rusefi.Timeouts.SECOND;

/**
 * Here we initiate a sequence of commands which would change target electronic throttle position so that we can
 * see how far the result would be how soon.
 * <p>
 * Error standard deviation is posted to {@link Sensor#ETB_CONTROL_QUALITY}
 * <p>
 * https://github.com/rusefi/rusefi/issues/494
 * <p>
 * 11/16/2017
 * (c) Andrey Belomutskiy
 *
 * @see TimeBasedBuffer
 */
public class EtbTestSequence {
    protected static final ScheduledExecutorService executor = Executors.newSingleThreadScheduledExecutor();

    private static final String BUTTON_TEXT = "Measure Quality Control";

    private final JButton button = new JButton(BUTTON_TEXT);
    private final JLabel result = new JLabel("Result: N/A");

    public EtbTestSequence() {
        button.addActionListener(e -> {
            button.setEnabled(false);
            // 3000 data points at 10Hz should be 300 seconds worth of data
            StandardTestSequence.metric.start(/* buffer size: */3000, /*period, ms: */ 100);

            AtomicInteger stepCounter = new AtomicInteger();
            AtomicInteger totalSteps = new AtomicInteger();

            TestSequenceStep lastStep = new TestSequenceStep(SECOND) {
                @Override
                protected void doJob() {
                    button.setEnabled(true);
                    button.setText(BUTTON_TEXT);
                    double value = StandardTestSequence.metric.getStandardDeviation();
                    result.setText(String.format("Final Result: %.3f", value));
                }
            };

            Runnable onEachStep = new Runnable() {
                @Override
                public void run() {
                    SwingUtilities.invokeLater(new Runnable() {
                        @Override
                        public void run() {
                            String state = stepCounter.incrementAndGet() + "/" + totalSteps.get();
                            button.setText("Running " + state);
                            double value = StandardTestSequence.metric.getStandardDeviation();
                            result.setText(String.format(state + " Result: %.3f", value));
                        }
                    });
                }
            };

            TestSequenceStep firstStep = new EtbTarget(10 * SECOND, 4, /*position*/onEachStep);
            TestSequenceStep result = StandardTestSequence.addSequence(firstStep, onEachStep);
            result.addNext(lastStep);

            totalSteps.set(count(firstStep));

            firstStep.execute(executor);
        });
    }

    private static int count(TestSequenceStep step) {
        int result = 0;
        while ((step = step.getNext()) != null)
            result++;
        return result;
    }

    public JButton getButton() {
        return button;
    }

    public JLabel getResult() {
        return result;
    }
}
