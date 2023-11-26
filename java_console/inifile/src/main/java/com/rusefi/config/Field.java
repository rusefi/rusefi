package com.rusefi.config;

import com.macfaq.io.LittleEndianOutputStream;
import com.opensr5.ConfigurationImage;
import com.rusefi.core.FileUtil;
import org.jetbrains.annotations.NotNull;

import java.io.ByteArrayOutputStream;
import java.io.IOException;
import java.nio.ByteBuffer;
import java.util.Objects;

import static com.rusefi.config.FieldType.*;

/**
 * @see Fields
 */

public class Field {
    public static final int NO_BIT_OFFSET = -1;
    private static final int FIELD_PRECISION = 3;

    private final String name;
    private final int offset;
    private final int stringSize;
    private final FieldType type;
    private final int bitOffset;
    private final String[] options;
    private double scale = 1;
    /**
     * LiveData fragments go one after another in the overall "outputs" region
     */
    private int baseOffset;

    public Field(String name, int offset, FieldType type) {
        this(name, offset, type, NO_BIT_OFFSET);
    }

    public Field(String name, int offset, FieldType type, String... options) {
        this(name, offset, type, NO_BIT_OFFSET, options);
    }

    public Field(String name, int offset, FieldType type, int bitOffset) {
        this(name, offset, type, bitOffset, null);
    }

    public Field(String name, int offset, FieldType type, int bitOffset, String[] options) {
        this(name, offset, 0, type, bitOffset, options);
    }

    public Field(String name, int offset, int stringSize, FieldType type, int bitOffset, String... options) {
        this.name = name;
        this.offset = offset;
        this.stringSize = stringSize;
        this.type = type;
        this.bitOffset = bitOffset;
        this.options = options;
    }

    public static Field findField(Field[] values, String instancePrefix, String fieldName) {
        Field field = findFieldOrNull(values, instancePrefix, fieldName);
        if (field == null)
            throw new IllegalStateException("No field: " + fieldName);
        return field;
    }

    /**
     * Finds field by name, ignoring case
     */
    public static Field findFieldOrNull(Field[] values, String instancePrefix, String fieldName) {
        Objects.requireNonNull(fieldName);
        for (Field f : values) {
            if (fieldName.equalsIgnoreCase(f.getName()))
                return f;
        }
        // 2nd pass - let's try to find field with prefix if it was not found without prefix
        if (!instancePrefix.isEmpty()) {
            fieldName = instancePrefix + "_" + fieldName;
            for (Field f : values) {
                if (fieldName.equalsIgnoreCase(f.getName()))
                    return f;
            }
        }
        return null;
    }

    public static int getStructureSize(Field[] values) {
        Field last = values[values.length - 1];
        // todo: at the moment we do not support arrays and
        // todo: a lot of information is missing for example for Bit type, but this implementation is good enough for now
        return last.offset + 4;
    }

    public static String niceToString(Number value) {
        return niceToString(value, FIELD_PRECISION);
    }

    public static String niceToString(Number value, int precision) {
        // not enum field
        if (value instanceof Float)
            return niceToString(value.floatValue(), precision);
        return value.toString();
    }

    public static String niceToString(double value, int precision) {
        int scale = (int) Math.log10(value);
        int places = 1 + Math.max(0, precision - scale);
        double toScale = Math.pow(10, places);
        return Double.toString(Math.round(value * toScale) / toScale);
    }

    public String getName() {
        return name;
    }

    public String setCommand() {
        if (type == FieldType.BIT)
            return "set_bit " + getOffset() + " " + bitOffset;
        return getType().getStoreCommand() + " " + getOffset();
    }

    public String getCommand() {
        if (type == FieldType.BIT)
            return "get_bit " + getOffset() + " " + bitOffset;
        return type.getLoadCommand() + " " + getOffset();
    }

    /**
     * todo: replace all (?) usages with #getTotalOffset?
     */
    public int getOffset() {
        return offset;
    }

    public int getTotalOffset() {
        return baseOffset + offset;
    }

    public String[] getOptions() {
        return options;
    }

    public int getBitOffset() {
        return bitOffset;
    }

    public FieldType getType() {
        return type;
    }

    @Override
    public String toString() {
        return "Field{" +
                name +
                ", o=" + offset +
                ", type=" + type +
                '}';
    }

    public Object getAnyValue(ConfigurationImage ci, double multiplier) {
        if (options == null) {
            // we are here for non-enum types
            return niceToString(getValue(ci, multiplier));
        }
        if (type != INT8)
            throw new IllegalStateException("Unsupported enum " + type);
        int ordinal = ci.getByteBuffer(offset, type.getStorageSize()).get();
        return options[ordinal];
    }

    // todo: move universal setValue one day
    public void setValueU32(byte[] content, int value) {
        ByteArrayOutputStream baos = new ByteArrayOutputStream();
        LittleEndianOutputStream dout = new LittleEndianOutputStream(baos);
        try {
            dout.writeInt(value);
            byte[] src = baos.toByteArray();
            System.arraycopy(src, 0, content, getOffset(), 4);
        } catch (IOException e) {
            throw new RuntimeException(e);
        }
    }

    public void setValue(byte[] content, boolean value) {
        ByteBuffer wrapped = FileUtil.littleEndianWrap(content, 0, content.length);
        if (bitOffset != NO_BIT_OFFSET) {
            int packed = wrapped.getInt();
            int thisBit = (value ? 1 : 0) << bitOffset;
            int mask = 1 << bitOffset;
            int newValue = (packed & ~mask) | thisBit;
            ByteArrayOutputStream baos = new ByteArrayOutputStream();
            LittleEndianOutputStream dout = new LittleEndianOutputStream(baos);
            // wow worst way to modify an integer in byte array? :)
            try {
                dout.writeInt(newValue);
//                dout.flush();
                byte[] src = baos.toByteArray();
                System.arraycopy(src, 0, content, getOffset(), 4);
                baos.close();
            } catch (IOException e) {
                throw new IllegalStateException(e);
            }
        }
    }

    /**
     * each usage is a potential bug?! we are supposed to have explicit multiplier for each field
     */
    @NotNull
    @Deprecated
    public Double getValue(ConfigurationImage ci) {
        return getValue(ci, 1);
    }

    // todo: rename to getNumberValue?
    @NotNull
    public Double getValue(ConfigurationImage ci, double multiplier) {
        Objects.requireNonNull(ci, "ConfigurationImage");
        Number value;
        ByteBuffer wrapped = ci.getByteBuffer(getOffset(), type.getStorageSize());
        if (bitOffset != NO_BIT_OFFSET) {
            int packed = wrapped.getInt();
            value = (packed >> bitOffset) & 1;
        } else if (type == INT8) {
            value = wrapped.get();
        } else if (type == UINT8) {
            byte signed = wrapped.get();
            value = signed & 0xFF;
        } else if (type == INT) {
            value = wrapped.getInt();
        } else if (type == INT16) {
            value = wrapped.getShort();
        } else if (type == UINT16) {
            short signed = wrapped.getShort();
            value = signed & 0xFFFF;
        } else {
            value = wrapped.getFloat();
        }
        return value.doubleValue() * multiplier;
    }

    @NotNull
    public ByteBuffer getByteBuffer(ConfigurationImage ci) {
        return ci.getByteBuffer(getOffset(), 4);
    }

    public static Field create(String name, int offset, FieldType type, int bitOffset) {
        return new Field(name, offset, type, bitOffset);
    }

    public static Field create(String name, int offset, FieldType type, String... options) {
        return new Field(name, offset, type, options);
    }

    public static Field create(String name, int offset, int stringSize, FieldType type) {
        return new Field(name, offset, stringSize, type, 0);
    }

    public static Field create(String name, int offset, FieldType type) {
        return new Field(name, offset, type);
    }

    public String getStringValue(ConfigurationImage image) {
        Objects.requireNonNull(image, "image");
        if (type != STRING)
            throw new IllegalStateException("Not a string parameter " + name);
        ByteBuffer bb = image.getByteBuffer(offset, stringSize);
        byte[] bytes = new byte[stringSize];
        bb.get(bytes);
        return new String(bytes).trim();
    }

    public boolean getBooleanValue(ConfigurationImage ci) {
        return getValue(ci) != 0.0;
    }

    public Field setScale(double scale) {
        this.scale = scale;
        return this;
    }

    public Field setBaseOffset(int baseOffset) {
        this.baseOffset = baseOffset;
        return this;
    }

    public double getScale() {
        return scale;
    }
}