package com.rusefi.ui.widgets.tune;

import com.rusefi.ui.widgets.CircularGauge;
import org.junit.jupiter.api.Test;

import java.awt.Color;

import static org.junit.jupiter.api.Assertions.*;

/** The bar behind a live value fills by where the value sits between the gauge's min and max. */
class LiveValueBarTest {
    @Test
    void fillsByPositionInRange() {
        assertEquals(0.0, LiveValueTrace.fraction(800, 800, 7800));
        assertEquals(0.5, LiveValueTrace.fraction(4300, 800, 7800));
        assertEquals(1.0, LiveValueTrace.fraction(7800, 800, 7800));
    }

    @Test
    void valuesOutsideTheRangeStayAtTheEnds() {
        assertEquals(0.0, LiveValueTrace.fraction(-40, 0, 100));
        assertEquals(1.0, LiveValueTrace.fraction(250, 0, 100));
    }

    @Test
    void noFillWithoutAValueOrARange() {
        assertTrue(Double.isNaN(LiveValueTrace.fraction(Double.NaN, 0, 100)));
        assertTrue(Double.isNaN(LiveValueTrace.fraction(50, 0, 0)));
        assertTrue(Double.isNaN(LiveValueTrace.fraction(50, 100, 0)));
        assertTrue(Double.isNaN(LiveValueTrace.fraction(50, Double.NaN, 100)));
    }

    @Test
    void withoutARangeItStaysAPlainLabel() {
        LiveValueBar plain = new LiveValueBar("12.5 V");
        int plainWidth = plain.getPreferredSize().width;
        plain.setRange(0, 0);
        assertFalse(plain.trace().hasRange());
        assertEquals(plainWidth, plain.getPreferredSize().width);
    }

    @Test
    void theBarKeepsItsWidthWhileTheTextChanges() {
        LiveValueBar bar = new LiveValueBar("---");
        bar.setRange(0, 8000);
        bar.setText("7999 RPM");
        assertEquals(LiveValueBar.BAR_WIDTH, bar.getPreferredSize().width);
        bar.setValue(2000);
        assertEquals(0.25, bar.trace().fraction());
    }

    @Test
    void followsTheGaugeWarningAndDangerLimits() {
        // afr1Gauge from gauge_declarations.ini, as in CircularGaugeTest
        LiveValueBar afr = new LiveValueBar("---");
        afr.setRange(10, 19.4);
        afr.setZones(CircularGauge.Zones.of(10, 19.4, 12, 13, 15, 16));
        Color normal = afr.getForeground();
        for (Object[] sample : new Object[][] {
            {0.0, CircularGauge.Zone.BELOW_SCALE}, // disconnected sensor below the scale: "no sensor?", not danger
            {11.0, CircularGauge.Zone.DANGER},
            {12.5, CircularGauge.Zone.WARNING},
            {14.0, CircularGauge.Zone.NORMAL},
            {15.5, CircularGauge.Zone.WARNING},
            {16.5, CircularGauge.Zone.DANGER},
            {Double.NaN, CircularGauge.Zone.NORMAL},
        }) {
            afr.setValue((Double) sample[0]);
            assertEquals(sample[1], afr.trace().zone(), "AFR " + sample[0]);
            assertEquals(sample[1] == CircularGauge.Zone.NORMAL, normal.equals(afr.getForeground()), "text colour at AFR " + sample[0]);
        }
    }

    @Test
    void limitsNeedARange() {
        LiveValueBar bar = new LiveValueBar("---");
        bar.setZones(CircularGauge.Zones.of(10, 19.4, 12, 13, 15, 16));
        bar.setValue(11);
        assertEquals(CircularGauge.Zone.NORMAL, bar.trace().zone());
    }

    @Test
    void keepsOnlyTheLastFewSeconds() {
        LiveValueBar rpm = new LiveValueBar("---");
        rpm.setRange(0, 8000);
        for (long t = 0; t <= 10_000; t += 100) {
            rpm.setValue(t % 200 == 0 ? 850 : 950, t);
        }
        // samples from 5000 ms to 10000 ms inclusive
        assertEquals(LiveValueTrace.HISTORY_MS / 100 + 1, rpm.trace().historySize());
    }

    @Test
    void noHistoryWithoutARange() {
        LiveValueBar plain = new LiveValueBar("---");
        plain.setValue(900, 0);
        plain.setValue(950, 100);
        assertEquals(0, plain.trace().historySize());
    }

    @Test
    void paintsTheHistoryIncludingGapsWithoutFailing() {
        LiveValueBar afr = new LiveValueBar("14.7 AFR");
        afr.setRange(10, 19.4);
        afr.setZones(CircularGauge.Zones.of(10, 19.4, 12, 13, 15, 16));
        double[] samples = {14.7, 15.2, Double.NaN, 13.9, 16.5};
        for (int i = 0; i < samples.length; i++) {
            afr.setValue(samples[i], i * 100L);
        }
        afr.setSize(LiveValueBar.BAR_WIDTH, 20);
        java.awt.image.BufferedImage image = new java.awt.image.BufferedImage(
            LiveValueBar.BAR_WIDTH, 20, java.awt.image.BufferedImage.TYPE_INT_ARGB);
        java.awt.Graphics2D g = image.createGraphics();
        try {
            afr.paint(g);
        } finally {
            g.dispose();
        }
    }

    @Test
    void historyZoomsToRecentSpreadButNotIntoNoise() {
        LiveValueBar rpm = new LiveValueBar("---");
        rpm.setRange(0, 8000);
        rpm.setValue(850, 0);
        rpm.setValue(950, 100);
        // idle hunting of 100 RPM spans most of the line's height (the 160 RPM minimum, centred)
        assertArrayEquals(new double[]{820, 980}, rpm.trace().historyScale(), 1e-9);
        rpm.setValue(1300, 200);
        // a wider swing zooms out to fit
        assertArrayEquals(new double[]{850, 1300}, rpm.trace().historyScale(), 1e-9);

        LiveValueBar steady = new LiveValueBar("---");
        steady.setRange(0, 8000);
        steady.setValue(2500, 0);
        steady.setValue(2502, 100);
        // 2 RPM of noise is drawn within a 160 RPM (2%) span, so it stays flat
        assertArrayEquals(new double[]{2421, 2581}, steady.trace().historyScale(), 1e-9);

        LiveValueBar atEnd = new LiveValueBar("---");
        atEnd.setRange(0, 100);
        atEnd.setValue(100, 0);
        // the span stays inside the bar's range
        assertArrayEquals(new double[]{98, 100}, atEnd.trace().historyScale(), 1e-9);
    }
}
