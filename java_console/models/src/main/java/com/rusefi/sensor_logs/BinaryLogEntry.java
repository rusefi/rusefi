package com.rusefi.sensor_logs;

import com.rusefi.core.SensorCategory;

import java.io.DataOutputStream;
import java.io.IOException;

public interface BinaryLogEntry {
    String getName();

    // TODO: getCategory() should return String
    SensorCategory getCategory();

    String getUnit();

    int getByteSize();

    void writeToLog(DataOutputStream dos, double value) throws IOException;
}
