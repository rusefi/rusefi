package com.rusefi.ts_plugin.knock;

import org.junit.jupiter.api.Test;

import javax.swing.*;
import java.awt.*;
import java.awt.image.BufferedImage;

import static org.junit.jupiter.api.Assertions.assertEquals;

public class KnockPlotStyleTest {
    /** The TunerStudio plugin keeps the plots' original dark look. */
    @Test
    public void defaultIsTheOriginalDarkLook() {
        KnockPlotStyle style = KnockPlotStyle.DEFAULT;
        assertEquals(new Color(0x1b, 0x1b, 0x1f), style.background);
        assertEquals(new Color(0xff, 0xff, 0xff, 40), style.grid);
        assertEquals(new Color(0xd0, 0xd0, 0xd0), style.axisText);
        assertEquals(new Color(0xff, 0xff, 0xff, 60), style.title);
        assertEquals(new Color(0xff, 0xff, 0xff, 90), style.cursor);
        assertEquals(Color.YELLOW, style.current);
        assertEquals(new Color(0xff, 0x6e, 0x6e), style.peak);
        assertEquals(Color.RED, style.peakMarker);
        assertEquals(Color.ORANGE, style.mouse);
        assertEquals(new Color(0x55, 0x8f, 0xd0, 140), style.magnitudeFill);
        assertEquals(new Color(0x9f, 0xd0, 0xff), style.magnitudeLine);
        assertEquals(Color.YELLOW, style.magnitudePeak);
    }

    @Test
    public void builderChangesOnlyWhatIsSet() {
        KnockPlotStyle light = KnockPlotStyle.DEFAULT.toBuilder().background(Color.WHITE).build();
        assertEquals(Color.WHITE, light.background);
        assertEquals(KnockPlotStyle.DEFAULT.grid, light.grid);
        assertEquals(KnockPlotStyle.DEFAULT.magnitudeLine, light.magnitudeLine);
    }

    @Test
    public void bothPlotsPaintWithTheGivenStyle() throws Exception {
        KnockPlotStyle style = KnockPlotStyle.DEFAULT.toBuilder().background(new Color(0xfa, 0xfb, 0xfc)).build();
        SwingUtilities.invokeAndWait(() -> {
            KnockCanvas spectrogram = new KnockCanvas(1, 1, true);
            spectrogram.setStyle(style);
            assertEquals(style.background.getRGB(), cornerPixel(spectrogram.getComponent()));

            KnockMagnitudeCanvas magnitudes = new KnockMagnitudeCanvas();
            magnitudes.setStyle(style);
            assertEquals(style.background.getRGB(), cornerPixel(magnitudes.getComponent()));
        });
    }

    @Test
    public void defaultKeepsTheOriginalLayout() {
        assertEquals(false, KnockPlotStyle.DEFAULT.logViewerLayout);
        assertEquals(null, KnockPlotStyle.DEFAULT.valueFontFamily);
    }

    /** Header chips, cursor box and colour bar must cope with tiny and narrow plots and edge pointers. */
    @Test
    public void logViewerLayoutPaintsAtAnySizeAndPointerPosition() throws Exception {
        KnockPlotStyle style = KnockPlotStyle.DEFAULT.toBuilder()
            .logViewerLayout(true).valueFontFamily(Font.MONOSPACED).build();
        SwingUtilities.invokeAndWait(() -> {
            int[][] sizes = {{1, 1}, {80, 40}, {300, 120}, {1600, 600}};
            for (int[] size : sizes) {
                KnockCanvas canvas = new KnockCanvas(2, 4, false);
                canvas.setStyle(style);
                canvas.setTitle("Cylinder 2");
                canvas.setFrequencyStart(1000);
                canvas.setFrequencyStep(300);
                float[] values = new float[64];
                values[20] = 200;
                canvas.processValues(values);
                int[][] pointers = {{-1, -1}, {0, 0}, {size[0] / 2, size[1] / 2}, {size[0] - 1, size[1] - 1}};
                for (int[] pointer : pointers) {
                    canvas.setMousePosition(pointer[0], pointer[1]);
                    paint(canvas.getComponent(), size[0], size[1]);
                    assertEquals(64, canvas.getCurrentMouseMagnitudes().length);
                }

                KnockMagnitudeCanvas magnitudes = new KnockMagnitudeCanvas();
                magnitudes.setStyle(style);
                magnitudes.processValues(values);
                paint(magnitudes.getComponent(), size[0], size[1]);
            }
        });
    }

    private static void paint(JComponent component, int width, int height) {
        component.setSize(width, height);
        BufferedImage image = new BufferedImage(width, height, BufferedImage.TYPE_INT_RGB);
        Graphics2D g = image.createGraphics();
        try {
            component.paint(g);
        } finally {
            g.dispose();
        }
    }

    /** Bottom-right pixel: plain background, clear of labels, legend and markers. */
    private static int cornerPixel(JComponent component) {
        component.setSize(320, 200);
        BufferedImage image = new BufferedImage(320, 200, BufferedImage.TYPE_INT_RGB);
        Graphics2D g = image.createGraphics();
        try {
            component.paint(g);
        } finally {
            g.dispose();
        }
        return image.getRGB(319, 199);
    }
}
