package com.rusefi;

public class TriggerSignal {
    final double angle;
    final int state;
    final int waveIndex;

    public TriggerSignal(int waveIndex, int state, double angle) {
        this.waveIndex = waveIndex;
        this.state = state;
        this.angle = angle;
    }

    @Override
    public String toString() {
        return "Signal{" +
                "signal=" + waveIndex +
                ", state=" + state +
                ", angle=" + angle +
                '}';
    }
}
