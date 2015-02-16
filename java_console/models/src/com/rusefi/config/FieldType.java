package com.rusefi.config;

public enum FieldType {
    INT, FLOAT, ANALOG_CHART_E;

    public String getCommand() {
        switch (this) {
            case FLOAT:
                return "get_float";
            case INT:
            case ANALOG_CHART_E:
            default:
                return "get_int";
        }
    }

}