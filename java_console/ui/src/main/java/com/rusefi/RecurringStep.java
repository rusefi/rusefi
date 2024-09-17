package com.rusefi;

import com.devexperts.logging.Logging;

import java.util.concurrent.CountDownLatch;

public class RecurringStep {
    private final static Logging log = Logging.getLogging(RecurringStep.class);

    private final Runnable initialStep;
    private final Runnable stepToRepeat;
    private final String threadName;
    private volatile boolean isStopped = false;
    private volatile boolean isSuspended = false;
    private volatile CountDownLatch suspendedCountDownLatch = createSuspendedCountDownLatch();

    public RecurringStep(final Runnable initialStep, final Runnable stepToRecur, final String threadName) {
        this.initialStep = initialStep;
        this.stepToRepeat = stepToRecur;
        this.threadName = threadName;
    }

    public void start() {
        final Thread workerThread = new Thread(() -> {
            boolean isFirstTime = true;
            while (!isStopped) {
                if (!checkSuspended()) {
                    if (isFirstTime) {
                        initialStep.run();
                        isFirstTime = false;
                    } else {
                        stepToRepeat.run();
                    }
                }
                try {
                    Thread.sleep(300);
                } catch (InterruptedException e) {
                    log.error("sleep interrupted", e);
                }
            }
        }, threadName);
        workerThread.setDaemon(true);
        workerThread.start();
    }

    public void stop() {
        isStopped = true;
    }

    synchronized void resume() {
        isSuspended = false;
    }

    synchronized CountDownLatch suspend() {
        if (!isSuspended) {
            isSuspended = true;
            suspendedCountDownLatch = createSuspendedCountDownLatch();
        }
        return suspendedCountDownLatch;
    }

    private synchronized boolean checkSuspended() {
        if (isSuspended) {
            suspendedCountDownLatch.countDown();
        }
        return isSuspended;
    }

    private static CountDownLatch createSuspendedCountDownLatch() {
        return new CountDownLatch(1);
    }
}
