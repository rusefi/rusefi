package com.rusefi.ui;

import com.rusefi.core.preferences.storage.Node;
import org.junit.jupiter.api.Test;

import javax.swing.*;
import java.awt.geom.Rectangle2D;

import static com.rusefi.ui.GaugesPanelLayoutTest.createContext;
import static com.rusefi.ui.GaugesPanelLayoutTest.hasDemand;
import static org.junit.jupiter.api.Assertions.*;

class GaugesPanelResetTest {
    @Test
    void gridResetReturnsToTheDefaultGaugesAndKeepsOtherPreferences() throws Exception {
        Node config = customGrid();
        config.setProperty("command", "keep command");
        SwingUtilities.invokeAndWait(() -> {
            GaugesPanel panel = new GaugesPanel(createContext(), config);
            try {
                panel.setActive(true);
                assertTrue(hasDemand("layoutCustomChannel"));
                panel.resetLayout();
                assertFalse(panel.isFreeMode());
                assertEquals(3, config.getIntProperty("gauges_rows", 0));
                assertEquals(3, config.getIntProperty("gauges_cols", 0));
                assertEquals("RPMGauge", config.getChild("element_0").getProperty("gauge"));
                assertEquals("internalMcuTemperatureGauge", config.getChild("element_1").getProperty("gauge"));
                assertEquals("VSSGauge", config.getChild("element_8").getProperty("gauge"));
                assertNull(config.getChild("element_0").getProperty("display", null), "Back to round gauges");
                assertFalse(config.getConfig().containsKey("element_9"), "Extra slots are dropped");
                assertFalse(hasDemand("layoutCustomChannel"));
                assertTrue(hasDemand("layoutDefaultChannel"));
                assertEquals("keep command", config.getProperty("command"));
            } finally {
                panel.destroy();
            }
        });
    }

    @Test
    void freeResetReturnsToTheGridItStartedFromAndStaysFree() throws Exception {
        Node config = customGrid();
        SwingUtilities.invokeAndWait(() -> {
            GaugesPanel panel = new GaugesPanel(createContext(), config);
            try {
                panel.setFreeMode(true);
                FreeGaugeArea area = panel.getFreeArea();
                area.setEditing(true);
                area.removeGauge(area.getItem(0));
                area.addGauge("RPMGauge");
                area.addGauge("RPMGauge");
                assertEquals(3, area.getGaugeCount());

                panel.resetLayout();
                assertTrue(panel.isFreeMode(), "Reset keeps the current mode");
                assertEquals(2, area.getGaugeCount(), "Back to the grid's gauges");
                assertEquals("CustomGauge", config.getChild("free_0").getProperty("gauge"));
                assertEquals("text", config.getChild("free_0").getProperty("display"), "The grid's presentation too");
                assertEquals(new Rectangle2D.Double(0, 0, 0.5, 1), GaugeLayout.getBounds(config.getChild("free_0")));
                assertEquals(new Rectangle2D.Double(0.5, 0, 0.5, 1), GaugeLayout.getBounds(config.getChild("free_1")));
                assertFalse(config.getConfig().containsKey("free_2"));
                assertEquals("CustomGauge", config.getChild("element_0").getProperty("gauge"), "The grid is untouched");
                assertEquals(2, config.getIntProperty("gauges_cols", 0));
            } finally {
                panel.destroy();
            }
        });
    }

    private static Node customGrid() {
        Node config = new Node();
        config.setProperty("gauges_rows", 1);
        config.setProperty("gauges_cols", 2);
        config.getChild("element_0").setProperty("gauge", "CustomGauge");
        config.getChild("element_0").setProperty("display", "text");
        config.getChild("element_1").setProperty("gauge", "RPMGauge");
        // Left over from a larger grid.
        config.getChild("element_9").setProperty("gauge", "CustomGauge");
        return config;
    }
}
