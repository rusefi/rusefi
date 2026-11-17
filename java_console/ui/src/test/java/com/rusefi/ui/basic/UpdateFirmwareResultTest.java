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
        assertTrue(text.contains("completed"), text);
        assertTrue(text.contains("COM14"), text);
        assertTrue(text.contains("reconnect requested"), text);
        assertTrue(text.contains("check firmware version and settings"), text);
    }

    @Test
    public void settingsWarningIsDistinctFromSuccessAndFailure() {
        assertEquals(new Color(160, 100, 0), UpdateFirmwareResult.WARNING.bannerColor());
        String text = UpdateFirmwareResult.WARNING.bannerText("COM14");
        assertTrue(text.contains("settings warnings remain"), text);
        assertTrue(text.contains("see the report"), text);
        assertTrue(text.contains("COM14"), text);
    }

    @Test
    public void failureIsRedAndExplainsIncompleteUpdate() {
        assertEquals(Color.RED, UpdateFirmwareResult.FAILURE.bannerColor());
        String text = UpdateFirmwareResult.FAILURE.bannerText("COM14");
        assertTrue(text.contains("did not complete all steps"), text);
        assertTrue(text.contains("programming may have succeeded"), text);
        assertTrue(text.contains("COM14"), text);
    }

    @Test
    public void noneIsNeutralConnectedLine() {
        assertEquals(Color.DARK_GRAY, UpdateFirmwareResult.NONE.bannerColor());
        assertEquals("Connected to COM14 — click Connect to open console",
            UpdateFirmwareResult.NONE.bannerText("COM14"));
    }
}
