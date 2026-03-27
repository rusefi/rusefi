package com.rusefi;

import com.rusefi.core.preferences.storage.PersistentConfiguration;
import com.rusefi.maintenance.jobs.AsyncJobExecutor;
import com.rusefi.maintenance.jobs.OpenBltAutoJob;
import com.rusefi.maintenance.jobs.OpenBltSwitchJob;
import com.rusefi.ui.UIContext;
import com.rusefi.ui.basic.MigrateSettingsCheckboxState;
import com.rusefi.ui.basic.StatusPanelWithProgressBar;

import javax.swing.*;
import java.awt.*;

public class DevicePane {
    private final JPanel content = new JPanel();

    public DevicePane(UIContext uiContext, String port, SerialPortType serialPortType, JTabbedPane tabbedPane) {
        content.setLayout(new BoxLayout(content, BoxLayout.Y_AXIS));
        JCheckBox autoUpdateBundle = new JCheckBox("Auto-update bundle", AutoupdateProperty.get());
        autoUpdateBundle.addActionListener(e -> PersistentConfiguration.setBoolProperty(AutoupdateProperty.AUTO_UPDATE_BUNDLE_PROPERTY, autoUpdateBundle.isSelected()));
        content.add(autoUpdateBundle);

        JCheckBox migrateSettings = new JCheckBox("Migrate Settings", true);
        migrateSettings.addActionListener(e -> MigrateSettingsCheckboxState.isMigrationNeeded = migrateSettings.isSelected());
        MigrateSettingsCheckboxState.isMigrationNeeded = true;
        content.add(migrateSettings);

        JPanel buttons = new JPanel(new FlowLayout(FlowLayout.LEFT, 0, 0));
        content.add(buttons);

        StatusPanelWithProgressBar statusPanel = new StatusPanelWithProgressBar();
        content.add(statusPanel.getContent());

        if (serialPortType == SerialPortType.Ecu) {
            buttons.add(new JLabel("Legacy Device Without OpenBLT"));
        } else if (serialPortType == SerialPortType.EcuWithOpenblt) {
            JButton switchButton = new JButton("Switch to OpenBLT");
            JButton autoButton = new JButton("Auto Update Firmware");

            switchButton.addActionListener(e -> {
                switchButton.setEnabled(false);
                autoButton.setEnabled(false);
                statusPanel.clear();
                OpenBltSwitchJob job = new OpenBltSwitchJob(new PortResult(port, serialPortType), content, uiContext.getLinkManager());
                AsyncJobExecutor.INSTANCE.executeJob(job, statusPanel, () -> SwingUtilities.invokeLater(() -> {
                    switchButton.setEnabled(true);
                    autoButton.setEnabled(true);
                }));
            });
            buttons.add(switchButton);

            autoButton.addActionListener(e -> {
                switchButton.setEnabled(false);
                autoButton.setEnabled(false);
                statusPanel.clear();
                setNonDeviceTabsEnabled(tabbedPane, false);
                if (tabbedPane != null) {
                    tabbedPane.putClientProperty("isUpdating", Boolean.TRUE);
                    tabbedPane.repaint();
                }
                OpenBltAutoJob job = new OpenBltAutoJob(new PortResult(port, serialPortType), content, ConnectivityContext.INSTANCE, uiContext.getLinkManager());
                AsyncJobExecutor.INSTANCE.executeJob(job, statusPanel, () -> SwingUtilities.invokeLater(() -> {
                    switchButton.setEnabled(true);
                    autoButton.setEnabled(true);
                    setNonDeviceTabsEnabled(tabbedPane, true);
                    if (tabbedPane != null) {
                        tabbedPane.putClientProperty("isUpdating", Boolean.FALSE);
                        tabbedPane.repaint();
                    }
                }));
            });
            buttons.add(autoButton);
        }
    }

    private static void setNonDeviceTabsEnabled(JTabbedPane tabbedPane, boolean enabled) {
        if (tabbedPane == null) return;
        for (int i = 0; i < tabbedPane.getTabCount(); i++) {
            if (!"Device".equals(tabbedPane.getTitleAt(i))) {
                tabbedPane.setEnabledAt(i, enabled);
            }
        }
    }

    public Component getContent() {
        return content;
    }
}
