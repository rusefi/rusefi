package com.rusefi.ui.widgets;

import com.rusefi.core.Sensor;
import com.rusefi.core.SensorCentral;
import com.rusefi.core.SensorStats;
import com.rusefi.core.ValueSource;

import java.util.LinkedList;
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
    private final ScheduledExecutorService executor = Executors.newSingleThreadScheduledExecutor();

    private final ValueSource target;
    private final ValueSource result;
    private final Sensor destination;
    private int maxSize = 100;

    /**
     * Last maxSize number of error measurements
     * GuardedBy(this)
     */
    private LinkedList<Double> errors = new LinkedList<>();

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
        float sum = 0;
        for (Double error : errors) {
            sum += error * error;
        }
        return Math.sqrt(sum / errors.size());
    }

    private synchronized void rememberCurrentError(double error) {
        if (errors.size() == maxSize)
            errors.removeFirst();
        errors.add(error);
    }
}
