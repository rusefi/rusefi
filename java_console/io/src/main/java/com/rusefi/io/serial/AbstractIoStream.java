package com.rusefi.io.serial;

import com.rusefi.io.IoStream;

import java.io.IOException;

public abstract class AbstractIoStream implements IoStream {
    private boolean isClosed;

    protected StreamStats streamStats = new StreamStats();

    @Override
    public StreamStats getStreamStats() {
        return streamStats;
    }

    @Override
    public void close() {
        isClosed = true;
    }

    @Override
    public void flush() throws IOException {
    }

    @Override
    public boolean isClosed() {
        return isClosed;
    }

    public class StreamStats {
        private long previousPacketArrivalTime;
        private int maxPacketGap;

        public long getPreviousPacketArrivalTime() {
            return previousPacketArrivalTime;
        }

        /**
         * @return maximum time in MS between full valid packets received via this stream
         */
        public int getMaxPacketGap() {
            return maxPacketGap;
        }

        public void onPacketArrived() {
            long now = System.currentTimeMillis();
            if (previousPacketArrivalTime != 0) {
                maxPacketGap = (int) Math.max(maxPacketGap, now - previousPacketArrivalTime);
            }
            previousPacketArrivalTime = now;
        }
    }
}
