package com.rusefi.ui.logview;

import com.rusefi.core.preferences.storage.Node;
import com.rusefi.logview.CsvLogReader;
import org.junit.jupiter.api.Test;

import javax.swing.SwingUtilities;
import java.awt.Point;
import java.awt.Rectangle;
import java.awt.image.BufferedImage;
import java.io.IOException;
import java.io.StringReader;
import java.util.Arrays;
import java.util.List;

import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertFalse;
import static org.junit.jupiter.api.Assertions.assertNotNull;
import static org.junit.jupiter.api.Assertions.assertNull;
import static org.junit.jupiter.api.Assertions.assertSame;
import static org.junit.jupiter.api.Assertions.assertTrue;

class ScatterViewTest {
    /**
     * 200 records 0.1 s apart: cruise (2000 rpm, 50 kPa) for the first half, then load (4000 rpm, 100 kPa) with
     * knock retard 2 in every 10th record. Lambda follows its target (1.0, then 0.85); oil pressure rises with RPM.
     */
    private static CsvLogReader log() throws IOException {
        StringBuilder text = new StringBuilder(
                "Time\tRPM\tMAP\tLambda 1\tFuel: target lambda\tKnock: Retard\tOil Pressure\tCLT\n"
                        + "s\trpm\tkPa\tlambda\tlambda\tdeg\tkPa\tC\n");
        for (int i = 0; i < 200; i++) {
            boolean load = i >= 100;
            double rpm = (load ? 4000 : 2000) + i % 7;
            text.append(i / 10.0).append('\t').append(rpm).append('\t').append(load ? 100 : 50).append('\t')
                    .append(load ? 0.86 : 1.01).append('\t').append(load ? 0.85 : 1.0).append('\t')
                    .append(load && i % 10 == 0 ? 2 : 0).append('\t').append(rpm / 20).append('\t')
                    .append(80 + i / 20).append('\n');
        }
        return CsvLogReader.read(new StringReader(text.toString()));
    }

    private static LogViewerPanel panel(HistogramSettings settings) throws IOException {
        LogViewerPanel panel = new LogViewerPanel(log(), "test.msl");
        panel.chart().plotLayout().clearPlot(0); // the test starts from an empty plot
        panel.setScatterSettings(settings);
        panel.setView(HistogramSettings.SCATTER);
        return panel;
    }

    private static void paint(ScatterPlot plot) {
        plot.setSize(900, 600);
        plot.paint(new BufferedImage(900, 600, BufferedImage.TYPE_INT_RGB).createGraphics());
    }

    @Test
    void collectsTheShownRecordsAndTheirStays() throws IOException {
        CsvLogReader log = log();
        boolean[] hidden = new boolean[log.sampleCount()];
        hidden[0] = true;
        ScatterData data = ScatterData.collect(log, 1, 2, 5, hidden, Double.NaN, Double.NaN);
        assertEquals(199, data.count);
        assertEquals(1, data.hiddenRecords);
        assertEquals(2000, data.xMin, 1e-6);
        assertEquals(4006, data.xMax, 1e-6);

        List<Integer> load = data.inBox(3500, 4500, 90, 110);
        assertEquals(100, load.size());
        List<double[]> stays = data.stays(load);
        assertEquals(1, stays.size(), "the load records follow each other");
        assertEquals(10, stays.get(0)[0], 1e-6);
        assertEquals(10, stays.get(0)[2], 1e-6);

        ScatterData between = ScatterData.collect(log, 1, 2, 5, new boolean[log.sampleCount()], 5, 9.9);
        assertEquals(50, between.count, "A–B: 5.0–9.9 s");
    }

    @Test
    void ticksAreRoundValuesAcrossTheRange() {
        assertEquals("[1000.0, 2000.0, 3000.0, 4000.0]",
                Arrays.toString(ScatterPlot.ticks(900, 4100, 400)));
        assertEquals("[0.8, 0.9, 1.0]", Arrays.toString(ScatterPlot.ticks(0.75, 1.02, 300)));
    }

    @Test
    void scatterIsTheThirdViewWithItsOwnAxesSection() throws IOException {
        HistogramSettings settings = new HistogramSettings(new Node());
        LogViewerPanel panel = panel(settings);
        assertEquals(HistogramSettings.SCATTER, panel.shownView());
        assertFalse(panel.statusBar().isVisible());
        ScatterView view = panel.scatter();
        assertEquals(1, view.strip().field('x'));
        assertEquals(2, view.strip().field('y'));
        assertEquals(3, view.strip().field('z'), "the measured lambda, not its target");
        assertEquals(SignalSections.SCATTER, panel.fieldList().getModel().getElementAt(0).section);

        view.strip().assignToActiveAxis(7);
        assertEquals("CLT", settings.signal('z'));
        assertEquals(7, panel.fieldList().getModel().getElementAt(3).field, "the section follows the axes");

        panel.setView(HistogramSettings.TIMELINE);
        assertTrue(panel.statusBar().isVisible());
        assertEquals(SignalSections.plotTitle(0), panel.fieldList().getModel().getElementAt(0).section);
    }

    @Test
    void recipesSetTheAxesColouringAndGuideLines() throws IOException {
        HistogramSettings settings = new HistogramSettings(new Node());
        ScatterView view = panel(settings).scatter();
        List<String> titles = new java.util.ArrayList<>();
        for (ScatterRecipes.Recipe recipe : view.availableRecipes()) {
            titles.add(recipe.title);
        }
        assertEquals(Arrays.asList("Where the engine ran", "Knock map", "Lambda vs target", "Oil pressure vs RPM"),
                titles, "no injector duty, vehicle speed or IAT in this log");

        view.apply(ScatterRecipes.byTitle("Lambda vs target"));
        assertEquals(4, view.strip().field('x'));
        assertEquals(3, view.strip().field('y'));
        assertEquals(1, view.strip().field('z'));
        assertEquals(ScatterPlot.ColourBy.Z, settings.colourBy());
        assertTrue(view.zButton.isSelected());
        assertEquals("Lambda vs target", view.activeRecipe().title);
        assertEquals("Recipe: Lambda vs target  ▾", view.recipesButton.getText());

        view.apply(ScatterRecipes.byTitle("Knock map"));
        assertEquals(5, view.strip().field('z'));
        assertEquals("[1, 2, 5]", Arrays.toString(new int[]{view.strip().field('x'), view.strip().field('y'),
                view.strip().field('z')}));
        assertFalse(view.strip().isSaturationWarningShown(), "no knock is 0, not a sensor limit");
        // the knock points (2) are on the scale, not the pale ones (0)
        assertEquals(2, view.plot().scaleLow(), 1e-6);

        view.strip().assign('z', 7);
        assertNull(view.activeRecipe(), "changing a signal leaves the recipe");
        assertEquals("Recipes  ▾", view.recipesButton.getText());
    }

    @Test
    void boxSelectionGoesToTheTimelineAndBack() throws IOException {
        LogViewerPanel panel = panel(new HistogramSettings(new Node()));
        ScatterView view = panel.scatter();
        view.plot().selectBox(3500, 4500, 90, 110);
        assertEquals(100, view.plot().selected().size());
        assertTrue(view.infoLabel.getText().startsWith("Selected 100 points (10.0 s)"), view.infoLabel.getText());

        view.showSelectionOnTimeline();
        assertEquals(HistogramSettings.TIMELINE, panel.shownView());
        VisitsBar bar = panel.visitsBar();
        assertTrue(bar.isVisible());
        assertEquals("Back to scatter", bar.backButton.getText());
        assertEquals(1, panel.chart().highlights().size());
        assertEquals(3, panel.chart().plotLayout().plotCount(), "X, Y and Z, one plot each");

        bar.backButton.doClick();
        assertEquals(HistogramSettings.SCATTER, panel.shownView());
    }

    @Test
    void aBoxBecomesAFilterOnBothSignals() throws IOException {
        HistogramSettings settings = new HistogramSettings(new Node());
        ScatterView view = panel(settings).scatter();
        view.plot().selectBox(3500, 4500, 90, 110);
        view.filterBox(false);
        HistogramFilter keep = view.strip().filters().get(0);
        assertEquals(HistogramFilter.Condition.OUTSIDE_BOX, keep.condition);
        assertEquals("Only RPM 3500–4500 and MAP 90–110", keep.label());
        assertEquals(100, view.plot().data().count, "only the load records are left");
        assertEquals(keep, settings.filters().get(0), "saved, with both signals");

        view.strip().removeFilter(0);
        view.plot().selectBox(3500, 4500, 90, 110);
        view.filterBox(true);
        assertEquals(HistogramFilter.Condition.IN_BOX, view.strip().filters().get(0).condition);
        assertEquals(100, view.plot().data().count, "the load records are hidden");
        assertEquals(100, view.plot().data().hiddenRecords);
    }

    @Test
    void colourByTimeRunsFromTheFirstPointToTheLast() throws IOException {
        ScatterView view = panel(new HistogramSettings(new Node())).scatter();
        view.timeButton.doClick();
        ScatterPlot plot = view.plot();
        assertEquals(ScatterPlot.ColourBy.TIME, plot.colourBy());
        assertEquals(0, plot.scaleLow(), 1e-6);
        assertEquals(19.9, plot.scaleHigh(), 1e-6);
        assertEquals("[0.0 s, 19.9 s]", Arrays.toString(plot.legendLabels()));
        assertFalse(plot.showsValues(), "no manual range for time");
        paint(plot);
    }

    @Test
    void twoPlotsSideBySideEachWithItsOwnSignals() throws IOException {
        HistogramSettings settings = new HistogramSettings(new Node());
        LogViewerPanel panel = panel(settings);
        ScatterView view = panel.scatter();
        view.twoPlotsButton.doClick();
        assertEquals(2, view.plots());
        assertEquals(2, settings.plots());
        ScatterPane second = view.second();
        assertNotNull(second);
        assertSame(view.first, view.active(), "the first stays active until the second is used");

        view.activate(second);
        assertSame(second.strip, view.strip());
        assertSame(second.strip, panel.activeStrip(), "the sidebar acts on the active plot");
        view.apply(ScatterRecipes.byTitle("Oil pressure vs RPM"));
        assertEquals(6, second.strip.field('y'));
        assertEquals(3, view.first.strip.field('z'), "the first plot keeps its signals");
        assertEquals("Oil Pressure", settings.child("second").signal('y'), "saved apart");
        assertEquals("Recipe: Oil pressure vs RPM  ▾", view.recipesButton.getText());

        view.activate(view.first);
        assertEquals("Recipes  ▾", view.recipesButton.getText(), "the toolbar shows the active plot's");
        view.onePlotButton.doClick();
        assertEquals(1, view.plots());
        assertSame(view.first, view.active());
    }

    /** Lays the view out at a size, all the way down, as a window would. */
    private static void layOut(java.awt.Container container, int width, int height) {
        container.setSize(width, height);
        layOutTree(container);
    }

    private static void layOutTree(java.awt.Container container) {
        container.doLayout();
        for (java.awt.Component child : container.getComponents()) {
            if (child instanceof java.awt.Container) {
                layOutTree((java.awt.Container) child);
            }
        }
    }

    @Test
    void twoPlotsSitSideBySideEvenAfterGoingBackToOne() throws IOException {
        ScatterView view = panel(new HistogramSettings(new Node())).scatter();
        // start on one plot (the default), go to two, back to one, and to two again
        view.twoPlotsButton.doClick();
        view.onePlotButton.doClick();
        view.twoPlotsButton.doClick();
        layOut(view, 1600, 900);
        Rectangle left = SwingUtilities.convertRectangle(view.first.getParent(), view.first.getBounds(), view);
        Rectangle right = SwingUtilities.convertRectangle(view.second().getParent(), view.second().getBounds(), view);
        assertTrue(left.width > 600 && right.width > 600, left + " " + right);
        assertTrue(left.x + left.width <= right.x, "side by side, not on top of each other: " + left + " " + right);
        assertEquals(left.y, right.y);
        javax.swing.JSplitPane split = (javax.swing.JSplitPane) view.first.getParent();
        assertSame(split, ((javax.swing.plaf.basic.BasicSplitPaneUI) split.getUI()).getDivider().getParent(),
                "the divider between them is still there to drag");
        assertEquals("Side by side", view.twoPlotsButton.getText());
    }

    /** The index in its toolbar of the toolbar item holding a component. */
    private static int toolbarIndex(java.awt.Component component, java.awt.Container toolbar) {
        java.awt.Component item = component;
        while (item.getParent() != toolbar) {
            item = item.getParent();
        }
        return java.util.Arrays.asList(toolbar.getComponents()).indexOf(item);
    }

    /** The toolbar item that is the Timeline / Histogram / Scatter switch. */
    private static int viewSwitchIndex(java.awt.Container toolbar) {
        java.awt.Component[] items = toolbar.getComponents();
        for (int i = 0; i < items.length; i++) {
            if (items[i] instanceof com.rusefi.ui.widgets.SegmentedControl
                    && "Timeline".equals(((javax.swing.AbstractButton) ((java.awt.Container) items[i]).getComponent(0)).getText())) {
                return i;
            }
        }
        return -1;
    }

    @Test
    void lightDarkSitsRightAfterTheViewSwitchInBothToolbars() throws IOException {
        LogViewerPanel panel = panel(new HistogramSettings(new Node()));
        ChartToolbar timeline = panel.toolbar();
        java.awt.Container scatter = (java.awt.Container) panel.scatter().getComponent(0);
        int timelineSwitch = viewSwitchIndex(timeline);
        int scatterSwitch = viewSwitchIndex(scatter);
        assertTrue(timelineSwitch >= 0 && scatterSwitch >= 0);
        // the switch, a gap, then Plot: Light | Dark
        assertEquals(timelineSwitch + 2, toolbarIndex(panel.toolbar().lightPlot, timeline));
        assertEquals(scatterSwitch + 2, toolbarIndex(panel.scatter().lightButton, scatter));
    }

    @Test
    void knockMapSkipsKnockSignalsThatNeverChange() throws IOException {
        CsvLogReader log = CsvLogReader.read(new StringReader(
                "Time\tRPM\tMAP\tKnock: Retard\tKnock: Current level\ns\trpm\tkPa\tdeg\tdB\n"
                        + "0\t1000\t30\t0\t-40\n1\t2000\t60\t0\t-30\n2\t3000\t90\t0\t-20\n"));
        ScatterRecipes.Recipe knock = ScatterRecipes.byTitle("Knock map");
        int[] fields = knock.fields(log);
        assertEquals(4, fields[2], "the level, since the retard stays 0");
        assertTrue(Double.isNaN(knock.fadeFor(log, fields[2])), "no fading by a dB level");
        assertEquals(ScatterRecipes.KNOCK_FADE, knock.fadeFor(log, 3), 1e-12);
    }

    @Test
    void hoverAndPickFindTheNearestPoint() throws IOException {
        ScatterView view = panel(new HistogramSettings(new Node())).scatter();
        ScatterPlot plot = view.plot();
        paint(plot);
        Rectangle area = plot.plotArea();
        double[] range = plot.view();
        int x = area.x + (int) Math.floor((4003 - range[0]) / (range[1] - range[0]) * area.width);
        int y = area.y + area.height - 1 - (int) Math.floor((100 - range[2]) / (range[3] - range[2]) * area.height);
        int point = plot.pointAt(new Point(x, y));
        assertTrue(point >= 0);
        assertEquals(100, plot.data().y[point], 1e-6);
        assertEquals(-1, plot.pointAt(new Point(area.x + 2, area.y + 2)), "nothing near the corner");

        plot.pick(point);
        assertTrue(view.infoLabel.getText().endsWith("Enter: show on timeline"), view.infoLabel.getText());
        assertNotNull(view.describe(point));
    }

    @Test
    void zoomKeepsWhileTheAxesStayAndDarkFollowsTheTimeline() throws IOException {
        LogViewerPanel panel = panel(new HistogramSettings(new Node()));
        ScatterView view = panel.scatter();
        view.plot().setView(3000, 4500, 80, 120);
        assertTrue(view.plot().isZoomed());
        view.strip().addFilter(HistogramFilter.of("CLT", HistogramFilter.Condition.AT_MOST, 80));
        assertTrue(view.plot().isZoomed(), "a filter keeps the zoom");
        view.strip().swapXY();
        assertFalse(view.plot().isZoomed(), "new axes show the whole plot");

        view.darkButton.doClick();
        assertTrue(panel.toolbar().darkPlot.isSelected(), "one Light / Dark setting for the timeline too");
        panel.setView(HistogramSettings.TIMELINE);
        panel.setView(HistogramSettings.SCATTER);
        assertTrue(view.darkButton.isSelected());
        paint(view.plot());
    }

    @Test
    void theLastViewIsRemembered() {
        HistogramSettings settings = new HistogramSettings(new Node());
        assertEquals(HistogramSettings.TIMELINE, settings.view());
        settings.setView(HistogramSettings.SCATTER);
        assertEquals(HistogramSettings.SCATTER, settings.view());
        assertFalse(settings.isHistogramView());
    }
}
