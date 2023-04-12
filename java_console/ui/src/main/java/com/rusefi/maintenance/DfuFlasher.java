package com.rusefi.maintenance;

import com.opensr5.ini.IniFileModel;
import com.rusefi.Launcher;
import com.rusefi.Timeouts;
import com.rusefi.autodetect.PortDetector;
import com.rusefi.autodetect.SerialAutoChecker;
import com.rusefi.io.DfuHelper;
import com.rusefi.io.IoStream;
import com.rusefi.io.serial.BufferedSerialIoStream;
import com.rusefi.ui.StatusConsumer;
import com.rusefi.ui.StatusWindow;
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

import static com.rusefi.StartupFrame.appendBundleName;

/**
 * @see FirmwareFlasher
 */
public class DfuFlasher {
    private static final String DFU_BINARY_LOCATION = Launcher.TOOLS_PATH + File.separator + "STM32_Programmer_CLI/bin";
    private static final String DFU_BINARY = "STM32_Programmer_CLI.exe";
    private static final String WMIC_DFU_QUERY_COMMAND = "wmic path win32_pnpentity where \"Caption like '%STM32%' and Caption like '%Bootloader%'\" get Caption,ConfigManagerErrorCode /format:list";
    private static final String WMIC_STLINK_QUERY_COMMAND = "wmic path win32_pnpentity where \"Caption like '%STLink%'\" get Caption,ConfigManagerErrorCode /format:list";

    public static void doAutoDfu(Object selectedItem, JComponent parent) {
        if (selectedItem == null) {
            JOptionPane.showMessageDialog(parent, "Failed to locate serial ports");
            return;
        }
        String port = selectedItem.toString();

        StatusWindow wnd = createStatusWindow();

        AtomicBoolean isSignatureValidated = rebootToDfu(parent, port, wnd);
        if (isSignatureValidated == null)
            return;
        if (isSignatureValidated.get()) {
            if (!ProgramSelector.IS_WIN) {
                wnd.append("Switched to DFU mode!");
                wnd.append("FOME console can only program on Windows");
                return;
            }
            submitAction(() -> {
                timeForDfuSwitch(wnd);
                executeDFU(wnd);
            });
        } else {
            wnd.append("Please use manual DFU to change bundle type.");
        }
    }

    private static void submitAction(Runnable r) {
        ExecHelper.submitAction(r, DfuFlasher.class + " thread");
    }

    @Nullable
    public static AtomicBoolean rebootToDfu(JComponent parent, String port, StatusWindow wnd) {
        AtomicBoolean isSignatureValidated = new AtomicBoolean(true);
        if (!PortDetector.isAutoPort(port)) {
            wnd.append("Using selected " + port + "\n");
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
                wnd.append("*** ERROR *** FOME has not responded on selected " + port + "\n" +
                        "Maybe try automatic serial port detection?");
                wnd.setErrorState();
                return null;
            }
            boolean isSignatureValidatedLocal = DfuHelper.sendDfuRebootCommand(parent, signature.get(), stream, wnd);
            isSignatureValidated.set(isSignatureValidatedLocal);
        } else {
            wnd.append("Auto-detecting port...\n");
            // instead of opening the just-detected port we execute the command using the same stream we used to discover port
            // it's more reliable this way
            // ISSUE: that's blocking stuff on UI thread at the moment, TODO smarter threading!
            port = PortDetector.autoDetectSerial(callbackContext -> {
                boolean isSignatureValidatedLocal = DfuHelper.sendDfuRebootCommand(parent, callbackContext.getSignature(), callbackContext.getStream(), wnd);
                isSignatureValidated.set(isSignatureValidatedLocal);
                return null;
            }).getSerialPort();
            if (port == null) {
                wnd.append("*** ERROR *** FOME serial port not detected");
                wnd.setErrorState();
                return null;
            } else {
                wnd.append("Detected FOME on " + port + "\n");
            }
        }
        return isSignatureValidated;
    }

    @NotNull
    protected static StatusWindow createStatusWindow() {
        StatusWindow wnd = new StatusWindow();
        wnd.showFrame(appendBundleName("DFU status " + Launcher.CONSOLE_VERSION));
        return wnd;
    }

    public static void runDfuErase() {
        StatusWindow wnd = createStatusWindow();
        submitAction(() -> {
            ExecHelper.executeCommand(DFU_BINARY_LOCATION,
                    getDfuEraseCommand(),
                    DFU_BINARY, wnd, new StringBuffer());
            // it's a lengthy operation let's signal end
            Toolkit.getDefaultToolkit().beep();
        });
    }

    public static void runDfuProgramming() {
        StatusWindow wnd = createStatusWindow();
        submitAction(() -> executeDFU(wnd));
    }

    private static void executeDFU(StatusWindow wnd) {
        boolean driverIsHappy = detectSTM32BootloaderDriverState(wnd);
        if (!driverIsHappy) {
            wnd.append("*** DRIVER ERROR? *** Did you have a chance to try 'Install Drivers' button on top of FOME console start screen?");
            wnd.setErrorState();
            return;
        }

        StringBuffer stdout = new StringBuffer();
        String errorResponse;
        try {
            errorResponse = ExecHelper.executeCommand(DFU_BINARY_LOCATION,
                    getDfuWriteCommand(),
                    DFU_BINARY, wnd, stdout);
        } catch (FileNotFoundException e) {
            wnd.append("ERROR: " + e);
            wnd.setErrorState();
            return;
        }
        if (stdout.toString().contains("Download verified successfully")) {
            // looks like sometimes we are not catching the last line of the response? 'Upgrade' happens before 'Verify'
            wnd.append("SUCCESS!");
            wnd.append("Please power cycle device to exit DFU mode");
            wnd.setSuccessState();
        } else if (stdout.toString().contains("Target device not found")) {
            wnd.append("ERROR: Device not connected or STM32 Bootloader driver not installed?");
            appendWindowsVersion(wnd);
            wnd.append("ERROR: Please try installing drivers using 'Install Drivers' button on FOME splash screen");
            wnd.append("ERROR: Alternatively please install drivers using Device Manager pointing at 'drivers/silent_st_drivers/DFU_Driver' folder");
            appendDeviceReport(wnd);
            wnd.setErrorState();
        } else {
            wnd.append(stdout.length() + " / " + errorResponse.length());
            appendWindowsVersion(wnd);
            appendDeviceReport(wnd);
            wnd.setErrorState();
        }
    }

    public static boolean detectSTM32BootloaderDriverState(StatusConsumer wnd) {
        return detectDevice(wnd, WMIC_DFU_QUERY_COMMAND, "ConfigManagerErrorCode=0");
    }

    private static boolean detectDevice(StatusConsumer wnd, String queryCommand, String pattern) {
        //        long now = System.currentTimeMillis();
        StringBuffer output = new StringBuffer();
        StringBuffer error = new StringBuffer();
        ExecHelper.executeCommand(queryCommand, wnd, output, error, null);
        wnd.append(output.toString());
        wnd.append(error.toString());
//        long cost = System.currentTimeMillis() - now;
//        System.out.println("DFU lookup cost " + cost + "ms");
        return output.toString().contains(pattern);
    }

    private static void appendWindowsVersion(StatusWindow wnd) {
        wnd.append("ERROR: does not look like DFU has worked!");
    }

    private static void appendDeviceReport(StatusWindow wnd) {
        for (String line : getDevicesReport()) {
            if (line.contains("STM Device in DFU Mode")) {
                wnd.append(" ******************************************************************");
                wnd.append(" ************* YOU NEED TO REMOVE LEGACY DFU DRIVER ***************");
                wnd.append(" ******************************************************************");
            }
            wnd.append("Devices: " + line);
        }
    }

    private static void timeForDfuSwitch(StatusWindow wnd) {
        wnd.append("Giving time for USB enumeration...");
        try {
            // two seconds not enough on my Windows 10
            Thread.sleep(3 * Timeouts.SECOND);
        } catch (InterruptedException e) {
            throw new IllegalStateException(e);
        }
    }

    private static String getDfuWriteCommand() throws FileNotFoundException {
        String prefix = "fome";
        String suffix = ".bin";
        String fileName = IniFileModel.findFile(Launcher.INPUT_FILES_PATH, prefix, suffix);
        if (fileName == null)
            throw new FileNotFoundException("File not found " + prefix + "*" + suffix);
        // we need quotes in case if absolute path contains spaces
        String hexAbsolutePath = quote(new File(fileName).getAbsolutePath());

        return DFU_BINARY_LOCATION + "/" + DFU_BINARY + " -c port=usb1 -w " + hexAbsolutePath + " 0x08000000 -v -s";
    }

    private static String quote(String absolutePath) {
        return "\"" + absolutePath + "\"";
    }

    private static String getDfuEraseCommand() {
        return DFU_BINARY_LOCATION + "/" + DFU_BINARY + " -c port=usb1 -e all";
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
                if (!lowerCase.contains("stm32") && !lowerCase.contains("dfu") && !lowerCase.contains("fome"))
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
