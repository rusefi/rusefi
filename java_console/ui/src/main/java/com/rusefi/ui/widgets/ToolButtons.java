package com.rusefi.ui.widgets;

import com.rusefi.core.ui.AutoupdateUtil;
import com.rusefi.maintenance.ExecHelper;
import com.rusefi.ui.PcanConnectorUI;

import javax.swing.*;
import java.awt.*;
import java.io.IOException;

public class ToolButtons {
    public static Component createShowDeviceManagerButton() {
        JButton showDeviceManager = new JButton(AutoupdateUtil.loadIcon("DeviceManager.png"));
        showDeviceManager.setMargin(new Insets(0, 0, 0, 0));
        showDeviceManager.setToolTipText("Show Device Manager");
        showDeviceManager.addActionListener(event -> {
            showDeviceManager();
        });
        return showDeviceManager;
    }

    public static void showDeviceManager() {
        try {
            Runtime.getRuntime().exec(ExecHelper.getBatchCommand("devmgmt.msc"));
        } catch (IOException ex) {
            throw new IllegalStateException(ex);
        }
    }

    public static Component createPcanConnectorButton() {
        JButton button = new JButton("PCAN");
        button.setToolTipText("PCAN connector for TS");
        button.addActionListener(e -> PcanConnectorUI.show());
        return button;
    }
}
