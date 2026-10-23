package com.rusefi.ui.logview;

import com.rusefi.core.preferences.storage.Node;
import com.rusefi.logview.CsvLogReader;
import org.junit.jupiter.api.Test;

import java.awt.image.BufferedImage;
import java.io.IOException;
import java.io.StringReader;
import java.util.Collections;
import java.util.List;

import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertFalse;
import static org.junit.jupiter.api.Assertions.assertNotEquals;
import static org.junit.jupiter.api.Assertions.assertNotNull;
import static org.junit.jupiter.api.Assertions.assertNull;
import static org.junit.jupiter.api.Assertions.assertTrue;

class HistogramViewTest {
    /**
     * 400 records 0.1 s apart: idle (1000 rpm, 30 kPa, lambda 1.0) then load (3000 rpm, 90 kPa, lambda 0.85), with
     * the wideband pegged at 0.68 in every 10th load record; TPS and CLT ride along.
     */
    private static CsvLogReader log() throws IOException {
        StringBuilder text = new StringBuilder("Time\tTPS\tRPM\tMAP\tLambda 1\tCLT\ns\t%\trpm\tkPa\tlambda\tC\n");
        for (int i = 0; i < 400; i++) {
            boolean load = i >= 200;
            // under load the readings barely vary but rarely repeat, like a real wideband
            double lambda = load ? (i % 10 == 0 ? 0.68 : 0.85 + (i % 97) * 0.00005) : 1.0 + (i % 5) * 0.001;
            text.append(i / 10.0).append('\t').append(load ? 80 : 2).append('\t').append(load ? 3000 : 1000)
                    .append('\t').append(load ? 90 : 30).append('\t').append(lambda).append('\t').append(70 + i / 40)
                    .append('\n');
        }
        return CsvLogReader.read(new StringReader(text.toString()));
    }

    private static LogViewerPanel histogramPanel(HistogramSettings settings) throws IOException {
        LogViewerPanel panel = new LogViewerPanel(log(), "test.msl");
        panel.chart().plotLayout().clearPlot(0); // the test starts from an empty plot
        panel.setHistogramSettings(settings);
        panel.setHistogramView(true);
        return panel;
    }

    private static void paint(HistogramTable table) {
        table.setSize(900, 600);
        table.paint(new BufferedImage(900, 600, BufferedImage.TYPE_INT_RGB).createGraphics());
    }

    @Test
    void opensOnRpmMapAndLambdaAndSwitchesBack() throws IOException {
        LogViewerPanel panel = histogramPanel(new HistogramSettings());
        HistogramView view = panel.histogram();
        assertTrue(panel.isHistogramView());
        assertFalse(panel.statusBar().isVisible(), "the histogram has its own status strip");
        assertEquals(2, view.field('x'));
        assertEquals(3, view.field('y'));
        assertEquals(4, view.field('z'));
        assertEquals('z', view.activeAxis(), "a sidebar click goes to Z first");

        panel.setHistogramView(false);
        assertFalse(panel.isHistogramView());
        assertTrue(panel.statusBar().isVisible());
    }

    @Test
    void sidebarClickPutsTheSignalOnTheActiveAxisAndSwapsDuplicates() throws IOException {
        HistogramSettings settings = new HistogramSettings(new Node());
        LogViewerPanel panel = histogramPanel(settings);
        HistogramView view = panel.histogram();
        view.assignToActiveAxis(5);
        assertEquals(5, view.field('z'));
        assertEquals("CLT", settings.signal('z'));
        assertEquals('z', view.axisOf(5));

        view.setActiveAxis('y');
        view.assignToActiveAxis(2);
        assertEquals(2, view.field('y'), "RPM moves from X to Y");
        assertEquals(3, view.field('x'), "and MAP takes its place on X");

        view.swapXY();
        assertEquals(2, view.field('x'));
        assertEquals(3, view.field('y'));
        assertEquals(0, view.axisOf(1));
    }

    @Test
    void cellsShowTheLambdaAtIdleAndUnderLoad() throws IOException {
        HistogramSettings settings = new HistogramSettings();
        settings.setColumns(2);
        settings.setRows(2);
        HistogramView view = histogramPanel(settings).histogram();
        HistogramTable table = view.table;
        assertEquals("1.00", table.text(0, 0));
        assertEquals("", table.text(1, 0), "no records at 3000 rpm and 30 kPa");
        assertTrue(table.position(table.histogram().cell(1, 1)) < table.position(table.histogram().cell(0, 0)),
                "lambda 0.85 sits lower on the palette than 1.00");

        view.modeButtons[HistogramSettings.Mode.MIN.ordinal()].doClick();
        assertEquals("0.68", table.text(1, 1), "the saturated readings set the minimum");
        view.afrButton.doClick();
        assertEquals("10.0", table.text(1, 1));
        view.modeButtons[HistogramSettings.Mode.TIME.ordinal()].doClick();
        assertEquals("20", table.text(1, 1), "200 records 0.1 s apart");
        view.modeButtons[HistogramSettings.Mode.COUNT.ordinal()].doClick();
        assertEquals("200", table.text(1, 1));
        paint(table);
    }

    @Test
    void saturationWarningAddsAFilterOrGoesAway() throws IOException {
        HistogramSettings settings = new HistogramSettings();
        settings.setColumns(2);
        settings.setRows(2);
        settings.setMode(HistogramSettings.Mode.MIN);
        HistogramView view = histogramPanel(settings).histogram();
        assertTrue(view.isSaturationWarningShown());
        assertTrue(view.bannerText.getText().contains("0.680 in 20 records"), view.bannerText.getText());
        assertTrue(view.banner.getToolTipText().endsWith("not real readings."), "the whole message, when cut short");
        assertEquals(view.excludeButton.getPreferredSize(), view.excludeButton.getMinimumSize(),
                "no layout shortens the button's label");

        view.excludeButton.doClick();
        assertFalse(view.isSaturationWarningShown());
        assertEquals(Collections.singletonList(HistogramFilter.of("Lambda 1", HistogramFilter.Condition.AT_MOST, 0.68)),
                view.filters());
        assertEquals(1, settings.filters().size(), "filters are remembered");
        assertEquals("0.85", view.table.text(1, 1));
        assertEquals("20 hidden by filters", view.hiddenLabel.getText());

        view.toggleFilter(0);
        assertEquals("0.68", view.table.text(1, 1), "a filter switched off hides nothing");
        assertFalse(view.isSaturationWarningShown(), "the switched-off filter still answers the warning");

        view.removeFilter(0);
        assertTrue(view.isSaturationWarningShown());
        view.keepButton.doClick();
        assertFalse(view.isSaturationWarningShown());
    }

    @Test
    void addAndEditFiltersThroughThePrompt() throws IOException {
        HistogramView view = histogramPanel(new HistogramSettings()).histogram();
        HistogramFilter throttle = new HistogramFilter("TPS", HistogramFilter.Condition.CHANGING, 100, Double.NaN,
                0.3, true);
        view.strip.filterPrompt = (title, initial) -> {
            assertNull(initial);
            return throttle;
        };
        view.addFilterButton.doClick();
        assertEquals(Collections.singletonList(throttle), view.filters());
        // the chip row: title, the chip, Add filter
        assertEquals(3, view.filterRow.getComponentCount());

        view.strip.filterPrompt = (title, initial) -> {
            assertEquals(throttle, initial);
            return HistogramFilter.of("CLT", HistogramFilter.Condition.AT_MOST, 70);
        };
        view.editFilter(0);
        assertEquals("CLT", view.filters().get(0).signal);
        view.strip.filterPrompt = (title, initial) -> null;
        view.editFilter(0);
        assertEquals("CLT", view.filters().get(0).signal, "cancel keeps the filter");
    }

    @Test
    void editorBuildsTheFilterAndCountsWhatItHides() throws IOException {
        CsvLogReader log = log();
        HistogramFilterEditor editor = new HistogramFilterEditor(log, null, 2, 3, 4);
        assertEquals("Pick a signal", editor.preview.getText());
        assertTrue(editor.selectSignal("RPM"));
        editor.condition.setSelectedItem(HistogramFilter.Condition.BETWEEN);
        editor.value.setText("2500");
        editor.value2.setText("3500");
        assertEquals("Hides 200 records (50.0% of the log)", editor.preview.getText());
        HistogramFilter filter = editor.filter();
        assertNotNull(filter);
        assertEquals("RPM between 2500–3500", filter.label());
        editor.value.setText("x");
        assertNull(editor.filter());
    }

    @Test
    void editorListsTheHistogramSignalsFirstAndNarrowsAsYouType() throws IOException {
        HistogramFilterEditor editor = new HistogramFilterEditor(log(), null, 2, 3, 4);
        javax.swing.ListModel<Integer> list = editor.signalList.getModel();
        assertEquals(5, list.getSize());
        assertEquals(2, (int) list.getElementAt(0), "X, Y and Z first");
        assertEquals(3, (int) list.getElementAt(1));
        assertEquals(4, (int) list.getElementAt(2));
        assertEquals(1, (int) list.getElementAt(3), "then the usual filter signals: TPS, CLT");
        assertEquals(5, (int) list.getElementAt(4));

        editor.search.setText("cl");
        assertEquals(1, list.getSize());
        assertEquals(5, (int) list.getElementAt(0));
        assertTrue(editor.selectSignal("RPM"), "picking a hidden signal clears the search");
        assertEquals("", editor.search.getText());
        assertEquals("In this log: 1000 to 3000 rpm", editor.range.getText());

        CsvLogReader withConstant = CsvLogReader.read(new StringReader(
                "Time\tRPM\tGear\ns\trpm\t\n0\t1000\t3\n1\t2000\t3\n"));
        HistogramFilterEditor constant = new HistogramFilterEditor(withConstant, null);
        assertEquals(1, constant.signalList.getModel().getSize(), "a signal that never changes cannot filter");
        assertFalse(constant.selectSignal("Gear"));
    }

    @Test
    void quickFiltersFillTheFormForTheUsualCases() throws IOException {
        CsvLogReader log = log();
        HistogramFilterEditor editor = new HistogramFilterEditor(log, null, 2, 3, 4);
        List<String> titles = new java.util.ArrayList<>();
        for (HistogramFilterEditor.QuickFilter quick : editor.quickFilters) {
            titles.add(quick.title);
        }
        assertEquals(java.util.Arrays.asList("Saturated Lambda 1", "Warm-up", "Throttle transients",
                "Engine off / cranking"), titles, "no fuel cut: the log has no dfcoActive");

        editor.fill(editor.quickFilters.get(1).filter);
        assertEquals(HistogramFilter.of("CLT", HistogramFilter.Condition.AT_MOST, 70), editor.filter());
        assertEquals("In this log: 70 to 79 C", editor.range.getText());
        assertTrue(editor.preview.getText().startsWith("Hides 40 records"), editor.preview.getText());

        editor.fill(editor.quickFilters.get(2).filter);
        HistogramFilter throttle = editor.filter();
        assertEquals(HistogramFilter.Condition.CHANGING, throttle.condition);
        assertEquals(0.3, throttle.holdSeconds, 1e-9);
    }

    @Test
    void editingAFilterWhoseSignalIsMissingKeepsIt() throws IOException {
        HistogramFilter boost = HistogramFilter.of("Boost", HistogramFilter.Condition.AT_LEAST, 150);
        HistogramFilterEditor editor = new HistogramFilterEditor(log(), boost, 2, 3, 4);
        assertEquals("Boost is not in this log", editor.range.getText());
        assertEquals(boost, editor.filter());
    }

    @Test
    void cellPanelFollowsTheSelectionAndCloses() throws IOException {
        HistogramSettings settings = new HistogramSettings();
        settings.setColumns(2);
        settings.setRows(2);
        HistogramView view = histogramPanel(settings).histogram();
        assertTrue(view.isCellPanelVisible());
        assertFalse(view.cellPanel.isShowingCell());

        view.table.select(1, 1);
        assertTrue(view.cellPanel.isShowingCell());
        assertTrue(view.cellPanel.titleLabel.getText().startsWith("3000 rpm"), view.cellPanel.titleLabel.getText());
        assertEquals("20.0 s", view.cellPanel.statValues[4].getText());
        assertEquals("200", view.cellPanel.statValues[5].getText());
        assertEquals("Visited 1 time  ·  longest stay 20.0 s", view.cellPanel.visitsLabel.getText());
        assertTrue(view.selectedLabel.getText().startsWith("Selected RPM 3000"), view.selectedLabel.getText());

        view.cellPanel.closeButton.doClick();
        assertFalse(view.isCellPanelVisible());
        assertFalse(view.cellButton.isSelected());
        assertFalse(settings.isCellPanelVisible());
        view.cellButton.doClick();
        assertTrue(view.isCellPanelVisible());
    }

    @Test
    void thinCellsAreHatchedOrHidden() throws IOException {
        HistogramSettings settings = new HistogramSettings();
        settings.setColumns(2);
        settings.setRows(2);
        settings.setMinTime(30);
        HistogramView view = histogramPanel(settings).histogram();
        HistogramTable table = view.table;
        assertTrue(table.isThin(table.histogram().cell(0, 0)), "20 s is under 30 s");
        assertEquals("1.00", table.text(0, 0), "a thin cell still shows its value");
        view.hideThinBox.doClick();
        assertEquals("", table.text(0, 0));
        paint(table);
    }

    @Test
    void betweenCursorsBinsOnlyTheTimeBetweenAAndB() throws IOException {
        HistogramSettings settings = new HistogramSettings();
        settings.setColumns(2);
        settings.setRows(2);
        LogViewerPanel panel = histogramPanel(settings);
        HistogramView view = panel.histogram();
        assertFalse(view.betweenCursorsButton.isEnabled(), "A and B are not set");

        MeasureCursors cursors = panel.chart().cursors();
        cursors.set(MeasureCursors.Which.A, 25);
        cursors.set(MeasureCursors.Which.B, 34.9);
        assertTrue(view.betweenCursorsButton.isEnabled());
        view.betweenCursorsButton.doClick();
        assertEquals("100 records used", view.recordsLabel.getText());

        cursors.clear();
        assertTrue(view.wholeLogButton.isSelected(), "clearing A and B goes back to the whole log");
        assertEquals("400 records used", view.recordsLabel.getText());
    }

    @Test
    void sidebarListsTheHistogramSignalsInsteadOfThePlotted() throws IOException {
        LogViewerPanel panel = new LogViewerPanel(log(), "test.msl");
        panel.chart().plotLayout().clearPlot(0); // the test starts from an empty plot
        panel.toggle(1);
        assertEquals(SignalSections.plotTitle(0), panel.fieldList().getModel().getElementAt(0).section);
        assertTrue(panel.plottedLabel().isVisible());

        panel.setHistogramView(true);
        javax.swing.ListModel<SignalSections.Row> rows = panel.fieldList().getModel();
        assertEquals(SignalSections.HISTOGRAM, rows.getElementAt(0).section);
        assertEquals(3, rows.getElementAt(0).count);
        assertEquals(2, rows.getElementAt(1).field, "X");
        assertEquals(3, rows.getElementAt(2).field, "Y");
        assertEquals(4, rows.getElementAt(3).field, "Z");
        assertFalse(panel.plottedLabel().isVisible());

        panel.histogram().assignToActiveAxis(5);
        assertEquals(5, panel.fieldList().getModel().getElementAt(3).field, "the section follows the axes");

        panel.setHistogramView(false);
        assertEquals(SignalSections.plotTitle(0), panel.fieldList().getModel().getElementAt(0).section);
        assertEquals(1, panel.fieldList().getModel().getElementAt(1).field, "the plotted signal is back");
    }

    @Test
    void colourSettingsAreKeptPerZSignal() throws IOException {
        HistogramSettings settings = new HistogramSettings(new Node());
        HistogramView view = histogramPanel(settings).histogram();
        assertEquals(HistogramColors.Palette.HEATMAP, view.colors().palette(), "the heatmap is the default");

        view.rebuildColorsPanel();
        view.colorsPanel.paletteButtons[HistogramColors.Palette.VIRIDIS.ordinal()].doClick();
        assertEquals(HistogramColors.Palette.VIRIDIS, settings.colors("Lambda 1").palette());
        assertEquals(HistogramColors.Palette.VIRIDIS, view.colors().palette());

        view.assignToActiveAxis(5);
        assertEquals(HistogramColors.Palette.HEATMAP, view.colors().palette(), "CLT has its own colours");
        view.assignToActiveAxis(4);
        assertEquals(HistogramColors.Palette.VIRIDIS, view.colors().palette());

        view.rebuildColorsPanel();
        view.colorsPanel.resetButton.doClick();
        assertEquals(HistogramColors.Palette.HEATMAP, settings.colors("Lambda 1").palette());
    }

    @Test
    void manualRangeStartsFromTheColoursOnScreenInTheShownUnit() throws IOException {
        HistogramSettings settings = new HistogramSettings(new Node());
        settings.setColumns(2);
        settings.setRows(2);
        settings.setShowAfr(true);
        HistogramView view = histogramPanel(settings).histogram();
        double low = view.table.scaleLow();
        double high = view.table.scaleHigh();

        view.rebuildColorsPanel();
        view.colorsPanel.rangeButtons[HistogramColors.Range.MANUAL.ordinal()].doClick();
        HistogramColors colors = settings.colors("Lambda 1");
        assertEquals(HistogramColors.Range.MANUAL, colors.range());
        assertEquals(low / HistogramSignals.STOICH_AFR, colors.manualLow(), 1e-9, "kept as lambda, the logged unit");
        assertEquals(high, view.table.scaleHigh(), 1e-9);

        view.colorsPanel.lowSpinner.setValue(13.0);
        view.colorsPanel.highSpinner.setValue(16.0);
        assertEquals(13.0, view.table.scaleLow(), 1e-9);
        assertEquals(16.0 / HistogramSignals.STOICH_AFR, colors.manualHigh(), 1e-9);
        assertEquals("≤ 13.0", view.table.legendLabels()[0], "the load cell (12.3) is richer than the range");
        assertEquals("16.0", view.table.legendLabels()[1]);
    }

    @Test
    void divergingCentresOnStoichiometricOrTheEnteredValue() throws IOException {
        HistogramSettings settings = new HistogramSettings(new Node());
        settings.setColumns(2);
        settings.setRows(2);
        HistogramView view = histogramPanel(settings).histogram();
        view.rebuildColorsPanel();
        view.colorsPanel.paletteButtons[HistogramColors.Palette.DIVERGING.ordinal()].doClick();
        HistogramTable table = view.table;
        assertEquals(1.0, table.scaleCentre(), 1e-9);
        assertEquals(1.0, (table.scaleLow() + table.scaleHigh()) / 2, 1e-9, "the same span on both sides");
        assertTrue(table.legendLabels()[0].endsWith("rich"));

        view.colorsPanel.autoCentreBox.doClick();
        view.colorsPanel.centreSpinner.setValue(0.9);
        assertEquals(0.9, table.scaleCentre(), 1e-9);
        assertEquals(0.5, table.position(0.9), 1e-9);

        view.rebuildColorsPanel();
        view.colorsPanel.reverseBox.doClick();
        assertTrue(table.legendLabels()[0].endsWith("lean"));
        assertTrue(table.position(0.85) > 0.5, "reversed: rich is at the high end");
    }

    private static HistogramView viewOf(String header) throws IOException {
        String[] names = header.split("\t");
        StringBuilder text = new StringBuilder("Time\t" + header + "\ns");
        for (String ignored : names) {
            text.append('\t');
        }
        text.append('\n');
        for (int i = 0; i < 20; i++) {
            text.append(i);
            for (int f = 0; f < names.length; f++) {
                text.append('\t').append(i * (f + 1));
            }
            text.append('\n');
        }
        LogViewerPanel panel = new LogViewerPanel(CsvLogReader.read(new StringReader(text.toString())), null);
        panel.chart().plotLayout().clearPlot(0); // the test starts from an empty plot
        panel.setHistogramView(true);
        return panel.histogram();
    }

    @Test
    void zIsTheMeasuredMixtureNeverATarget() throws IOException {
        HistogramView view = viewOf("RPM\tMAP\tFuel: target AFR\tLambda 2");
        assertEquals(4, view.field('z'), "Lambda 2, not the target before it");
        assertFalse(view.mixtureMissingLabel.isVisible());

        HistogramView noLambda = viewOf("RPM\tMAP\tFuel: target AFR\tCLT");
        assertNotEquals(3, noLambda.field('z'));
        assertTrue(noLambda.isMixtureMissing());
        assertTrue(noLambda.mixtureMissingLabel.isVisible());

        noLambda.assign('z', 3);
        assertEquals(3, noLambda.field('z'), "the user may still pick the target");
        assertFalse(noLambda.mixtureMissingLabel.isVisible());
    }

    @Test
    void onlyTheUsersPicksAreRemembered() throws IOException {
        HistogramSettings settings = new HistogramSettings(new Node());
        HistogramView view = histogramPanel(settings).histogram();
        assertNull(settings.signal('x'), "automatic picks are not saved");
        assertNull(settings.signal('z'));

        view.assign('z', 5);
        assertEquals("CLT", settings.signal('z'));
        assertNull(settings.signal('x'));
        assertNull(settings.signal('y'));

        view.swapXY();
        assertEquals("MAP", settings.signal('x'), "a swap is a choice for both axes");
        assertEquals("RPM", settings.signal('y'));
    }

    @Test
    void settingsCarryOverToTheNextLog() throws IOException {
        Node node = new Node();
        HistogramSettings settings = new HistogramSettings(node);
        LogViewerPanel first = histogramPanel(settings);
        first.histogram().assignToActiveAxis(5);
        first.histogram().modeButtons[HistogramSettings.Mode.TIME.ordinal()].doClick();

        HistogramSettings reloaded = new HistogramSettings(node);
        assertTrue(reloaded.isHistogramView());
        assertEquals(HistogramSettings.Mode.TIME, reloaded.mode());
        HistogramView second = histogramPanel(reloaded).histogram();
        assertEquals(5, second.field('z'), "CLT stays on Z");
        assertTrue(second.modeButtons[HistogramSettings.Mode.TIME.ordinal()].isSelected());
    }
}
