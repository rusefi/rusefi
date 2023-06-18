package com.rusefi.output.variables;

public class VariableRecord {
    public final String type;
    private final String humanName;
    private final String fullName;
    private final String conditional;


    public VariableRecord(String humanName, String fullName, String type, String conditional) {
        this.humanName = humanName;
        this.fullName = fullName;
        this.type = type;
        this.conditional = conditional;
    }

    public String getUserName() {
        return humanName;
    }

    public String getFullName() {
        return fullName;
    }

    public String getConditional() {
        return conditional;
    }
}
