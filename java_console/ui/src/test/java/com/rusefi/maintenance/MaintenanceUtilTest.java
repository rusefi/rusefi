package com.rusefi.maintenance;

import org.junit.jupiter.api.Test;

import static com.rusefi.maintenance.MaintenanceUtil.containsPattern;
import static org.junit.jupiter.api.Assertions.assertFalse;
import static org.junit.jupiter.api.Assertions.assertTrue;

/**
 * Tests for the device-detection output matcher. Windows tooling output separates key/value with
 * ": " while our detection patterns use "=" (e.g. the DFU driver-state probe looks for
 * {@code ConfigManagerErrorCode=0}), so the normalization branch is what makes DFU/PCAN detection
 * work at all. [tag:better_ux_for_flashing]
 */
public class MaintenanceUtilTest {

    @Test
    public void exactSubstringMatches() {
        assertTrue(containsPattern("some ConfigManagerErrorCode=0 tail", "ConfigManagerErrorCode=0"));
    }

    @Test
    public void windowsColonSpaceOutputMatchesEqualsPattern() {
        // Real-world shape of `Get-WmiObject`/wmic output: "Key: Value" lines
        String output = "Caption: STM32 Bootloader\r\nConfigManagerErrorCode: 0\r\n";
        assertTrue(containsPattern(output, "ConfigManagerErrorCode=0"),
            "colon+space separated output must match the equals-style pattern");
    }

    @Test
    public void spacesInPatternAreIgnored() {
        assertTrue(containsPattern("ConfigManagerErrorCode:0", "ConfigManagerErrorCode = 0"));
    }

    @Test
    public void nonMatchingOutputDoesNotMatch() {
        assertFalse(containsPattern("Caption: STM32 Bootloader\r\nConfigManagerErrorCode: 22\r\n",
            "ConfigManagerErrorCode=0"));
        assertFalse(containsPattern("", "ConfigManagerErrorCode=0"));
    }
}
