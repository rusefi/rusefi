package com.rusefi.ui;

import com.opensr5.ini.GaugeModel;
import com.opensr5.ini.IniFileModelMocks;
import com.opensr5.ini.IniValue;
import com.rusefi.core.preferences.storage.Node;
import org.junit.jupiter.api.Test;

import javax.swing.*;
import java.awt.Component;

import static org.junit.jupiter.api.Assertions.*;

/**
 * The graph's custom upper / lower limits (gear > Scale). Reproduces the current, wrong behaviour; the fix
 * updates these expectations.
 */
class SensorLiveGraphCustomScaleTest {
    private static final double[] READINGS = {20, 30};

    @Test
    void customLimitsStayWhenTheGraphSwitchesToAnotherGauge() throws Throwable {
        SwingUtilities.invokeAndWait(() -> {
            UIContext context = context();
            Node config = new Node();
            config.setProperty("upper", 6000.0);
            config.setProperty("lower", 1000.0);
            SensorLiveGraph graph = graph(context, config, "RPMGauge");
            try {
                graph.setSensor("TempGauge");
                SensorLiveGraph.VisibleRange range = graph.getRange(READINGS, gauge(context, "TempGauge"));
                // BUG: the RPM limits now scale a -40..140 temperature graph
                assertEquals(1000, range.minValue);
                assertEquals(6000, range.maxValue);
            } finally {
                graph.destroy();
            }
        });
    }

    @Test
    void theMenuDoesNotShowThatACustomLimitIsSetAndCannotClearIt() throws Throwable {
        SwingUtilities.invokeAndWait(() -> {
            UIContext context = context();
            Node config = new Node();
            config.setProperty("upper", 6000.0);
            SensorLiveGraph graph = graph(context, config, "RPMGauge");
            try {
                JMenu scale = scaleMenu(graph);
                JCheckBoxMenuItem upper = (JCheckBoxMenuItem) item(scale, "Use custom upper");
                // BUG: a custom upper is in use, yet the item is unchecked
                assertFalse(upper.isSelected());
                // BUG: there is no item that removes a custom limit; unchecking does nothing either
                assertNull(item(scale, "Reset to gauge range"));
            } finally {
                graph.destroy();
            }
        });
    }

    @Test
    void cancellingTheLimitDialogThrows() {
        // JOptionPane.showInputDialog returns null on Cancel, which the menu passes to Node.isNumeric
        // BUG: Double.parseDouble(null) throws NullPointerException, not NumberFormatException
        assertThrows(NullPointerException.class, () -> Node.isNumeric(null));
    }

    @Test
    void aLowerLimitAboveTheUpperOneSilentlyZoomsToTheReadings() throws Throwable {
        SwingUtilities.invokeAndWait(() -> {
            UIContext context = context();
            Node config = new Node();
            config.setProperty("upper", 50.0);
            config.setProperty("lower", 100.0);
            SensorLiveGraph graph = graph(context, config, "RPMGauge");
            try {
                SensorLiveGraph.VisibleRange range = graph.getRange(READINGS, gauge(context, "RPMGauge"));
                // BUG: neither the limits nor the gauge range: the readings' own range, with no message
                assertTrue(range.minValue > 15 && range.maxValue < 35);
            } finally {
                graph.destroy();
            }
        });
    }

    private static UIContext context() {
        IniFileModelMocks.GaugeRegistry gauges = IniFileModelMocks.mutableWithGauges();
        gauges.register(gauge("RPMGauge", "rpmChannel", 0, 8000));
        gauges.register(gauge("TempGauge", "tempChannel", -40, 140));
        UIContext context = new UIContext();
        context.iniFileState.setIniFileModelForTest(gauges.model);
        return context;
    }

    private static GaugeModel gauge(String name, String channel, double min, double max) {
        return new GaugeModel(name, channel, IniValue.ofString(name), IniValue.ofString("units"),
            IniValue.ofNumeric(min), IniValue.ofNumeric(max), IniValue.ofNumeric(min), IniValue.ofNumeric(min),
            IniValue.ofNumeric(max), IniValue.ofNumeric(max), IniValue.ofNumeric(0), IniValue.ofNumeric(0));
    }

    private static GaugeModel gauge(UIContext context, String name) {
        return context.iniFileState.getIniFileModel().getGauge(name);
    }

    private static SensorLiveGraph graph(UIContext context, Node config, String gauge) {
        SensorLiveGraph graph = new SensorLiveGraph(context, config, gauge, null);
        graph.setActive(false);
        return graph;
    }

    private static JMenu scaleMenu(SensorLiveGraph graph) {
        for (Component component : graph.createOptionsMenu().getComponents()) {
            if (component instanceof JMenu && "Scale".equals(((JMenu) component).getText())) {
                return (JMenu) component;
            }
        }
        throw new AssertionError("no Scale menu");
    }

    private static JMenuItem item(JMenu menu, String text) {
        for (Component component : menu.getMenuComponents()) {
            if (component instanceof JMenuItem && text.equals(((JMenuItem) component).getText())) {
                return (JMenuItem) component;
            }
        }
        return null;
    }
}
