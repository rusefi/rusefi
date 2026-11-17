package com.rusefi.logview;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.Reader;
import java.util.ArrayList;
import java.util.List;

import static com.rusefi.config.generated.VariableRegistryValues.GAUGE_NAME_TIME;

/** A tab-delimited time-series log with an optional preamble and a units row. */
public final class CsvLogReader {
    private final String[] names;
    private final String[] units;
    private final double[][] rows;
    private final double[] minimum;
    private final double[] maximum;
    private final int[] digits;

    private CsvLogReader(String[] names, String[] units, double[][] rows, double[] minimum, double[] maximum,
                         int[] digits) {
        this.names = names;
        this.units = units;
        this.rows = rows;
        this.minimum = minimum;
        this.maximum = maximum;
        this.digits = digits;
    }

    static CsvLogReader fromRows(String[] names, String[] units, List<double[]> samples) throws IOException {
        if (samples.isEmpty()) {
            throw new IOException("Log has no numeric samples");
        }
        double[] minimum = new double[names.length];
        double[] maximum = new double[names.length];
        java.util.Arrays.fill(minimum, Double.POSITIVE_INFINITY);
        java.util.Arrays.fill(maximum, Double.NEGATIVE_INFINITY);
        boolean[] fractional = new boolean[names.length];
        for (double[] row : samples) {
            for (int i = 0; i < names.length; i++) {
                double value = row[i];
                if (Double.isFinite(value)) {
                    minimum[i] = Math.min(minimum[i], value);
                    maximum[i] = Math.max(maximum[i], value);
                    fractional[i] |= value != Math.rint(value);
                }
            }
        }
        int[] digits = new int[names.length];
        for (int i = 0; i < names.length; i++) {
            digits[i] = displayDigits(fractional[i], Math.max(Math.abs(minimum[i]), Math.abs(maximum[i])));
        }
        return new CsvLogReader(names, units, samples.toArray(new double[0][]), minimum, maximum, digits);
    }

    /**
     * Decimals for showing a field: none when every value is whole, otherwise about four significant digits
     * at the field's largest magnitude (6194 rpm, 100.0 %, 81.52 C, 0.970 lambda). Logs do not carry usable
     * precision: ECU logs declare 3 decimals for every fractional field, console recordings 2 for all.
     */
    static int displayDigits(boolean fractional, double largestMagnitude) {
        if (!fractional || !(largestMagnitude > 0) || !Double.isFinite(largestMagnitude)) {
            return 0;
        }
        return Math.max(0, Math.min(3, 3 - (int) Math.floor(Math.log10(largestMagnitude))));
    }

    public static CsvLogReader read(Reader source) throws IOException {
        BufferedReader reader = new BufferedReader(source);
        String line;
        String[] names = null;
        while ((line = reader.readLine()) != null) {
            String[] fields = line.split("\t", -1);
            if (fields.length > 1 && GAUGE_NAME_TIME.equalsIgnoreCase(fields[0].trim())) {
                names = fields;
                break;
            }
        }
        if (names == null) {
            throw new IOException("No " + GAUGE_NAME_TIME + " column found in log");
        }

        line = reader.readLine();
        if (line == null) {
            throw new IOException("Missing log units row");
        }
        String[] units = line.split("\t", -1);
        if (units.length < names.length) {
            String[] padded = new String[names.length];
            System.arraycopy(units, 0, padded, 0, units.length);
            for (int i = units.length; i < padded.length; i++) {
                padded[i] = "";
            }
            units = padded;
        }
        double timeScale = units[0].trim().equalsIgnoreCase("ms") ? 0.001 : 1.0;

        List<double[]> rows = new ArrayList<>();
        while ((line = reader.readLine()) != null) {
            if (line.isEmpty() || line.startsWith("MARK")) {
                continue;
            }
            String[] fields = line.split("\t", -1);
            double time = parseNumber(fields[0]);
            if (!Double.isFinite(time)) {
                continue;
            }
            double[] row = new double[names.length];
            for (int i = 0; i < names.length; i++) {
                double value = i < fields.length ? parseNumber(fields[i]) : Double.NaN;
                if (i == 0) {
                    value *= timeScale;
                }
                row[i] = value;
            }
            rows.add(row);
        }
        return fromRows(names, units, rows);
    }

    private static double parseNumber(String value) {
        try {
            return Double.parseDouble(value.trim());
        } catch (NumberFormatException e) {
            return Double.NaN;
        }
    }

    public int fieldCount() { return names.length; }
    public int sampleCount() { return rows.length; }
    public String name(int field) { return names[field].trim(); }
    public String unit(int field) { return units[field].trim(); }
    public double value(int sample, int field) { return rows[sample][field]; }
    public double minimum(int field) { return minimum[field]; }
    public double maximum(int field) { return maximum[field]; }
    /** True when the field never changes in this log (or has no values): most logged channels do not. */
    public boolean isConstant(int field) { return !(maximum[field] > minimum[field]); }
    /** Decimals to show for this field's values; see {@link #displayDigits}. */
    public int digits(int field) { return digits[field]; }

    /** First sample at or after {@code time}; {@link #sampleCount()} when none. Time (field 0) must not decrease. */
    public int firstSampleAtOrAfter(double time) {
        int low = 0;
        int high = rows.length;
        while (low < high) {
            int middle = (low + high) >>> 1;
            if (rows[middle][0] < time) {
                low = middle + 1;
            } else {
                high = middle;
            }
        }
        return low;
    }

    public int nearestSample(double time) {
        int after = firstSampleAtOrAfter(time);
        if (after == 0) {
            return 0;
        }
        if (after == rows.length) {
            return rows.length - 1;
        }
        return time - rows[after - 1][0] <= rows[after][0] - time ? after - 1 : after;
    }

    /** Samples {@code from} to {@code to} (both included) as a log of their own; its ranges cover only them. */
    public CsvLogReader slice(int from, int to) throws IOException {
        if (from < 0 || to >= rows.length || from > to) {
            throw new IllegalArgumentException("Samples " + from + ".." + to + " outside 0.." + (rows.length - 1));
        }
        return fromRows(names, units, java.util.Arrays.asList(rows).subList(from, to + 1));
    }

    /** Smallest positive gap between consecutive samples, or 0 if all samples share one time. */
    public double minimumSampleInterval() {
        double result = Double.POSITIVE_INFINITY;
        for (int i = 1; i < rows.length; i++) {
            double gap = rows[i][0] - rows[i - 1][0];
            if (gap > 0) {
                result = Math.min(result, gap);
            }
        }
        return Double.isFinite(result) ? result : 0;
    }
}
