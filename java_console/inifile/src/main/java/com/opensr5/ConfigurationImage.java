package com.opensr5;

import com.opensr5.ini.field.EnumIniField;
import com.opensr5.ini.field.IniField;
import com.opensr5.ini.field.ScalarIniField;
import com.rusefi.config.Field;
import com.rusefi.config.FieldType;
import com.rusefi.core.ByteBufferUtil;
import org.jetbrains.annotations.NotNull;
import org.jetbrains.annotations.Nullable;

import java.nio.ByteBuffer;
import java.util.Objects;

import static com.rusefi.config.FieldType.*;

/**
 * mutable presentation of ECU calibrations
 * in the MCU firmware that's config/engineConfiguration
 * <p>
 * Andrey Belomutskiy, (c) 2013-2020
 * 3/6/2015
 */
public class ConfigurationImage {
    private final byte[] content;

    public ConfigurationImage(int size) {
        content = new byte[size];
    }

    public ConfigurationImage(byte[] content) {
        this.content = Objects.requireNonNull(content);
    }

    public static void setScalarValue(ByteBuffer wrapped, FieldType type, String value, int bitOffset, double multiplier, double serializationOffset) {
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

    public static int getBitRange(int ordinal, int bitPosition, int bitSize) {
        ordinal = ordinal >> bitPosition;
        ordinal = ordinal & ((1 << (bitSize)) - 1);
        return ordinal;
    }

    public static boolean getBit(int ordinal, int bitPosition) {
        return getBitRange(ordinal, bitPosition, 1) == 1;
    }

    public void setBitValue(EnumIniField enumIniField, int ordinal) {
        int value = getByteBuffer(enumIniField).getInt();
        value = EnumIniField.setBitRange(value, ordinal, enumIniField.getBitPosition(), enumIniField.getBitSize0() + 1);
        getByteBuffer(enumIniField).putInt(value);
    }

    @NotNull
    public ByteBuffer getByteBuffer(int offset, int size) {
        return ByteBufferUtil.littleEndianWrap(content, offset, size);
    }

    public boolean isEmpty() {
        return content.length == 0;
    }

    public int getSize() {
        return content.length;
    }

    public byte[] getContent() {
        return content;
    }

    @SuppressWarnings("MethodDoesntCallSuperMethod")
    @Override
    public ConfigurationImage clone() {
        byte[] copy = content.clone();
        return new ConfigurationImage(copy);
    }

    /**
     * @return a COPY of the specified range
     */
    public byte[] getRange(int offset, int size) {
        byte[] r = new byte[size];
        System.arraycopy(content, offset, r, 0, size);
        return r;
    }

    @Override
    public String toString() {
        return "ConfigurationImage{" +
                "size=" + content.length +
                '}';
    }

    /**
     * Read a numeric value from this configuration image
     * Handles ScalarIniField (applying multiplier) and EnumIniField (extracting bit range).
     *
     * @return the value, or null if the field type is not supported
     */
    @Nullable
    public Double readNumericValue(IniField field) {
        if (field instanceof ScalarIniField) {
            ScalarIniField scalarField = (ScalarIniField) field;
            ByteBuffer bb = getByteBuffer(scalarField.getOffset(), 4);
            double rawValue = scalarField.getType().readRawValue(bb);
            return rawValue * scalarField.getMultiplier();
        }

        if (field instanceof EnumIniField) {
            EnumIniField enumField = (EnumIniField) field;
            ByteBuffer bb = getByteBuffer(enumField.getOffset(), 4);
            int rawValue = (int) enumField.getType().readRawValue(bb);
            int bitCount = enumField.getBitSize0() + 1;
            int mask = (1 << bitCount) - 1;
            return (double) ((rawValue >> enumField.getBitPosition()) & mask);
        }

        return null;
    }

    @NotNull
    public ByteBuffer getByteBuffer(EnumIniField enumIniField) {
        Objects.requireNonNull(this, "image enum getter");
        if (getSize() < enumIniField.getOffset() + 4)
            throw new IllegalArgumentException("OutOfBounds while " + enumIniField.getName() + " " + enumIniField.getOffset());
        return getByteBuffer(enumIniField.getOffset(), 4);
    }
}
