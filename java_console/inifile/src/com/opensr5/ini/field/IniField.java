package com.opensr5.ini.field;

import com.opensr5.ConfigurationImage;
import com.rusefi.tune.xml.Constant;

public abstract class IniField {
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

    public abstract int getSize();

    public String getValue(ConfigurationImage image) {
        return null;
    }

    public void setValue(ConfigurationImage image, Constant constant) {
        throw new UnsupportedOperationException("On " + getClass());
    }
}
