package com.rusefi.ui.widgets;

import org.junit.jupiter.api.Test;

import javax.swing.*;
import java.awt.*;
import java.awt.event.MouseEvent;
import java.awt.geom.Area;
import java.awt.image.BufferedImage;
import java.util.concurrent.atomic.AtomicInteger;
import java.util.concurrent.atomic.AtomicLong;

import static org.junit.jupiter.api.Assertions.*;

class DetachedGaugePanelTest {
    @Test
    void repaintClearsTheBufferBehindTransparentEdges() throws Exception {
        SwingUtilities.invokeAndWait(() -> {
            JPanelWithListener wrapper = new JPanelWithListener(new BorderLayout());
            CircularGauge gauge = new CircularGauge(0, 100, CircularGaugeTest.model(0, 100, 80, 0));
            wrapper.add(gauge);
            DetachedGaugePanel panel = new DetachedGaugePanel(wrapper, () -> { }, shape -> { });
            panel.setSize(320, 320);
            panel.doLayout();
            wrapper.doLayout();
            BufferedImage reused = new BufferedImage(320, 320, BufferedImage.TYPE_INT_ARGB);
            Graphics2D g = reused.createGraphics();
            g.setColor(Color.WHITE);
            g.fillRect(0, 0, 320, 320);
            g.dispose();
            try {
                for (int value : new int[] {44, 80, 0, 44}) {
                    gauge.setValue(value);
                    g = reused.createGraphics();
                    panel.paint(g);
                    g.dispose();
                    BufferedImage fresh = CircularGaugeTest.render(gauge, 320, 320);
                    assertArrayEquals(fresh.getRGB(0, 0, 320, 320, null, 0, 320),
                        reused.getRGB(0, 0, 320, 320, null, 0, 320),
                        "Repainting must preserve the alpha of a fresh render, including every edge pixel");
                }
            } finally {
                panel.dispose();
            }
        });
    }

    @Test
    void nativeMaskPreservesAntialiasedEdgePixels() throws Exception {
        SwingUtilities.invokeAndWait(() -> {
            JPanelWithListener wrapper = new JPanelWithListener(new BorderLayout());
            CircularGauge gauge = new CircularGauge(0, 100, CircularGaugeTest.model(0, 100, 80, 0));
            gauge.setValue(44);
            wrapper.add(gauge);
            DetachedGaugePanel panel = new DetachedGaugePanel(wrapper, () -> { }, shape -> { });
            try {
                for (int size : new int[] {320, 333, 600, 1200}) {
                    panel.setSize(size, size);
                    panel.doLayout();
                    wrapper.doLayout();
                    for (double scale : new double[] {1, 1.25, 2}) {
                        int pixels = (int) Math.ceil(size * scale);
                        BufferedImage face = new BufferedImage(pixels, pixels, BufferedImage.TYPE_INT_ARGB);
                        Graphics2D g = face.createGraphics();
                        g.scale(scale, scale);
                        gauge.paint(g);
                        g.dispose();

                        // Native window shaping is a binary clip, without alpha coverage.
                        BufferedImage mask = new BufferedImage(pixels, pixels, BufferedImage.TYPE_BYTE_BINARY);
                        g = mask.createGraphics();
                        g.scale(scale, scale);
                        g.setColor(Color.WHITE);
                        g.setRenderingHint(RenderingHints.KEY_ANTIALIASING, RenderingHints.VALUE_ANTIALIAS_OFF);
                        g.fill(panel.windowShape());
                        g.dispose();
                        int clipped = 0;
                        for (int y = 0; y < pixels; y++) {
                            for (int x = 0; x < pixels; x++) {
                                if ((face.getRGB(x, y) >>> 24) != 0 && (mask.getRGB(x, y) & 0xFFFFFF) == 0) {
                                    clipped++;
                                }
                            }
                        }
                        assertEquals(0, clipped, "The native mask must preserve every painted rim pixel at "
                            + size + "px, scale " + scale);
                    }
                }
            } finally {
                panel.dispose();
            }
        });
    }

    @Test
    void windowShapeIncludesOnlyTheDialAndVisibleCloseButton() throws Exception {
        SwingUtilities.invokeAndWait(() -> {
            AtomicLong now = new AtomicLong();
            DetachedGaugeHover hover = new DetachedGaugeHover(now::get);
            JPanelWithListener wrapper = new JPanelWithListener(new BorderLayout());
            JLabel gauge = new JLabel("Gauge");
            wrapper.add(gauge);
            AtomicInteger closed = new AtomicInteger();
            DetachedGaugePanel panel = new DetachedGaugePanel(wrapper, closed::incrementAndGet, shape -> { }, hover);
            JButton close = (JButton) panel.getComponent(0);
            try {
                for (int size : new int[] {320, 600, 1200}) {
                    panel.setSize(size, size);
                    panel.doLayout();
                    Shape circle = panel.windowShape();
                    assertTrue(circle.contains(size / 2.0, size / 2.0));
                    assertFalse(circle.contains(0, 0));
                    Area overlap = new Area(circle);
                    overlap.intersect(new Area(close.getBounds()));
                    assertTrue(overlap.isEmpty(), "Close control must be outside the circle");
                    enter(gauge);
                    assertTrue(close.isVisible());
                    assertTrue(panel.windowShape().contains(close.getBounds()));
                    assertFalse(panel.windowShape().contains(0, 0));
                    now.addAndGet(1_000_000_000L);
                    hover.expire();
                    assertFalse(close.isVisible());
                    assertFalse(panel.windowShape().intersects(close.getBounds()));
                }
                enter(gauge);
                close.doClick();
                assertEquals(1, closed.get());
            } finally {
                panel.dispose();
            }
        });
    }

    @Test
    void hoverSurvivesGaugeReplacementAndDisposalRemovesListeners() throws Exception {
        SwingUtilities.invokeAndWait(() -> {
            AtomicLong now = new AtomicLong();
            DetachedGaugeHover hover = new DetachedGaugeHover(now::get);
            JPanelWithListener wrapper = new JPanelWithListener(new BorderLayout());
            JLabel oldGauge = new JLabel("Old gauge");
            wrapper.add(oldGauge);
            DetachedGaugePanel panel = new DetachedGaugePanel(wrapper, () -> { }, shape -> { }, hover);
            panel.setSize(320, 320);
            panel.doLayout();
            JButton close = (JButton) panel.getComponent(0);
            JLabel newGauge = new JLabel("New gauge");
            try {
                wrapper.removeAllChildrenAndListeners();
                assertEquals(0, oldGauge.getMouseListeners().length);
                assertEquals(0, oldGauge.getMouseMotionListeners().length);
                wrapper.add(newGauge);
                enter(newGauge);
                assertTrue(close.isVisible());
                now.set(900_000_000);
                enter(close);
                now.set(1_000_000_000);
                hover.expire();
                assertTrue(close.isVisible(), "Moving from the gauge to its close button renews visibility");
            } finally {
                panel.dispose();
            }
            assertFalse(close.isVisible());
            assertEquals(0, newGauge.getMouseListeners().length);
            assertEquals(0, newGauge.getMouseMotionListeners().length);
            enter(newGauge);
            assertFalse(close.isVisible());
        });
    }

    private static void enter(Component component) {
        component.dispatchEvent(new MouseEvent(component, MouseEvent.MOUSE_ENTERED, 0, 0, 10, 10, 0, false));
    }
}
