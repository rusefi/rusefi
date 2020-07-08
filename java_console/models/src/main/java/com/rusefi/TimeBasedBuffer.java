package com.rusefi;

import java.util.TreeMap;

/**
 * Last X seconds of values.
 * <p>
 * This data structure holds only one value for each millisecond timestamp but that should be totally fine
 * for our purposes
 *
 * @see ClosedLoopControlQualityMetric
 * @see EtbTestSequence
 */
public class TimeBasedBuffer implements DataBuffer {
    private final TreeMap<Long, Double> values = new TreeMap<>();
    private final long duration;

    public TimeBasedBuffer(long duration) {
        this.duration = duration;
    }

    private synchronized void truncate() {
        long now = currentTimeMillis();
        values.headMap(now - duration).clear();
    }

    @Override
    public synchronized void clear() {
        values.clear();
    }

    protected long currentTimeMillis() {
        return System.currentTimeMillis();
    }

    @Override
    public synchronized void add(double value) {
        values.put(currentTimeMillis(), value);
    }

    public synchronized int getSize() {
        truncate();
        return values.size();
    }

    public synchronized double[] getValues() {
        truncate();
        double[] result = new double[values.size()];
        int i = 0;
        for (Double v : values.values())
            result[i++] = v;
        return result;
    }
}
