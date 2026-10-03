package com.rusefi.ui.widgets.tune;

import org.junit.jupiter.api.Test;

import static org.junit.jupiter.api.Assertions.*;

class Surface3DViewTest {
    @Test
    void unevenBinsKeepTheirRelativeSpacing() {
        assertArrayEquals(new double[]{-0.5, -0.4, 0.5},
            Surface3DView.axisPositions(new Double[]{1000.0, 1500.0, 6000.0}, 3), 1e-9);
        assertArrayEquals(new double[]{-0.5, 0.3, 0.5},
            Surface3DView.axisPositions(new Double[]{100.0, 20.0, 0.0}, 3), 1e-9);
    }

    @Test
    void unusableAxesFallBackToEvenSpacing() {
        for (Double[] bins : new Double[][]{null, {1.0}, {1.0, 1.0, 1.0},
            {1.0, 3.0, 2.0}, {1.0, null, 3.0}, {1.0, Double.NaN, 3.0}}) {
            assertFalse(Surface3DView.hasUsableAxis(bins, 3));
            assertArrayEquals(new double[]{-0.5, 0, 0.5}, Surface3DView.axisPositions(bins, 3), 1e-9);
        }
        assertArrayEquals(new double[]{0}, Surface3DView.axisPositions(new Double[]{1.0}, 1));
    }
}
