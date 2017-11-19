package com.rusefi.maintenance;

import com.rusefi.ui.OlderDiscoveryChecbbox;

import javax.swing.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.io.*;

/**
 * (c) Andrey Belomutskiy 2013-2017
 * 2/4/15
 */
public class FirmwareFlasher extends ProcessStatusWindow {
    public static final String IMAGE_DEBUG_FILE = "rusefi_debug.bin";
    public static final String IMAGE_RELEASE_FILE = "rusefi_release.bin";
    static final String OPENOCD_EXE = "openocd/openocd.exe";
    static final String BINARY_LOCATION = ".";
    private static final String SUCCESS_MESSAGE_TAG = "shutdown command invoked";
    private static final String FAILED_MESSAGE_TAG = "failed";
    private static final String NO_DRIVER_MESSAGE_TAG = "failed with LIBUSB_ERROR_NOT_SUPPORTED";

    private final JButton button;
    private String fileName;

    public FirmwareFlasher(String fileName, String buttonTest) {
        this.fileName = fileName;
        button = new JButton(buttonTest);
        button.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent event) {
                int dialogResult = JOptionPane.showConfirmDialog(button, "Do you really want to update firmware? Please disconnect vehicle battery before erasing.",
                        "Please disconnect from vehicle", JOptionPane.YES_NO_OPTION);
                if (dialogResult != JOptionPane.YES_OPTION)
                    return;


                wnd.showFrame("rusEfi Firmware Flasher");

                Runnable runnable = new Runnable() {
                    @Override
                    public void run() {
                        doFlashFirmware();
                    }
                };
                submitAction(runnable);
            }
        });
    }

    public static String getOpenovdCommad() {
        String cfg = OlderDiscoveryChecbbox.olderMode ? "stm32f4discovery.cfg" : "stm32f429disc1.cfg";
        return BINARY_LOCATION + File.separator +  OPENOCD_EXE + " -f openocd/" + cfg;
    }

    private void doFlashFirmware() {
        if (!new File(fileName).exists()) {
            wnd.appendMsg(fileName + " not found, cannot proceed !!!");
            return;
        }
        StringBuffer error = executeCommand(getOpenovdCommad() + " -c \"program " +
                fileName +
                " verify reset exit 0x08000000\"");
        if (error.toString().contains(NO_DRIVER_MESSAGE_TAG)) {
            wnd.appendMsg(" !!! ERROR: looks like stm32 driver is not installed? The link is above !!!");
        } else if (error.toString().contains(SUCCESS_MESSAGE_TAG) && !error.toString().toLowerCase().contains(FAILED_MESSAGE_TAG)) {
            wnd.appendMsg("Flashing looks good!");
        } else {
            wnd.appendMsg("!!! FIRMWARE FLASH: DOES NOT LOOK RIGHT !!!");
        }
    }

    public JButton getButton() {
        return button;
    }

}
