package com.rusefi.config;

public class BitHelper {
    public static String setCommand(Field field) {
        if (field.getType() == FieldType.BIT)
            return "set_bit " + field.getOffset() + " " + field.getBitOffset() + " 1";
        return field.getType().getStoreCommand() + " " + field.getOffset();
    }

    public static String getCommand(Field field) {
        if (field.getType() == FieldType.BIT)
            return "get_bit " + field.getOffset() + " " + field.getBitOffset();
        return field.getType().getLoadCommand() + " " + field.getOffset();
    }
}
