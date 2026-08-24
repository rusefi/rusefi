package com.rusefi.ui.basic;

import org.junit.jupiter.api.Test;

import java.awt.Color;

import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertTrue;

/**
 * The persistent post-reconnect banner mapping (#9832): a successful update must leave a green
 * "complete" line, a failure a red one, and a plain reconnect (no update) the neutral line.
 */
public class UpdateFirmwareResultTest {
    @Test
    public void successIsGreenAndSaysComplete() {
        assertEquals(new Color(0, 128, 0), UpdateFirmwareResult.SUCCESS.bannerColor());
        String text = UpdateFirmwareResult.SUCCESS.bannerText("COM14");
        assertTrue(text.contains("complete"), text);
        assertTrue(text.contains("COM14"), text);
    }

    @Test
    public void failureIsRedAndSaysFailed() {
        assertEquals(Color.RED, UpdateFirmwareResult.FAILURE.bannerColor());
        String text = UpdateFirmwareResult.FAILURE.bannerText("COM14");
        assertTrue(text.contains("failed"), text);
        assertTrue(text.contains("COM14"), text);
    }

    @Test
    public void noneIsNeutralConnectedLine() {
        assertEquals(Color.DARK_GRAY, UpdateFirmwareResult.NONE.bannerColor());
        assertEquals("Connected to COM14 — click Connect to open console",
            UpdateFirmwareResult.NONE.bannerText("COM14"));
    }
}
