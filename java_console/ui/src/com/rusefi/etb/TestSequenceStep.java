package com.rusefi.etb;

import com.rusefi.FileLog;
import com.rusefi.core.MessagesCentral;

import java.util.concurrent.ScheduledExecutorService;
import java.util.concurrent.TimeUnit;

public abstract class TestSequenceStep {
    /**
     * First we execute {{@link #doJob()}} then we schedule next step for after 'nextStepDelay' time passes.
     */
    protected final long nextStepDelay;
    private final EtbTarget.Condition condition;
    private TestSequenceStep next;

    public TestSequenceStep(long nextStepDelay, EtbTarget.Condition condition) {
        this.nextStepDelay = nextStepDelay;
        this.condition = condition;
    }

    public static int count(TestSequenceStep step) {
        int result = 0;
        while ((step = step.getNext()) != null)
            result++;
        return result;
    }

    public void execute(ScheduledExecutorService executor) {
        boolean shouldRun = condition.shouldRunTask();
        if (shouldRun)
            doJob();
        if (next != null) {
            if (shouldRun) {
                FileLog.MAIN.logLine("Scheduling " + next + " with " + nextStepDelay + "ms delay");
                executor.schedule(() -> next.execute(executor), nextStepDelay, TimeUnit.MILLISECONDS);
            } else {
                // we've skipped current job and we skip the delay as well
                next.execute(executor);
            }
        } else {
            MessagesCentral.getInstance().postMessage(TestSequenceStep.class, "ETB test sequence done!");
        }
    }

    protected abstract void doJob();

    public TestSequenceStep addNext(long duration, double position, Runnable onEachStep, Condition condition) {
        return addNext(new EtbTarget(duration, position, onEachStep, condition));
    }

    public TestSequenceStep addNext(TestSequenceStep step) {
        next = step;
        return next;
    }

    public TestSequenceStep getNext() {
        return next;
    }

    public interface Condition {
        boolean shouldRunTask();

        Condition YES = () -> true;
    }
}
