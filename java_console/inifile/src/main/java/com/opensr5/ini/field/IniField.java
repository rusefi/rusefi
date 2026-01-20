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
        s = s.replaceAll("\\{", "").replaceAll("\\}", "");
        int dividerIndex = s.indexOf('/');
        if (dividerIndex != -1) {
            return Double.parseDouble(s.substring(0, dividerIndex)) / Double.parseDouble(s.substring(dividerIndex + 1));
        } else {
            return Double.parseDouble(s);
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
