package com.rusefi.config;

public enum FieldType {
    INT,
    INT16,
    BIT,
    FLOAT;

    private String getTypeForCommand() {
        switch (this) {
            case FLOAT:
                return "float";
            case INT:
            default:
                return "int";
        }
    }

    public String getLoadCommand() {
        return "get_" + getTypeForCommand();
    }

    public String getStoreCommand() {
        return "set_" + getTypeForCommand();
    }
}