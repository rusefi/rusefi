package com.rusefi.etb;

import com.rusefi.io.CommandQueue;

public class EtbTarget extends TestSequenceStep {
    /**
     * 0-100 percent open
     */
    private final double position;
    private final Runnable onEachStep;

    public EtbTarget(long duration, double position, Runnable onEachStep) {
        super(duration);
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
                "duration=" + duration +
                ", position=" + position +
                '}';
    }
}
