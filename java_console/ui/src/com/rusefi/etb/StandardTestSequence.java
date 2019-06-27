package com.rusefi.etb;

import com.rusefi.core.Sensor;
import com.rusefi.core.SensorCentral;
import org.jetbrains.annotations.NotNull;

import static com.rusefi.Timeouts.SECOND;
import static com.rusefi.ui.etb.EtbTestSequence.PAST_DEPTH;

public class StandardTestSequence {
    public final static ClosedLoopControlQualityMetric metric = createMetric();

    @NotNull
    protected static ClosedLoopControlQualityMetric createMetric() {
        return new ClosedLoopControlQualityMetric(
                SensorCentral.getInstance().getValueSource(Sensor.PPS),
                SensorCentral.getInstance().getValueSource(Sensor.TPS),
                Sensor.ETB_CONTROL_QUALITY,
                PAST_DEPTH
        );
    }

    public static TestSequenceStep addSequence(TestSequenceStep first_step, Runnable onEachStep, TestSequenceStep.Condition condition) {
        TestSequenceStep secondStep = new TestSequenceStep(SECOND, EtbTarget.Condition.YES) {
            @Override
            protected void doJob() {
                metric.reset();
            }
        };
        TestSequenceStep result = first_step.addNext(secondStep)
                .addNext(10 * SECOND, 4 /*position*/, onEachStep, condition)
                .addNext(5 * SECOND, 6, /*position*/onEachStep, condition)
                .addNext(5 * SECOND, 8, /*position*/onEachStep, condition)
                .addNext(5 * SECOND, 6, /*position*/onEachStep, condition)
                .addNext(5 * SECOND, 4, /*position*/onEachStep, condition)
                .addNext(5 * SECOND, 2, /*position*/onEachStep, condition)
                .addNext(5 * SECOND, 0, /*position*/onEachStep, condition)
                .addNext(5 * SECOND, 10, /*position*/onEachStep, condition)
                .addNext(3 * SECOND, 0, /*position*/onEachStep, condition)
                .addNext(1 * SECOND, 6, /*position*/onEachStep, condition)
                .addNext(1 * SECOND, 8, /*position*/onEachStep, condition)
                .addNext(1 * SECOND, 6, /*position*/onEachStep, condition)
                .addNext(1 * SECOND, 4, /*position*/onEachStep, condition)
                .addNext(1 * SECOND, 2, /*position*/onEachStep, condition)
                .addNext(1 * SECOND, 0, /*position*/onEachStep, condition)
                .addNext(1 * SECOND, 10, /*position*/onEachStep, condition)
                .addNext(3 * SECOND, 7, /*position*/onEachStep, condition)
                .addNext(3 * SECOND, 6, /*position*/onEachStep, condition)
                .addNext(3 * SECOND, 5, /*position*/onEachStep, condition)
                .addNext(3 * SECOND, 4, /*position*/onEachStep, condition)
                .addNext(3 * SECOND, 3, /*position*/onEachStep, condition)
                .addNext(3 * SECOND, 2, /*position*/onEachStep, condition)
                .addNext(3 * SECOND, 1, /*position*/onEachStep, condition)
                .addNext(3 * SECOND, 0, /*position*/onEachStep, condition)
                .addNext(10 * SECOND, 6, /*position*/onEachStep, condition)
                .addNext(10 * SECOND, 30, /*position*/onEachStep, condition)
                .addNext(10 * SECOND, 50, /*position*/onEachStep, condition)
                .addNext(10 * SECOND, 70, /*position*/onEachStep, condition)
                .addNext(10 * SECOND, 95, /*position*/onEachStep, condition)
                .addNext(10 * SECOND, 50, /*position*/onEachStep, condition)
                .addNext(10 * SECOND, 70, /*position*/onEachStep, condition)
                .addNext(10 * SECOND, 4, /*position*/onEachStep, condition)
                .addNext(10 * SECOND, 0, /*position*/onEachStep, condition)
                ;
        return result;
    }
}
