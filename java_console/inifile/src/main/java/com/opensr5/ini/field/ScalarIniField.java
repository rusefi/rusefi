package com.opensr5.ini.field;

import com.opensr5.ConfigurationImage;
import com.rusefi.config.Field;
import com.rusefi.config.FieldType;
import com.rusefi.config.StringFormatter;
import com.rusefi.tune.xml.Constant;

import java.nio.ByteBuffer;
import java.util.LinkedList;
import java.util.Objects;

import static com.rusefi.config.FieldType.*;

public class ScalarIniField extends IniField {
    private final String unit;
    private final FieldType type;
    private final String digits;
    private final double serializationOffset;
    private final double multiplier;

    public ScalarIniField(String name, int offset, String unit, FieldType type, double multiplier, String digits,
                          double serializationOffset) {
        super(name, offset);
        this.unit = unit;
        this.type = type;
        this.digits = digits;
        this.serializationOffset = serializationOffset;
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
            Double value = f.getValue(image, multiplier) + serializationOffset;
            return StringFormatter.niceToString(value, StringFormatter.FIELD_PRECISION);
        } catch (Throwable e) {
            throw new IllegalStateException("While getting " + getName(), e);
        }
    }

    @Override
    public void setValue(ConfigurationImage image, Constant constant) {
        Objects.requireNonNull(image, "image for setter");
        Field f = new Field(getName(), getOffset(), getType());
        ByteBuffer wrapped = image.getByteBuffer(getOffset(), type.getStorageSize());
        setValue(wrapped, type, constant.getValue(), f.getBitOffset(), multiplier, serializationOffset);
    }

    public static void setValue(ByteBuffer wrapped, FieldType type, String value, int bitOffset, double multiplier, double serializationOffset) {
        double v = (Double.parseDouble(value) - serializationOffset)/ multiplier;
        if (bitOffset != Field.NO_BIT_OFFSET) {
            throw new UnsupportedOperationException("For " + value);
//            int packed = wrapped.getInt();
//            value = (packed >> bitOffset) & 1;
        } else if (type == INT8 || type == UINT8) {
            wrapped.put((byte) Math.round(v));
        } else if (type == INT) {
            wrapped.putInt((int) Math.round(v));
        } else if (type == INT16 || type == UINT16) {
            wrapped.putShort((short) Math.round(v));
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
        int offsetWithinConfigurationImage = Integer.parseInt(list.get(3));
        String digits = list.get(9);

        String unit = list.get(4);
        double multiplier = IniField.parseDouble(list.get(5));
        double serializationOffset = IniField.parseDouble(list.get(6));

        return new ScalarIniField(name, offsetWithinConfigurationImage, unit, type, multiplier, digits, serializationOffset);
    }
}
