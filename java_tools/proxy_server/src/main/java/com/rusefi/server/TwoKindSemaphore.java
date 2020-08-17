package com.rusefi.server;

import com.devexperts.logging.Logging;

import java.util.concurrent.Semaphore;
import java.util.concurrent.TimeUnit;

public class TwoKindSemaphore {
    private final static Logging log = Logging.getLogging(TwoKindSemaphore.class);

    // we have to distinguish between long-term usage by application and short-term usage just to refresh gauges
    private final static int LONG_TERM = 2;
    private final static int SHORT_TERM = 1;
    private final Semaphore semaphore = new Semaphore(LONG_TERM);
    private UserDetails owner;

    public void releaseFromLongTermUsage() {
        log.info("release " + owner);
        semaphore.release(LONG_TERM);
        // not atomic but that's fine, isUsed is the source of truth
        owner = null;
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
     * @param tuner
     */
    public boolean acquireForLongTermUsage(UserDetails tuner) {
        return acquireForLongTermUsage(tuner, 10);
    }

    public boolean acquireForLongTermUsage(UserDetails tuner, int timeout) {
        try {
            boolean isAcquired = semaphore.tryAcquire(LONG_TERM, timeout, TimeUnit.SECONDS);
            if (isAcquired) {
                owner = tuner;
            }
            return isAcquired;
        } catch (InterruptedException e) {
            throw new IllegalStateException(e);
        }
    }

    public UserDetails getOwner() {
        return owner;
    }

    @Override
    public String toString() {
        return "TwoKindSemaphore{" +
                "semaphore=" + semaphore +
                ", owner=" + owner +
                '}';
    }
}
