package com.rusefi.tune.ve;

import org.junit.jupiter.api.Test;

import static com.rusefi.tune.ve.BaseVeGenerator.*;
import static org.junit.jupiter.api.Assertions.*;

public class BaseVeGeneratorTest {

    private static final double D  = 1e-9; // exact-formula comparisons (no rounding between stages)
    private static final double DQ = 0.15; // tolerance after smoothing

    private static final double[] RPM6 = {1000, 2000, 3000, 4000, 5000, 6000};
    private static final double[] MAP6 = {20, 40, 60, 80, 100, 120};

    private static Request stock4vStockNoVvt() {
        return new Request(true, 1000, 6000,
            HeadArchetype.STOCK_4V, CamBehavior.STOCK, false, Aspiration.NATURALLY_ASPIRATED);
    }

    // -------------------------------------------------------------------------
    // Four-stroke SI gate
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
        // Midpoint between idle(1000,70) and lowRpm(2200,88) => 1600 RPM => 79.0
        assertEquals(79.0, rpmVe(1600, 1000,2200,3400,4700,6000, 70,88,98,89,80), D);
        // Midpoint between peakRpm(3400,98) and highRpm(4700,89) => 4050 => 93.5
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
        // 30 kPa: midpoint 20(0.65)..40(0.72) => 0.685
        assertEquals(0.685, mapVeFactor(30), D);
        // 50 kPa: midpoint 40(0.72)..60(0.82) => 0.77
        assertEquals(0.77,  mapVeFactor(50), D);
        // 90 kPa: midpoint 80(0.92)..100(1.00) => 0.96
        assertEquals(0.96,  mapVeFactor(90), D);
    }

    // -------------------------------------------------------------------------
    // Smoothing kernel
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
        double[][] in = {
            {10, 20, 30},
            {40, 50, 60},
            {70, 80, 90}
        };
        // center=50, N=20,S=80,E=60,W=40, NW=10,NE=30,SW=70,SE=90
        // (4*50 + 2*(20+80+60+40) + (10+30+70+90)) / 16 = (200+400+200)/16 = 800/16 = 50
        double[][] out = smoothPass(in);
        assertEquals(50.0, out[1][1], D);
        assertEquals(10.0, out[0][0], D);
        assertEquals(90.0, out[2][2], D);
    }

    @Test
    public void testDegenerateTableSmoothing() {
        // 1x1: copy
        double[][] t1 = {{75.0}};
        assertEquals(75.0, smoothPass(t1)[0][0], D);

        // 2x5: no interior rows
        double[][] t2 = {{50,60,70,80,90},{55,65,75,85,95}};
        double[][] r2 = smoothPass(t2);
        for (int r = 0; r < 2; r++) {
            for (int c = 0; c < 5; c++) {
                assertEquals(t2[r][c], r2[r][c], D);
            }
        }

        // 5x2: no interior cols
        double[][] t3 = {{50,60},{55,65},{60,70},{65,75},{70,80}};
        double[][] r3 = smoothPass(t3);
        for (int r = 0; r < 5; r++) {
            for (int c = 0; c < 2; c++) {
                assertEquals(t3[r][c], r3[r][c], D);
            }
        }
    }

    // -------------------------------------------------------------------------
    // Full generate() surface properties
    // -------------------------------------------------------------------------

    @Test
    public void testAllValuesInBounds() {
        for (HeadArchetype head : HeadArchetype.values()) {
            for (CamBehavior cam : CamBehavior.values()) {
                for (boolean vvt : new boolean[]{false, true}) {
                    Request req = new Request(true, 900, 7000, head, cam, vvt,
                        Aspiration.NATURALLY_ASPIRATED);
                    Result r = generate(req, RPM6, MAP6);
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
        Request req = stock4vStockNoVvt();
        Result r1 = generate(req, RPM6, MAP6);
        Result r2 = generate(req, RPM6, MAP6);
        assertEquals(r1.veTable.length, r2.veTable.length);
        for (int row = 0; row < r1.veTable.length; row++) {
            assertArrayEquals(r1.veTable[row], r2.veTable[row], 0.0);
        }
    }

    @Test
    public void testProfileId() {
        assertEquals(PROFILE_ID, generate(stock4vStockNoVvt(), RPM6, MAP6).profileId);
    }

    @Test
    public void testVvtBroadensCurveAtLowAndHighRpm() {
        double[] rpmFull  = {900, 1500, 3828, 5500, 7000};
        double[] mapAt100 = {100};
        Request noVvt   = new Request(true, 900, 7000, HeadArchetype.STOCK_4V,
            CamBehavior.STOCK, false, Aspiration.NATURALLY_ASPIRATED);
        Request withVvt = new Request(true, 900, 7000, HeadArchetype.STOCK_4V,
            CamBehavior.STOCK, true,  Aspiration.NATURALLY_ASPIRATED);

        Result rN = generate(noVvt,   rpmFull, mapAt100);
        Result rV = generate(withVvt, rpmFull, mapAt100);

        // VVT reduces idle and redline drops: idle (col 0) and redline (col 4) must be >= no-VVT
        assertTrue(rV.veTable[0][0] >= rN.veTable[0][0], "VVT should raise or maintain idle VE");
        assertTrue(rV.veTable[0][4] >= rN.veTable[0][4], "VVT should raise or maintain redline VE");
    }

    @Test
    public void testAspirationDoesNotAffectValues() {
        Request naReq   = new Request(true, 1000, 6000, HeadArchetype.STOCK_4V,
            CamBehavior.STOCK, false, Aspiration.NATURALLY_ASPIRATED);
        Request turbReq = new Request(true, 1000, 6000, HeadArchetype.STOCK_4V,
            CamBehavior.STOCK, false, Aspiration.TURBOCHARGED);
        Request scReq   = new Request(true, 1000, 6000, HeadArchetype.STOCK_4V,
            CamBehavior.STOCK, false, Aspiration.SUPERCHARGED);

        Result rNa   = generate(naReq,   RPM6, MAP6);
        Result rTurb = generate(turbReq, RPM6, MAP6);
        Result rSc   = generate(scReq,   RPM6, MAP6);

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
        for (HeadArchetype head : HeadArchetype.values()) {
            Result r = generate(new Request(true, 900, 7000, head,
                CamBehavior.STOCK, false, Aspiration.NATURALLY_ASPIRATED), RPM6, MAP6);
            assertEquals(MAP6.length, r.veTable.length);
            for (double[] row : r.veTable) {
                assertEquals(RPM6.length, row.length);
            }
        }
    }
}
