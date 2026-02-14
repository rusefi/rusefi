package com.rusefi;

import com.rusefi.ui.UIContext;

import javax.swing.*;
import java.awt.*;

public class DevicePane {
    private final JPanel content = new JPanel();

    public DevicePane(UIContext uiContext, SerialPortType serialPortType) {
        if (serialPortType == SerialPortType.Ecu) {
            content.add(new JLabel("Legacy Device Without OpenBLT"));
        }
    }

    public Component getContent() {
        return content;
    }
}
