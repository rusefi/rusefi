package com.rusefi.ui.logview;

import com.rusefi.logview.CsvLogReader;
import org.junit.jupiter.api.Test;

import javax.swing.JMenuItem;
import javax.swing.JPopupMenu;
import java.awt.Component;
import java.awt.Rectangle;
import java.awt.event.ActionEvent;
import java.awt.event.InputEvent;
import java.awt.event.MouseEvent;
import java.awt.image.BufferedImage;
import java.io.IOException;
import java.io.StringReader;
import java.util.Arrays;
import java.util.concurrent.atomic.AtomicInteger;

import static com.rusefi.ui.logview.MeasureCursors.Which.A;
import static com.rusefi.ui.logview.MeasureCursors.Which.B;
import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertFalse;
import static org.junit.jupiter.api.Assertions.assertNotNull;
import static org.junit.jupiter.api.Assertions.assertNull;
import static org.junit.jupiter.api.Assertions.assertTrue;

/** The A and B measurement cursors: placing, moving, clearing, and what the chart and Stats panel show. */
class MeasureCursorsTest {
    private static final int WIDTH = 800;
    private static final int HEIGHT = 500;

    /** Time 0..10 s, 0.1 s apart; RPM = 1000 + 50 per sample, so RPM at t is 1000 + 500 t. */
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

    /** Screen x of a time in the chart's whole-log view. */
    private static int xOf(LogChart chart, double time) {
        Rectangle area = chart.bands().get(0).traces;
        return area.x + (int) Math.round(time / 10 * area.width);
    }

    private static int middleY(LogChart chart) {
        Rectangle area = chart.bands().get(0).traces;
        return area.y + area.height / 2;
    }

    private static void click(LogChart chart, int x, int y) {
        chart.dispatchEvent(new MouseEvent(chart, MouseEvent.MOUSE_PRESSED, 0, InputEvent.BUTTON1_DOWN_MASK, x, y, 1, false, MouseEvent.BUTTON1));
        chart.dispatchEvent(new MouseEvent(chart, MouseEvent.MOUSE_RELEASED, 0, 0, x, y, 1, false, MouseEvent.BUTTON1));
        paint(chart);
    }

    private static void drag(LogChart chart, int fromX, int fromY, int toX) {
        chart.dispatchEvent(new MouseEvent(chart, MouseEvent.MOUSE_PRESSED, 0, InputEvent.BUTTON1_DOWN_MASK, fromX, fromY, 1, false, MouseEvent.BUTTON1));
        chart.dispatchEvent(new MouseEvent(chart, MouseEvent.MOUSE_DRAGGED, 0, InputEvent.BUTTON1_DOWN_MASK, toX, fromY, 1, false, MouseEvent.BUTTON1));
        chart.dispatchEvent(new MouseEvent(chart, MouseEvent.MOUSE_RELEASED, 0, 0, toX, fromY, 1, false, MouseEvent.BUTTON1));
        paint(chart);
    }

    private static JMenuItem item(JPopupMenu menu, String text) {
        for (Component component : menu.getComponents()) {
            if (component instanceof JMenuItem && text.equals(((JMenuItem) component).getText())) {
                return (JMenuItem) component;
            }
        }
        return null;
    }

    @Test
    void placesAThenBThenMovesTheNearerOne() {
        MeasureCursors cursors = new MeasureCursors();
        AtomicInteger changes = new AtomicInteger();
        cursors.addListener(changes::incrementAndGet);
        assertEquals(A, cursors.place(2, 0, 0));
        assertEquals(B, cursors.place(5, 3, 0));
        assertTrue(cursors.isRange());
        assertEquals(2, cursors.start());
        assertEquals(5, cursors.end());
        assertEquals(B, cursors.place(6, 4, 1), "nearer to B");
        assertEquals(A, cursors.place(1, 1, 5), "nearer to A");
        assertEquals(1, cursors.start());

        cursors.clear();
        assertTrue(cursors.isEmpty());
        assertEquals(5, changes.get());
        cursors.clear();
        assertEquals(5, changes.get(), "clearing nothing changes nothing");
    }

    @Test
    void clicksPlaceCursorsOnlyWhileMeasuring() throws IOException {
        LogChart chart = chart();
        click(chart, xOf(chart, 2), middleY(chart));
        assertTrue(chart.cursors().isEmpty(), "a plain click only selects the plot");

        chart.setMeasuring(true);
        click(chart, xOf(chart, 2), middleY(chart));
        click(chart, xOf(chart, 5), middleY(chart));
        assertEquals(2.0, chart.cursors().time(A), 1e-9, "snapped to the sample at 2 s");
        assertEquals(5.0, chart.cursors().time(B), 1e-9);

        click(chart, xOf(chart, 6), middleY(chart));
        assertEquals(6.0, chart.cursors().time(B), 1e-9, "a third click moves the nearer cursor");
        assertEquals(2.0, chart.cursors().time(A), 1e-9);

        chart.setMeasuring(false);
        assertTrue(chart.cursors().isEmpty(), "turning Measure off removes them");
    }

    @Test
    void aDragStillZoomsWhileMeasuring() throws IOException {
        LogChart chart = chart();
        chart.setMeasuring(true);
        drag(chart, xOf(chart, 2), middleY(chart), xOf(chart, 4));
        assertTrue(chart.cursors().isEmpty());
        assertEquals(2.0, chart.timeWindow().start(), 0.05);
        assertEquals(4.0, chart.timeWindow().end(), 0.05);
    }

    @Test
    void draggingAFlagMovesItsCursor() throws IOException {
        LogChart chart = chart();
        chart.setMeasuring(true);
        click(chart, xOf(chart, 2), middleY(chart));
        Rectangle flag = chart.cursorFlag(A);
        assertNotNull(flag);
        assertNull(chart.cursorFlag(B));
        drag(chart, flag.x + flag.width / 2, flag.y + flag.height / 2, xOf(chart, 7));
        assertEquals(7.0, chart.cursors().time(A), 1e-9);
        assertFalse(chart.cursors().has(B), "dragging a flag is not a click");
    }

    @Test
    void escapeClearsTheCursors() throws IOException {
        LogChart chart = chart();
        chart.setMeasuring(true);
        click(chart, xOf(chart, 2), middleY(chart));
        chart.getActionMap().get(chart.getInputMap(LogChart.WHEN_FOCUSED)
                .get(javax.swing.KeyStroke.getKeyStroke(java.awt.event.KeyEvent.VK_ESCAPE, 0)))
                .actionPerformed(new ActionEvent(chart, ActionEvent.ACTION_PERFORMED, ""));
        assertTrue(chart.cursors().isEmpty());
        assertTrue(chart.isMeasuring(), "Measure stays on for the next measurement");
    }

    @Test
    void withBothSetTheLegendShowsTheChangeAndStatisticsCoverAToB() throws IOException {
        LogChart chart = chart();
        chart.cursors().set(A, 2);
        chart.cursors().set(B, 6);
        paint(chart);
        assertEquals("A 2.00 s   B 6.00 s   Δt 4.00 s", chart.measureText());
        assertEquals("+2000", chart.delta(1), "RPM 2000 at 2 s, 4000 at 6 s");
        assertTrue(chart.legendValue(1, -1).endsWith("Δ+2000"), chart.legendValue(1, -1));
        TraceStats stats = chart.visibleStats().get(1);
        assertEquals(2000, stats.min, 1e-9, "between A and B, not over the whole log");
        assertEquals(4000, stats.max, 1e-9);

        chart.cursors().set(A, 8);
        assertEquals("-1000", chart.delta(1), "B before A: the change is B minus A (4000 - 5000)");
        assertEquals("A 8.00 s   B 6.00 s   Δt 2.00 s", chart.measureText());
    }

    @Test
    void plotMenuZoomsToAToBAndClears() throws IOException {
        LogChart chart = chart();
        assertNull(item(chart.plotMenu(0), "Zoom to A–B"), "not offered before measuring");
        chart.setMeasuring(true);
        assertFalse(item(chart.plotMenu(0), "Zoom to A–B").isEnabled());
        chart.cursors().set(A, 3);
        chart.cursors().set(B, 5);
        item(chart.plotMenu(0), "Zoom to A–B").doClick();
        assertEquals(3.0, chart.timeWindow().start(), 1e-9);
        assertEquals(5.0, chart.timeWindow().end(), 1e-9);
        item(chart.plotMenu(0), "Clear A/B").doClick();
        assertTrue(chart.cursors().isEmpty());
        assertNotNull(item(chart.plotMenu(0), "Move up"), "the usual items stay first");
        assertEquals("Move up", ((JMenuItem) chart.plotMenu(0).getComponent(0)).getText());
    }

    @Test
    void statsPanelShowsAAndBAndTheChange() throws IOException {
        LogViewerPanel panel = new LogViewerPanel(log(), null);
        panel.chart().plotLayout().clearPlot(0); // the test starts from an empty plot
        panel.toggle(1);
        StatsPanel stats = panel.statsPanel();
        assertEquals(Arrays.asList(StatsPanel.COLUMNS), columnNames(stats));

        panel.toolbar().measureButton.doClick();
        assertTrue(panel.chart().isMeasuring());
        panel.chart().cursors().set(A, 2);
        panel.chart().cursors().set(B, 6);
        assertEquals(Arrays.asList(StatsPanel.MEASURE_COLUMNS), columnNames(stats));
        assertEquals("2000", stats.text(0, 1));
        assertEquals("4000", stats.text(0, 2));
        assertEquals("+2000", stats.text(0, 3));
        assertEquals("2000", stats.text(0, 4), "min between A and B");
        assertTrue(stats.footer.getText().contains("between A and B"));

        panel.toolbar().measureButton.doClick();
        assertEquals(Arrays.asList(StatsPanel.COLUMNS), columnNames(stats), "Measure off: back to the cursor column");
    }

    private static java.util.List<String> columnNames(StatsPanel stats) {
        java.util.List<String> names = new java.util.ArrayList<>();
        for (int column = 0; column < stats.table.getColumnCount(); column++) {
            names.add(stats.table.getColumnName(column));
        }
        return names;
    }
}
