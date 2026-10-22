package com.rusefi.ui.logview;

import com.rusefi.logview.CsvLogReader;
import org.junit.jupiter.api.Test;

import java.awt.event.InputEvent;
import java.awt.event.MouseEvent;
import java.awt.event.MouseWheelEvent;
import java.awt.geom.PathIterator;
import java.awt.image.BufferedImage;
import java.io.IOException;
import java.io.StringReader;

import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertFalse;
import static org.junit.jupiter.api.Assertions.assertTrue;

class LogChartNavigationTest {
    private static final int WIDTH = 600;
    private static final int HEIGHT = 400;
    private static final int Y = 200;

    /** Time 0..100 s at 1 ms steps, so the plot has far more samples than pixels. */
    private static CsvLogReader denseLog() throws IOException {
        StringBuilder text = new StringBuilder("Time\tRPM\ns\trpm\n");
        for (int i = 0; i <= 100_000; i++) {
            text.append(i / 1000.0).append('\t').append(i % 977).append('\n');
        }
        return CsvLogReader.read(new StringReader(text.toString()));
    }

    private static LogChart paintedChart(CsvLogReader log, boolean plotRpm) {
        boolean[] displayed = new boolean[log.fieldCount()];
        displayed[1] = plotRpm;
        LogChart chart = new LogChart(log, displayed);
        chart.setSize(WIDTH, HEIGHT);
        repaint(chart);
        return chart;
    }

    private static void repaint(LogChart chart) {
        chart.paint(new BufferedImage(WIDTH, HEIGHT, BufferedImage.TYPE_INT_RGB).createGraphics());
    }

    private static int plotLeft() {
        return LogChart.LEFT;
    }

    private static int plotRight() {
        return WIDTH - 22;
    }

    private static int xAtFraction(double fraction) {
        return plotLeft() + (int) Math.round(fraction * (plotRight() - plotLeft()));
    }

    private static void wheel(LogChart chart, int x, int notches, int modifiers) {
        chart.dispatchEvent(new MouseWheelEvent(chart, MouseEvent.MOUSE_WHEEL, 0, modifiers, x, Y, 0, false,
                MouseWheelEvent.WHEEL_UNIT_SCROLL, 1, notches));
    }

    private static void mouse(LogChart chart, int id, int x, int modifiers, int button, int clicks) {
        chart.dispatchEvent(new MouseEvent(chart, id, 0, modifiers, x, Y, clicks, false, button));
    }

    @Test
    void wheelZoomsAroundPointer() throws IOException {
        LogChart chart = paintedChart(denseLog(), false);
        TimeWindow window = chart.timeWindow();
        int x = xAtFraction(0.25);
        // the fraction of the plot width at that whole pixel
        double fraction = (x - plotLeft()) / (double) (plotRight() - plotLeft());
        double anchor = window.start() + fraction * window.span();

        wheel(chart, x, -1, 0);
        assertEquals(100 / LogChart.WHEEL_ZOOM, window.span(), 1e-9);
        assertEquals(anchor, window.start() + fraction * window.span(), 1e-6);

        wheel(chart, x, 1, 0);
        assertTrue(window.isFull());
    }

    @Test
    void shiftWheelPans() throws IOException {
        LogChart chart = paintedChart(denseLog(), false);
        TimeWindow window = chart.timeWindow();
        window.show(40, 60);

        wheel(chart, xAtFraction(0.5), 2, InputEvent.SHIFT_DOWN_MASK);
        assertEquals(40 + 2 * LogChart.WHEEL_PAN * 20, window.start(), 1e-9);
        assertEquals(20, window.span(), 1e-9);
    }

    @Test
    void dragSelectsRangeToZoomInto() throws IOException {
        LogChart chart = paintedChart(denseLog(), false);
        TimeWindow window = chart.timeWindow();
        int from = xAtFraction(0.6);
        int to = xAtFraction(0.2);

        mouse(chart, MouseEvent.MOUSE_PRESSED, from, InputEvent.BUTTON1_DOWN_MASK, MouseEvent.BUTTON1, 1);
        mouse(chart, MouseEvent.MOUSE_DRAGGED, to, InputEvent.BUTTON1_DOWN_MASK, MouseEvent.BUTTON1, 0);
        mouse(chart, MouseEvent.MOUSE_RELEASED, to, 0, MouseEvent.BUTTON1, 1);

        assertEquals(20, window.start(), 0.2);
        assertEquals(60, window.end(), 0.2);
    }

    @Test
    void shortDragIsNotAZoom() throws IOException {
        LogChart chart = paintedChart(denseLog(), false);
        int x = xAtFraction(0.5);

        mouse(chart, MouseEvent.MOUSE_PRESSED, x, InputEvent.BUTTON1_DOWN_MASK, MouseEvent.BUTTON1, 1);
        mouse(chart, MouseEvent.MOUSE_DRAGGED, x + LogChart.MIN_SELECTION_PIXELS - 1, InputEvent.BUTTON1_DOWN_MASK, MouseEvent.BUTTON1, 0);
        mouse(chart, MouseEvent.MOUSE_RELEASED, x + LogChart.MIN_SELECTION_PIXELS - 1, 0, MouseEvent.BUTTON1, 1);

        assertTrue(chart.timeWindow().isFull());
    }

    @Test
    void rightDragPansWithThePointer() throws IOException {
        LogChart chart = paintedChart(denseLog(), false);
        TimeWindow window = chart.timeWindow();
        window.show(40, 60);
        int from = xAtFraction(0.5);
        int to = xAtFraction(0.75);

        mouse(chart, MouseEvent.MOUSE_PRESSED, from, InputEvent.BUTTON3_DOWN_MASK, MouseEvent.BUTTON3, 1);
        mouse(chart, MouseEvent.MOUSE_DRAGGED, to, InputEvent.BUTTON3_DOWN_MASK, MouseEvent.BUTTON3, 0);
        mouse(chart, MouseEvent.MOUSE_RELEASED, to, 0, MouseEvent.BUTTON3, 1);

        // Dragging right by a quarter of the plot reveals earlier data.
        assertEquals(35, window.start(), 0.1);
        assertEquals(20, window.span(), 1e-9);
    }

    @Test
    void doubleClickShowsWholeLog() throws IOException {
        LogChart chart = paintedChart(denseLog(), false);
        chart.timeWindow().show(10, 20);
        assertFalse(chart.timeWindow().isFull());

        mouse(chart, MouseEvent.MOUSE_CLICKED, xAtFraction(0.5), 0, MouseEvent.BUTTON1, 2);
        assertTrue(chart.timeWindow().isFull());
    }

    @Test
    void tracePathSizeDependsOnPlotWidthNotSampleCount() throws IOException {
        LogChart chart = paintedChart(denseLog(), true);
        int segments = segmentCount(chart);
        // At most four points per pixel column, versus 100k samples.
        assertTrue(segments <= 4 * (plotRight() - plotLeft() + 2), "segments: " + segments);

        chart.timeWindow().show(50, 50.05);
        repaint(chart);
        int zoomedSegments = segmentCount(chart);
        assertTrue(zoomedSegments > 0 && zoomedSegments < 100, "visible samples only: " + zoomedSegments);
    }

    private static int segmentCount(LogChart chart) {
        int count = 0;
        for (PathIterator iterator = chart.tracePath(1).getPathIterator(null); !iterator.isDone(); iterator.next()) {
            count++;
        }
        return count;
    }

    @Test
    void niceStep() {
        assertEquals(1, LogChart.niceStep(0.7), 1e-12);
        assertEquals(2, LogChart.niceStep(1.3), 1e-12);
        assertEquals(5, LogChart.niceStep(4.1), 1e-12);
        assertEquals(10, LogChart.niceStep(6), 1e-12);
        assertEquals(0.02, LogChart.niceStep(0.017), 1e-12);
    }
}
