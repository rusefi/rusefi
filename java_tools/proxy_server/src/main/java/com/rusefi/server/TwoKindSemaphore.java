package com.rusefi.server;

import java.util.concurrent.Semaphore;
import java.util.concurrent.TimeUnit;

public class TwoKindSemaphore {
    // we have to distinguish between long-term usage by application and short-term usage just to refresh gauges
    private final static int LONG_TERM = 2;
    private final static int SHORT_TERM = 1;
    private final Semaphore semaphore = new Semaphore(LONG_TERM);

    public void releaseFromLongTermUsage() {
        semaphore.release(LONG_TERM);
    }

    public boolean acquireForShortTermUsage() {
        return semaphore.tryAcquire(SHORT_TERM);
    }

    public void releaseFromShortTermUsage() {
        semaphore.release(SHORT_TERM);
    }

    public boolean isUsed() {
        // short-term usages of only one permit does not count
        return semaphore.availablePermits() == 0;
    }

    /**
     * @return true if acquired successfully, false if not
     */
    public boolean acquireForLongTermUsage() {
        return acquireForLongTermUsage(10);
    }

    public boolean acquireForLongTermUsage(int timeout) {
        try {
            return semaphore.tryAcquire(LONG_TERM, timeout, TimeUnit.SECONDS);
        } catch (InterruptedException e) {
            throw new IllegalStateException(e);
        }
    }
}
