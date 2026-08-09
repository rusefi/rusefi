package com.rusefi.maintenance;

import com.rusefi.core.io.ConnectedEcuTarget;
import com.rusefi.io.UpdateOperationCallbacks;
import org.junit.jupiter.api.Test;

import java.nio.charset.StandardCharsets;
import java.util.Base64;
import java.util.List;

import static com.rusefi.maintenance.MaintenanceUtil.confirmFirmwareMatchesBoard;
import static com.rusefi.maintenance.MaintenanceUtil.containsPattern;
import static org.junit.jupiter.api.Assertions.assertEquals;
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

    // ---- confirmFirmwareMatchesBoard / ensureFirmwareForConnectedTarget, user-decision branches ----
    // Driven through the injected UserConfirm seam — no JOptionPane. Note the bundled
    // board_compatibility allowlist (shared_io.properties) is on the test classpath, so mismatch tests
    // use a board target outside that list to deterministically reach the confirmation.

    private static class RecordingConfirm implements MaintenanceUtil.UserConfirm {
        final boolean answer;
        int calls;
        String lastMessage;

        RecordingConfirm(boolean answer) {
            this.answer = answer;
        }

        @Override
        public boolean confirm(String message, String title, int messageType) {
            calls++;
            lastMessage = message;
            return answer;
        }
    }

    @Test
    public void mismatchedFirmwareDeclinedByUserBlocksTheFlash() {
        RecordingConfirm confirm = new RecordingConfirm(false);
        assertFalse(confirmFirmwareMatchesBoard(
            "rusefi_release_2026-07-01_uaefi_pro_1234567890_deadbeef.srec",
            UpdateOperationCallbacks.DUMMY, boardTarget("proteus_h7"), confirm));
        assertEquals(1, confirm.calls);
        assertTrue(confirm.lastMessage.contains("uaefi_pro"), confirm.lastMessage);
        assertTrue(confirm.lastMessage.contains("proteus_h7"), confirm.lastMessage);
    }

    @Test
    public void mismatchedFirmwareExplicitlyAcceptedProceeds() {
        RecordingConfirm confirm = new RecordingConfirm(true);
        assertTrue(confirmFirmwareMatchesBoard(
            "rusefi_release_2026-07-01_uaefi_pro_1234567890_deadbeef.srec",
            UpdateOperationCallbacks.DUMMY, boardTarget("proteus_h7"), confirm));
        assertEquals(1, confirm.calls);
    }

    @Test
    public void matchingFirmwareNeverAsksTheUser() {
        RecordingConfirm confirm = new RecordingConfirm(false);
        assertTrue(confirmFirmwareMatchesBoard(
            "rusefi_release_2026-07-01_uaefi_1234567890_deadbeef.srec",
            UpdateOperationCallbacks.DUMMY, boardTarget("uaefi"), confirm));
        assertEquals(0, confirm.calls, "a verified match must not nag the user");
    }

    @Test
    public void unverifiedTargetDeclinedCancelsFirmwareUpdate() {
        // fresh instance = no live connection this session (board sitting in a bootloader):
        // the target is only a guess, so a declined confirmation must fail closed
        RecordingConfirm confirm = new RecordingConfirm(false);
        assertFalse(MaintenanceUtil.ensureFirmwareForConnectedTarget(
            UpdateOperationCallbacks.DUMMY, new ConnectedEcuTarget(), confirm));
        assertEquals(1, confirm.calls);
        assertTrue(confirm.lastMessage.contains("No live ECU"), confirm.lastMessage);
    }

    @Test
    public void liveVerifiedTargetSkipsUnverifiedConfirmation() {
        // a live-verified board matching the bundle needs no confirmation and no download
        String bundleTarget = com.rusefi.core.io.BundleUtil.getBundleTarget();
        RecordingConfirm confirm = new RecordingConfirm(false);
        assertTrue(MaintenanceUtil.ensureFirmwareForConnectedTarget(UpdateOperationCallbacks.DUMMY,
            boardTarget(bundleTarget != null ? bundleTarget : "uaefi"), confirm));
        assertEquals(0, confirm.calls);
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

    // ---- PCAN detection command shape ----
    // Regression guard for the "console cannot find PCAN" bug. The query used to be a single string run
    // through Runtime.exec(String), whose tokenizer splits on whitespace and keeps quotes as literal
    // characters — so the PowerShell script arrived shredded into broken argv entries and the WMI query
    // silently matched nothing. The script is now transported as Base64 UTF-16LE via -EncodedCommand
    // (see MaintenanceUtil.powershellEncodedCommand): a pure-ASCII, quote-free payload that survives
    // Windows command-line quoting verbatim and decodes back to the intact script byte-exact.

    @Test
    public void pcanQueryIsEncodedCommandSurvivingQuoting() {
        List<String> command = MaintenanceUtil.PCAN_QUERY_COMMAND;
        assertEquals(4, command.size());
        assertEquals("powershell", command.get(0));
        assertEquals("-NoProfile", command.get(1));
        assertEquals("-EncodedCommand", command.get(2));
        // payload has no spaces and no quotes — immune to command-line quoting on any platform
        assertTrue(command.get(3).matches("[A-Za-z0-9+/=]+"));
        // and it decodes back to the whole script, including the quotes around the WQL filter
        String decoded = new String(Base64.getDecoder().decode(command.get(3)), StandardCharsets.UTF_16LE);
        assertEquals(
            "Get-CimInstance Win32_PnPEntity -Filter \"Caption like '%PCAN-USB%'\" | Select-Object Caption, ConfigManagerErrorCode | Format-List",
            decoded);
    }

    @Test
    public void describeQueryCommandShowsDecodedScript() {
        String described = MaintenanceUtil.describeQueryCommand(MaintenanceUtil.PCAN_QUERY_COMMAND);
        assertTrue(described.contains("Caption like '%PCAN-USB%'"), described);
        assertFalse(described.contains("="), "the base64 payload must not leak into the log line: " + described);
    }
}
