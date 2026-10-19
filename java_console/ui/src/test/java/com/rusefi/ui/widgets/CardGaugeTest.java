package com.rusefi.ui.widgets;

import org.junit.jupiter.api.Test;

import javax.swing.JComponent;
import java.awt.Graphics2D;
import java.awt.image.BufferedImage;

import static org.junit.jupiter.api.Assertions.*;

class CardGaugeTest {
    private static DigitalGauge text(double min, double max, double danger, int decimals) {
        return new DigitalGauge(CircularGaugeTest.model(min, max, danger, decimals), null, null);
    }

    private static BarGauge bar(boolean vertical, double danger) {
        return new BarGauge(CircularGaugeTest.model(0, 100, danger, 0), null, null, vertical);
    }

    @Test
    void formatsWithTheIniDecimalsAndFlagsTheDangerZone() {
        DigitalGauge gauge = text(0, 100, 80, 1);
        assertEquals("---", gauge.getDisplayText(), "No reading yet");
        gauge.setValue(42.345);
        assertEquals("42.3", gauge.getDisplayText());
        assertFalse(gauge.isDanger(gauge.getValue()));
        gauge.setValue(80);
        assertTrue(gauge.isDanger(80), "Danger starts at the threshold, inclusively");
        gauge.setValue(-0.0);
        assertEquals("0.0", gauge.getDisplayText(), "No negative zero");
        assertEquals(0, gauge.fraction(-5));
        assertEquals(1, gauge.fraction(150), "The bar clamps; the text still shows the real value");
        assertEquals(0, gauge.fraction(Double.NaN));
        assertEquals(0.8, gauge.dangerFraction(), 1e-9);
    }

    @Test
    void invalidRangesAndMissingDangerStayRenderable() {
        for (double[] range : new double[][] {{10, 10}, {20, 10}, {Double.NaN, Double.NaN}}) {
            DigitalGauge gauge = text(range[0], range[1], Double.NaN, 0);
            assertTrue(gauge.maxValue > gauge.minValue);
            assertEquals(gauge.maxValue, gauge.dangerThreshold, "Without a danger value the zone starts at the maximum");
            gauge.setValue(5);
            render(gauge, 200, 100);
        }
    }

    @Test
    void barsFillInProportionAndTurnRedInTheDangerZone() {
        for (boolean vertical : new boolean[] {false, true}) {
            int width = vertical ? 120 : 300;
            int height = vertical ? 300 : 120;
            BarGauge gauge = bar(vertical, 90);
            assertEquals(vertical, gauge.isVertical());
            gauge.setValue(Double.NaN);
            assertEquals(0, yellow(render(gauge, width, height)), "No reading, no fill");
            gauge.setValue(25);
            int quarter = yellow(render(gauge, width, height));
            gauge.setValue(50);
            int half = yellow(render(gauge, width, height));
            assertTrue(quarter > 0);
            assertEquals(2.0, (double) half / quarter, 0.25, "Fill grows with the value");
            int redBefore = red(render(gauge, width, height));
            gauge.setValue(95);
            BufferedImage danger = render(gauge, width, height);
            assertEquals(0, yellow(danger), "Danger replaces the yellow fill");
            assertTrue(red(danger) > redBefore + half / 2, "The fill turns red");
        }
    }

    @Test
    void cardsLeaveTheirCornersTransparentAndSurviveTinySizes() {
        JComponent[] gauges = {text(0, 100, 80, 0), bar(false, 80), bar(true, 80)};
        for (JComponent gauge : gauges) {
            ((GaugeDisplay) gauge).setValue(60);
            ((GaugeDisplay) gauge).setTitle("A very long gauge title that cannot possibly fit in the card");
            BufferedImage image = render(gauge, 240, 160);
            assertEquals(0, image.getRGB(0, 0) >>> 24, "Overlapped gauges show through the rounded corner");
            for (int size : new int[] {10, 25, 40, 60}) {
                render(gauge, size, size);
                render(gauge, size * 4, size);
                render(gauge, size, size * 4);
            }
        }
    }

    @Test
    void longTitlesAreShortenedWithAnEllipsis() {
        BufferedImage image = new BufferedImage(10, 10, BufferedImage.TYPE_INT_ARGB);
        Graphics2D g = image.createGraphics();
        g.setFont(GaugeStyle.font(java.awt.Font.PLAIN, 12));
        String fitted = GaugeStyle.fit(g, "Coolant temperature sensor", 60);
        assertTrue(fitted.endsWith("\u2026"));
        assertTrue(g.getFontMetrics().stringWidth(fitted) <= 60);
        assertEquals("CLT", GaugeStyle.fit(g, "CLT", 60));
        g.dispose();
    }

    private static BufferedImage render(JComponent gauge, int width, int height) {
        gauge.setSize(width, height);
        BufferedImage image = new BufferedImage(width, height, BufferedImage.TYPE_INT_ARGB);
        Graphics2D g = image.createGraphics();
        gauge.paint(g);
        g.dispose();
        return image;
    }

    /** Pixels of the yellow fill gradient. */
    private static int yellow(BufferedImage image) {
        return count(image, (r, g, b) -> r > 235 && g > 185 && g < 235 && b < 160);
    }

    private static int red(BufferedImage image) {
        return count(image, (r, g, b) -> r > 200 && g < 90 && b < 90);
    }

    private interface ColorTest {
        boolean matches(int r, int g, int b);
    }

    private static int count(BufferedImage image, ColorTest test) {
        int count = 0;
        for (int y = 0; y < image.getHeight(); y++) {
            for (int x = 0; x < image.getWidth(); x++) {
                int argb = image.getRGB(x, y);
                if ((argb >>> 24) == 255 && test.matches((argb >> 16) & 0xFF, (argb >> 8) & 0xFF, argb & 0xFF)) {
                    count++;
                }
            }
        }
        return count;
    }
}
