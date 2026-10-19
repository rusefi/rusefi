package com.rusefi.ui.console;

import org.junit.jupiter.api.Test;

import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertNull;

public class TitleBarPanelTest {
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
