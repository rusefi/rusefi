package com.rusefi;

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
        if (type.equals(ConfigStructure.UINT8_T))
            return 1;
        if (type.equals(ConfigStructure.INT_16_T) || type.equals(ConfigStructure.UINT_16_T)) {
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
        if ("int16_t".equals(type))
            return "S16";
        if ("uint16_t".equals(type))
            return "U16";
        return type;
    }

    public static boolean isFloat(String type) {
        return "float".equals(type) || type.equalsIgnoreCase("angle_t");
    }
}
