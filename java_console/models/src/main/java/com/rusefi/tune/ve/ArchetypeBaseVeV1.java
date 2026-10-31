package com.rusefi.tune.ve;

import java.util.ArrayList;
import java.util.List;
import java.util.Objects;

/**
 * Archetype Base VE v1 — a transparent heuristic for conventional four-stroke SI engines.
 * <p>
 * Profile ID: {@value #PROFILE_ID}.
 * Constants are part of the version; change only under a new profile ID with updated fixtures.
 */
public class ArchetypeBaseVeV1 extends BaseVeGenerator {

    public static final String PROFILE_ID    = "archetype-base-ve-v1";
    public static final String DISPLAY_NAME  = "Archetype Base VE v1";

    // MAP factor lookup: {absoluteKpa, factor}. Must remain in ascending kPa order.
    static final double[][] MAP_FACTORS = {
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

    /** Immutable configuration for one generation run. */
    public static class Request {
        public final double idleRpm;
        public final double maximumRpm;
        public final HeadArchetype head;
        public final CamBehavior cam;
        public final boolean hasVvt;
        public final Aspiration aspiration;

        /**
         * @param isFourStrokeSI must be {@code true}; the generator blocks other engine types
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
                    PROFILE_ID + " supports only conventional reciprocating " +
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

    private final Request request;

    public ArchetypeBaseVeV1(Request request) {
        this.request = Objects.requireNonNull(request, "request");
    }

    @Override
    public String getProfileId() {
        return PROFILE_ID;
    }

    @Override
    public String getDisplayName() {
        return DISPLAY_NAME;
    }

    @Override
    public Result generate(double[] rpmAxis, double[] mapAxis) {
        Objects.requireNonNull(rpmAxis, "rpmAxis");
        Objects.requireNonNull(mapAxis, "mapAxis");
        if (rpmAxis.length == 0) throw new IllegalArgumentException("rpmAxis must not be empty.");
        if (mapAxis.length == 0) throw new IllegalArgumentException("mapAxis must not be empty.");

        // Apply VVT broadening multipliers to cam drops
        double idleDrop    = request.cam.idleDrop    * (request.hasVvt ? 0.90 : 1.0);
        double lowRpmDrop  = request.cam.lowRpmDrop  * (request.hasVvt ? 0.75 : 1.0);
        double redlineDrop = request.cam.redlineDrop * (request.hasVvt ? 0.75 : 1.0);

        // RPM shape anchors
        double peakRpm   = request.idleRpm + request.cam.peakPosition * (request.maximumRpm - request.idleRpm);
        double lowRpm    = request.idleRpm + 0.5 * (peakRpm - request.idleRpm);
        double highRpm   = peakRpm + 0.5 * (request.maximumRpm - peakRpm);
        double peakVe    = request.head.peakVe;
        double idleVe    = peakVe - idleDrop;
        double lowVe     = peakVe - lowRpmDrop;
        double highVe    = peakVe - redlineDrop / 2.0;
        double redlineVe = peakVe - redlineDrop;

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

        // Build raw surface and pre-clamp
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
                "Rows above 100 kPa use the same WOT curve shape as atmospheric; " +
                "STFT/LTFT may be disabled or limited at high load.");
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
}
