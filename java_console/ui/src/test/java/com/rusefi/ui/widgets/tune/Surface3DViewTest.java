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
            color(image, -0.35, 0, -0.5), "The low-value facet occupies the left of the surface");
        assertEquals(CalibrationColorPalette.background(25, 0, 100),
            color(image, 0.15, 0, -0.25), "The high-value corner colors the right facet");
    }

    @Test
    void rectangularSurfaceRendersLastDataRowInUpperLeftFacet() {
        Surface3DView surface = new Surface3DView();
        surface.setSize(400, 400);
        surface.setData(new Double[][]{{0.0, 0.0}, {0.0, 0.0}, {0.0, 100.0}},
            new Double[]{1000.0, 6000.0}, new Double[]{40.0, 80.0, 120.0}, 0, 100);
        BufferedImage image = render(surface);

        assertEquals(CalibrationColorPalette.background(0, 0, 100),
            color(image, 0, -0.25, -0.5), "The first-row facet has only low values");
        assertEquals(CalibrationColorPalette.background(25, 0, 100),
            color(image, 0, 0.25, -0.25), "The upper-left facet includes the last row's high-value corner");
    }

    /** The rendered color at a facet's center, given in cube coordinates. */
    private static Color color(BufferedImage image, double x, double y, double z) {
        Surface3DView.Point3D point = Surface3DView.project(x, y, z, Surface3DView.DEFAULT_YAW,
            Surface3DView.DEFAULT_PITCH, Surface3DView.fitScale(400, 400), 200, 200);
        return new Color(image.getRGB(point.x, point.y));
    }

    @Test
    void selectedCellLightsItsFacetsAndMarksItsPoint() {
        Surface3DView surface = new Surface3DView();
        surface.setSize(400, 400);
        // Columns at x = -0.5, 0, 0.5 and rows at y = -0.5, 0.5, all at the lowest value.
        surface.setData(new Double[][]{{0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}},
            new Double[]{1000.0, 2000.0, 3000.0}, new Double[]{40.0, 120.0}, 0, 100);
        Color unselected = CalibrationColorPalette.background(0, 0, 100);

        // The middle column's first cell is a corner of both facets.
        surface.setSelection(new boolean[][]{{false, true, false}, {false, false, false}});
        BufferedImage image = render(surface);
        assertEquals(Color.ORANGE, color(image, -0.25, 0.1, -0.5), "Left facet sharing the cell");
        assertEquals(Color.ORANGE, color(image, 0.25, 0.1, -0.5), "Right facet sharing the cell");
        assertEquals(Surface3DView.SELECTED_POINT_COLOR, color(image, 0, -0.5, -0.5), "The cell's own point is marked");

        // A corner cell belongs to one facet only.
        surface.setSelection(new boolean[][]{{false, false, false}, {false, false, true}});
        image = render(surface);
        assertEquals(unselected, color(image, -0.25, 0.1, -0.5), "Facet not touching the cell");
        assertEquals(Color.ORANGE, color(image, 0.25, 0.1, -0.5));
        assertEquals(Surface3DView.SELECTED_POINT_COLOR, color(image, 0.5, 0.5, -0.5));

        surface.setSelection(null);
        assertFalse(surface.isSelected(1, 2));
        image = render(surface);
        assertEquals(unselected, color(image, 0.25, 0.1, -0.5));
        assertNotEquals(Surface3DView.SELECTED_POINT_COLOR, color(image, 0.5, 0.5, -0.5));
    }

    @Test
    void axisValuesAreCompact() {
        assertEquals("7000", Surface3DView.formatValue(7000));
        assertEquals("120", Surface3DView.formatValue(120.4));
        assertEquals("42.9", Surface3DView.formatValue(42.94));
        assertEquals("-3", Surface3DView.formatValue(-3.0));
        assertEquals("0.5", Surface3DView.formatValue(0.5));
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
    void surfaceAndAxisLabelsFitThePanelAtAnyRotation() {
        // A wide, short preview such as one under a small table: the height limits the scale.
        int width = 1400;
        int height = 330;
        double scale = Surface3DView.fitScale(width, height);
        for (int yaw = 0; yaw < 360; yaw += 5) {
            for (int pitch = 0; pitch < 360; pitch += 5) {
                // Corners of the cube grown by the label offset: every axis label anchor lies inside.
                double half = 0.5 + Surface3DView.LABEL_OFFSET;
                for (int corner = 0; corner < 8; corner++) {
                    Surface3DView.Point3D point = Surface3DView.project(
                        (corner & 1) * 2 * half - half, ((corner >> 1) & 1) * 2 * half - half,
                        ((corner >> 2) & 1) * 2 * half - half,
                        Math.toRadians(yaw), Math.toRadians(pitch), scale, width / 2, height / 2);
                    int marginX = Surface3DView.LABEL_HALF_WIDTH;
                    int marginY = Surface3DView.LABEL_HALF_HEIGHT;
                    assertTrue(point.x >= marginX && point.x <= width - marginX
                            && point.y >= marginY && point.y <= height - marginY,
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
