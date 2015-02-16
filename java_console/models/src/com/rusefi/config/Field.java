package com.rusefi.config;

import com.rusefi.core.Pair;

import java.util.Objects;

/**
 * @see com.rusefi.config.Fields
 */

public class Field {
    private static final String INT_VALUE_PREFIX = "int @";
    private static final String FLOAT_VALUE_PREFIX = "float @";

    private final int offset;
    private final FieldType type;

    public Field(int offset, FieldType type) {
        this.offset = offset;
        this.type = type;
    }

    public int getOffset() {
        return offset;
    }

    public FieldType getType() {
        return type;
    }

    public static boolean isIntValueMessage(String message) {
        return message.startsWith(INT_VALUE_PREFIX);
    }


    public static boolean isFloatValueMessage(String message) {
        return message.startsWith(FLOAT_VALUE_PREFIX);
    }

    public static Pair<Integer, ?> parseResponse(String message) {
        try {
            if (isIntValueMessage(message)) {
                message = message.substring(INT_VALUE_PREFIX.length());
                String[] a = message.split(" is ");
                if (a.length != 2)
                    return null;
                int index = Integer.parseInt(a[0]);
                int value = Integer.parseInt(a[1]);
                return new Pair<>(index, value);
            }
            if (isFloatValueMessage(message)) {
                message = message.substring(FLOAT_VALUE_PREFIX.length());
                String[] a = message.split(" is ");
                if (a.length != 2)
                    return null;
                int index = Integer.parseInt(a[0]);
                double value = Double.parseDouble(a[1]);
                return new Pair<>(index, value);
            }
            return null;
        } catch (NumberFormatException e) {
            return null;
        }
    }
}