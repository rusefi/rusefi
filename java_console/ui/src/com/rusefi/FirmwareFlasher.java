package com.rusefi;

import com.rusefi.ui.FrameHelper;

import javax.swing.*;
import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.io.IOException;

/**
 * (c) Andrey Belomutskiy 2013-2015
 * 2/4/15
 */
public class FirmwareFlasher {
    private static final String OPEN_OCD_COMMAND = "openocd/bin/openocd-0.8.0.exe -f interface/stlink-v2.cfg -f board/stm32f4discovery.cfg -c init -c targets -c \"halt\" -c \"flash write_image erase rusefi.elf\" -c \"verify_image rusefi.elf\" -c \"reset run\" -c shutdown";

    private final JButton button = new JButton("wip");
    private final JTextArea log = new JTextArea();

    public FirmwareFlasher() {
        log.setLineWrap(true);

        button.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent event) {
                FrameHelper f = new FrameHelper();

                appendMsg("Executing " + OPEN_OCD_COMMAND);

                f.showFrame(log, false);

                try {
                    Process p = Runtime.getRuntime().exec(OPEN_OCD_COMMAND);
                

                } catch (IOException e) {
                    appendMsg("Error: " + e);
                }


            }
        });
    }

    private void appendMsg(String s) {
        log.append(s);
    }

    public static boolean isWindows() {
        return System.getProperty("os.name").toLowerCase().contains("win");
    }

    public static Component getContent() {
        return new FirmwareFlasher().button;
    }
}
