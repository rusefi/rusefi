package com.opensr5.ini.field;

import com.opensr5.ini.ExpressionEvaluator;
import com.rusefi.config.FieldType;

import java.util.LinkedList;
import java.util.Objects;

public class ScalarIniField extends IniField {
    private final String unit;
    private final FieldType type;
    private final String digits;
    private final double serializationOffset;
    private final double multiplier;
    private final String scaleExpression;
    private final String translateExpression;

    public ScalarIniField(String name, int offset, String unit, FieldType type, double multiplier, String digits,
                          double serializationOffset) {
        this(name, offset, unit, type, multiplier, digits, serializationOffset, null, null);
    }

    private ScalarIniField(String name, int offset, String unit, FieldType type, double multiplier, String digits,
                           double serializationOffset, String scaleExpression, String translateExpression) {
        super(name, offset);
        this.unit = unit;
        this.type = Objects.requireNonNull(type);
        this.digits = digits;
        this.serializationOffset = serializationOffset;
        if (multiplier == 0)
            throw new IllegalArgumentException("Multiplier should not be zero");
        this.multiplier = multiplier;
        this.scaleExpression = scaleExpression;
        this.translateExpression = translateExpression;
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

    public String getScaleExpression() {
        return scaleExpression;
    }

    public String getTranslateExpression() {
        return translateExpression;
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
        String scale = list.get(5);
        String translate = list.get(6);

        // Keep dynamic presentation metadata separate from the base values used for tune I/O.
        return new ScalarIniField(name, offsetWithinConfigurationImage, unit, type, IniField.parseDouble(scale),
            digits, IniField.parseDouble(translate), dynamicExpression(scale), dynamicExpression(translate));
    }

    private static String dynamicExpression(String value) {
        return ExpressionEvaluator.extractVariables(value).isEmpty() ? null : value;
    }
}
