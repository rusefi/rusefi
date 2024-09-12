package com.rusefi.io.serial;

import com.rusefi.binaryprotocol.IncomingDataBuffer;
import com.rusefi.io.IoStream;

import java.io.IOException;
import java.util.concurrent.CopyOnWriteArrayList;
import java.util.concurrent.atomic.AtomicInteger;

public abstract class AbstractIoStream implements IoStream {
    private boolean isClosed;

    // todo: this ioLock needs better documentation!
    private final Object ioLock = new Object();

    protected final StreamStats streamStats = new StreamStats();
    private final AtomicInteger bytesOut = new AtomicInteger();
    private long latestActivity;
    private final CopyOnWriteArrayList<Runnable> closeListeners = new CopyOnWriteArrayList<>();

    public IncomingDataBuffer createDataBuffer() {
        IncomingDataBuffer incomingData = new IncomingDataBuffer(getClass().getSimpleName(), getStreamStats());
        setInputListener(incomingData::addData);
        return incomingData;
    }

    @Override
    public StreamStats getStreamStats() {
        return streamStats;
    }

    @Override
    public void close() {
        if (isClosed)
            return;
        isClosed = true;
        for (Runnable listener : closeListeners)
            listener.run();
    }

    @Override
    public void addCloseListener(Runnable listener) {
        closeListeners.add(listener);
    }

    @Override
    public Object getIoLock() {
        return ioLock;
    }

    @Override
    public void write(byte[] bytes) throws IOException {
        bytesOut.addAndGet(bytes.length);
    }

    @Override
    public void flush() throws IOException {
    }

    @Override
    public void onActivity() {
        latestActivity = System.currentTimeMillis();
    }

    public long latestActivityTime() {
        return latestActivity;
    }

    @Override
    public boolean isClosed() {
        return isClosed;
    }

    public class StreamStats {
        private long previousPacketArrivalTime;
        private int maxPacketGap;
        private final AtomicInteger totalBytesArrived = new AtomicInteger();

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
            AbstractIoStream.this.onActivity();
        }

        public void onArrived(int length) {
            totalBytesArrived.addAndGet(length);
        }
    }

    @Override
    public int getBytesIn() {
        return streamStats.totalBytesArrived.get();
    }

    @Override
    public int getBytesOut() {
        return bytesOut.get();
    }
}
