package com.rusefi.ui.logview;

import com.rusefi.logview.CsvLogReader;

import java.util.List;
import java.util.Locale;
import java.util.Objects;

/**
 * A rule that leaves log records out of the histogram, e.g. "Front Lambda ≤ 0.68" for a saturated sensor or
 * "TPS changing faster than 50 /s, and 300 ms after" for transients. Filters name their signal rather than a
 * field index, so they carry over to other logs; one whose signal is missing hides nothing. Immutable.
 */
final class HistogramFilter {
    enum Condition {
        AT_MOST("≤"),
        AT_LEAST("≥"),
        BETWEEN("between"),
        OUTSIDE("outside"),
        /** The signal's rate of change, in its unit per second, is at least the value. */
        CHANGING("changing faster than");

        final String text;

        Condition(String text) {
            this.text = text;
        }

        boolean needsSecondValue() {
            return this == BETWEEN || this == OUTSIDE;
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
    /** The upper end for {@link Condition#BETWEEN} and {@link Condition#OUTSIDE}; unused otherwise. */
    final double value2;
    /** Records this long after a match are hidden too, e.g. while the mixture settles after a throttle change. */
    final double holdSeconds;
    final boolean enabled;

    HistogramFilter(String signal, Condition condition, double value, double value2, double holdSeconds,
                    boolean enabled) {
        this.signal = Objects.requireNonNull(signal);
        this.condition = Objects.requireNonNull(condition);
        this.value = condition.needsSecondValue() ? Math.min(value, value2) : value;
        this.value2 = condition.needsSecondValue() ? Math.max(value, value2) : value2;
        this.holdSeconds = Math.max(0, holdSeconds);
        this.enabled = enabled;
    }

    static HistogramFilter of(String signal, Condition condition, double value) {
        return new HistogramFilter(signal, condition, value, Double.NaN, 0, true);
    }

    HistogramFilter withEnabled(boolean enabled) {
        return new HistogramFilter(signal, condition, value, value2, holdSeconds, enabled);
    }

    /** "Front Lambda ≤ 0.68", "RPM between 800–1200", "TPS changing faster than 50 /s". */
    String label() {
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

    /** The signal's field in the log, or -1 when it is not there. */
    int field(CsvLogReader log) {
        for (int field = 1; field < log.fieldCount(); field++) {
            if (log.name(field).equals(signal)) {
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
        int count = 0;
        double holdUntil = Double.NEGATIVE_INFINITY;
        for (int sample = 0; sample < log.sampleCount(); sample++) {
            double time = log.value(sample, 0);
            boolean match;
            if (condition == Condition.CHANGING) {
                match = sample > 0 && rate(log, field, sample) >= value;
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
        return String.join(SEPARATOR, signal, condition.name(), Double.toString(value), Double.toString(value2),
                Double.toString(holdSeconds), Boolean.toString(enabled));
    }

    /** A filter saved by {@link #encode}, or null when the text is not one. */
    static HistogramFilter decode(String text) {
        String[] parts = text.split(SEPARATOR, -1);
        if (parts.length != 6 || parts[0].isEmpty()) {
            return null;
        }
        try {
            return new HistogramFilter(parts[0], Condition.valueOf(parts[1]), Double.parseDouble(parts[2]),
                    Double.parseDouble(parts[3]), Double.parseDouble(parts[4]), Boolean.parseBoolean(parts[5]));
        } catch (IllegalArgumentException e) {
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
                && Double.compare(holdSeconds, that.holdSeconds) == 0 && enabled == that.enabled;
    }

    @Override
    public int hashCode() {
        return Objects.hash(signal, condition, value, value2, holdSeconds, enabled);
    }

    @Override
    public String toString() {
        return label() + (holdSeconds > 0 ? " " + holdLabel() : "") + (enabled ? "" : " (off)");
    }
}
