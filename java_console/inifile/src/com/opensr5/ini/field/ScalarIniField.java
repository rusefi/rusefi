package com.opensr5.ini.field;

import com.opensr5.ConfigurationImage;
import com.rusefi.config.Field;
import com.rusefi.config.FieldType;
import com.rusefi.tune.xml.Constant;

import java.nio.ByteBuffer;
import java.util.LinkedList;

import static com.rusefi.config.FieldType.*;

public class ScalarIniField extends IniField {
    private final String unit;
    private final FieldType type;

    public ScalarIniField(String name, int offset, String unit, FieldType type) {
        super(name, offset);
        this.unit = unit;
        this.type = type;
    }

    @Override
    public String getUnits() {
        return unit;
    }

    public FieldType getType() {
        return type;
    }

    @Override
    public String getValue(ConfigurationImage image) {
        Field f = new Field(getName(), getOffset(), getType());
        try {
            return f.getValue(image).toString();
        } catch (Throwable e) {
            throw new IllegalStateException("While getting " + getName(), e);
        }
    }

    @Override
    public void setValue(ConfigurationImage image, Constant constant) {
        Field f = new Field(getName(), getOffset(), getType());
        ByteBuffer wrapped = image.getByteBuffer(getOffset(), type.getStorageSize());
        setValue(wrapped, type, constant.getValue(), f.getBitOffset());
    }

    public static void setValue(ByteBuffer wrapped, FieldType type, String value, int bitOffset) {
        if (bitOffset != Field.NO_BIT_OFFSET) {
            throw new UnsupportedOperationException("For " + value);
//            int packed = wrapped.getInt();
//            value = (packed >> bitOffset) & 1;
        } else if (type == INT8 || type == UINT8) {
            wrapped.put((byte) Double.parseDouble(value));
        } else if (type == INT) {
            wrapped.putInt((int) Double.parseDouble(value));
        } else if (type == INT16 || type == UINT16) {
            wrapped.putShort((short) Double.parseDouble(value));
        } else {
            wrapped.putFloat(Float.parseFloat(value));
        }
    }

    public static ScalarIniField parse(LinkedList<String> list) {
        String name = list.get(0);
        FieldType type = FieldType.parseTs(list.get(2));
        int offset = Integer.parseInt(list.get(3));

        String unit = list.get(4);

        return new ScalarIniField(name, offset, unit, type);
    }
}
