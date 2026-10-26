package com.rusefi.ui;

import com.opensr5.ini.GaugeModel;
import com.opensr5.ini.IniFileMetaInfoImpl;
import com.opensr5.ini.IniFileModel;
import com.opensr5.ini.RawIniFile;
import com.rusefi.binaryprotocol.BinaryProtocol;
import com.rusefi.core.preferences.storage.Node;
import com.rusefi.ini.reader.IniFileReaderUtil;
import com.rusefi.core.SensorCategory;
import com.rusefi.ui.widgets.GaugeOptionsButton;
import org.junit.jupiter.api.Test;

import javax.swing.*;
import java.awt.Component;
import java.awt.Graphics2D;
import java.awt.image.BufferedImage;
import java.io.InputStream;
import java.util.ArrayList;
import java.util.List;

import static org.junit.jupiter.api.Assertions.*;
import static org.mockito.Mockito.mock;
import static org.mockito.Mockito.when;

class SensorLiveGraphTest {
    @Test
    void gaugeMenuUsesLoadedIniWithoutAnEcuConnection() throws Throwable {
        IniFileModel ini;
        try (InputStream input = getClass().getResourceAsStream("/january.ini")) {
            assertNotNull(input);
            RawIniFile raw = IniFileReaderUtil.read(input, "/january.ini");
            ini = IniFileReaderUtil.readIniFile(raw, "/january.ini", new IniFileMetaInfoImpl(raw));
        }
        List<String> expectedPaths = new ArrayList<>();
        ini.getGaugeCategories().forEach((category, model) -> model.getGauges().forEach(
            gauge -> expectedPaths.add(category + " / "
                + (gauge.getTitleValue().isExpression() ? gauge.getName() : gauge.getTitle()))));
        assertTrue(expectedPaths.size() > 100, "Exercise the sandbox's full INI catalog");

        SwingUtilities.invokeAndWait(() -> {
            UIContext context = new UIContext();
            context.iniFileState.setIniFileModelForTest(ini);
            assertNull(context.getBinaryProtocol());
            SensorLiveGraph graph = new SensorLiveGraph(context, new Node(), "RPMGauge", null);
            try {
                graph.setActive(false);
                JPopupMenu menu = new JPopupMenu();
                graph.addChangeSensorItems(menu);
                assertEquals(expectedPaths, gaugePaths(menu));

                BinaryProtocol protocol = mock(BinaryProtocol.class);
                when(protocol.getIniFileNullable()).thenReturn(ini);
                context.getLinkManager().setBinaryProtocolForTests(protocol);
                JPopupMenu connectedMenu = new JPopupMenu();
                graph.addChangeSensorItems(connectedMenu);
                assertEquals(expectedPaths, gaugePaths(connectedMenu));
            } finally {
                graph.destroy();
            }
        });
    }

    @Test
    void rightClickListsGaugeGroupsAndTheGearHoldsGraphOptions() throws Throwable {
        SwingUtilities.invokeAndWait(() -> {
            UIContext context = new UIContext();
            JMenu displayAs = new JMenu(GaugesGridElement.DISPLAY_AS);
            SensorLiveGraph graph = new SensorLiveGraph(context, new Node(), "RPMGauge", displayAs);
            try {
                graph.setActive(false);
                JPopupMenu menu = graph.createPopupMenu();
                assertTrue(menu.getComponent(0) instanceof JPanel, "Search field is at the top");
                List<String> groups = new ArrayList<>();
                for (SensorCategory category : SensorCategory.values()) {
                    groups.add(category.getName());
                }
                assertEquals(groups, visibleItems(menu), "Without an .ini, the built-in groups are listed directly");

                assertTrue(graph.getComponent(0) instanceof GaugeOptionsButton);
                assertEquals(java.util.Arrays.asList("Refresh period", "Scale", GaugesGridElement.DISPLAY_AS),
                    visibleItems(graph.createOptionsMenu()));
            } finally {
                graph.destroy();
            }
        });
    }

    private static List<String> visibleItems(JPopupMenu menu) {
        List<String> items = new ArrayList<>();
        for (Component component : menu.getComponents()) {
            if (component instanceof JMenuItem && component.isVisible()) {
                items.add(((JMenuItem) component).getText());
            }
        }
        return items;
    }

    /**
     * Output channels are polled only when something subscribes to them by channel name.
     * The graph used to subscribe with its gauge name, so on its own it never got data: it
     * showed readings only when another widget happened to subscribe to the same channel.
     */
    @Test
    void graphSubscribesToItsChannelAndFollowsGaugeChanges() throws Throwable {
        SwingUtilities.invokeAndWait(() -> {
            SensorLiveGraph graph = new SensorLiveGraph(GaugesPanelLayoutTest.createContext(), new Node(), "CustomGauge", null);
            try {
                graph.setActive(true);
                assertTrue(GaugesPanelLayoutTest.hasDemand("layoutCustomChannel"));
                assertFalse(GaugesPanelLayoutTest.hasDemand("CustomGauge"), "The gauge name is not a channel");

                graph.setSensor("RPMGauge");
                assertTrue(GaugesPanelLayoutTest.hasDemand("layoutDefaultChannel"));
                assertFalse(GaugesPanelLayoutTest.hasDemand("layoutCustomChannel"), "The old channel is released");

                graph.setActive(false);
                assertFalse(GaugesPanelLayoutTest.hasDemand("layoutDefaultChannel"), "Hidden graphs stop polling");
                graph.setActive(true);
            } finally {
                graph.destroy();
            }
            assertFalse(GaugesPanelLayoutTest.hasDemand("layoutDefaultChannel"));
        });
    }

    @Test
    void scaleComesFromTheLoadedIniEvenWithoutAnEcu() throws Throwable {
        SwingUtilities.invokeAndWait(() -> {
            UIContext context = GaugesPanelLayoutTest.createContext();
            assertNull(context.getBinaryProtocol());
            SensorLiveGraph graph = new SensorLiveGraph(context, new Node(), "RPMGauge", null);
            try {
                graph.setActive(false);
                GaugeModel model = context.iniFileState.getIniFileModel().getGauge("RPMGauge");
                SensorLiveGraph.VisibleRange range = graph.getRange(new double[] {1000, 2000}, model);
                assertEquals(0, range.minValue, "Previously 0..0 offline, which drew nothing");
                assertEquals(8000, range.maxValue);

                range = graph.getRange(new double[] {Double.NaN, 1000, 2000}, null);
                assertTrue(range.minValue < 1000 && range.maxValue > 2000, "No bounds: fit the readings, skipping gaps");
                range = graph.getRange(new double[] {Double.NaN}, null);
                assertTrue(range.maxValue > range.minValue, "No readings still gives a usable scale");
                range = graph.getRange(new double[] {5, 5}, null);
                assertTrue(range.minValue < 5 && range.maxValue > 5, "A flat signal stays in the middle");
            } finally {
                graph.destroy();
            }
        });
    }

    @Test
    void drawsAModernCardWithASmoothLineAndBreaksAtGaps() throws Throwable {
        SwingUtilities.invokeAndWait(() -> {
            SensorLiveGraph graph = new SensorLiveGraph(GaugesPanelLayoutTest.createContext(), new Node(), "RPMGauge", null);
            try {
                graph.setActive(false);
                assertFalse(graph.isOpaque(), "Rounded corners show what is behind the card");
                assertNull(graph.getBorder(), "No black frame");
                BufferedImage empty = render(graph);
                assertEquals(0, amber(empty), "No readings, no line");
                assertTrue(grayText(empty) > 20, "An empty graph says it is waiting for data");
                assertEquals(0, empty.getRGB(0, 0) >>> 24);

                for (int i = 0; i < 31; i++) {
                    graph.addValue(i == 15 ? Double.NaN : 1000 + 100 * i);
                }
                assertTrue(amber(render(graph)) > 200, "The line and its fill are drawn");
                for (int size : new int[] {10, 39, 40, 60, 90}) {
                    graph.setSize(size, size);
                    render(graph, size, size);
                    render(graph, size * 5, size);
                }
            } finally {
                graph.destroy();
            }
        });
    }

    private static BufferedImage render(SensorLiveGraph graph) {
        return render(graph, 400, 200);
    }

    private static BufferedImage render(SensorLiveGraph graph, int width, int height) {
        graph.setSize(width, height);
        BufferedImage image = new BufferedImage(width, height, BufferedImage.TYPE_INT_ARGB);
        Graphics2D g = image.createGraphics();
        graph.paint(g);
        g.dispose();
        return image;
    }

    /** Gray text pixels in the middle of the plot, where the waiting message goes. */
    private static int grayText(BufferedImage image) {
        int count = 0;
        for (int y = image.getHeight() * 2 / 5; y < image.getHeight() * 4 / 5; y++) {
            for (int x = image.getWidth() / 3; x < image.getWidth() * 2 / 3; x++) {
                int argb = image.getRGB(x, y);
                int r = (argb >> 16) & 0xFF;
                int g = (argb >> 8) & 0xFF;
                int b = argb & 0xFF;
                if ((argb >>> 24) == 255 && r < 190 && Math.abs(r - g) < 20 && Math.abs(g - b) < 20) {
                    count++;
                }
            }
        }
        return count;
    }

    /** Pixels of the amber line and the strongest part of its fill. */
    private static int amber(BufferedImage image) {
        int count = 0;
        for (int y = 0; y < image.getHeight(); y++) {
            for (int x = 0; x < image.getWidth(); x++) {
                int argb = image.getRGB(x, y);
                int r = (argb >> 16) & 0xFF;
                int g = (argb >> 8) & 0xFF;
                int b = argb & 0xFF;
                if ((argb >>> 24) == 255 && r > 200 && g > 140 && g < 215 && b < 120) {
                    count++;
                }
            }
        }
        return count;
    }

    private static List<String> gaugePaths(JPopupMenu menu) {
        List<String> paths = new ArrayList<>();
        for (Component component : menu.getComponents()) {
            JMenu category = (JMenu) component;
            for (Component child : category.getMenuComponents()) {
                paths.add(category.getText() + " / " + ((JMenuItem) child).getText());
            }
        }
        return paths;
    }
}
