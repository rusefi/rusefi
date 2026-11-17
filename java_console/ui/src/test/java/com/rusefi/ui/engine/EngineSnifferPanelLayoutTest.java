package com.rusefi.ui.engine;

import com.rusefi.core.preferences.storage.Node;
import com.rusefi.ui.UIContext;
import com.rusefi.ui.util.UiUtils;
import com.rusefi.ui.widgets.JoinedButtons;
import org.junit.jupiter.api.Test;

import javax.swing.*;
import java.awt.*;
import java.util.List;

import static org.junit.jupiter.api.Assertions.*;

class EngineSnifferPanelLayoutTest {
    private static final String CHART = "r!1199!64224414!Injector 2!u!64225149!Injector 2!d!64225303!,";

    @Test
    void toolbarFollowsTheConsoleStyleAndEmptyStateTracksCaptures() throws Throwable {
        EngineSnifferPanel[] panel = new EngineSnifferPanel[1];
        Node config = new Node();
        SwingUtilities.invokeAndWait(() -> {
            panel[0] = new EngineSnifferPanel(new UIContext(), config);
            List<Component> all = UiUtils.getAllComponents(panel[0].getPanel());

            JButton clear = button(all, "clear");
            assertTrue(clear.getParent() instanceof JoinedButtons);
            assertSame(clear.getParent(), button(all, "save image").getParent());
            assertSame(clear.getParent(), button(all, "pause").getParent());
            JButton reset = button(all, "100%");
            assertTrue(reset.getParent() instanceof JoinedButtons, "Zoom in, reset and out form one group");
            assertFalse(reset.isEnabled(), "Reset is disabled at 100%");
            assertTrue(labelTexts(all).contains(EngineSnifferStatusPanel.ENGINE_CYCLE_LEGEND));
            assertTrue(labelTexts(all).contains(EngineSnifferStatusPanel.TIME_SCALE_LEGEND));

            assertTrue(panel[0].isEmptyStateShownForTests(), "No capture yet");
            assertTrue(labelTexts(all).contains(EngineSnifferPanel.EMPTY_TITLE));
            panel[0].displayChart(CHART);
        });
        // the chart is shown from an invokeLater queued by displayChart
        SwingUtilities.invokeAndWait(() -> {
            assertFalse(panel[0].isEmptyStateShownForTests(), "A capture with edges shows the chart");
            panel[0].getPanel().setSize(1200, 700);
            layOut(panel[0].getPanel());
            List<Component> all = UiUtils.getAllComponents(panel[0].getPanel());
            List<SnifferRowLabel> labels = new java.util.ArrayList<>();
            List<UpDownImage> rows = new java.util.ArrayList<>();
            for (Component c : all) {
                if (c instanceof SnifferRowLabel) {
                    labels.add((SnifferRowLabel) c);
                } else if (c instanceof UpDownImage) {
                    rows.add((UpDownImage) c);
                }
            }
            assertEquals(rows.size(), labels.size(), "One channel label per chart row");
            assertTrue(labels.get(0).getParent().getParent() instanceof JViewport, "The channel column is the row header");
            assertTrue(labels.stream().anyMatch(label -> "1 event".equals(label.getDetailsForTests())),
                "The injector row counts its one pulse (an up and a down edge)");
            assertEquals(labels.get(0).getWidth() + rows.get(0).getWidth(), panel[0].renderChartImage().getWidth(),
                "Save image includes the channel column");

            assertFalse(panel[0].isDarkPlotForTests());
            toggle(all, EngineSnifferPanel.PLOT_DARK).doClick();
            assertTrue(panel[0].isDarkPlotForTests());
            assertEquals(SnifferPalette.of(true).background, rows.get(0).getBackground(), "Rows take the dark plot background");
            assertTrue(config.getBoolProperty(EngineSnifferPanel.DARK_PLOT_KEY, false), "The plot choice is remembered");

            button(all, "clear").doClick();
            assertTrue(panel[0].isEmptyStateShownForTests(), "Clear returns to the waiting message");
            assertTrue(labels.stream().allMatch(label -> label.getDetailsForTests().endsWith("0 events")));
        });
    }

    /** Lays out a tree that is not in a window, where validate() does nothing. */
    private static void layOut(Component c) {
        if (c instanceof Container) {
            ((Container) c).doLayout();
            for (Component child : ((Container) c).getComponents()) {
                layOut(child);
            }
        }
    }

    private static JToggleButton toggle(List<Component> all, String text) {
        return (JToggleButton) all.stream()
            .filter(c -> c instanceof JToggleButton && text.equals(((JToggleButton) c).getText()))
            .findFirst().orElseThrow(() -> new AssertionError("No toggle " + text));
    }

    private static JButton button(List<Component> all, String text) {
        return (JButton) all.stream()
            .filter(c -> c instanceof JButton && text.equalsIgnoreCase(((JButton) c).getText()))
            .findFirst().orElseThrow(() -> new AssertionError("No button " + text));
    }

    private static List<String> labelTexts(List<Component> all) {
        List<String> texts = new java.util.ArrayList<>();
        for (Component c : all) {
            if (c instanceof JLabel) {
                texts.add(((JLabel) c).getText());
            }
        }
        return texts;
    }
}
