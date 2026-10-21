package com.rusefi.ui.logview;

import com.rusefi.logview.CsvLogReader;
import org.junit.jupiter.api.Test;

import java.io.IOException;
import java.io.StringReader;

import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertNotSame;
import static org.junit.jupiter.api.Assertions.assertSame;
import static org.junit.jupiter.api.Assertions.assertTrue;

class TraceStatsTest {
    /** Time 0..9 s; RPM rises and falls with a flat top; MAP has a gap at 4 and 5 s. */
    private static CsvLogReader log() throws IOException {
        String[] rpm = {"900", "1500", "3000", "3000", "3000", "2000", "1200", "800", "800", "1000"};
        String[] map = {"30", "40", "90", "95", "", "", "60", "35", "32", "33"};
        StringBuilder text = new StringBuilder("Time\tRPM\tMAP\ns\trpm\tkPa\n");
        for (int i = 0; i < rpm.length; i++) {
            text.append(i).append('\t').append(rpm[i]).append('\t').append(map[i]).append('\n');
        }
        return CsvLogReader.read(new StringReader(text.toString()));
    }

    @Test
    void minMaxAverageOverTheRange() throws IOException {
        TraceStats stats = TraceStats.of(log(), 1, 0, 9);
        assertEquals(800, stats.min, 1e-9);
        assertEquals(3000, stats.max, 1e-9);
        assertEquals(17200 / 10.0, stats.average, 1e-9);
        assertEquals(10, stats.count);
    }

    @Test
    void extremesPointAtTheirFirstSample() throws IOException {
        TraceStats stats = TraceStats.of(log(), 1, 0, 9);
        assertEquals(2, stats.maxSample, "first of the flat 3000 top");
        assertEquals(7, stats.minSample, "first of the two 800s");
    }

    @Test
    void rangeLimitsTheSamplesBothEndsIncluded() throws IOException {
        TraceStats stats = TraceStats.of(log(), 1, 5, 7);
        assertEquals(3, stats.count);
        assertEquals(800, stats.min, 1e-9);
        assertEquals(2000, stats.max, 1e-9);
    }

    @Test
    void missingValuesAreSkipped() throws IOException {
        TraceStats stats = TraceStats.of(log(), 2, 3, 6);
        assertEquals(2, stats.count, "4 s and 5 s are empty");
        assertEquals(60, stats.min, 1e-9);
        assertEquals(95, stats.max, 1e-9);
        assertEquals(77.5, stats.average, 1e-9);
    }

    @Test
    void rangeWithoutValuesIsEmpty() throws IOException {
        assertTrue(TraceStats.of(log(), 2, 4, 5).isEmpty());
        assertTrue(TraceStats.of(log(), 1, 3.2, 3.8).isEmpty(), "between two samples");
        assertEquals(-1, TraceStats.EMPTY.minSample);
    }

    @Test
    void visibleStatsComputeOncePerWindow() throws IOException {
        CsvLogReader log = log();
        TimeWindow window = new TimeWindow(0, 9, 1);
        VisibleStats stats = new VisibleStats(log, window);
        TraceStats whole = stats.get(1);
        assertSame(whole, stats.get(1), "cached while the window stays");

        window.show(5, 7);
        TraceStats zoomed = stats.get(1);
        assertNotSame(whole, zoomed);
        assertEquals(2000, zoomed.max, 1e-9);
    }
}
