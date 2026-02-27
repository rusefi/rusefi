package com.rusefi;

import com.rusefi.core.preferences.storage.PersistentConfiguration;
import com.rusefi.io.UpdateOperationCallbacks;
import com.rusefi.maintenance.jobs.OpenBltAutoJob;
import com.rusefi.maintenance.jobs.OpenBltSwitchJob;
import com.rusefi.ui.StatusWindow;
import com.rusefi.ui.UIContext;

import javax.swing.*;
import java.awt.*;

public class DevicePane {
    private final JPanel content = new JPanel();

    public DevicePane(UIContext uiContext, String port, SerialPortType serialPortType) {
        JCheckBox autoUpdateBundle = new JCheckBox("Auto-update bundle", PersistentConfiguration.getBoolProperty(AutoupdateProperty.AUTO_UPDATE_BUNDLE_PROPERTY, true));
        autoUpdateBundle.addActionListener(e -> PersistentConfiguration.setBoolProperty(AutoupdateProperty.AUTO_UPDATE_BUNDLE_PROPERTY, autoUpdateBundle.isSelected()));
        content.add(autoUpdateBundle);

        if (serialPortType == SerialPortType.Ecu) {
            content.add(new JLabel("Legacy Device Without OpenBLT"));
        } else if (serialPortType == SerialPortType.EcuWithOpenblt) {
            JButton switchButton = new JButton("OpenBltSwitchJob");
            switchButton.addActionListener(e -> {
                UpdateOperationCallbacks callbacks = StatusWindow.createAndShowFrame("OpenBLT");
                new Thread(() -> {
                    OpenBltSwitchJob job = new OpenBltSwitchJob(new PortResult(port, serialPortType), content);
                    job.doJob(callbacks, () -> {
                    });
                }).start();
            });
            content.add(switchButton);

            JButton autoButton = new JButton("OpenBltAutoJob");
            autoButton.addActionListener(e -> {
                UpdateOperationCallbacks callbacks = StatusWindow.createAndShowFrame("OpenBLT");
                new Thread(() -> {
                    OpenBltAutoJob job = new OpenBltAutoJob(new PortResult(port, serialPortType), content, ConnectivityContext.INSTANCE);
                    job.doJob(callbacks, () -> {
                    });
                }).start();
            });
            content.add(autoButton);
        }
    }

    public Component getContent() {
        return content;
    }
}
