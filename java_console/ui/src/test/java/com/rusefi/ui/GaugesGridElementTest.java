package com.rusefi.ui;

import com.opensr5.ini.GaugeCategoryModel;
import com.opensr5.ini.IniFileModel;
import com.rusefi.core.preferences.storage.Node;
import com.rusefi.ui.widgets.BarGauge;
import com.rusefi.ui.widgets.DigitalGauge;
import com.rusefi.ui.widgets.GaugeDisplayKind;
import com.rusefi.ui.widgets.GaugePopups;
import com.rusefi.ui.widgets.GaugeScaleOverrides;
import org.junit.jupiter.api.Test;

import javax.swing.*;
import java.awt.*;
import java.awt.event.InputEvent;
import java.awt.event.MouseEvent;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Collections;
import java.util.List;

import static org.junit.jupiter.api.Assertions.*;
import static org.mockito.Mockito.when;

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

    @Test
    void scaleSettingsAreStoredPerSlotAndDroppedWhenTheGaugeChanges() throws Throwable {
        List<JPopupMenu> shown = new ArrayList<>();
        GaugePopups.setShownForUnitTest(shown::add);
        try {
            SwingUtilities.invokeAndWait(() -> {
                UIContext context = GaugesPanelLayoutTest.createContext();
                IniFileModel ini = context.iniFileState.getIniFileModel();
                GaugeCategoryModel category = new GaugeCategoryModel("Test",
                    Arrays.asList(ini.getGauge("RPMGauge"), ini.getGauge("CustomGauge")));
                when(ini.getGaugeCategories()).thenReturn(Collections.singletonMap("Test", category));
                Node config = new Node();
                config.setProperty("gauge", "RPMGauge");
                GaugesGridElement element = GaugesGridElement.create(context, config, "RPMGauge");
                try {
                    GaugeScaleDialog dialog = element.createScaleDialog();
                    assertEquals("0", dialog.min.getClientProperty("JTextField.placeholderText"));
                    assertEquals("8000", dialog.max.getClientProperty("JTextField.placeholderText"),
                        "Expression units are not shown");
                    assertEquals("", dialog.max.getText(), "Nothing overridden yet");
                    assertEquals("INI default (0)", dialog.decimals.getSelectedItem());

                    element.setScaleOverrides(new GaugeScaleOverrides(null, 6000.0, 5500.5, 1));
                    assertEquals("6000.0", config.getProperty(GaugeScaleOverrides.MAX));
                    assertNull(config.getProperty(GaugeScaleOverrides.MIN, null));
                    dialog = element.createScaleDialog();
                    assertEquals("6000", dialog.max.getText());
                    assertEquals("5500.5", dialog.danger.getText());
                    assertEquals("1", dialog.decimals.getSelectedItem());
                    assertEquals(new GaugeScaleOverrides(null, 6000.0, 5500.5, 1), dialog.parse().overrides);
                    dialog.reset.doClick(0);
                    assertEquals(GaugeScaleOverrides.NONE, dialog.parse().overrides, "Reset clears every field");

                    ((AbstractButton) ((Container) gauge(element)).getComponent(0)).doClick(0);
                    assertEquals(Arrays.asList(GaugesGridElement.DISPLAY_AS, GaugesGridElement.SCALE,
                        "Pop out to window"), visibleItems(shown.get(shown.size() - 1)), "The gear holds the display options");

                    rightClick(gauge(element));
                    JPopupMenu menu = shown.get(shown.size() - 1);
                    assertEquals(Collections.singletonList("Test"), visibleItems(menu),
                        "Right-click lists the gauge groups directly");

                    findItem(menu, "CustomGauge").doClick(0);
                    assertEquals("CustomGauge", config.getProperty("gauge"));
                    assertTrue(GaugeScaleOverrides.read(config).isEmpty(), "Settings for RPM do not carry over");
                } finally {
                    element.destroy();
                }
            });
        } finally {
            GaugePopups.setShownForUnitTest(null);
        }
    }

    @Test
    void scaleDialogNeedsTheIniDefinition() throws Throwable {
        SwingUtilities.invokeAndWait(() -> {
            GaugesGridElement element = GaugesGridElement.create(new UIContext(), new Node(), "RPMGauge");
            try {
                assertNull(element.createScaleDialog(), "No .ini gauge definition yet");
            } finally {
                element.destroy();
            }
        });
    }

    private static void rightClick(Component target) {
        target.dispatchEvent(new MouseEvent(target, MouseEvent.MOUSE_PRESSED, System.currentTimeMillis(),
            InputEvent.BUTTON3_DOWN_MASK, 10, 10, 1, true, MouseEvent.BUTTON3));
    }

    private static List<String> visibleItems(JPopupMenu menu) {
        List<String> items = new ArrayList<>();
        for (Component child : menu.getComponents()) {
            if (child instanceof JMenuItem && child.isVisible()) {
                items.add(((JMenuItem) child).getText());
            }
        }
        return items;
    }

    private static JMenuItem findItem(MenuElement menu, String text) {
        for (MenuElement child : menu.getSubElements()) {
            if (child instanceof JMenuItem && !(child instanceof JMenu) && text.equals(((JMenuItem) child).getText())) {
                return (JMenuItem) child;
            }
            JMenuItem found = findItem(child, text);
            if (found != null) {
                return found;
            }
        }
        return null;
    }

    private static Component gauge(GaugesGridElement element) {
        return element.getContent().getComponent(0);
    }
}
