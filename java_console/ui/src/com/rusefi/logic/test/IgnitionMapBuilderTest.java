package com.rusefi.logic.test;

import com.rusefi.logic.IgnitionMapBuilder;
import org.junit.Test;

import static com.rusefi.logic.IgnitionMapBuilder.ChamberStyle.*;
import static com.rusefi.logic.IgnitionMapBuilder.getBoreCorrection;
import static com.rusefi.logic.IgnitionMapBuilder.getTopAdvanceForBore;
import static com.rusefi.logic.IgnitionMapBuilder.round10;
import static junit.framework.Assert.assertEquals;

/**
 * (c) Andrey Belomutskiy 2012-2015
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

        assertEquals(22.0, getTopAdvanceForBore(SWITL_TUMBLE, 89, 9, 101.6));
        assertEquals(32.2, getTopAdvanceForBore(SWITL_TUMBLE, 89, 9, 145));
    }
}
