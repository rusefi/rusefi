package com.rusefi.sensor_logs;

public interface SensorLog {
    double getSecondsSinceFileStart();

    void writeSensorLogLine();

    void close();
}
