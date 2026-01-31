package com.rusefi.core;

import com.devexperts.logging.Logging;
import com.opensr5.ini.GaugeModel;
import com.opensr5.ini.IniFileModel;
import com.opensr5.ini.IniMemberNotFound;
import com.opensr5.ini.field.IniField;
import com.opensr5.ini.field.ScalarIniField;
import com.rusefi.binaryprotocol.RealIniFileProvider;
import org.jetbrains.annotations.NotNull;

import java.nio.ByteBuffer;
import java.util.Map;

import static com.devexperts.logging.Logging.getLogging;
import static com.rusefi.core.ByteBufferUtil.littleEndianWrap;

public interface ISensorHolder {
    Logging log = getLogging(ISensorHolder.class);

    default void grabSensorValues(byte[] response, @NotNull IniFileModel ini) {
        for (Map.Entry<String, GaugeModel> e : ini.getGauges().entrySet()) {
            try {
                String gaugeName = e.getKey();
                GaugeModel gaugeModel = e.getValue();
                IniField sensorField = ini.getOutputChannel(gaugeModel.getChannel());
                if (sensorField instanceof ScalarIniField) {
                    ByteBuffer bb = getByteBuffer(response, gaugeName, sensorField.getOffset());
                    ScalarIniField scalarField = (ScalarIniField) sensorField;
                    double rawValue = getRawValue(bb, scalarField.getType());
                    double scaledValue = rawValue * scalarField.getMultiplier();
                    setValue(scaledValue, gaugeName);
                } else {
                    log.warn("Not found for " + e);
                }
            } catch (IniMemberNotFound ex) {
                // todo: suddenly we need expressions here
//                coolantTemperature = { useMetricOnInterface ? coolant : (coolant * 1.8 + 32) }
//                intakeTemperature = { intake }
                log.warn("Member not found for " + e);
//                throw new IllegalStateException(ex);
            }
        }
    }

    static double getRawValue(ByteBuffer bb, com.rusefi.config.FieldType type) {
        switch (type) {
            case FLOAT:
                return bb.getFloat();
            case INT:
                return bb.getInt();
            case UINT16:
                return bb.getInt() & 0xFFFF;
            case INT16:
                return (short) (bb.getInt() & 0xFFFF);
            case UINT8:
                return bb.getInt() & 0xFF;
            case INT8:
                return (byte) (bb.getInt() & 0xFF);
            default:
                throw new UnsupportedOperationException("type " + type);
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

    double getValue(String sensorName);

    boolean setValue(double value, Sensor sensor);

    boolean setValue(double value, String sensorName);
}
