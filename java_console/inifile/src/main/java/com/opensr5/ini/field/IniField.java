package com.opensr5.ini.field;

import com.opensr5.ConfigurationImage;

public abstract class IniField {
    private final String name;
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

    /**
     * @see com.rusefi.config.Field#getValue
     */
    public String getValue(ConfigurationImage image) {
        return null;
    }
}
