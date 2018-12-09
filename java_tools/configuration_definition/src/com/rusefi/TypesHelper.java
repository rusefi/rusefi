package com.rusefi;

import static com.rusefi.ConfigStructure.INT_16_T;
import static com.rusefi.ConfigStructure.UINT8_T;
import static com.rusefi.ConfigStructure.UINT_16_T;

/**
 * 1/22/15
 */
public class TypesHelper {

    public static int getElementSize(String type) {
        if (type == null)
            throw new NullPointerException("type");
        if (ConfigDefinition.structures.containsKey(type))
            return ConfigDefinition.structures.get(type).totalSize;
        if (ConfigDefinition.tsCustomSize.containsKey(type))
            return ConfigDefinition.tsCustomSize.get(type);
        if (type.equals(UINT8_T))
            return 1;
        if (type.equals(INT_16_T) || type.equals(UINT_16_T)) {
            return 2;
        }
        return 4;
    }

    static String convertToTs(String type) {
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
        if (UINT8_T.equals(type))
            return "U08";
        System.out.println("No TS type convesion for " + type);
        return type;
    }

    public static boolean isFloat(String type) {
        return "float".equals(type) || type.equalsIgnoreCase("angle_t");
    }
}
