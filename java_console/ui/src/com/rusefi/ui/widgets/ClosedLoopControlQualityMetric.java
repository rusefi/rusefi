package com.rusefi.ui.widgets;

import com.rusefi.CyclicBuffer;
import com.rusefi.DataBuffer;
import com.rusefi.core.Sensor;
import com.rusefi.core.SensorCentral;
import com.rusefi.core.SensorStats;
import com.rusefi.core.ValueSource;

import java.util.concurrent.Executors;
import java.util.concurrent.ScheduledExecutorService;
import java.util.concurrent.TimeUnit;

/**
 * https://github.com/rusefi/rusefi/issues/494
 * <p>
 * 11/16/2017
 * (c) Andrey Belomutskiy
 *
 * @see SensorStats
 */
public class ClosedLoopControlQualityMetric {
    private static final int BUFFER_SIZE = 100;
    private final ScheduledExecutorService executor = Executors.newSingleThreadScheduledExecutor();

    private final ValueSource target;
    private final ValueSource result;
    private final Sensor destination;

    /**
     * Buffer of recent error measurements
     * GuardedBy(this)
     */
    private DataBuffer errors = new CyclicBuffer(BUFFER_SIZE);

    /**
     * @param target what value are we trying to achieve
     * @param result what value do we actually have
     */
    public ClosedLoopControlQualityMetric(ValueSource target, ValueSource result, Sensor destination) {
        this.target = target;
        this.result = result;
        this.destination = destination;
    }

    public void start() {
        executor.scheduleAtFixedRate(() -> {
            rememberCurrentError(target.getValue() - result.getValue());
            SensorCentral.getInstance().setValue(getStandardDeviation(), destination);

        }, 0, 100, TimeUnit.MILLISECONDS);
    }

    public synchronized void reset() {
        errors.clear();
    }

    private synchronized double getStandardDeviation() {
        return DataBuffer.getStandardDeviation(errors.getValues());
    }

    private synchronized void rememberCurrentError(double error) {
        errors.add(error);
    }
}
