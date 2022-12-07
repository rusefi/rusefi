package com.rusefi.core;

import java.nio.ByteBuffer;

import static com.rusefi.core.FileUtil.littleEndianWrap;

public interface ISensorHolder {
    default void grabSensorValues(byte[] response) {
        for (Sensor sensor : Sensor.values()) {
            if (sensor.getType() == null) {
                // for example ETB_CONTROL_QUALITY, weird use-case
                continue;
            }

            int offset = 1 + sensor.getOffset();
            int size = 4;
            if (offset + size > response.length) {
                throw new IllegalArgumentException(sensor + String.format(" but %d+%d in %d", offset, size, response.length));
            }
            ByteBuffer bb = littleEndianWrap(response, offset, size);

            double rawValue = sensor.getValueForChannel(bb);
            double scaledValue = rawValue * sensor.getScale();
            setValue(scaledValue, sensor);
        }
    }

    double getValue(Sensor sensor);

    boolean setValue(double value, Sensor sensor);
}
