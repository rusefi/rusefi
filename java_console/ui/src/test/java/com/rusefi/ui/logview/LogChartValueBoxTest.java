package com.rusefi.ui.logview;

import com.rusefi.logview.CsvLogReader;
import org.junit.jupiter.api.Test;

import java.awt.Point;
import java.awt.Rectangle;
import java.awt.event.InputEvent;
import java.awt.event.KeyEvent;
import java.awt.event.MouseEvent;
import java.awt.image.BufferedImage;
import java.io.IOException;
import java.io.StringReader;
import java.util.Arrays;
import java.util.Collections;
import java.util.List;

import static org.junit.jupiter.api.Assertions.assertArrayEquals;
import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertFalse;
import static org.junit.jupiter.api.Assertions.assertNull;
import static org.junit.jupiter.api.Assertions.assertTrue;

/** The values at the cursor beside the cursor line: a label per trace, or a list box per plot. */
class LogChartValueBoxTest {
    private static final int WIDTH = 800;
    private static final int HEIGHT = 500;

    /** Time 0..10 s; RPM 1000..6000; TPS 0..100; MAP equals TPS, so their traces overlap. */
    private static CsvLogReader log() throws IOException {
        StringBuilder text = new StringBuilder("Time\tRPM\tTPS\tMAP\ns\trpm\t%\tkPa\n");
        for (int i = 0; i <= 100; i++) {
            text.append(i / 10.0).append('\t').append(1000 + i * 50).append('\t')
                    .append(i).append('\t').append(i).append('\n');
        }
        return CsvLogReader.read(new StringReader(text.toString()));
    }

    /** RPM alone in the first plot, TPS and MAP in the second. */
    private static LogChart chart(ChartSettings settings) throws IOException {
        PlotLayout layout = new PlotLayout(4);
        layout.add(1);
        layout.addPlot();
        layout.add(2);
        layout.add(3);
        LogChart chart = new LogChart(log(), layout, settings);
        paint(chart);
        return chart;
    }

    private static void paint(LogChart chart) {
        chart.setSize(WIDTH, HEIGHT);
        chart.paint(new BufferedImage(WIDTH, HEIGHT, BufferedImage.TYPE_INT_RGB).createGraphics());
    }

    private static Point inTraces(LogChart chart, int plot, double xFraction) {
        Rectangle area = chart.bands().get(plot).traces;
        return new Point(area.x + (int) (area.width * xFraction), area.y + area.height / 3);
    }

    private static void move(LogChart chart, Point p) {
        chart.dispatchEvent(new MouseEvent(chart, MouseEvent.MOUSE_MOVED, 0, 0, p.x, p.y, 0, false));
    }

    private static void click(LogChart chart, Point p) {
        chart.dispatchEvent(new MouseEvent(chart, MouseEvent.MOUSE_PRESSED, 0, InputEvent.BUTTON1_DOWN_MASK, p.x, p.y, 1, false, MouseEvent.BUTTON1));
        chart.dispatchEvent(new MouseEvent(chart, MouseEvent.MOUSE_RELEASED, 0, 0, p.x, p.y, 1, false, MouseEvent.BUTTON1));
    }

    private static LogChart.ValueBox boxOf(LogChart chart, int field) {
        for (LogChart.ValueBox box : chart.valueBoxes()) {
            if (box.field == field) {
                return box;
            }
        }
        throw new AssertionError("no label for " + field);
    }

    @Test
    void labelsSitBesideTheirTraces() throws IOException {
        LogChart chart = chart(new ChartSettings());
        assertTrue(chart.valueBoxes().isEmpty(), "nothing before the pointer is over the plots");

        move(chart, inTraces(chart, 0, 0.5));
        paint(chart);
        LogChart.ValueBox rpm = boxOf(chart, 1);
        assertEquals(Collections.singletonList("3500 rpm"), rpm.lines);
        assertTrue(rpm.bounds.x > rpm.point.x, "right of the cursor line");
        assertEquals(rpm.point.y, rpm.bounds.y + rpm.bounds.height / 2, 1, "centred on its trace");
        assertTrue(chart.bands().get(0).traces.contains(rpm.bounds));
    }

    @Test
    void overlappingTracesGetStackedLabelsInsteadOfOverlappingOnes() throws IOException {
        LogChart chart = chart(new ChartSettings());
        move(chart, inTraces(chart, 1, 0.5));
        paint(chart);
        LogChart.ValueBox tps = boxOf(chart, 2);
        LogChart.ValueBox map = boxOf(chart, 3);
        assertEquals(Collections.singletonList("50 %"), tps.lines);
        assertEquals(Collections.singletonList("50 kPa"), map.lines);
        assertEquals(tps.point, map.point, "the traces cross at the cursor");
        assertFalse(tps.bounds.intersects(map.bounds), tps.bounds + " and " + map.bounds + " apart");
    }

    @Test
    void spreadLabelsPushesOverlapsApartAndKeepsThemInside() {
        assertArrayEquals(new int[]{41, 91}, LogChart.spreadLabels(new double[]{50, 100}, 18, 2, 0, 200),
                "far apart: each centred on its point");
        assertArrayEquals(new int[]{41, 61}, LogChart.spreadLabels(new double[]{50, 50}, 18, 2, 0, 200),
                "same point: the second goes below");
        assertArrayEquals(new int[]{162, 182}, LogChart.spreadLabels(new double[]{199, 200}, 18, 2, 0, 200),
                "at the bottom edge: pushed back up");
        assertNull(LogChart.spreadLabels(new double[]{10, 20, 30}, 18, 2, 0, 50), "too many for the height");
    }

    @Test
    void labelsThatDoNotFitFallBackToTheList() throws IOException {
        StringBuilder text = new StringBuilder("Time\tA\tB\tC\tD\tE\tF\ns\t\t\t\t\t\t\n");
        for (int i = 0; i <= 10; i++) {
            text.append(i).append("\t1\t2\t3\t4\t5\t6\n");
        }
        PlotLayout layout = new PlotLayout(7);
        for (int field = 1; field <= 6; field++) {
            layout.add(field);
        }
        // four more plots leave the first one too short for six labels
        for (int i = 0; i < 4; i++) {
            layout.addPlot();
        }
        LogChart chart = new LogChart(CsvLogReader.read(new StringReader(text.toString())), layout, new ChartSettings());
        paint(chart);
        move(chart, inTraces(chart, 0, 0.5));
        paint(chart);
        assertEquals(1, chart.valueBoxes().size());
        assertEquals(-1, chart.valueBoxes().get(0).field);
        assertEquals(Arrays.asList("1", "2", "3", "4", "5", "6"), chart.valueBoxes().get(0).lines);
    }

    @Test
    void listShowsOneBoxPerPlot() throws IOException {
        ChartSettings settings = new ChartSettings();
        settings.setCursorValues(ChartSettings.CursorValues.LIST);
        LogChart chart = chart(settings);
        move(chart, inTraces(chart, 1, 0.5));
        paint(chart);
        List<LogChart.ValueBox> boxes = chart.valueBoxes();
        assertEquals(2, boxes.size());
        assertEquals(Collections.singletonList("3500 rpm"), boxes.get(0).lines);
        assertEquals(Arrays.asList("50 %", "50 kPa"), boxes.get(1).lines);
        for (LogChart.ValueBox box : boxes) {
            Rectangle area = chart.bands().get(box.plot).traces;
            assertTrue(area.contains(box.bounds), "box " + box.bounds + " inside its plot " + area);
        }
    }

    @Test
    void valuesFlipLeftOfTheCursorNearTheRightEdge() throws IOException {
        for (ChartSettings.CursorValues mode : new ChartSettings.CursorValues[]{
                ChartSettings.CursorValues.LABELS, ChartSettings.CursorValues.LIST}) {
            ChartSettings settings = new ChartSettings();
            settings.setCursorValues(mode);
            LogChart chart = chart(settings);
            Point right = inTraces(chart, 0, 0.98);
            move(chart, right);
            paint(chart);
            Rectangle bounds = chart.valueBoxes().get(0).bounds;
            assertTrue(bounds.x + bounds.width < right.x, mode + " left of the line");
        }
    }

    @Test
    void offAndPointerOverTheLegendShowNoValues() throws IOException {
        ChartSettings settings = new ChartSettings();
        LogChart chart = chart(settings);
        Rectangle header = chart.bands().get(0).header;
        move(chart, new Point(header.x + header.width / 2, header.y + header.height / 2));
        paint(chart);
        assertTrue(chart.valueBoxes().isEmpty(), "the legend shows its own values");

        settings.setCursorValues(ChartSettings.CursorValues.OFF);
        move(chart, inTraces(chart, 0, 0.5));
        paint(chart);
        assertTrue(chart.valueBoxes().isEmpty());
    }

    @Test
    void pinKeepsTheValuesAtTheClickedTimeUntilClickedAgainOrEscape() throws IOException {
        ChartSettings settings = new ChartSettings();
        settings.setPinValues(true);
        LogChart chart = chart(settings);
        Point quarter = inTraces(chart, 0, 0.25);
        move(chart, quarter);
        paint(chart);
        assertTrue(chart.valueBoxes().isEmpty(), "hovering alone shows nothing");

        click(chart, quarter);
        move(chart, inTraces(chart, 0, 0.75));
        paint(chart);
        assertEquals(Collections.singletonList("2250 rpm"), boxOf(chart, 1).lines, "stays at the clicked time");

        click(chart, quarter);
        paint(chart);
        assertTrue(chart.valueBoxes().isEmpty(), "a second click at the same time hides it");

        click(chart, quarter);
        chart.pressKeyForTests(KeyEvent.VK_ESCAPE, 0);
        paint(chart);
        assertTrue(chart.valueBoxes().isEmpty(), "Esc hides it");
    }
}
