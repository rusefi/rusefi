package com.rusefi.io.serial;

import com.rusefi.Timeouts;

import java.util.LinkedList;

public class RateCounter {

    private final LinkedList<Pair> timeStamps = new LinkedList<>();

    /**
     * @return number of events in the last second
     */
    public int getCurrentRate() {
        return getCurrentRate(System.currentTimeMillis());
    }

    public synchronized int getCurrentRate(long now) {
        long threshold = now - Timeouts.SECOND;

        while (!timeStamps.isEmpty() && timeStamps.peekFirst().timestamp < threshold)
            timeStamps.removeFirst();

        int result = 0;
        for (Pair pair : timeStamps)
            result += pair.value;
        return result;
    }

    public void add() {
        add(System.currentTimeMillis());
    }

    public synchronized void add(long now) {
        timeStamps.add(new Pair(now, 1));
    }

    private static class Pair {
        private final long timestamp;
        private final int value;

        public Pair(long timestamp, int value) {
            this.timestamp = timestamp;
            this.value = value;
        }

        public long getTimestamp() {
            return timestamp;
        }

        public int getValue() {
            return value;
        }
    }
}
