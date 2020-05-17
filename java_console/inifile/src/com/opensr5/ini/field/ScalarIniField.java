package com.opensr5.ini.field;

import com.opensr5.ConfigurationImage;
import com.rusefi.config.Field;
import com.rusefi.config.FieldType;

import java.util.LinkedList;

public class ScalarIniField extends IniField {
    private final String unit;
    private final FieldType type;

    public ScalarIniField(String name, int offset, String unit, FieldType type) {
        super(name, offset);
        this.unit = unit;
        this.type = type;
    }

    @Override
    public String getUnits() {
        return unit;
    }

    public FieldType getType() {
        return type;
    }

    @Override
    public String getValue(ConfigurationImage image) {
        Field f = new Field(getName(), getOffset(), getType());
        return f.getValue(image).toString();
    }

    public static ScalarIniField parse(LinkedList<String> list) {
        String name = list.get(0);
        FieldType type = FieldType.parseTs(list.get(2));
        int offset = Integer.parseInt(list.get(3));

        String unit = list.get(4);

        return new ScalarIniField(name, offset, unit, type);
    }
}
