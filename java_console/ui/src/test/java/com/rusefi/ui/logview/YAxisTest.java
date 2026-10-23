package com.rusefi.ui.logview;

import com.rusefi.logview.CsvLogReader;
import org.junit.jupiter.api.Test;

import java.awt.image.BufferedImage;
import java.io.IOException;
import java.io.StringReader;
import java.util.ArrayList;
import java.util.List;

import static org.junit.jupiter.api.Assertions.assertArrayEquals;
import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertTrue;

/** The y axis: which signal's scale it shows, its ticks, and how signals are scaled. */
class YAxisTest {
    private static final int WIDTH = 800;
    private static final int HEIGHT = 500;
    static final int RPM = 1;
    static final int TPS = 2;
    static final int CLT = 3;
    static final int IAT = 4;

    /** Time 0..10 s: RPM 1000..6000, TPS 0..100 %, CLT 60..90 C, IAT 20..40 C. */
    static CsvLogReader log() throws IOException {
        StringBuilder text = new StringBuilder("Time\tRPM\tTPS\tCLT\tIAT\ns\trpm\t%\tC\tC\n");
        for (int i = 0; i <= 100; i++) {
            text.append(i / 10.0).append('\t').append(1000 + i * 50).append('\t').append(i)
                    .append('\t').append(60 + i * 0.3).append('\t').append(20 + i * 0.2).append('\n');
        }
        return CsvLogReader.read(new StringReader(text.toString()));
    }

    static LogChart chart(int... fields) throws IOException {
        PlotLayout layout = new PlotLayout(5);
        for (int field : fields) {
            layout.add(field);
        }
        LogChart chart = new LogChart(log(), layout, new ChartSettings());
        paint(chart);
        return chart;
    }

    static void paint(LogChart chart) {
        chart.setSize(WIDTH, HEIGHT);
        chart.paint(new BufferedImage(WIDTH, HEIGHT, BufferedImage.TYPE_INT_RGB).createGraphics());
    }

    static List<Double> tickValues(LogChart chart) {
        List<Double> values = new ArrayList<>();
        for (double[] tick : chart.axisTicks(chart.bands().get(0))) {
            values.add(tick[0]);
        }
        return values;
    }

    @Test
    void twoUnitsGetALeftAndARightAxis() throws IOException {
        LogChart chart = chart(RPM, TPS);
        LogChart.Band band = chart.bands().get(0);
        assertEquals(RPM, chart.leftAxisField(band), "the first unit on the left");
        assertEquals(TPS, chart.rightAxisField(band), "the second on the right");
        assertEquals(List.of(0.0, 20.0, 40.0, 60.0, 80.0, 100.0), values(chart.rightAxisTicks(band)));
        chart.focus().hover(TPS);
        assertEquals(RPM, chart.leftAxisField(band), "the left axis keeps its unit");
        assertEquals(TPS, chart.rightAxisField(band));
    }

    @Test
    void withThreeUnitsTheRightAxisFollowsTheHighlightedTrace() throws IOException {
        LogChart chart = chart(RPM, TPS, CLT);
        LogChart.Band band = chart.bands().get(0);
        assertEquals(TPS, chart.rightAxisField(band));
        chart.focus().hover(CLT);
        assertEquals(CLT, chart.rightAxisField(band));
        assertEquals(RPM, chart.leftAxisField(band));
        chart.focus().clearHover();
        chart.focus().togglePin(CLT);
        assertEquals(CLT, chart.rightAxisField(band), "or the pinned one");
    }

    @Test
    void oneUnitHasNoRightAxis() throws IOException {
        LogChart chart = chart(CLT, IAT);
        LogChart.Band band = chart.bands().get(0);
        assertEquals(-1, chart.rightAxisField(band));
        assertTrue(chart.rightAxisTicks(band).isEmpty());
        chart.focus().hover(IAT);
        assertEquals(IAT, chart.leftAxisField(band), "the highlighted trace of the left unit");
    }

    private static List<Double> values(List<double[]> ticks) {
        List<Double> values = new ArrayList<>();
        for (double[] tick : ticks) {
            values.add(tick[0]);
        }
        return values;
    }

    @Test
    void ticksAreRoundValuesAcrossTheScale() throws IOException {
        LogChart chart = chart(RPM);
        // 1000..6000 over 4 divisions: 1250 is nearest to a step of 1000
        assertEquals(List.of(1000.0, 2000.0, 3000.0, 4000.0, 5000.0, 6000.0), tickValues(chart));
        double[] top = chart.axisTicks(chart.bands().get(0)).get(5);
        assertEquals(chart.bands().get(0).traces.y, top[1], 1e-9, "the maximum is at the top of the plot");
    }

    @Test
    void nearestNiceSteps() {
        assertEquals(1000, LogChart.nearestNiceStep(1250), 1e-9);
        assertEquals(2000, LogChart.nearestNiceStep(1600), 1e-9);
        assertEquals(5, LogChart.nearestNiceStep(4), 1e-9);
        assertEquals(0.1, LogChart.nearestNiceStep(0.12), 1e-12);
    }

    @Test
    void aShortPlotGetsFewerTicks() throws IOException {
        PlotLayout layout = new PlotLayout(5);
        layout.add(RPM);
        LogChart chart = new LogChart(log(), layout, new ChartSettings());
        chart.setSize(WIDTH, 150);
        chart.paint(new BufferedImage(WIDTH, 150, BufferedImage.TYPE_INT_RGB).createGraphics());
        List<double[]> ticks = chart.axisTicks(chart.bands().get(0));
        for (int i = 1; i < ticks.size(); i++) {
            assertTrue(Math.abs(ticks.get(i)[1] - ticks.get(i - 1)[1]) >= 20, "labels stay apart");
        }
    }

    @Test
    void theAxisOfAnotherPlotIsNotTakenOver() throws IOException {
        PlotLayout layout = new PlotLayout(5);
        layout.add(RPM);
        layout.addToNewPlot(TPS);
        LogChart chart = new LogChart(log(), layout, new ChartSettings());
        paint(chart);
        chart.focus().hover(TPS);
        assertEquals(RPM, chart.leftAxisField(chart.bands().get(0)), "TPS is in the second plot");
        assertEquals(-1, chart.rightAxisField(chart.bands().get(0)));
        assertEquals(TPS, chart.leftAxisField(chart.bands().get(1)));
    }

    @Test
    void anEmptyPlotHasNoAxis() throws IOException {
        LogChart chart = chart();
        assertEquals(-1, chart.leftAxisField(chart.bands().get(0)));
        assertTrue(chart.axisTicks(chart.bands().get(0)).isEmpty());
    }

    @Test
    void byDefaultASignalIsScaledToItsWholeLogRange() throws IOException {
        LogChart chart = chart(RPM, TPS);
        assertArrayEquals(new double[]{1000, 6000}, chart.range(RPM), 1e-9);
        assertArrayEquals(new double[]{0, 100}, chart.range(TPS), 1e-9);
    }

    @Test
    void signalsWithTheSameUnitShareAScale() throws IOException {
        LogChart chart = chart(RPM, CLT, IAT);
        assertArrayEquals(new double[]{20, 90}, chart.range(CLT), 1e-9, "CLT 60..90 and IAT 20..40, both in C");
        assertArrayEquals(new double[]{20, 90}, chart.range(IAT), 1e-9);
        assertArrayEquals(new double[]{1000, 6000}, chart.range(RPM), 1e-9, "other units keep their own");

        javax.swing.JCheckBoxMenuItem sameUnit = null;
        for (java.awt.Component item : chart.plotMenu(0).getComponents()) {
            if (item instanceof javax.swing.JCheckBoxMenuItem
                    && "Same unit, same scale".equals(((javax.swing.JCheckBoxMenuItem) item).getText())) {
                sameUnit = (javax.swing.JCheckBoxMenuItem) item;
            }
        }
        assertTrue(sameUnit != null && sameUnit.isSelected(), "on by default, in the plot menu");
        sameUnit.doClick();
        assertArrayEquals(new double[]{60, 90}, chart.range(CLT), 1e-9, "off: each signal its own range");
        assertArrayEquals(new double[]{20, 40}, chart.range(IAT), 1e-9);
    }

    @Test
    void sharingIsPerPlotAndFollowsMoves() throws IOException {
        PlotLayout layout = new PlotLayout(5);
        layout.add(CLT);
        layout.addToNewPlot(IAT);
        LogChart chart = new LogChart(log(), layout, new ChartSettings());
        paint(chart);
        assertArrayEquals(new double[]{60, 90}, chart.range(CLT), 1e-9, "IAT is in another plot");
        layout.setSameUnitShared(1, false);
        layout.move(1, -1);
        assertTrue(!layout.isSameUnitShared(0) && layout.isSameUnitShared(1), "the setting moves with its plot");
    }

    private static javax.swing.JMenuItem scaleItem(LogChart chart, int field, String prefix) {
        javax.swing.JMenu scale = chart.scaleSubmenu(field);
        for (java.awt.Component item : scale.getMenuComponents()) {
            if (item instanceof javax.swing.JMenuItem && ((javax.swing.JMenuItem) item).getText().startsWith(prefix)) {
                return (javax.swing.JMenuItem) item;
            }
        }
        throw new AssertionError("no " + prefix);
    }

    @Test
    void visibleTimeRescalesWithTheView() throws IOException {
        LogChart chart = chart(RPM);
        assertTrue(scaleItem(chart, RPM, "Whole log").isSelected(), "the default");
        scaleItem(chart, RPM, "Visible time").doClick();
        chart.timeWindow().show(2, 4);
        assertArrayEquals(new double[]{2000, 3000}, chart.range(RPM), 1e-9, "RPM between 2 and 4 s");
        assertTrue(scaleItem(chart, RPM, "Visible time").isSelected());

        scaleItem(chart, RPM, "Whole log").doClick();
        assertArrayEquals(new double[]{1000, 6000}, chart.range(RPM), 1e-9);
    }

    @Test
    void aCustomScaleIsFixedAndLeavesTheSharedScale() throws IOException {
        LogChart chart = chart(CLT, IAT);
        chart.customScalePrompt = (label, current) -> {
            assertArrayEquals(new double[]{20, 90}, current, 1e-9, "the dialog starts from the current scale");
            return new double[]{0, 120};
        };
        scaleItem(chart, CLT, "Custom").doClick();
        assertArrayEquals(new double[]{0, 120}, chart.range(CLT), 1e-9);
        assertArrayEquals(new double[]{20, 40}, chart.range(IAT), 1e-9, "IAT no longer shares with a fixed CLT");
        String custom = scaleItem(chart, CLT, "Custom").getText();
        assertTrue(custom.matches("Custom \\(0[.0]*–120[.0]*\\)…"), "the menu shows the range: " + custom);

        chart.customScalePrompt = (label, current) -> null;
        scaleItem(chart, CLT, "Whole log").doClick();
        scaleItem(chart, CLT, "Custom").doClick();
        assertTrue(scaleItem(chart, CLT, "Whole log").isSelected(), "cancelling keeps the previous choice");
    }

    @Test
    void choicesAreKeptByNameAndCorruptOnesAreIgnored() {
        com.rusefi.core.preferences.storage.Node node = new com.rusefi.core.preferences.storage.Node();
        new SignalScales(node).setCustom("Lambda", new double[]{0.7, 1.3});
        new SignalScales(node).setVisible("RPM");
        SignalScales reloaded = new SignalScales(node);
        assertEquals(SignalScales.Mode.CUSTOM, reloaded.mode("Lambda"));
        assertArrayEquals(new double[]{0.7, 1.3}, reloaded.custom("Lambda"), 1e-12);
        assertEquals(SignalScales.Mode.VISIBLE, reloaded.mode("RPM"));
        assertEquals(SignalScales.Mode.WHOLE_LOG, reloaded.mode("TPS"));
        node.setProperty("MAP", "custom:5:1");
        assertEquals(SignalScales.Mode.WHOLE_LOG, reloaded.mode("MAP"), "min above max is ignored");
    }

    @Test
    void theLegendAndSignalMenusOfferTheScale() throws IOException {
        LogViewerPanel panel = new LogViewerPanel(log(), null);
        panel.chart().plotLayout().clearPlot(0); // the test starts from an empty plot
        panel.toggle(RPM);
        boolean inLegend = false;
        for (java.awt.Component item : panel.chart().legendMenu(RPM).getComponents()) {
            inLegend |= item instanceof javax.swing.JMenu && "Scale".equals(((javax.swing.JMenu) item).getText());
        }
        assertTrue(inLegend);
        LogViewerPanelTest.selectSignal(panel, RPM);
        boolean inList = false;
        for (java.awt.Component item : panel.rowMenu(panel.fieldList().getSelectedValue()).getComponents()) {
            inList |= item instanceof javax.swing.JMenu && "Scale".equals(((javax.swing.JMenu) item).getText());
        }
        assertTrue(inList, "a plotted signal's right-click menu in the list too");
    }

    /** As in a real log: VBatt about 0.04 V and a Nernst voltage about 0.8 V must not share one scale. */
    @Test
    void farApartSignalsWithTheSameUnitKeepTheirOwnScales() throws IOException {
        StringBuilder text = new StringBuilder("Time\tVBatt\tNernst\ns\tV\tV\n");
        for (int i = 0; i <= 20; i++) {
            text.append(i).append('\t').append(0.040 + i * 0.0001).append('\t').append(0.70 + i * 0.01).append('\n');
        }
        PlotLayout layout = new PlotLayout(5);
        layout.add(1);
        layout.add(2);
        LogChart chart = new LogChart(CsvLogReader.read(new StringReader(text.toString())), layout, new ChartSettings());
        paint(chart);
        assertArrayEquals(new double[]{0.040, 0.042}, chart.range(1), 1e-9, "VBatt keeps its detail");
        assertArrayEquals(new double[]{0.70, 0.90}, chart.range(2), 1e-9);
    }

    @Test
    void comparableSignalsStillShare() throws IOException {
        LogChart chart = chart(CLT, IAT);
        assertEquals(1, chart.sharedClusters(List.of(CLT, IAT)).size(), "CLT 30 C and IAT 20 C wide: one scale");
    }

    @Test
    void theChartKeepsRoomForARightAxisOnlyWhenNeeded() throws IOException {
        LogChart single = chart(CLT, IAT);
        LogChart.Band band = single.bands().get(0);
        int singleRight = band.traces.x + band.traces.width;
        LogChart mixed = chart(RPM, TPS);
        LogChart.Band mixedBand = mixed.bands().get(0);
        assertTrue(mixedBand.traces.x + mixedBand.traces.width < singleRight, "a narrower plot leaves room on the right");
    }

    @Test
    void sameUnitOnDifferentScalesShowsTheAxisOnlyForTheHighlightedTrace() throws IOException {
        LogChart chart = chart(CLT, IAT);
        LogChart.Band band = chart.bands().get(0);
        assertEquals(CLT, chart.leftAxisField(band), "shared: the numbers fit both, always shown");

        chart.plotLayout().setSameUnitShared(0, false);
        assertEquals(-1, chart.leftAxisField(band), "separate scales: no numbers that would mislead");
        assertTrue(chart.axisTicks(band).isEmpty());
        chart.focus().hover(IAT);
        assertEquals(IAT, chart.leftAxisField(band), "the highlighted trace's scale");
        assertEquals(List.of(20.0, 25.0, 30.0, 35.0, 40.0), values(chart.axisTicks(band)));
        chart.focus().clearHover();
        assertEquals(-1, chart.leftAxisField(band));
    }

    @Test
    void withSeparateScalesOnlyTheHighlightedTraceHasAnAxisOnTheLeft() throws IOException {
        PlotLayout layout = new PlotLayout(5);
        layout.add(RPM);
        layout.add(TPS);
        layout.add(CLT);
        layout.setSameUnitShared(0, false);
        LogChart chart = new LogChart(log(), layout, new ChartSettings());
        paint(chart);
        LogChart.Band band = chart.bands().get(0);
        assertEquals(-1, chart.leftAxisField(band), "no axis until a trace is highlighted");
        assertEquals(-1, chart.rightAxisField(band));
        chart.focus().hover(CLT);
        assertEquals(CLT, chart.leftAxisField(band), "the highlighted trace, on the left whatever its unit");
        assertEquals(-1, chart.rightAxisField(band), "never a right axis");
        paint(chart);
        assertEquals(chart.bands().get(0).traces.x + chart.bands().get(0).traces.width,
                WIDTH - 22, "no room kept for a right axis");
    }

    /** VBatt about 0.04 V and a Nernst voltage about 0.8 V do not share: the axis names whose scale it shows. */
    @Test
    void theAxisTitleNamesTheSignalWhenItsUnitHasSeveralScales() throws IOException {
        StringBuilder text = new StringBuilder("Time\tVBatt\tNernst\ns\tV\tV\n");
        for (int i = 0; i <= 20; i++) {
            text.append(i).append('\t').append(0.040 + i * 0.0001).append('\t').append(0.70 + i * 0.01).append('\n');
        }
        PlotLayout layout = new PlotLayout(3);
        layout.add(1);
        layout.add(2);
        LogChart chart = new LogChart(CsvLogReader.read(new StringReader(text.toString())), layout, new ChartSettings());
        paint(chart);
        LogChart.Band band = chart.bands().get(0);
        assertEquals(1, chart.leftAxisField(band));
        assertEquals("VBatt · V", chart.axisTitle(band, 1));
        chart.focus().hover(2);
        assertEquals(2, chart.leftAxisField(band), "the highlighted V signal's scale");
        assertEquals("Nernst · V", chart.axisTitle(band, 2));

        LogChart shared = chart(CLT, IAT);
        assertEquals("C", shared.axisTitle(shared.bands().get(0), CLT), "one shared scale: just the unit");
    }

    private static javax.swing.JRadioButtonMenuItem axisItem(javax.swing.JMenu menu, String text) {
        for (java.awt.Component item : menu.getMenuComponents()) {
            if (item instanceof javax.swing.JRadioButtonMenuItem && text.equals(((javax.swing.JRadioButtonMenuItem) item).getText())) {
                return (javax.swing.JRadioButtonMenuItem) item;
            }
        }
        throw new AssertionError("no " + text);
    }

    @Test
    void theLeftAndRightAxisMenusFixTheUnits() throws IOException {
        LogChart chart = chart(RPM, TPS, CLT);
        LogChart.Band band = chart.bands().get(0);
        javax.swing.JMenu left = chart.axisMenu(0, true);
        assertTrue(axisItem(left, "Automatic").isSelected());
        axisItem(left, "C").doClick();
        assertEquals(CLT, chart.leftAxisField(band));
        assertEquals(RPM, chart.rightAxisField(band), "automatic right: the first other unit");

        javax.swing.JMenu right = chart.axisMenu(0, false);
        assertTrue(!axisItem(right, "C").isEnabled(), "the left unit cannot also be on the right");
        axisItem(right, "%").doClick();
        chart.focus().hover(RPM);
        assertEquals(TPS, chart.rightAxisField(band), "a chosen unit stays despite the highlight");
        axisItem(chart.axisMenu(0, false), "None").doClick();
        assertEquals(-1, chart.rightAxisField(band));

        chart.plotLayout().setSameUnitShared(0, false);
        assertTrue(!chart.axisMenu(0, true).isEnabled() && !chart.axisMenu(0, false).isEnabled(),
                "with separate scales the axis follows the highlighted trace");
    }

    @Test
    void hoveringAnAxisTellsWhoseScaleItIs() throws IOException {
        LogChart chart = chart(RPM, TPS);
        LogChart.Band band = chart.bands().get(0);
        int y = band.traces.y + band.traces.height / 2;
        assertEquals("Scale of RPM (rpm)", chart.getToolTipText(
                new java.awt.event.MouseEvent(chart, java.awt.event.MouseEvent.MOUSE_MOVED, 0, 0, 10, y, 0, false)));
        assertEquals("Scale of TPS (%)", chart.getToolTipText(
                new java.awt.event.MouseEvent(chart, java.awt.event.MouseEvent.MOUSE_MOVED, 0, 0, WIDTH - 10, y, 0, false)));
        assertEquals(null, chart.getToolTipText(new java.awt.event.MouseEvent(chart,
                java.awt.event.MouseEvent.MOUSE_MOVED, 0, 0, WIDTH / 2, y, 0, false)), "no tooltip over the traces");
    }
}
