package com.rusefi.maintenance;

import com.devexperts.logging.Logging;
import com.rusefi.FileLog;
import com.rusefi.autoupdate.Autoupdate;
import com.rusefi.core.FindFileHelper;
import com.rusefi.core.io.BoardCompatibility;
import com.rusefi.core.io.BundleUtil;
import com.rusefi.core.io.ConnectedEcuTarget;
import com.rusefi.io.UpdateOperationCallbacks;

import javax.swing.*;
import java.io.File;

import static com.devexperts.logging.Logging.getLogging;

public class MaintenanceUtil {
    private static final Logging log = getLogging(MaintenanceUtil.class);

    private static final String WMIC_PCAN_QUERY_COMMAND = "powershell -NoProfile -Command \"Get-CimInstance Win32_PnPEntity -Filter \\\"Caption like '%PCAN-USB%'\\\" | Select-Object Caption, ConfigManagerErrorCode | Format-List\"";

    static boolean detectDevice(UpdateOperationCallbacks callbacks, String queryCommand, String pattern) throws ErrorExecutingCommand {
        if (!FileLog.isWindows()) {
            return false;
        }
        long now = System.currentTimeMillis();
        StringBuffer output = new StringBuffer();
        StringBuffer error = new StringBuffer();
        ExecHelper.executeCommand(queryCommand, callbacks, output, error, null);
        callbacks.logLine(output.toString());
        callbacks.logLine(error.toString());
        long cost = System.currentTimeMillis() - now;
        String duration = "detectDevice lookup cost " + cost + "ms; ";
        String nicerOutput = output.length() == 0 ? "(empty)" : output.toString();
        log.info(duration + queryCommand + " says " + nicerOutput);
        return containsPattern(output.toString(), pattern);
    }

    static boolean containsPattern(String output, String pattern) {
        if (output.contains(pattern)) {
            return true;
        }

        String normalizedOutput = output.replace(":", "=").replace(" ", "");
        String normalizedPattern = pattern.replace(" ", "");
        return normalizedOutput.contains(normalizedPattern);
    }

    public static boolean detectPcan(UpdateOperationCallbacks wnd) {
        try {
            return detectDevice(wnd, WMIC_PCAN_QUERY_COMMAND, "PCAN");
        } catch (ErrorExecutingCommand e) {
            log.error("detectPcan error: " + e, e);
            return false;
        }
    }

    public static long getBinaryModificationTimestamp() {
        String fileName = FindFileHelper.isObfuscated() ? FindFileHelper.findSrecFile() : FindFileHelper.findFirmwareFile();
        return new File(fileName).lastModified();
    }

    /**
     * A board already sitting in a bootloader (OpenBLT or DFU) has no live signature, so — unlike the
     * reboot path in {@link com.rusefi.io.BootloaderHelper} — nothing has fetched the right firmware yet.
     * On a universal bundle the flashers would otherwise program whatever firmware is on disk (the bundle
     * default), not the board that was just connected. Resolve the target from the persisted
     * last-connected board (it survives the disconnect) and download it first; fail closed rather than
     * flash the wrong firmware onto a different board. [tag:better_ux_for_flashing] / #9714
     *
     * @return true if it is safe to proceed with flashing
     */
    public static boolean ensureFirmwareForConnectedTarget(final UpdateOperationCallbacks callbacks,
                                                           final ConnectedEcuTarget connectedEcuTarget) {
        final String bundleTarget = BundleUtil.getBundleTarget();
        final String ecuTarget = connectedEcuTarget.effectiveTarget();
        // A board in a bootloader cannot identify itself. If there is no live signature this session the
        // target is only a guess (persisted last board, or the bundle default) — confirm before flashing
        // so we never program a possibly-swapped board silently. Independent of foreign-vs-matching: even
        // flashing the bundle's own firmware onto an unverified board deserves a check. Live-verified
        // flashes skip this. [tag:better_ux_for_flashing]
        if (!connectedEcuTarget.isLiveTargetKnown() && !confirmUnverifiedTarget(ecuTarget)) {
            callbacks.logLine("Firmware update cancelled — unverified board target \"" + ecuTarget + "\".");
            return false;
        }

        final boolean foreignBoard = bundleTarget != null && ecuTarget != null
            && !bundleTarget.equalsIgnoreCase(ecuTarget);
        if (!foreignBoard) {
            // Single-board bundle, or the target matches the bundle: the on-disk firmware is already right.
            return true;
        }
        if (!BoardCompatibility.matchesCompatibility(ecuTarget)) {
            callbacks.logLine(String.format(
                "Refusing to flash: \"%s\" is not compatible with this bundle (\"%s\").", ecuTarget, bundleTarget));
            return false;
        }
        callbacks.logLine("[universal_bundle]: downloading firmware for \"" + ecuTarget + "\"...");
        if (!Autoupdate.ensureFirmwareForTarget(ecuTarget, callbacks::updateProgress, callbacks::logLine)) {
            callbacks.logLine(String.format(
                "Universal bundle could not download firmware for \"%s\" — aborting to avoid flashing the wrong firmware.",
                ecuTarget));
            return false;
        }
        return true;
    }

    /**
     * Ask the user to confirm flashing an unverified (persisted-guess) board target. Runs on the EDT and
     * blocks the calling job thread for the answer; fails closed (no flash) if interrupted. [tag:better_ux_for_flashing]
     */
    private static boolean confirmUnverifiedTarget(final String ecuTarget) {
        return confirmOnEdt(String.format(
            "No live ECU is connected to verify the board.\n\n" +
            "About to flash firmware for: \"%s\".\n" +
            "If the plugged-in board is not \"%s\", flashing the wrong firmware can brick it.\n\n" +
            "Flash \"%s\" firmware?", ecuTarget, ecuTarget, ecuTarget),
            "Confirm firmware target", JOptionPane.WARNING_MESSAGE);
    }

    /**
     * Fail-safe guard against the #1 way to brick an ECU: flashing firmware built for a different,
     * incompatible board. The firmware artifact name encodes its target (e.g. "uaefi_pro"); if that does
     * not match — and is not compatible with — the connected board's target, make the user explicitly
     * confirm. This catches the case where the bundle dir still holds another board's downloaded firmware
     * and {@link FindFileHelper#findSrecFile()} blindly picks it. [tag:better_ux_for_flashing]
     *
     * @return true if it is safe to proceed with flashing {@code firmwareFile}
     */
    public static boolean confirmFirmwareMatchesBoard(final String firmwareFile, final UpdateOperationCallbacks callbacks,
                                                      final ConnectedEcuTarget connectedEcuTarget) {
        final String fileTarget = FindFileHelper.extractTargetFromFirmwareName(firmwareFile);
        final String boardTarget = connectedEcuTarget.effectiveTarget();
        if (fileTarget == null || boardTarget == null || fileTarget.equalsIgnoreCase(boardTarget)) {
            return true;
        }
        // Boards that can share firmware (compatibility list / _QC_ hack) are fine.
        if (BoardCompatibility.isEcuCompatible(fileTarget, boardTarget)) {
            return true;
        }
        callbacks.logLine(String.format(
            "*** FIRMWARE/BOARD MISMATCH *** file is built for \"%s\" but the connected board is \"%s\".",
            fileTarget, boardTarget));
        return confirmOnEdt(String.format(
            "DANGER: about to flash \"%s\" firmware onto a \"%s\" board.\n\n" +
            "This is almost certainly the WRONG firmware and will likely BRICK the board.\n\n" +
            "Flash anyway?", fileTarget, boardTarget),
            "Firmware / board mismatch", JOptionPane.ERROR_MESSAGE);
    }

    private static boolean confirmOnEdt(final String message, final String title, final int messageType) {
        final boolean[] confirmed = {false};
        final Runnable ask = () -> confirmed[0] = JOptionPane.showConfirmDialog(
            null, message, title, JOptionPane.OK_CANCEL_OPTION, messageType) == JOptionPane.OK_OPTION;
        try {
            if (SwingUtilities.isEventDispatchThread()) {
                ask.run();
            } else {
                SwingUtilities.invokeAndWait(ask);
            }
        } catch (Exception e) {
            log.warn("confirmOnEdt interrupted, treating as cancel: " + e);
            return false;
        }
        return confirmed[0];
    }
}
