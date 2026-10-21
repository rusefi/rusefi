package com.rusefi.ui.logview;

import com.rusefi.logview.CsvLogReader;

import java.util.Locale;

/** Formats log values with each field's display precision, so a field reads the same everywhere. */
final class ValueFormat {
    /** Shown for a missing value or an empty range. */
    static final String NONE = "—";
    /** Between the ends of a range, without spaces so ranges stay compact in the legend. */
    static final String RANGE = "–";

    private ValueFormat() {
    }

    static String format(CsvLogReader log, int field, double value) {
        if (!Double.isFinite(value)) {
            return NONE;
        }
        return String.format(Locale.ROOT, "%." + log.digits(field) + "f", value);
    }

    static String range(CsvLogReader log, int field, TraceStats stats) {
        if (stats.isEmpty()) {
            return NONE;
        }
        return format(log, field, stats.min) + RANGE + format(log, field, stats.max);
    }
}
