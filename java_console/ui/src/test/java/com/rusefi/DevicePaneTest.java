package com.rusefi;

import org.junit.jupiter.api.Test;

import static org.junit.jupiter.api.Assertions.assertFalse;
import static org.junit.jupiter.api.Assertions.assertTrue;

/**
 * Tests for the DevicePane presentation decisions around bootloader states: which session states count
 * as "board sitting in a bootloader", what guidance the status line shows, and which tabs stay usable
 * without a live ECU. [tag:better_ux_for_flashing]
 */
public class DevicePaneTest {

    @Test
    public void onlyDfuAndOpenBltCountAsBootloaderStates() {
        assertTrue(DevicePane.isBootloaderState(SessionState.DEVICE_IN_DFU));
        assertTrue(DevicePane.isBootloaderState(SessionState.DEVICE_IN_BLT));

        assertFalse(DevicePane.isBootloaderState(SessionState.DISCONNECTED));
        assertFalse(DevicePane.isBootloaderState(SessionState.CONNECTING));
        assertFalse(DevicePane.isBootloaderState(SessionState.CONNECTED));
        assertFalse(DevicePane.isBootloaderState(SessionState.FLASHING));
    }

    @Test
    public void dfuGuidanceMentionsDfuAndReflectsPlatformSupport() {
        String guidance = DevicePane.bootloaderGuidance(SessionState.DEVICE_IN_DFU);
        assertTrue(guidance.contains("DFU"), guidance);
        if (FileLog.isWindows()) {
            // DFU flashing works here — send the user to the update button
            assertTrue(guidance.contains("Update Firmware"), guidance);
        } else {
            // DFU flashing is Windows-only (STM32_Programmer_CLI.exe) — say so instead of a dead end
            assertTrue(guidance.contains("Windows"), guidance);
        }
    }

    @Test
    public void openBltGuidanceSendsUserToUpdateButton() {
        String guidance = DevicePane.bootloaderGuidance(SessionState.DEVICE_IN_BLT);
        assertTrue(guidance.contains("OpenBLT"), guidance);
        assertTrue(guidance.contains("Update Firmware"), guidance);
    }

    @Test
    public void bootloaderAllowsOfflineTabsButFlashingBlocksTuning() {
        assertTrue(DevicePane.isOfflineCapableTab("Device"));
        assertTrue(DevicePane.isOfflineCapableTab("Tuning"));
        assertTrue(DevicePane.isOfflineCapableTab("Pinout"));

        assertFalse(DevicePane.isOfflineCapableTab("Gauges"));
        assertFalse(DevicePane.isOfflineCapableTab("Messages"));
        assertFalse(DevicePane.isOfflineCapableTab(null));

        assertTrue(DevicePane.isTabEnabled("Tuning", SessionState.DEVICE_IN_BLT));
        assertTrue(DevicePane.isTabEnabled("Tuning", SessionState.DEVICE_IN_DFU));
        assertFalse(DevicePane.isTabEnabled("Tuning", SessionState.FLASHING));
        assertTrue(DevicePane.isTabEnabled("Device", SessionState.FLASHING));
        assertTrue(DevicePane.isTabEnabled("Pinout", SessionState.FLASHING));
        assertFalse(DevicePane.isTabEnabled("Messages", SessionState.FLASHING));
    }
}
