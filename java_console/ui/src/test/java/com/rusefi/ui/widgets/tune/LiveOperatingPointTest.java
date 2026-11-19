package com.rusefi.ui.widgets.tune;

import org.junit.jupiter.api.Test;

import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertNull;

public class LiveOperatingPointTest {
    private static final Double[] RPM = {1000.0, 2000.0, 4000.0};

    @Test
    public void valueOnABinIsThatBin() {
        assertEquals(0.0, LiveOperatingPoint.axisPosition(RPM, 3, 1000), 1e-9);
        assertEquals(1.0, LiveOperatingPoint.axisPosition(RPM, 3, 2000), 1e-9);
        assertEquals(2.0, LiveOperatingPoint.axisPosition(RPM, 3, 4000), 1e-9);
    }

    @Test
    public void valueBetweenBinsIsFractional() {
        assertEquals(0.25, LiveOperatingPoint.axisPosition(RPM, 3, 1250), 1e-9);
        assertEquals(1.5, LiveOperatingPoint.axisPosition(RPM, 3, 3000), 1e-9);
    }

    @Test
    public void valueOutsideTheAxisClampsToTheNearerEnd() {
        assertEquals(0.0, LiveOperatingPoint.axisPosition(RPM, 3, 0), 1e-9);
        assertEquals(2.0, LiveOperatingPoint.axisPosition(RPM, 3, 9000), 1e-9);
    }

    @Test
    public void invertedAxisCountsFromItsFirstDisplayedBin() {
        Double[] descending = {4000.0, 2000.0, 1000.0};
        assertEquals(0.5, LiveOperatingPoint.axisPosition(descending, 3, 3000), 1e-9);
        assertEquals(2.0, LiveOperatingPoint.axisPosition(descending, 3, 500), 1e-9);
        assertEquals(0.0, LiveOperatingPoint.axisPosition(descending, 3, 7000), 1e-9);
    }

    @Test
    public void binsBeyondTheDataAreIgnored() {
        assertEquals(1.0, LiveOperatingPoint.axisPosition(RPM, 2, 3000), 1e-9);
    }

    @Test
    public void repeatedBinsDoNotDivideByZero() {
        Double[] flat = {0.0, 0.0, 0.0};
        assertEquals(0.0, LiveOperatingPoint.axisPosition(flat, 3, 0), 1e-9);
    }

    @Test
    public void noValueOrNoAxisGivesNoPosition() {
        assertNull(LiveOperatingPoint.axisPosition(RPM, 3, Double.NaN));
        assertNull(LiveOperatingPoint.axisPosition(RPM, 3, Double.POSITIVE_INFINITY));
        assertNull(LiveOperatingPoint.axisPosition(null, 3, 1000));
        assertNull(LiveOperatingPoint.axisPosition(new Double[0], 3, 1000));
        assertNull(LiveOperatingPoint.axisPosition(new Double[]{1000.0, null}, 2, 1000));
    }
}
