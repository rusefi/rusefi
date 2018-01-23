package com.rusefi.ui.widgets;

import com.rusefi.FileLog;
import com.rusefi.SensorSnifferCentral;
import com.rusefi.core.Sensor;
import com.rusefi.core.SensorCentral;
import com.rusefi.io.CommandQueue;

import javax.swing.*;
import java.util.concurrent.Executors;
import java.util.concurrent.ScheduledExecutorService;
import java.util.concurrent.TimeUnit;

/**
 * https://github.com/rusefi/rusefi/issues/494
 * <p>
 * Here we initiate a sequence of commands which would change target electronic throttle position so that we can
 * see how far the result would be how soon.
 * <p>
 * 11/16/2017
 * (c) Andrey Belomutskiy
 */
public class EtbTestSequence {
    private final ScheduledExecutorService executor = Executors.newSingleThreadScheduledExecutor();
    private boolean isStarted;

    private ClosedLoopControlQualityMetric metric = new ClosedLoopControlQualityMetric(
            SensorCentral.getInstance().getValueSource(Sensor.PPS),
            SensorCentral.getInstance().getValueSource(Sensor.TPS),
            Sensor.ETB_CONTROL_QUALITY
    );

    private final static long SECOND = 1000;

    private final JButton button = new JButton("ETB Test");

    private static EtbTarget FIRST_STEP = new EtbTarget(SECOND, 0);

    static {
        FIRST_STEP
            .addNext(5 * SECOND, 30)
            .addNext(10 * SECOND, 50);
    }

    public EtbTestSequence() {
        button.addActionListener(e -> {
            if (!isStarted) {
                metric.start();
                isStarted = true;
            }
            FIRST_STEP.execute(executor);
        });
    }

    public JButton getButton() {
        return button;
    }

    static class EtbTarget {
        final long duration;
        /**
         * 0-100 percent open
         */
        final double position;

        EtbTarget next;

        public EtbTarget(long duration, double position) {
            this.duration = duration;
            this.position = position;
        }

        public EtbTarget addNext(long duration, double position) {
            next = new EtbTarget(duration, position);
            return next;
        }

        public void execute(ScheduledExecutorService executor) {
            CommandQueue.getInstance().write("set mock_pedal_position " + position);
            if (next != null) {
                FileLog.MAIN.logLine("Scheduling " + next);
                executor.schedule(() -> next.execute(executor),
                        duration, TimeUnit.MILLISECONDS);
            }
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
