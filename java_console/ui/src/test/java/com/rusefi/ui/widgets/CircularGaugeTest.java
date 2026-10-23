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
    void roundsToIniValueDecimalsIncludingNegativeTies() {
        CircularGauge gauge = gauge(0, 100, 80, 0);
        gauge.setValue(43);
        assertEquals("43", gauge.getDisplayText());
        gauge.setValue(-5.5);
        assertEquals("-6", gauge.getDisplayText());
        gauge = gauge(-100, 100, 80, 1);
        gauge.setValue(-103.26);
        assertEquals("-103.3", gauge.getDisplayText());
        gauge = gauge(0, 10, 8, 2);
        gauge.setValue(2.126);
        assertEquals("2.13", gauge.getDisplayText());

        // wb1nernstVoltage: -0.2..1.1 V with 3 decimals. The dash's range-based 0.25 step
        // showed 0.45 as 0.550 and 0.838 as 0.800.
        gauge = new CircularGauge(-0.2, 1.1, model(-0.2, 1.1, 0.35, 0.40, 0.50, 0.55, 3));
        for (double[] sample : new double[][] {{0.45, 0.45}, {0.838, 0.838}, {0.3804, 0.38}, {-0.15, -0.15}}) {
            gauge.setValue(sample[0]);
            assertEquals(sample[1], gauge.getValue(), 1e-9);
        }
        gauge.setValue(0.838);
        assertEquals("0.838", gauge.getDisplayText());
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
        int progress = CircularGauge.PROGRESS.getRGB() & 0xFFFFFF;
        assertEquals(progress, rgb(image, 70, 150), "Filled side of the dial");
        assertNotEquals(progress, rgb(image, 330, 150), "Unfilled side of the dial");
        assertEquals(CircularGauge.NEEDLE.getRGB() & 0xFFFFFF, rgb(image, 200, 40), "Half-scale needle points straight up");
        assertNotEquals(CircularGauge.NEEDLE.getRGB() & 0xFFFFFF, rgb(image, 200, 120), "Hub hides the inner needle");
        assertEquals(0xE53935, rgb(image, 384, 200), "Danger ring at the end of the scale");
        gauge.setValue(80);
        image = render(gauge, 400, 400);
        assertEquals(CircularGauge.DANGER_FILL.getRGB() & 0xFFFFFF, rgb(image, 70, 150),
            "Danger starts at the threshold, inclusively");
        gauge.setHighDangerValue(100);
        image = render(gauge, 400, 400);
        // model() sets the high warning value equal to the original danger value.
        assertEquals(CircularGauge.WARNING_FILL.getRGB() & 0xFFFFFF, rgb(image, 70, 150),
            "Raising the danger threshold leaves the reading in the warning zone");
        gauge.setValue(50);
        assertEquals(progress, rgb(render(gauge, 400, 400), 70, 150));
    }

    @Test
    void reusesUnchangedArtworkAndInvalidatesForVisualChanges() {
        CircularGauge gauge = gauge(0, 100, 80, 0);
        gauge.setValue(50);
        render(gauge, 200, 200);
        render(gauge, 200, 200);
        assertEquals(1, gauge.getArtworkRenderCountForTests());
        gauge.setTitle("Updated title");
        render(gauge, 200, 200);
        assertEquals(2, gauge.getArtworkRenderCountForTests());
        gauge.setUnitString("rpm");
        render(gauge, 200, 200);
        assertEquals(3, gauge.getArtworkRenderCountForTests());
        gauge.setValue(60);
        render(gauge, 200, 200);
        assertEquals(4, gauge.getArtworkRenderCountForTests());
        render(gauge, 250, 250);
        assertEquals(5, gauge.getArtworkRenderCountForTests());
        BufferedImage image = new BufferedImage(500, 500, BufferedImage.TYPE_INT_ARGB_PRE);
        Graphics2D scaled = image.createGraphics();
        scaled.scale(2, 2);
        gauge.paint(scaled);
        scaled.dispose();
        assertEquals(6, gauge.getArtworkRenderCountForTests());
        gauge.setHighDangerValue(100);
        render(gauge, 250, 250);
        assertEquals(7, gauge.getArtworkRenderCountForTests());
    }

    @Test
    void fractionalDeviceOriginAndHalfPixelCenterMatchVectorPainting() {
        CircularGauge gauge = gauge(0, 100, 80, 0);
        gauge.setValue(55);
        gauge.setSize(201, 200);
        for (double[] transform : new double[][] {{1.25, 1.25, 0.3, 0.6}, {1.25, 1.5, 0.4, 0.2}}) {
            for (int[] clip : new int[][] {{0, 0, 350, 360}, {35, 48, 83, 76}}) {
                BufferedImage cached = new BufferedImage(400, 400, BufferedImage.TYPE_INT_ARGB_PRE);
                BufferedImage vector = new BufferedImage(400, 400, BufferedImage.TYPE_INT_ARGB_PRE);
                Graphics2D cachedGraphics = cached.createGraphics();
                Graphics2D vectorGraphics = vector.createGraphics();
                for (Graphics2D graphics : new Graphics2D[] {cachedGraphics, vectorGraphics}) {
                    graphics.scale(transform[0], transform[1]);
                    graphics.translate(transform[2], transform[3]);
                    graphics.setClip(clip[0], clip[1], clip[2], clip[3]);
                }
                gauge.paint(cachedGraphics);
                gauge.paintVectorForTests(vectorGraphics);
                cachedGraphics.dispose();
                vectorGraphics.dispose();
                assertArrayEquals(
                    cached.getRGB(0, 0, 400, 400, null, 0, 400),
                    vector.getRGB(0, 0, 400, 400, null, 0, 400),
                    "Fractional-origin cached rendering must preserve direct vector pixels and partial clips: "
                        + pixelDifferenceSummary(cached, vector));
            }
        }
    }

    @Test
    void zoneChangesInvalidateArtworkEvenWhenDisplayedValueDoesNotChange() {
        CircularGauge gauge = gauge(0, 100, 80, 0);
        gauge.setValue(79.9);
        render(gauge, 400, 400);
        assertEquals("80", gauge.getDisplayText());
        assertEquals(CircularGauge.Zone.NORMAL, gauge.getZone());
        gauge.setValue(80);
        BufferedImage danger = render(gauge, 400, 400);
        assertEquals("80", gauge.getDisplayText());
        assertEquals(2, gauge.getArtworkRenderCountForTests());
        assertEquals(CircularGauge.DANGER_FILL.getRGB() & 0xFFFFFF, rgb(danger, 70, 150));
    }

    @Test
    void oversizedDeviceTransformsUseVectorFallbackWithoutAllocatingHugeRasters() {
        CircularGauge gauge = gauge(0, 100, 80, 0);
        gauge.setSize(200, 200);
        for (int i = 0; i < 2; i++) {
            BufferedImage image = new BufferedImage(8, 8, BufferedImage.TYPE_INT_ARGB_PRE);
            Graphics2D graphics = image.createGraphics();
            graphics.scale(25, 25);
            gauge.paint(graphics);
            graphics.dispose();
        }
        assertEquals(2, gauge.getArtworkRenderCountForTests(), "Oversized targets must bypass the raster cache");
    }

    private static GaugeModel model(double min, double max, double lowDanger, double lowWarning,
                                    double highWarning, double highDanger, int decimals) {
        return new GaugeModel("ZoneTestGauge", "zoneTestChannel",
            IniValue.ofString("WBO1: nernst V"), IniValue.ofString("V"),
            IniValue.ofNumeric(min), IniValue.ofNumeric(max),
            IniValue.ofNumeric(lowDanger), IniValue.ofNumeric(lowWarning),
            IniValue.ofNumeric(highWarning), IniValue.ofNumeric(highDanger),
            IniValue.ofNumeric(decimals), IniValue.ofNumeric(decimals));
    }

    @Test
    void classifiesReadingsByTsWarningAndDangerLimits() {
        // afr1Gauge from gauge_declarations.ini
        CircularGauge afr = new CircularGauge(10, 19.4, model(10, 19.4, 12, 13, 15, 16, 2));
        for (Object[] sample : new Object[][] {
            {0.0, CircularGauge.Zone.NORMAL}, // disconnected sensor, below the scale: TS shows no alarm
            {11.0, CircularGauge.Zone.DANGER},
            {12.5, CircularGauge.Zone.WARNING},
            {14.0, CircularGauge.Zone.NORMAL},
            {15.5, CircularGauge.Zone.WARNING},
            {16.5, CircularGauge.Zone.DANGER},
            {25.0, CircularGauge.Zone.DANGER},
            {Double.NaN, CircularGauge.Zone.NORMAL},
        }) {
            afr.setValue((Double) sample[0]);
            assertEquals(sample[1], afr.getZone(), "AFR " + sample[0]);
        }

        // A low limit equal to the minimum adds no zone: a TPS-style 0..100 gauge reading 0 is normal.
        CircularGauge tps = new CircularGauge(0, 100, model(0, 100, 0, 0, 100, 100, 0));
        tps.setValue(0);
        assertEquals(CircularGauge.Zone.NORMAL, tps.getZone());
    }

    @Test
    void paintsWarningAndDangerZonesOnTheRim() {
        // wb1nernstVoltage from gauge_declarations.ini
        CircularGauge gauge = new CircularGauge(-0.2, 1.1, model(-0.2, 1.1, 0.35, 0.40, 0.50, 0.55, 3));
        BufferedImage image = render(gauge, 400, 400);
        assertEquals(CircularGauge.DANGER.getRGB() & 0xFFFFFF, rimRgb(image, gauge, 0.0), "Low danger");
        assertEquals(CircularGauge.WARNING.getRGB() & 0xFFFFFF, rimRgb(image, gauge, 0.375), "Low warning");
        assertNotEquals(CircularGauge.WARNING.getRGB() & 0xFFFFFF, rimRgb(image, gauge, 0.45), "Normal band");
        assertNotEquals(CircularGauge.DANGER.getRGB() & 0xFFFFFF, rimRgb(image, gauge, 0.45), "Normal band");
        assertEquals(CircularGauge.WARNING.getRGB() & 0xFFFFFF, rimRgb(image, gauge, 0.525), "High warning");
        assertEquals(CircularGauge.DANGER.getRGB() & 0xFFFFFF, rimRgb(image, gauge, 0.8), "High danger");

        gauge.setValue(0.45);
        assertEquals(CircularGauge.PROGRESS.getRGB() & 0xFFFFFF, rgb(render(gauge, 400, 400), 70, 150));
        gauge.setValue(0.52);
        assertEquals(CircularGauge.WARNING_FILL.getRGB() & 0xFFFFFF, rgb(render(gauge, 400, 400), 70, 150));
        gauge.setValue(0.838);
        assertEquals(CircularGauge.DANGER_FILL.getRGB() & 0xFFFFFF, rgb(render(gauge, 400, 400), 70, 150));
    }

    /** Colour of the zone ring at {@code value}'s position on a 400x400 render. */
    private static int rimRgb(BufferedImage image, CircularGauge gauge, double value) {
        double min = -0.2;
        double max = 1.1;
        double angle = Math.toRadians(135 + (value - min) / (max - min) * 270);
        return rgb(image, (int) Math.round(200 + 184 * Math.cos(angle)), (int) Math.round(200 + 184 * Math.sin(angle)));
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

    private static String pixelDifferenceSummary(BufferedImage first, BufferedImage second) {
        int differences = 0;
        int alphaDifferences = 0;
        int maxRgbDifference = 0;
        int maxDifferenceX = -1;
        int maxDifferenceY = -1;
        int minX = first.getWidth();
        int minY = first.getHeight();
        int maxX = -1;
        int maxY = -1;
        for (int y = 0; y < first.getHeight(); y++) {
            for (int x = 0; x < first.getWidth(); x++) {
                int a = first.getRGB(x, y);
                int b = second.getRGB(x, y);
                if (a != b) {
                    differences++;
                    minX = Math.min(minX, x);
                    minY = Math.min(minY, y);
                    maxX = Math.max(maxX, x);
                    maxY = Math.max(maxY, y);
                    if ((a >>> 24) != (b >>> 24)) {
                        alphaDifferences++;
                    }
                    int rgbDifference = Math.max(Math.abs(((a >>> 16) & 0xff) - ((b >>> 16) & 0xff)),
                        Math.max(Math.abs(((a >>> 8) & 0xff) - ((b >>> 8) & 0xff)), Math.abs((a & 0xff) - (b & 0xff))));
                    if (rgbDifference > maxRgbDifference) {
                        maxRgbDifference = rgbDifference;
                        maxDifferenceX = x;
                        maxDifferenceY = y;
                    }
                }
            }
        }
        return "differentPixels=" + differences + ", alphaDifferences=" + alphaDifferences
            + ", maxRgbDifference=" + maxRgbDifference + " at " + maxDifferenceX + "," + maxDifferenceY
            + ", bounds=" + minX + "," + minY + ".." + maxX + "," + maxY;
    }
}
