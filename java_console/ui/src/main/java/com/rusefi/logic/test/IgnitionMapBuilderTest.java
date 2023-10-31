package com.rusefi.logic.test;

import org.junit.Test;

import static com.rusefi.logic.IgnitionMapBuilder.*;
import static com.rusefi.logic.IgnitionMapBuilder.ChamberStyle.*;
import static junit.framework.Assert.assertEquals;

/**
 * Andrey Belomutskiy, (c) 2013-2020
 * 12/24/2015
 */
public class IgnitionMapBuilderTest {
    private static final double EPS = 0.001;

    @Test
    public void testIgnitionMapBuilder() {
        assertEquals(1.1, round10(1.1));
        assertEquals(1.1, round10(1.123));

        assertEquals(0.0, getBoreCorrection(4 * 25.4));
        assertEquals(6.0, getBoreCorrection(5 * 25.4), EPS);

        assertEquals(35.0, getTopAdvanceForBore(OPEN_CHAMBER, 98, 8, 101.6));
        assertEquals(33.0, getTopAdvanceForBore(OPEN_CHAMBER, 98, 11, 101.6));

        assertEquals(22.0, getTopAdvanceForBore(SWIRL_TUMBLE, 89, 9, 101.6));
        assertEquals(32.2, getTopAdvanceForBore(SWIRL_TUMBLE, 89, 9, 145));

        assertEquals(10.0, interpolate(0, 10, 10, 20, 0));
        assertEquals(20.0, interpolate(0, 10, 10, 20, 10));

        assertEquals(10.0, getAdvanceForRpm(0, 36));
        assertEquals(10.0, getAdvanceForRpm(600, 36));
        assertEquals(36.0, getAdvanceForRpm(6500, 36));

        assertEquals(16.5, getAdvanceForRpm(1200, 36));
        assertEquals(29.5, getAdvanceForRpm(2400, 36));

        assertEquals(36.0, getInitialAdvance(6000, 100, 36));
        assertEquals(10.0, getInitialAdvance(600, 100, 36));

        assertEquals(44.0, getInitialAdvance(6000, 20, 36));

        assertEquals(34.3, getInitialAdvance(2400, 40, 36));
        assertEquals(42.0, getInitialAdvance(4400, 40, 36));
        assertEquals(11.6, getInitialAdvance(600, 20, 36));
    }
}
