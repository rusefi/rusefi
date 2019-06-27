package com.rusefi.etb;

import com.rusefi.CyclicBuffer;
import com.rusefi.DataBuffer;
import com.rusefi.NamedThreadFactory;
import com.rusefi.core.Sensor;
import com.rusefi.core.SensorCentral;
import com.rusefi.core.SensorStats;
import com.rusefi.core.ValueSource;

import java.util.Objects;
import java.util.concurrent.Executors;
import java.util.concurrent.ScheduledExecutorService;
import java.util.concurrent.TimeUnit;

/**
 * https://github.com/rusefi/rusefi/issues/494
 * <p>
 * https://en.wikipedia.org/wiki/Standard_deviation of error posted to specified sensor
 * <p>
 * 11/16/2017
 * (c) Andrey Belomutskiy
 *
 * @see SensorStats
 */
public class ClosedLoopControlQualityMetric {
    private final ScheduledExecutorService executor = Executors.newSingleThreadScheduledExecutor(new NamedThreadFactory(ClosedLoopControlQualityMetric.class.getSimpleName()));
    private final int delayDepth;

    private final ValueSource target;
    private final ValueSource result;
    private final Sensor destination;
    private boolean isStarted;

    /**
     * Buffer of recent error measurements
     * GuardedBy(this)
     */
    private DataBuffer errorsBuffer;
    private CyclicBuffer targetBuffer;

    /**
     * @param delayDepth
     * @param target     what value are we trying to achieve
     * @param result     what value do we actually have
     */
    public ClosedLoopControlQualityMetric(ValueSource target, ValueSource result, Sensor destination, int delayDepth) {
        this.target = target;
        this.result = result;
        this.destination = destination;
        this.delayDepth = delayDepth;
    }

    public void start(int bufferSize, int periodMs) {
        if (isStarted)
            return;
        isStarted = true;

        create(bufferSize);
        executor.scheduleAtFixedRate(() -> {
            add();
            SensorCentral.getInstance().setValue(getStandardDeviation(), destination);

        }, 0, periodMs, TimeUnit.MILLISECONDS);
    }

    public void add() {
        double targetValue = target.getValue();
        double resultValue = result.getValue();
        double error = Math.abs(targetValue - resultValue);
        int pointer = targetBuffer.getPointer();
        for (int i = 0; i < Math.min(delayDepth - 1, targetBuffer.getSize()); i++) {
            double thisError = Math.abs(targetBuffer.get(pointer - i) - resultValue);
            error = Math.min(error, thisError);
        }
        rememberCurrentError(error, targetValue);
    }

    public void create(int bufferSize) {
        errorsBuffer = new CyclicBuffer(bufferSize);
        targetBuffer = new CyclicBuffer(delayDepth);
    }

    public synchronized void reset() {
        errorsBuffer.clear();
    }

    public synchronized double getStandardDeviation() {
        return DataBuffer.getStandardDeviation(errorsBuffer.getValues());
    }

    private synchronized void rememberCurrentError(double error, double targetValue) {
        Objects.requireNonNull(errorsBuffer, "errorsBuffer");
        errorsBuffer.add(error);
        targetBuffer.add(targetValue);
    }
}
