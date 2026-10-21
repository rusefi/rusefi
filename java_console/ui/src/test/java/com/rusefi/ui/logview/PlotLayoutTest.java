package com.rusefi.ui.logview;

import org.junit.jupiter.api.Test;

import java.util.Arrays;
import java.util.Collections;

import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertFalse;
import static org.junit.jupiter.api.Assertions.assertTrue;

class PlotLayoutTest {
    @Test
    void startsWithOneActivePlot() {
        PlotLayout layout = PlotLayout.single(6, new boolean[]{false, true, false, true, false, false});
        assertEquals(1, layout.plotCount());
        assertEquals(0, layout.activePlot());
        assertEquals(Arrays.asList(1, 3), layout.fields(0));
        assertFalse(layout.canRemovePlot());
    }

    @Test
    void newSignalsGoToTheActivePlot() {
        PlotLayout layout = new PlotLayout(10);
        layout.add(1);
        layout.addPlot();
        assertEquals(1, layout.activePlot());
        layout.add(2);
        layout.setActive(0);
        layout.add(3);
        assertEquals(Arrays.asList(1, 3), layout.fields(0));
        assertEquals(Collections.singletonList(2), layout.fields(1));
        assertEquals(1, layout.plotOf(2));
        assertEquals(-1, layout.plotOf(4));
    }

    @Test
    void aSignalIsInOnePlotOnly() {
        PlotLayout layout = new PlotLayout(10);
        layout.add(1);
        layout.addPlot();
        layout.add(1);
        assertTrue(layout.fields(1).isEmpty());
        layout.addToNewPlot(1);
        assertEquals(2, layout.plotCount(), "already plotted: no new plot");
    }

    @Test
    void addToNewPlotCreatesAndActivatesIt() {
        PlotLayout layout = new PlotLayout(10);
        layout.add(1);
        layout.addToNewPlot(2);
        assertEquals(2, layout.plotCount());
        assertEquals(1, layout.activePlot());
        assertEquals(Collections.singletonList(2), layout.fields(1));
    }

    @Test
    void coloursSurviveMovesAndRemovals() {
        PlotLayout layout = new PlotLayout(10);
        layout.add(1);
        layout.addToNewPlot(2);
        layout.addToNewPlot(3);
        int slotOf3 = layout.colors().slot(3);
        layout.move(2, -1);
        layout.removePlot(0);
        assertEquals(slotOf3, layout.colors().slot(3));
        assertFalse(layout.isPlotted(1), "traces of a removed plot are no longer plotted");
    }

    @Test
    void removePlotKeepsAValidActivePlot() {
        PlotLayout layout = new PlotLayout(10);
        layout.addPlot();
        layout.addPlot();
        assertEquals(2, layout.activePlot());
        layout.removePlot(2);
        assertEquals(1, layout.activePlot());
        layout.setActive(1);
        layout.removePlot(0);
        assertEquals(0, layout.activePlot(), "active plot moved up with its index");
        layout.removePlot(0);
        assertEquals(1, layout.plotCount(), "the last plot stays");
    }

    @Test
    void moveCarriesTheActivePlot() {
        PlotLayout layout = new PlotLayout(10);
        layout.add(1);
        layout.addToNewPlot(2);
        layout.setActive(0);
        layout.move(0, 1);
        assertEquals(Collections.singletonList(2), layout.fields(0));
        assertEquals(Collections.singletonList(1), layout.fields(1));
        assertEquals(1, layout.activePlot());
        layout.move(1, 1);
        assertEquals(1, layout.activePlot(), "moving past the end does nothing");
    }

    @Test
    void plotCountIsLimited() {
        PlotLayout layout = new PlotLayout(20);
        for (int i = 1; i < PlotLayout.MAX_PLOTS + 3; i++) {
            layout.addToNewPlot(i);
        }
        assertEquals(PlotLayout.MAX_PLOTS, layout.plotCount());
        assertFalse(layout.canAddPlot());
        assertEquals(PlotLayout.MAX_PLOTS + 2, layout.plottedCount(), "extra signals join the last plot");
    }

    @Test
    void moveToKeepsTheColour() {
        PlotLayout layout = new PlotLayout(10);
        layout.add(1);
        layout.add(2);
        layout.addPlot();
        int slot = layout.colors().slot(2);
        layout.moveTo(2, 1);
        assertEquals(Collections.singletonList(1), layout.fields(0));
        assertEquals(Collections.singletonList(2), layout.fields(1));
        assertEquals(slot, layout.colors().slot(2));
        layout.moveTo(5, 0);
        assertEquals(Arrays.asList(1, 5), layout.fields(0), "an unplotted signal is added");
    }

    @Test
    void moveToNewPlotKeepsTheColour() {
        PlotLayout layout = new PlotLayout(10);
        layout.add(1);
        layout.add(2);
        int slot = layout.colors().slot(2);
        layout.moveToNewPlot(2);
        assertEquals(2, layout.plotCount());
        assertEquals(1, layout.activePlot());
        assertEquals(Collections.singletonList(2), layout.fields(1));
        assertEquals(slot, layout.colors().slot(2));
        layout.moveToNewPlot(7);
        assertEquals(Collections.singletonList(7), layout.fields(2), "an unplotted signal gets its own plot");
    }

    @Test
    void heightWeightsFollowTheirPlots() {
        PlotLayout layout = new PlotLayout(10);
        layout.addPlot();
        layout.addPlot();
        layout.setWeights(new double[]{100, 200, 300});
        layout.move(2, -1);
        assertEquals(300, layout.weight(1), 1e-9);
        assertEquals(200, layout.weight(2), 1e-9);
        layout.removePlot(0);
        assertEquals(300, layout.weight(0), 1e-9);
        layout.addPlot();
        assertEquals(1, layout.weight(2), 1e-9);
        layout.resetWeights();
        assertEquals(1, layout.weight(0), 1e-9);
        assertEquals(1, layout.weight(1), 1e-9);
    }

    @Test
    void listenersHearChanges() {
        PlotLayout layout = new PlotLayout(10);
        int[] changes = {0};
        layout.addListener(() -> changes[0]++);
        layout.add(1);
        layout.addPlot();
        layout.setActive(0);
        layout.remove(1);
        layout.removePlot(1);
        assertEquals(5, changes[0]);
    }
}
