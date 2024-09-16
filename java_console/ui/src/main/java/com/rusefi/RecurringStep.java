package com.rusefi;

import com.devexperts.logging.Logging;

public class RecurringStep {
    private final static Logging log = Logging.getLogging(RecurringStep.class);

    private final Runnable initialStep;
    private final Runnable stepToRepeat;
    private final String threadName;
    private volatile boolean isRunning = true;

    public RecurringStep(final Runnable initialStep, final Runnable stepToRecur, final String threadName) {
        this.initialStep = initialStep;
        this.stepToRepeat = stepToRecur;
        this.threadName = threadName;
    }

    void start() {
        final Thread workerThread = new Thread(() -> {
            boolean isFirstTime = true;
            while (isRunning) {
                if (isFirstTime) {
                    initialStep.run();
                    isFirstTime = false;
                } else {
                    stepToRepeat.run();
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

    void stop() {
        isRunning = false;
    }
}
