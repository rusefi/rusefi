package com.rusefi.ui;

import com.rusefi.core.preferences.storage.Node;
import org.junit.jupiter.api.Test;

import javax.swing.*;

import static org.junit.jupiter.api.Assertions.*;

class GaugesGridElementTest {
    @Test
    void displayAsMenuChecksTheCurrentModeAndSwitches() throws Throwable {
        SwingUtilities.invokeAndWait(() -> {
            Node config = new Node();
            GaugesGridElement element = GaugesGridElement.create(new UIContext(), config, "RPMGauge");
            try {
                JMenu menu = element.createDisplayAsMenu(false);
                assertEquals(GaugesGridElement.DISPLAY_AS, menu.getText());
                JMenuItem round = menu.getItem(0);
                JMenuItem graph = menu.getItem(1);
                assertEquals(GaugesGridElement.ROUND_GAUGE, round.getText());
                assertEquals(GaugesGridElement.LIVE_GRAPH, graph.getText());
                assertTrue(round.isSelected());
                assertFalse(graph.isSelected());

                round.doClick(0);
                assertFalse(config.getBoolProperty("type"), "Choosing the current mode changes nothing");

                graph.doClick(0);
                assertTrue(config.getBoolProperty("type"));
                assertEquals(2, element.getContent().getComponentCount(), "Live graph mode shows two graphs");
                assertTrue(element.getContent().getComponent(0) instanceof SensorLiveGraph);

                JMenu graphMenu = element.createDisplayAsMenu(true);
                assertTrue(graphMenu.getItem(1).isSelected());
                graphMenu.getItem(0).doClick(0);
                assertFalse(config.getBoolProperty("type"));
                assertFalse(element.getContent().getComponent(0) instanceof SensorLiveGraph);
            } finally {
                element.destroy();
            }
        });
    }
}
