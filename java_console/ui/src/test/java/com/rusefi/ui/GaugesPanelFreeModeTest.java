package com.rusefi.ui;

import com.rusefi.core.preferences.storage.Node;
import org.junit.jupiter.api.Test;
import org.junit.jupiter.api.io.TempDir;

import javax.swing.*;
import java.awt.*;
import java.awt.geom.Rectangle2D;
import java.io.IOException;
import java.io.Reader;
import java.io.Writer;
import java.nio.charset.StandardCharsets;
import java.nio.file.Files;
import java.nio.file.Path;
import java.util.Properties;

import static com.rusefi.ui.GaugesPanelLayoutTest.createContext;
import static com.rusefi.ui.GaugesPanelLayoutTest.hasDemand;
import static org.junit.jupiter.api.Assertions.*;

class GaugesPanelFreeModeTest {
    @TempDir
    Path directory;

    @Test
    void everySwitchToFreeStartsFromTheCurrentGrid() throws Exception {
        Node config = twoColumnGrid();
        // A free layout left over from an earlier, larger grid.
        config.setProperty("free_count", 15);
        for (int i = 0; i < 15; i++) {
            config.getChild("free_" + i).setProperty("gauge", "CustomGauge");
        }
        SwingUtilities.invokeAndWait(() -> {
            GaugesPanel panel = new GaugesPanel(createContext(), config);
            try {
                panel.setActive(true);
                assertFalse(panel.isFreeMode(), "Grid is the default");
                assertTrue(panel.getGridSizeMenu().isEnabled());
                assertEquals("Grid layout · 1 × 2", panel.getLayoutModeLabel().getText(), "the toolbar shows the current layout");

                panel.setFreeMode(true);
                assertTrue(panel.isFreeMode());
                assertEquals("free", config.getProperty("gauges_mode"));
                assertFalse(panel.getGridSizeMenu().isEnabled(), "Grid Size only applies to the grid");
                assertEquals("Free layout", panel.getLayoutModeLabel().getText());
                assertEquals(2, config.getIntProperty("free_count", 0), "The current grid replaces the old free layout");
                assertFalse(config.getConfig().containsKey("free_2"));
                assertEquals("CustomGauge", config.getChild("free_0").getProperty("gauge"));
                assertEquals("RPMGauge", config.getChild("free_1").getProperty("gauge"));
                assertTrue(config.getChild("free_1").getBoolProperty("type"), "Display mode is copied");
                assertEquals(new Rectangle2D.Double(0, 0, 0.5, 1), GaugeLayout.getBounds(config.getChild("free_0")));
                assertEquals(new Rectangle2D.Double(0.5, 0, 0.5, 1), GaugeLayout.getBounds(config.getChild("free_1")));
                assertTrue(hasDemand("layoutCustomChannel"));

                JPanel free = panel.getFreePanel();
                free.setSize(400, 300);
                free.doLayout();
                assertEquals(2, free.getComponentCount());
                assertEquals(new Rectangle(200, 0, 200, 300), free.getComponent(0).getBounds(),
                    "The later gauge is first in Swing order, so it paints on top");
                assertFalse(free.isOptimizedDrawingEnabled(), "Overlapping gauges repaint in stacking order");
                assertFalse(free.getComponent(0).isOpaque(), "An overlapped gauge shows around the dial");

                // A free-only change: the grid must not see it, and Grid drops the free layout.
                config.getChild("free_0").setProperty("gauge", "RPMGauge");
                GaugeLayout.setBounds(config.getChild("free_1"), new Rectangle2D.Double(0.25, 0.25, 0.5, 0.5));
                panel.setFreeMode(false);
                assertFalse(panel.isFreeMode());
                assertEquals("CustomGauge", config.getChild("element_0").getProperty("gauge"));
                assertFalse(config.getConfig().containsKey("free_count"));
                assertFalse(config.getConfig().containsKey("free_0"));
                assertTrue(hasDemand("layoutCustomChannel"));

                panel.setFreeMode(true);
                assertEquals("CustomGauge", config.getChild("free_0").getProperty("gauge"),
                    "Returning to Free starts from the grid again");
                assertEquals(new Rectangle2D.Double(0.5, 0, 0.5, 1), GaugeLayout.getBounds(config.getChild("free_1")));
            } finally {
                panel.destroy();
            }
        });
        assertFalse(hasDemand("layoutDefaultChannel"));
    }

    @Test
    void restoresFreeModeAndKeepsDamagedBoundsOnScreen() throws Exception {
        Node config = twoColumnGrid();
        config.setProperty("gauges_mode", "free");
        config.setProperty("free_count", 1);
        Node element = config.getChild("free_0");
        element.setProperty("gauge", "CustomGauge");
        element.setProperty("x", 0.9);
        element.setProperty("y", -3.0);
        element.setProperty("w", 0.5);
        element.setProperty("h", "NaN");
        SwingUtilities.invokeAndWait(() -> {
            GaugesPanel panel = new GaugesPanel(createContext(), config);
            try {
                assertTrue(panel.isFreeMode());
                assertEquals(new Rectangle2D.Double(0.5, 0, 0.5, GaugeLayout.MIN_FREE_SIZE),
                    GaugeLayout.getBounds(element));
                assertEquals(1, panel.getFreePanel().getComponentCount());
            } finally {
                panel.destroy();
            }
        });
    }

    @Test
    void layoutFilesRoundTripTheFreeLayoutAndStayVersionOneWithoutIt() throws Exception {
        Path gridFile = directory.resolve("grid.gauges");
        GaugeLayout.save(gridFile, twoColumnGrid());
        assertEquals("1", readProperties(gridFile).getProperty("version"),
            "Grid-only layouts stay readable by older consoles");
        Node leftover = twoColumnGrid();
        GaugeLayout.seedFreeFromGrid(leftover);
        GaugeLayout.save(gridFile, leftover);
        assertEquals("1", readProperties(gridFile).getProperty("version"),
            "A free layout left over in Grid mode is not saved");

        Node config = twoColumnGrid();
        GaugeLayout.seedFreeFromGrid(config);
        config.setProperty("gauges_mode", "free");
        Node top = config.getChild("free_1").getChild("top");
        top.setProperty("sensor", "layoutGraphChannel");
        config.getChild("free_0").setProperty("display", "hbar");
        GaugeLayout.setBounds(config.getChild("free_1"), new Rectangle2D.Double(0.1, 0.2, 0.3, 0.4));
        Path freeFile = directory.resolve("free.gauges");
        GaugeLayout.save(freeFile, config);
        assertEquals("2", readProperties(freeFile).getProperty("version"));

        Node loaded = GaugeLayout.load(freeFile);
        assertEquals("free", loaded.getProperty("gauges_mode"));
        assertEquals(2, loaded.getIntProperty("free_count", 0));
        assertEquals("CustomGauge", loaded.getChild("free_0").getProperty("gauge"));
        assertEquals("hbar", loaded.getChild("free_0").getProperty("display"));
        assertTrue(loaded.getChild("free_1").getBoolProperty("type"));
        assertEquals(top.getConfig(), loaded.getChild("free_1").getChild("top").getConfig());
        assertEquals(new Rectangle2D.Double(0.1, 0.2, 0.3, 0.4), GaugeLayout.getBounds(loaded.getChild("free_1")));
        assertEquals("CustomGauge", loaded.getChild("element_0").getProperty("gauge"), "The grid is saved too");
    }

    @Test
    void loadingSwitchesModesAndAVersionOneFileReturnsToGrid() throws Exception {
        Node source = twoColumnGrid();
        GaugeLayout.seedFreeFromGrid(source);
        source.setProperty("gauges_mode", "free");
        Path freeFile = directory.resolve("free.gauges");
        GaugeLayout.save(freeFile, source);
        Path gridFile = directory.resolve("grid.gauges");
        GaugeLayout.save(gridFile, twoColumnGrid());

        Node config = twoColumnGrid();
        SwingUtilities.invokeAndWait(() -> {
            GaugesPanel panel = new GaugesPanel(createContext(), config);
            try {
                panel.loadLayout(freeFile);
                assertTrue(panel.isFreeMode());
                assertEquals(2, panel.getFreePanel().getComponentCount());

                panel.loadLayout(gridFile);
                assertFalse(panel.isFreeMode());
                assertFalse(config.getConfig().containsKey("free_count"), "The old free layout is discarded");
                assertFalse(config.getConfig().containsKey("free_0"));
            } catch (IOException e) {
                throw new AssertionError(e);
            } finally {
                panel.destroy();
            }
        });
    }

    @Test
    void invalidFreeLayoutsAreRejected() throws Exception {
        Node config = twoColumnGrid();
        GaugeLayout.seedFreeFromGrid(config);
        config.setProperty("gauges_mode", "free");
        Path file = directory.resolve("free.gauges");
        GaugeLayout.save(file, config);
        Properties valid = readProperties(file);
        String[][] invalidSettings = {
            {"version", "3"}, {"gauges_mode", "other"}, {"free_count", "-1"}, {"free_count", "65"},
            {"free_count", "3"}, {"free_0.gauge", " "}, {"free_0.x", "1.5"}, {"free_0.x", "abc"},
            {"free_0.w", "0"}, {"free_0.h", "0.01"}, {"free_1.x", "0.75"}, {"free_1.type", "maybe"}, {"free_1.display", "dial"}
        };
        for (String[] setting : invalidSettings) {
            Properties invalid = new Properties();
            invalid.putAll(valid);
            invalid.setProperty(setting[0], setting[1]);
            writeProperties(file, invalid);
            assertThrows(IOException.class, () -> GaugeLayout.load(file), setting[0] + "=" + setting[1]);
        }
        Properties missingMode = new Properties();
        missingMode.putAll(valid);
        missingMode.remove("gauges_mode");
        writeProperties(file, missingMode);
        assertThrows(IOException.class, () -> GaugeLayout.load(file));
    }

    private static Node twoColumnGrid() {
        Node config = new Node();
        config.setProperty("gauges_rows", 1);
        config.setProperty("gauges_cols", 2);
        config.getChild("element_0").setProperty("gauge", "CustomGauge");
        config.getChild("element_1").setProperty("gauge", "RPMGauge");
        config.getChild("element_1").setProperty("type", true);
        return config;
    }

    private static Properties readProperties(Path file) throws IOException {
        Properties properties = new Properties();
        try (Reader reader = Files.newBufferedReader(file, StandardCharsets.UTF_8)) {
            properties.load(reader);
        }
        return properties;
    }

    private static void writeProperties(Path file, Properties properties) throws IOException {
        try (Writer writer = Files.newBufferedWriter(file, StandardCharsets.UTF_8)) {
            properties.store(writer, "invalid");
        }
    }
}
