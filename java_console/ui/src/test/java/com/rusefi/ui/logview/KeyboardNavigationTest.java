package com.rusefi.ui.logview;

import com.rusefi.logview.CsvLogReader;
import org.junit.jupiter.api.Test;

import java.awt.event.InputEvent;
import java.awt.event.KeyEvent;
import java.awt.event.MouseEvent;
import java.awt.image.BufferedImage;
import java.io.IOException;
import java.io.StringReader;

import static com.rusefi.ui.logview.MeasureCursors.Which.A;
import static com.rusefi.ui.logview.MeasureCursors.Which.B;
import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertFalse;
import static org.junit.jupiter.api.Assertions.assertTrue;

/** Keys on the plots: stepping the cursor, panning, zooming, the ends of the log and the A / B cursors. */
class KeyboardNavigationTest {
    private static final int WIDTH = 800;
    private static final int HEIGHT = 500;

    /** Time 0..10 s, samples 0.1 s apart (101 samples). */
    private static CsvLogReader log() throws IOException {
        StringBuilder text = new StringBuilder("Time\tRPM\ns\trpm\n");
        for (int i = 0; i <= 100; i++) {
            text.append(i / 10.0).append('\t').append(1000 + i * 50).append('\n');
        }
        return CsvLogReader.read(new StringReader(text.toString()));
    }

    private static LogChart chart() throws IOException {
        PlotLayout layout = new PlotLayout(2);
        layout.add(1);
        LogChart chart = new LogChart(log(), layout, new ChartSettings());
        paint(chart);
        return chart;
    }

    private static void paint(LogChart chart) {
        chart.setSize(WIDTH, HEIGHT);
        chart.paint(new BufferedImage(WIDTH, HEIGHT, BufferedImage.TYPE_INT_RGB).createGraphics());
    }

    private static void key(LogChart chart, int key) {
        chart.pressKeyForTests(key, 0);
    }

    @Test
    void arrowsStepTheCursorSampleBySample() throws IOException {
        LogChart chart = chart();
        assertEquals(-1, chart.cursorSample());
        key(chart, KeyEvent.VK_RIGHT);
        assertEquals(50, chart.cursorSample(), "the first key puts the cursor in the middle of the view");
        key(chart, KeyEvent.VK_RIGHT);
        assertEquals(51, chart.cursorSample());
        key(chart, KeyEvent.VK_LEFT);
        assertEquals(50, chart.cursorSample());
        chart.pressKeyForTests(KeyEvent.VK_RIGHT, InputEvent.SHIFT_DOWN_MASK);
        assertEquals(50 + LogChart.FAST_STEP, chart.cursorSample());
        assertEquals(6.0, chart.cursorTime(), 1e-9, "the cursor time follows the sample");
        assertTrue(chart.cursorLineX() > 0, "and the cursor line is drawn");
    }

    @Test
    void steppingOutOfTheViewScrollsIt() throws IOException {
        LogChart chart = chart();
        chart.timeWindow().show(2, 4);
        key(chart, KeyEvent.VK_RIGHT);
        assertEquals(30, chart.cursorSample());
        for (int i = 0; i < 15; i++) {
            key(chart, KeyEvent.VK_RIGHT);
        }
        assertEquals(45, chart.cursorSample());
        assertEquals(4.5, chart.timeWindow().end(), 1e-9, "the view moved to keep the cursor visible");
        assertEquals(2.0, chart.timeWindow().span(), 1e-9, "without zooming");
    }

    @Test
    void homeAndEndGoToTheEndsOfTheLog() throws IOException {
        LogChart chart = chart();
        chart.timeWindow().show(4, 6);
        key(chart, KeyEvent.VK_END);
        assertEquals(100, chart.cursorSample());
        assertEquals(10.0, chart.timeWindow().end(), 1e-9);
        key(chart, KeyEvent.VK_HOME);
        assertEquals(0, chart.cursorSample());
        assertEquals(0.0, chart.timeWindow().start(), 1e-9);
        key(chart, KeyEvent.VK_LEFT);
        assertEquals(0, chart.cursorSample(), "stays at the first sample");
    }

    @Test
    void ctrlArrowsPanAndZeroShowsTheWholeLog() throws IOException {
        LogChart chart = chart();
        chart.timeWindow().show(2, 4);
        chart.pressKeyForTests(KeyEvent.VK_RIGHT, InputEvent.CTRL_DOWN_MASK);
        assertEquals(3.0, chart.timeWindow().start(), 1e-9, "half the visible time");
        chart.pressKeyForTests(KeyEvent.VK_LEFT, InputEvent.CTRL_DOWN_MASK);
        assertEquals(2.0, chart.timeWindow().start(), 1e-9);
        key(chart, KeyEvent.VK_0);
        assertEquals(0.0, chart.timeWindow().start(), 1e-9);
        assertEquals(10.0, chart.timeWindow().end(), 1e-9);
    }

    @Test
    void plusAndMinusZoomAroundTheCursor() throws IOException {
        LogChart chart = chart();
        key(chart, KeyEvent.VK_END);
        key(chart, KeyEvent.VK_HOME);
        for (int i = 0; i < 20; i++) {
            key(chart, KeyEvent.VK_RIGHT);
        }
        double before = chart.timeWindow().span();
        key(chart, KeyEvent.VK_PLUS);
        assertEquals(before / LogChart.KEY_ZOOM, chart.timeWindow().span(), 1e-9);
        assertTrue(chart.timeWindow().start() <= 2.0 && chart.timeWindow().end() >= 2.0, "the cursor stays in view");
        key(chart, KeyEvent.VK_EQUALS);
        assertEquals(before / LogChart.KEY_ZOOM / LogChart.KEY_ZOOM, chart.timeWindow().span(), 1e-9,
                "= works like + without Shift");
        key(chart, KeyEvent.VK_MINUS);
        key(chart, KeyEvent.VK_SUBTRACT);
        assertEquals(before, chart.timeWindow().span(), 1e-9);
    }

    @Test
    void aAndBDropMeasurementCursorsAndEscapeClearsThenUnpins() throws IOException {
        LogViewerPanel panel = new LogViewerPanel(log(), null);
        panel.toggle(1);
        LogChart chart = panel.chart();
        paint(chart);
        key(chart, KeyEvent.VK_RIGHT);
        key(chart, KeyEvent.VK_A);
        assertTrue(chart.isMeasuring(), "A turns Measure on");
        assertTrue(panel.toolbar().measureButton.isSelected(), "and the toolbar shows it");
        assertEquals(5.0, chart.cursors().time(A), 1e-9);
        for (int i = 0; i < 10; i++) {
            key(chart, KeyEvent.VK_RIGHT);
        }
        key(chart, KeyEvent.VK_B);
        assertEquals(6.0, chart.cursors().time(B), 1e-9);

        chart.focus().togglePin(1);
        key(chart, KeyEvent.VK_ESCAPE);
        assertTrue(chart.cursors().isEmpty(), "Esc clears A and B first");
        assertEquals(1, chart.focus().pinned());
        key(chart, KeyEvent.VK_ESCAPE);
        assertEquals(TraceFocus.NONE, chart.focus().pinned(), "then releases the highlighted trace");
    }

    @Test
    void theMouseTakesTheCursorBack() throws IOException {
        LogChart chart = chart();
        key(chart, KeyEvent.VK_HOME);
        assertEquals(0, chart.cursorSample());
        int x = chart.bands().get(0).traces.x + chart.bands().get(0).traces.width / 2;
        int y = chart.bands().get(0).traces.y + 20;
        chart.dispatchEvent(new MouseEvent(chart, MouseEvent.MOUSE_MOVED, 0, 0, x, y, 0, false));
        assertFalse(chart.cursorSample() == 0, "the cursor follows the pointer again");
    }
}
