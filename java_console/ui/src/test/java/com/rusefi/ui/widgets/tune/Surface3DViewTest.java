package com.rusefi.ui.widgets.tune;

import org.junit.jupiter.api.Test;

import java.awt.Color;
import java.awt.Graphics2D;
import java.awt.image.BufferedImage;

import static org.junit.jupiter.api.Assertions.*;

class Surface3DViewTest {
    @Test
    void rectangularSurfaceRendersColumnsAlongXAndRowsAlongY() {
        Surface3DView surface = new Surface3DView();
        surface.setSize(400, 400);
        surface.setData(new Double[][]{{0.0, 0.0, 0.0}, {0.0, 0.0, 100.0}},
            new Double[]{1000.0, 2500.0, 6000.0}, new Double[]{40.0, 120.0}, 0, 100);
        BufferedImage image = render(surface);

        assertEquals(CalibrationColorPalette.background(0, 0, 100),
            new Color(image.getRGB(120, 170)), "The low-value facet occupies the left of the surface");
        assertEquals(CalibrationColorPalette.background(25, 0, 100),
            new Color(image.getRGB(270, 130)), "The high-value corner colors the right facet");
    }

    @Test
    void rectangularSurfaceRendersLastDataRowInUpperLeftFacet() {
        Surface3DView surface = new Surface3DView();
        surface.setSize(400, 400);
        surface.setData(new Double[][]{{0.0, 0.0}, {0.0, 0.0}, {0.0, 100.0}},
            new Double[]{1000.0, 6000.0}, new Double[]{40.0, 80.0, 120.0}, 0, 100);
        BufferedImage image = render(surface);

        assertEquals(CalibrationColorPalette.background(0, 0, 100),
            new Color(image.getRGB(260, 180)), "The first-row facet has only low values");
        assertEquals(CalibrationColorPalette.background(25, 0, 100),
            new Color(image.getRGB(120, 120)), "The upper-left facet includes the last row's high-value corner");
    }

    private static BufferedImage render(Surface3DView surface) {
        BufferedImage image = new BufferedImage(400, 400, BufferedImage.TYPE_INT_RGB);
        Graphics2D graphics = image.createGraphics();
        try {
            surface.paint(graphics);
        } finally {
            graphics.dispose();
        }
        return image;
    }

    @Test
    void unevenBinsKeepTheirRelativeSpacing() {
        assertArrayEquals(new double[]{-0.5, -0.4, 0.5},
            Surface3DView.axisPositions(new Double[]{1000.0, 1500.0, 6000.0}, 3), 1e-9);
        assertArrayEquals(new double[]{-0.5, 0.3, 0.5},
            Surface3DView.axisPositions(new Double[]{100.0, 20.0, 0.0}, 3), 1e-9);
    }

    @Test
    void unusableAxesFallBackToEvenSpacing() {
        for (Double[] bins : new Double[][]{null, {1.0}, {1.0, 1.0, 1.0},
            {1.0, 3.0, 2.0}, {1.0, null, 3.0}, {1.0, Double.NaN, 3.0}}) {
            assertFalse(Surface3DView.hasUsableAxis(bins, 3));
            assertArrayEquals(new double[]{-0.5, 0, 0.5}, Surface3DView.axisPositions(bins, 3), 1e-9);
        }
        assertArrayEquals(new double[]{0}, Surface3DView.axisPositions(new Double[]{1.0}, 1));
    }
}
