package com.rusefi.ui.logview;

import com.rusefi.logview.CsvLogReader;
import org.junit.jupiter.api.Test;

import java.awt.Component;
import java.awt.event.InputEvent;
import java.awt.event.MouseEvent;
import java.awt.image.BufferedImage;
import java.io.IOException;
import java.io.StringReader;
import java.util.ArrayList;
import java.util.Collections;
import java.util.List;

import static org.junit.jupiter.api.Assertions.assertEquals;

/** A pan-drag that reaches the edge of the plots (issue #10669). */
class LogChartEdgePanTest {
    private static final int WIDTH = 600;
    private static final int HEIGHT = 400;
    private static final int Y = 200;
    private static final int PLOT_RIGHT = WIDTH - 22;

    private static CsvLogReader log() throws IOException {
        StringBuilder text = new StringBuilder("Time\tRPM\ns\trpm\n");
        for (int i = 0; i <= 10_000; i++) {
            text.append(i / 100.0).append('\t').append(i % 977).append('\n');
        }
        return CsvLogReader.read(new StringReader(text.toString()));
    }

    private static LogChart chart() throws IOException {
        CsvLogReader log = log();
        LogChart chart = new LogChart(log, new boolean[log.fieldCount()]);
        chart.setSize(WIDTH, HEIGHT);
        chart.paint(new BufferedImage(WIDTH, HEIGHT, BufferedImage.TYPE_INT_RGB).createGraphics());
        return chart;
    }

    private static void drag(LogChart chart, int id, int x) {
        int modifiers = id == MouseEvent.MOUSE_RELEASED ? 0 : InputEvent.BUTTON3_DOWN_MASK;
        chart.dispatchEvent(new MouseEvent(chart, id, 0, modifiers, x, Y, id == MouseEvent.MOUSE_DRAGGED ? 0 : 1,
                false, MouseEvent.BUTTON3));
    }

    /** Seconds one pixel of drag pans while 10 s are shown. */
    private static double secondsPerPixel() {
        return 10.0 / (PLOT_RIGHT - LogChart.LEFT);
    }

    /** Records pointer moves, and reports them done or not. */
    private static final class FakeWarp implements PointerWarp {
        final List<Integer> targets = new ArrayList<>();
        final boolean works;

        FakeWarp(boolean works) {
            this.works = works;
        }

        @Override
        public boolean moveTo(Component component, int x, int y) {
            targets.add(x);
            return works;
        }
    }

    @Test
    void panGoesOnPastTheEdgeByWrappingThePointer() throws IOException {
        LogChart chart = chart();
        FakeWarp warp = new FakeWarp(true);
        chart.pointerWarp = warp;
        chart.manualEdgeScrollForTests = true;
        TimeWindow window = chart.timeWindow();
        window.show(60, 70);
        int middle = (LogChart.LEFT + PLOT_RIGHT) / 2;

        drag(chart, MouseEvent.MOUSE_PRESSED, middle);
        drag(chart, MouseEvent.MOUSE_DRAGGED, PLOT_RIGHT);
        double atEdge = window.start();
        assertEquals(60 - (PLOT_RIGHT - middle) * secondsPerPixel(), atEdge, 1e-6);
        // the pointer was moved to the left side
        int target = LogChart.LEFT + LogChart.WRAP_INSET;
        assertEquals(Collections.singletonList(target), warp.targets);

        // a drag event queued before the move still carries the edge x: it must not jump the view
        drag(chart, MouseEvent.MOUSE_DRAGGED, PLOT_RIGHT);
        assertEquals(atEdge, window.start(), 1e-9);
        // the move itself: no change, then dragging on keeps panning at the same rate
        drag(chart, MouseEvent.MOUSE_DRAGGED, target);
        assertEquals(atEdge, window.start(), 1e-9);
        drag(chart, MouseEvent.MOUSE_DRAGGED, target + 100);
        assertEquals(atEdge - 100 * secondsPerPixel(), window.start(), 1e-6);
        assertEquals(10, window.span(), 1e-9);

        drag(chart, MouseEvent.MOUSE_RELEASED, target + 100);
        assertEquals(atEdge - 100 * secondsPerPixel(), window.start(), 1e-6);
    }

    @Test
    void oneDragCrossesTheWholeLog() throws IOException {
        LogChart chart = chart();
        chart.pointerWarp = new FakeWarp(true);
        chart.manualEdgeScrollForTests = true;
        TimeWindow window = chart.timeWindow();
        window.show(0, 10);
        int target = PLOT_RIGHT - LogChart.WRAP_INSET;

        drag(chart, MouseEvent.MOUSE_PRESSED, PLOT_RIGHT - 50);
        for (int i = 0; i < 30; i++) {
            // drag left to the edge, the pointer reappears on the right
            drag(chart, MouseEvent.MOUSE_DRAGGED, LogChart.LEFT);
            drag(chart, MouseEvent.MOUSE_DRAGGED, target);
        }
        drag(chart, MouseEvent.MOUSE_RELEASED, target);

        // dragging left reveals later data: from the start of the 100 s log to its end in one drag
        assertEquals(90, window.start(), 1e-9);
        // and back the other way
        drag(chart, MouseEvent.MOUSE_PRESSED, LogChart.LEFT + 50);
        for (int i = 0; i < 30; i++) {
            drag(chart, MouseEvent.MOUSE_DRAGGED, PLOT_RIGHT);
            drag(chart, MouseEvent.MOUSE_DRAGGED, LogChart.LEFT + LogChart.WRAP_INSET);
        }
        drag(chart, MouseEvent.MOUSE_RELEASED, LogChart.LEFT + LogChart.WRAP_INSET);
        assertEquals(0, window.start(), 1e-9);
    }

    @Test
    void withoutPointerMovesHoldingAtTheEdgeKeepsScrolling() throws IOException {
        LogChart chart = chart();
        chart.pointerWarp = new FakeWarp(false);
        chart.manualEdgeScrollForTests = true;
        TimeWindow window = chart.timeWindow();
        window.show(60, 70);
        int middle = (LogChart.LEFT + PLOT_RIGHT) / 2;

        drag(chart, MouseEvent.MOUSE_PRESSED, middle);
        drag(chart, MouseEvent.MOUSE_DRAGGED, PLOT_RIGHT);
        double atEdge = window.start();
        chart.edgeScrollTickForTests();
        chart.edgeScrollTickForTests();
        assertEquals(atEdge - 2 * LogChart.EDGE_SCROLL_PIXELS * secondsPerPixel(), window.start(), 1e-6);

        // back inside the edge, the pointer pans by hand again and the ticks stop
        drag(chart, MouseEvent.MOUSE_DRAGGED, PLOT_RIGHT - 30);
        double inside = window.start();
        chart.edgeScrollTickForTests();
        assertEquals(inside, window.start(), 1e-9);
        drag(chart, MouseEvent.MOUSE_RELEASED, PLOT_RIGHT - 30);
    }

    @Test
    void anIgnoredPointerMoveFallsBackToScrolling() throws IOException {
        LogChart chart = chart();
        // the desktop accepts the move but the pointer stays (e.g. Wayland)
        chart.pointerWarp = new FakeWarp(true);
        chart.manualEdgeScrollForTests = true;
        TimeWindow window = chart.timeWindow();
        window.show(60, 70);

        drag(chart, MouseEvent.MOUSE_PRESSED, (LogChart.LEFT + PLOT_RIGHT) / 2);
        drag(chart, MouseEvent.MOUSE_DRAGGED, PLOT_RIGHT);
        double atEdge = window.start();
        chart.warpCheckForTests();
        assertEquals(atEdge, window.start(), 1e-9);
        chart.edgeScrollTickForTests();
        assertEquals(atEdge - LogChart.EDGE_SCROLL_PIXELS * secondsPerPixel(), window.start(), 1e-6);
        // and drag events at the edge pan from there without a jump
        double scrolled = window.start();
        drag(chart, MouseEvent.MOUSE_DRAGGED, PLOT_RIGHT);
        assertEquals(scrolled, window.start(), 1e-9);
        drag(chart, MouseEvent.MOUSE_RELEASED, PLOT_RIGHT);
    }
}
