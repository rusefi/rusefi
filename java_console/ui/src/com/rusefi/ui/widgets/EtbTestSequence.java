package com.rusefi.ui.widgets;

import com.rusefi.FileLog;
import com.rusefi.TimeBasedBuffer;
import com.rusefi.core.MessagesCentral;
import com.rusefi.core.Sensor;
import com.rusefi.core.SensorCentral;
import com.rusefi.io.CommandQueue;

import javax.swing.*;
import java.util.concurrent.Executors;
import java.util.concurrent.ScheduledExecutorService;
import java.util.concurrent.TimeUnit;

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
    private final ScheduledExecutorService executor = Executors.newSingleThreadScheduledExecutor();
    private boolean isStarted;

    protected final ClosedLoopControlQualityMetric metric = new ClosedLoopControlQualityMetric(
            SensorCentral.getInstance().getValueSource(Sensor.PPS),
            SensorCentral.getInstance().getValueSource(Sensor.TPS),
            Sensor.ETB_CONTROL_QUALITY
    );

    private final static long SECOND = 1000;

    private final JButton button = new JButton("ETB Test");

    private SequenceStep FIRST_STEP = new EtbTarget(10 * SECOND, 4 /*position*/);

    {
        FIRST_STEP.add(new SequenceStep(SECOND) {
            @Override
            protected void doJob() {
                metric.reset();
            }
        })
                .addNext(10 * SECOND, 4 /*position*/)
                .addNext(5 * SECOND, 6 /*position*/)
                .addNext(5 * SECOND, 8 /*position*/)
                .addNext(5 * SECOND, 6 /*position*/)
                .addNext(5 * SECOND, 4 /*position*/)
                .addNext(5 * SECOND, 2 /*position*/)
                .addNext(5 * SECOND, 0 /*position*/)
                .addNext(5 * SECOND, 10 /*position*/)
                .addNext(10 * SECOND, 6 /*position*/)
                .addNext(10 * SECOND, 30 /*position*/)
                .addNext(10 * SECOND, 50 /*position*/)
                .addNext(10 * SECOND, 70 /*position*/)
                .addNext(10 * SECOND, 100 /*position*/)
                .addNext(10 * SECOND, 50 /*position*/)
                .addNext(10 * SECOND, 70 /*position*/)
                .addNext(10 * SECOND, 4 /*position*/)
                .addNext(10 * SECOND, 0 /*position*/)
        ;
    }

    public EtbTestSequence() {
        button.addActionListener(e -> {
            if (!isStarted) {
                metric.start(/* buffer size: */1000, /*period, ms: */ 100);
                isStarted = true;
            }
            FIRST_STEP.execute(executor);
        });
    }

    public JButton getButton() {
        return button;
    }

    static abstract class SequenceStep {
        final long duration;
        SequenceStep next;

        public SequenceStep(long duration) {
            this.duration = duration;
        }

        public void execute(ScheduledExecutorService executor) {
            doJob();
            if (next != null) {
                FileLog.MAIN.logLine("Scheduling " + next);
                executor.schedule(() -> next.execute(executor), duration, TimeUnit.MILLISECONDS);
            } else {
                MessagesCentral.getInstance().postMessage(EtbTestSequence.class, "ETB test sequence done!");
            }
        }

        protected abstract void doJob();

        public SequenceStep addNext(long duration, double position) {
            return add(new EtbTarget(duration, position));
        }

        private SequenceStep add(SequenceStep step) {
            next = step;
            return next;
        }
    }

    static class EtbTarget extends SequenceStep {
        /**
         * 0-100 percent open
         */
        final double position;

        public EtbTarget(long duration, double position) {
            super(duration);
            this.position = position;
        }

        protected void doJob() {
            CommandQueue.getInstance().write("set mock_pedal_position " + position);
        }

        @Override
        public String toString() {
            return "EtbTarget{" +
                    "duration=" + duration +
                    ", position=" + position +
                    '}';
        }
    }
}
