package com.opensr5.ini.field;

import com.opensr5.ConfigurationImage;

public class IniField {
    private final String name;
    private final int offset;

    public IniField(String name, int offset) {
        this.name = name;
        this.offset = offset;
    }

    public String getName() {
        return name;
    }

    public String getUnits() {
        return null;
    }

    public int getOffset() {
        return offset;
    }

    public String getValue(ConfigurationImage image) {
        return null;
    }
}
