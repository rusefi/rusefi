package com.rusefi.ui.logview;

import com.rusefi.logview.CsvLogReader;
import org.junit.jupiter.api.Test;

import java.awt.event.MouseEvent;
import java.awt.image.BufferedImage;
import java.io.IOException;
import java.io.StringReader;
import java.util.List;

import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertFalse;
import static org.junit.jupiter.api.Assertions.assertNull;
import static org.junit.jupiter.api.Assertions.assertTrue;

class LogChartSettingsTest {
    private static final int WIDTH = 600;
    private static final int HEIGHT = 400;
    private static final int BACKGROUND = PlotPalette.LIGHT.background.getRGB();

    /** Time 0..100 s at 1 ms steps. */
    private static CsvLogReader denseLog() throws IOException {
        StringBuilder text = new StringBuilder("Time\tRPM\ns\trpm\n");
        for (int i = 0; i <= 100_000; i++) {
            text.append(i / 1000.0).append('\t').append(i % 977).append('\n');
        }
        return CsvLogReader.read(new StringReader(text.toString()));
    }

    private static BufferedImage paint(LogChart chart) {
        chart.setSize(WIDTH, HEIGHT);
        BufferedImage image = new BufferedImage(WIDTH, HEIGHT, BufferedImage.TYPE_INT_RGB);
        chart.paint(image.createGraphics());
        return image;
    }

    /** Paints the chart and counts non-background pixels in the first plot's traces, below the empty-plot hint. */
    private static int plotInk(LogChart chart) {
        BufferedImage image = paint(chart);
        java.awt.Rectangle area = chart.bands().get(0).traces;
        int count = 0;
        for (int y = area.y + 40; y < area.y + area.height - 2; y++) {
            for (int x = area.x + 2; x < area.x + area.width - 2; x++) {
                if (image.getRGB(x, y) != BACKGROUND) {
                    count++;
                }
            }
        }
        return count;
    }

    @Test
    void gridIsOffByDefault() throws IOException {
        CsvLogReader log = denseLog();
        LogChart chart = new LogChart(log, new boolean[log.fieldCount()]);
        assertEquals(0, plotInk(chart));
    }

    @Test
    void verticalAndHorizontalGridsToggleIndependently() throws IOException {
        CsvLogReader log = denseLog();
        ChartSettings grid = new ChartSettings();
        LogChart chart = new LogChart(log, new boolean[log.fieldCount()], grid);

        grid.setVertical(true);
        int vertical = plotInk(chart);
        assertTrue(vertical > 0);

        grid.setVertical(false);
        grid.setHorizontal(true);
        int horizontal = plotInk(chart);
        assertTrue(horizontal > 0);

        grid.setVertical(true);
        assertTrue(plotInk(chart) > Math.max(vertical, horizontal));
    }

    @Test
    void moreHorizontalDivisionsDrawMoreLines() throws IOException {
        CsvLogReader log = denseLog();
        ChartSettings grid = new ChartSettings();
        grid.setHorizontal(true);
        LogChart chart = new LogChart(log, new boolean[log.fieldCount()], grid);

        grid.setHorizontalDivisions(2);
        int few = plotInk(chart);
        grid.setHorizontalDivisions(20);
        assertTrue(plotInk(chart) > few);
    }

    @Test
    void timeSpacingControlsTickDensity() throws IOException {
        CsvLogReader log = denseLog();
        ChartSettings grid = new ChartSettings();
        LogChart chart = new LogChart(log, new boolean[log.fieldCount()], grid);
        paint(chart);

        grid.setTimeSpacing(ChartSettings.MIN_TIME_SPACING);
        double dense = chart.timeTickStep();
        grid.setTimeSpacing(ChartSettings.MAX_TIME_SPACING);
        assertTrue(chart.timeTickStep() > dense);
    }

    @Test
    void sampleLinesOnlyOnceSamplesAreFarEnoughApart() throws IOException {
        CsvLogReader log = denseLog();
        LogChart chart = new LogChart(log, new boolean[log.fieldCount()]);
        paint(chart);
        assertNull(chart.sampleGridTimes(), "100k samples over ~500 px are too dense");

        chart.timeWindow().show(50, 50.05);
        List<Double> times = chart.sampleGridTimes();
        // 1 ms samples across a 50 ms window: one line per sample, roughly 10 px apart.
        assertEquals(51, times.size());
        assertEquals(50, times.get(0), 1e-9);
        assertEquals(50.05, times.get(times.size() - 1), 1e-9);
    }

    /**
     * Real logs contain samples sharing one timestamp (about 15% of test-log-1.msl). Such samples must
     * share one line, not count as two lines 0 px apart, which hid every sample line at any zoom.
     */
    @Test
    void sampleLinesWithDuplicateTimestamps() throws IOException {
        StringBuilder text = new StringBuilder("Time\tRPM\ns\trpm\n");
        for (int i = 0; i <= 100; i++) {
            // 10 ms samples, every fifth one logged twice with the same time
            text.append(i / 100.0).append('\t').append(i).append('\n');
            if (i % 5 == 0) {
                text.append(i / 100.0).append('\t').append(i + 1).append('\n');
            }
        }
        CsvLogReader log = CsvLogReader.read(new StringReader(text.toString()));
        LogChart chart = new LogChart(log, new boolean[log.fieldCount()]);
        paint(chart);
        // 0.2 s visible: 21 distinct times about 26 px apart
        chart.timeWindow().show(0.4, 0.6);
        List<Double> times = chart.sampleGridTimes();
        assertEquals(21, times.size(), "one line per distinct time");
        assertEquals(0.4, times.get(0), 1e-9);
        assertEquals(0.6, times.get(times.size() - 1), 1e-9);
    }

    @Test
    void sampleLinesAppearOnRealLogWhenZoomedIn() throws IOException {
        CsvLogReader log = LogViewerSandbox.load("/logview/test-log-1.msl");
        LogChart chart = new LogChart(log, new boolean[log.fieldCount()]);
        paint(chart);
        assertNull(chart.sampleGridTimes(), "3191 samples over ~500 px are too dense");

        double start = log.value(1000, 0);
        chart.timeWindow().show(start, start + 0.3);
        List<Double> times = chart.sampleGridTimes();
        assertTrue(times != null && times.size() > 10, "300 ms of a ~4 ms log: " + times);
    }

    @Test
    void sampleModeDrawsLinesWhenZoomedIn() throws IOException {
        CsvLogReader log = denseLog();
        ChartSettings grid = new ChartSettings();
        grid.setVertical(true);
        grid.setVerticalMode(ChartSettings.VerticalMode.SAMPLES);
        LogChart chart = new LogChart(log, new boolean[log.fieldCount()], grid);

        assertEquals(0, plotInk(chart), "too dense at full view: no lines");
        chart.timeWindow().show(50, 50.05);
        assertTrue(plotInk(chart) > 0);
    }

    /** Samples one second apart over 0..10 s, about 50 px apart on screen. */
    private static CsvLogReader sparseLog() throws IOException {
        StringBuilder text = new StringBuilder("Time\tRPM\ns\trpm\n");
        for (int i = 0; i <= 10; i++) {
            text.append(i).append('\t').append(i * 100).append('\n');
        }
        return CsvLogReader.read(new StringReader(text.toString()));
    }

    private static int moveMouseToTime(LogChart chart, double time) {
        int left = LogChart.LEFT;
        int right = WIDTH - 22;
        int x = left + (int) Math.round(time / 10 * (right - left));
        chart.dispatchEvent(new MouseEvent(chart, MouseEvent.MOUSE_MOVED, 0, 0, x, 200, 0, false));
        return x;
    }

    @Test
    void cursorSnapsToNearestSampleByDefault() throws IOException {
        CsvLogReader log = sparseLog();
        LogChart chart = new LogChart(log, new boolean[log.fieldCount()]);
        paint(chart);

        int mouseX = moveMouseToTime(chart, 3.3);
        int sampleX = moveMouseToTime(chart, 3);
        moveMouseToTime(chart, 3.3);
        assertEquals(sampleX, chart.cursorLineX());
        assertTrue(mouseX - sampleX > 10, "snapping must visibly move the line");
    }

    @Test
    void freeCursorFollowsMouse() throws IOException {
        CsvLogReader log = sparseLog();
        ChartSettings settings = new ChartSettings();
        settings.setSnapCursor(false);
        boolean[] displayed = new boolean[log.fieldCount()];
        displayed[1] = true;
        LogChart chart = new LogChart(log, displayed, settings);
        paint(chart);

        int mouseX = moveMouseToTime(chart, 3.3);
        assertEquals(mouseX, chart.cursorLineX());
        // Painting with the sample marker must not fail.
        paint(chart);
    }

    @Test
    void cursorHiddenOutsidePlot() throws IOException {
        CsvLogReader log = sparseLog();
        LogChart chart = new LogChart(log, new boolean[log.fieldCount()]);
        paint(chart);
        chart.dispatchEvent(new MouseEvent(chart, MouseEvent.MOUSE_MOVED, 0, 0, 5, 200, 0, false));
        assertEquals(-1, chart.cursorLineX());
        paint(chart);
        assertNull(chart.timeBadge(), "no cursor, no time badge");
    }

    @Test
    void timeLabelsNeverRunIntoEachOther() throws IOException {
        CsvLogReader log = denseLog();
        LogChart chart = new LogChart(log, new boolean[log.fieldCount()]);
        for (int fontSize : new int[]{12, 24}) {
            chart.setFont(new java.awt.Font(java.awt.Font.SANS_SERIF, java.awt.Font.PLAIN, fontSize));
            for (double span : new double[]{100, 10, 1, 0.1, 0.008}) {
                chart.timeWindow().show(37.7794, 37.7794 + span);
                paint(chart);
                List<java.awt.Rectangle> labels = chart.timeLabelBounds();
                assertTrue(labels.size() >= 2, span + " s at " + fontSize + " pt: " + labels.size() + " labels");
                for (int i = 1; i < labels.size(); i++) {
                    java.awt.Rectangle left = labels.get(i - 1);
                    assertTrue(left.x + left.width < labels.get(i).x,
                            span + " s at " + fontSize + " pt: label " + i + " overlaps the one before");
                }
            }
        }
    }

    @Test
    void zoomedInTimeLabelsShowTheFullTime() throws IOException {
        CsvLogReader log = denseLog();
        LogChart chart = new LogChart(log, new boolean[log.fieldCount()]);
        chart.timeWindow().show(37.7794, 37.7874);
        paint(chart);
        assertEquals("37.780", chart.timeLabel(37.780, 0.001));
        assertEquals("37.7802", chart.timeLabel(37.7802, 0.0002));
        assertEquals("37", chart.timeLabel(37, 1));
    }

    @Test
    void cursorTimeShowsOnTheTimeAxisUnderTheCursorLine() throws IOException {
        CsvLogReader log = denseLog();
        LogChart chart = new LogChart(log, new boolean[log.fieldCount()]);
        paint(chart);
        java.awt.Rectangle traces = chart.bands().get(0).traces;
        int x = traces.x + traces.width / 2;
        chart.dispatchEvent(new MouseEvent(chart, MouseEvent.MOUSE_MOVED, 0, 0, x, traces.y + 10, 0, false));
        paint(chart);
        java.awt.Rectangle badge = chart.timeBadge();
        assertTrue(badge.x < chart.cursorLineX() && chart.cursorLineX() < badge.x + badge.width, "centred on the line");
        assertTrue(badge.y > traces.y + traces.height, "below the plots, on the time axis");
        assertEquals(String.format(java.util.Locale.ROOT, "%.2f s", chart.cursorTime()), chart.timeBadgeText());

        // with a large font the badge reaches the title row: the title steps aside
        chart.setFont(new java.awt.Font(java.awt.Font.SANS_SERIF, java.awt.Font.PLAIN, 24));
        paint(chart);
        int middle = chart.timeTitleBounds().x + chart.timeTitleBounds().width / 2;
        chart.dispatchEvent(new MouseEvent(chart, MouseEvent.MOUSE_MOVED, 0, 0, middle, traces.y + 10, 0, false));
        paint(chart);
        assertFalse(chart.timeBadge().intersects(chart.timeTitleBounds()), "title clear of the badge");
        chart.setFont(null);

        // at the right edge the badge stays inside the chart
        chart.dispatchEvent(new MouseEvent(chart, MouseEvent.MOUSE_MOVED, 0, 0, traces.x + traces.width, traces.y + 10, 0, false));
        paint(chart);
        assertTrue(chart.timeBadge().x + chart.timeBadge().width <= WIDTH);
    }

    @Test
    void plotBackgroundFollowsLightDarkSetting() throws IOException {
        CsvLogReader log = sparseLog();
        ChartSettings settings = new ChartSettings();
        LogChart chart = new LogChart(log, new boolean[log.fieldCount()], settings);
        assertEquals(PlotPalette.LIGHT.background.getRGB(), paint(chart).getRGB(2, 2));
        settings.setDarkPlot(true);
        assertEquals(PlotPalette.DARK.background.getRGB(), paint(chart).getRGB(2, 2));
    }

    @Test
    void settingsClampToRange() {
        ChartSettings grid = new ChartSettings();
        assertFalse(grid.isVertical());
        assertFalse(grid.isHorizontal());
        grid.setTimeSpacing(1);
        assertEquals(ChartSettings.MIN_TIME_SPACING, grid.getTimeSpacing());
        grid.setTimeSpacing(10_000);
        assertEquals(ChartSettings.MAX_TIME_SPACING, grid.getTimeSpacing());
        grid.setHorizontalDivisions(0);
        assertEquals(ChartSettings.MIN_HORIZONTAL_DIVISIONS, grid.getHorizontalDivisions());
        grid.setHorizontalDivisions(500);
        assertEquals(ChartSettings.MAX_HORIZONTAL_DIVISIONS, grid.getHorizontalDivisions());
    }

    @Test
    void toolbarUpdatesSettingsAndRepaints() {
        ChartSettings grid = new ChartSettings();
        int[] repaints = {0};
        ChartToolbar toolbar = new ChartToolbar(grid, () -> repaints[0]++);
        assertEquals("Grid: Off  ▾", toolbar.gridButton.getText());
        assertFalse(toolbar.samplesMode.isEnabled());
        assertFalse(toolbar.horizontalDivisions.isEnabled());

        toolbar.verticalBox.doClick();
        assertTrue(grid.isVertical());
        assertTrue(toolbar.samplesMode.isEnabled());
        assertEquals("Grid: Vertical  ▾", toolbar.gridButton.getText());

        toolbar.samplesMode.doClick();
        assertEquals(ChartSettings.VerticalMode.SAMPLES, grid.getVerticalMode());
        assertFalse(toolbar.timeTicksMode.isSelected());
        assertFalse(toolbar.timeSpacing.isEnabled(), "spacing does not apply to sample lines");

        toolbar.horizontalBox.doClick();
        assertTrue(toolbar.horizontalDivisions.isEnabled());
        toolbar.horizontalDivisions.setValue(8);
        assertEquals(8, grid.getHorizontalDivisions());
        assertEquals("Grid: Both  ▾", toolbar.gridButton.getText());

        assertTrue(toolbar.snapCursor.isSelected());
        toolbar.freeCursor.doClick();
        assertFalse(grid.isSnapCursor());
        assertFalse(toolbar.snapCursor.isSelected());

        assertTrue(toolbar.lightPlot.isSelected());
        toolbar.darkPlot.doClick();
        assertTrue(grid.isDarkPlot());
        assertFalse(toolbar.lightPlot.isSelected());
        assertTrue(repaints[0] >= 6);
    }

    @Test
    void valuesButtonNamesTheChoices() {
        ChartSettings settings = new ChartSettings();
        ChartToolbar toolbar = new ChartToolbar(settings, () -> {
        });
        assertEquals("Values: Labels  ▾", toolbar.valuesButton.getText());

        toolbar.atClick.doClick();
        assertTrue(settings.isPinValues());
        assertEquals("Values: Labels at click  ▾", toolbar.valuesButton.getText());

        toolbar.valuesList.doClick();
        assertEquals(ChartSettings.CursorValues.LIST, settings.getCursorValues());
        assertEquals("Values: List at click  ▾", toolbar.valuesButton.getText());

        toolbar.valuesOff.doClick();
        assertEquals("Values: Off  ▾", toolbar.valuesButton.getText());
        assertFalse(toolbar.atClick.isEnabled(), "nothing to place while off");
    }
}
