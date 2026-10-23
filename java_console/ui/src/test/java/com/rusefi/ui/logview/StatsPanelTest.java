package com.rusefi.ui.logview;

import com.formdev.flatlaf.FlatClientProperties;
import com.rusefi.logview.CsvLogReader;
import org.junit.jupiter.api.Test;

import java.awt.Rectangle;
import java.awt.event.MouseEvent;
import java.awt.image.BufferedImage;
import java.io.IOException;
import java.io.StringReader;
import java.util.List;

import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertFalse;
import static org.junit.jupiter.api.Assertions.assertTrue;

/** The Stats panel and the legend's visible-range summary. */
class StatsPanelTest {
    /** Time 0..10 s at 0.1 s; RPM ramps 1000..6000, TPS 0.2..100.2, CLT around 82 C. */
    private static CsvLogReader log() throws IOException {
        StringBuilder text = new StringBuilder("Time\tRPM\tTPS\tCLT\ns\trpm\t%\tC\n");
        for (int i = 0; i <= 100; i++) {
            text.append(i / 10.0).append('\t').append(1000 + i * 50).append('\t').append(i).append(".2\t")
                    .append(81.5 + (i % 7) * 0.25).append('\n');
        }
        return CsvLogReader.read(new StringReader(text.toString()));
    }

    private static LogViewerPanel panel() throws IOException {
        LogViewerPanel panel = new LogViewerPanel(log(), "test.msl");
        panel.chart().plotLayout().clearPlot(0); // the test starts from an empty plot
        // RPM and TPS in the first plot, CLT in a second
        panel.toggle(1);
        panel.toggle(2);
        panel.addToNewPlot(3);
        return panel;
    }

    private static void paint(LogChart chart) {
        chart.setSize(700, 500);
        chart.paint(new BufferedImage(700, 500, BufferedImage.TYPE_INT_RGB).createGraphics());
    }

    @Test
    void rowsAreGroupedByPlotWhenStacked() throws IOException {
        LogViewerPanel panel = panel();
        List<StatsPanel.Row> rows = panel.statsPanel().currentRows();
        assertEquals(5, rows.size(), "PLOT 1, RPM, TPS, PLOT 2, CLT");
        assertTrue(rows.get(0).isHeading());
        assertEquals(1, rows.get(1).field);
        assertTrue(rows.get(3).isHeading());
        assertEquals("PLOT 2", panel.statsPanel().text(3, 0));

        panel.chart().plotLayout().removePlot(1);
        assertEquals(2, panel.statsPanel().currentRows().size(), "one plot: no headings");
    }

    @Test
    void valuesUseTheVisibleWindowAndFieldPrecision() throws IOException {
        LogViewerPanel panel = panel();
        StatsPanel stats = panel.statsPanel();
        assertEquals("RPM", stats.text(1, 0));
        assertEquals("1000", stats.text(1, 2));
        assertEquals("6000", stats.text(1, 3));
        assertEquals("3500", stats.text(1, 4));
        assertEquals("100.2", stats.text(2, 3), "TPS gets one decimal");

        panel.chart().timeWindow().show(2, 4);
        stats.refreshValues();
        assertEquals("2000", stats.text(1, 2));
        assertEquals("3000", stats.text(1, 3));
        assertEquals(ValueFormat.NONE, stats.text(1, 1), "no cursor");
    }

    @Test
    void cursorColumnFollowsTheCursor() throws IOException {
        LogViewerPanel panel = panel();
        LogChart chart = panel.chart();
        paint(chart);
        Rectangle area = chart.bands().get(0).traces;
        // half way along 0..10 s
        chart.dispatchEvent(new MouseEvent(chart, MouseEvent.MOUSE_MOVED, 0, 0,
                area.x + area.width / 2, area.y + 10, 0, false));
        assertEquals("3500", panel.statsPanel().text(1, 1));
    }

    @Test
    void legendSummarisesTheVisibleRangeWhenThePointerIsAway() throws IOException {
        LogViewerPanel panel = panel();
        LogChart chart = panel.chart();
        assertEquals("1000–6000", chart.legendValue(1, -1));
        chart.timeWindow().show(2, 4);
        assertEquals("2000–3000", chart.legendValue(1, -1));
        assertEquals("2500", chart.legendValue(1, 30), "with a cursor sample: its value");
    }

    @Test
    void toolbarButtonAndCloseShowAndHideThePanel() throws IOException {
        LogViewerPanel panel = panel();
        ChartToolbar toolbar = panel.toolbar();
        assertFalse(panel.isStatsVisible(), "off by default");
        assertEquals("viewToggle", toolbar.statsButton.getClientProperty(FlatClientProperties.STYLE_CLASS));

        toolbar.statsButton.doClick();
        assertTrue(panel.isStatsVisible());

        panel.statsPanel().closeButton.doClick();
        assertFalse(panel.isStatsVisible());
        assertFalse(toolbar.statsButton.isSelected(), "the toolbar button follows the close button");
    }

    @Test
    void footerNamesTheWindow() throws IOException {
        LogViewerPanel panel = panel();
        panel.chart().timeWindow().show(2, 4);
        panel.statsPanel().refreshValues();
        assertEquals("<html>Min, max and avg over the visible 2.00–4.00 s<br>"
                + "Hover a row to highlight its trace; click to keep it highlighted</html>", panel.statsPanel().footer.getText());

        for (int plot = panel.chart().plotLayout().plotCount() - 1; plot > 0; plot--) {
            panel.chart().plotLayout().removePlot(plot);
        }
        panel.toggle(1);
        panel.toggle(2);
        assertEquals("Plot signals to see their statistics", panel.statsPanel().footer.getText());
    }

    /** A long signal name stays on one line: shortened with "…", the unit kept while there is room. */
    @Test
    void longSignalNamesStayOnOneLine() {
        StatsPanel.SignalCell cell = new StatsPanel.SignalCell();
        cell.set(new TraceDot(), "Intake Air IAT", "deg C", new java.awt.Font("Dialog", java.awt.Font.PLAIN, 12),
                java.awt.Color.BLACK, java.awt.Color.GRAY, java.awt.Color.WHITE);
        String[] wide = cell.shown(400);
        assertEquals("Intake Air IAT", wide[0]);
        assertEquals("deg C", wide[1]);

        String[] medium = cell.shown(130);
        assertTrue(medium[0].endsWith("…"), medium[0]);
        assertEquals("deg C", medium[1], "the unit stays while the name keeps some room");

        String[] narrow = cell.shown(80);
        assertTrue(narrow[0].endsWith("…"), narrow[0]);
        assertEquals("", narrow[1], "then the unit is left out");
        assertEquals("Intake Air IAT (deg C)", cell.getToolTipText(), "the tooltip has it all");
    }
}
