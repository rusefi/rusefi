package com.rusefi.ui.console;

import org.junit.jupiter.api.Test;

import javax.swing.*;

import static org.junit.jupiter.api.Assertions.*;

public class TabTitleTest {
    @Test
    public void selectedTabIsBoldWithoutChangingWidth() throws Exception {
        SwingUtilities.invokeAndWait(() -> {
            JTabbedPane pane = new JTabbedPane();
            JPanel gauges = new JPanel();
            JPanel tuning = new JPanel();
            pane.addTab("Gauges", gauges);
            pane.addTab("Tuning", tuning);
            TabbedPanel.TabTitle gaugesTitle = new TabbedPanel.TabTitle(pane, gauges, "Gauges", null);
            TabbedPanel.TabTitle tuningTitle = new TabbedPanel.TabTitle(pane, tuning, "Tuning", null);
            pane.setTabComponentAt(0, gaugesTitle);
            pane.setTabComponentAt(1, tuningTitle);

            assertTrue(gaugesTitle.getFont().isBold(), "the selected tab is bold");
            assertFalse(tuningTitle.getFont().isBold());
            int tuningPlainWidth = tuningTitle.getPreferredSize().width;

            pane.setSelectedIndex(1);
            assertFalse(gaugesTitle.getFont().isBold());
            assertTrue(tuningTitle.getFont().isBold());
            assertEquals(tuningPlainWidth, tuningTitle.getPreferredSize().width,
                "a tab reserves its bold width, so selecting it does not move the other tabs");
            assertEquals(1, pane.indexOfTab("Tuning"), "the tab keeps its title for lookups");
        });
    }
}
