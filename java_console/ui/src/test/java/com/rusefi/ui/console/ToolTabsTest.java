package com.rusefi.ui.console;

import com.rusefi.ui.UIContext;
import org.junit.jupiter.api.Test;

import javax.swing.*;
import java.awt.*;
import java.util.Arrays;

import static org.junit.jupiter.api.Assertions.*;
import static org.mockito.Mockito.mock;

class ToolTabsTest {
    @Test
    void toolTabsOpenAtTheEndAndClose() throws Exception {
        SwingUtilities.invokeAndWait(() -> {
            TabbedPanel panel = new TabbedPanel(mock(UIContext.class));
            JTabbedPane pane = panel.tabbedPane;
            panel.addTab("Gauges", new JPanel());
            panel.addTab("Device", new JPanel());
            panel.insertTab("Tuning", new JPanel(), 1);
            assertEquals("Tuning", pane.getTitleAt(1), "Tuning is inserted right after Gauges");

            ToolTabs tools = new ToolTabs(panel);
            JPanel messages = new JPanel();
            tools.register("Messages", messages);
            tools.register("Engine Sniffer", new JPanel());
            assertEquals(Arrays.asList("Messages", "Engine Sniffer"), tools.getTitles());
            assertEquals(3, pane.getTabCount(), "registering does not add a tab");
            assertFalse(tools.isOpen("Messages"));

            tools.open("Messages");
            assertEquals(4, pane.getTabCount());
            assertEquals("Messages", pane.getTitleAt(3));
            assertSame(messages, pane.getSelectedComponent());
            Component title = pane.getTabComponentAt(3);

            pane.setSelectedIndex(0);
            tools.open("Messages");
            assertEquals(4, pane.getTabCount(), "opening an open tab only selects it");
            assertSame(messages, pane.getSelectedComponent());

            JButton close = findButton((Container) title);
            assertNotNull(close, "a tool tab has a close button");
            close.doClick();
            assertFalse(tools.isOpen("Messages"));
            assertEquals(3, pane.getTabCount());

            tools.open("Messages");
            assertSame(title, pane.getTabComponentAt(pane.indexOfComponent(messages)),
                "reopening reuses the tab title instead of building another");

            tools.open("Unknown");
            assertEquals(4, pane.getTabCount());
        });
    }

    private static JButton findButton(Container container) {
        for (Component c : container.getComponents()) {
            if (c instanceof JButton) {
                return (JButton) c;
            }
        }
        return null;
    }
}
