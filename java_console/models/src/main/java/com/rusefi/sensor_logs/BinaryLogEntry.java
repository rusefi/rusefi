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

    default int getDataSize() {
        switch (getByteSize()) {
            case 0:
            case 1:
                return 1;
            case 2:
            case 3:
                return 2;
            case 4:
            case 5:
            case 7:
                return 4;
            case 6:
                return 8;
            default:
                throw new UnsupportedOperationException("MLG type " + getByteSize());
        }
    }

    default float getScale() {
        return 1;
    }

    void writeToLog(DataOutputStream dos, double value) throws IOException;
}
