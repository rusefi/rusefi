package com.rusefi.tune.ve;

import java.util.ArrayList;
import java.util.List;
import java.util.Objects;

/**
 * Archetype Base VE v1 — Turbocharged variant.
 * <p>
 * Profile ID: {@value #PROFILE_ID}.
 * Constants are part of the version; change only under a new profile ID with updated fixtures.
 * <p>
 * Differences from {@link ArchetypeBaseVeV1}:
 * <ul>
 *   <li>RPM curve drops reduced at idle/low RPM ({@link #IDLE_DROP_MULT}, {@link #LOW_DROP_MULT}) —
 *       boost pressure compensates for cam deficiency at low RPM.</li>
 *   <li>Redline drop slightly increased ({@link #REDLINE_DROP_MULT}) —
 *       exhaust backpressure penalty at high RPM under boost.</li>
 *   <li>MAP rows above 100 kPa use a linear upward-sloping factor ({@link #BOOST_SLOPE})
 *       instead of the flat 1.00 cap in v1. Excess fuel (rich) is safer than lean at first start.</li>
 *   <li>When VVT is present, an additional RPM-dependent factor ramps up above peak RPM in boost
 *       rows ({@link #VVT_BOOST_SLOPE}), reflecting boost-driven scavenging gains.</li>
 *   <li>VE ceiling raised to {@link #TURBO_CLAMP_MAX} to accommodate high-boost calibrated values.</li>
 * </ul>
 */
public class ArchetypeBaseVeV1Turbo extends BaseVeGenerator {

    public static final String PROFILE_ID   = "archetype-base-ve-v1-turbo";
    public static final String DISPLAY_NAME = "Archetype Base VE v1 \u2014 Turbocharged";

    static final double TURBO_CLAMP_MAX = 160.0;

    // Boost pressure helps cylinder filling at low RPM where cam timing is suboptimal.
    static final double IDLE_DROP_MULT    = 0.50;
    static final double LOW_DROP_MULT     = 0.50;
    // Exhaust backpressure at high RPM under full boost slightly worsens redline breathing.
    static final double REDLINE_DROP_MULT = 1.10;

    // factor = 1.0 + BOOST_SLOPE * (kpa - 100) / 100  for MAP >= 100 kPa.
    // Aggressive default: excess fuel is safer than lean on a first-start boosted engine.
    static final double BOOST_SLOPE = 0.20;

    // Additional slope above BOOST_SLOPE that ramps linearly from peakRpm to maxRpm
    // in boost rows when VVT is present. At maxRpm, effective slope = BOOST_SLOPE + VVT_BOOST_SLOPE.
    static final double VVT_BOOST_SLOPE = 0.25;

    private final ArchetypeBaseVeV1.Request request;

    public ArchetypeBaseVeV1Turbo(ArchetypeBaseVeV1.Request request) {
        Objects.requireNonNull(request, "request");
        if (request.aspiration != ArchetypeBaseVeV1.Aspiration.TURBOCHARGED) {
            throw new IllegalArgumentException(
                PROFILE_ID + " requires aspiration == TURBOCHARGED; got " + request.aspiration);
        }
        this.request = request;
    }

    @Override
    public String getProfileId() { return PROFILE_ID; }

    @Override
    public String getDisplayName() { return DISPLAY_NAME; }

    @Override
    public Result generate(double[] rpmAxis, double[] mapAxis) {
        Objects.requireNonNull(rpmAxis, "rpmAxis");
        Objects.requireNonNull(mapAxis, "mapAxis");
        if (rpmAxis.length == 0) throw new IllegalArgumentException("rpmAxis must not be empty.");
        if (mapAxis.length == 0) throw new IllegalArgumentException("mapAxis must not be empty.");

        // VVT broadening applied first, then turbo multipliers on top
        double idleDrop    = request.cam.idleDrop    * (request.hasVvt ? 0.90 : 1.0) * IDLE_DROP_MULT;
        double lowRpmDrop  = request.cam.lowRpmDrop  * (request.hasVvt ? 0.75 : 1.0) * LOW_DROP_MULT;
        double redlineDrop = request.cam.redlineDrop * (request.hasVvt ? 0.75 : 1.0) * REDLINE_DROP_MULT;

        double peakRpm   = request.idleRpm + request.cam.peakPosition * (request.maximumRpm - request.idleRpm);
        double lowRpm    = request.idleRpm + 0.5 * (peakRpm - request.idleRpm);
        double highRpm   = peakRpm + 0.5 * (request.maximumRpm - peakRpm);
        double peakVe    = request.head.peakVe;
        double idleVe    = peakVe - idleDrop;
        double lowVe     = peakVe - lowRpmDrop;
        double highVe    = peakVe - redlineDrop / 2.0;
        double redlineVe = peakVe - redlineDrop;

        double[] rpmCurve = new double[rpmAxis.length];
        for (int c = 0; c < rpmAxis.length; c++) {
            rpmCurve[c] = ArchetypeBaseVeV1.rpmVe(rpmAxis[c],
                request.idleRpm, lowRpm, peakRpm, highRpm, request.maximumRpm,
                idleVe, lowVe, peakVe, highVe, redlineVe);
        }

        int nLoad = mapAxis.length;
        int nRpm  = rpmAxis.length;
        double[][] table = new double[nLoad][nRpm];
        for (int r = 0; r < nLoad; r++) {
            double kpa = mapAxis[r];
            for (int c = 0; c < nRpm; c++) {
                double mf;
                if (kpa < 100.0) {
                    mf = ArchetypeBaseVeV1.mapVeFactor(kpa);
                } else {
                    mf = 1.0 + BOOST_SLOPE * (kpa - 100.0) / 100.0;
                    if (request.hasVvt && rpmAxis[c] > peakRpm) {
                        double rpmFrac = Math.min(1.0,
                            (rpmAxis[c] - peakRpm) / (request.maximumRpm - peakRpm));
                        mf += VVT_BOOST_SLOPE * rpmFrac * (kpa - 100.0) / 100.0;
                    }
                }
                table[r][c] = boostClamp(rpmCurve[c] * mf);
            }
        }

        table = smoothPass(table);
        table = smoothPass(table);

        for (int r = 0; r < nLoad; r++) {
            for (int c = 0; c < nRpm; c++) {
                table[r][c] = boostClamp(table[r][c]);
            }
        }

        List<String> warnings = new ArrayList<>();
        warnings.add(
            "Turbocharged: start at wastegate/minimum boost only. " +
            "Verify lambda, fuel pressure, and MAP/IAT calibration before increasing boost. " +
            "Boost rows above 100 kPa use an approximate upward-sloping factor (base slope " +
            BOOST_SLOPE + " per 100 kPa); actual values require lambda-driven calibration. " +
            "VE ceiling raised to " + (int) TURBO_CLAMP_MAX + "%.");

        return new Result(table, PROFILE_ID, warnings);
    }

    private static double boostClamp(double v) {
        return Math.max(CLAMP_MIN, Math.min(TURBO_CLAMP_MAX, v));
    }
}
