package com.rusefi.ui.widgets;

import com.rusefi.FileLog;
import com.rusefi.TimeBasedBuffer;
import com.rusefi.core.MessagesCentral;
import com.rusefi.core.Sensor;
import com.rusefi.core.SensorCentral;
import com.rusefi.io.CommandQueue;
import org.jetbrains.annotations.NotNull;

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
    protected static final ScheduledExecutorService executor = Executors.newSingleThreadScheduledExecutor();

    protected final static ClosedLoopControlQualityMetric metric = createMetric();

    @NotNull
    protected static ClosedLoopControlQualityMetric createMetric() {
        return new ClosedLoopControlQualityMetric(
                SensorCentral.getInstance().getValueSource(Sensor.PPS),
                SensorCentral.getInstance().getValueSource(Sensor.TPS),
                Sensor.ETB_CONTROL_QUALITY
        );
    }

    private final static long SECOND = 1000;

    private final JButton button = new JButton("ETB Test");

    private SequenceStep FIRST_STEP = new EtbTarget(10 * SECOND, 4 /*position*/);

    {
        addSequence(FIRST_STEP);
    }

    protected static SequenceStep addSequence(SequenceStep first_step) {
        return first_step.addNext(new SequenceStep(SECOND) {
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
                .addNext(3 * SECOND, 0 /*position*/)
                .addNext(1 * SECOND, 6 /*position*/)
                .addNext(1 * SECOND, 8 /*position*/)
                .addNext(1 * SECOND, 6 /*position*/)
                .addNext(1 * SECOND, 4 /*position*/)
                .addNext(1 * SECOND, 2 /*position*/)
                .addNext(1 * SECOND, 0 /*position*/)
                .addNext(1 * SECOND, 10 /*position*/)
                .addNext(3 * SECOND, 7 /*position*/)
                .addNext(3 * SECOND, 6 /*position*/)
                .addNext(3 * SECOND, 5 /*position*/)
                .addNext(3 * SECOND, 4 /*position*/)
                .addNext(3 * SECOND, 3 /*position*/)
                .addNext(3 * SECOND, 2 /*position*/)
                .addNext(3 * SECOND, 1 /*position*/)
                .addNext(3 * SECOND, 0 /*position*/)
//                .addNext(10 * SECOND, 6 /*position*/)
//                .addNext(10 * SECOND, 30 /*position*/)
//                .addNext(10 * SECOND, 50 /*position*/)
//                .addNext(10 * SECOND, 70 /*position*/)
//                .addNext(10 * SECOND, 100 /*position*/)
//                .addNext(10 * SECOND, 50 /*position*/)
//                .addNext(10 * SECOND, 70 /*position*/)
//                .addNext(10 * SECOND, 4 /*position*/)
//                .addNext(10 * SECOND, 0 /*position*/)
                ;
    }

    public EtbTestSequence() {
        button.addActionListener(e -> {
            // 3000 data points at 10Hz should be 300 seconds worth of data
            metric.start(/* buffer size: */3000, /*period, ms: */ 100);
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
                FileLog.MAIN.logLine("Scheduling " + next + " with " + duration + "ms delay");
                executor.schedule(() -> next.execute(executor), duration, TimeUnit.MILLISECONDS);
            } else {
                MessagesCentral.getInstance().postMessage(EtbTestSequence.class, "ETB test sequence done!");
            }
        }

        protected abstract void doJob();

        public SequenceStep addNext(long duration, double position) {
            return addNext(new EtbTarget(duration, position));
        }

        public SequenceStep addNext(SequenceStep step) {
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
