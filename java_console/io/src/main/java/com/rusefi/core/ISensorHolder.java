package com.rusefi.core;

import com.opensr5.ini.IniFileModel;
import org.jetbrains.annotations.NotNull;

import java.nio.ByteBuffer;

import static com.rusefi.core.FileUtil.littleEndianWrap;

public interface ISensorHolder {
    default void grabSensorValues(byte[] response, @NotNull IniFileModel binaryProtocol) {
        for (Sensor sensor : Sensor.values()) {
            if (sensor.getType() == null) {
                // for example ETB_CONTROL_QUALITY, weird use-case
                continue;
            }

            ByteBuffer bb = getByteBufferForSensor(response, sensor);

            double rawValue = sensor.getValueForChannel(bb);
            double scaledValue = rawValue * sensor.getScale();
            setValue(scaledValue, sensor);
        }
    }

    @Deprecated
    static @NotNull ByteBuffer getByteBufferForSensor(byte[] response, Sensor sensor) {
        return getByteBuffer(response, sensor.toString(), sensor.getOffset());
    }

    static @NotNull ByteBuffer getByteBuffer(byte[] response, String message, int fieldOffset) {
        int offset = fieldOffset + 1; // first byte is response code
        int size = 4;
        if (offset + size > response.length) {
            throw new IllegalArgumentException(message + String.format(" but %d+%d in %d", offset, size, response.length));
        }
        return littleEndianWrap(response, offset, size);
    }

    double getValue(Sensor sensor);

    boolean setValue(double value, Sensor sensor);
}
