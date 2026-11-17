package com.rusefi.ui.logview;

import com.rusefi.logview.CsvLogReader;

import java.util.ArrayList;
import java.util.List;

/**
 * The records a scatter plot shows: those in the time range that no enabled filter hides and that have X and Y
 * values, with their Z, time and the seconds each stands for (for density by time). Kept as plain arrays so a plot
 * of hundreds of thousands of points draws quickly.
 */
final class ScatterData {
    final CsvLogReader log;
    final int xField;
    final int yField;
    final int zField;
    final int count;
    final float[] x;
    final float[] y;
    /** NaN where the record has no Z. */
    final float[] z;
    /** The log record each point is, e.g. for its time. */
    final int[] sample;
    /** Seconds each point stands for; see {@link Histogram#weights}. */
    final float[] weight;
    final double xMin;
    final double xMax;
    final double yMin;
    final double yMax;
    /** Records in the time range left out by the filters. */
    final int hiddenRecords;
    final double totalTime;

    private ScatterData(CsvLogReader log, int xField, int yField, int zField, int count, float[] x, float[] y,
                        float[] z, int[] sample, float[] weight, int hiddenRecords) {
        this.log = log;
        this.xField = xField;
        this.yField = yField;
        this.zField = zField;
        this.count = count;
        this.x = x;
        this.y = y;
        this.z = z;
        this.sample = sample;
        this.weight = weight;
        this.hiddenRecords = hiddenRecords;
        double xLow = Double.POSITIVE_INFINITY;
        double xHigh = Double.NEGATIVE_INFINITY;
        double yLow = Double.POSITIVE_INFINITY;
        double yHigh = Double.NEGATIVE_INFINITY;
        double total = 0;
        for (int i = 0; i < count; i++) {
            xLow = Math.min(xLow, x[i]);
            xHigh = Math.max(xHigh, x[i]);
            yLow = Math.min(yLow, y[i]);
            yHigh = Math.max(yHigh, y[i]);
            total += weight[i];
        }
        xMin = count > 0 ? xLow : 0;
        xMax = count > 0 ? xHigh : 1;
        yMin = count > 0 ? yLow : 0;
        yMax = count > 0 ? yHigh : 1;
        totalTime = total;
    }

    /** The shown records from {@code start} to {@code end} seconds (NaN for the whole log). */
    static ScatterData collect(CsvLogReader log, int xField, int yField, int zField, boolean[] hidden, double start,
                               double end) {
        int first = Double.isNaN(start) ? 0 : log.firstSampleAtOrAfter(start);
        int last = Double.isNaN(end) ? log.sampleCount() : log.firstSampleAtOrAfter(Math.nextUp(end));
        int capacity = Math.max(0, last - first);
        float[] x = new float[capacity];
        float[] y = new float[capacity];
        float[] z = new float[capacity];
        int[] sample = new int[capacity];
        float[] weight = new float[capacity];
        double[] weights = Histogram.weights(log);
        int n = 0;
        int hiddenCount = 0;
        for (int s = first; s < last; s++) {
            if (hidden[s]) {
                hiddenCount++;
                continue;
            }
            double xv = log.value(s, xField);
            double yv = log.value(s, yField);
            if (!Double.isFinite(xv) || !Double.isFinite(yv)) {
                continue;
            }
            x[n] = (float) xv;
            y[n] = (float) yv;
            z[n] = (float) log.value(s, zField);
            sample[n] = s;
            weight[n] = (float) weights[s];
            n++;
        }
        return new ScatterData(log, xField, yField, zField, n, x, y, z, sample, weight, hiddenCount);
    }

    double time(int point) {
        return log.value(sample[point], 0);
    }

    /** The points inside a box of X and Y values, in time order. */
    List<Integer> inBox(double xFrom, double xTo, double yFrom, double yTo) {
        double xLow = Math.min(xFrom, xTo);
        double xHigh = Math.max(xFrom, xTo);
        double yLow = Math.min(yFrom, yTo);
        double yHigh = Math.max(yFrom, yTo);
        List<Integer> points = new ArrayList<>();
        for (int i = 0; i < count; i++) {
            if (x[i] >= xLow && x[i] <= xHigh && y[i] >= yLow && y[i] <= yHigh) {
                points.add(i);
            }
        }
        return points;
    }

    /**
     * Runs of consecutive records among the given points (in time order), as stays for the timeline: {start, end,
     * seconds}, each from its first record to its last record plus the time that one stands for.
     */
    List<double[]> stays(List<Integer> points) {
        List<double[]> stays = new ArrayList<>();
        double[] current = null;
        int previousSample = Integer.MIN_VALUE;
        for (int point : points) {
            int s = sample[point];
            double t = log.value(s, 0);
            if (current == null || s != previousSample + 1) {
                current = new double[]{t, t, 0};
                stays.add(current);
            }
            current[1] = t + weight[point];
            current[2] += weight[point];
            previousSample = s;
        }
        return stays;
    }
}
