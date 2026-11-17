package com.rusefi.ui.widgets;

/** How a gauge slot presents its channel; {@link #getKey()} is the stored and saved form. */
public enum GaugeDisplayKind {
    ROUND("round", "Round gauge"),
    TEXT("text", "Text"),
    HORIZONTAL_BAR("hbar", "Horizontal bar"),
    VERTICAL_BAR("vbar", "Vertical bar"),
    LIVE_GRAPH("graph", "Live graph");

    private final String key;
    private final String label;

    GaugeDisplayKind(String key, String label) {
        this.key = key;
        this.label = label;
    }

    public String getKey() {
        return key;
    }

    public String getLabel() {
        return label;
    }

    /** @return the kind stored as {@code key}, or null for an unknown or missing key */
    public static GaugeDisplayKind lookup(String key) {
        for (GaugeDisplayKind kind : values()) {
            if (kind.key.equals(key)) {
                return kind;
            }
        }
        return null;
    }
}
