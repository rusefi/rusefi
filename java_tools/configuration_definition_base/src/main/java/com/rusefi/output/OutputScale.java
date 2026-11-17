package com.rusefi.output;

import com.rusefi.ConfigField;

import static com.rusefi.TokenUtils.tokenizeWithBraces;

/** Constant INI conversions shared by SD logging and Lua output lookup. */
public final class OutputScale {
    private OutputScale() {
    }

    // [tag:live_data_output_scale] Always use the INI metadata at telemetry consumers without
    // changing model storage: engineering = (raw + translation) * scale.
    // scaled_channel already applies its packing scale in C++, but not the translation.
    public static float getAdditionalScale(ConfigField field) {
        if (field.isBit()) {
            return 1;
        }
        float scale = getConstant(field, 1, 1);
        return field.isHasAutoscale() ? 1 : scale;
    }

    /** MLG stores translation in raw payload units, before applying the descriptor scale. */
    public static float getTranslation(ConfigField field) {
        return getConstant(field, 2, 0);
    }

    /** Lua reads unpacked C++ values, so packed channels need translation in those units. */
    public static float getAdditionalTranslation(ConfigField field) {
        float translation = getTranslation(field);
        if (field.isHasAutoscale() && translation != 0) {
            translation = finiteConstant((float) (translation * field.autoscaleSpecNumber()), field);
        }
        return translation;
    }

    private static float getConstant(ConfigField field, int index, float defaultValue) {
        if (field.isBit()) {
            return defaultValue;
        }
        String[] tokens = tokenizeWithBraces(field.getTsInfo());
        return tokens.length <= index ? defaultValue : finiteConstant((float) parseConstant(tokens[index]), field);
    }

    private static float finiteConstant(float value, ConfigField field) {
        if (!Float.isFinite(value)) {
            throw new IllegalArgumentException("Output conversion requires a finite constant: " + field.getName());
        }
        return value;
    }

    private static double parseConstant(String value) {
        value = value.trim();
        if (value.startsWith("{") && value.endsWith("}")) {
            value = value.substring(1, value.length() - 1).trim();
        }
        String[] parts = value.split("/", -1);
        if (parts.length == 1) {
            return Double.parseDouble(parts[0]);
        }
        if (parts.length == 2) {
            return Double.parseDouble(parts[0].trim()) / Double.parseDouble(parts[1].trim());
        }
        throw new IllegalArgumentException("Output conversion requires a constant number or division: " + value);
    }
}
