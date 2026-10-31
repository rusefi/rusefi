package com.opensr5.ini;

public class VeAnalyzeFilter {
    private final String name;
    private final String displayName;
    private final String outputChannel;
    private final String operator;
    private final double defaultValue;
    private final boolean userAdjustable;

    public VeAnalyzeFilter(String name, String displayName, String outputChannel, String operator, double defaultValue, boolean userAdjustable) {
        this.name = name;
        this.displayName = displayName;
        this.outputChannel = outputChannel;
        this.operator = operator;
        this.defaultValue = defaultValue;
        this.userAdjustable = userAdjustable;
    }

    public String getName() {
        return name;
    }

    public String getDisplayName() {
        return displayName;
    }

    public String getOutputChannel() {
        return outputChannel;
    }

    public String getOperator() {
        return operator;
    }

    public double getDefaultValue() {
        return defaultValue;
    }

    public boolean isUserAdjustable() {
        return userAdjustable;
    }

    @Override
    public String toString() {
        return "VeAnalyzeFilter{" +
                "name='" + name + '\'' +
                ", displayName='" + displayName + '\'' +
                ", outputChannel='" + outputChannel + '\'' +
                ", operator='" + operator + '\'' +
                ", defaultValue=" + defaultValue +
                ", userAdjustable=" + userAdjustable +
                '}';
    }
}
