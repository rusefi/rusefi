package com.rusefi;

import java.util.Objects;

/**
 * 1/22/15
 */
public class TypesHelper {
    private static final String INT8_T = "int8_t";
    public static final String UINT8_T = "uint8_t";
    public static final String UINT_16_T = "uint16_t";
    public static final String INT_16_T = "int16_t";
    private static final String FLOAT_T = "float";
    private static final String INT_32_T = "int";
    private static final String UINT_32_T = "uint32_t";

    public static int getElementSize(ReaderState state, String type) {
        Objects.requireNonNull(state);
        if (type == null)
            return 0;
        if (state != null && state.structures.containsKey(type))
            return state.structures.get(type).totalSize;
        if (state != null && state.tsCustomSize.containsKey(type))
            return state.tsCustomSize.get(type);
        if (isPrimitive1byte(type))
            return 1;
        if (isPrimitive2byte(type)) {
            return 2;
        }
        if (isPrimitive4byte(type)) {
            return 4;
        }
        throw new IllegalArgumentException("Unknown type " + type);
    }

    public static boolean isPrimitive(String type) {
        return isPrimitive1byte(type) || isPrimitive2byte(type) || isPrimitive4byte(type);
    }

    private static boolean isPrimitive1byte(String type) {
        return type.equals(INT8_T) || type.equals(UINT8_T);
    }

    private static boolean isPrimitive2byte(String type) {
        return type.equals(INT_16_T) || type.equals(UINT_16_T);
    }

    private static boolean isPrimitive4byte(String type) {
        return type.equals(INT_32_T) || type.equals(UINT_32_T) || isFloat(type);
    }

    public static String convertToTs(String type) {
        if (isFloat(type))
            return "F32";
        if ("uint32_t".equals(type))
            return "U32";
        if ("int32_t".equals(type) || "int".equals(type))
            return "S32";
        if (INT_16_T.equals(type))
            return "S16";
        if (UINT_16_T.equals(type))
            return "U16";
        if (INT8_T.equals(type))
            return "S08";
        if (UINT8_T.equals(type))
            return "U08";
        System.out.println("No TS type convesion for " + type);
        return type;
    }

    public static boolean isFloat(String type) {
        return FLOAT_T.equals(type) ||
                type.equalsIgnoreCase("floatms_t") ||
                type.equalsIgnoreCase("angle_t");
    }
}
