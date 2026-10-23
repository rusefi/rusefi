package com.rusefi.ui.logview;

import com.rusefi.logview.CsvLogReader;
import org.junit.jupiter.api.Test;

import java.io.IOException;
import java.io.StringReader;
import java.util.Arrays;

import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertFalse;
import static org.junit.jupiter.api.Assertions.assertNotNull;
import static org.junit.jupiter.api.Assertions.assertNull;
import static org.junit.jupiter.api.Assertions.assertTrue;

class HistogramFilterTest {
    /** Ten records 0.1 s apart: TPS steps from 10 to 60 at 0.5 s, lambda reads 0.68 at 0.2 and 0.3 s. */
    private static CsvLogReader log() throws IOException {
        StringBuilder text = new StringBuilder("Time\tTPS\tLambda 1\ns\t%\tlambda\n");
        for (int i = 0; i < 10; i++) {
            double lambda = i == 2 || i == 3 ? 0.68 : 1.0;
            text.append(i / 10.0).append('\t').append(i < 5 ? 10 : 60).append('\t').append(lambda).append('\n');
        }
        return CsvLogReader.read(new StringReader(text.toString()));
    }

    private static String marks(CsvLogReader log, HistogramFilter filter) {
        boolean[] hidden = new boolean[log.sampleCount()];
        filter.mark(log, hidden);
        StringBuilder text = new StringBuilder();
        for (boolean h : hidden) {
            text.append(h ? 'x' : '.');
        }
        return text.toString();
    }

    @Test
    void conditionsCompareTheSignal() throws IOException {
        CsvLogReader log = log();
        assertEquals("..xx......", marks(log, HistogramFilter.of("Lambda 1", HistogramFilter.Condition.AT_MOST, 0.68)));
        assertEquals(".....xxxxx", marks(log, HistogramFilter.of("TPS", HistogramFilter.Condition.AT_LEAST, 60)));
        assertEquals("..xx......", marks(log, new HistogramFilter("Lambda 1", HistogramFilter.Condition.BETWEEN,
                0.7, 0.6, 0, true)), "the ends may be given in either order");
        assertEquals("..xx......", marks(log, new HistogramFilter("Lambda 1", HistogramFilter.Condition.OUTSIDE,
                0.9, 1.1, 0, true)));
    }

    @Test
    void rateOfChangeAndHoldTimeCatchTransients() throws IOException {
        CsvLogReader log = log();
        // 50 % in 0.1 s is 500 %/s, at the record logged at 0.5 s
        HistogramFilter tip = HistogramFilter.of("TPS", HistogramFilter.Condition.CHANGING, 100);
        assertEquals(".....x....", marks(log, tip));
        HistogramFilter settle = new HistogramFilter("TPS", HistogramFilter.Condition.CHANGING, 100, Double.NaN,
                0.25, true);
        assertEquals(".....xxx..", marks(log, settle), "0.6 and 0.7 s are within 0.25 s of the change");
        assertEquals(3, settle.count(log));
        assertEquals("+ 250 ms after", settle.holdLabel());
    }

    @Test
    void enabledFiltersCombineAndMissingSignalsHideNothing() throws IOException {
        CsvLogReader log = log();
        HistogramFilter lambda = HistogramFilter.of("Lambda 1", HistogramFilter.Condition.AT_MOST, 0.68);
        HistogramFilter throttle = HistogramFilter.of("TPS", HistogramFilter.Condition.AT_LEAST, 60);
        HistogramFilter missing = HistogramFilter.of("Boost", HistogramFilter.Condition.AT_LEAST, 0);
        boolean[] hidden = HistogramFilter.hidden(log, Arrays.asList(lambda, throttle.withEnabled(false), missing));
        assertTrue(hidden[2]);
        assertFalse(hidden[6], "a disabled filter hides nothing");
        assertEquals(0, missing.count(log));
        assertEquals(-1, missing.field(log));
    }

    @Test
    void labelsAndSavedForm() {
        HistogramFilter filter = new HistogramFilter("TPS", HistogramFilter.Condition.CHANGING, 50, Double.NaN,
                0.3, false);
        assertEquals("TPS changing faster than 50 /s", filter.label());
        assertEquals("Front Lambda ≤ 0.68",
                HistogramFilter.of("Front Lambda", HistogramFilter.Condition.AT_MOST, 0.68).label());
        assertEquals("RPM between 800–1200", new HistogramFilter("RPM", HistogramFilter.Condition.BETWEEN,
                800, 1200, 0, true).label());
        assertEquals(filter, HistogramFilter.decode(filter.encode()));
        assertNull(HistogramFilter.decode("nonsense"));
        assertNull(HistogramFilter.decode("TPS\tNOPE\t1\t2\t0\ttrue"));
    }

    @Test
    void saturationIsAPileOfRecordsAtTheLowestOrHighestValue() throws IOException {
        // lambda spread over 0.900..1.099 in 0.001 steps, pegged at 0.68 in every 40th record; gear 1..4, as
        // common in 4th as in any other gear
        StringBuilder text = new StringBuilder("Time\tLambda 1\tRPM\tGear\ns\tlambda\trpm\t\n");
        for (int i = 0; i < 1000; i++) {
            double lambda = i % 40 == 0 ? 0.68 : 0.9 + (i * 37 % 200) / 1000.0;
            text.append(i / 100.0).append('\t').append(lambda).append('\t').append(800 + i)
                    .append('\t').append(1 + i % 4).append('\n');
        }
        CsvLogReader log = CsvLogReader.read(new StringReader(text.toString()));
        assertNull(HistogramSignals.saturation(log, 3), "a common end value is not a pile");
        HistogramSignals.Saturation saturation = HistogramSignals.saturation(log, 1);
        assertNotNull(saturation);
        assertTrue(saturation.low);
        assertEquals(0.68, saturation.value, 1e-9);
        assertEquals(25, saturation.records);
        assertEquals(HistogramFilter.of("Lambda 1", HistogramFilter.Condition.AT_MOST, 0.68),
                saturation.filter("Lambda 1"));
        assertNull(HistogramSignals.saturation(log, 2), "every RPM value appears once");

        // knock retard: 0 most of the time, or a few times, is "no knock", not a sensor limit
        StringBuilder knock = new StringBuilder("Time\tKnock: Retard\tPressure\ns\tdeg\tkPa\n");
        for (int i = 0; i < 1000; i++) {
            knock.append(i / 100.0).append('\t').append(i % 3 == 0 ? 0 : 1 + i % 5 * 0.1)
                    .append('\t').append(i < 600 ? 250 : 100 + i * 0.01).append('\n');
        }
        CsvLogReader knockLog = CsvLogReader.read(new StringReader(knock.toString()));
        assertNull(HistogramSignals.saturation(knockLog, 1), "a pile at 0");
        assertNull(HistogramSignals.saturation(knockLog, 2), "60% at one value is where the signal rests");
    }

    @Test
    void defaultSignalsAndMixtureUnits() throws IOException {
        CsvLogReader log = CsvLogReader.read(new StringReader(
                "Time\tTPS\tMAP\tRPM\tFront Lambda\tTarget AFR\ns\t%\tkPa\trpm\tlambda\tAFR\n0\t1\t2\t3\t4\t5\n"));
        assertEquals(3, HistogramSignals.find(log, HistogramSignals.X_NAMES));
        assertEquals(2, HistogramSignals.find(log, HistogramSignals.Y_NAMES), "MAP before TPS");
        assertEquals(4, HistogramSignals.find(log, HistogramSignals.Z_NAMES));
        assertEquals(-1, HistogramSignals.find(log, "Boost"));
        assertEquals(4, HistogramSignals.findMixture(log));

        CsvLogReader targetsOnly = CsvLogReader.read(new StringReader(
                "Time\tTarget AFR\tFuel: target lambda\ns\tAFR\tlambda\n0\t14.7\t1\n"));
        assertTrue(HistogramSignals.find(targetsOnly, HistogramSignals.Z_NAMES) > 0, "the names alone match targets");
        assertEquals(-1, HistogramSignals.findMixture(targetsOnly), "targets are not measurements");
        assertTrue(HistogramSignals.isLambda(log, 4));
        assertFalse(HistogramSignals.isAfr(log, 4));
        assertTrue(HistogramSignals.isAfr(log, 5));
        assertFalse(HistogramSignals.isLambda(log, 5));
    }
}
