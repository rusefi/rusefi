package com.rusefi.ui.widgets;

import com.opensr5.ini.GaugeModel;
import com.opensr5.ini.IniValue;
import org.junit.jupiter.api.Test;

import javax.swing.JPanel;
import java.awt.BorderLayout;
import java.awt.Color;
import java.awt.Graphics2D;
import java.awt.image.BufferedImage;

import static org.junit.jupiter.api.Assertions.*;

class CircularGaugeTest {
    static GaugeModel model(double min, double max, double danger, int decimals) {
        return new GaugeModel("CircularTestGauge", "circularTestChannel",
            IniValue.ofString("Coolant temperature"), IniValue.ofString("C"),
            IniValue.ofNumeric(min), IniValue.ofNumeric(max),
            IniValue.ofNumeric(min), IniValue.ofNumeric(min),
            IniValue.ofNumeric(danger), IniValue.ofNumeric(danger),
            IniValue.ofNumeric(decimals), IniValue.ofNumeric(decimals));
    }

    private static CircularGauge gauge(double min, double max, double danger, int decimals) {
        return new CircularGauge(min, max, model(min, max, danger, decimals));
    }

    @Test
    void matchesDashboardQuantizationIncludingNegativeTies() {
        CircularGauge gauge = gauge(0, 100, 80, 0);
        gauge.setValue(43);
        assertEquals("44", gauge.getDisplayText());
        gauge.setValue(-5);
        assertEquals("-6", gauge.getDisplayText());
        gauge = gauge(-100, 100, 80, 0);
        gauge.setValue(-103);
        assertEquals("-104", gauge.getDisplayText());
        gauge = gauge(0, 10, 8, 2);
        gauge.setValue(2.125);
        assertEquals("2.25", gauge.getDisplayText());

        for (double[] sample : new double[][] {
            {39, 1.125, 1.25}, {40, 1.5, 2}, {75, 3, 4}, {150, 3, 4}, {151, 6, 8}
        }) {
            gauge = gauge(0, sample[0], sample[0], 2);
            gauge.setValue(sample[1]);
            assertEquals(sample[2], gauge.getValue());
        }
    }

    @Test
    void sharesRounded270DegreePositionAndOnlyClampsVisuals() {
        CircularGauge gauge = gauge(0, 200, 160, 0);
        for (double[] sample : new double[][] {{0, 0}, {16, 22}, {100, 135}, {200, 270}, {252, 270}, {-8, 0}}) {
            gauge.setValue(sample[0]);
            assertEquals(sample[0], gauge.getValue());
            assertEquals((int) sample[1], gauge.getProgressDegrees());
        }
        gauge.setValue(252);
        assertEquals("252", gauge.getDisplayText());
        gauge = gauge(0, 100, 80, 0);
        gauge.setValue(250);
        assertEquals("250", gauge.getDisplayText());
        assertEquals(270, gauge.getProgressDegrees());
    }

    @Test
    void missingReadingsAndInvalidRangesRemainRenderable() {
        for (double[] range : new double[][] {{10, 10}, {20, 10}, {Double.NaN, Double.NaN}, {0, Double.POSITIVE_INFINITY}}) {
            CircularGauge gauge = gauge(range[0], range[1], Double.NaN, 0);
            gauge.setValue(15);
            assertTrue(gauge.getProgressDegrees() >= 0 && gauge.getProgressDegrees() <= 270);
            for (double invalid : new double[] {Double.NaN, Double.NEGATIVE_INFINITY, Double.POSITIVE_INFINITY}) {
                gauge.setValue(invalid);
                assertEquals("---", gauge.getDisplayText());
                assertEquals(0, gauge.getProgressDegrees());
                render(gauge, 150, 150);
            }
        }
    }

    @Test
    void paintsProgressNeedleHubAndDangerZoneInDashboardOrder() {
        CircularGauge gauge = gauge(0, 100, 80, 0);
        gauge.setValue(50);
        BufferedImage image = render(gauge, 400, 400);
        assertEquals(0xD0E128, rgb(image, 70, 150), "Filled side of the dial");
        assertNotEquals(0xD0E128, rgb(image, 330, 150), "Unfilled side of the dial");
        assertEquals(0xC60000, rgb(image, 200, 25), "Half-scale needle points straight up");
        assertEquals(0x202020, rgb(image, 200, 120), "Hub hides the inner needle");
        assertEquals(0xFF0000, rgb(image, 394, 200), "Danger ring at the end of the scale");
        gauge.setValue(80);
        image = render(gauge, 400, 400);
        assertEquals(0xFF0000, rgb(image, 70, 150), "Danger starts at the threshold, inclusively");
        gauge.setHighDangerValue(100);
        image = render(gauge, 400, 400);
        assertEquals(0xD0E128, rgb(image, 70, 150), "Changing threshold updates the fill");
    }

    @Test
    void staysCircularAndClearsOldNeedleAcrossSizesAndValues() {
        for (int size : new int[] {80, 150, 333, 800}) {
            CircularGauge gauge = gauge(0, 100, 80, 0);
            BufferedImage image = new BufferedImage(size + 100, size, BufferedImage.TYPE_INT_ARGB);
            // Swing repaints a transparent component's opaque parent first.
            // Painting repeatedly onto transparent pixels alone accumulates AA edges.
            JPanel parent = new JPanel(new BorderLayout());
            parent.setBackground(Color.WHITE);
            parent.add(gauge);
            parent.setSize(image.getWidth(), image.getHeight());
            parent.doLayout();
            for (int value : new int[] {0, 20, 50, 100, 30}) {
                gauge.setValue(value);
                Graphics2D g = image.createGraphics();
                parent.paint(g);
                g.dispose();
            }
            BufferedImage fresh = new BufferedImage(image.getWidth(), size, BufferedImage.TYPE_INT_ARGB);
            Graphics2D g = fresh.createGraphics();
            parent.paint(g);
            g.dispose();
            assertArrayEquals(fresh.getRGB(0, 0, fresh.getWidth(), size, null, 0, fresh.getWidth()),
                image.getRGB(0, 0, image.getWidth(), size, null, 0, image.getWidth()));
            assertEquals(Color.WHITE.getRGB(), image.getRGB(25, size / 2), "Parent shows through a wide cell's margin");
            assertNotEquals(Color.WHITE.getRGB(), image.getRGB(50 + size / 2, size / 2));
        }
    }

    static BufferedImage render(CircularGauge gauge, int width, int height) {
        gauge.setSize(width, height);
        BufferedImage image = new BufferedImage(width, height, BufferedImage.TYPE_INT_ARGB);
        Graphics2D g = image.createGraphics();
        gauge.paint(g);
        g.dispose();
        return image;
    }

    private static int rgb(BufferedImage image, int x, int y) {
        return image.getRGB(x, y) & 0xFFFFFF;
    }
}
