package com.rusefi.output;

import com.rusefi.ConfigField;

import static com.rusefi.TokenUtils.tokenizeWithBraces;

/** Scale-only conversions for live-data fragments with applyOutputScale enabled. */
public final class OutputScale {
    private OutputScale() {
    }

    // [tag:live_data_output_scale] Use the INI metadata at telemetry consumers without
    // changing model storage. scaled_channel already applies its packing scale in C++.
    public static float getAdditionalScale(ConfigField field) {
        if (field.isBit()) {
            return 1;
        }
        String[] tokens = tokenizeWithBraces(field.getTsInfo());
        if (tokens.length < 2) {
            return 1;
        }
        if (tokens.length > 2 && parseConstant(tokens[2]) != 0) {
            throw new IllegalArgumentException("applyOutputScale requires zero offset: " + field.getName());
        }
        if (field.isHasAutoscale()) {
            return 1;
        }
        float scale = (float) parseConstant(tokens[1]);
        if (!Float.isFinite(scale)) {
            throw new IllegalArgumentException("applyOutputScale requires a finite constant: " + field.getName());
        }
        return scale;
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
        throw new IllegalArgumentException("applyOutputScale requires a constant number or division: " + value);
    }
}
