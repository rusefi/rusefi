package com.rusefi.sensor_logs;

import com.opensr5.ini.field.EnumIniField;
import com.opensr5.ini.field.IniField;
import com.opensr5.ini.field.ScalarIniField;
import com.rusefi.config.FieldType;
import com.rusefi.core.ISensorHolder;
import com.rusefi.core.SensorCategory;

import java.io.DataOutputStream;
import java.io.IOException;
import java.nio.ByteBuffer;

public class CustomBinaryLogEntry implements BinaryLogEntry {
    private final String name;
    private final IniField field;

    public CustomBinaryLogEntry(String name, IniField field) {
        this.name = name;
        this.field = field;
    }

    @Override
    public String getName() {
        return name;
    }

    @Override
    public SensorCategory getCategory() {
        return SensorCategory.OTHERS;
    }

    @Override
    public String getUnit() {
        return field instanceof ScalarIniField ? ((ScalarIniField) field).getUnits() : "";
    }

    @Override
    public int getByteSize() {
        switch (getLogType()) {
            case UINT8:
                return 0; // Wait, Sensor.java says 0 for UINT8. Is that right?
            case INT8:
                return 1;
            case UINT16:
                return 2;
            case INT16:
                return 3;
            case INT:
                return 4;
            case FLOAT:
                return 7;
            default:
                throw new UnsupportedOperationException("" + getLogType());
        }
    }

    @Override
    public float getScale() {
        return field instanceof ScalarIniField ? (float) ((ScalarIniField) field).getMultiplier() : 1;
    }

    @Override
    public void writeToLog(DataOutputStream dos, double value) throws IOException {
        switch (getLogType()) {
            case INT8:
            case UINT8:
                dos.write((int) value);
                return;
            case FLOAT:
                dos.writeFloat((float) value);
                return;
            case UINT16:
            case INT16:
                dos.writeShort((int) value);
                return;
            case INT:
                dos.writeInt((int) value);
                return;
            default:
                throw new UnsupportedOperationException("type " + getLogType());
        }
    }

    public double getValue(byte[] response) {
        ByteBuffer bb = ISensorHolder.getByteBuffer(response, getName(), field.getOffset(), field.getSize());
        if (field instanceof ScalarIniField) {
            return ((ScalarIniField) field).getType().readRawValue(bb);
        }

        EnumIniField enumField = (EnumIniField) field;
        long rawValue = (long) enumField.getType().readRawValue(bb);
        int bitCount = enumField.getBitSize0() + 1;
        long mask = bitCount == 32 ? 0xFFFFFFFFL : (1L << bitCount) - 1;
        return (rawValue >>> enumField.getBitPosition()) & mask;
    }

    private FieldType getLogType() {
        if (field instanceof ScalarIniField) {
            return ((ScalarIniField) field).getType();
        }

        int bitCount = ((EnumIniField) field).getBitSize0() + 1;
        if (bitCount <= 8) {
            return FieldType.UINT8;
        }
        if (bitCount <= 16) {
            return FieldType.UINT16;
        }
        return FieldType.INT;
    }
}
