package com.rusefi.ui.console;

import com.formdev.flatlaf.FlatClientProperties;
import com.rusefi.ui.UIContext;
import org.junit.jupiter.api.Test;

import javax.swing.Icon;
import javax.swing.JTabbedPane;
import javax.swing.SwingUtilities;

import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertNotNull;
import static org.junit.jupiter.api.Assertions.assertNull;

public class TabbedPanelTest {
    @Test
    public void builtInTabsHaveIcons() {
        String[] tabs = {
            "Gauges", "Messages", "Lua Scripting", "Engine Sniffer", "Tuning",
            "Knock Analyzer", "Pinout", "Manage Tunes", "Device", "OBD Codes",
            "AI Troubleshooting", "SLCAN Sniffer"
        };

        for (String tab : tabs) {
            Icon icon = TabbedPanel.loadTabIcon(tab);
            assertNotNull(icon, tab + " should have an icon");
            assertEquals(18, icon.getIconWidth(), tab + " icon width");
            assertEquals(18, icon.getIconHeight(), tab + " icon height");
        }

        assertNull(TabbedPanel.loadTabIcon("Custom Tab"));
    }

    /** Wrapping put the first tabs on a second row and stretched that row's tabs; keep one row. */
    @Test
    public void tabsStayOnOneRowWithAMoreTabsButton() throws Exception {
        SwingUtilities.invokeAndWait(() -> {
            JTabbedPane pane = new TabbedPanel(new UIContext()).tabbedPane;
            assertEquals(JTabbedPane.SCROLL_TAB_LAYOUT, pane.getTabLayoutPolicy());
            assertEquals(FlatClientProperties.TABBED_PANE_POLICY_AS_NEEDED,
                pane.getClientProperty(FlatClientProperties.TABBED_PANE_TABS_POPUP_POLICY));
        });
    }
}
