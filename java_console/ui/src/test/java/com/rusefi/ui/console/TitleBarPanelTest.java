package com.rusefi.ui.console;

import com.rusefi.UiProperties;
import org.junit.jupiter.api.Test;

import javax.swing.JLabel;
import javax.swing.SwingUtilities;
import java.awt.Component;
import java.awt.Dimension;
import java.util.Arrays;

import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertNull;
import static org.junit.jupiter.api.Assertions.assertTrue;

public class TitleBarPanelTest {
    @Test
    public void titleBarDisplaysTheConfiguredProductName() throws Exception {
        SwingUtilities.invokeAndWait(() -> {
            TitleBarPanel panel = new TitleBarPanel();
            assertEquals(UiProperties.getDisplayName(), ((JLabel) panel.getComponent(0)).getText());
        });
    }

    /**
     * A narrow window used to wrap the chips onto a second line, which the title bar cut off.
     * The row now stays on one line and hides the trailing chips that do not fit.
     */
    @Test
    public void narrowTitleBarHidesTrailingChipsInsteadOfWrapping() throws Exception {
        SwingUtilities.invokeAndWait(() -> {
            TitleBarPanel panel = new TitleBarPanel();
            panel.update("20261026", Arrays.asList("uaefi_pro · master.2026.10.23", "uaefi COM4"), "full title");
            Dimension preferred = panel.getPreferredSize();

            panel.setSize(preferred);
            panel.doLayout();
            for (Component c : panel.getComponents()) {
                assertTrue(c.getWidth() > 0, "Everything fits at the preferred width");
                assertEquals(0, c.getY() + c.getHeight() / 2 - preferred.height / 2, 1, "One centred row");
            }

            Component connection = panel.getComponent(3);
            panel.setSize(preferred.width - connection.getPreferredSize().width / 2, preferred.height);
            panel.doLayout();
            assertEquals(0, connection.getWidth(), "The connection chip is dropped first");
            assertTrue(panel.getComponent(2).getWidth() > 0, "The firmware chip still fits");

            panel.setSize(panel.getComponent(0).getPreferredSize().width, preferred.height);
            panel.doLayout();
            assertTrue(panel.getComponent(0).getWidth() > 0, "The product name always stays");
            for (int i = 1; i < panel.getComponentCount(); i++) {
                assertEquals(0, panel.getComponent(i).getWidth());
            }
            assertEquals(panel.getComponent(0).getPreferredSize().width, panel.getMinimumSize().width,
                "The menu bar can shrink the panel down to the product name");
        });
    }

    @Test
    public void firmwareLabelShowsBoardBranchAndDate() {
        assertEquals("uaefi_pro · master.2026.08.13",
            TitleBarPanel.firmwareLabel("rusEFI master.2026.08.13.uaefi_pro.3151813420"));
    }

    @Test
    public void firmwareLabelKeepsUnparsableSignatureWithoutPrefix() {
        assertEquals("custom build", TitleBarPanel.firmwareLabel("rusEFI custom build"));
        assertEquals("no INI", TitleBarPanel.firmwareLabel("no INI"));
        assertNull(TitleBarPanel.firmwareLabel(null));
    }

    @Test
    public void connectionLabelShowsBuildIdAndPort() {
        // format from printVersionForConsole: <version>@<hash> <buildId> <engineType> <uptime>
        assertEquals("simulator :29001",
            TitleBarPanel.connectionLabel("20260813@3151813420 simulator MINIMAL_PINS 18556", "29001"));
        assertEquals("uaefi121 COM7",
            TitleBarPanel.connectionLabel("20260813@3151813420 uaefi121 MINIMAL_PINS 5", "COM7"));
    }

    @Test
    public void connectionLabelBeforeVersionReportShowsPortOnly() {
        assertEquals("COM7", TitleBarPanel.connectionLabel("N/A", "COM7"));
        assertEquals(":29001", TitleBarPanel.connectionLabel(null, "29001"));
        assertNull(TitleBarPanel.connectionLabel("N/A", null));
    }
}
