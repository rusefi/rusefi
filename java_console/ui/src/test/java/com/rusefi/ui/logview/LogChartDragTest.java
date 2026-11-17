package com.rusefi.ui.logview;

import com.rusefi.logview.CsvLogReader;
import org.junit.jupiter.api.Test;

import java.awt.Point;
import java.awt.Rectangle;
import java.awt.event.InputEvent;
import java.awt.event.MouseEvent;
import java.awt.image.BufferedImage;
import java.io.IOException;
import java.io.StringReader;
import java.util.Arrays;
import java.util.Collections;

import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertFalse;
import static org.junit.jupiter.api.Assertions.assertTrue;

/** Dropping signals on plots and resizing plots by dragging the gap between them. */
class LogChartDragTest {
    private static final int WIDTH = 700;
    private static final int HEIGHT = 600;

    private static CsvLogReader log() throws IOException {
        StringBuilder text = new StringBuilder("Time\tRPM\tTPS\tMAP\tCLT\ns\trpm\t%\tkPa\tC\n");
        for (int i = 0; i <= 100; i++) {
            text.append(i / 10.0).append('\t').append(1000 + i * 30).append('\t').append(i).append('\t')
                    .append(30 + i).append('\t').append(80 + i % 3).append('\n');
        }
        return CsvLogReader.read(new StringReader(text.toString()));
    }

    /** RPM and TPS in plot 1, MAP in plot 2 (active). */
    private static PlotLayout twoPlots() {
        PlotLayout layout = new PlotLayout(5);
        layout.add(1);
        layout.add(2);
        layout.addToNewPlot(3);
        return layout;
    }

    private static LogChart chart(PlotLayout layout) throws IOException {
        LogChart chart = new LogChart(log(), layout, new ChartSettings());
        paint(chart);
        return chart;
    }

    private static void paint(LogChart chart) {
        chart.setSize(WIDTH, HEIGHT);
        chart.paint(new BufferedImage(WIDTH, HEIGHT, BufferedImage.TYPE_INT_RGB).createGraphics());
    }

    private static Point center(Rectangle r) {
        return new Point(r.x + r.width / 2, r.y + r.height / 2);
    }

    private static void mouse(LogChart chart, int id, Point p, int modifiers, int clicks) {
        chart.dispatchEvent(new MouseEvent(chart, id, 0, modifiers, p.x, p.y, clicks, false, MouseEvent.BUTTON1));
    }

    @Test
    void dropTargetsArePlotsAndTheAddRow() throws IOException {
        LogChart chart = chart(twoPlots());
        assertEquals(0, chart.dropTargetAt(center(chart.bands().get(0).traces)));
        assertEquals(0, chart.dropTargetAt(center(chart.bands().get(0).header)), "the legend header counts");
        assertEquals(1, chart.dropTargetAt(center(chart.bands().get(1).traces)));
        assertEquals(LogChart.ADD_ROW_TARGET, chart.dropTargetAt(center(chart.addPlotRow())));
        assertEquals(LogChart.NO_TARGET, chart.dropTargetAt(null));
    }

    @Test
    void signalFromTheListDropsIntoThePlotUnderIt() throws IOException {
        PlotLayout layout = twoPlots();
        LogChart chart = chart(layout);
        assertTrue(chart.dropSignal(4, center(chart.bands().get(0).traces)));
        assertEquals(Arrays.asList(1, 2, 4), layout.fields(0));
        assertEquals(0, layout.activePlot(), "the drop target becomes active");
    }

    @Test
    void traceDraggedToAnotherPlotKeepsItsColour() throws IOException {
        PlotLayout layout = twoPlots();
        LogChart chart = chart(layout);
        int slot = layout.colors().slot(2);
        assertTrue(chart.dropSignal(2, center(chart.bands().get(1).traces)));
        assertEquals(Collections.singletonList(1), layout.fields(0));
        assertEquals(Arrays.asList(3, 2), layout.fields(1));
        assertEquals(slot, layout.colors().slot(2));
    }

    @Test
    void dropOnAddRowMakesANewPlot() throws IOException {
        PlotLayout layout = twoPlots();
        LogChart chart = chart(layout);
        assertTrue(chart.dropSignal(1, center(chart.addPlotRow())));
        assertEquals(3, layout.plotCount());
        assertEquals(Collections.singletonList(1), layout.fields(2));
        assertEquals(Collections.singletonList(2), layout.fields(0));
    }

    @Test
    void dropOutsideThePlotsIsRefused() throws IOException {
        PlotLayout layout = twoPlots();
        LogChart chart = chart(layout);
        Rectangle last = chart.bands().get(1).traces;
        assertFalse(chart.dropSignal(4, new Point(10, last.y + last.height + 20)), "time axis area");
        assertFalse(layout.isPlotted(4));
    }

    @Test
    void draggingTheGapResizesTheTwoPlots() throws IOException {
        PlotLayout layout = twoPlots();
        LogChart chart = chart(layout);
        int firstBefore = chart.bands().get(0).traces.height;
        int secondBefore = chart.bands().get(1).traces.height;
        Point gap = center(chart.dividers().get(0));

        mouse(chart, MouseEvent.MOUSE_PRESSED, gap, InputEvent.BUTTON1_DOWN_MASK, 1);
        mouse(chart, MouseEvent.MOUSE_DRAGGED, new Point(gap.x, gap.y + 60), InputEvent.BUTTON1_DOWN_MASK, 0);
        mouse(chart, MouseEvent.MOUSE_RELEASED, new Point(gap.x, gap.y + 60), 0, 1);
        paint(chart);

        assertEquals(firstBefore + 60, chart.bands().get(0).traces.height, 2);
        assertEquals(secondBefore - 60, chart.bands().get(1).traces.height, 2);
        assertTrue(chart.timeWindow().isFull(), "resizing is not a range selection");
    }

    @Test
    void resizingStopsAtTheMinimumHeight() throws IOException {
        PlotLayout layout = twoPlots();
        LogChart chart = chart(layout);
        Point gap = center(chart.dividers().get(0));
        mouse(chart, MouseEvent.MOUSE_PRESSED, gap, InputEvent.BUTTON1_DOWN_MASK, 1);
        mouse(chart, MouseEvent.MOUSE_DRAGGED, new Point(gap.x, gap.y + 2000), InputEvent.BUTTON1_DOWN_MASK, 0);
        mouse(chart, MouseEvent.MOUSE_RELEASED, new Point(gap.x, gap.y + 2000), 0, 1);
        paint(chart);
        assertTrue(chart.bands().get(1).traces.height >= 20, "lower plot keeps a usable height");
    }

    @Test
    void doubleClickOnTheGapMakesPlotsEqual() throws IOException {
        PlotLayout layout = twoPlots();
        layout.setWeights(new double[]{400, 100});
        LogChart chart = chart(layout);
        chart.timeWindow().show(2, 4);
        mouse(chart, MouseEvent.MOUSE_CLICKED, center(chart.dividers().get(0)), 0, 2);
        paint(chart);
        int first = chart.bands().get(0).header.height + chart.bands().get(0).traces.height;
        int second = chart.bands().get(1).header.height + chart.bands().get(1).traces.height;
        assertEquals(first, second, 2);
        assertFalse(chart.timeWindow().isFull(), "the double-click does not reset the zoom");
    }

    @Test
    void pressingALegendItemDoesNotSelectARange() throws IOException {
        LogChart chart = chart(twoPlots());
        LogChart.LegendItem rpm = chart.bands().get(0).legend.get(0);
        Point name = new Point(rpm.bounds.x + 25, rpm.bounds.y + rpm.bounds.height / 2);
        mouse(chart, MouseEvent.MOUSE_PRESSED, name, InputEvent.BUTTON1_DOWN_MASK, 1);
        mouse(chart, MouseEvent.MOUSE_RELEASED, name, 0, 1);
        assertTrue(chart.timeWindow().isFull());
        assertTrue(chart.plotLayout().isPlotted(1));
    }
}
