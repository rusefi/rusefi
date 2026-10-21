package com.rusefi.logview;

import org.junit.jupiter.api.Test;

import java.io.IOException;
import java.util.ArrayList;
import java.util.List;

import static org.junit.jupiter.api.Assertions.assertEquals;

class CsvLogReaderTimeLookupTest {
    private static CsvLogReader log(double... times) throws IOException {
        List<double[]> rows = new ArrayList<>();
        for (double time : times) {
            rows.add(new double[]{time, time * 10});
        }
        return CsvLogReader.fromRows(new String[]{"Time", "RPM"}, new String[]{"s", ""}, rows);
    }

    @Test
    void firstSampleAtOrAfter() throws IOException {
        CsvLogReader log = log(0, 1, 2, 2, 5);
        assertEquals(0, log.firstSampleAtOrAfter(-1));
        assertEquals(0, log.firstSampleAtOrAfter(0));
        assertEquals(1, log.firstSampleAtOrAfter(0.5));
        assertEquals(2, log.firstSampleAtOrAfter(2));
        assertEquals(4, log.firstSampleAtOrAfter(3));
        assertEquals(5, log.firstSampleAtOrAfter(6));
    }

    @Test
    void nearestSample() throws IOException {
        CsvLogReader log = log(0, 1, 2, 5);
        assertEquals(0, log.nearestSample(-3));
        assertEquals(1, log.nearestSample(1.4));
        assertEquals(2, log.nearestSample(1.6));
        assertEquals(2, log.nearestSample(3.5));
        assertEquals(3, log.nearestSample(3.6));
        assertEquals(3, log.nearestSample(99));
    }

    @Test
    void minimumSampleIntervalIgnoresDuplicateTimes() throws IOException {
        assertEquals(0.25, log(0, 1, 1, 1.25, 3).minimumSampleInterval(), 1e-12);
        assertEquals(0, log(4, 4).minimumSampleInterval());
    }
}
