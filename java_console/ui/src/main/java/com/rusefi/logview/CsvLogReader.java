package com.rusefi.logview;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.Reader;
import java.util.ArrayList;
import java.util.List;

/** A tab-delimited time-series log with an optional preamble and a units row. */
public final class CsvLogReader {
    private final String[] names;
    private final String[] units;
    private final double[][] rows;
    private final double[] minimum;
    private final double[] maximum;

    private CsvLogReader(String[] names, String[] units, double[][] rows, double[] minimum, double[] maximum) {
        this.names = names;
        this.units = units;
        this.rows = rows;
        this.minimum = minimum;
        this.maximum = maximum;
    }

    static CsvLogReader fromRows(String[] names, String[] units, List<double[]> samples) throws IOException {
        if (samples.isEmpty()) {
            throw new IOException("Log has no numeric samples");
        }
        double[] minimum = new double[names.length];
        double[] maximum = new double[names.length];
        java.util.Arrays.fill(minimum, Double.POSITIVE_INFINITY);
        java.util.Arrays.fill(maximum, Double.NEGATIVE_INFINITY);
        for (double[] row : samples) {
            for (int i = 0; i < names.length; i++) {
                double value = row[i];
                if (Double.isFinite(value)) {
                    minimum[i] = Math.min(minimum[i], value);
                    maximum[i] = Math.max(maximum[i], value);
                }
            }
        }
        return new CsvLogReader(names, units, samples.toArray(new double[0][]), minimum, maximum);
    }

    public static CsvLogReader read(Reader source) throws IOException {
        BufferedReader reader = new BufferedReader(source);
        String line;
        String[] names = null;
        while ((line = reader.readLine()) != null) {
            String[] fields = line.split("\t", -1);
            if (fields.length > 1 && "Time".equalsIgnoreCase(fields[0].trim())) {
                names = fields;
                break;
            }
        }
        if (names == null) {
            throw new IOException("No Time column found in log");
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
}
