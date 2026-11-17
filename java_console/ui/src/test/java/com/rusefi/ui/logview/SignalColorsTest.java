package com.rusefi.ui.logview;

import com.rusefi.core.preferences.storage.Node;
import com.rusefi.logview.CsvLogReader;
import org.junit.jupiter.api.Test;

import javax.swing.JCheckBoxMenuItem;
import javax.swing.JMenu;
import javax.swing.JMenuItem;
import javax.swing.JPopupMenu;
import java.awt.Color;
import java.awt.Component;
import java.awt.Rectangle;
import java.awt.image.BufferedImage;
import java.io.IOException;
import java.io.StringReader;
import java.util.concurrent.atomic.AtomicInteger;

import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertFalse;
import static org.junit.jupiter.api.Assertions.assertNotNull;
import static org.junit.jupiter.api.Assertions.assertNull;
import static org.junit.jupiter.api.Assertions.assertTrue;

class SignalColorsTest {
    private static final int RPM = 1;
    private static final int CLT = 2;

    private static CsvLogReader log() throws IOException {
        return CsvLogReader.read(new StringReader("Time\tRPM\tCLT\ns\trpm\tC\n0\t800\t80\n1\t900\t81\n"));
    }

    private static LogViewerPanel panel(SignalColors colors) throws IOException {
        LogViewerPanel panel = new LogViewerPanel(log(), null, new ChartSettings(), new SignalLists(),
                new SidebarState(), colors, null, null);
        panel.chart().plotLayout().clearPlot(0); // the test starts from an empty plot
        return panel;
    }

    private static SwatchGrid grid(Component[] components) {
        for (Component component : components) {
            if (component instanceof SwatchGrid) {
                return (SwatchGrid) component;
            }
        }
        throw new AssertionError("no swatches");
    }

    private static JMenuItem item(Component[] components, String text) {
        for (Component component : components) {
            if (component instanceof JMenuItem && text.equals(((JMenuItem) component).getText())) {
                return (JMenuItem) component;
            }
        }
        return null;
    }

    @Test
    void aSwatchFollowsTheLightOrDarkPlotACustomColourDoesNot() {
        SignalColors colors = new SignalColors();
        assertNull(colors.resolve("RPM", PlotPalette.LIGHT), "automatic until chosen");

        colors.setSwatch("RPM", 9);
        assertEquals(PlotPalette.LIGHT.trace(9), colors.resolve("RPM", PlotPalette.LIGHT));
        assertEquals(PlotPalette.DARK.trace(9), colors.resolve("RPM", PlotPalette.DARK));

        colors.setCustom("RPM", new Color(0x123456));
        assertEquals(new Color(0x123456), colors.resolve("RPM", PlotPalette.LIGHT));
        assertEquals(new Color(0x123456), colors.resolve("RPM", PlotPalette.DARK));
        assertEquals(-1, colors.swatch("RPM"));

        colors.setAutomatic("RPM");
        assertNull(colors.resolve("RPM", PlotPalette.LIGHT));
    }

    @Test
    void choicesAreKeptByNameInTheConfig() {
        Node node = new Node();
        new SignalColors(node).setSwatch("RPM", 3);
        new SignalColors(node).setCustom("CLT", new Color(0xabcdef));
        SignalColors reloaded = new SignalColors(node);
        assertEquals(3, reloaded.swatch("RPM"));
        assertEquals(new Color(0xabcdef), reloaded.custom("CLT"));

        node.setProperty("TPS", "swatch:99");
        node.setProperty("MAP", "#zz");
        assertNull(reloaded.resolve("TPS", PlotPalette.LIGHT), "a corrupt choice is automatic");
        assertNull(reloaded.resolve("MAP", PlotPalette.LIGHT));
    }

    @Test
    void everyChangeIsReported() {
        SignalColors colors = new SignalColors();
        AtomicInteger changes = new AtomicInteger();
        colors.addListener(changes::incrementAndGet);
        colors.setSwatch("RPM", 1);
        colors.setCustom("RPM", Color.RED);
        colors.setAutomatic("RPM");
        assertEquals(3, changes.get());
    }

    @Test
    void pickingASwatchRecoloursTheTraceEverywhere() throws IOException {
        SignalColors colors = new SignalColors();
        LogViewerPanel panel = panel(colors);
        panel.toggle(RPM);
        LogChart chart = panel.chart();
        Color automatic = chart.colorOf(RPM, chart.palette());
        assertEquals(chart.palette().trace(0), automatic);

        AtomicInteger repaints = new AtomicInteger();
        chart.addColorListener(repaints::incrementAndGet);
        JPopupMenu menu = chart.colorMenu(RPM);
        assertTrue(((JCheckBoxMenuItem) item(menu.getComponents(), "Automatic")).isSelected());
        grid(menu.getComponents()).pick(10);

        assertEquals(chart.palette().trace(10), chart.colorOf(RPM, chart.palette()));
        assertEquals(10, colors.swatch("RPM"));
        assertEquals(1, repaints.get(), "the list and the Stats table repaint");
        assertFalse(((JCheckBoxMenuItem) item(chart.colorMenu(RPM).getComponents(), "Automatic")).isSelected());

        item(chart.colorMenu(RPM).getComponents(), "Automatic").doClick();
        assertEquals(automatic, chart.colorOf(RPM, chart.palette()));
    }

    @Test
    void customAsksForAColourAndCancelKeepsTheOldOne() throws IOException {
        SignalColors colors = new SignalColors();
        LogViewerPanel panel = panel(colors);
        panel.toggle(RPM);
        LogChart chart = panel.chart();

        chart.customColorPrompt = (label, initial) -> null;
        item(chart.colorMenu(RPM).getComponents(), "Custom…").doClick();
        assertNull(colors.resolve("RPM", chart.palette()), "cancelled");

        chart.customColorPrompt = (label, initial) -> new Color(0x336699);
        item(chart.colorMenu(RPM).getComponents(), "Custom…").doClick();
        assertEquals(new Color(0x336699), chart.colorOf(RPM, chart.palette()));
        assertTrue(((JCheckBoxMenuItem) item(chart.colorMenu(RPM).getComponents(), "Custom…")).isSelected());
    }

    @Test
    void theSignalMenuHasAColourSubmenuAndTheChoiceCarriesToTheNextLog() throws IOException {
        SignalColors colors = new SignalColors();
        LogViewerPanel panel = panel(colors);
        LogViewerPanelTest.selectSignal(panel, CLT);
        JPopupMenu menu = panel.rowMenu(panel.fieldList().getSelectedValue());
        JMenu colour = null;
        for (Component component : menu.getComponents()) {
            if (component instanceof JMenu && "Colour".equals(((JMenu) component).getText())) {
                colour = (JMenu) component;
            }
        }
        assertNotNull(colour, "right-click a signal: Colour ▸");
        grid(colour.getMenuComponents()).pick(4);
        assertEquals(4, colors.swatch("CLT"), "an unplotted signal can be coloured before plotting it");

        LogViewerPanel next = panel(colors);
        next.toggle(CLT);
        assertEquals(next.chart().palette().trace(4), next.chart().colorOf(CLT, next.chart().palette()));
    }

    @Test
    void theLegendColourMarkOpensThePickerAndTheLegendHasAMenu() throws IOException {
        LogViewerPanel panel = panel(new SignalColors());
        panel.toggle(RPM);
        LogChart chart = panel.chart();
        chart.setSize(700, 400);
        chart.paint(new BufferedImage(700, 400, BufferedImage.TYPE_INT_RGB).createGraphics());
        LogChart.LegendItem item = chart.bands().get(0).legend.get(0);
        Rectangle mark = LogChart.colorMark(item);
        assertTrue(mark.contains(item.bounds.x + 6, item.bounds.y + 6), "the colour bar is in the mark");
        assertFalse(mark.contains(item.bounds.x + 40, item.bounds.y + 6), "the name is not: a click there pins");

        JPopupMenu legend = chart.legendMenu(RPM);
        assertNotNull(item(legend.getComponents(), "Remove from plot"));
        assertNotNull(item(legend.getComponents(), "Colour"));
    }
}
