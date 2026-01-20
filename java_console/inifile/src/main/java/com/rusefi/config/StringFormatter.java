package com.rusefi.config;

public class StringFormatter {
    public static final int FIELD_PRECISION = 5;

    public static String niceToString(Number value, int precision) {
        // not enum field
        if (value instanceof Float)
            return niceToString(value.floatValue(), precision);
        if (value instanceof Double)
            return niceToString(value.doubleValue(), precision);
        return value.toString();
    }

    public static String niceToString(double value, int precision) {
        int scale = (int) Math.log10(value);
        int places = 1 + Math.max(0, precision - scale);
        double toScale = Math.pow(10, places);
        return Double.toString(Math.round(value * toScale) / toScale);
    }

    public static String niceToString(Number value) {
        return StringFormatter.niceToString(value, FIELD_PRECISION);
    }
}
