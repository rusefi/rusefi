package com.rusefi.maintenance;

import com.rusefi.*;
import com.rusefi.autodetect.PortDetector;
import com.rusefi.autodetect.SerialAutoChecker;
import com.rusefi.config.generated.Integration;
import com.rusefi.core.FindFileHelper;
import com.rusefi.core.net.ConnectionAndMeta;
import com.rusefi.io.BootloaderHelper;
import com.rusefi.io.IoStream;
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

import static com.rusefi.core.FindFileHelper.INPUT_FILES_PATH;

/**
 * @see StLinkFlasher
 */
public class DfuFlasher {
    public static final String BOOTLOADER_BIN_FILE = INPUT_FILES_PATH + "/" + "openblt.bin";
    private static final String DFU_CMD_TOOL_LOCATION = Launcher.TOOLS_PATH + File.separator + "STM32_Programmer_CLI/bin";
    private static final String DFU_CMD_TOOL = "STM32_Programmer_CLI.exe";
    private static final String WMIC_DFU_QUERY_COMMAND = "wmic path win32_pnpentity where \"Caption like '%STM32%' and Caption like '%Bootloader%'\" get Caption,ConfigManagerErrorCode /format:list";
    private static final String WMIC_DFU_QUERY_H7_COMMAND = "wmic path win32_pnpentity where \"Caption like '%DFU%' and Caption like '%FS Mode%'\" get Caption,ConfigManagerErrorCode /format:list";
    public static boolean dfuEnabledInCaseOfError = true;

    public static boolean haveBootloaderBinFile() {
        return new File(BOOTLOADER_BIN_FILE).exists();
    }

    public static boolean doAutoDfu(
        final JComponent parent,
        final PortResult port,
        final UpdateOperationCallbacks callbacks, ConnectivityContext connectivityContext
    ) {
        return CalibrationsHelper.updateFirmwareAndRestorePreviousCalibrations(
            port,
            callbacks,
            () -> dfuUpdateFirmware(parent, port.port, callbacks), connectivityContext
        );
    }

    private static boolean dfuUpdateFirmware(
        final JComponent parent,
        final String port,
        final UpdateOperationCallbacks callbacks
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
            if (executeDFU(callbacks, FindFileHelper.findFirmwareFile())) {
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

    public static void runDfuProgramming(UpdateOperationCallbacks callbacks, final Runnable onJobFinished) {
        submitAction(() -> {
            JobHelper.doJob(
                () -> executeDfuAndPaintStatusPanel(callbacks, FindFileHelper.findFirmwareFile()),
                onJobFinished
            );
        });
    }

    public static void runOpenBltInitialProgramming(UpdateOperationCallbacks callbacks, final Runnable onJobFinished) {
        submitAction(() -> {
            JobHelper.doJob(
                () -> executeDfuAndPaintStatusPanel(callbacks, DfuFlasher.BOOTLOADER_BIN_FILE),
                onJobFinished
            );
        });
    }

    private static void executeDfuAndPaintStatusPanel(
        final UpdateOperationCallbacks callbacks,
        final String firmwareBinFile
    ) {
        if (executeDFU(callbacks, firmwareBinFile)) {
            callbacks.done();
        } else {
            callbacks.error();
        }
    }

    private static boolean executeDFU(UpdateOperationCallbacks callbacks, String firmwareBinFile) {
        boolean driverIsHappy = detectSTM32BootloaderDriverState(callbacks);
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

    public static boolean detectSTM32BootloaderDriverState(UpdateOperationCallbacks callbacks) {
        String command = ConnectionAndMeta.getBoolean("is_h7") ? WMIC_DFU_QUERY_H7_COMMAND : WMIC_DFU_QUERY_COMMAND;
        return MaintenanceUtil.detectDevice(callbacks, command, "ConfigManagerErrorCode=0", dfuEnabledInCaseOfError);
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
