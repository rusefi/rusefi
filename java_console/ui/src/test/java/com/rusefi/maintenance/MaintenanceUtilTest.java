package com.rusefi.maintenance;

import com.rusefi.core.io.ConnectedEcuTarget;
import com.rusefi.io.UpdateOperationCallbacks;
import org.junit.jupiter.api.Test;

import static com.rusefi.maintenance.MaintenanceUtil.confirmFirmwareMatchesBoard;
import static com.rusefi.maintenance.MaintenanceUtil.containsPattern;
import static org.junit.jupiter.api.Assertions.assertFalse;
import static org.junit.jupiter.api.Assertions.assertTrue;

/**
 * Tests for the device-detection output matcher and the flash-time brick guard. Windows tooling output
 * separates key/value with ": " while our detection patterns use "=" (e.g. the DFU driver-state probe
 * looks for {@code ConfigManagerErrorCode=0}), so the normalization branch is what makes DFU/PCAN
 * detection work at all. [tag:better_ux_for_flashing]
 * <p>
 * Only the guard's safe (true-returning) branches are covered: the genuine-mismatch branch opens a
 * blocking JOptionPane and needs a confirm seam first (Tier 3 in
 * docs/java-connectivity-ui-unit-testing.md).
 */
public class MaintenanceUtilTest {

    private static ConnectedEcuTarget boardTarget(String liveTarget) {
        ConnectedEcuTarget target = new ConnectedEcuTarget();
        target.setConnectedTargetForUnitTest(liveTarget);
        return target;
    }

    // ---- confirmFirmwareMatchesBoard, safe branches ----

    @Test
    public void matchingFirmwareTargetPassesWithoutConfirmation() {
        assertTrue(confirmFirmwareMatchesBoard(
            "rusefi_release_2026-07-01_uaefi_1234567890_deadbeef_update.srec",
            UpdateOperationCallbacks.DUMMY, boardTarget("uaefi")));
    }

    @Test
    public void firmwareTargetMatchIsCaseInsensitive() {
        assertTrue(confirmFirmwareMatchesBoard(
            "rusefi_release_2026-07-01_uaefi_1234567890_deadbeef.srec",
            UpdateOperationCallbacks.DUMMY, boardTarget("UAEFI")));
    }

    @Test
    public void unparseableFirmwareNameDoesNotFalseAlarm() {
        // a plain dev build ("rusefi.srec") carries no target token — the guard must stay quiet
        assertTrue(confirmFirmwareMatchesBoard("rusefi.srec",
            UpdateOperationCallbacks.DUMMY, boardTarget("uaefi")));
        assertTrue(confirmFirmwareMatchesBoard(null,
            UpdateOperationCallbacks.DUMMY, boardTarget("uaefi")));
    }

    @Test
    public void qcFirmwareIsCompatibleWithAnyBoard() {
        // [tag:QC_firmware] QC images self-identify as "normal" firmware — the _QC_ hack must pass
        assertTrue(confirmFirmwareMatchesBoard(
            "rusefi_release_2026-07-01_uaefi_QC_variant_1234567890_deadbeef.srec",
            UpdateOperationCallbacks.DUMMY, boardTarget("proteus_f4")));
    }

    // ---- containsPattern ----

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
