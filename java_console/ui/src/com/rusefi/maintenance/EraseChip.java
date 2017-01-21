package com.rusefi.maintenance;

import javax.swing.*;
import java.awt.event.ActionEvent;

/**
 * (c) Andrey Belomutskiy 2013-2017
 */
public class EraseChip extends ProcessStatusWindow {
    private final JButton button = new JButton("Erase Chip");

    private static final String OPEN_OCD_COMMAND = FirmwareFlasher.OPENOCD_BIN +
            " -c init -c targets -c \"halt\" -c \"flash erase_address 0x08000000 0x080000\" -c shutdown";

    public EraseChip() {
        button.addActionListener(new AbstractAction() {
            @Override
            public void actionPerformed(ActionEvent e) {
                int dialogResult = JOptionPane.showConfirmDialog(button, "Do you really want to reset stm32 chip and all settings? Please disconnect battery before erasing.",
                        "Please disconnect from vehicle", JOptionPane.YES_NO_OPTION);
                if (dialogResult != JOptionPane.YES_OPTION)
                    return;
                wnd.showFrame("rusEfi Firmware Flasher");
                submitAction(new Runnable() {
                    @Override
                    public void run() {
                        executeCommand(OPEN_OCD_COMMAND);
                    }
                });
            }
        });
    }

    public JButton getButton() {
        return button;
    }
}