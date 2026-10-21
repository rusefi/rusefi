package com.rusefi.ui.logview;

import org.junit.jupiter.api.Test;

import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertFalse;
import static org.junit.jupiter.api.Assertions.assertTrue;

class TraceColorsTest {
    @Test
    void slotsFollowPlottingOrder() {
        TraceColors colors = new TraceColors(6);
        colors.plot(1);
        colors.plot(3);
        colors.plot(4);
        assertEquals(0, colors.slot(1));
        assertEquals(1, colors.slot(3));
        assertEquals(2, colors.slot(4));
        assertEquals(-1, colors.slot(2));
        assertEquals(3, colors.plottedCount());
    }

    @Test
    void removingOneTraceKeepsTheOthersColours() {
        TraceColors colors = new TraceColors(10);
        colors.plot(5);
        colors.plot(2);
        colors.plot(7);
        colors.remove(2);
        assertEquals(0, colors.slot(5));
        assertEquals(2, colors.slot(7));
        // the freed slot is reused before a new one
        colors.plot(9);
        assertEquals(1, colors.slot(9));
    }

    @Test
    void moreTracesThanColoursReuseLeastUsedSlot() {
        TraceColors colors = new TraceColors(20);
        for (int field = 1; field <= PlotPalette.TRACE_COUNT; field++) {
            colors.plot(field);
        }
        colors.plot(10);
        assertEquals(0, colors.slot(10));
        colors.plot(11);
        assertEquals(1, colors.slot(11));
    }

    @Test
    void toggleAndRepeatedPlot() {
        TraceColors colors = new TraceColors(4);
        colors.toggle(2);
        assertTrue(colors.isPlotted(2));
        colors.plot(2);
        assertEquals(1, colors.plottedCount());
        colors.toggle(2);
        assertFalse(colors.isPlotted(2));
    }
}
