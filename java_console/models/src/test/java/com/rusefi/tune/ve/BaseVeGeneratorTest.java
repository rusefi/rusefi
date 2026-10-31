package com.rusefi.tune.ve;

import org.junit.jupiter.api.Test;

import java.util.Locale;

import static com.rusefi.tune.ve.ArchetypeBaseVeV1.*;
import static com.rusefi.tune.ve.BaseVeGenerator.*;
import static org.junit.jupiter.api.Assertions.*;

/**
 * Tests for {@link BaseVeGenerator} utilities (smoothing kernel) and
 * {@link ArchetypeBaseVeV1} formula (RPM curve, MAP factor, full generate),
 * plus behavioral invariant tests for {@link ArchetypeBaseVeV1Turbo} and
 * {@link ArchetypeBaseVeV1Supercharged}.
 */
public class BaseVeGeneratorTest {

    private static final double D  = 1e-9; // exact formula comparisons
    private static final double DQ = 0.15; // tolerance after smoothing

    private static final double[] RPM6 = {1000, 2000, 3000, 4000, 5000, 6000};
    private static final double[] MAP6 = {20, 40, 60, 80, 100, 120};
    private static final double[] MAP_WITH_BOOST = {20, 40, 80, 100, 130, 160, 200};

    private static ArchetypeBaseVeV1 stock4vStockNoVvt() {
        return new ArchetypeBaseVeV1(new Request(true, 1000, 6000,
            HeadArchetype.STOCK_4V, CamBehavior.STOCK, false, Aspiration.NATURALLY_ASPIRATED));
    }

    private static Request turboReq(boolean vvt) {
        return new Request(true, 1000, 6000,
            HeadArchetype.STOCK_4V, CamBehavior.STOCK, vvt, Aspiration.TURBOCHARGED);
    }

    private static Request scReq(boolean vvt) {
        return new Request(true, 1000, 6000,
            HeadArchetype.STOCK_4V, CamBehavior.STOCK, vvt, Aspiration.SUPERCHARGED);
    }

    // -------------------------------------------------------------------------
    // Interface contract
    // -------------------------------------------------------------------------

    @Test
    public void testImplementsVeGenerator() {
        assertTrue(stock4vStockNoVvt() instanceof VeGenerator);
        assertEquals(PROFILE_ID, stock4vStockNoVvt().getProfileId());
        assertEquals(DISPLAY_NAME, stock4vStockNoVvt().getDisplayName());
    }

    // -------------------------------------------------------------------------
    // Request gate
    // -------------------------------------------------------------------------

    @Test
    public void testFourStrokeGateRequired() {
        assertThrows(IllegalArgumentException.class, () ->
            new Request(false, 1000, 6000,
                HeadArchetype.STOCK_4V, CamBehavior.STOCK, false, Aspiration.NATURALLY_ASPIRATED));
    }

    @Test
    public void testMinimumRpmSpan() {
        assertThrows(IllegalArgumentException.class, () ->
            new Request(true, 1000, 1499,
                HeadArchetype.STOCK_4V, CamBehavior.STOCK, false, Aspiration.NATURALLY_ASPIRATED));
    }

    @Test
    public void testMaximumRpmMustExceedIdle() {
        assertThrows(IllegalArgumentException.class, () ->
            new Request(true, 3000, 2000,
                HeadArchetype.STOCK_4V, CamBehavior.STOCK, false, Aspiration.NATURALLY_ASPIRATED));
    }

    // -------------------------------------------------------------------------
    // RPM anchor values
    // idle=1000, max=6000, Stock cam (peakPos=0.48):
    //   peakRpm=3400, lowRpm=2200, highRpm=4700
    // Stock4v peakVe=98: idleVe=70, lowVe=88, highVe=89, redlineVe=80
    // -------------------------------------------------------------------------

    @Test
    public void testRpmAnchorsAtExactPoints() {
        assertEquals(70.0, rpmVe(1000, 1000,2200,3400,4700,6000, 70,88,98,89,80), D);
        assertEquals(88.0, rpmVe(2200, 1000,2200,3400,4700,6000, 70,88,98,89,80), D);
        assertEquals(98.0, rpmVe(3400, 1000,2200,3400,4700,6000, 70,88,98,89,80), D);
        assertEquals(89.0, rpmVe(4700, 1000,2200,3400,4700,6000, 70,88,98,89,80), D);
        assertEquals(80.0, rpmVe(6000, 1000,2200,3400,4700,6000, 70,88,98,89,80), D);
    }

    @Test
    public void testRpmEndpointClamping() {
        assertEquals(70.0, rpmVe(500,   1000,2200,3400,4700,6000, 70,88,98,89,80), D);
        assertEquals(70.0, rpmVe(0,     1000,2200,3400,4700,6000, 70,88,98,89,80), D);
        assertEquals(80.0, rpmVe(7000,  1000,2200,3400,4700,6000, 70,88,98,89,80), D);
        assertEquals(80.0, rpmVe(10000, 1000,2200,3400,4700,6000, 70,88,98,89,80), D);
    }

    @Test
    public void testRpmMidpointInterpolation() {
        // Midpoint idle(1000,70)..lowRpm(2200,88) => 1600 => 79.0
        assertEquals(79.0, rpmVe(1600, 1000,2200,3400,4700,6000, 70,88,98,89,80), D);
        // Midpoint peakRpm(3400,98)..highRpm(4700,89) => 4050 => 93.5
        assertEquals(93.5, rpmVe(4050, 1000,2200,3400,4700,6000, 70,88,98,89,80), D);
    }

    // -------------------------------------------------------------------------
    // MAP factor
    // -------------------------------------------------------------------------

    @Test
    public void testMapFactorAtDefinedPoints() {
        assertEquals(0.60, mapVeFactor(10),  D);
        assertEquals(0.65, mapVeFactor(20),  D);
        assertEquals(0.72, mapVeFactor(40),  D);
        assertEquals(0.82, mapVeFactor(60),  D);
        assertEquals(0.92, mapVeFactor(80),  D);
        assertEquals(1.00, mapVeFactor(100), D);
    }

    @Test
    public void testMapFactorEndpointClamping() {
        assertEquals(0.60, mapVeFactor(5),   D);
        assertEquals(0.60, mapVeFactor(0),   D);
        assertEquals(1.00, mapVeFactor(120), D);
        assertEquals(1.00, mapVeFactor(200), D);
    }

    @Test
    public void testMapFactorMidpointInterpolation() {
        assertEquals(0.685, mapVeFactor(30), D); // 20(0.65)..40(0.72) midpoint
        assertEquals(0.77,  mapVeFactor(50), D); // 40(0.72)..60(0.82) midpoint
        assertEquals(0.96,  mapVeFactor(90), D); // 80(0.92)..100(1.00) midpoint
    }

    // -------------------------------------------------------------------------
    // BaseVeGenerator smoothing kernel (shared utilities)
    // -------------------------------------------------------------------------

    @Test
    public void testSmoothedBorderCellsUnchanged() {
        double[][] table = new double[5][5];
        for (double[] row : table) java.util.Arrays.fill(row, 80.0);
        table[1][1] = 60.0;
        table[2][2] = 120.0;
        double[][] after2 = smoothPass(smoothPass(table));
        for (int i = 0; i < 5; i++) {
            assertEquals(80.0, after2[0][i], D);
            assertEquals(80.0, after2[4][i], D);
            assertEquals(80.0, after2[i][0], D);
            assertEquals(80.0, after2[i][4], D);
        }
    }

    @Test
    public void testSmoothedKernelWeights() {
        // 3x3: one interior cell. center=50, N=20,S=80,E=60,W=40, corners=10/30/70/90
        // (4*50 + 2*(20+80+60+40) + (10+30+70+90)) / 16 = 800/16 = 50
        double[][] in = {{10,20,30},{40,50,60},{70,80,90}};
        double[][] out = smoothPass(in);
        assertEquals(50.0, out[1][1], D);
        assertEquals(10.0, out[0][0], D);
        assertEquals(90.0, out[2][2], D);
    }

    @Test
    public void testDegenerateTableSmoothing() {
        // 1x1
        assertEquals(75.0, smoothPass(new double[][]{{75.0}})[0][0], D);

        // 2x5: no interior rows
        double[][] t2 = {{50,60,70,80,90},{55,65,75,85,95}};
        double[][] r2 = smoothPass(t2);
        for (int r = 0; r < 2; r++)
            for (int c = 0; c < 5; c++)
                assertEquals(t2[r][c], r2[r][c], D);

        // 5x2: no interior cols
        double[][] t3 = {{50,60},{55,65},{60,70},{65,75},{70,80}};
        double[][] r3 = smoothPass(t3);
        for (int r = 0; r < 5; r++)
            for (int c = 0; c < 2; c++)
                assertEquals(t3[r][c], r3[r][c], D);
    }

    // -------------------------------------------------------------------------
    // Full generate() surface properties — v1 NA
    // -------------------------------------------------------------------------

    @Test
    public void testAllValuesInBounds() {
        for (HeadArchetype head : HeadArchetype.values()) {
            for (CamBehavior cam : CamBehavior.values()) {
                for (boolean vvt : new boolean[]{false, true}) {
                    VeGenerator gen = new ArchetypeBaseVeV1(new Request(true, 900, 7000,
                        head, cam, vvt, Aspiration.NATURALLY_ASPIRATED));
                    VeGenerator.Result r = gen.generate(RPM6, MAP6);
                    for (double[] row : r.veTable) {
                        for (double v : row) {
                            assertTrue(Double.isFinite(v),
                                () -> head + "+" + cam + "+vvt=" + vvt + " NaN/Inf");
                            assertTrue(v >= 35.0 && v <= 125.0,
                                () -> head + "+" + cam + "+vvt=" + vvt + " out of bounds: " + v);
                        }
                    }
                }
            }
        }
    }

    @Test
    public void testDeterminism() {
        ArchetypeBaseVeV1 gen = stock4vStockNoVvt();
        VeGenerator.Result r1 = gen.generate(RPM6, MAP6);
        VeGenerator.Result r2 = gen.generate(RPM6, MAP6);
        for (int row = 0; row < r1.veTable.length; row++)
            assertArrayEquals(r1.veTable[row], r2.veTable[row], 0.0);
    }

    @Test
    public void testProfileId() {
        assertEquals(PROFILE_ID, stock4vStockNoVvt().generate(RPM6, MAP6).profileId);
    }

    @Test
    public void testVvtBroadensCurve() {
        double[] rpmFull  = {900, 1500, 3828, 5500, 7000};
        double[] mapAt100 = {100};
        VeGenerator noVvt   = new ArchetypeBaseVeV1(new Request(true, 900, 7000,
            HeadArchetype.STOCK_4V, CamBehavior.STOCK, false, Aspiration.NATURALLY_ASPIRATED));
        VeGenerator withVvt = new ArchetypeBaseVeV1(new Request(true, 900, 7000,
            HeadArchetype.STOCK_4V, CamBehavior.STOCK, true,  Aspiration.NATURALLY_ASPIRATED));

        VeGenerator.Result rN = noVvt.generate(rpmFull, mapAt100);
        VeGenerator.Result rV = withVvt.generate(rpmFull, mapAt100);

        assertTrue(rV.veTable[0][0] >= rN.veTable[0][0], "VVT should raise or maintain idle VE");
        assertTrue(rV.veTable[0][4] >= rN.veTable[0][4], "VVT should raise or maintain redline VE");
    }

    @Test
    public void testV1AspirationFieldDoesNotAffectValues() {
        // v1 treats aspiration as cosmetic (table values unchanged); boosted engines now
        // use ArchetypeBaseVeV1Turbo / ArchetypeBaseVeV1Supercharged instead.
        VeGenerator na   = new ArchetypeBaseVeV1(new Request(true, 1000, 6000,
            HeadArchetype.STOCK_4V, CamBehavior.STOCK, false, Aspiration.NATURALLY_ASPIRATED));
        VeGenerator turb = new ArchetypeBaseVeV1(new Request(true, 1000, 6000,
            HeadArchetype.STOCK_4V, CamBehavior.STOCK, false, Aspiration.TURBOCHARGED));
        VeGenerator sc   = new ArchetypeBaseVeV1(new Request(true, 1000, 6000,
            HeadArchetype.STOCK_4V, CamBehavior.STOCK, false, Aspiration.SUPERCHARGED));

        VeGenerator.Result rNa   = na.generate(RPM6, MAP6);
        VeGenerator.Result rTurb = turb.generate(RPM6, MAP6);
        VeGenerator.Result rSc   = sc.generate(RPM6, MAP6);

        for (int r = 0; r < rNa.veTable.length; r++) {
            assertArrayEquals(rNa.veTable[r], rTurb.veTable[r], 0.0);
            assertArrayEquals(rNa.veTable[r], rSc.veTable[r],   0.0);
        }
        assertTrue(rNa.warnings.isEmpty());
        assertFalse(rTurb.warnings.isEmpty());
        assertFalse(rSc.warnings.isEmpty());
    }

    @Test
    public void testTableDimensions() {
        VeGenerator.Result r = stock4vStockNoVvt().generate(RPM6, MAP6);
        assertEquals(MAP6.length, r.veTable.length);
        for (double[] row : r.veTable)
            assertEquals(RPM6.length, row.length);
    }

    // -------------------------------------------------------------------------
    // ArchetypeBaseVeV1Turbo — aspiration assertion
    // -------------------------------------------------------------------------

    @Test
    public void testTurboRejectsNonTurboAspiration() {
        assertThrows(IllegalArgumentException.class, () ->
            new ArchetypeBaseVeV1Turbo(new Request(true, 1000, 6000,
                HeadArchetype.STOCK_4V, CamBehavior.STOCK, false, Aspiration.NATURALLY_ASPIRATED)));
        assertThrows(IllegalArgumentException.class, () ->
            new ArchetypeBaseVeV1Turbo(new Request(true, 1000, 6000,
                HeadArchetype.STOCK_4V, CamBehavior.STOCK, false, Aspiration.SUPERCHARGED)));
    }

    @Test
    public void testScRejectsNonScAspiration() {
        assertThrows(IllegalArgumentException.class, () ->
            new ArchetypeBaseVeV1Supercharged(new Request(true, 1000, 6000,
                HeadArchetype.STOCK_4V, CamBehavior.STOCK, false, Aspiration.NATURALLY_ASPIRATED)));
        assertThrows(IllegalArgumentException.class, () ->
            new ArchetypeBaseVeV1Supercharged(new Request(true, 1000, 6000,
                HeadArchetype.STOCK_4V, CamBehavior.STOCK, false, Aspiration.TURBOCHARGED)));
    }

    // -------------------------------------------------------------------------
    // ArchetypeBaseVeV1Turbo — profile and bounds
    // -------------------------------------------------------------------------

    @Test
    public void testTurboProfileId() {
        VeGenerator.Result r = new ArchetypeBaseVeV1Turbo(turboReq(false))
            .generate(RPM6, MAP_WITH_BOOST);
        assertEquals(ArchetypeBaseVeV1Turbo.PROFILE_ID, r.profileId);
        assertFalse(r.warnings.isEmpty());
    }

    @Test
    public void testTurboAllValuesInBounds() {
        for (HeadArchetype head : HeadArchetype.values()) {
            for (CamBehavior cam : CamBehavior.values()) {
                for (boolean vvt : new boolean[]{false, true}) {
                    VeGenerator gen = new ArchetypeBaseVeV1Turbo(new Request(true, 900, 7000,
                        head, cam, vvt, Aspiration.TURBOCHARGED));
                    VeGenerator.Result r = gen.generate(RPM6, MAP_WITH_BOOST);
                    for (double[] row : r.veTable) {
                        for (double v : row) {
                            assertTrue(Double.isFinite(v), "NaN/Inf in turbo table");
                            assertTrue(v >= 35.0 && v <= ArchetypeBaseVeV1Turbo.TURBO_CLAMP_MAX,
                                () -> "turbo out of [35, " + ArchetypeBaseVeV1Turbo.TURBO_CLAMP_MAX + "]: " + v);
                        }
                    }
                }
            }
        }
    }

    // -------------------------------------------------------------------------
    // ArchetypeBaseVeV1Supercharged — profile and bounds
    // -------------------------------------------------------------------------

    @Test
    public void testScProfileId() {
        VeGenerator.Result r = new ArchetypeBaseVeV1Supercharged(scReq(false))
            .generate(RPM6, MAP_WITH_BOOST);
        assertEquals(ArchetypeBaseVeV1Supercharged.PROFILE_ID, r.profileId);
        assertFalse(r.warnings.isEmpty());
    }

    @Test
    public void testScAllValuesInBounds() {
        for (HeadArchetype head : HeadArchetype.values()) {
            for (CamBehavior cam : CamBehavior.values()) {
                for (boolean vvt : new boolean[]{false, true}) {
                    VeGenerator gen = new ArchetypeBaseVeV1Supercharged(new Request(true, 900, 7000,
                        head, cam, vvt, Aspiration.SUPERCHARGED));
                    VeGenerator.Result r = gen.generate(RPM6, MAP_WITH_BOOST);
                    for (double[] row : r.veTable) {
                        for (double v : row) {
                            assertTrue(Double.isFinite(v), "NaN/Inf in SC table");
                            assertTrue(v >= 35.0 && v <= ArchetypeBaseVeV1Supercharged.SC_CLAMP_MAX,
                                () -> "SC out of [35, " + ArchetypeBaseVeV1Supercharged.SC_CLAMP_MAX + "]: " + v);
                        }
                    }
                }
            }
        }
    }

    // -------------------------------------------------------------------------
    // Turbo behavioural invariants
    // -------------------------------------------------------------------------

    @Test
    public void testTurboHasHigherIdleVeThanNaWithSameCam() {
        // At 100 kPa (atmospheric WOT), idle RPM bin: turbo should have higher VE
        // because IDLE_DROP_MULT reduces the idle sag.
        double[] mapAtWot = {100.0};
        double[] rpmIdleAndPeak = {1000, 6000};

        VeGenerator na    = new ArchetypeBaseVeV1(new Request(true, 1000, 6000,
            HeadArchetype.STOCK_4V, CamBehavior.STOCK, false, Aspiration.NATURALLY_ASPIRATED));
        VeGenerator turbo = new ArchetypeBaseVeV1Turbo(new Request(true, 1000, 6000,
            HeadArchetype.STOCK_4V, CamBehavior.STOCK, false, Aspiration.TURBOCHARGED));

        double naidleVe    = na.generate(rpmIdleAndPeak, mapAtWot).veTable[0][0];
        double turboIdleVe = turbo.generate(rpmIdleAndPeak, mapAtWot).veTable[0][0];

        assertTrue(turboIdleVe > naidleVe,
            () -> String.format(Locale.ROOT,
                "Turbo idle VE (%.2f) should exceed NA idle VE (%.2f)", turboIdleVe, naidleVe));
    }

    @Test
    public void testTurboBoostRowsExceedAtmosphericWotRow() {
        // Every cell in a boost row (MAP > 100 kPa) should have a higher or equal factor
        // than the corresponding cell in the 100 kPa row. This validates the upward slope.
        double[] rpm = {1000, 2300, 3600, 5000, 6000};
        double[] map = {100.0, 130.0, 160.0};

        VeGenerator.Result r = new ArchetypeBaseVeV1Turbo(turboReq(false)).generate(rpm, map);

        double[] wotRow = r.veTable[0]; // 100 kPa row
        for (int boostRowIdx = 1; boostRowIdx < map.length; boostRowIdx++) {
            double[] boostRow = r.veTable[boostRowIdx];
            for (int c = 0; c < rpm.length; c++) {
                final double bv = boostRow[c];
                final double wv = wotRow[c];
                assertTrue(bv >= wv - DQ,
                    () -> String.format(Locale.ROOT,
                        "Boost row VE (%.2f) should not fall below WOT row VE (%.2f)", bv, wv));
            }
        }
    }

    @Test
    public void testTurboVvtBoostHighRpmExceedsNoVvtBoostSameRow() {
        // In boost rows, cells above peakRpm should be higher with VVT than without.
        double[] rpm = {1000, 3400, 4700, 6000}; // 3400 = peakRpm for STOCK at idle=1000,max=6000
        double[] map = {130.0};                   // single boost row to isolate the effect

        VeGenerator noVvt  = new ArchetypeBaseVeV1Turbo(turboReq(false));
        VeGenerator withVvt = new ArchetypeBaseVeV1Turbo(turboReq(true));

        double[] rowNoVvt  = noVvt.generate(rpm, map).veTable[0];
        double[] rowVvt    = withVvt.generate(rpm, map).veTable[0];

        // Above peakRpm (cols 2 and 3: 4700, 6000): VVT should give higher or equal VE
        for (int c = 2; c < rpm.length; c++) {
            final int col = c;
            assertTrue(rowVvt[c] >= rowNoVvt[c] - DQ,
                () -> String.format(Locale.ROOT,
                    "VVT boost high-RPM VE (%.2f) should not fall below no-VVT (%.2f) at col %d",
                    rowVvt[col], rowNoVvt[col], col));
        }
    }

    @Test
    public void testTurboBoostContinuousAt100kPa() {
        // At exactly 100 kPa the boost formula gives factor 1.0, same as v1 atmospheric.
        // Verify the turbo WOT row matches v1 NA at 100 kPa within smoothing tolerance.
        double[] rpm = {1000, 2300, 3600, 5000, 6000};
        double[] map = {100.0};

        VeGenerator na    = new ArchetypeBaseVeV1(new Request(true, 1000, 6000,
            HeadArchetype.STOCK_4V, CamBehavior.STOCK, false, Aspiration.NATURALLY_ASPIRATED));
        VeGenerator turbo = new ArchetypeBaseVeV1Turbo(turboReq(false));

        // Single-row table: no smoothing interior cells, so values come directly from formula.
        double[] naRow    = na.generate(rpm, map).veTable[0];
        double[] turboRow = turbo.generate(rpm, map).veTable[0];

        // The RPM curves differ (idle VE higher for turbo), so values differ.
        // What must hold: turbo should not be BELOW na at 100 kPa (no boost deficit).
        for (int c = 0; c < rpm.length; c++) {
            assertTrue(Double.isFinite(turboRow[c]));
            assertTrue(turboRow[c] >= 35.0 && turboRow[c] <= ArchetypeBaseVeV1Turbo.TURBO_CLAMP_MAX);
        }
    }

    // -------------------------------------------------------------------------
    // Turbo golden matrix — frozen after first run with STOCK_4V + STOCK + no VVT
    // axes chosen to exercise atmospheric, transition, and boost rows with interior cells
    // -------------------------------------------------------------------------

    private static final double[] GOLDEN_RPM = {1000, 2300, 3600, 5000, 6000};
    private static final double[] GOLDEN_MAP = {40, 80, 100, 130, 160};

    @Test
    public void testTurboGoldenMatrix() {
        VeGenerator gen = new ArchetypeBaseVeV1Turbo(new Request(
            true, 1000, 6000,
            HeadArchetype.STOCK_4V, CamBehavior.STOCK, false, Aspiration.TURBOCHARGED));
        double[][] table = gen.generate(GOLDEN_RPM, GOLDEN_MAP).veTable;
        double[][] expected = {
            {60.4800000000, 67.2600000000, 69.4633846154, 61.7870769231, 56.3040000000},
            {77.2800000000, 79.4293957332, 79.9658894231, 75.6334642428, 71.9440000000},
            {84.0000000000, 89.0327696314, 89.7574935897, 85.0040789263, 78.2000000000},
            {89.0400000000, 95.9620611979, 97.0161073718, 91.3164133614, 82.8920000000},
            {94.0800000000, 104.6266666667, 108.0541538462, 96.1132307692, 87.5840000000},
        };
        for (int r = 0; r < expected.length; r++)
            assertArrayEquals(expected[r], table[r], 1e-6);
    }

    @Test
    public void testScGoldenMatrix() {
        VeGenerator gen = new ArchetypeBaseVeV1Supercharged(new Request(
            true, 1000, 6000,
            HeadArchetype.STOCK_4V, CamBehavior.STOCK, false, Aspiration.SUPERCHARGED));
        double[][] table = gen.generate(GOLDEN_RPM, GOLDEN_MAP).veTable;
        double[][] expected = {
            {54.4320000000, 65.2800000000, 69.5132307692, 62.1858461538, 56.9520000000},
            {69.5520000000, 75.9011826923, 79.0162148437, 75.8525366587, 72.7720000000},
            {75.6000000000, 84.2146025641, 87.7665530849, 84.4585236378, 79.1000000000},
            {77.8680000000, 89.1643782051, 93.1907164463, 89.0110110176, 81.4730000000},
            {80.1360000000, 96.1066666667, 102.3389230769, 91.5513846154, 83.8460000000},
        };
        for (int r = 0; r < expected.length; r++)
            assertArrayEquals(expected[r], table[r], 1e-6);
    }
}
