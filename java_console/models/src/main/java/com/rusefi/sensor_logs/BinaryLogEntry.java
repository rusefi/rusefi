package com.rusefi.sensor_logs;

import java.io.DataOutputStream;
import java.io.IOException;

public interface BinaryLogEntry {
    String getName();

    String getUnit();

    int getByteSize();

    void writeToLog(DataOutputStream dos, double value) throws IOException;
}
