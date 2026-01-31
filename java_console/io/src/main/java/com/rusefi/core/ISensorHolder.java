package com.rusefi.core;

import com.opensr5.ini.IniFileModel;
import com.opensr5.ini.IniMemberNotFound;
import com.opensr5.ini.field.IniField;
import org.jetbrains.annotations.NotNull;

import java.nio.ByteBuffer;

import static com.rusefi.core.ByteBufferUtil.littleEndianWrap;

public interface ISensorHolder {
    default void grabSensorValues(byte[] response, @NotNull IniFileModel ini) {
        for (Sensor sensor : Sensor.values()) {
            if (sensor.getType() == null) {
                // for example ETB_CONTROL_QUALITY, weird use-case
                continue;
            }
            IniField sensorField;
            String sensorInternalName = sensor.getNativeName();
            try {
                sensorField = ini.getOutputChannel(sensorInternalName);
            } catch (IniMemberNotFound e) {
                throw new RuntimeException("Not found by " + sensorInternalName, e);
            }

            ByteBuffer bb = getByteBuffer(response, sensor.toString(), sensorField.getOffset());

            double rawValue = sensor.getValueForChannel(bb);
            double scaledValue = rawValue * sensor.getScale();
            setValue(scaledValue, sensor);
        }
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
