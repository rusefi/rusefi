package com.rusefi.maintenance;

import com.devexperts.logging.Logging;
import com.fazecast.jSerialComm.SerialPort;
import com.rusefi.*;
import com.rusefi.autodetect.PortDetector;
import com.rusefi.binaryprotocol.BinaryProtocol;
import com.rusefi.config.generated.Integration;
import com.rusefi.core.FindFileHelper;
import com.rusefi.io.IoStream;
import com.rusefi.io.LinkManager;
import com.rusefi.io.UpdateOperationCallbacks;
import com.rusefi.io.can.PCanRawPort;
import com.rusefi.io.can.SocketCanRawPort;
import com.rusefi.io.serial.BufferedSerialIoStream;
import com.rusefi.maintenance.jobs.OpenbltRebooter;
import com.rusefi.updater.OpenbltDetectorStrategy;
import org.jetbrains.annotations.Nullable;

import javax.swing.*;
import java.io.EOFException;
import java.io.IOException;
import java.util.ArrayList;
import java.util.List;
import java.util.concurrent.ExecutionException;
import java.util.concurrent.TimeUnit;
import java.util.function.BooleanSupplier;

import static com.devexperts.logging.Logging.getLogging;
import static com.rusefi.SerialPortType.OpenBlt;
import static com.rusefi.maintenance.CalibrationsHelper.*;
import static com.rusefi.maintenance.CallbacksWaitingUtil.TOTAL_WAIT_SECONDS;
import static com.rusefi.maintenance.CallbacksWaitingUtil.waitForPredicate;

/** Firmware operations shared by the console and MCP, independent of the firmware selector widget. */
public final class FirmwareOperations {
    private static final Logging log = getLogging(FirmwareOperations.class);

    private FirmwareOperations() {
    }

    interface CanFlashAction {
        void flash(String fileName, OpenbltJni.OpenbltCallbacks callbacks) throws IOException;
    }

    public static void rebootToDfu(JComponent parent, String selectedPort, UpdateOperationCallbacks callbacks) {
        String port = selectedPort == null ? PortDetector.AUTO : selectedPort;
        DfuFlasher.rebootToDfu(parent, port, callbacks, Integration.CMD_REBOOT_DFU);
    }

    public static void rebootToOpenblt(JComponent parent, String selectedPort, UpdateOperationCallbacks callbacks) {
        String port = selectedPort == null ? PortDetector.AUTO : selectedPort;
        DfuFlasher.rebootToDfu(parent, port, callbacks, Integration.CMD_REBOOT_OPENBLT);
    }

    public static void flashOpenBltCan(
        JComponent parent,
        UpdateOperationCallbacks callbacks,
        PortScanner scanner
    ) {
        if (FileLog.is32bitJava()) {
            showError32bitJava(parent);
            return;
        }

        if (flashCanWithSuspendedScanner(
            LinkManager.PCAN,
            FindFileHelper.findSrecFile(),
            callbacks,
            scanner,
            (firmware, openbltCallbacks) -> {
                try {
                    OpenbltJni.flashCan(firmware, openbltCallbacks);
                } finally {
                    OpenbltJni.stop(openbltCallbacks);
                }
            })) {
            callbacks.done();
        } else {
            callbacks.error();
        }
    }

    private static boolean waitForEcuPortDisappeared(
        final PortResult ecuPort,
        final UpdateOperationCallbacks callbacks
    ) {
        // Scanner is already suspended by the caller (bltUpdateFirmware).
        return waitForPredicate(
            String.format("Waiting for ECU on port %s to reboot to OpenBlt for up to " + TOTAL_WAIT_SECONDS + " seconds...", ecuPort),
            () -> {
                // Directly probe the port rather than relying on the scanner snapshot.
                // The scanner cache would keep reporting EcuWithOpenblt even after the
                // ECU has already entered OpenBLT mode (the OS port does not disappear).
                try (IoStream stream = BufferedSerialIoStream.openPort(ecuPort.port)) {
                    if (stream == null) {
                        log.info("Port " + ecuPort.port + " is unavailable — ECU is rebooting");
                        return true;
                    }
                    if (OpenbltDetectorStrategy.isPortOpenblt(stream)) {
                        log.info("Port " + ecuPort.port + " is now in OpenBLT mode");
                        return true;
                    }
                } catch (EOFException e) {
                    // readByte timed out — keep waiting until a write error or
                    // null stream indicates the ECU has actually reset.
                    log.info("Port " + ecuPort.port + " still responding as ECU firmware (XCP ignored)");
                } catch (Exception e) {
                    log.info("Port " + ecuPort.port + " probe error (ECU transitioning): " + e.getMessage());
                    return true;
                }
                log.info("Port " + ecuPort.port + " still responding as ECU firmware");
                return false;
            },
            callbacks
        );
    }

    private static List<PortResult> waitForNewOpenBltPortAppeared(
        final List<PortResult> openBltPortsBefore,
        final UpdateOperationCallbacks callbacks
    ) {
        // Scanner is suspended by the caller (bltUpdateFirmware). Scan all system
        // COM ports ourselves so we can find OpenBLT even when it enumerates on a
        // different port number than the original ECU port (common with USB-CDC where
        // the bootloader has a different USB PID and gets a new COM assignment).
        final List<PortResult> newPorts = new ArrayList<>();
        waitForPredicate(
            "Waiting for new OpenBlt port to appear...",
            () -> {
                for (SerialPort sp : SerialPort.getCommPorts()) {
                    final String portName = sp.getSystemPortName();
                    if (openBltPortsBefore.stream().anyMatch(p -> p.port.equals(portName))) {
                        continue; // skip ports that were already in OpenBLT mode before
                    }
                    try (IoStream stream = BufferedSerialIoStream.openPort(portName)) {
                        if (OpenbltDetectorStrategy.isPortOpenblt(stream)) {
                            log.info("Direct probe: port " + portName + " is in OpenBLT mode");
                            newPorts.add(new PortResult(portName, OpenBlt));
                            return true;
                        }
                    } catch (Exception e) {
                        log.info("Probe of " + portName + " error: " + e.getMessage());
                    }
                }
                return false;
            },
            callbacks
        );
        return newPorts;
    }

    public static boolean flashOpenbltSerialAutomatic(
        JComponent parent,
        PortResult ecuPort,
        BinaryProtocol bp,
        LinkManager lm,
        UpdateOperationCallbacks callbacks,
        ConnectivityContext connectivityContext
    ) {
        return flashOpenbltSerialAutomatic(parent, ecuPort, bp, lm, callbacks, connectivityContext, null);
    }

    public static boolean flashOpenbltSerialAutomatic(
        JComponent parent,
        PortResult ecuPort,
        BinaryProtocol bp,
        LinkManager lm,
        UpdateOperationCallbacks callbacks,
        ConnectivityContext connectivityContext,
        @Nullable String firmwareSrecFile
    ) {
        return flashOpenbltSerialAutomatic(parent, ecuPort, bp, lm, callbacks, connectivityContext,
            firmwareSrecFile, CalibrationsHelper.FirmwareUpdatePolicy.FORWARD_MIGRATION);
    }

    public static boolean flashOpenbltSerialAutomatic(
        JComponent parent,
        PortResult ecuPort,
        BinaryProtocol bp,
        LinkManager lm,
        UpdateOperationCallbacks callbacks,
        ConnectivityContext connectivityContext,
        @Nullable String firmwareSrecFile,
        CalibrationsHelper.FirmwareUpdatePolicy policy
    ) {
        // Also protect callers that invoke this API without an OpenBltAutoJob.
        if (!FirmwareFlashEligibility.isAllowed(bp == null ? null : bp.signature, firmwareSrecFile, callbacks)) {
            return false;
        }
        return updateFirmwareAndRestorePreviousCalibrations(
            parent, ecuPort, bp, lm, callbacks,
            () -> bltUpdateFirmware(parent, ecuPort, callbacks, connectivityContext, firmwareSrecFile),
            connectivityContext, policy);
    }

    public static boolean flashOpenbltCanAutomatic(
        JComponent parent,
        PortResult ecuPort,
        BinaryProtocol bp,
        LinkManager lm,
        UpdateOperationCallbacks callbacks,
        ConnectivityContext connectivityContext,
        @Nullable String firmwareSrecFile,
        CalibrationsHelper.FirmwareUpdatePolicy policy
    ) {
        if (!isAutomaticCanPort(ecuPort.port)
            || lm == null
            || bp == null
            || !ecuPort.port.equals(lm.getLastTriedPort())) {
            callbacks.logLine("CAN firmware update requires a matching live CAN ECU connection.");
            return false;
        }

        if (!FirmwareFlashEligibility.isAllowed(bp.signature, firmwareSrecFile, callbacks)) {
            return false;
        }

        if (!MaintenanceUtil.ensureFirmwareForConnectedTarget(
            callbacks, connectivityContext.getConnectedEcuTarget())) {
            return false;
        }

        final String fileName = firmwareSrecFile != null
            ? firmwareSrecFile
            : FindFileHelper.findSrecFileForConnectedBoard(connectivityContext.getConnectedEcuTarget());
        if (fileName == null) {
            callbacks.logLine(".srec image file not found");
            return false;
        }
        if (!MaintenanceUtil.confirmFirmwareMatchesBoard(
            fileName, callbacks, connectivityContext.getConnectedEcuTarget())) {
            callbacks.logLine("Firmware update aborted - firmware/board mismatch.");
            return false;
        }

        final OpenBltFlasher.PreparedFirmware preparedFirmware;
        try {
            preparedFirmware = OpenBltFlasher.prepareFirmware(fileName, makeOpenbltCallbacks(callbacks));
        } catch (IOException e) {
            callbacks.logLine("Unable to load firmware file: " + e.getMessage());
            return false;
        }

        return updateFirmwareAndRestorePreviousCalibrations(
            parent, ecuPort, bp, lm, callbacks,
            () -> prepareCanHandoff(ecuPort.port, lm, callbacks,
                () -> OpenbltRebooter.rebootToOpenblt(parent, bp, callbacks)),
            () -> canUpdateFirmware(
                callbacks, connectivityContext, ecuPort.port, fileName, preparedFirmware),
            connectivityContext, policy);
    }

    static boolean prepareCanHandoff(
        String canPort,
        LinkManager lm,
        UpdateOperationCallbacks callbacks,
        BooleanSupplier rebootToOpenBlt
    ) {
        final boolean[] commandSent = {false};
        try {
            lm.submit(() -> commandSent[0] = rebootToOpenBlt.getAsBoolean()).get();
            if (!commandSent[0]) {
                callbacks.logLine(canPort + " OpenBLT reboot command was not sent.");
            }
            return commandSent[0];
        } catch (InterruptedException e) {
            Thread.currentThread().interrupt();
            callbacks.logLine("Interrupted while sending the " + canPort + " OpenBLT reboot command.");
        } catch (ExecutionException e) {
            callbacks.logLine("Failed to send the " + canPort + " OpenBLT reboot command: " + e.getCause());
        }
        return false;
    }

    private static boolean canUpdateFirmware(
        UpdateOperationCallbacks callbacks,
        ConnectivityContext connectivityContext,
        String canPort,
        String fileName,
        OpenBltFlasher.PreparedFirmware preparedFirmware
    ) {
        return flashCanWithSuspendedScanner(
            canPort,
            fileName,
            callbacks,
            connectivityContext.getPortScanner(),
            (firmware, openbltCallbacks) -> OpenBltFlasher.flashCan(
                preparedFirmware,
                LinkManager.SOCKET_CAN.equals(canPort) ? new SocketCanRawPort() : new PCanRawPort(),
                openbltCallbacks));
    }

    static boolean flashCanWithSuspendedScanner(
        String canPort,
        String fileName,
        UpdateOperationCallbacks callbacks,
        PortScanner scanner,
        CanFlashAction flashAction
    ) {
        try {
            try {
                if (!scanner.suspend().await(30, TimeUnit.SECONDS)) {
                    callbacks.logLine("Timed out waiting for " + canPort + " discovery to stop.");
                    return false;
                }
            } catch (InterruptedException e) {
                Thread.currentThread().interrupt();
                callbacks.logLine("Interrupted while waiting for " + canPort + " discovery to stop.");
                return false;
            }

            final OpenbltJni.OpenbltCallbacks openbltCallbacks = makeOpenbltCallbacks(callbacks);
            callbacks.logLine("flashCan " + canPort + " " + fileName);
            flashAction.flash(fileName, openbltCallbacks);
            callbacks.logLine("Update completed successfully!");
            return true;
        } catch (Throwable e) {
            callbacks.logLine("flashOpenBltCan Error: " + e);
            log.error("flashOpenBltCan " + e, e);
            return false;
        } finally {
            // Remove the pre-flash ECU result so calibration restore waits for a fresh firmware reply.
            try {
                scanner.invalidatePort(canPort);
            } finally {
                scanner.resume();
            }
        }
    }

    private static boolean bltUpdateFirmware(JComponent parent, PortResult ecuPort, UpdateOperationCallbacks callbacks,
                                             ConnectivityContext connectivityContext, @Nullable String firmwareSrecFile) {
        // Suspend the scanner for the entire reboot → detect → flash sequence so it
        // never races with our direct port probes for exclusive COM port access on Windows.
        try {
            connectivityContext.getPortScanner().suspend().await(30, TimeUnit.SECONDS);
        } catch (InterruptedException e) {
            Thread.currentThread().interrupt();
        }
        try {
            return bltUpdateFirmwareWithSuspendedScanner(parent, ecuPort, callbacks, connectivityContext, firmwareSrecFile);
        } finally {
            // Invalidate the cache so the scanner re-inspects the port (now running new
            // firmware) on its first post-resume scan cycle.
            connectivityContext.getPortScanner().invalidatePort(ecuPort.port);
            connectivityContext.getPortScanner().resume();
        }
    }

    private static boolean bltUpdateFirmwareWithSuspendedScanner(JComponent parent, PortResult ecuPort,
                                                                 UpdateOperationCallbacks callbacks,
                                                                 ConnectivityContext connectivityContext,
                                                                 @Nullable String firmwareSrecFile) {
        // Snapshot pre-existing OpenBLT ports so we can ignore them when searching
        // for the newly-appeared bootloader port after the reboot.
        final List<PortResult> openBltPortsBefore = connectivityContext.getCurrentHardware().getKnownPorts(OpenBlt);

        rebootToOpenblt(parent, ecuPort.port, callbacks);

        // invoking blocking method
        final boolean isEcuPortDisappeared = waitForEcuPortDisappeared(ecuPort, callbacks);

        if (!isEcuPortDisappeared) {
            callbacks.logLine("Looks like your ECU still haven't rebooted to OpenBLT");
            callbacks.logLine("");
            callbacks.logLine("Try closing and opening console again");
            callbacks.logLine("");
            return false;
        }

        final List<PortResult> newItems = waitForNewOpenBltPortAppeared(openBltPortsBefore, callbacks);

        // Check that exactly one thing appeared in the "after" list
        if (newItems.isEmpty()) {
            callbacks.logLine("Looks like your ECU disappeared during the update process. Please try again.");
            return false;
        }

        if (newItems.size() > 1) {
            // More than one port appeared? whattt?
            callbacks.logLine("Unable to find ECU after reboot as multiple serial ports appeared.");
            return false;
        }

        final String openbltPort = newItems.get(0).port;

        callbacks.logLine("Serial port " + openbltPort + " appeared, programming firmware...");

        return flashOpenbltSerial(
            parent, openbltPort, callbacks, connectivityContext.getConnectedEcuTarget(), firmwareSrecFile);
    }

    private static OpenbltJni.OpenbltCallbacks makeOpenbltCallbacks(UpdateOperationCallbacks callbacks) {
        return new OpenbltJni.OpenbltCallbacks() {
            @Override
            public void log(String line) {
                callbacks.logLine(line);
            }

            @Override
            public void updateProgress(int percent) {
                callbacks.updateProgress(percent);
            }

            @Override
            public void error(String line) {
                throw new RuntimeException(line);
            }

            @Override
            public void setPhase(String title, boolean hasProgress) {
                callbacks.logLine("Phase: " + title);
            }
        };
    }

    private static void showError32bitJava(JComponent parent) {
        JOptionPane.showMessageDialog(parent, "64 bit java required. 32 bit java not supported!",
            "Error", JOptionPane.ERROR_MESSAGE);
    }

    public static boolean flashOpenbltSerial(JComponent parent, String port, UpdateOperationCallbacks callbacks,
                                              com.rusefi.core.io.ConnectedEcuTarget connectedEcuTarget) {
        return flashOpenbltSerial(parent, port, callbacks, connectedEcuTarget, null);
    }

    public static boolean flashOpenbltSerial(JComponent parent, String port, UpdateOperationCallbacks callbacks,
                                             com.rusefi.core.io.ConnectedEcuTarget connectedEcuTarget,
                                             @Nullable String firmwareSrecFile) {
        if (FileLog.is32bitJava()) {
            showError32bitJava(parent);
            return false;
        }

        OpenbltJni.OpenbltCallbacks cb = makeOpenbltCallbacks(callbacks);

        String fileName = firmwareSrecFile != null
            ? firmwareSrecFile
            : FindFileHelper.findSrecFileForConnectedBoard(connectedEcuTarget);
        if (fileName == null) {
            callbacks.logLine(".srec image file not found");
            return false;
        }
        // refuse to silently flash firmware built for a different board (e.g. a dev-build fallback or a naming quirk). [tag:better_ux_for_flashing]
        if (!MaintenanceUtil.confirmFirmwareMatchesBoard(fileName, callbacks, connectedEcuTarget)) {
            callbacks.logLine("Firmware update aborted — firmware/board mismatch.");
            return false;
        }
        try {
            callbacks.logLine("flashSerial " + fileName);
            OpenBltFlasher.flashSerial(fileName, port, cb);

            callbacks.logLine("Update completed successfully!");
            return true;
        } catch (Throwable e) {
            callbacks.logLine("flashOpenbltSerial Error: " + e);
            log.error("flashOpenbltSerial " + e, e);
            return false;
        }
    }

    public static boolean wipeOpenbltSerial(String port, UpdateOperationCallbacks callbacks,
                                            OpenBltWipeArtifact artifact) {
        OpenbltJni.OpenbltCallbacks cb = makeOpenbltCallbacks(callbacks);

        // Once a wipe is confirmed, fail safe: never let a later manual flash restore the suspect tune,
        // even if this erase is interrupted after invalidating the application vector.
        CalibrationsHelper.discardLastEcuCalibrations();
        callbacks.logLine("Previous session tune discarded; the next manual update will keep firmware defaults.");
        try {
            OpenBltFlasher.eraseSerial(artifact, port, cb);
            callbacks.logLine("Emergency wipe completed. The ECU remains in OpenBLT.");
            return true;
        } catch (Throwable e) {
            callbacks.logLine("Emergency OpenBLT wipe error: " + e);
            log.error("wipeOpenbltSerial " + e, e);
            return false;
        }
    }

    private static boolean isAutomaticCanPort(String port) {
        return LinkManager.PCAN.equals(port) || LinkManager.SOCKET_CAN.equals(port);
    }
}
