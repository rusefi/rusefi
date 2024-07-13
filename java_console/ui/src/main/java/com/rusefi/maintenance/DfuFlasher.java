package com.rusefi.maintenance;

import com.rusefi.FileLog;
import com.rusefi.Launcher;
import com.rusefi.Timeouts;
import com.rusefi.autodetect.PortDetector;
import com.rusefi.autodetect.SerialAutoChecker;
import com.rusefi.core.FindFileHelper;
import com.rusefi.config.generated.Fields;
import com.rusefi.io.DfuHelper;
import com.rusefi.io.IoStream;
import com.rusefi.io.UpdateOperationCallbacks;
import com.rusefi.io.serial.BufferedSerialIoStream;
import org.jetbrains.annotations.NotNull;
import org.jetbrains.annotations.Nullable;

import javax.swing.*;
import java.awt.*;
import java.io.*;
import java.util.ArrayList;
import java.util.Collections;
import java.util.List;
import java.util.concurrent.CountDownLatch;
import java.util.concurrent.atomic.AtomicBoolean;
import java.util.concurrent.atomic.AtomicReference;
import java.util.function.Function;

import static com.rusefi.core.FindFileHelper.INPUT_FILES_PATH;

/**
 * @see StLinkFlasher
 */
public class DfuFlasher {
    public static final String BOOTLOADER_BIN_FILE = INPUT_FILES_PATH + "/" + "openblt.bin";
    private static final String DFU_CMD_TOOL_LOCATION = Launcher.TOOLS_PATH + File.separator + "STM32_Programmer_CLI/bin";
    private static final String DFU_CMD_TOOL = "STM32_Programmer_CLI.exe";
    private static final String WMIC_DFU_QUERY_COMMAND = "wmic path win32_pnpentity where \"Caption like '%STM32%' and Caption like '%Bootloader%'\" get Caption,ConfigManagerErrorCode /format:list";

    public static boolean haveBootloaderBinFile() {
        return new File(BOOTLOADER_BIN_FILE).exists();
    }

    public static void doAutoDfu(JComponent parent, String port, UpdateOperationCallbacks callbacks) {
        if (port == null) {
            JOptionPane.showMessageDialog(parent, "Failed to locate serial ports");
            return;
        }

        AtomicBoolean isSignatureValidated = rebootToDfu(parent, port, callbacks, Fields.CMD_REBOOT_DFU);
        if (isSignatureValidated == null)
            return;
        if (isSignatureValidated.get()) {
            if (!FileLog.isWindows()) {
                callbacks.append("Switched to DFU mode!", true);
                callbacks.append("rusEFI console can only program on Windows", true);
                return;
            }

            submitAction(() -> {
                timeForDfuSwitch(callbacks);
                executeDFU(callbacks, FindFileHelper.FIRMWARE_BIN_FILE);
            });
        } else {
            callbacks.log("Please use manual DFU to change bundle type.", true);
        }
    }

    private static void submitAction(Runnable r) {
        ExecHelper.submitAction(r, DfuFlasher.class + " thread");
    }

    @Nullable
    public static AtomicBoolean rebootToDfu(JComponent parent, String port, UpdateOperationCallbacks callbacks, String command) {
        AtomicBoolean isSignatureValidated = new AtomicBoolean(true);
        if (!PortDetector.isAutoPort(port)) {
            callbacks.log("Using selected " + port + "\n", true);
            IoStream stream = BufferedSerialIoStream.openPort(port);
            AtomicReference<String> signature = new AtomicReference<>();
            new SerialAutoChecker(PortDetector.DetectorMode.DETECT_TS, port, new CountDownLatch(1)).checkResponse(stream, new Function<SerialAutoChecker.CallbackContext, Void>() {
                @Override
                public Void apply(SerialAutoChecker.CallbackContext callbackContext) {
                    signature.set(callbackContext.getSignature());
                    return null;
                }
            });
            if (signature.get() == null) {
                callbacks.append("", true);
                callbacks.append("", true);
                callbacks.append("", true);
                callbacks.append("Make sure TUNERSTUDIO IS DISCONNECTED FROM ECU", true);
                callbacks.append("", true);
                callbacks.append("", true);
                callbacks.append("", true);
              callbacks.append("*** ERROR *** rusEFI has not responded on selected " + port + "\n" +
                        "Maybe try automatic serial port detection?", true);
                callbacks.error();
                return null;
            }
            boolean isSignatureValidatedLocal = DfuHelper.sendDfuRebootCommand(parent, signature.get(), stream, callbacks, command);
            isSignatureValidated.set(isSignatureValidatedLocal);
        } else {
            callbacks.log("Auto-detecting port...\n", true);
            // instead of opening the just-detected port we execute the command using the same stream we used to discover port
            // it's more reliable this way
            // ISSUE: that's blocking stuff on UI thread at the moment, TODO smarter threading!
            port = PortDetector.autoDetectSerial(callbackContext -> {
                boolean isSignatureValidatedLocal = DfuHelper.sendDfuRebootCommand(parent, callbackContext.getSignature(), callbackContext.getStream(), callbacks, command);
                isSignatureValidated.set(isSignatureValidatedLocal);
                return null;
            }).getSerialPort();
            if (port == null) {
                callbacks.append("*** ERROR *** rusEFI serial port not detected", true);
                callbacks.error();
                return null;
            } else {
                callbacks.append("Detected rusEFI on " + port + "\n", true);
            }
        }
        return isSignatureValidated;
    }

    public static void runDfuEraseAsync(UpdateOperationCallbacks callbacks) {
        submitAction(() -> {
            runDfuErase(callbacks);
            // it's a lengthy operation let's signal end
            Toolkit.getDefaultToolkit().beep();
        });
    }

    private static void runDfuErase(UpdateOperationCallbacks callbacks) {
        try {
            ExecHelper.executeCommand(DFU_CMD_TOOL_LOCATION,
                    getDfuEraseCommand(),
                DFU_CMD_TOOL, callbacks);
        } catch (FileNotFoundException e) {
            callbacks.log(e.toString(), true);
            callbacks.error();
        }
    }

    public static void runDfuProgramming(UpdateOperationCallbacks callbacks) {
        submitAction(() -> executeDFU(callbacks, FindFileHelper.FIRMWARE_BIN_FILE));
    }

    public static void runOpenBltInitialProgramming(UpdateOperationCallbacks callbacks) {
        submitAction(() -> executeDFU(callbacks, DfuFlasher.BOOTLOADER_BIN_FILE));
    }

    private static void executeDFU(UpdateOperationCallbacks callbacks, String firmwareBinFile) {
        boolean driverIsHappy = detectSTM32BootloaderDriverState(callbacks);
        if (!driverIsHappy) {
            callbacks.append("*** DRIVER ERROR? *** Did you have a chance to try 'Install Drivers' button on top of rusEFI console start screen?", true);
            callbacks.error();
            return;
        }

        StringBuffer stdout = new StringBuffer();
        String errorResponse;
        try {
            errorResponse = ExecHelper.executeCommand(DFU_CMD_TOOL_LOCATION,
                    getDfuWriteCommand(firmwareBinFile),
                DFU_CMD_TOOL, callbacks, stdout);
        } catch (FileNotFoundException e) {
            callbacks.log("ERROR: " + e, true);
            callbacks.error();
            return;
        }

        if (stdout.toString().contains("Download verified successfully")) {
            // looks like sometimes we are not catching the last line of the response? 'Upgrade' happens before 'Verify'
            callbacks.log("SUCCESS!", true);
            callbacks.log("Please power cycle device to exit DFU mode", true);
            callbacks.done();
        } else if (stdout.toString().contains("Target device not found")) {
            callbacks.append("ERROR: Device not connected or STM32 Bootloader driver not installed?", true);
            appendWindowsVersion(callbacks);
            callbacks.append("ERROR: Please try installing drivers using 'Install Drivers' button on rusEFI splash screen", true);
            callbacks.append("ERROR: Alternatively please install drivers using Device Manager pointing at 'drivers/silent_st_drivers/DFU_Driver' folder", true);
            appendDeviceReport(callbacks);
            callbacks.error();
        } else {
            appendWindowsVersion(callbacks);
            appendDeviceReport(callbacks);
            callbacks.log(stdout.length() + " / " + errorResponse.length(), true);
            callbacks.error();
        }
    }

    public static boolean detectSTM32BootloaderDriverState(UpdateOperationCallbacks callbacks) {
        return MaintenanceUtil.detectDevice(callbacks, WMIC_DFU_QUERY_COMMAND, "ConfigManagerErrorCode=0");
    }

    private static void appendWindowsVersion(UpdateOperationCallbacks callbacks) {
        callbacks.log("ERROR: does not look like DFU has worked!", true);
    }

    private static void appendDeviceReport(UpdateOperationCallbacks callbacks) {
        for (String line : getDevicesReport()) {
            if (line.contains("STM Device in DFU Mode")) {
                callbacks.log(" ******************************************************************", true);
                callbacks.log(" ************* YOU NEED TO REMOVE LEGACY DFU DRIVER ***************", true);
                callbacks.log(" ******************************************************************", true);
            }
            callbacks.log("Devices: " + line, true);
        }
    }

    private static void timeForDfuSwitch(UpdateOperationCallbacks callbacks) {
        callbacks.log("Giving time for USB enumeration...", true);
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
