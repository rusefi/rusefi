package com.rusefi.ui.logview;

import com.rusefi.logview.CsvLogReader;

import java.util.Locale;

/**
 * What the histogram knows about signals: which ones to bin by default, whether Z is a mixture (lambda or AFR)
 * that can be shown either way, and readings stuck at a sensor's limit.
 */
final class HistogramSignals {
    /** Gasoline stoichiometric air/fuel ratio, for showing lambda as AFR. */
    static final double STOICH_AFR = 14.7;
    /** A reading at a signal's lowest or highest value in at least this share of records looks saturated. */
    static final double SATURATION_SHARE = 0.01;
    /** ...and in at least this many records, so a short log's single low reading does not count. */
    static final int SATURATION_RECORDS = 20;
    /** More than this share at one value is the signal resting there, not a sensor stuck at its limit. */
    static final double SATURATION_MAX_SHARE = 0.5;

    static final String[] X_NAMES = {"RPM", "Engine Speed"};
    static final String[] Y_NAMES = {"MAP", "Engine load", "Load", "TPS"};
    /** The measured mixture, first sensor first. */
    static final String[] Z_NAMES = {"Lambda 1", "Front Lambda", "Lambda", "Lambda 2", "Rear Lambda", "AFR",
            "Air/Fuel Ratio"};
    /**
     * Never picked for Z automatically: a target ("Fuel: target AFR") only shows the tune's own table back, not
     * what the engine did.
     */
    static final String Z_EXCLUDED = "target";

    private HistogramSignals() {
    }

    /** The first of the names (any case) in the log, then the first signal whose name contains one; or -1. */
    static int find(CsvLogReader log, String... names) {
        return findExcept(log, null, names);
    }

    /** Like {@link #find(CsvLogReader, String...)}, skipping signals whose name contains {@code excluded}. */
    static int findExcept(CsvLogReader log, String excluded, String... names) {
        for (String name : names) {
            for (int field = 1; field < log.fieldCount(); field++) {
                if (log.name(field).equalsIgnoreCase(name) && !isExcluded(log, field, excluded)) {
                    return field;
                }
            }
        }
        for (String name : names) {
            String wanted = name.toLowerCase(Locale.ROOT);
            for (int field = 1; field < log.fieldCount(); field++) {
                if (log.name(field).toLowerCase(Locale.ROOT).contains(wanted) && !isExcluded(log, field, excluded)) {
                    return field;
                }
            }
        }
        return -1;
    }

    private static boolean isExcluded(CsvLogReader log, int field, String excluded) {
        return excluded != null && log.name(field).toLowerCase(Locale.ROOT).contains(excluded);
    }

    /** The measured lambda or AFR for Z, or -1 when the log has none. */
    static int findMixture(CsvLogReader log) {
        return findExcept(log, Z_EXCLUDED, Z_NAMES);
    }

    /** The field called exactly {@code name}, or -1. */
    static int byName(CsvLogReader log, String name) {
        if (name == null) {
            return -1;
        }
        for (int field = 1; field < log.fieldCount(); field++) {
            if (log.name(field).equals(name)) {
                return field;
            }
        }
        return -1;
    }

    /** A signal for an axis when the preferred names are missing: the first non-constant one not already used. */
    static int fallback(CsvLogReader log, int... used) {
        return fallbackExcept(log, null, used);
    }

    /** Like {@link #fallback}, skipping signals whose name contains {@code excluded}. */
    static int fallbackExcept(CsvLogReader log, String excluded, int... used) {
        for (int field = 1; field < log.fieldCount(); field++) {
            boolean taken = false;
            for (int other : used) {
                taken |= other == field;
            }
            if (!taken && !log.isConstant(field) && !isExcluded(log, field, excluded)) {
                return field;
            }
        }
        return Math.min(1, log.fieldCount() - 1);
    }

    static boolean isLambda(CsvLogReader log, int field) {
        String unit = log.unit(field).toLowerCase(Locale.ROOT);
        return unit.equals("lambda") || unit.equals("λ")
                || log.name(field).toLowerCase(Locale.ROOT).contains("lambda") && !isAfr(log, field);
    }

    static boolean isAfr(CsvLogReader log, int field) {
        String name = log.name(field).toLowerCase(Locale.ROOT);
        return log.unit(field).equalsIgnoreCase("afr") || name.contains("afr") || name.contains("air/fuel");
    }

    /** A reading stuck at a sensor's limit: the value, which end, and in how many records. */
    static final class Saturation {
        final double value;
        final boolean low;
        final int records;
        final double share;

        Saturation(double value, boolean low, int records, double share) {
            this.value = value;
            this.low = low;
            this.records = records;
            this.share = share;
        }

        /** The filter that leaves these readings out. */
        HistogramFilter filter(String signal) {
            return HistogramFilter.of(signal, low ? HistogramFilter.Condition.AT_MOST
                    : HistogramFilter.Condition.AT_LEAST, value);
        }
    }

    /**
     * A field's lowest or highest value when the log sits exactly on it in many records, far more than on the
     * next value in, as a wideband controller reports its limit while the mixture is out of its range; null
     * otherwise. A value that is merely common at the end of the range (a quantized signal) does not count.
     * The end with more records wins when both qualify.
     */
    static Saturation saturation(CsvLogReader log, int field) {
        if (log.isConstant(field)) {
            return null;
        }
        double min = log.minimum(field);
        double max = log.maximum(field);
        double nextLow = Double.POSITIVE_INFINITY;
        double nextHigh = Double.NEGATIVE_INFINITY;
        for (int sample = 0; sample < log.sampleCount(); sample++) {
            double value = log.value(sample, field);
            if (Double.isFinite(value)) {
                if (value > min) {
                    nextLow = Math.min(nextLow, value);
                }
                if (value < max) {
                    nextHigh = Math.max(nextHigh, value);
                }
            }
        }
        int atMin = 0;
        int atMax = 0;
        int atNextLow = 0;
        int atNextHigh = 0;
        int finite = 0;
        for (int sample = 0; sample < log.sampleCount(); sample++) {
            double value = log.value(sample, field);
            if (Double.isFinite(value)) {
                finite++;
                atMin += value == min ? 1 : 0;
                atMax += value == max ? 1 : 0;
                atNextLow += value == nextLow ? 1 : 0;
                atNextHigh += value == nextHigh ? 1 : 0;
            }
        }
        // 0 is a signal's "off" (no knock, engine stopped), not a sensor limit
        boolean lowPiled = min != 0 && isPile(atMin, atNextLow, finite);
        boolean highPiled = max != 0 && isPile(atMax, atNextHigh, finite);
        if (!lowPiled && !highPiled) {
            return null;
        }
        boolean low = lowPiled && (!highPiled || atMin >= atMax);
        int records = low ? atMin : atMax;
        return new Saturation(low ? min : max, low, records, (double) records / finite);
    }

    /**
     * Readings at an end are a pile when there are many of them, and several times those on the next value; but
     * not when they are most of the log, which is a signal's resting state (no knock, no fuel cut).
     */
    private static boolean isPile(int atEnd, int atNext, int finite) {
        return atEnd >= SATURATION_RECORDS && atEnd >= finite * SATURATION_SHARE && atEnd >= 5 * atNext
                && atEnd <= finite * SATURATION_MAX_SHARE;
    }
}
