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
            new Color(image.getRGB(145, 179)), "The low-value facet occupies the left of the surface");
        assertEquals(CalibrationColorPalette.background(25, 0, 100),
            new Color(image.getRGB(248, 152)), "The high-value corner colors the right facet");
    }

    @Test
    void rectangularSurfaceRendersLastDataRowInUpperLeftFacet() {
        Surface3DView surface = new Surface3DView();
        surface.setSize(400, 400);
        surface.setData(new Double[][]{{0.0, 0.0}, {0.0, 0.0}, {0.0, 100.0}},
            new Double[]{1000.0, 6000.0}, new Double[]{40.0, 80.0, 120.0}, 0, 100);
        BufferedImage image = render(surface);

        assertEquals(CalibrationColorPalette.background(0, 0, 100),
            new Color(image.getRGB(241, 186)), "The first-row facet has only low values");
        assertEquals(CalibrationColorPalette.background(25, 0, 100),
            new Color(image.getRGB(145, 145)), "The upper-left facet includes the last row's high-value corner");
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
    void surfaceFitsThePanelAtAnyRotation() {
        // A wide, short preview such as one under a small table: the height limits the scale.
        int width = 1400;
        int height = 330;
        double scale = Surface3DView.fitScale(width, height);
        for (int yaw = 0; yaw < 360; yaw += 5) {
            for (int pitch = 0; pitch < 360; pitch += 5) {
                for (int corner = 0; corner < 8; corner++) {
                    Surface3DView.Point3D point = Surface3DView.project(
                        (corner & 1) - 0.5, ((corner >> 1) & 1) - 0.5, ((corner >> 2) & 1) - 0.5,
                        Math.toRadians(yaw), Math.toRadians(pitch), scale, width / 2, height / 2);
                    assertTrue(point.x >= 0 && point.x <= width && point.y >= 0 && point.y <= height,
                        "Corner " + corner + " at yaw " + yaw + ", pitch " + pitch + " is cut: " + point.x + "," + point.y);
                }
            }
        }
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
