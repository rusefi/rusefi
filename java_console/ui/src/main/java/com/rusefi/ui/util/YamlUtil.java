package com.rusefi.ui.util;

import java.util.List;

public final class YamlUtil {

    private YamlUtil() {}

    /** Returns the object's toString(), or an empty string if null. */
    public static String toStr(Object val) {
        return val == null ? "" : val.toString();
    }

    /** Parses an int from a Number or a String object; returns 0 on failure. */
    public static int toInt(Object val) {
        if (val instanceof Number) return ((Number) val).intValue();
        if (val instanceof String) {
            try { return Integer.parseInt((String) val); } catch (NumberFormatException ignored) {}
        }
        return 0;
    }

    /**
     * Converts a value that may be either a single object or a List to a comma-separated string.
     * Useful for YAML fields (e.g. "class") that accept either a scalar or a sequence.
     */
    public static String toCommaSeparated(Object val) {
        if (val == null) return "";
        if (val instanceof List) {
            List<?> list = (List<?>) val;
            return String.join(", ", list.stream().map(Object::toString).toArray(String[]::new));
        }
        return val.toString();
    }
}
