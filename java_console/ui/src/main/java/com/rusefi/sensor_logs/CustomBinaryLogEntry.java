package com.rusefi.sensor_logs;

import com.opensr5.ini.GaugeModel;
import com.opensr5.ini.field.IniField;
import com.opensr5.ini.field.ScalarIniField;
import com.rusefi.config.FieldType;
import com.rusefi.core.ISensorHolder;
import com.rusefi.core.SensorCategory;
import com.rusefi.core.SensorCentral;

import java.io.DataOutputStream;
import java.io.IOException;
import java.nio.ByteBuffer;

public class CustomBinaryLogEntry implements BinaryLogEntry {
    private final GaugeModel gaugeModel;
    private final ScalarIniField scalarField;

    public CustomBinaryLogEntry(GaugeModel gaugeModel, ScalarIniField scalarField) {
        this.gaugeModel = gaugeModel;
        this.scalarField = scalarField;
    }

    @Override
    public String getName() {
        return gaugeModel.getName();
    }

    @Override
    public SensorCategory getCategory() {
        return SensorCategory.OTHERS;
    }

    @Override
    public String getUnit() {
        return gaugeModel.getUnits();
    }

    @Override
    public int getByteSize() {
        switch (scalarField.getType()) {
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
                throw new UnsupportedOperationException("" + scalarField.getType());
        }
    }

    @Override
    public void writeToLog(DataOutputStream dos, double value) throws IOException {
        switch (scalarField.getType()) {
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
                throw new UnsupportedOperationException("type " + scalarField.getType());
        }
    }

    public double getValue(byte[] response) {
        ByteBuffer bb = ISensorHolder.getByteBuffer(response, getName(), scalarField.getOffset());
        double rawValue = getValueForChannel(bb);
        return rawValue * scalarField.getMultiplier();
    }

    private double getValueForChannel(ByteBuffer bb) {
        switch (scalarField.getType()) {
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
                throw new UnsupportedOperationException("type " + scalarField.getType());
        }
    }
}
