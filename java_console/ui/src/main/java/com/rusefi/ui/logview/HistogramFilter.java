package com.rusefi.ui.logview;

import com.rusefi.logview.CsvLogReader;

import java.util.List;
import java.util.Locale;
import java.util.Objects;

/**
 * A rule that leaves log records out of the histogram or scatter plot, e.g. "Front Lambda ≤ 0.68" for a saturated
 * sensor or "TPS changing faster than 50 /s, and 300 ms after" for transients. A box filter, made by selecting a
 * box on the scatter plot, works on two signals at once: it hides the records in the box, or keeps only those.
 * Filters name their signals rather than field indices, so they carry over to other logs; one whose signal is
 * missing hides nothing. Immutable.
 */
final class HistogramFilter {
    enum Condition {
        AT_MOST("≤"),
        AT_LEAST("≥"),
        BETWEEN("between"),
        OUTSIDE("outside"),
        /** The signal's rate of change, in its unit per second, is at least the value. */
        CHANGING("changing faster than"),
        /** Records with the signal in value..value2 and the second signal in value3..value4. */
        IN_BOX("in the box"),
        /** Records not in the box: keeps only the box. */
        OUTSIDE_BOX("outside the box");

        final String text;

        Condition(String text) {
            this.text = text;
        }

        boolean needsSecondValue() {
            return this == BETWEEN || this == OUTSIDE || isBox();
        }

        /** Made from a scatter plot's box; the filter dialog does not offer it. */
        boolean isBox() {
            return this == IN_BOX || this == OUTSIDE_BOX;
        }

        @Override
        public String toString() {
            return text;
        }
    }

    /** Between the fields of a saved filter; log signal names never contain it, logs being tab-delimited. */
    private static final String SEPARATOR = "\t";

    final String signal;
    final Condition condition;
    final double value;
    /** The upper end for {@link Condition#BETWEEN}, {@link Condition#OUTSIDE} and the boxes; unused otherwise. */
    final double value2;
    /** A box's second signal and its range; null and NaN otherwise. */
    final String signal2;
    final double value3;
    final double value4;
    /** Records this long after a match are hidden too, e.g. while the mixture settles after a throttle change. */
    final double holdSeconds;
    final boolean enabled;

    HistogramFilter(String signal, Condition condition, double value, double value2, double holdSeconds,
                    boolean enabled) {
        this(signal, condition, value, value2, null, Double.NaN, Double.NaN, holdSeconds, enabled);
    }

    private HistogramFilter(String signal, Condition condition, double value, double value2, String signal2,
                            double value3, double value4, double holdSeconds, boolean enabled) {
        this.signal = Objects.requireNonNull(signal);
        this.condition = Objects.requireNonNull(condition);
        this.value = condition.needsSecondValue() ? Math.min(value, value2) : value;
        this.value2 = condition.needsSecondValue() ? Math.max(value, value2) : value2;
        this.signal2 = condition.isBox() ? Objects.requireNonNull(signal2) : null;
        this.value3 = condition.isBox() ? Math.min(value3, value4) : Double.NaN;
        this.value4 = condition.isBox() ? Math.max(value3, value4) : Double.NaN;
        this.holdSeconds = Math.max(0, holdSeconds);
        this.enabled = enabled;
    }

    static HistogramFilter of(String signal, Condition condition, double value) {
        return new HistogramFilter(signal, condition, value, Double.NaN, 0, true);
    }

    /** A box of X and Y values: {@code hide} the records in it, or else keep only them. */
    static HistogramFilter box(String xSignal, double x1, double x2, String ySignal, double y1, double y2,
                               boolean hide) {
        return new HistogramFilter(xSignal, hide ? Condition.IN_BOX : Condition.OUTSIDE_BOX, x1, x2, ySignal, y1,
                y2, 0, true);
    }

    HistogramFilter withEnabled(boolean enabled) {
        return new HistogramFilter(signal, condition, value, value2, signal2, value3, value4, holdSeconds, enabled);
    }

    /**
     * "Front Lambda ≤ 0.68", "RPM between 800–1200", "TPS changing faster than 50 /s", "RPM 2000–3000 and MAP
     * 40–60" (a box hidden), "Only RPM 2000–3000 and MAP 40–60" (a box kept).
     */
    String label() {
        if (condition.isBox()) {
            String box = signal + " " + number(value) + ValueFormat.RANGE + number(value2) + " and " + signal2 + " "
                    + number(value3) + ValueFormat.RANGE + number(value4);
            return condition == Condition.OUTSIDE_BOX ? "Only " + box : box;
        }
        String text = signal + " " + condition.text + " " + number(value);
        if (condition.needsSecondValue()) {
            text += ValueFormat.RANGE + number(value2);
        } else if (condition == Condition.CHANGING) {
            text += " /s";
        }
        return text;
    }

    /** "+ 300 ms after", or empty without a hold time. */
    String holdLabel() {
        return holdSeconds > 0 ? String.format(Locale.ROOT, "+ %.0f ms after", holdSeconds * 1000) : "";
    }

    static String number(double value) {
        if (value == Math.rint(value) && Math.abs(value) < 1e9) {
            return String.format(Locale.ROOT, "%d", (long) value);
        }
        String text = String.format(Locale.ROOT, "%.4f", value);
        return text.replaceAll("0+$", "").replaceAll("\\.$", "");
    }

    private boolean matches(double v) {
        if (Double.isNaN(v)) {
            return false;
        }
        switch (condition) {
            case AT_MOST:
                return v <= value;
            case AT_LEAST:
                return v >= value;
            case BETWEEN:
                return v >= value && v <= value2;
            case OUTSIDE:
                return v < value || v > value2;
            default:
                throw new IllegalStateException(condition.name());
        }
    }

    /** The signal's field in the log, or -1 when it (or a box's second signal) is not there. */
    int field(CsvLogReader log) {
        int field = fieldOf(log, signal);
        return condition.isBox() && fieldOf(log, signal2) < 0 ? -1 : field;
    }

    private static int fieldOf(CsvLogReader log, String name) {
        for (int field = 1; field < log.fieldCount(); field++) {
            if (log.name(field).equals(name)) {
                return field;
            }
        }
        return -1;
    }

    /**
     * Marks the records this filter hides, whether or not it is enabled.
     *
     * @return how many records it hides, counting those already marked
     */
    int mark(CsvLogReader log, boolean[] hidden) {
        int field = field(log);
        if (field < 0) {
            return 0;
        }
        int field2 = condition.isBox() ? fieldOf(log, signal2) : -1;
        int count = 0;
        double holdUntil = Double.NEGATIVE_INFINITY;
        for (int sample = 0; sample < log.sampleCount(); sample++) {
            double time = log.value(sample, 0);
            boolean match;
            if (condition == Condition.CHANGING) {
                match = sample > 0 && rate(log, field, sample) >= value;
            } else if (condition.isBox()) {
                double x = log.value(sample, field);
                double y = log.value(sample, field2);
                boolean inside = x >= value && x <= value2 && y >= value3 && y <= value4;
                match = condition == Condition.IN_BOX ? inside : !inside;
            } else {
                match = matches(log.value(sample, field));
            }
            if (match) {
                holdUntil = time + holdSeconds;
            }
            if (match || time <= holdUntil) {
                hidden[sample] = true;
                count++;
            }
        }
        return count;
    }

    /** Change per second from the previous record; 0 between records logged at the same time. */
    private static double rate(CsvLogReader log, int field, int sample) {
        double dt = log.value(sample, 0) - log.value(sample - 1, 0);
        double change = Math.abs(log.value(sample, field) - log.value(sample - 1, field));
        return dt > 0 && Double.isFinite(change) ? change / dt : 0;
    }

    /** How many records this filter hides on its own. */
    int count(CsvLogReader log) {
        return mark(log, new boolean[log.sampleCount()]);
    }

    /** The records hidden by the enabled filters; one entry per record. */
    static boolean[] hidden(CsvLogReader log, List<HistogramFilter> filters) {
        boolean[] hidden = new boolean[log.sampleCount()];
        for (HistogramFilter filter : filters) {
            if (filter.enabled) {
                filter.mark(log, hidden);
            }
        }
        return hidden;
    }

    /** One line for the console config; see {@link #decode}. */
    String encode() {
        String text = String.join(SEPARATOR, signal, condition.name(), Double.toString(value), Double.toString(value2),
                Double.toString(holdSeconds), Boolean.toString(enabled));
        return condition.isBox() ? String.join(SEPARATOR, text, signal2, Double.toString(value3),
                Double.toString(value4)) : text;
    }

    /** A filter saved by {@link #encode}, or null when the text is not one. */
    static HistogramFilter decode(String text) {
        String[] parts = text.split(SEPARATOR, -1);
        if (parts.length != 6 && parts.length != 9 || parts[0].isEmpty()) {
            return null;
        }
        try {
            Condition condition = Condition.valueOf(parts[1]);
            if (condition.isBox() != (parts.length == 9)) {
                return null;
            }
            return new HistogramFilter(parts[0], condition, Double.parseDouble(parts[2]),
                    Double.parseDouble(parts[3]), condition.isBox() ? parts[6] : null,
                    condition.isBox() ? Double.parseDouble(parts[7]) : Double.NaN,
                    condition.isBox() ? Double.parseDouble(parts[8]) : Double.NaN,
                    Double.parseDouble(parts[4]), Boolean.parseBoolean(parts[5]));
        } catch (IllegalArgumentException | NullPointerException e) {
            return null;
        }
    }

    @Override
    public boolean equals(Object other) {
        if (!(other instanceof HistogramFilter)) {
            return false;
        }
        HistogramFilter that = (HistogramFilter) other;
        return signal.equals(that.signal) && condition == that.condition
                && Double.compare(value, that.value) == 0 && Double.compare(value2, that.value2) == 0
                && Objects.equals(signal2, that.signal2) && Double.compare(value3, that.value3) == 0
                && Double.compare(value4, that.value4) == 0
                && Double.compare(holdSeconds, that.holdSeconds) == 0 && enabled == that.enabled;
    }

    @Override
    public int hashCode() {
        return Objects.hash(signal, condition, value, value2, signal2, value3, value4, holdSeconds, enabled);
    }

    @Override
    public String toString() {
        return label() + (holdSeconds > 0 ? " " + holdLabel() : "") + (enabled ? "" : " (off)");
    }
}
