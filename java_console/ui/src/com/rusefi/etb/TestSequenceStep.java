package com.rusefi.etb;

import com.rusefi.FileLog;
import com.rusefi.core.MessagesCentral;

import java.util.concurrent.ScheduledExecutorService;
import java.util.concurrent.TimeUnit;

public abstract class TestSequenceStep {
    protected final long duration;
    private TestSequenceStep next;

    public TestSequenceStep(long duration) {
        this.duration = duration;
    }

    public void execute(ScheduledExecutorService executor) {
        doJob();
        if (next != null) {
            FileLog.MAIN.logLine("Scheduling " + next + " with " + duration + "ms delay");
            executor.schedule(() -> next.execute(executor), duration, TimeUnit.MILLISECONDS);
        } else {
            MessagesCentral.getInstance().postMessage(TestSequenceStep.class, "ETB test sequence done!");
        }
    }

    protected abstract void doJob();

    public TestSequenceStep addNext(long duration, double position, Runnable onEachStep) {
        return addNext(new EtbTarget(duration, position, onEachStep));
    }

    public TestSequenceStep addNext(TestSequenceStep step) {
        next = step;
        return next;
    }

    public TestSequenceStep getNext() {
        return next;
    }
}
