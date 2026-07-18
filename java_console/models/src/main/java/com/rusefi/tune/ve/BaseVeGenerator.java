package com.rusefi.tune.ve;

import java.util.ArrayList;
import java.util.Collections;
import java.util.List;
import java.util.Objects;

/**
 * Pure headless VE table generator — no Swing, no INI, no ConfigurationImage.
 * <p>
 * Profile "archetype-base-ve-v1": constants are versioned. Change only under a new
 * profile ID with updated tests, and maybe a real engine test?.
 */
public class BaseVeGenerator {

    public static final String PROFILE_ID = "archetype-base-ve-v1";

    private static final double CLAMP_MIN = 35.0;
    private static final double CLAMP_MAX = 125.0;

    // MAP factor lookup: {absoluteKpa, factor}. Points must remain in ascending kPa order.
    private static final double[][] MAP_FACTORS = {
        {10, 0.60}, {20, 0.65}, {40, 0.72}, {60, 0.82}, {80, 0.92}, {100, 1.00}
    };

    public enum HeadArchetype {
        RESTRICTIVE_2V("Restrictive/legacy 2-valve", 85.0),
        PORTED_2V("Good/ported 2-valve", 92.0),
        STOCK_4V("Stock modern 4-valve", 98.0),
        HIGHFLOW_4V("High-flow/performance 4-valve", 104.0);

        public final String label;
        public final double peakVe;

        HeadArchetype(String label, double peakVe) {
            this.label = label;
            this.peakVe = peakVe;
        }
    }

    public enum CamBehavior {
        STOCK("Stock",           0.48, 28, 10, 18),
        MILD("Mild",             0.58, 34, 14, 14),
        AGGRESSIVE("Aggressive", 0.68, 42, 20, 10);

        public final String label;
        public final double peakPosition;
        public final double idleDrop;
        public final double lowRpmDrop;
        public final double redlineDrop;

        CamBehavior(String label, double peakPosition,
                    double idleDrop, double lowRpmDrop, double redlineDrop) {
            this.label = label;
            this.peakPosition = peakPosition;
            this.idleDrop = idleDrop;
            this.lowRpmDrop = lowRpmDrop;
            this.redlineDrop = redlineDrop;
        }
    }

    public enum Aspiration {
        NATURALLY_ASPIRATED("Naturally aspirated"),
        TURBOCHARGED("Turbocharged"),
        SUPERCHARGED("Supercharged");

        public final String label;

        Aspiration(String label) {
            this.label = label;
        }
    }

    /** Immutable generation request. */
    public static class Request {
        public final double idleRpm;
        public final double maximumRpm;
        public final HeadArchetype head;
        public final CamBehavior cam;
        public final boolean hasVvt;
        public final Aspiration aspiration;

        /**
         * @param isFourStrokeSI must be true; the generator blocks non-SI engine types
         */
        public Request(boolean isFourStrokeSI,
                       double idleRpm,
                       double maximumRpm,
                       HeadArchetype head,
                       CamBehavior cam,
                       boolean hasVvt,
                       Aspiration aspiration) {
            if (!isFourStrokeSI) {
                throw new IllegalArgumentException(
                    "archetype-base-ve-v1 supports only conventional reciprocating " +
                    "four-stroke spark-ignition engines.");
            }
            if (!Double.isFinite(idleRpm) || !Double.isFinite(maximumRpm)) {
                throw new IllegalArgumentException("idleRpm and maximumRpm must be finite.");
            }
            if (maximumRpm <= idleRpm) {
                throw new IllegalArgumentException("maximumRpm must be greater than idleRpm.");
            }
            if (maximumRpm - idleRpm < 500) {
                throw new IllegalArgumentException("maximumRpm - idleRpm must be at least 500 RPM.");
            }
            this.idleRpm = idleRpm;
            this.maximumRpm = maximumRpm;
            this.head = Objects.requireNonNull(head, "head");
            this.cam = Objects.requireNonNull(cam, "cam");
            this.hasVvt = hasVvt;
            this.aspiration = Objects.requireNonNull(aspiration, "aspiration");
        }
    }

    /**
     * Immutable generation result.
     * veTable[loadRow][rpmCol] — same orientation as ECU storage (load is first index).
     */
    public static class Result {
        public final double[][] veTable;
        public final String profileId;
        public final List<String> warnings;

        Result(double[][] veTable, String profileId, List<String> warnings) {
            this.veTable = veTable;
            this.profileId = profileId;
            this.warnings = Collections.unmodifiableList(new ArrayList<>(warnings));
        }
    }

    /**
     * Generate a VE surface for the given request and target axes.
     *
     * @param request  validated generation parameters
     * @param rpmAxis  ECU RPM axis values, strictly ascending, finite (length >= 1)
     * @param mapAxis  ECU MAP/load axis values in kPa, strictly ascending, finite (length >= 1)
     * @return Result with veTable[loadRow][rpmCol], profileId, and warnings
     */
    public static Result generate(Request request, double[] rpmAxis, double[] mapAxis) {
        Objects.requireNonNull(request, "request");
        Objects.requireNonNull(rpmAxis, "rpmAxis");
        Objects.requireNonNull(mapAxis, "mapAxis");
        if (rpmAxis.length == 0) {
            throw new IllegalArgumentException("rpmAxis must not be empty.");
        }
        if (mapAxis.length == 0) {
            throw new IllegalArgumentException("mapAxis must not be empty.");
        }

        // Apply VVT broadening multipliers to cam drops
        double vvtIdleMult    = request.hasVvt ? 0.90 : 1.0;
        double vvtRangeMult   = request.hasVvt ? 0.75 : 1.0;
        double idleDrop    = request.cam.idleDrop    * vvtIdleMult;
        double lowRpmDrop  = request.cam.lowRpmDrop  * vvtRangeMult;
        double redlineDrop = request.cam.redlineDrop * vvtRangeMult;

        // Compute RPM shape anchors
        double peakRpm  = request.idleRpm + request.cam.peakPosition * (request.maximumRpm - request.idleRpm);
        double lowRpm   = request.idleRpm + 0.5 * (peakRpm - request.idleRpm);
        double highRpm  = peakRpm + 0.5 * (request.maximumRpm - peakRpm);
        double peakVe   = request.head.peakVe;
        double idleVe   = peakVe - idleDrop;
        double lowVe    = peakVe - lowRpmDrop;
        double highVe   = peakVe - redlineDrop / 2.0;
        double redlineVe= peakVe - redlineDrop;

        // WOT RPM curve at each axis bin
        double[] rpmCurve = new double[rpmAxis.length];
        for (int c = 0; c < rpmAxis.length; c++) {
            rpmCurve[c] = rpmVe(rpmAxis[c],
                request.idleRpm, lowRpm, peakRpm, highRpm, request.maximumRpm,
                idleVe, lowVe, peakVe, highVe, redlineVe);
        }

        // MAP factor at each load bin
        double[] mapFactor = new double[mapAxis.length];
        for (int r = 0; r < mapAxis.length; r++) {
            mapFactor[r] = mapVeFactor(mapAxis[r]);
        }

        // Build raw surface, pre-clamp
        int nLoad = mapAxis.length;
        int nRpm  = rpmAxis.length;
        double[][] table = new double[nLoad][nRpm];
        for (int r = 0; r < nLoad; r++) {
            for (int c = 0; c < nRpm; c++) {
                table[r][c] = clamp(rpmCurve[c] * mapFactor[r]);
            }
        }

        // Two deterministic weighted 3×3 smoothing passes
        table = smoothPass(table);
        table = smoothPass(table);

        // Post-smooth clamp
        for (int r = 0; r < nLoad; r++) {
            for (int c = 0; c < nRpm; c++) {
                table[r][c] = clamp(table[r][c]);
            }
        }

        List<String> warnings = new ArrayList<>();
        if (request.aspiration != Aspiration.NATURALLY_ASPIRATED) {
            warnings.add(
                "Boosted: initial operation at wastegate/minimum boost only. " +
                "Verify lambda, fuel pressure, and MAP/IAT calibration before increasing load. " +
                "Rows above 100 kPa use the same WOT curve shape as atmospheric; ");
        }

        return new Result(table, PROFILE_ID, warnings);
    }

    // -- formula helpers (package-private for testing) --

    static double rpmVe(double rpm,
                        double idleRpm, double lowRpm, double peakRpm,
                        double highRpm, double maxRpm,
                        double idleVe, double lowVe, double peakVe,
                        double highVe, double redlineVe) {
        if (rpm <= idleRpm) return idleVe;
        if (rpm >= maxRpm)  return redlineVe;
        if (rpm <= lowRpm)  return lerp(idleRpm, idleVe,  lowRpm,  lowVe,     rpm);
        if (rpm <= peakRpm) return lerp(lowRpm,  lowVe,   peakRpm, peakVe,    rpm);
        if (rpm <= highRpm) return lerp(peakRpm, peakVe,  highRpm, highVe,    rpm);
        return              lerp(highRpm, highVe, maxRpm,  redlineVe, rpm);
    }

    static double mapVeFactor(double kpa) {
        if (kpa <= MAP_FACTORS[0][0]) {
            return MAP_FACTORS[0][1];
        }
        if (kpa >= MAP_FACTORS[MAP_FACTORS.length - 1][0]) {
            return MAP_FACTORS[MAP_FACTORS.length - 1][1];
        }
        for (int i = 1; i < MAP_FACTORS.length; i++) {
            if (kpa <= MAP_FACTORS[i][0]) {
                return lerp(MAP_FACTORS[i-1][0], MAP_FACTORS[i-1][1],
                            MAP_FACTORS[i][0],   MAP_FACTORS[i][1], kpa);
            }
        }
        return MAP_FACTORS[MAP_FACTORS.length - 1][1];
    }

    /**
     * One weighted 3×3 smoothing pass. Interior cells only; border cells copied unchanged.
     * Tables with fewer than 3 rows or fewer than 3 columns have no interior cells;
     * the output is an identical copy of the input (correct, not an error).
     */
    static double[][] smoothPass(double[][] in) {
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

    private static double lerp(double x0, double y0, double x1, double y1, double x) {
        return y0 + (x - x0) / (x1 - x0) * (y1 - y0);
    }

    private static double clamp(double v) {
        return Math.max(CLAMP_MIN, Math.min(CLAMP_MAX, v));
    }
}
