package com.rusefi.trigger;

public class ComplexTriggerSignal extends TriggerEvent {
    private double width;
    private int count;
    private double lastAngle;

    public ComplexTriggerSignal(int waveIndex, double angle, double width, int count, double lastAngle) {
        super(waveIndex, angle);
        this.width = width;
        this.count = count;
        this.lastAngle = lastAngle;
    }

    public ComplexTriggerSignal(int waveIndex, double angle, double width) {
        this(waveIndex, angle, width, 1, angle);
    }

    @Override
    public String toString() {
        return "Signal{" +
                "signal=" + getWaveIndex() +
                ", angle=" + getAngle() +
                ", width=" + getWidth() +
                ", count=" + getCount() +
                ", lastAngle=" + getLastAngle() +
                '}';
    }

    public double getWidth() {
        return width;
    }

    public int getCount() {
        return count;
    }

    public double getLastAngle() {
        return lastAngle;
    }

    public void setWidth(double width) {
        this.width = width;
    }

    public void setCount(int count) {
        this.count = count;
    }

    public void setLastAngle(double lastAngle) {
        this.lastAngle = lastAngle;
    }

    public void adjustAngle(double delta, double wrap) {
        super.adjustAngle(delta, wrap);
        this.lastAngle = lastAngle + delta;
				if (lastAngle < 0) {
						this.lastAngle += wrap;
				}
    }
}
