package com.rusefi.ui.logview;

import org.junit.jupiter.api.Test;

import java.awt.Color;
import java.io.IOException;
import java.io.InputStream;
import java.util.Properties;

import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertNotNull;
import static org.junit.jupiter.api.Assertions.assertTrue;

/**
 * Both plot palettes must keep every trace readable on its background and distinguishable from the
 * other traces, including for red-green colour-blind viewers. [tag:console_theme]
 */
class PlotPaletteTest {
    /** WCAG 2 minimum contrast for graphics such as chart lines. */
    private static final double MIN_TRACE_CONTRAST = 3.0;
    /** WCAG 2 minimum contrast for normal-size text. */
    private static final double MIN_TEXT_CONTRAST = 4.5;
    /** CIEDE2000 difference between any two traces in normal vision. */
    private static final double MIN_TRACE_DIFFERENCE = 15;
    /** CIEDE2000 difference between any two traces with deuteranopia or protanopia. */
    private static final double MIN_COLOR_BLIND_DIFFERENCE = 5;

    private static PlotPalette palette(boolean dark) {
        return dark ? PlotPalette.DARK : PlotPalette.LIGHT;
    }

    @Test
    void tracesStandOutFromBackground() {
        for (boolean dark : new boolean[]{false, true}) {
            checkTracesStandOutFromBackground(palette(dark), dark ? "dark" : "light");
        }
    }

    private static void checkTracesStandOutFromBackground(PlotPalette palette, String mode) {
        for (int i = 0; i < PlotPalette.TRACE_COUNT; i++) {
            double contrast = ColorMetrics.contrast(palette.trace(i), palette.background);
            assertTrue(contrast >= MIN_TRACE_CONTRAST, mode + " trace " + (i + 1) + " contrast " + contrast);
        }
    }

    @Test
    void textIsReadable() {
        for (boolean dark : new boolean[]{false, true}) {
            checkTextIsReadable(palette(dark), dark ? "dark" : "light");
        }
    }

    private static void checkTextIsReadable(PlotPalette palette, String mode) {
        assertTrue(ColorMetrics.contrast(palette.label, palette.background) >= MIN_TEXT_CONTRAST, mode + " label");
        assertTrue(ColorMetrics.contrast(palette.hint, palette.background) >= MIN_TEXT_CONTRAST, mode + " hint");
    }

    @Test
    void tracesAreDistinguishable() {
        for (boolean dark : new boolean[]{false, true}) {
            checkTracesAreDistinguishable(palette(dark), dark ? "dark" : "light");
        }
    }

    private static void checkTracesAreDistinguishable(PlotPalette palette, String mode) {
        for (int i = 0; i < PlotPalette.TRACE_COUNT; i++) {
            for (int j = i + 1; j < PlotPalette.TRACE_COUNT; j++) {
                Color a = palette.trace(i);
                Color b = palette.trace(j);
                String pair = mode + " traces " + (i + 1) + " and " + (j + 1);
                double normal = ColorMetrics.deltaE2000(a, b);
                assertTrue(normal >= MIN_TRACE_DIFFERENCE, pair + ": " + normal);
                for (double[][] deficiency : new double[][][]{ColorMetrics.DEUTERANOPIA, ColorMetrics.PROTANOPIA}) {
                    double simulated = ColorMetrics.deltaE2000(
                            ColorMetrics.simulate(a, deficiency), ColorMetrics.simulate(b, deficiency));
                    assertTrue(simulated >= MIN_COLOR_BLIND_DIFFERENCE, pair + " colour-blind: " + simulated);
                }
            }
        }
    }

    /** CIEDE2000 difference between a picker-only swatch and every other swatch. */
    private static final double MIN_SWATCH_DIFFERENCE = 10;

    /**
     * The four picker-only swatches are chosen by the user, never assigned automatically: readable on the plot and
     * clearly different from every other swatch, with a looser difference than the automatic traces.
     */
    @Test
    void pickerSwatchesAreReadableAndDistinct() {
        for (boolean dark : new boolean[]{false, true}) {
            PlotPalette palette = palette(dark);
            String mode = dark ? "dark" : "light";
            for (int i = PlotPalette.TRACE_COUNT; i < PlotPalette.SWATCH_COUNT; i++) {
                double contrast = ColorMetrics.contrast(palette.trace(i), palette.background);
                assertTrue(contrast >= MIN_TRACE_CONTRAST, mode + " swatch " + (i + 1) + " contrast " + contrast);
                for (int j = 0; j < PlotPalette.SWATCH_COUNT; j++) {
                    if (j != i) {
                        double difference = ColorMetrics.deltaE2000(palette.trace(i), palette.trace(j));
                        assertTrue(difference >= MIN_SWATCH_DIFFERENCE,
                                mode + " swatches " + (i + 1) + " and " + (j + 1) + ": " + difference);
                    }
                }
            }
        }
    }

    /** The checks above run on the Java constants; the theme file must hold the same colours. */
    @Test
    void themeFileMatchesConstants() throws IOException {
        Properties theme = new Properties();
        try (InputStream stream = PlotPalette.class.getResourceAsStream("/com/rusefi/ui/theme/FlatLightLaf.properties")) {
            assertNotNull(stream);
            theme.load(stream);
        }
        for (boolean dark : new boolean[]{false, true}) {
            checkThemeFile(theme, palette(dark), "Console.LogChart." + (dark ? "dark" : "light") + ".");
        }
    }

    private static void checkThemeFile(Properties theme, PlotPalette palette, String prefix) {
        assertColor(theme, prefix + "background", palette.background);
        assertColor(theme, prefix + "grid", palette.grid);
        assertColor(theme, prefix + "label", palette.label);
        assertColor(theme, prefix + "hint", palette.hint);
        assertColor(theme, prefix + "cursor", palette.cursor);
        assertColor(theme, prefix + "selection", palette.selection);
        assertColor(theme, prefix + "accent", palette.accent);
        for (int i = 0; i < PlotPalette.SWATCH_COUNT; i++) {
            assertColor(theme, prefix + "trace" + (i + 1), palette.trace(i));
        }
    }

    private static void assertColor(Properties theme, String key, Color expected) {
        String value = theme.getProperty(key);
        assertNotNull(value, key);
        String hex = value.trim().substring(1);
        int rgb = Integer.parseInt(hex.substring(0, 6), 16);
        int alpha = hex.length() == 8 ? Integer.parseInt(hex.substring(6), 16) : 255;
        assertEquals(expected, new Color((alpha << 24) | rgb, true), key);
    }
}
