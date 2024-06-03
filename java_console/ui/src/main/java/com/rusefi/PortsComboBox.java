package com.rusefi;

import javax.swing.*;

import static com.rusefi.ui.util.UiUtils.setToolTip;

public class PortsComboBox {
    private final JComboBox<SerialPortScanner.PortResult> comboPorts = new JComboBox<>();

    public PortsComboBox() {
        if (FileLog.isWindows()) {
            setToolTip(comboPorts, "Use 'Device Manager' icon above to launch Device Manager",
                "In 'Ports' section look for ",
                "'STMicroelectronics Virtual COM Port' for USB port",
                "'USB Serial Port' for TTL port");
        }
    }

    public JComboBox<SerialPortScanner.PortResult> getComboPorts() {
        return comboPorts;
    }
}
