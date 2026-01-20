package com.opensr5.ini.field;

import com.opensr5.ConfigurationImage;
import com.rusefi.config.Field;
import com.rusefi.config.FieldType;
import com.rusefi.config.StringFormatter;
import com.rusefi.tune.xml.Constant;

import java.nio.ByteBuffer;
import java.util.LinkedList;
import java.util.Objects;

public class ScalarIniField extends IniField {
    private final String unit;
    private final FieldType type;
    private final String digits;
    private final double serializationOffset;
    private final double multiplier;

    public ScalarIniField(String name, int offset, String unit, FieldType type, double multiplier, String digits,
                          double serializationOffset) {
        super(name, offset);
        this.unit = unit;
        this.type = type;
        this.digits = digits;
        this.serializationOffset = serializationOffset;
        if (multiplier == 0)
            throw new IllegalArgumentException("Multiplier should not be zero");
        this.multiplier = multiplier;
    }

    @Override
    public String getDigits() {
        return digits;
    }

    @Override
    public String getUnits() {
        return unit;
    }

    public FieldType getType() {
        return type;
    }

    public double getMultiplier() {
        return multiplier;
    }

    public double getSerializationOffset() {
        return serializationOffset;
    }

    @Override
    public <T> T accept(IniFieldVisitor<T> visitor) {
        return visitor.visit(this);
    }

    @Override
    public int getSize() {
        return type.getStorageSize();
    }


    @Override
    public String toString() {
        return "ScalarIniField{" +
                "name=" + getName() +
                ", offset=" + getOffset() +
                ", unit='" + unit + '\'' +
                ", type=" + type +
                '}';
    }

    public static ScalarIniField parse(LinkedList<String> list) {
        String name = list.get(0);
        FieldType type = FieldType.parseTs(list.get(2));
        int offsetWithinConfigurationImage = Integer.parseInt(list.get(3));
        String digits = list.get(9);

        String unit = list.get(4);
        double multiplier = IniField.parseDouble(list.get(5));
        double serializationOffset = IniField.parseDouble(list.get(6));

        return new ScalarIniField(name, offsetWithinConfigurationImage, unit, type, multiplier, digits, serializationOffset);
    }
}
