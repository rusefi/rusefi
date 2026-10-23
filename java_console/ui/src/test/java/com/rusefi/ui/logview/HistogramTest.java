package com.rusefi.ui.logview;

import com.rusefi.logview.CsvLogReader;
import org.junit.jupiter.api.Test;

import java.io.IOException;
import java.io.StringReader;
import java.util.Arrays;
import java.util.Collections;

import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertNotNull;
import static org.junit.jupiter.api.Assertions.assertNull;
import static org.junit.jupiter.api.Assertions.assertThrows;
import static org.junit.jupiter.api.Assertions.assertTrue;

class HistogramTest {
    private static final double EPS = 1e-9;

    /** Rows of {time, rpm, map, lambda}. */
    private static CsvLogReader log(double[]... rows) throws IOException {
        StringBuilder text = new StringBuilder("Time\tRPM\tMAP\tLambda 1\ns\trpm\tkPa\tlambda\n");
        for (double[] row : rows) {
            for (int i = 0; i < row.length; i++) {
                text.append(i == 0 ? "" : "\t").append(Double.isNaN(row[i]) ? "" : Double.toString(row[i]));
            }
            text.append('\n');
        }
        return CsvLogReader.read(new StringReader(text.toString()));
    }

    private static Histogram compute(CsvLogReader log, HistogramAxis x, HistogramAxis y) {
        return Histogram.compute(log, 1, 2, 3, x, y, new boolean[log.sampleCount()], Double.NaN, Double.NaN);
    }

    @Test
    void axisPutsValuesInTheNearestBinAndClampsAtTheEnds() {
        HistogramAxis axis = new HistogramAxis(new double[]{1000, 2000, 4000});
        assertEquals(0, axis.index(500), "below the first centre: first bin");
        assertEquals(0, axis.index(1500), "half way goes to the lower bin");
        assertEquals(1, axis.index(1501));
        assertEquals(1, axis.index(3000));
        assertEquals(2, axis.index(3001));
        assertEquals(2, axis.index(9000), "above the last centre: last bin");
        assertEquals(-1, axis.index(Double.NaN));
        assertEquals(500, axis.lower(0), EPS);
        assertEquals(1500, axis.upper(0), EPS);
        assertEquals(3000, axis.upper(1), EPS);
        assertEquals(5000, axis.upper(2), EPS);
        assertThrows(IllegalArgumentException.class, () -> new HistogramAxis(new double[]{1, 1}));
    }

    @Test
    void evenAxisSpansTheRangeOrIsOneBin() {
        HistogramAxis axis = HistogramAxis.even(800, 2300, 4);
        assertEquals(4, axis.size());
        assertEquals(800, axis.center(0), EPS);
        assertEquals(1300, axis.center(1), EPS);
        assertEquals(2300, axis.center(3), EPS);
        assertEquals(1, HistogramAxis.even(5, 5, 16).size(), "a constant signal has one bin");
    }

    @Test
    void cellsAreWeightedByTheTimeEachRecordStandsFor() throws IOException {
        // 1 s in the first cell at lambda 1.0, then 3 s in the second at 0.9 and 0.8 (1 s and 2 s)
        CsvLogReader log = log(
                new double[]{0, 1000, 30, 1.0},
                new double[]{1, 3000, 80, 0.9},
                new double[]{2, 3000, 80, 0.8},
                new double[]{4, 3000, 80, 0.8});
        HistogramAxis x = new HistogramAxis(new double[]{1000, 3000});
        HistogramAxis y = new HistogramAxis(new double[]{30, 80});
        Histogram histogram = compute(log, x, y);

        Histogram.Cell idle = histogram.cell(0, 0);
        assertEquals(1, idle.count);
        assertEquals(1, idle.time, EPS);
        assertEquals(1.0, idle.mean(), EPS);
        assertNull(histogram.cell(1, 0));
        assertNull(histogram.cell(0, 1));

        Histogram.Cell load = histogram.cell(1, 1);
        assertEquals(3, load.count);
        // the last record counts one typical (median) interval: 1 s
        assertEquals(4, load.time, EPS);
        assertEquals((0.9 * 1 + 0.8 * 2 + 0.8 * 1) / 4, load.mean(), EPS);
        assertEquals(0.8, load.min, EPS);
        assertEquals(0.9, load.max, EPS);
        assertEquals(Math.sqrt(0.25 * 0.81 + 0.75 * 0.64 - Math.pow(0.825, 2)), load.sigma(), 1e-6);
        assertEquals(4, histogram.usedRecords);
        assertEquals(5, histogram.totalTime, EPS);
    }

    @Test
    void aGapCountsOnlyAFewTypicalIntervals() throws IOException {
        double[][] rows = new double[12][];
        for (int i = 0; i < 10; i++) {
            rows[i] = new double[]{i * 0.1, 1000, 30, 1.0};
        }
        // the logger paused for a minute
        rows[10] = new double[]{60.9, 1000, 30, 1.0};
        rows[11] = new double[]{61.0, 1000, 30, 1.0};
        CsvLogReader log = log(rows);
        assertEquals(0.1, Histogram.typicalInterval(log), 1e-6);
        double[] weights = Histogram.weights(log);
        assertEquals(0.1 * Histogram.GAP_LIMIT, weights[9], 1e-6);
        Histogram histogram = compute(log, new HistogramAxis(new double[]{1000}), new HistogramAxis(new double[]{30}));
        assertEquals(1.2 + 0.1 * Histogram.GAP_LIMIT - 0.1, histogram.cell(0, 0).time, 1e-6);
    }

    @Test
    void hiddenRecordsAndMissingValuesAreLeftOut() throws IOException {
        CsvLogReader log = log(
                new double[]{0, 1000, 30, 1.0},
                new double[]{1, 1000, 30, 0.68},
                new double[]{2, 1000, 30, Double.NaN},
                new double[]{3, 1000, 30, 1.02});
        boolean[] hidden = HistogramFilter.hidden(log,
                Collections.singletonList(HistogramFilter.of("Lambda 1", HistogramFilter.Condition.AT_MOST, 0.68)));
        Histogram histogram = Histogram.compute(log, 1, 2, 3, new HistogramAxis(new double[]{1000}),
                new HistogramAxis(new double[]{30}), hidden, Double.NaN, Double.NaN);
        Histogram.Cell cell = histogram.cell(0, 0);
        assertEquals(2, cell.count);
        assertEquals(1.0, cell.min, EPS);
        assertEquals(1, histogram.hiddenRecords);
        assertEquals(2, histogram.usedRecords);
    }

    @Test
    void timeRangeKeepsRecordsBetweenItsEnds() throws IOException {
        CsvLogReader log = log(
                new double[]{0, 1000, 30, 1.0},
                new double[]{1, 1000, 30, 1.0},
                new double[]{2, 1000, 30, 1.0},
                new double[]{3, 1000, 30, 1.0});
        Histogram histogram = Histogram.compute(log, 1, 2, 3, new HistogramAxis(new double[]{1000}),
                new HistogramAxis(new double[]{30}), new boolean[4], 1, 2);
        assertEquals(2, histogram.cell(0, 0).count, "the records at 1 s and 2 s");
    }

    @Test
    void automaticAxesSpanTheBinnedRecords() throws IOException {
        CsvLogReader log = log(
                new double[]{0, 0, 20, 1.0},
                new double[]{1, 800, 30, 1.0},
                new double[]{2, 2300, 90, 0.9},
                new double[]{3, 1600, 60, 0.95});
        boolean[] hidden = new boolean[4];
        // engine off: hidden, so the RPM axis starts at idle
        hidden[0] = true;
        Histogram histogram = Histogram.compute(log, 1, 2, 3, 4, 3, hidden, Double.NaN, Double.NaN);
        assertEquals(800, histogram.xAxis.center(0), EPS);
        assertEquals(2300, histogram.xAxis.center(3), EPS);
        assertEquals(30, histogram.yAxis.center(0), EPS);
        assertEquals(90, histogram.yAxis.center(2), EPS);
        assertEquals(3, histogram.usedRecords);
    }

    @Test
    void detailSpreadsTheMiddle90PercentAndCountsTheTails() throws IOException {
        // 100 readings 0.900..0.999 and three at 2.0 (fuel cut): the spread must not stretch to 2.0
        double[][] rows = new double[103][];
        for (int i = 0; i < 103; i++) {
            rows[i] = new double[]{i * 0.1, 1000, 30, i < 100 ? 0.9 + i * 0.001 : 2.0};
        }
        CsvLogReader log = log(rows);
        Histogram histogram = compute(log, new HistogramAxis(new double[]{1000}), new HistogramAxis(new double[]{30}));
        Histogram.Detail detail = histogram.detail(0, 0, 10);
        assertEquals(6, detail.above, "the three 2.0 readings and the top of the normal ones (5% of 103)");
        assertTrue(detail.high < 1.0, "the spread ends among the normal readings: " + detail.high);
        assertTrue(detail.low > 0.9 && detail.below > 0, "the lowest 5% are counted below");
        assertEquals(103, detail.total());
        assertEquals(histogram.cell(0, 0).mean(), detail.mean, 1e-12);

        HistogramTable table = new HistogramTable();
        table.set(histogram, HistogramSettings.Mode.MEAN, HistogramTable.Mixture.LAMBDA, 0, false,
                new HistogramColors(new com.rusefi.core.preferences.storage.Node()));
        String spread = HistogramCellPanel.spread(detail, 1, table);
        assertTrue(spread.startsWith("<html>90% of readings between 0.91 and 1.00<br>"), spread);
        assertTrue(spread.contains(detail.below + " below, 6 above"), spread);

        HistogramCellPanel.Distribution bars = new HistogramCellPanel.Distribution();
        bars.setSize(300, 80);
        bars.set(detail, 1, table::formatZ);
        java.awt.event.MouseEvent atEnd = new java.awt.event.MouseEvent(bars, java.awt.event.MouseEvent.MOUSE_MOVED,
                0, 0, 295, 30, 0, false);
        assertEquals("6 readings (6%) above 1.00", bars.getToolTipText(atEnd));
        bars.paint(new java.awt.image.BufferedImage(300, 80, java.awt.image.BufferedImage.TYPE_INT_RGB).createGraphics());
    }

    @Test
    void detailCountsVisitsAndSpreadsValues() throws IOException {
        CsvLogReader log = log(
                new double[]{0, 1000, 30, 0.9},
                new double[]{1, 1000, 30, 1.05},
                new double[]{2, 3000, 80, 0.85},
                new double[]{3, 1000, 30, 1.1},
                new double[]{4, 1000, 30, 1.08});
        Histogram histogram = compute(log, new HistogramAxis(new double[]{1000, 3000}),
                new HistogramAxis(new double[]{30, 80}));
        Histogram.Detail detail = histogram.detail(0, 0, 2);
        assertNotNull(detail);
        assertEquals(2, detail.visits);
        assertEquals(2, detail.longestVisit, EPS);
        assertEquals(0.9, detail.low, EPS);
        assertEquals(1.1, detail.high, EPS);
        assertEquals("[1, 3]", Arrays.toString(detail.distribution));
        assertNull(histogram.detail(1, 0, 2));
    }
}
