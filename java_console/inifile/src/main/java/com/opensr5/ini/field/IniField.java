package com.opensr5.ini.field;

import com.opensr5.ConfigurationImage;
import com.rusefi.tune.xml.Constant;

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
        s = s.replace("{", "").replace("}", "").trim();
        if (s.contains("?")) {
            // The console only needs a storage approximation; it does not evaluate INI expressions.
            int questionIndex = s.indexOf('?');
            int colonIndex = s.indexOf(':', questionIndex + 1);
            if (questionIndex == 0 || colonIndex < 0 || colonIndex == questionIndex + 1
                || colonIndex == s.length() - 1 || s.indexOf('?', questionIndex + 1) >= 0
                || s.indexOf(':', colonIndex + 1) >= 0) {
                throw new NumberFormatException("Malformed conditional expression: " + s);
            }
            double whenTrue = parseDouble(s.substring(questionIndex + 1, colonIndex).trim());
            parseDouble(s.substring(colonIndex + 1).trim());
            return whenTrue;
        }
        int dividerIndex = s.indexOf('/');
        if (dividerIndex != -1) {
            return Double.parseDouble(s.substring(0, dividerIndex).trim()) / Double.parseDouble(s.substring(dividerIndex + 1).trim());
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

    /**
     * @see com.rusefi.config.Field#getValue
     */
    public String getValue(ConfigurationImage image) {
        return null;
    }

    public void setValue(ConfigurationImage image, Constant constant) {
        throw new UnsupportedOperationException("On " + getClass());
    }

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
