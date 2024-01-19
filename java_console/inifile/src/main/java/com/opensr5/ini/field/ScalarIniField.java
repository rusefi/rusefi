package com.opensr5.ini.field;

import com.opensr5.ConfigurationImage;
import com.rusefi.config.Field;
import com.rusefi.config.FieldType;

import java.nio.ByteBuffer;
import java.util.LinkedList;

import static com.rusefi.config.FieldType.*;

public class ScalarIniField extends IniField {
    private final String unit;
    private final FieldType type;
    private final String digits;
    private final double multiplier;

    public ScalarIniField(String name, int offset, String unit, FieldType type, double multiplier, String digits) {
        super(name, offset);
        this.unit = unit;
        this.type = type;
        this.digits = digits;
        if (multiplier == 0)
            throw new IllegalArgumentException("Multiplier should not be zero");
        this.multiplier = multiplier;
    }

    @Override
    public String getDigits() {
        return digits;
    }

    @Override
    public String getUnits() {
        return unit;
    }

    public FieldType getType() {
        return type;
    }

    @Override
    public int getSize() {
        return type.getStorageSize();
    }

    @Override
    public String getValue(ConfigurationImage image) {
        Field f = new Field(getName(), getOffset(), getType());
        try {
            return f.getValue(image, multiplier).toString();
        } catch (Throwable e) {
            throw new IllegalStateException("While getting " + getName(), e);
        }
    }

    public void setValue(ConfigurationImage image, String value) {
        Field f = new Field(getName(), getOffset(), getType());
        ByteBuffer wrapped = image.getByteBuffer(getOffset(), type.getStorageSize());
        setValue(wrapped, type, value, f.getBitOffset(), multiplier);
    }

    public static void setValue(ByteBuffer wrapped, FieldType type, String value, int bitOffset, double multiplier) {
        double v = Double.parseDouble(value) / multiplier;
        if (bitOffset != Field.NO_BIT_OFFSET) {
            throw new UnsupportedOperationException("For " + value);
//            int packed = wrapped.getInt();
//            value = (packed >> bitOffset) & 1;
        } else if (type == INT8 || type == UINT8) {
            wrapped.put((byte) v);
        } else if (type == INT) {
            wrapped.putInt((int) v);
        } else if (type == INT16 || type == UINT16) {
            wrapped.putShort((short) v);
        } else {
            wrapped.putFloat((float) v);
        }
    }

    @Override
    public String toString() {
        return "ScalarIniField{" +
                "name=" + getName() +
                ", offset=" + getOffset() +
                ", unit='" + unit + '\'' +
                ", type=" + type +
                '}';
    }

    public static ScalarIniField parse(LinkedList<String> list) {
        String name = list.get(0);
        FieldType type = FieldType.parseTs(list.get(2));
        int offset = Integer.parseInt(list.get(3));
        String digits = list.get(9);

        String unit = list.get(4);
        double multiplier = IniField.parseDouble(list.get(5));

        return new ScalarIniField(name, offset, unit, type, multiplier, digits);
    }
}
