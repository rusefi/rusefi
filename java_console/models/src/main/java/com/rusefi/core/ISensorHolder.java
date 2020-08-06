package com.rusefi.core;

import java.nio.ByteBuffer;
import java.nio.ByteOrder;

public interface ISensorHolder {
    default void grabSensorValues(byte[] response) {
        for (Sensor sensor : Sensor.values()) {
            if (sensor.getType() == null) {
                // for example ETB_CONTROL_QUALITY, weird use-case
                continue;
            }

            ByteBuffer bb = ByteBuffer.wrap(response, 1 + sensor.getOffset(), 4);
            bb.order(ByteOrder.LITTLE_ENDIAN);

            double rawValue = sensor.getValueForChannel(bb);
            double scaledValue = rawValue * sensor.getScale();
            setValue(scaledValue, sensor);
        }
    }

    double getValue(Sensor sensor);

    boolean setValue(double value, Sensor sensor);
}
