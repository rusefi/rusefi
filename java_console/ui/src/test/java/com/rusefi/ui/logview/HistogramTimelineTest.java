package com.rusefi.ui.logview;

import com.opensr5.ConfigurationImage;
import com.opensr5.ini.DatalogEntry;
import com.opensr5.ini.IniFileModel;
import com.opensr5.ini.TableModel;
import com.opensr5.ini.field.ArrayIniField;
import com.rusefi.config.FieldType;
import com.rusefi.core.preferences.storage.Node;
import com.rusefi.logview.CsvLogReader;
import org.junit.jupiter.api.Test;

import java.io.IOException;
import java.io.StringReader;
import java.util.Arrays;
import java.util.Collections;
import java.util.List;
import java.util.Optional;
import java.util.concurrent.atomic.AtomicReference;

import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertFalse;
import static org.junit.jupiter.api.Assertions.assertTrue;
import static org.mockito.Mockito.mock;
import static org.mockito.Mockito.when;

/** The histogram's links out: the timeline (step 7), copying and keys (step 8) and tune table bins (step 9). */
class HistogramTimelineTest {
    /**
     * 350 records 0.1 s apart: idle (1000 rpm, 30 kPa) 0–10 s, load (3000 rpm, 90 kPa) 10–20 s, idle 20–30 s and
     * load again 30–35 s. "VE load" follows MAP at a tenth.
     */
    private static CsvLogReader log() throws IOException {
        StringBuilder text = new StringBuilder("Time\tRPM\tMAP\tLambda 1\tVE load\ns\trpm\tkPa\tlambda\t%\n");
        for (int i = 0; i < 350; i++) {
            boolean load = i >= 100 && i < 200 || i >= 300;
            text.append(i / 10.0).append('\t').append(load ? 3000 : 1000).append('\t').append(load ? 90 : 30)
                    .append('\t').append(load ? 0.85 : 1.0).append('\t').append(load ? 9 : 3).append('\n');
        }
        return CsvLogReader.read(new StringReader(text.toString()));
    }

    private static HistogramSettings twoByTwo() {
        HistogramSettings settings = new HistogramSettings(new Node());
        settings.setColumns(2);
        settings.setRows(2);
        return settings;
    }

    private static LogViewerPanel panel(HistogramSettings settings, IniFileModel ini) throws IOException {
        LogViewerPanel panel = new LogViewerPanel(log(), "test.msl", new ChartSettings(), new SignalLists(),
                new SidebarState(), ini, null);
        panel.chart().plotLayout().clearPlot(0); // the test starts from an empty plot
        panel.setHistogramSettings(settings);
        panel.setHistogramView(true);
        return panel;
    }

    // ---- step 7: timeline ----

    @Test
    void visitsAreTheSeparateStaysInACell() throws IOException {
        HistogramView view = panel(twoByTwo(), null).histogram();
        List<double[]> stays = view.table.histogram().visits(1, 1);
        assertEquals(2, stays.size());
        assertEquals(10, stays.get(0)[0], 1e-6);
        assertEquals(20, stays.get(0)[1], 1e-6, "until the next record");
        assertEquals(10, stays.get(0)[2], 1e-6);
        assertEquals(30, stays.get(1)[0], 1e-6);
        assertEquals(5, stays.get(1)[2], 1e-6);
    }

    @Test
    void showOnTimelineShadesTheStaysAndStepsThroughThem() throws IOException {
        LogViewerPanel panel = panel(twoByTwo(), null);
        HistogramView view = panel.histogram();
        view.table.select(1, 1);
        view.showSelectedOnTimeline();

        assertFalse(panel.isHistogramView());
        assertEquals(3, panel.chart().plotLayout().plotCount(), "an empty timeline gets X, Y and Z, one plot each");
        VisitsBar bar = panel.visitsBar();
        assertTrue(bar.isVisible());
        assertTrue(bar.titleLabel.getText().startsWith("RPM 3000 · MAP 90  ·  2 stays"), bar.titleLabel.getText());
        LogChart chart = panel.chart();
        assertEquals(2, chart.highlights().size());
        assertEquals(0, chart.currentHighlight(), "the longest stay first");
        assertTrue(chart.timeWindow().start() <= 10 && chart.timeWindow().end() >= 20, "the stay is in view");

        bar.nextButton.doClick();
        assertEquals(1, chart.currentHighlight());
        assertTrue(bar.positionLabel.getText().startsWith("Stay 2 of 2"), bar.positionLabel.getText());
        bar.nextButton.doClick();
        assertEquals(0, chart.currentHighlight(), "wraps around");

        bar.backButton.doClick();
        assertTrue(panel.isHistogramView());
        assertEquals(2, chart.highlights().size(), "the shading stays until cleared");
        panel.setHistogramView(false);
        bar.closeButton.doClick();
        assertTrue(chart.highlights().isEmpty());
        assertFalse(bar.isVisible());
    }

    @Test
    void showOnTimelineKeepsThePlotsTheUserMade() throws IOException {
        LogViewerPanel panel = panel(twoByTwo(), null);
        panel.setHistogramView(false);
        panel.toggle(4);
        panel.setHistogramView(true);
        panel.histogram().table.select(0, 0);
        panel.histogram().showSelectedOnTimeline();
        assertEquals(1, panel.chart().plotLayout().plotCount());
        assertEquals(Collections.singletonList(4), panel.chart().plotLayout().fields(0));
    }

    @Test
    void anEmptyCellHasNoStaysToShow() throws IOException {
        LogViewerPanel panel = panel(twoByTwo(), null);
        HistogramView view = panel.histogram();
        view.table.select(1, 0);
        view.showSelectedOnTimeline();
        assertTrue(panel.isHistogramView());
        assertTrue(view.selectedLabel.getText().startsWith("No records in"), view.selectedLabel.getText());
    }

    // ---- step 8: keys and copy ----

    @Test
    void arrowsMoveAndShiftArrowsSelectABlock() throws IOException {
        HistogramView view = panel(twoByTwo(), null).histogram();
        HistogramTable table = view.table;
        table.move(0, 0, false);
        assertEquals(0, table.selectedX());
        assertEquals(1, table.selectedY(), "nothing selected: the top left cell");
        table.move(1, 0, false);
        table.move(1, 0, false);
        assertEquals(1, table.selectedX(), "stops at the edge");
        table.move(0, -1, true);
        assertEquals("[1, 0, 1, 1]", Arrays.toString(table.block()));
        assertTrue(view.selectedLabel.getText().startsWith("Selected 1 × 2 cells"), view.selectedLabel.getText());
        table.selectAll();
        assertEquals("[0, 0, 1, 1]", Arrays.toString(table.block()));
        table.clearSelection();
        assertEquals(null, table.block());
    }

    @Test
    void copyGivesTabSeparatedCellsTopRowFirst() throws IOException {
        HistogramView view = panel(twoByTwo(), null).histogram();
        HistogramTable table = view.table;
        AtomicReference<String> copied = new AtomicReference<>();
        table.clipboard = copied::set;

        table.copy(false);
        assertEquals("\t0.85\n1.00\t\n", copied.get(), "the whole table: high MAP on top, empty cells blank");
        assertEquals("Copied 2 × 2 cells", view.selectedLabel.getText());

        table.select(0, 0);
        table.extendTo(1, 0);
        table.copy(true);
        assertEquals("\t1000\t3000\n30\t1.00\t\n", copied.get());
        assertEquals("Copied 2 × 1 cells with their axes", view.selectedLabel.getText());
    }

    // ---- step 9: tune table bins ----

    /** An .ini with a 4 × 3 VE table on RPMValue and veTableYAxisDisplay, logged as RPM and VE load. */
    private static IniFileModel ini() {
        IniFileModel ini = mock(IniFileModel.class);
        TableModel ve = new TableModel("veTableTbl", "veTableMap", "\"VE\"", "Help", "RPM", "Load",
                "veRpmBins", "RPMValue", false, "veLoadBins", "veTableYAxisDisplay", false, "veTable",
                "Up", "Down", 10.0, "normal");
        when(ini.getTables()).thenReturn(Collections.singletonMap("veTableTbl", ve));
        when(ini.findIniField("veRpmBins")).thenReturn(Optional.of(
                new ArrayIniField("veRpmBins", 0, FieldType.UINT16, 1, 4, "RPM", 1.0, "0", "8000", "0")));
        when(ini.findIniField("veLoadBins")).thenReturn(Optional.of(
                new ArrayIniField("veLoadBins", 8, FieldType.UINT16, 1, 3, "%", 1.0, "0", "100", "0")));
        when(ini.getDatalogEntries()).thenReturn(Arrays.asList(new DatalogEntry("RPMValue", "RPM"),
                new DatalogEntry("veTableYAxis", "VE load")));
        return ini;
    }

    /** RPM bins 800, 1500, 2500, 4000; load bins 2, 5, 10. */
    private static ConfigurationImage tune() {
        ConfigurationImage image = new ConfigurationImage(16);
        int[] values = {800, 1500, 2500, 4000, 2, 5, 10};
        for (int i = 0; i < values.length; i++) {
            FieldType.UINT16.writeRawValue(image.getByteBuffer(i * 2, 2), values[i]);
        }
        return image;
    }

    @Test
    void yDefaultsToTheVeTablesLoadSignal() throws IOException {
        HistogramView view = panel(new HistogramSettings(new Node()), ini()).histogram();
        assertEquals(1, view.field('x'));
        assertEquals(4, view.field('y'), "VE load, behind veTableYAxisDisplay, rather than MAP");
    }

    @Test
    void binsFromATuneTableLineUpWithItsCells() throws IOException {
        HistogramSettings settings = new HistogramSettings(new Node());
        settings.setSignal('y', "MAP");
        LogViewerPanel panel = panel(settings, ini());
        panel.setTuneSupplier(HistogramTimelineTest::tune);
        HistogramView view = panel.histogram();

        view.setBinsTable("veTableTbl");
        Histogram histogram = view.table.histogram();
        assertEquals(4, histogram.xAxis.size());
        assertEquals(2500, histogram.xAxis.center(2), 1e-9);
        assertEquals(10, histogram.yAxis.center(2), 1e-9);
        assertEquals("Bins: VE (4 × 3)  ▾", view.binsButton.getText());
        assertFalse(view.columnsSpinner.isEnabled());
        assertEquals(2, view.field('y'), "MAP was picked by hand and stays");
        assertTrue(view.isBinsNoticeShown(), "but the table's load axis is VE load");
        assertTrue(view.binsNoticeLabel.getText().contains("Y is MAP, but the VE's Y axis is VE load"),
                view.binsNoticeLabel.getText());

        view.assign('y', 4);
        assertFalse(view.isBinsNoticeShown());
        assertEquals("veTableTbl", settings.binsTable());

        view.setBinsTable(null);
        assertEquals(HistogramSettings.DEFAULT_BINS, view.table.histogram().xAxis.size(), "even bins again");
        assertTrue(view.columnsSpinner.isEnabled());
    }

    @Test
    void withoutATuneTheBinsStayEvenAndSayWhy() throws IOException {
        HistogramSettings settings = twoByTwo();
        settings.setBinsTable("veTableTbl");
        HistogramView view = panel(settings, ini()).histogram();
        assertEquals(2, view.table.histogram().xAxis.size());
        assertEquals("Bins: 2 × 2 (no tune)  ▾", view.binsButton.getText());
        assertTrue(view.isBinsNoticeShown());
        assertTrue(view.binsNoticeLabel.getText().startsWith("No tune"), view.binsNoticeLabel.getText());
        view.rebuildBinsPopup();
    }

    @Test
    void tableListPutsTheMainTablesFirst() {
        IniFileModel ini = ini();
        assertEquals(1, TuneTables.tables(ini).size());
        assertEquals("VE", TuneTables.title(TuneTables.tables(ini).get(0)));
        assertTrue(TuneTables.tables(null).isEmpty());
    }
}
