package com.rusefi.config;

import java.nio.ByteBuffer;

public enum FieldType {
    // Signed
    INT8(1),
    INT16(2),
    INT(4),

    // Unsigned
    UINT8(1),
    UINT16(2),

    BIT(/*bits are stored in 4 byte packs */4),
    FLOAT(4),

    STRING(0);

    // todo: this is used for text protocol parsing - constant should be reused between firmware and console
    public static final String INT_TYPE_STRING = "int";
    public static final String FLOAT_TYPE_STRING = "float";
    public static final String BYTE_TYPE_STRING = "byte";
    public static final String UBYTE_TYPE_STRING = "ubyte";
    public static final String SHORT_TYPE_STRING = "short";
    public static final String USHORT_TYPE_STRING = "ushort";

    private final int storageSize;

    FieldType(int storageSize) {
        this.storageSize = storageSize;
    }

    private String getTypeForCommand() {
        switch (this) {
            case FLOAT:
                return FLOAT_TYPE_STRING;
            case INT16:
                return SHORT_TYPE_STRING;
            case UINT16:
                return USHORT_TYPE_STRING;
            case INT8:
                return BYTE_TYPE_STRING;
            case UINT8:
                return UBYTE_TYPE_STRING;
            case INT:
            default:
                return INT_TYPE_STRING;
        }
    }

    public static FieldType parseTs(String type) {
        if (type.equalsIgnoreCase("f32"))
            return FLOAT;
        if (type.equalsIgnoreCase("s32") || type.equalsIgnoreCase("u32"))
            return INT;
        if (type.equalsIgnoreCase("s16"))
            return INT16;
        if (type.equalsIgnoreCase("u16"))
            return UINT16;
        if (type.equalsIgnoreCase("s08"))
            return INT8;
        if (type.equalsIgnoreCase("u08"))
            return UINT8;
        throw new IllegalStateException(type);
    }

    public String getLoadCommand() {
        return "get_" + getTypeForCommand();
    }

    public String getStoreCommand() {
        return "set_" + getTypeForCommand();
    }

    public int getStorageSize() {
        if (this == STRING)
            throw new UnsupportedOperationException("storage size is unclear on " + this);
        return storageSize;
    }

    public boolean isString() {
        return this == STRING;
    }

    /**
     * Read a raw numeric value from a little-endian ByteBuffer according to this type.
     * The buffer must contain at least 4 bytes from the current position.
     */
    public double readRawValue(ByteBuffer bb) {
        switch (this) {
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
                throw new UnsupportedOperationException("type " + this);
        }
    }

    public boolean isNumeric() {
        switch (this) {
            case INT8:
            case INT16:
            case INT:
            case UINT8:
            case UINT16:
            case FLOAT: {
                return true;
            }
            default: {
                return false;
            }
        }
    }
}
