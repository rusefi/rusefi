package com.rusefi.maintenance;

import javax.swing.*;
import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.io.*;

/**
 * (c) Andrey Belomutskiy 2013-2015
 * 2/4/15
 */
public class FirmwareFlasher extends ProcessStatusWindow {
    private static final String IMAGE_FILE = "rusefi.bin";
    static final String OPENOCD_BIN = "openocd/bin/openocd-0.8.0.exe";
    private static final String OPEN_OCD_COMMAND = OPENOCD_BIN + " -f interface/stlink-v2.cfg -f board/stm32f4discovery.cfg -c \"program " +
            IMAGE_FILE +
            " verify reset exit 0x08000000\"";
    private static final String SUCCESS_MESSAGE_TAG = "shutdown command invoked";

    private final JButton button = new JButton("Program Firmware");

    public FirmwareFlasher() {
        button.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent event) {
                showFrame();

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

    private void doFlashFirmware() {
        if (!new File(IMAGE_FILE).exists()) {
            appendMsg(IMAGE_FILE + " not found, cannot proceed !!!");
            return;
        }
        StringBuffer error = executeCommand(OPEN_OCD_COMMAND);
        if (error.toString().contains(SUCCESS_MESSAGE_TAG)) {
            appendMsg("!!! Looks good!!!");
        } else {
            appendMsg("!!! FIRMWARE FLASH: DOES NOT LOOK RIGHT !!!");
        }
    }

    public static Component getContent() {
        return new FirmwareFlasher().button;
    }
}
