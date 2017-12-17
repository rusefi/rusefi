package com.rusefi.maintenance;

import org.jetbrains.annotations.NotNull;

import javax.swing.*;
import java.awt.event.ActionEvent;

/**
 * (c) Andrey Belomutskiy 2013-2017
 */
public class EraseChip extends ProcessStatusWindow {
    private static final String FLASH_SIZE = "0x0100000";
    private static final String ERASE_COMMAND_SUFFIX = " -c init -c targets -c \"halt\" -c \"flash erase_address 0x08000000 " + FLASH_SIZE + "\" -c shutdown";

    private final JButton button = new JButton("Erase Chip");

    public EraseChip() {
        button.addActionListener(new AbstractAction() {
            @Override
            public void actionPerformed(ActionEvent e) {
                int dialogResult = JOptionPane.showConfirmDialog(button, "Do you really want to reset stm32 chip and all settings? Please disconnect battery before erasing.",
                        "Please disconnect from vehicle", JOptionPane.YES_NO_OPTION);
                if (dialogResult != JOptionPane.YES_OPTION)
                    return;
                wnd.showFrame("rusEfi Firmware Flasher");
                submitAction(() -> executeCommand(getEraseCommand()));
            }
        });
    }

    @NotNull
    private String getEraseCommand() {
        return FirmwareFlasher.getOpenovdCommad() + ERASE_COMMAND_SUFFIX;
    }

    public JButton getButton() {
        return button;
    }
}