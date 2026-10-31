package com.rusefi.ui.console;

import org.junit.jupiter.api.Test;

import javax.swing.Icon;

import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertNotNull;
import static org.junit.jupiter.api.Assertions.assertNull;

public class TabbedPanelTest {
    @Test
    public void builtInTabsHaveIcons() {
        String[] tabs = {
            "Gauges", "Messages", "Lua Scripting", "Engine Sniffer", "Tuning",
            "Knock Analyzer", "Pinout", "Manage Tunes", "Device"
        };

        for (String tab : tabs) {
            Icon icon = TabbedPanel.loadTabIcon(tab);
            assertNotNull(icon, tab + " should have an icon");
            assertEquals(18, icon.getIconWidth(), tab + " icon width");
            assertEquals(18, icon.getIconHeight(), tab + " icon height");
        }

        assertNull(TabbedPanel.loadTabIcon("Custom Tab"));
    }
}
