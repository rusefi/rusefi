package com.rusefi.trigger;

public class TriggerSignal extends TriggerEvent {
    private final int state;
    private double gap;

    public TriggerSignal(int waveIndex, double angle, int state, double gap) {
        super(waveIndex, angle);
        this.state = state;
        this.gap = gap;
    }

    public TriggerSignal(int waveIndex, double angle, int state) {
        this(waveIndex, angle, state, Double.NaN);
    }

    @Override
    public String toString() {
        return "Signal{" +
                "signal=" + getWaveIndex() +
                ", angle=" + getAngle() +
                ", state=" + getState() +
                ", gap=" + getGap() +
                '}';
    }

    public int getState() {
        return state;
    }

    public double getGap() {
        return gap;
    }

    public void setGap(double gap) {
        this.gap = gap;
    }
}
