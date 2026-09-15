package com.rusefi.ui;

import java.util.Arrays;

/**
 * Pure, UI-independent model for lambda-driven VE-table autotune.
 *
 * <p>The model mirrors the classic TunerStudio "VE Analyze Live" correction:
 * each valid closed-loop sample yields a per-cell error ratio
 * {@code actualLambda / targetLambda}; a damped fraction of the mean error is
 * then applied to the corresponding VE cell:</p>
 *
 * <pre>newVe = ve * (1 + damping * (meanRatio - 1))</pre>
 *
 * <p>No ECU / Swing / ini dependencies: the pane feeds it axes, current VE and
 * samples. This keeps it unit-testable on any host platform.</p>
 */
public class VeAnalyzeModel {

    /** Per-cell confidence based on the number of contributing samples. */
    public enum Confidence {
        NONE, LOW, MEDIUM, HIGH
    }

    private static final int LOW_THRESHOLD = 5;
    private static final int MEDIUM_THRESHOLD = 20;

    private final double[] rpmAxis;   // ascending
    private final double[] loadAxis;  // ascending
    private final double[][] currentVe; // [loadIndex][rpmIndex]

    private final double[][] errorSum;   // [load][rpm] accumulated actual/target ratios
    private final int[][] sampleCounts;  // [load][rpm]

    private final double damping;

    public VeAnalyzeModel(double[] rpmAxis, double[] loadAxis, double[][] currentVe) {
        this(rpmAxis, loadAxis, currentVe, 0.5);
    }

    public VeAnalyzeModel(double[] rpmAxis, double[] loadAxis, double[][] currentVe, double damping) {
        if (rpmAxis == null || loadAxis == null || currentVe == null) {
            throw new IllegalArgumentException("axes and currentVe must not be null");
        }
        if (currentVe.length != loadAxis.length) {
            throw new IllegalArgumentException(
                "currentVe rows (" + currentVe.length + ") != loadAxis length (" + loadAxis.length + ")");
        }
        for (double[] row : currentVe) {
            if (row == null || row.length != rpmAxis.length) {
                throw new IllegalArgumentException(
                    "currentVe columns (" + (row == null ? "null" : row.length) + ") != rpmAxis length (" + rpmAxis.length + ")");
            }
        }
        this.rpmAxis = rpmAxis;
        this.loadAxis = loadAxis;
        this.currentVe = currentVe;
        this.damping = damping;

        this.errorSum = new double[loadAxis.length][rpmAxis.length];
        this.sampleCounts = new int[loadAxis.length][rpmAxis.length];
    }

    /** Index of the nearest RPM cell, or -1 if out of range. */
    public int rpmIndex(double rpm) {
        return index(rpmAxis, rpm);
    }

    /** Index of the nearest load cell, or -1 if out of range. */
    public int loadIndex(double load) {
        return index(loadAxis, load);
    }

    /**
     * Accumulate one valid sample.
     *
     * @param egoActive    whether closed-loop (EGO) correction is currently active
     * @param actualLambda measured lambda (or AFR — any consistent unit)
     * @param targetLambda target lambda in the same unit as {@code actualLambda}
     * @return the cell [load, rpm] that received the sample, or null when rejected
     */
    public int[] addSample(double rpm, double load, double actualLambda, double targetLambda, boolean egoActive) {
        if (!egoActive || targetLambda <= 0 || actualLambda <= 0) {
            return null;
        }
        int r = rpmIndex(rpm);
        int c = loadIndex(load);
        if (r < 0 || c < 0) {
            return null;
        }
        errorSum[c][r] += actualLambda / targetLambda;
        sampleCounts[c][r]++;
        return new int[]{c, r};
    }

    /** Damped, corrected VE table [load][rpm]. Cells without data are unchanged. */
    public double[][] getSuggestedVe() {
        double[][] suggested = new double[loadAxis.length][rpmAxis.length];
        for (int c = 0; c < loadAxis.length; c++) {
            for (int r = 0; r < rpmAxis.length; r++) {
                int n = sampleCounts[c][r];
                if (n == 0) {
                    suggested[c][r] = currentVe[c][r];
                } else {
                    double meanRatio = errorSum[c][r] / n;
                    suggested[c][r] = currentVe[c][r] * (1.0 + damping * (meanRatio - 1.0));
                }
            }
        }
        return suggested;
    }

    /**
     * Per-cell correction as a percentage: {@code (suggested - current) / current * 100}.
     * Zero where the current VE is ~0 or the cell has no data.
     */
    public double[][] getDeltaPercent() {
        double[][] suggested = getSuggestedVe();
        double[][] delta = new double[loadAxis.length][rpmAxis.length];
        for (int c = 0; c < loadAxis.length; c++) {
            for (int r = 0; r < rpmAxis.length; r++) {
                double cur = currentVe[c][r];
                delta[c][r] = Math.abs(cur) > 1e-9 ? (suggested[c][r] - cur) / cur * 100.0 : 0.0;
            }
        }
        return delta;
    }

    public double[][] getCurrentVe() {
        double[][] copy = new double[loadAxis.length][rpmAxis.length];
        for (int c = 0; c < loadAxis.length; c++) {
            copy[c] = Arrays.copyOf(currentVe[c], rpmAxis.length);
        }
        return copy;
    }

    public int[][] getSampleCounts() {
        int[][] copy = new int[loadAxis.length][rpmAxis.length];
        for (int c = 0; c < loadAxis.length; c++) {
            copy[c] = Arrays.copyOf(sampleCounts[c], rpmAxis.length);
        }
        return copy;
    }

    public double[] getRpmAxis() {
        return Arrays.copyOf(rpmAxis, rpmAxis.length);
    }

    public double[] getLoadAxis() {
        return Arrays.copyOf(loadAxis, loadAxis.length);
    }

    public double getDamping() {
        return damping;
    }

    public int getTotalCells() {
        return loadAxis.length * rpmAxis.length;
    }

    public int getCellsWithData() {
        int count = 0;
        for (int[] row : sampleCounts) {
            for (int n : row) {
                if (n > 0) {
                    count++;
                }
            }
        }
        return count;
    }

    /** Fraction of cells that have at least one sample (0..1). */
    public double getCoverage() {
        int total = getTotalCells();
        return total == 0 ? 0.0 : (double) getCellsWithData() / total;
    }

    public Confidence getConfidence(int loadIdx, int rpmIdx) {
        if (loadIdx < 0 || loadIdx >= sampleCounts.length || rpmIdx < 0 || rpmIdx >= rpmAxis.length) {
            return Confidence.NONE;
        }
        int n = sampleCounts[loadIdx][rpmIdx];
        if (n <= 0) {
            return Confidence.NONE;
        }
        if (n < LOW_THRESHOLD) {
            return Confidence.LOW;
        }
        if (n <= MEDIUM_THRESHOLD) {
            return Confidence.MEDIUM;
        }
        return Confidence.HIGH;
    }

    public void reset() {
        for (int c = 0; c < loadAxis.length; c++) {
            Arrays.fill(errorSum[c], 0.0);
            Arrays.fill(sampleCounts[c], 0);
        }
    }

    private static int index(double[] axis, double value) {
        if (value < axis[0] || value > axis[axis.length - 1]) {
            return -1;
        }
        int idx = 0;
        double best = Math.abs(value - axis[0]);
        for (int i = 1; i < axis.length; i++) {
            double d = Math.abs(value - axis[i]);
            if (d < best) {
                best = d;
                idx = i;
            }
        }
        return idx;
    }

    /**
     * Bilinear interpolation on a table {@code z[y][x]} over {@code xAxis}/{@code yAxis},
     * clamped to the axis extents.
     */
    public static double interpolate(double x, double y, double[] xAxis, double[] yAxis, double[][] z) {
        if (xAxis.length == 0 || yAxis.length == 0 || z.length != yAxis.length) {
            return Double.NaN;
        }
        int ix = lowerIndex(xAxis, x);
        int iy = lowerIndex(yAxis, y);

        double x0 = xAxis[ix], x1 = xAxis[Math.min(ix + 1, xAxis.length - 1)];
        double y0 = yAxis[iy], y1 = yAxis[Math.min(iy + 1, yAxis.length - 1)];

        double tx = (x1 == x0) ? 0.0 : (x - x0) / (x1 - x0);
        double ty = (y1 == y0) ? 0.0 : (y - y0) / (y1 - y0);

        double z00 = z[iy][ix];
        double z10 = z[iy][Math.min(ix + 1, xAxis.length - 1)];
        double z01 = z[Math.min(iy + 1, yAxis.length - 1)][ix];
        double z11 = z[Math.min(iy + 1, yAxis.length - 1)][Math.min(ix + 1, xAxis.length - 1)];

        double zx0 = z00 + (z10 - z00) * tx;
        double zx1 = z01 + (z11 - z01) * tx;
        return zx0 + (zx1 - zx0) * ty;
    }

    private static int lowerIndex(double[] axis, double value) {
        int idx = 0;
        while (idx < axis.length - 2 && axis[idx + 1] <= value) {
            idx++;
        }
        return idx;
    }
}
