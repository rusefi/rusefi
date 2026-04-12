package com.rusefi.trigger;

public class TriggerSignal {
    private final double angle;
    private final int state;
    private final int waveIndex;
    private final double gap;

    public TriggerSignal(int waveIndex, int state, double angle, double gap) {
        this.waveIndex = waveIndex;
        this.state = state;
        this.angle = angle;
        this.gap = gap;
    }

    @Override
    public String toString() {
        return "Signal{" +
                "signal=" + getWaveIndex() +
                ", state=" + getState() +
                ", angle=" + getAngle() +
                '}';
    }

    public double getAngle() {
        return angle;
    }

    public int getState() {
        return state;
    }

    public int getWaveIndex() {
        return waveIndex;
    }

    public double getGap() {
        return gap;
    }
}
