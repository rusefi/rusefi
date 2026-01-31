package com.opensr5.ini.field;

import java.util.Objects;

public abstract class IniField {
    private final String name;
    // offset within binary page
    private final int offset;

    public IniField(String name, int offset) {
        this.name = name;
        this.offset = offset;
    }

    public static double parseDouble(String s) {
        // todo: real implementation
        // TODO: replace with new ExpressionEvaluator
        s = s.replaceAll("\\{", "").replaceAll("\\}", "").trim();
        // If this is a complex expression with ternary operator, try to extract the true branch
        // this is related to the lambdaTable using the true branch as default on the fuel tests
        // [tag:lambdaTable]
        if (s.contains("?")) {
            int questionIndex = s.indexOf('?');
            int colonIndex = s.lastIndexOf(':');
            if (questionIndex >= 0 && colonIndex > questionIndex) {
                s = s.substring(questionIndex + 1, colonIndex).trim();
                // Recursively parse the true branch
                return parseDouble(s);
            }
            // If we can't extract a true branch, return default
            return 1.0;
        }
        int dividerIndex = s.indexOf('/');
        if (dividerIndex != -1) {
            return Double.parseDouble(s.substring(0, dividerIndex).trim()) / Double.parseDouble(s.substring(dividerIndex + 1).trim());
        } else {
            try {
                return Double.parseDouble(s);
            } catch (NumberFormatException e) {
                return 0;
            }
        }
    }

    public String getName() {
        return name;
    }

    public String getUnits() {
        return null;
    }

    public String getDigits() {
        return null;
    }

    public int getOffset() {
        return offset;
    }

    public abstract int getSize();

    public abstract <T> T accept(IniFieldVisitor<T> visitor);

    @Override
    public boolean equals(Object o) {
        if (this == o) return true;
        if (o == null || getClass() != o.getClass()) return false;
        IniField iniField = (IniField) o;
        return getOffset() == iniField.getOffset() && Objects.equals(getName(), iniField.getName());
    }

    @Override
    public int hashCode() {
        return Objects.hash(getName(), getOffset());
    }

    @Override
    public String toString() {
        return "IniField{" +
            "name='" + name + '\'' +
            ", offset=" + offset +
            '}';
    }
}
