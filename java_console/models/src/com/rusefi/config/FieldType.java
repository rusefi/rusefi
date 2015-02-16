package com.rusefi.config;

public enum FieldType {
    INT, FLOAT, ANALOG_CHART_E;


    private String getTypeForCommand() {
        switch (this) {
            case FLOAT:
                return "float";
            case INT:
            case ANALOG_CHART_E:
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