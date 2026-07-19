package com.rusefi.tune.ve;

/**
 * Abstract base for VE table generators.
 * Provides shared numeric utilities: 3×3 smoothing, linear interpolation, and clamping.
 * Concrete implementations supply their own configuration types, enums, and formula.
 *
 * @see ArchetypeBaseVeV1
 */
public abstract class BaseVeGenerator implements VeGenerator {

    /** Lower clamp bound for all generated VE values (%). */
    protected static final double CLAMP_MIN = 35.0;
    /** Upper clamp bound for all generated VE values (%). */
    protected static final double CLAMP_MAX = 125.0;

    /**
     * One weighted 3×3 smoothing pass over a 2D table.
     * Only interior cells (rows 1..N-2, cols 1..M-2) are averaged; border cells are copied.
     * Tables with fewer than 3 rows or fewer than 3 columns have no interior cells;
     * the pass returns an identical copy (correct, not an error).
     *
     * @param in source matrix [rows][cols]
     * @return new matrix with interior cells smoothed
     */
    protected static double[][] smoothPass(double[][] in) {
        int nRows = in.length;
        int nCols = nRows == 0 ? 0 : in[0].length;
        double[][] out = new double[nRows][nCols];
        for (int r = 0; r < nRows; r++) {
            for (int c = 0; c < nCols; c++) {
                if (r == 0 || r == nRows - 1 || c == 0 || c == nCols - 1) {
                    out[r][c] = in[r][c];
                } else {
                    out[r][c] = (4 * in[r][c]
                        + 2 * (in[r-1][c] + in[r+1][c] + in[r][c-1] + in[r][c+1])
                        + in[r-1][c-1] + in[r-1][c+1] + in[r+1][c-1] + in[r+1][c+1]
                    ) / 16.0;
                }
            }
        }
        return out;
    }

    /** Linear interpolation between {@code (x0,y0)} and {@code (x1,y1)} at {@code x}. */
    protected static double lerp(double x0, double y0, double x1, double y1, double x) {
        return y0 + (x - x0) / (x1 - x0) * (y1 - y0);
    }

    /** Clamp {@code v} to [{@link #CLAMP_MIN}, {@link #CLAMP_MAX}]. */
    protected static double clamp(double v) {
        return Math.max(CLAMP_MIN, Math.min(CLAMP_MAX, v));
    }
}
