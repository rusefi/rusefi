package com.rusefi.maintenance;

import javax.swing.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.io.File;

import static com.rusefi.Launcher.INPUT_FILES_PATH;

/**
 * (c) Andrey Belomutskiy 2013-2018
 * 2/4/15
 */
public class FirmwareFlasher extends ProcessStatusWindow {
    // Even on Windows openOCD insists on "/" for path separator
    public static final String IMAGE_FILE = INPUT_FILES_PATH + "/" + "rusefi.bin";
    public static final String IMAGE_NO_ASSERTS_FILE = INPUT_FILES_PATH + "/" + "rusefi_no_asserts.bin";
    static final String OPENOCD_EXE = "openocd/openocd.exe";
    static final String BINARY_LOCATION = ".";
    private static final String SUCCESS_MESSAGE_TAG = "shutdown command invoked";
    private static final String FAILED_MESSAGE_TAG = "failed";
    private static final String NO_DRIVER_MESSAGE_TAG = "failed with LIBUSB_ERROR_NOT_SUPPORTED";
    public static final String TITLE = "rusEfi Firmware Flasher";
    public static final String DONE = "DONE!";

    private final JButton button;
    private String fileName;

    public FirmwareFlasher(String fileName, String buttonTest, String tooltip) {
        this.fileName = fileName;
        button = new JButton(buttonTest);
        button.setToolTipText(tooltip);
        button.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent event) {
                int dialogResult = JOptionPane.showConfirmDialog(button, "Do you really want to update firmware? Please disconnect vehicle battery before erasing.",
                        "Please disconnect from vehicle", JOptionPane.YES_NO_OPTION);
                if (dialogResult != JOptionPane.YES_OPTION)
                    return;


                wnd.showFrame(TITLE);

                ExecHelper.submitAction(() -> doFlashFirmware(), getClass() + " extProcessThread");
            }
        });
    }

    public static String getOpenocdCommad() {
        String cfg = "stm32f4discovery.cfg";
        return OPENOCD_EXE + " -f openocd/" + cfg;
    }

    private void doFlashFirmware() {
        if (!new File(fileName).exists()) {
            wnd.appendMsg(fileName + " not found, cannot proceed !!!");
            wnd.setStatus("ERROR");
            return;
        }
        StatusAnimation sa = new StatusAnimation(wnd);
        StringBuffer error = executeCommand(getOpenocdCommad() + " -c \"program " +
                fileName +
                " verify reset exit 0x08000000\"");
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

}
