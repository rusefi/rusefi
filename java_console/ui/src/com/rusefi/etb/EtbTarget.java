package com.rusefi.etb;

import com.rusefi.io.CommandQueue;

public class EtbTarget extends TestSequenceStep {
    /**
     * 0-100 percent open
     */
    private final double position;
    private final Runnable onEachStep;

    public EtbTarget(long duration, double position, Runnable onEachStep, Condition condition) {
        super(duration, condition);
        this.position = position;
        this.onEachStep = onEachStep;
    }

    protected void doJob() {
        CommandQueue.getInstance().write("set mock_pedal_position " + position);
        if (onEachStep != null)
            onEachStep.run();
    }

    @Override
    public String toString() {
        return "EtbTarget{" +
                "nextStepDelay=" + nextStepDelay +
                ", position=" + position +
                '}';
    }
}
