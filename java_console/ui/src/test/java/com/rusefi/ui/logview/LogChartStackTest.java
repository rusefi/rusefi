package com.rusefi.ui.logview;

import com.rusefi.logview.CsvLogReader;
import org.junit.jupiter.api.Test;

import javax.swing.JMenuItem;
import javax.swing.JPopupMenu;
import java.awt.Rectangle;
import java.awt.event.InputEvent;
import java.awt.event.MouseEvent;
import java.awt.geom.Rectangle2D;
import java.awt.image.BufferedImage;
import java.io.IOException;
import java.io.StringReader;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;

import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertFalse;
import static org.junit.jupiter.api.Assertions.assertTrue;

class LogChartStackTest {
    private static final int WIDTH = 700;
    private static final int HEIGHT = 600;

    /** Samples over 0..10 s with four signals. */
    private static CsvLogReader log() throws IOException {
        StringBuilder text = new StringBuilder("Time\tRPM\tTPS\tMAP\tCLT\ns\trpm\t%\tkPa\tC\n");
        for (int i = 0; i <= 100; i++) {
            text.append(i / 10.0).append('\t').append(1000 + i * 30).append('\t').append(i).append('\t')
                    .append(30 + i).append('\t').append(80 + i % 3).append('\n');
        }
        return CsvLogReader.read(new StringReader(text.toString()));
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

    private static void press(LogChart chart, int x, int y) {
        chart.dispatchEvent(new MouseEvent(chart, MouseEvent.MOUSE_PRESSED, 0, InputEvent.BUTTON1_DOWN_MASK, x, y, 1, false, MouseEvent.BUTTON1));
        chart.dispatchEvent(new MouseEvent(chart, MouseEvent.MOUSE_RELEASED, 0, 0, x, y, 1, false, MouseEvent.BUTTON1));
        paint(chart);
    }

    private static int centerX(Rectangle r) {
        return r.x + r.width / 2;
    }

    private static int centerY(Rectangle r) {
        return r.y + r.height / 2;
    }

    private static PlotLayout twoPlots() {
        PlotLayout layout = new PlotLayout(5);
        layout.add(1);
        layout.add(2);
        layout.addToNewPlot(3);
        return layout;
    }

    @Test
    void plotsStackTopToBottomWithEqualTraceAreas() throws IOException {
        LogChart chart = chart(twoPlots());
        List<LogChart.Band> bands = chart.bands();
        assertEquals(2, bands.size());
        Rectangle first = bands.get(0).traces;
        Rectangle second = bands.get(1).traces;
        assertTrue(first.y + first.height < bands.get(1).header.y, "plots do not overlap");
        assertTrue(Math.abs(first.height - second.height) <= 1);
        assertTrue(chart.addPlotRow().y > second.y + second.height, "add row is below the plots and axis");
    }

    @Test
    void eachTraceIsDrawnInsideItsOwnPlot() throws IOException {
        LogChart chart = chart(twoPlots());
        Rectangle first = chart.bands().get(0).traces;
        Rectangle second = chart.bands().get(1).traces;
        Rectangle2D rpm = chart.tracePath(1).getBounds2D();
        Rectangle2D map = chart.tracePath(3).getBounds2D();
        assertTrue(rpm.getMinY() >= first.y && rpm.getMaxY() <= first.y + first.height, rpm.toString());
        assertTrue(map.getMinY() >= second.y && map.getMaxY() <= second.y + second.height, map.toString());
    }

    @Test
    void clickingAPlotMakesItActive() throws IOException {
        PlotLayout layout = twoPlots();
        LogChart chart = chart(layout);
        assertEquals(1, layout.activePlot());
        press(chart, centerX(chart.bands().get(0).traces), centerY(chart.bands().get(0).traces));
        assertEquals(0, layout.activePlot());
        assertTrue(chart.timeWindow().isFull(), "a click is not a zoom");
    }

    @Test
    void addPlotRowAddsAnActivePlot() throws IOException {
        PlotLayout layout = twoPlots();
        LogChart chart = chart(layout);
        press(chart, centerX(chart.addPlotRow()), centerY(chart.addPlotRow()));
        assertEquals(3, layout.plotCount());
        assertEquals(2, layout.activePlot());
        assertEquals(3, chart.bands().size());
    }

    @Test
    void legendCloseRemovesTheTrace() throws IOException {
        PlotLayout layout = twoPlots();
        LogChart chart = chart(layout);
        LogChart.LegendItem tps = chart.bands().get(0).legend.get(1);
        assertEquals(2, tps.field);
        press(chart, centerX(tps.close), centerY(tps.close));
        assertEquals(Arrays.asList(1), layout.fields(0));
        assertFalse(layout.isPlotted(2));
    }

    @Test
    void clickingLegendNameKeepsTheTrace() throws IOException {
        PlotLayout layout = twoPlots();
        LogChart chart = chart(layout);
        LogChart.LegendItem rpm = chart.bands().get(0).legend.get(0);
        press(chart, rpm.bounds.x + 25, centerY(rpm.bounds));
        assertTrue(layout.isPlotted(1));
        assertEquals(0, layout.activePlot());
    }

    @Test
    void plotMenuMovesAndRemoves() throws IOException {
        PlotLayout layout = twoPlots();
        LogChart chart = chart(layout);
        JPopupMenu first = chart.plotMenu(0);
        assertFalse(item(first, "Move up").isEnabled());
        item(first, "Move down").doClick();
        assertEquals(Arrays.asList(3), layout.fields(0));

        JPopupMenu second = chart.plotMenu(1);
        assertFalse(item(second, "Move down").isEnabled());
        item(second, "Remove plot").doClick();
        assertEquals(1, layout.plotCount());
        assertFalse(item(chart.plotMenu(0), "Remove plot").isEnabled(), "the last plot stays");
    }

    @Test
    void clearPlotEmptiesItAndKeepsThePlot() throws IOException {
        PlotLayout layout = twoPlots();
        LogChart chart = chart(layout);
        List<Integer> first = new ArrayList<>(layout.fields(0));
        assertFalse(first.isEmpty());
        item(chart.plotMenu(0), "Clear plot").doClick();
        assertEquals(2, layout.plotCount(), "the plot stays");
        assertTrue(layout.fields(0).isEmpty());
        for (int field : first) {
            assertFalse(layout.isPlotted(field));
        }
        assertEquals(Arrays.asList(3), layout.fields(1), "the other plot keeps its signals");
        assertFalse(item(chart.plotMenu(0), "Clear plot").isEnabled(), "nothing left to clear");
    }

    @Test
    void cursorAndZoomAreShared() throws IOException {
        PlotLayout layout = twoPlots();
        LogChart chart = chart(layout);
        Rectangle second = chart.bands().get(1).traces;
        int from = second.x + second.width / 4;
        int to = second.x + second.width / 2;
        chart.dispatchEvent(new MouseEvent(chart, MouseEvent.MOUSE_PRESSED, 0, InputEvent.BUTTON1_DOWN_MASK, from, centerY(second), 1, false, MouseEvent.BUTTON1));
        chart.dispatchEvent(new MouseEvent(chart, MouseEvent.MOUSE_DRAGGED, 0, InputEvent.BUTTON1_DOWN_MASK, to, centerY(second), 0, false, MouseEvent.BUTTON1));
        chart.dispatchEvent(new MouseEvent(chart, MouseEvent.MOUSE_RELEASED, 0, 0, to, centerY(second), 1, false, MouseEvent.BUTTON1));
        assertEquals(2.5, chart.timeWindow().start(), 0.1);
        assertEquals(5, chart.timeWindow().end(), 0.1);

        // the cursor follows the mouse over any plot, including headers
        Rectangle firstHeader = chart.bands().get(0).header;
        chart.dispatchEvent(new MouseEvent(chart, MouseEvent.MOUSE_MOVED, 0, 0, to, firstHeader.y + 2, 0, false));
        assertTrue(chart.cursorLineX() >= 0);
    }

    @Test
    void doubleClickOnAddRowDoesNotResetZoom() throws IOException {
        LogChart chart = chart(twoPlots());
        chart.timeWindow().show(2, 4);
        Rectangle row = chart.addPlotRow();
        chart.dispatchEvent(new MouseEvent(chart, MouseEvent.MOUSE_CLICKED, 0, 0, centerX(row), centerY(row), 2, false, MouseEvent.BUTTON1));
        assertFalse(chart.timeWindow().isFull());
    }

    @Test
    void addRowSaysWhenFull() throws IOException {
        PlotLayout layout = new PlotLayout(5);
        for (int i = 1; i < PlotLayout.MAX_PLOTS; i++) {
            layout.addPlot();
        }
        LogChart chart = chart(layout);
        press(chart, centerX(chart.addPlotRow()), centerY(chart.addPlotRow()));
        assertEquals(PlotLayout.MAX_PLOTS, layout.plotCount());
    }

    private static JMenuItem item(JPopupMenu menu, String text) {
        for (java.awt.Component component : menu.getComponents()) {
            if (component instanceof JMenuItem && text.equals(((JMenuItem) component).getText())) {
                return (JMenuItem) component;
            }
        }
        throw new AssertionError("no menu item " + text);
    }
}
