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
import java.util.List;

import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertFalse;
import static org.junit.jupiter.api.Assertions.assertNotNull;
import static org.junit.jupiter.api.Assertions.assertNull;
import static org.junit.jupiter.api.Assertions.assertTrue;

/** Highlighting one trace: hover, pin, faded traces, min/max markers and the detail card. */
class LogChartFocusTest {
    private static final int WIDTH = 800;
    private static final int HEIGHT = 500;

    /** Time 0..10 s; RPM ramps 1000..6000 (min at the start, max at the end); TPS dips in the middle; FLAT is constant. */
    private static CsvLogReader log() throws IOException {
        StringBuilder text = new StringBuilder("Time\tRPM\tTPS\tFLAT\ns\trpm\t%\t\n");
        for (int i = 0; i <= 100; i++) {
            text.append(i / 10.0).append('\t').append(1000 + i * 50).append('\t')
                    .append(50 + Math.abs(i - 50) * 0.9).append("\t7\n");
        }
        return CsvLogReader.read(new StringReader(text.toString()));
    }

    private static LogChart chart() throws IOException {
        PlotLayout layout = new PlotLayout(4);
        layout.add(1);
        layout.add(2);
        layout.add(3);
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

    private static Point legendName(LogChart chart, int field) {
        for (LogChart.LegendItem item : chart.bands().get(0).legend) {
            if (item.field == field) {
                return new Point(item.bounds.x + 25, item.bounds.y + item.bounds.height / 2);
            }
        }
        throw new AssertionError("no legend item for " + field);
    }

    private static void move(LogChart chart, Point p) {
        chart.dispatchEvent(new MouseEvent(chart, MouseEvent.MOUSE_MOVED, 0, 0, p.x, p.y, 0, false));
    }

    private static void click(LogChart chart, Point p) {
        chart.dispatchEvent(new MouseEvent(chart, MouseEvent.MOUSE_PRESSED, 0, InputEvent.BUTTON1_DOWN_MASK, p.x, p.y, 1, false, MouseEvent.BUTTON1));
        chart.dispatchEvent(new MouseEvent(chart, MouseEvent.MOUSE_RELEASED, 0, 0, p.x, p.y, 1, false, MouseEvent.BUTTON1));
    }

    @Test
    void hoveringALegendItemFocusesItsTraceAndFadesTheOthers() throws IOException {
        LogChart chart = chart();
        PlotPalette palette = chart.palette();
        assertEquals(255, chart.traceColor(2, palette).getAlpha(), "nothing focused: all opaque");

        move(chart, legendName(chart, 1));
        assertEquals(1, chart.focus().focused());
        assertEquals(255, chart.traceColor(1, palette).getAlpha());
        assertEquals(LogChart.FADED_ALPHA, chart.traceColor(2, palette).getAlpha());

        chart.dispatchEvent(new MouseEvent(chart, MouseEvent.MOUSE_EXITED, 0, 0, 0, 0, 0, false));
        assertEquals(TraceFocus.NONE, chart.focus().focused());
    }

    @Test
    void clickPinsAndSecondClickReleases() throws IOException {
        LogChart chart = chart();
        click(chart, legendName(chart, 2));
        assertEquals(2, chart.focus().pinned());
        move(chart, center(chart.bands().get(0).traces));
        assertEquals(2, chart.focus().focused(), "stays focused after the pointer leaves the legend");

        click(chart, legendName(chart, 2));
        assertEquals(TraceFocus.NONE, chart.focus().pinned());
    }

    @Test
    void removingThePinnedTraceClearsTheFocus() throws IOException {
        LogChart chart = chart();
        click(chart, legendName(chart, 2));
        chart.plotLayout().remove(2);
        assertEquals(TraceFocus.NONE, chart.focus().focused());
    }

    @Test
    void markersSitAtTheVisibleExtremesAndTagsStayInsideThePlot() throws IOException {
        LogChart chart = chart();
        chart.focus().togglePin(1);
        paint(chart);
        Rectangle area = chart.bands().get(0).traces;
        List<LogChart.Marker> markers = chart.markers();
        assertEquals(2, markers.size());
        LogChart.Marker max = markers.get(0);
        LogChart.Marker min = markers.get(1);
        assertTrue(max.max);
        assertEquals("6000", max.text);
        assertEquals("1000", min.text);
        // RPM peaks at the top right and starts at the bottom left: both tags flip to stay inside
        assertTrue(max.tag.y > max.point.y, "max tag below its point at the top edge");
        assertTrue(min.tag.y < min.point.y, "min tag above its point at the bottom edge");
        for (LogChart.Marker marker : markers) {
            assertTrue(area.contains(marker.tag), marker.text + " tag " + marker.tag + " inside " + area);
        }

        chart.timeWindow().show(2, 4);
        paint(chart);
        assertEquals("3000", chart.markers().get(0).text, "follows the visible window");
    }

    @Test
    void flatTraceGetsOneMarker() throws IOException {
        LogChart chart = chart();
        chart.focus().togglePin(3);
        paint(chart);
        assertEquals(1, chart.markers().size());
    }

    @Test
    void cardShowsTheHoveredTracesStatistics() throws IOException {
        LogChart chart = chart();
        assertNull(chart.card());
        move(chart, legendName(chart, 1));
        paint(chart);
        Rectangle card = chart.card();
        assertNotNull(card);
        assertTrue(new Rectangle(0, 0, WIDTH, HEIGHT).contains(card), "inside the chart");
        List<String> lines = chart.cardLines();
        assertEquals("RPM visible 0.0–10.0 s", lines.get(0));
        assertEquals("max 6000 rpm @ 10.00 s", lines.get(1));
        assertEquals("min 1000 rpm @ 0.00 s", lines.get(2));
        assertEquals("avg 3500 rpm", lines.get(3));
    }

    /**
     * The card opens under the hovered legend item, at the top left of the plot; a max at the start of the visible
     * time would be hidden there, so the card moves to a spot that keeps the markers visible.
     */
    @Test
    void cardKeepsAMaxAtTheStartVisible() throws IOException {
        StringBuilder text = new StringBuilder("Time\tVBatt\ns\tV\n");
        for (int i = 0; i <= 100; i++) {
            text.append(i / 10.0).append('\t').append(14 - i * 0.02).append('\n');
        }
        PlotLayout layout = new PlotLayout(2);
        layout.add(1);
        LogChart chart = new LogChart(CsvLogReader.read(new StringReader(text.toString())), layout, new ChartSettings());
        paint(chart);
        move(chart, legendName(chart, 1));
        paint(chart);

        LogChart.Marker max = chart.markers().get(0);
        assertTrue(max.max);
        Rectangle card = chart.card();
        assertNotNull(card);
        for (LogChart.Marker marker : chart.markers()) {
            assertFalse(card.intersects(marker.tag), "the card " + card + " leaves the tag " + marker.tag + " visible");
            assertFalse(card.contains(marker.point), "and the marked point");
        }
        assertTrue(new Rectangle(0, 0, WIDTH, HEIGHT).contains(card), "inside the chart");
    }

    /** With nothing in the way the card stays in its usual place, under the legend item. */
    @Test
    void cardStaysUnderTheLegendItemWhenNothingIsInTheWay() throws IOException {
        LogChart chart = chart();
        move(chart, legendName(chart, 1));
        paint(chart);
        Rectangle card = chart.card();
        LogChart.LegendItem rpm = chart.bands().get(0).legend.get(0);
        assertEquals(rpm.bounds.y + rpm.bounds.height + 6, card.y, "RPM peaks at the right: the usual spot is clear");
        assertEquals(Math.max(4, rpm.bounds.x - 6), card.x);
    }

    @Test
    void pinnedTraceWithoutHoverHasMarkersButNoCard() throws IOException {
        LogChart chart = chart();
        click(chart, legendName(chart, 1));
        move(chart, center(chart.bands().get(0).traces));
        paint(chart);
        assertNull(chart.card());
        assertFalse(chart.markers().isEmpty());
    }

    @Test
    void statsRowsFocusAndPinToo() throws IOException {
        LogViewerPanel panel = new LogViewerPanel(log(), null);
        panel.chart().plotLayout().clearPlot(0); // the test starts from an empty plot
        panel.toggle(1);
        panel.toggle(2);
        StatsPanel stats = panel.statsPanel();
        LogChart chart = panel.chart();
        int row = -1;
        for (int i = 0; i < stats.currentRows().size(); i++) {
            if (stats.fieldAt(i) == 1) {
                row = i;
            }
        }
        Rectangle cell = stats.table.getCellRect(row, 0, true);
        Point p = center(cell);
        stats.table.dispatchEvent(new MouseEvent(stats.table, MouseEvent.MOUSE_MOVED, 0, 0, p.x, p.y, 0, false));
        assertEquals(1, chart.focus().focused());
        stats.table.dispatchEvent(new MouseEvent(stats.table, MouseEvent.MOUSE_CLICKED, 0, 0, p.x, p.y, 1, false, MouseEvent.BUTTON1));
        stats.table.dispatchEvent(new MouseEvent(stats.table, MouseEvent.MOUSE_EXITED, 0, 0, p.x, p.y, 0, false));
        assertEquals(1, chart.focus().pinned());
        assertEquals(1, chart.focus().focused());
    }
}
