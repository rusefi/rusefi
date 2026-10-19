package com.rusefi.ui;

import com.rusefi.core.preferences.storage.Node;
import com.rusefi.ui.widgets.BarGauge;
import com.rusefi.ui.widgets.DigitalGauge;
import com.rusefi.ui.widgets.GaugeDisplayKind;
import org.junit.jupiter.api.Test;

import javax.swing.*;
import java.awt.*;

import static org.junit.jupiter.api.Assertions.*;

class GaugesGridElementTest {
    @Test
    void displayAsMenuListsEveryPresentationAndChecksTheCurrentOne() throws Throwable {
        SwingUtilities.invokeAndWait(() -> {
            Node config = new Node();
            GaugesGridElement element = GaugesGridElement.create(new UIContext(), config, "RPMGauge");
            try {
                JMenu menu = element.createDisplayAsMenu(GaugeDisplayKind.ROUND);
                assertEquals(GaugesGridElement.DISPLAY_AS, menu.getText());
                assertEquals(GaugeDisplayKind.values().length, menu.getItemCount());
                String[] labels = {"Round gauge", "Text", "Horizontal bar", "Vertical bar", "Live graph"};
                for (int i = 0; i < labels.length; i++) {
                    assertEquals(labels[i], menu.getItem(i).getText());
                    assertEquals(i == 0, menu.getItem(i).isSelected());
                }

                menu.getItem(0).doClick(0);
                assertNull(config.getProperty(GaugesGridElement.DISPLAY, null), "Choosing the current mode changes nothing");

                menu.getItem(4).doClick(0);
                assertEquals("graph", config.getProperty(GaugesGridElement.DISPLAY));
                assertTrue(config.getBoolProperty("type"), "Older consoles still see a live graph");
                assertEquals(2, element.getContent().getComponentCount(), "Live graph mode shows two graphs");
                assertTrue(element.getContent().getComponent(0) instanceof SensorLiveGraph);
                assertTrue(element.createDisplayAsMenu(GaugeDisplayKind.LIVE_GRAPH).getItem(4).isSelected());

                element.setDisplayKind(GaugeDisplayKind.VERTICAL_BAR);
                assertEquals("vbar", config.getProperty(GaugesGridElement.DISPLAY));
                assertFalse(config.getBoolProperty("type"), "Older consoles fall back to a round gauge");
                assertEquals(GaugeDisplayKind.VERTICAL_BAR, element.getDisplayKind());
            } finally {
                element.destroy();
            }
        });
    }

    @Test
    void textAndBarPresentationsAreBuiltFromTheStoredKind() throws Throwable {
        SwingUtilities.invokeAndWait(() -> {
            Node config = new Node();
            config.setProperty("gauge", "RPMGauge");
            GaugesGridElement element = GaugesGridElement.create(GaugesPanelLayoutTest.createContext(), config, "RPMGauge");
            try {
                element.setDisplayKind(GaugeDisplayKind.TEXT);
                assertTrue(gauge(element) instanceof DigitalGauge);
                element.setDisplayKind(GaugeDisplayKind.HORIZONTAL_BAR);
                assertTrue(gauge(element) instanceof BarGauge);
                element.setDisplayKind(GaugeDisplayKind.ROUND);
                assertFalse(gauge(element) instanceof BarGauge);
            } finally {
                element.destroy();
            }
        });
    }

    /**
     * Switching a slot to Live graph used to add the two graphs without laying them out, unlike
     * the round gauge path. They stayed zero-sized until some other layout pass, which users saw
     * as a delay before the graph appeared.
     */
    @Test
    void switchingToLiveGraphLaysOutTheGraphsAtOnce() throws Throwable {
        SwingUtilities.invokeAndWait(() -> {
            Node config = new Node();
            GaugesGridElement element = GaugesGridElement.create(GaugesPanelLayoutTest.createContext(), config, "RPMGauge");
            try {
                JPanel wrapper = element.getContent();
                wrapper.setSize(400, 300);
                element.setDisplayKind(GaugeDisplayKind.LIVE_GRAPH);
                Component top = wrapper.getComponent(0);
                Component bottom = wrapper.getComponent(1);
                assertEquals(new Rectangle(0, 0, 400, 150), top.getBounds(), "Sized immediately, still empty");
                assertEquals(new Rectangle(0, 150, 400, 150), bottom.getBounds());
            } finally {
                element.destroy();
            }
        });
    }

    @Test
    void legacyFlagStillSelectsTheLiveGraph() throws Throwable {
        SwingUtilities.invokeAndWait(() -> {
            Node config = new Node();
            config.setProperty("type", true);
            GaugesGridElement element = GaugesGridElement.create(new UIContext(), config, "RPMGauge");
            try {
                assertEquals(GaugeDisplayKind.LIVE_GRAPH, element.getDisplayKind());
                config.setProperty(GaugesGridElement.DISPLAY, "unknown");
                assertEquals(GaugeDisplayKind.LIVE_GRAPH, element.getDisplayKind(), "Unknown kinds fall back");
            } finally {
                element.destroy();
            }
        });
    }

    private static Component gauge(GaugesGridElement element) {
        return element.getContent().getComponent(0);
    }
}
