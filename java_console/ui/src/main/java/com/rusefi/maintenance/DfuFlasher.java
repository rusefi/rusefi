package com.rusefi.maintenance;

import com.rusefi.*;
import com.rusefi.autodetect.PortDetector;
import com.rusefi.autodetect.SerialAutoChecker;
import com.rusefi.config.generated.Integration;
import com.rusefi.core.FindFileHelper;
import com.rusefi.core.io.ConnectedEcuTarget;
import com.rusefi.core.net.ConnectionAndMeta;
import com.rusefi.binaryprotocol.BinaryProtocol;
import com.rusefi.io.BootloaderHelper;
import com.rusefi.io.IoStream;
import com.rusefi.io.LinkManager;
import com.rusefi.io.UpdateOperationCallbacks;
import com.rusefi.io.serial.BufferedSerialIoStream;
import com.rusefi.maintenance.jobs.JobHelper;
import org.jetbrains.annotations.NotNull;
import org.jetbrains.annotations.Nullable;

import javax.swing.*;
import java.awt.*;
import java.io.*;
import java.util.ArrayList;
import java.util.Collections;
import java.util.List;
import java.util.concurrent.atomic.AtomicBoolean;
import java.util.concurrent.atomic.AtomicReference;

/**
 * @see StLinkFlasher
 */
public class DfuFlasher {
    private static final String DFU_CMD_TOOL_LOCATION = Launcher.TOOLS_PATH + File.separator + "STM32_Programmer_CLI/bin";
    private static final String DFU_CMD_TOOL = "STM32_Programmer_CLI.exe";
    private static final String WMIC_DFU_QUERY_COMMAND = "powershell -NoProfile -Command \"Get-CimInstance Win32_PnPEntity -Filter \\\"Caption like '%STM32%' and Caption like '%Bootloader%'\\\" | Select-Object Caption, ConfigManagerErrorCode | Format-List\"";
    private static final String WMIC_DFU_QUERY_H7_COMMAND = "powershell -NoProfile -Command \"Get-CimInstance Win32_PnPEntity -Filter \\\"Caption like '%DFU%' and Caption like '%FS Mode%'\\\" | Select-Object Caption, ConfigManagerErrorCode | Format-List\"";
    /**
     * Set to true when the DFU device-detection command itself fails to execute (e.g. insufficient privileges).
     * Used by the UI to show a "Run as Administrator" hint after a grace period.
     */
    public static volatile boolean dfuDetectionCommandFailed = false;

    public static boolean haveBootloaderBinFile() {
        return new File(FindFileHelper.findBootloaderFile()).exists();
    }

    public static boolean doAutoDfu(
        final JComponent parent,
        final PortResult port,
        final BinaryProtocol bp,
        final LinkManager lm,
        final UpdateOperationCallbacks callbacks,
        final ConnectivityContext connectivityContext
    ) {
        return doAutoDfu(parent, port, bp, lm, callbacks, connectivityContext, null);
    }

    public static boolean doAutoDfu(
        final JComponent parent,
        final PortResult port,
        final BinaryProtocol bp,
        final LinkManager lm,
        final UpdateOperationCallbacks callbacks,
        final ConnectivityContext connectivityContext,
        final @Nullable String firmwareBinFile
    ) {
        return doAutoDfu(parent, port, bp, lm, callbacks, connectivityContext, firmwareBinFile,
            CalibrationsHelper.FirmwareUpdatePolicy.FORWARD_MIGRATION);
    }

    public static boolean doAutoDfu(
        final JComponent parent,
        final PortResult port,
        final BinaryProtocol bp,
        final LinkManager lm,
        final UpdateOperationCallbacks callbacks,
        final ConnectivityContext connectivityContext,
        final @Nullable String firmwareBinFile,
        final CalibrationsHelper.FirmwareUpdatePolicy policy
    ) {
        return CalibrationsHelper.updateFirmwareAndRestorePreviousCalibrations(
            parent, port, bp, lm, callbacks,
            () -> dfuUpdateFirmware(parent, port.port, callbacks, connectivityContext.getConnectedEcuTarget(), firmwareBinFile),
            connectivityContext, policy);
    }

    private static boolean dfuUpdateFirmware(
        final JComponent parent,
        final String port,
        final UpdateOperationCallbacks callbacks,
        final ConnectedEcuTarget connectedEcuTarget,
        final @Nullable String firmwareBinFile
    ) {
        if (port == null) {
            JOptionPane.showMessageDialog(parent, "Failed to locate serial ports");
            return false;
        }

        AtomicBoolean isSignatureValidated = rebootToDfu(parent, port, callbacks, Integration.CMD_REBOOT_DFU);
        if (isSignatureValidated == null)
            return false;
        if (isSignatureValidated.get()) {
            if (!FileLog.isWindows()) {
                callbacks.logLine("Switched to DFU mode!");
                callbacks.logLine("rusEFI console can only program on Windows");
                return false;
            }

            timeForDfuSwitch(callbacks);
            final String fileName = firmwareBinFile != null
                ? firmwareBinFile
                : FindFileHelper.findFirmwareFileForConnectedBoard(connectedEcuTarget);
            if (executeDFU(callbacks, fileName, connectedEcuTarget)) {
                // We need to wait to allow connection to ECU port (see #7403)
                timeForDfuSwitch(callbacks);
                return true;
            } else {
                return false;
            }
        } else {
            callbacks.logLine("Please use manual DFU to change bundle type.");
            return false;
        }
    }

    private static void submitAction(Runnable r) {
        ExecHelper.submitAction(r, DfuFlasher.class + " thread");
    }

    @Nullable
    public static AtomicBoolean rebootToDfu(JComponent parent, String port, UpdateOperationCallbacks callbacks, String command) {
        AtomicBoolean isSignatureValidated = new AtomicBoolean(true);
        if (!PortDetector.isAutoPort(port)) {
            callbacks.logLine("Using selected " + port + "\n");
            try (final IoStream stream = BufferedSerialIoStream.openPort(port)) {
                AtomicReference<String> signature = new AtomicReference<>();
                SerialAutoChecker.checkResponse(stream, callbackContext -> {
                    signature.set(callbackContext.getSignature());
                    return null;
                });
                if (signature.get() == null) {
                    callbacks.logLine("");
                    callbacks.logLine("");
                    callbacks.logLine("");
                    callbacks.logLine("Make sure TUNERSTUDIO IS DISCONNECTED FROM ECU");
                    callbacks.logLine("");
                    callbacks.logLine("");
                    callbacks.logLine("");
                    callbacks.logLine("*** ERROR *** rusEFI has not responded on selected " + port + "\n" +
                        "Maybe try automatic serial port detection?");
                    callbacks.error();
                    return null;
                }
                boolean isSignatureValidatedLocal = BootloaderHelper.sendBootloaderRebootCommand(parent, signature.get(), stream, callbacks, command);
                isSignatureValidated.set(isSignatureValidatedLocal);
            }
        } else {
            callbacks.logLine("Auto-detecting port...\n");
            // instead of opening the just-detected port we execute the command using the same stream we used to discover port
            // it's more reliable this way
            // ISSUE: that's blocking stuff on UI thread at the moment, TODO smarter threading!
            port = PortDetector.autoDetectSerial(callbackContext -> {
                boolean isSignatureValidatedLocal = BootloaderHelper.sendBootloaderRebootCommand(parent, callbackContext.getSignature(), callbackContext.getStream(), callbacks, command);
                isSignatureValidated.set(isSignatureValidatedLocal);
                return null;
            }).getSerialPort();
            if (port == null) {
                callbacks.logLine("*** ERROR *** ECU serial port not detected");
                callbacks.error();
                return null;
            } else {
                callbacks.logLine("ECU Detected on " + port + "\n");
            }
        }
        return isSignatureValidated;
    }

    public static void runDfuEraseAsync(UpdateOperationCallbacks callbacks, final Runnable onJobFinished) {
        submitAction(() -> {
            JobHelper.doJob(() -> {
                    runDfuErase(callbacks);
                    // it's a lengthy operation let's signal end
                    Toolkit.getDefaultToolkit().beep();
                },
                onJobFinished
            );
        });
    }

    private static void runDfuErase(UpdateOperationCallbacks callbacks) {
        try {
            ExecHelper.executeCommand(DFU_CMD_TOOL_LOCATION,
                    getDfuEraseCommand(),
                DFU_CMD_TOOL, callbacks);
        } catch (FileNotFoundException e) {
            callbacks.logLine(e.toString());
            callbacks.error();
        }
    }

    public static void runDfuProgramming(UpdateOperationCallbacks callbacks, final Runnable onJobFinished,
                                         final ConnectedEcuTarget connectedEcuTarget) {
        submitAction(() -> {
            JobHelper.doJob(
                () -> {
                    // A board sitting in DFU has no live signature, so fetch the right firmware for the
                    // persisted last-connected board first; fail closed rather than flash the bundle
                    // default onto a different board on a universal bundle. [tag:better_ux_for_flashing] / #9714
                    if (!MaintenanceUtil.ensureFirmwareForConnectedTarget(callbacks, connectedEcuTarget)) {
                        callbacks.error();
                        return;
                    }
                    executeDfuAndPaintStatusPanel(callbacks, FindFileHelper.findFirmwareFileForConnectedBoard(connectedEcuTarget), connectedEcuTarget);
                },
                onJobFinished
            );
        });
    }

    public static void runOpenBltInitialProgramming(UpdateOperationCallbacks callbacks, final Runnable onJobFinished,
                                                    final ConnectedEcuTarget connectedEcuTarget) {
        submitAction(() -> {
            JobHelper.doJob(
                () -> executeDfuAndPaintStatusPanel(callbacks,
                    FindFileHelper.findBootloaderFileForConnectedBoard(connectedEcuTarget), connectedEcuTarget),
                onJobFinished
            );
        });
    }

    private static void executeDfuAndPaintStatusPanel(
        final UpdateOperationCallbacks callbacks,
        final String firmwareBinFile,
        final ConnectedEcuTarget connectedEcuTarget
    ) {
        if (executeDFU(callbacks, firmwareBinFile, connectedEcuTarget)) {
            callbacks.done();
        } else {
            callbacks.error();
        }
    }

    private static boolean executeDFU(UpdateOperationCallbacks callbacks, String firmwareBinFile,
                                      ConnectedEcuTarget connectedEcuTarget) {
        // Refuse to silently flash firmware built for a different board (brick guard, [tag:better_ux_for_flashing]).
        if (!MaintenanceUtil.confirmFirmwareMatchesBoard(firmwareBinFile, callbacks, connectedEcuTarget)) {
            callbacks.logLine("Firmware update aborted — firmware/board mismatch.");
            return false;
        }
        boolean driverIsHappy = detectSTM32BootloaderDriverState(callbacks, connectedEcuTarget);
        if (!driverIsHappy) {
            callbacks.logLine("*** DRIVER ERROR? *** Did you have a chance to try 'Install Drivers' button on top of rusEFI console start screen?");
            return false;
        }

        StringBuffer stdout = new StringBuffer();
        String errorResponse;
        try {
            errorResponse = ExecHelper.executeCommand(DFU_CMD_TOOL_LOCATION,
                    getDfuWriteCommand(firmwareBinFile),
                DFU_CMD_TOOL, callbacks, stdout);
        } catch (FileNotFoundException e) {
            callbacks.logLine("ERROR: " + e);
            return false;
        }

        if (stdout.toString().contains("Download verified successfully")) {
            // looks like sometimes we are not catching the last line of the response? 'Upgrade' happens before 'Verify'
            callbacks.logLine("SUCCESS!");
            callbacks.logLine("Please power cycle device to exit DFU mode");
            return true;
        } else if (stdout.toString().contains("Target device not found")) {
            callbacks.logLine("ERROR: Device not connected or STM32 Bootloader driver not installed?");
            appendWindowsVersion(callbacks);
            callbacks.logLine("ERROR: Please try installing drivers using 'Install Drivers' button on rusEFI splash screen");
            callbacks.logLine("ERROR: Alternatively please install drivers using Device Manager pointing at 'drivers/silent_st_drivers/DFU_Driver' folder");
            appendDeviceReport(callbacks);
            return false;
        } else {
            appendWindowsVersion(callbacks);
            appendDeviceReport(callbacks);
            callbacks.logLine(stdout.length() + " / " + errorResponse.length());
            return false;
        }
    }

    public static boolean detectSTM32BootloaderDriverState(UpdateOperationCallbacks callbacks,
                                                           ConnectedEcuTarget connectedEcuTarget) {
        if (!FileLog.isWindows()) {
            // The WMIC/driver check below is Windows-only. On Linux the STM32 system bootloader is
            // visible directly over USB as 0483:df11 ("STM Device in DFU Mode"), so probe lsusb so the
            // console at least reflects the DFU state (flashing it is still Windows-only, see [tag:better_ux_for_flashing]).
            return FileLog.isLinux() && detectStm32DfuViaLsusb(callbacks);
        }
        // #9714: a universal bundle's is_h7 property can't cover every board, so trust the connected
        // ECU's target first (e.g. "uaefi_pro_h7"), falling back to the bundled is_h7 property.
        String effectiveTarget = connectedEcuTarget.effectiveTarget();
        boolean isH7 = (effectiveTarget != null && effectiveTarget.contains("h7")) || ConnectionAndMeta.getBoolean("is_h7");
        String command = isH7 ? WMIC_DFU_QUERY_H7_COMMAND : WMIC_DFU_QUERY_COMMAND;
        try {
            return MaintenanceUtil.detectDevice(callbacks, command, "ConfigManagerErrorCode=0");
        } catch (ErrorExecutingCommand e) {
            dfuDetectionCommandFailed = true;
            callbacks.logLine("detectSTM32BootloaderDriverState IOError: " + e);
            return false;
        }
    }

    // STM32 system bootloader enumerates as USB 0483:df11 regardless of chip family; detect it on Linux
    // via lsusb so a board reset into DFU surfaces in the console instead of leaving the watchdog spinning
    // on the vanished serial port. [tag:better_ux_for_flashing]
    private static boolean detectStm32DfuViaLsusb(UpdateOperationCallbacks callbacks) {
        try {
            Process process = new ProcessBuilder("lsusb").redirectErrorStream(true).start();
            StringBuilder output = new StringBuilder();
            try (java.io.BufferedReader reader =
                     new java.io.BufferedReader(new java.io.InputStreamReader(process.getInputStream()))) {
                String line;
                while ((line = reader.readLine()) != null) {
                    output.append(line).append('\n');
                }
            }
            process.waitFor(3, java.util.concurrent.TimeUnit.SECONDS);
            return output.toString().toLowerCase().contains("0483:df11");
        } catch (Exception e) {
            callbacks.logLine("lsusb DFU probe failed: " + e);
            return false;
        }
    }

    private static void appendWindowsVersion(UpdateOperationCallbacks callbacks) {
        callbacks.logLine("ERROR: does not look like DFU has worked!");
    }

    private static void appendDeviceReport(UpdateOperationCallbacks callbacks) {
        for (String line : getDevicesReport()) {
            if (line.contains("STM Device in DFU Mode")) {
                callbacks.logLine(" ******************************************************************");
                callbacks.logLine(" ************* YOU NEED TO REMOVE LEGACY DFU DRIVER ***************");
                callbacks.logLine(" ******************************************************************");
            }
            callbacks.logLine("Devices: " + line);
        }
    }

    private static void timeForDfuSwitch(UpdateOperationCallbacks callbacks) {
        callbacks.logLine("Giving time for USB enumeration...");
        try {
            // two seconds not enough on my Windows 10
            Thread.sleep(3 * Timeouts.SECOND);
        } catch (InterruptedException e) {
            throw new IllegalStateException(e);
        }
    }

    private static String getDfuWriteCommand(String fileName) throws FileNotFoundException {
        // we need quotes in case if absolute path contains spaces
        String quotedAbsolutePath = quote(new File(fileName).getAbsolutePath());

        return DFU_CMD_TOOL_LOCATION + "/" + DFU_CMD_TOOL + " -c port=usb1 -w " + quotedAbsolutePath + " 0x08000000 -v -s";
    }

    private static String quote(String absolutePath) {
        return "\"" + absolutePath + "\"";
    }

    private static String getDfuEraseCommand() {
        return DFU_CMD_TOOL_LOCATION + "/" + DFU_CMD_TOOL + " -c port=usb1 -e all";
    }

    @NotNull
    static List<String> getDevicesReport() {
        // todo: assert windows 10, explicit message if not
        List<String> report = new ArrayList<>();

        try {
            Process powerShellProcess = Runtime.getRuntime().exec("powershell \"Get-PnpDevice -PresentOnly\"");
            // Getting the results
            powerShellProcess.getOutputStream().close();

            String line;
            BufferedReader stdout = new BufferedReader(new InputStreamReader(powerShellProcess.getInputStream()));
            while ((line = stdout.readLine()) != null) {
                String lowerCase = line.toLowerCase();
                if (!lowerCase.contains("stm32") && !lowerCase.contains("dfu") && !lowerCase.contains("rusefi"))
                    continue;
                report.add(line);
            }
            stdout.close();
            return report;
        } catch (IOException e) {
            return Collections.emptyList();
        }
    }
}
