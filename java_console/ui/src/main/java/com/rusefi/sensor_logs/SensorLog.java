package com.rusefi.sensor_logs;

public abstract class SensorLog {
    public abstract double getSecondsSinceFileStart();

    abstract void writeSensorLogLine();
}
