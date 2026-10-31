package com.rusefi.ui;

import com.opensr5.ini.GaugeModel;
import com.opensr5.ini.IniFileModelMocks;
import com.opensr5.ini.IniValue;
import com.rusefi.core.SensorCentral;
import com.rusefi.core.preferences.storage.Node;
import org.junit.jupiter.api.Test;
import org.junit.jupiter.api.io.TempDir;

import javax.swing.*;
import java.awt.*;
import java.io.IOException;
import java.io.Reader;
import java.io.Writer;
import java.nio.charset.StandardCharsets;
import java.nio.file.Files;
import java.nio.file.Path;
import java.util.Properties;

import static org.junit.jupiter.api.Assertions.*;

class GaugesPanelLayoutTest {
    @TempDir
    Path directory;

    @Test
    void restoresSelectedGaugeAndOffersLayoutFileControls() throws Exception {
        UIContext context = createContext();
        Node config = new Node();
        config.setProperty("gauges_rows", 1);
        config.setProperty("gauges_cols", 1);
        config.getChild("element_0").setProperty("gauge", "CustomGauge");
        SwingUtilities.invokeAndWait(() -> {
            GaugesPanel panel = new GaugesPanel(context, config);
            try {
                panel.setActive(true);
                assertTrue(hasDemand("layoutCustomChannel"));
                assertFalse(hasDemand("layoutDefaultChannel"));
                assertNotNull(findButton(panel.getContent(), "Save Layout..."));
                assertNotNull(findButton(panel.getContent(), "Load Layout..."));
            } finally {
                panel.destroy();
            }
        });
        assertFalse(hasDemand("layoutCustomChannel"));
        assertFalse(hasDemand("layoutDefaultChannel"));
    }

    @Test
    void loadingReplacesGridAndSubscriptionsAndPreservesOtherPreferences() throws Exception {
        Path file = directory.resolve("custom.gauges");
        Node source = singleGauge("CustomGauge");
        source.setProperty("gauges_cols", 2);
        source.getChild("element_1").setProperty("gauge", "RPMGauge");
        source.getChild("element_1").setProperty("type", true);
        source.getChild("element_1").getChild("top").setProperty("sensor", "layoutGraphChannel");
        GaugeLayout.save(file, source);

        Node config = singleGauge("RPMGauge");
        config.setProperty("command", "keep command");
        config.getChild("warnings").setProperty("setting", "keep warning");
        config.getChild("element_8").setProperty("gauge", "stale hidden gauge");
        SwingUtilities.invokeAndWait(() -> {
            GaugesPanel panel = new GaugesPanel(createContext(), config);
            try {
                panel.setActive(true);
                assertTrue(hasDemand("layoutDefaultChannel"));
                panel.loadLayout(file);
                assertTrue(hasDemand("layoutCustomChannel"));
                assertTrue(hasDemand("layoutGraphChannel"));
                assertFalse(hasDemand("layoutDefaultChannel"));
                assertEquals(2, config.getIntProperty("gauges_cols", 0));
                assertFalse(config.getConfig().containsKey("element_8"));
                assertEquals("keep command", config.getProperty("command"));
                assertEquals("keep warning", config.getChild("warnings").getProperty("setting"));
                panel.setActive(false);
                panel.loadLayout(file);
                assertFalse(hasDemand("layoutCustomChannel"));
                assertFalse(hasDemand("layoutGraphChannel"));
                panel.saveLayout(directory.resolve("resaved.gauges"));
            } catch (IOException e) {
                throw new AssertionError(e);
            } finally {
                panel.destroy();
            }
        });
        assertFalse(hasDemand("layoutCustomChannel"));
        assertFalse(hasDemand("layoutGraphChannel"));
        Node restored = GaugeLayout.load(directory.resolve("resaved.gauges"));
        assertEquals("CustomGauge", restored.getChild("element_0").getProperty("gauge"));
    }

    @Test
    void roundTripPreservesBothGraphsAndExcludesUnrelatedPreferences() throws Exception {
        Node config = singleGauge("CustomGauge");
        Node element = config.getChild("element_0");
        element.setProperty("type", true);
        Node top = element.getChild("top");
        top.setProperty("sensor", "Temperature\u00b0Gauge");
        top.setProperty("period", 50);
        top.setProperty("auto_scale", true);
        top.setProperty("lower", -40.5);
        top.setProperty("upper", 150.0);
        Node bottom = element.getChild("bottom");
        bottom.setProperty("sensor", "RPMGauge");
        bottom.setProperty("period", 1000);
        bottom.setProperty("auto_scale", false);
        bottom.setProperty("lower", 0.0);
        bottom.setProperty("upper", 8000.0);
        config.setProperty("command", "unrelated");
        config.getChild("element_1").setProperty("gauge", "hidden");
        Path file = directory.resolve("graphs.gauges");
        GaugeLayout.save(file, config);
        Node loaded = GaugeLayout.load(file);
        assertEquals(1, loaded.getIntProperty("gauges_rows", 0));
        assertEquals(1, loaded.getIntProperty("gauges_cols", 0));
        Node loadedElement = loaded.getChild("element_0");
        assertEquals("CustomGauge", loadedElement.getProperty("gauge"));
        assertTrue(loadedElement.getBoolProperty("type"));
        assertEquals(top.getConfig(), loadedElement.getChild("top").getConfig());
        assertEquals(bottom.getConfig(), loadedElement.getChild("bottom").getConfig());
        String text = new String(Files.readAllBytes(file), StandardCharsets.UTF_8);
        assertFalse(text.contains("unrelated"));
        assertFalse(text.contains("hidden"));
    }

    @Test
    void malformedOrUnsupportedFilesLeaveTheCurrentLayoutAndDemandIntact() throws Exception {
        Path file = directory.resolve("invalid.gauges");
        GaugeLayout.save(file, singleGauge("CustomGauge"));
        Properties valid = new Properties();
        try (Reader reader = Files.newBufferedReader(file, StandardCharsets.UTF_8)) {
            valid.load(reader);
        }
        String[][] invalidSettings = {
            {"format", "other"}, {"version", "2"}, {"gauges_rows", "0"},
            {"gauges_rows", "4"}, {"gauges_cols", "6"}, {"gauges_cols", "1.5"},
            {"gauges_rows", "2147483647"}, {"element_0.gauge", ""},
            {"element_0.type", "maybe"}, {"element_0.top.period", "-1"},
            {"element_0.bottom.auto_scale", "yes"}, {"element_0.top.lower", "NaN"},
            {"element_0.bottom.upper", "Infinity"}, {"element_0.top.sensor", " "}
        };
        Node config = singleGauge("RPMGauge");
        SwingUtilities.invokeAndWait(() -> {
            GaugesPanel panel = new GaugesPanel(createContext(), config);
            try {
                panel.setActive(true);
                for (String[] setting : invalidSettings) {
                    Properties invalid = new Properties();
                    invalid.putAll(valid);
                    invalid.setProperty(setting[0], setting[1]);
                    try (Writer writer = Files.newBufferedWriter(file, StandardCharsets.UTF_8)) {
                        invalid.store(writer, "invalid");
                    }
                    assertThrows(IOException.class, () -> panel.loadLayout(file), setting[0]);
                    assertEquals("RPMGauge", config.getChild("element_0").getProperty("gauge"));
                    assertEquals(1, config.getIntProperty("gauges_cols", 0));
                    assertTrue(hasDemand("layoutDefaultChannel"));
                    assertFalse(hasDemand("layoutCustomChannel"));
                }
                Files.write(file, "not a gauge layout".getBytes(StandardCharsets.UTF_8));
                assertThrows(IOException.class, () -> panel.loadLayout(file));
                assertThrows(IOException.class, () -> panel.loadLayout(directory.resolve("missing.gauges")));
            } catch (IOException e) {
                throw new AssertionError(e);
            } finally {
                panel.destroy();
            }
        });
    }

    private static Node singleGauge(String name) {
        Node config = new Node();
        config.setProperty("gauges_rows", 1);
        config.setProperty("gauges_cols", 1);
        config.getChild("element_0").setProperty("gauge", name);
        return config;
    }

    static UIContext createContext() {
        IniFileModelMocks.GaugeRegistry gauges = IniFileModelMocks.mutableWithGauges();
        register(gauges, "RPMGauge", "layoutDefaultChannel");
        register(gauges, "CustomGauge", "layoutCustomChannel");
        UIContext context = new UIContext();
        context.iniFileState.setIniFileModelForTest(gauges.model);
        return context;
    }

    private static void register(IniFileModelMocks.GaugeRegistry gauges, String name, String channel) {
        gauges.register(new GaugeModel(name, channel,
            IniValue.ofExpression(name), IniValue.ofExpression("units"),
            IniValue.ofNumeric(0), IniValue.ofNumeric(8000),
            IniValue.ofNumeric(0), IniValue.ofNumeric(1000),
            IniValue.ofNumeric(7000), IniValue.ofNumeric(8000),
            IniValue.ofNumeric(0), IniValue.ofNumeric(0)));
    }

    static boolean hasDemand(String channel) {
        return SensorCentral.getInstance().getOutputChannelDemand().getChannels().contains(channel.toLowerCase());
    }

    private static JButton findButton(Container container, String text) {
        for (Component child : container.getComponents()) {
            if (child instanceof JButton && text.equals(((JButton) child).getText())) {
                return (JButton) child;
            }
            if (child instanceof Container) {
                JButton found = findButton((Container) child, text);
                if (found != null) {
                    return found;
                }
            }
        }
        return null;
    }
}
