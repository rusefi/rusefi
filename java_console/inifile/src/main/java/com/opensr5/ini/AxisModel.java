package com.opensr5.ini;

import com.opensr5.ConfigurationImage;
import com.opensr5.ini.field.IniField;

public class AxisModel {
    private final IniValue min;
    private final IniValue max;
    private final int step;

    public AxisModel(double min, double max, int step) {
        this.min = IniValue.ofNumeric(min);
        this.max = IniValue.ofNumeric(max);
        this.step = step;
    }

    public AxisModel(String min, String max, int step) {
        this.min = IniValue.parseNumeric(min);
        this.max = IniValue.parseNumeric(max);
        this.step = step;
    }

    public double getMin() {
        return fallbackValue(min);
    }

    public double getMax() {
        return fallbackValue(max);
    }

    /** Resolve against the tune when opening a curve, without mutating shared INI metadata. */
    public AxisModel resolve(IniFileModel ini, ConfigurationImage image) {
        return new AxisModel(resolveValue(min, ini, image), resolveValue(max, ini, image), step);
    }

    private static double resolveValue(IniValue value, IniFileModel ini, ConfigurationImage image) {
        if (value.isNumeric()) {
            return value.getNumericValue();
        }
        Double resolved = ExpressionEvaluator.evaluateNumericExpression(value.getRawString(), ini, image);
        return resolved != null && Double.isFinite(resolved) ? resolved : fallbackValue(value);
    }

    private static double fallbackValue(IniValue value) {
        return value.isNumeric() ? value.getNumericValue() : IniField.parseDouble(value.getRawString());
    }

    public int getStep() {
        return step;
    }

    @Override
    public String toString() {
        return "AxisModel{" +
                "min=" + min +
                ", max=" + max +
                ", step=" + step +
                '}';
    }
}
