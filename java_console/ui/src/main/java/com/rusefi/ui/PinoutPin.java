package com.rusefi.ui;

import com.rusefi.ui.util.PinColors;

/**
 * One pin of a connector YAML, as the Pinout tab shows it: its name, what it is for, where it sits on the
 * connector image (when the YAML places it) and what the current tune uses it for.
 */
final class PinoutPin {
    /** Used by the tune, free for the tune to use, or fixed (power and ground, which the tune does not set). */
    enum Use {USED, FREE, FIXED}

    final String name;
    final String function;
    /** The YAML {@code type}, as written; empty when there is none. */
    final String yamlType;
    /** The YAML {@code class}, comma separated when a list. */
    final String classes;
    /** The pin's name in the tune's pin lists ({@code ts_name} with {@code ___} replaced); empty for fixed pins. */
    final String tsName;
    /** Pigtail wire colour, e.g. "red" or "orange/brown"; empty when not listed. */
    final String wire;
    final PinColors.Resolved type;
    /** Pixel position in the connector image, or -1 when the YAML does not place the pin. */
    int x = -1;
    int y = -1;
    /** Labels of the tune settings using this pin, comma separated; empty when none. */
    String tuneUse = "";

    PinoutPin(String name, String function, String yamlType, String classes, String tsName, String wire) {
        this.name = name;
        this.function = function;
        this.yamlType = yamlType;
        this.classes = classes;
        this.tsName = tsName;
        this.wire = wire;
        type = PinColors.resolve(yamlType, function, classes);
    }

    boolean isPlaced() {
        return x >= 0 && y >= 0;
    }

    Use use() {
        if (!tuneUse.isEmpty()) {
            return Use.USED;
        }
        return tsName.isEmpty() ? Use.FIXED : Use.FREE;
    }
}
