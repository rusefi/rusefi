package com.rusefi.ui.widgets;

import com.rusefi.core.preferences.storage.Node;
import org.jetbrains.annotations.Nullable;

import java.util.Objects;

/**
 * The user's replacements for a gauge's .ini min, max, high danger value and value decimals.
 * A {@code null} setting keeps the .ini value. They are stored on the gauge slot's
 * preferences node and in saved layout files under {@link #KEYS}.
 */
public final class GaugeScaleOverrides {
    public static final String MIN = "min";
    public static final String MAX = "max";
    public static final String DANGER = "danger";
    public static final String DECIMALS = "decimals";
    public static final String[] KEYS = {MIN, MAX, DANGER, DECIMALS};
    public static final GaugeScaleOverrides NONE = new GaugeScaleOverrides(null, null, null, null);

    @Nullable
    private final Double min;
    @Nullable
    private final Double max;
    @Nullable
    private final Double danger;
    @Nullable
    private final Integer decimals;

    public GaugeScaleOverrides(@Nullable Double min, @Nullable Double max, @Nullable Double danger,
                               @Nullable Integer decimals) {
        this.min = min;
        this.max = max;
        this.danger = danger;
        this.decimals = decimals;
    }

    @Nullable
    public Double getMin() {
        return min;
    }

    @Nullable
    public Double getMax() {
        return max;
    }

    @Nullable
    public Double getDanger() {
        return danger;
    }

    @Nullable
    public Integer getDecimals() {
        return decimals;
    }

    public boolean isEmpty() {
        return min == null && max == null && danger == null && decimals == null;
    }

    /** The settings stored on {@code node}; a missing or unreadable setting keeps the .ini value. */
    public static GaugeScaleOverrides read(Node node) {
        String decimalsText = node.getProperty(DECIMALS, null);
        return new GaugeScaleOverrides(number(node.getProperty(MIN, null)), number(node.getProperty(MAX, null)),
            number(node.getProperty(DANGER, null)), isValid(DECIMALS, decimalsText) ? Integer.valueOf(decimalsText) : null);
    }

    /** Replaces the settings stored on {@code node} with these. */
    public void writeTo(Node node) {
        clear(node);
        if (min != null) {
            node.setProperty(MIN, min);
        }
        if (max != null) {
            node.setProperty(MAX, max);
        }
        if (danger != null) {
            node.setProperty(DANGER, danger);
        }
        if (decimals != null) {
            node.setProperty(DECIMALS, decimals);
        }
    }

    public static void clear(Node node) {
        for (String key : KEYS) {
            node.getConfig().remove(key);
        }
    }

    /** Whether {@code value} is an acceptable stored form of setting {@code key}. */
    public static boolean isValid(String key, @Nullable String value) {
        if (value == null) {
            return false;
        }
        if (DECIMALS.equals(key)) {
            try {
                int decimals = Integer.parseInt(value);
                return decimals >= 0 && decimals <= GaugeScale.MAX_DECIMALS;
            } catch (NumberFormatException e) {
                return false;
            }
        }
        return number(value) != null;
    }

    @Nullable
    private static Double number(@Nullable String value) {
        if (value == null) {
            return null;
        }
        try {
            double parsed = Double.parseDouble(value.trim());
            return Double.isFinite(parsed) ? parsed : null;
        } catch (NumberFormatException e) {
            return null;
        }
    }

    /** Outcome of {@link #parse}: the overrides, or why the entered text is not acceptable. */
    public static final class Parsed {
        @Nullable
        public final GaugeScaleOverrides overrides;
        @Nullable
        public final String error;

        private Parsed(@Nullable GaugeScaleOverrides overrides, @Nullable String error) {
            this.overrides = overrides;
            this.error = error;
        }
    }

    /**
     * Reads the settings typed into the scale dialog. Empty text keeps the .ini value; the
     * resulting range, with .ini values filling the gaps, must have min below max.
     */
    public static Parsed parse(String minText, String maxText, String dangerText, @Nullable Integer decimals,
                               GaugeScale ini) {
        Double min;
        Double max;
        Double danger;
        try {
            min = field(minText, "Min");
            max = field(maxText, "Max");
            danger = field(dangerText, "Danger from");
        } catch (IllegalArgumentException e) {
            return new Parsed(null, e.getMessage());
        }
        double effectiveMin = min != null ? min : ini.getMin();
        double effectiveMax = max != null ? max : ini.getMax();
        if (!(effectiveMin < effectiveMax) || !Double.isFinite(effectiveMax - effectiveMin)) {
            return new Parsed(null, "Min must be below Max");
        }
        if (decimals != null && (decimals < 0 || decimals > GaugeScale.MAX_DECIMALS)) {
            return new Parsed(null, "Decimals must be 0 to " + GaugeScale.MAX_DECIMALS);
        }
        return new Parsed(new GaugeScaleOverrides(min, max, danger, decimals), null);
    }

    @Nullable
    private static Double field(String text, String label) {
        if (text == null || text.trim().isEmpty()) {
            return null;
        }
        Double value = number(text);
        if (value == null) {
            throw new IllegalArgumentException(label + " must be a number");
        }
        return value;
    }

    @Override
    public boolean equals(Object o) {
        if (this == o) {
            return true;
        }
        if (!(o instanceof GaugeScaleOverrides)) {
            return false;
        }
        GaugeScaleOverrides that = (GaugeScaleOverrides) o;
        return Objects.equals(min, that.min) && Objects.equals(max, that.max)
            && Objects.equals(danger, that.danger) && Objects.equals(decimals, that.decimals);
    }

    @Override
    public int hashCode() {
        return Objects.hash(min, max, danger, decimals);
    }

    @Override
    public String toString() {
        return "GaugeScaleOverrides{min=" + min + ", max=" + max + ", danger=" + danger + ", decimals=" + decimals + '}';
    }
}
