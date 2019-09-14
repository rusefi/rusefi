package com.rusefi.maintenance;

import com.rusefi.Launcher;
import com.rusefi.ui.StatusWindow;
import org.jetbrains.annotations.NotNull;

import javax.swing.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.io.File;

import static com.rusefi.Launcher.INPUT_FILES_PATH;
import static com.rusefi.ui.storage.PersistentConfiguration.getConfig;

/**
 * @see DfuFlasher
 * <p>
 * (c) Andrey Belomutskiy 2013-2018
 * 2/4/15
 */
public class FirmwareFlasher {
    // Even on Windows openOCD insists on "/" for path separator
    public static final String IMAGE_FILE = INPUT_FILES_PATH + File.separator + "rusefi.bin";
    public static final String IMAGE_NO_ASSERTS_FILE = INPUT_FILES_PATH + File.separator + "rusefi_no_asserts.bin";
    /**
     * SWD ST-LINK/V2 mode
     */
    private static final String OPENOCD_EXE = Launcher.TOOLS_PATH + File.separator + "openocd/openocd.exe";
    // todo: combine this with Launcher#TOOLS_PATH?
    static final String BINARY_LOCATION = ".";
    private static final String SUCCESS_MESSAGE_TAG = "shutdown command invoked";
    private static final String FAILED_MESSAGE_TAG = "failed";
    private static final String NO_DRIVER_MESSAGE_TAG = "failed with LIBUSB_ERROR_NOT_SUPPORTED";
    public static final String TITLE = "rusEfi ST-LINK Firmware Flasher";
    public static final String DONE = "DONE!";

    private final JButton button;
    private String fileName;

    public FirmwareFlasher(String fileName, String buttonTest, String tooltip) {
        this.fileName = fileName;
        button = new JButton(buttonTest);
        button.setToolTipText(tooltip);
        button.addActionListener(event -> doUpdateFirmware(fileName, button));
    }

    public static void doUpdateFirmware(String fileName, JComponent component) {
        StatusWindow wnd = new StatusWindow();
        int dialogResult = JOptionPane.showConfirmDialog(component, "Do you really want to update firmware? Please disconnect vehicle battery before erasing.",
                "Please disconnect from vehicle", JOptionPane.YES_NO_OPTION);
        if (dialogResult != JOptionPane.YES_OPTION)
            return;

        wnd.showFrame(TITLE);

        ExecHelper.submitAction(() -> doFlashFirmware(wnd, fileName), FirmwareFlasher.class + " extProcessThread");
    }

    public static String getOpenocdCommand() {
        String cfg = getHardwareKind().getOpenOcdName();
        return OPENOCD_EXE + " -f openocd/" + cfg;
    }

    protected static StringBuffer executeOpenOCDCommand(String command, StatusWindow wnd) {
        return ExecHelper.executeCommand(BINARY_LOCATION,
                BINARY_LOCATION + File.separator + command,
                OPENOCD_EXE, wnd);
    }

    private static void doFlashFirmware(StatusWindow wnd, String fileName) {
        if (!new File(fileName).exists()) {
            wnd.appendMsg(fileName + " not found, cannot proceed !!!");
            wnd.setStatus("ERROR");
            return;
        }
        StatusAnimation sa = new StatusAnimation(wnd);
        StringBuffer error = executeOpenOCDCommand(getOpenocdCommand() + " -c \"program " +
                fileName +
                " verify reset exit 0x08000000\"", wnd);
        if (error.toString().contains(NO_DRIVER_MESSAGE_TAG)) {
            wnd.appendMsg(" !!! ERROR: looks like stm32 driver is not installed? The link is above !!!");
        } else if (error.toString().contains(SUCCESS_MESSAGE_TAG) && !error.toString().toLowerCase().contains(FAILED_MESSAGE_TAG)) {
            wnd.appendMsg("Flashing looks good!");
            sa.stop();
            wnd.setStatus(DONE);
        } else {
            wnd.appendMsg("!!! FIRMWARE FLASH: DOES NOT LOOK RIGHT !!!");
        }
    }

    public JButton getButton() {
        return button;
    }

    @NotNull
    public static HwPlatform getHardwareKind() {
        String value = getConfig().getRoot().getProperty("hardware", HwPlatform.F4.name());
        return HwPlatform.resolve(value);
    }

}
