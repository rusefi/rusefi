package com.rusefi.ui.logview;

import com.rusefi.core.preferences.storage.Node;
import com.rusefi.logview.CsvLogReader;
import org.junit.jupiter.api.Test;

import java.awt.Color;
import java.io.IOException;
import java.io.StringReader;

import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertFalse;
import static org.junit.jupiter.api.Assertions.assertNotEquals;
import static org.junit.jupiter.api.Assertions.assertTrue;

class HistogramColorsTest {
    @Test
    void defaultsAndSavedChoices() {
        Node node = new Node();
        HistogramColors colors = new HistogramColors(node);
        assertEquals(HistogramColors.Palette.HEATMAP, colors.palette());
        assertEquals(HistogramColors.Range.ROBUST, colors.range());
        assertFalse(colors.reversed());
        assertTrue(Double.isNaN(colors.centre()), "automatic centre");

        colors.setPalette(HistogramColors.Palette.DIVERGING);
        colors.setReversed(true);
        colors.setManual(1.1, 0.8);
        colors.setCentre(0.95);
        HistogramColors reloaded = new HistogramColors(node);
        assertEquals(HistogramColors.Palette.DIVERGING, reloaded.palette());
        assertTrue(reloaded.reversed());
        assertEquals(0.8, reloaded.manualLow(), 1e-12, "the ends may be entered in either order");
        assertEquals(1.1, reloaded.manualHigh(), 1e-12);
        assertEquals(0.95, reloaded.centre(), 1e-12);

        reloaded.setCentre(Double.NaN);
        assertTrue(Double.isNaN(new HistogramColors(node).centre()), "back to automatic");
        reloaded.reset();
        assertEquals(HistogramColors.Palette.HEATMAP, new HistogramColors(node).palette());
    }

    @Test
    void palettesRunFromLowToHighColours() {
        Color heatLow = HistogramColors.color(HistogramColors.Palette.HEATMAP, 0);
        Color heatHigh = HistogramColors.color(HistogramColors.Palette.HEATMAP, 1);
        assertTrue(heatLow.getBlue() > heatLow.getRed(), "the heatmap starts blue: " + heatLow);
        assertTrue(heatHigh.getRed() > heatHigh.getBlue(), "and ends red: " + heatHigh);
        assertEquals(new Color(0x440154), HistogramColors.color(HistogramColors.Palette.VIRIDIS, 0));
        assertEquals(new Color(0xfde725), HistogramColors.color(HistogramColors.Palette.VIRIDIS, 1));
        assertEquals(HistogramColors.color(HistogramColors.Palette.VIRIDIS, 1),
                HistogramColors.color(HistogramColors.Palette.VIRIDIS, 7), "clamped");
        Color centre = HistogramColors.color(HistogramColors.Palette.DIVERGING, 0.5);
        assertNotEquals(centre, HistogramColors.color(HistogramColors.Palette.DIVERGING, 0));
        assertNotEquals(centre, HistogramColors.color(HistogramColors.Palette.DIVERGING, 1));
    }

    @Test
    void textIsWhiteOnDarkCellsAndDarkOnLightOnes() {
        assertTrue(HistogramColors.wantsLightText(new Color(0x440154)));
        assertFalse(HistogramColors.wantsLightText(new Color(0xfde725)));
        assertFalse(HistogramColors.wantsLightText(Color.WHITE));
        assertTrue(HistogramColors.wantsLightText(Color.BLACK));
    }

    @Test
    void quantileInterpolatesBetweenSortedValues() {
        double[] values = {5, 1, 3, 2, 4, 99, 99};
        assertEquals(1, HistogramColors.quantile(values, 5, 0), 1e-12, "only the first 5 values count");
        assertEquals(3, HistogramColors.quantile(values, 5, 0.5), 1e-12);
        assertEquals(5, HistogramColors.quantile(values, 5, 1), 1e-12);
        assertEquals(1.2, HistogramColors.quantile(values, 5, 0.05), 1e-12);
        assertTrue(Double.isNaN(HistogramColors.quantile(values, 0, 0.5)));
    }

    /** One cell per RPM bin, lambda 0.900..0.987 across 30 of them, and one cell reading 0.50. */
    private static HistogramTable table(HistogramColors colors) throws IOException {
        StringBuilder text = new StringBuilder("Time\tRPM\tMAP\tLambda 1\ns\trpm\tkPa\tlambda\n");
        for (int i = 0; i < 31; i++) {
            double lambda = i == 30 ? 0.5 : 0.9 + i * 0.003;
            text.append(i).append('\t').append(1000 + i * 100).append('\t').append(50).append('\t').append(lambda)
                    .append('\n');
        }
        CsvLogReader log = CsvLogReader.read(new StringReader(text.toString()));
        Histogram histogram = Histogram.compute(log, 1, 2, 3, 31, 2, new boolean[31], Double.NaN, Double.NaN);
        HistogramTable table = new HistogramTable();
        table.set(histogram, HistogramSettings.Mode.MEAN, HistogramTable.Mixture.LAMBDA, 0, false, colors);
        return table;
    }

    @Test
    void noOutliersKeepsOneOddCellFromWashingOutTheRest() throws IOException {
        HistogramColors colors = new HistogramColors(new Node());
        HistogramTable robust = table(colors);
        assertTrue(robust.scaleLow() > 0.8, "the 0.50 cell is left out of the range: " + robust.scaleLow());
        assertEquals(0, robust.position(0.5), 1e-12, "and gets the low end colour");
        assertTrue(robust.legendLabels()[0].startsWith("≤ "));

        colors.setRange(HistogramColors.Range.AUTO);
        HistogramTable auto = table(colors);
        assertEquals(0.5, auto.scaleLow(), 1e-9);
        assertEquals(0.987, auto.scaleHigh(), 1e-9);
        assertFalse(auto.legendLabels()[0].startsWith("≤"));
        assertTrue(auto.position(0.98) - auto.position(0.90) < robust.position(0.98) - robust.position(0.90),
                "with the outlier in the range the normal cells get fewer colours");
    }

    @Test
    void reverseFlipsThePalette() throws IOException {
        HistogramColors colors = new HistogramColors(new Node());
        colors.setRange(HistogramColors.Range.AUTO);
        double forward = table(colors).position(0.9);
        colors.setReversed(true);
        assertEquals(1 - forward, table(colors).position(0.9), 1e-12);
    }
}
