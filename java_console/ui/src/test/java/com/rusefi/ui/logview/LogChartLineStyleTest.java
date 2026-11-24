package com.rusefi.ui.logview;

import com.rusefi.core.preferences.storage.Node;
import com.rusefi.logview.CsvLogReader;
import org.junit.jupiter.api.Test;

import javax.swing.JMenuItem;
import java.awt.BasicStroke;
import java.awt.Color;
import java.awt.image.BufferedImage;
import java.io.IOException;
import java.io.StringReader;

import static org.junit.jupiter.api.Assertions.assertArrayEquals;
import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertFalse;
import static org.junit.jupiter.api.Assertions.assertNull;
import static org.junit.jupiter.api.Assertions.assertTrue;

/**
 * Trace lines: a default width in pixels (Grid popup), and per signal its own width, solid / dashed / dotted and
 * the dash and gap lengths ({@link TraceLineEditor}, "Line…" in the signal's menus).
 */
class LogChartLineStyleTest {
    private static final int WIDTH = 600;
    private static final int HEIGHT = 400;
    private static final int SIGNAL = 1;
    private static final int OTHER = 2;

    /** Two ramps, so each trace crosses the whole plot. */
    private static CsvLogReader log() throws IOException {
        StringBuilder text = new StringBuilder("Time\tSignal\tOther\ns\tV\tV\n");
        for (int i = 0; i <= 100; i++) {
            text.append(i / 10.0).append('\t').append(i).append('\t').append(100 - i).append('\n');
        }
        return CsvLogReader.read(new StringReader(text.toString()));
    }

    private static LogChart chart(ChartSettings settings, int... fields) throws IOException {
        CsvLogReader log = log();
        PlotLayout layout = new PlotLayout(log.fieldCount());
        for (int field : fields) {
            layout.add(field);
        }
        LogChart chart = new LogChart(log, layout, settings);
        chart.setSize(WIDTH, HEIGHT);
        return chart;
    }

    /** Pixels that differ from the plot background below the legend, where the trace is drawn. */
    private static int inkedPixels(LogChart chart) {
        BufferedImage image = new BufferedImage(WIDTH, HEIGHT, BufferedImage.TYPE_INT_RGB);
        chart.paint(image.createGraphics());
        int background = chart.palette().background.getRGB();
        int count = 0;
        for (int y = 80; y < HEIGHT - 40; y++) {
            for (int x = 80; x < WIDTH - 20; x++) {
                if (image.getRGB(x, y) != background) {
                    count++;
                }
            }
        }
        return count;
    }

    private static float[] dashes(LogChart chart, int field) {
        return ((BasicStroke) chart.traceStroke(field, PlotPalette.LIGHT, false)).getDashArray();
    }

    @Test
    void defaultWidthFollowsThePlotUntilChosenAndASignalCanHaveItsOwn() throws IOException {
        ChartSettings settings = new ChartSettings();
        LogChart chart = chart(settings, SIGNAL, OTHER);
        assertEquals(2f, chart.traceWidth(SIGNAL, PlotPalette.LIGHT, false));
        assertEquals(1.5f, chart.traceWidth(SIGNAL, PlotPalette.DARK, false));
        assertEquals(3f, chart.traceWidth(SIGNAL, PlotPalette.LIGHT, true), "the highlighted trace a pixel wider");

        settings.setDefaultLineWidth(2.5);
        assertEquals(2.5f, chart.traceWidth(SIGNAL, PlotPalette.DARK, false), "a chosen width on both plots");

        settings.setTraceLine("Signal", TraceStyle.Spec.DEFAULT.withWidth(4));
        assertEquals(4f, chart.traceWidth(SIGNAL, PlotPalette.LIGHT, false));
        assertEquals(2.5f, chart.traceWidth(OTHER, PlotPalette.LIGHT, false), "other signals keep the default");

        settings.setDefaultLineWidth(Double.NaN);
        assertEquals(2f, chart.traceWidth(OTHER, PlotPalette.LIGHT, false), "follows the plot again");
        settings.setDefaultLineWidth(100);
        assertEquals(TraceStyle.MAX_WIDTH, settings.getDefaultLineWidth());
    }

    @Test
    void dashesFollowTheWidthUntilTypedIn() throws IOException {
        ChartSettings settings = new ChartSettings();
        LogChart chart = chart(settings, SIGNAL);
        assertNull(dashes(chart, SIGNAL), "solid by default");

        settings.setTraceLine("Signal", TraceStyle.Spec.DEFAULT.withLine(TraceStyle.Line.DASHED));
        assertArrayEquals(new float[]{9, 6}, dashes(chart, SIGNAL), "automatic at 2 px");
        settings.setTraceLine("Signal", settings.getTraceLine("Signal").withWidth(4));
        assertArrayEquals(new float[]{15, 10}, dashes(chart, SIGNAL), "automatic dashes grow with the width");

        settings.setTraceLine("Signal", settings.getTraceLine("Signal").withDash(20).withGap(3));
        assertArrayEquals(new float[]{20, 3}, dashes(chart, SIGNAL));
        settings.setTraceLine("Signal", settings.getTraceLine("Signal").withWidth(1));
        assertArrayEquals(new float[]{20, 3}, dashes(chart, SIGNAL), "typed lengths stay in pixels");

        BasicStroke highlighted = (BasicStroke) chart.traceStroke(SIGNAL, PlotPalette.LIGHT, true);
        assertEquals(2f, highlighted.getLineWidth());
        assertArrayEquals(new float[]{20, 3}, highlighted.getDashArray(), "highlighting keeps the pattern");
    }

    @Test
    void dotsAreAsWideAsTheLineWithTheSpacingBetweenThem() throws IOException {
        ChartSettings settings = new ChartSettings();
        LogChart chart = chart(settings, SIGNAL);
        settings.setTraceLine("Signal", TraceStyle.Spec.DEFAULT.withLine(TraceStyle.Line.DOTTED).withWidth(3).withGap(5));
        BasicStroke stroke = (BasicStroke) chart.traceStroke(SIGNAL, PlotPalette.LIGHT, false);
        assertEquals(BasicStroke.CAP_ROUND, stroke.getEndCap());
        assertEquals(8f, stroke.getDashArray()[0] + stroke.getDashArray()[1], 0.02f, "dot 3 px + space 5 px");
        assertEquals(1, new TraceStyle.Spec(1, TraceStyle.Line.DOTTED, 0, -4).gap, "never below a pixel");
    }

    @Test
    void widthAndDashesChangeWhatIsPainted() throws IOException {
        ChartSettings settings = new ChartSettings();
        LogChart chart = chart(settings, SIGNAL);
        int normal = inkedPixels(chart);

        settings.setTraceLine("Signal", TraceStyle.Spec.DEFAULT.withWidth(4));
        int thick = inkedPixels(chart);
        assertTrue(thick > normal, "a wider line covers more of the plot: " + thick + " vs " + normal);

        settings.setTraceLine("Signal", TraceStyle.Spec.DEFAULT.withLine(TraceStyle.Line.DASHED));
        int dashed = inkedPixels(chart);
        assertTrue(dashed < normal, "gaps between the dashes: " + dashed + " vs " + normal);

        settings.setTraceLine("Signal", settings.getTraceLine("Signal").withGap(30));
        assertTrue(inkedPixels(chart) < dashed, "wider gaps");
    }

    @Test
    void editorEditsOneSignalAndShowsTheLengthsItsLineUses() {
        ChartSettings settings = new ChartSettings();
        int[] repaints = {0};
        TraceLineEditor editor = new TraceLineEditor(settings, "Signal", Color.RED, 2, () -> repaints[0]++);
        assertTrue(editor.defaultWidth.isSelected());
        assertEquals("Default (2 px)", editor.defaultWidth.getText());
        assertEquals("solid", editor.shownLengths());
        assertFalse(editor.resetButton.isEnabled(), "nothing to reset");

        editor.widthInput.setValue(3.5);
        assertTrue(editor.ownWidth.isSelected(), "typing a width makes it the signal's own");
        assertEquals(3.5, settings.getTraceLine("Signal").width);

        editor.lineSegments[TraceStyle.Line.DASHED.ordinal()].doClick();
        assertEquals(TraceStyle.Line.DASHED, settings.getLineStyle("Signal"));
        assertEquals("dashed", editor.shownLengths());
        assertEquals(14, editor.dashInput.getValue(), "shows the automatic dash at 3.5 px");
        assertTrue(Double.isNaN(settings.getTraceLine("Signal").dash), "still automatic");

        editor.dashInput.setValue(25);
        assertEquals(25, settings.getTraceLine("Signal").dash);
        editor.lineSegments[TraceStyle.Line.DOTTED.ordinal()].doClick();
        assertEquals("dotted", editor.shownLengths());
        editor.spacingInput.setValue(7);
        assertEquals(7, settings.getTraceLine("Signal").gap);

        editor.defaultWidth.doClick();
        assertTrue(Double.isNaN(settings.getTraceLine("Signal").width));
        assertEquals(TraceStyle.Spec.DEFAULT, settings.getTraceLine("Other"), "other signals untouched");

        editor.resetButton.doClick();
        assertTrue(settings.getTraceLine("Signal").isDefault());
        assertEquals("solid", editor.shownLengths());
        assertTrue(repaints[0] > 0, "the chart repaints after each change");
    }

    /** An option pane keeps its first size, so switching the line must not change the editor's (see CLAUDE.md). */
    @Test
    void editorKeepsItsSizeWhenTheLineChanges() {
        TraceLineEditor editor = new TraceLineEditor(new ChartSettings(), "Signal", Color.RED, 2, () -> {
        });
        java.awt.Dimension solid = editor.getPreferredSize();
        editor.lineSegments[TraceStyle.Line.DASHED.ordinal()].doClick();
        assertEquals(solid, editor.getPreferredSize());
        editor.lineSegments[TraceStyle.Line.DOTTED.ordinal()].doClick();
        assertEquals(solid, editor.getPreferredSize());
    }

    @Test
    void cancelPutsTheLineBackAndOkKeepsIt() throws IOException {
        ChartSettings settings = new ChartSettings();
        TraceStyle.Spec before = TraceStyle.Spec.DEFAULT.withLine(TraceStyle.Line.DOTTED);
        settings.setTraceLine("Signal", before);
        LogChart chart = chart(settings, SIGNAL);
        String[] title = {null};
        chart.lineConfirm = (parent, editor, dialogTitle) -> {
            title[0] = dialogTitle;
            editor.lineSegments[TraceStyle.Line.DASHED.ordinal()].doClick();
            assertEquals(TraceStyle.Line.DASHED, settings.getLineStyle("Signal"), "the plot follows while editing");
            return false;
        };
        assertFalse(chart.editLine(SIGNAL));
        assertEquals("Line — Signal", title[0]);
        assertEquals(before, settings.getTraceLine("Signal"), "Cancel puts the line back");

        chart.lineConfirm = (parent, editor, dialogTitle) -> {
            editor.widthInput.setValue(5.0);
            return true;
        };
        assertTrue(chart.editLine(SIGNAL));
        assertEquals(5, settings.getTraceLine("Signal").width);
        assertEquals(TraceStyle.Line.DOTTED, settings.getLineStyle("Signal"));
    }

    @Test
    void legendMenuOffersLine() throws IOException {
        LogChart chart = chart(new ChartSettings(), SIGNAL);
        int[] opened = {0};
        chart.lineConfirm = (parent, editor, title) -> {
            opened[0]++;
            return true;
        };
        for (java.awt.Component component : chart.legendMenu(SIGNAL).getComponents()) {
            if (component instanceof JMenuItem && "Line…".equals(((JMenuItem) component).getText())) {
                ((JMenuItem) component).doClick();
            }
        }
        assertEquals(1, opened[0], "Line… in the legend menu opens the dialog");
    }

    @Test
    void linesSurviveARestartAndDefaultsAreNotStored() {
        Node node = new Node();
        ChartSettings settings = new ChartSettings(node);
        TraceStyle.Spec target = new TraceStyle.Spec(3, TraceStyle.Line.DASHED, 12, Double.NaN);
        settings.setTraceLine("AFR target", target);
        settings.setTraceLine("RPM", TraceStyle.Spec.DEFAULT.withLine(TraceStyle.Line.DOTTED));
        settings.setTraceLine("RPM", TraceStyle.Spec.DEFAULT);
        assertFalse(node.getConfig().containsKey("lineWidth"), "an untouched width is not stored");
        settings.setDefaultLineWidth(1.5);

        ChartSettings restored = new ChartSettings(node);
        assertEquals(target, restored.getTraceLine("AFR target"));
        assertTrue(Double.isNaN(restored.getTraceLine("AFR target").gap), "an automatic gap stays automatic");
        assertTrue(restored.getTraceLine("RPM").isDefault());
        assertFalse(node.getChild("traceLines").getConfig().containsKey("RPM"), "the default line is not stored");
        assertEquals(1.5, restored.getDefaultLineWidth());

        assertNull(TraceStyle.Spec.decode("WAVY;1;2;3"));
        assertNull(TraceStyle.Spec.decode("DASHED;1;2"));
        assertNull(TraceStyle.Spec.decode("DASHED;NaN;2;3"));
    }

    @Test
    void gridPopupSetsTheDefaultWidthInPixels() {
        ChartSettings settings = new ChartSettings();
        settings.setDarkPlot(false);
        int[] changes = {0};
        ChartToolbar toolbar = new ChartToolbar(settings, () -> changes[0]++);
        assertEquals(2.0, toolbar.traceWidthInput.getValue(), "shows the plot's own width");
        assertFalse(toolbar.traceWidthPlotDefault.isEnabled());

        toolbar.darkPlot.doClick();
        assertEquals(1.5, toolbar.traceWidthInput.getValue(), "follows the dark plot while untouched");
        assertTrue(Double.isNaN(settings.getDefaultLineWidth()), "switching plots does not choose a width");

        changes[0] = 0;
        toolbar.traceWidthInput.setValue(3.0);
        assertEquals(3.0, settings.getDefaultLineWidth());
        assertEquals(1, changes[0], "the chart is told to repaint");
        assertTrue(toolbar.traceWidthPlotDefault.isEnabled());

        toolbar.traceWidthPlotDefault.doClick();
        assertTrue(Double.isNaN(settings.getDefaultLineWidth()));
        assertEquals(1.5, toolbar.traceWidthInput.getValue());
    }

    @Test
    void resetAllLinesPutsEverySignalAndTheDefaultWidthBack() {
        Node node = new Node();
        ChartSettings settings = new ChartSettings(node);
        settings.setDarkPlot(false);
        int[] changes = {0};
        ChartToolbar toolbar = new ChartToolbar(settings, () -> changes[0]++);
        assertFalse(toolbar.resetLinesButton.isEnabled(), "nothing to reset");

        settings.setTraceLine("AFR target", new TraceStyle.Spec(3, TraceStyle.Line.DASHED, 12, 4));
        settings.setTraceLine("MAP", TraceStyle.Spec.DEFAULT.withLine(TraceStyle.Line.DOTTED));
        // what opening the Grid popup does
        toolbar.updateControls();
        assertTrue(toolbar.resetLinesButton.isEnabled(), "opening Grid notices lines set from the signal menus");
        toolbar.traceWidthInput.setValue(4.0);

        changes[0] = 0;
        toolbar.resetLinesButton.doClick();
        assertTrue(settings.getTraceLine("AFR target").isDefault());
        assertTrue(settings.getTraceLine("MAP").isDefault());
        assertTrue(Double.isNaN(settings.getDefaultLineWidth()));
        assertEquals(2.0, toolbar.traceWidthInput.getValue(), "back to the plot's own width");
        assertFalse(toolbar.resetLinesButton.isEnabled());
        assertEquals(1, changes[0], "the chart is told to repaint");
        assertFalse(new ChartSettings(node).hasCustomLines(), "the reset is saved");
    }
}
