package com.rusefi.logview;

import org.junit.jupiter.api.Test;

import java.io.IOException;
import java.io.StringReader;
import java.io.StringWriter;

import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertThrows;
import static org.junit.jupiter.api.Assertions.assertTrue;

class MslWriterTest {
    private static CsvLogReader log() throws IOException {
        return CsvLogReader.read(new StringReader("\"original\"\nTime\tRPM\tAFR\tGear\nms\trpm\tAFR\t\n"
                + "0\t800\t14.7\t1\n100\t1200\t13.25\t\n200\t2500\t12.9\t2\n300\t3000\t12.5\t3\n"));
    }

    private static String write(CsvLogReader log, int from, int to) throws IOException {
        StringWriter out = new StringWriter();
        MslWriter.write(log, from, to, "export \"test\"", out);
        return out.toString();
    }

    @Test
    void writesTheSamplesInRangeWithTimeInSeconds() throws IOException {
        assertEquals("\"export 'test'\"\n"
                + "Time\tRPM\tAFR\tGear\n"
                + "s\trpm\tAFR\t\n"
                + "0.1\t1200\t13.25\t\n"
                + "0.2\t2500\t12.9\t2\n", write(log(), 1, 2));
    }

    @Test
    void writtenLogReadsBackTheSame() throws IOException {
        CsvLogReader original = log();
        CsvLogReader copy = CsvLogReader.read(new StringReader(write(original, 0, 3)));
        assertEquals(original.sampleCount(), copy.sampleCount());
        for (int sample = 0; sample < original.sampleCount(); sample++) {
            for (int field = 0; field < original.fieldCount(); field++) {
                assertEquals(original.value(sample, field), copy.value(sample, field), 1e-9,
                        original.name(field) + " at " + sample);
            }
        }
    }

    @Test
    void valuesUseTheLogsPrecisionWithoutScalingNoise() {
        assertEquals("0.3", MslWriter.format(0.1 * 3, false));
        assertEquals("0.00001", MslWriter.format(1e-5, false));
        assertEquals("-42", MslWriter.format(-42, false));
        assertEquals("", MslWriter.format(Double.NaN, false));
        assertEquals("1234.56789", MslWriter.format(1234.56789, true), "time keeps its digits");
    }

    @Test
    void sliceKeepsTheSamplesAndRecomputesRanges() throws IOException {
        CsvLogReader slice = log().slice(2, 3);
        assertEquals(2, slice.sampleCount());
        assertEquals(0.2, slice.value(0, 0), 1e-12);
        assertEquals(2500, slice.minimum(1));
        assertTrue(slice.maximum(1) == 3000);
        assertThrows(IllegalArgumentException.class, () -> log().slice(3, 2));
    }
}
