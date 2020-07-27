package com.rusefi.ts_plugin;

import com.rusefi.auth.AutoTokenUtil;
import com.rusefi.autodetect.PortDetector;
import com.rusefi.autoupdate.AutoupdateUtil;
import com.rusefi.proxy.NetworkConnector;
import com.rusefi.proxy.NetworkConnectorContext;
import com.rusefi.ui.AuthTokenPanel;
import com.rusefi.ui.util.URLLabel;
import org.putgemin.VerticalFlowLayout;

import javax.swing.*;

/**
 * @see PluginEntry
 */
public class BroadcastTab {
    private final JComponent content = new JPanel(new VerticalFlowLayout());

    private final JLabel help = new URLLabel(RemoteTab.HOWTO_REMOTE_TUNING);

    private final JLabel status = new JLabel();

    public BroadcastTab() {
        JButton broadcast = new JButton("Broadcast");

        broadcast.addActionListener(e -> {
            String authToken = AuthTokenPanel.getAuthToken();
            if (!AutoTokenUtil.isToken(authToken)) {
                status.setText("Auth token is required to broadcast ECU");
                return;
            }

            new Thread(() -> {
                String autoDetectedPort = PortDetector.autoDetectSerial(null);
                SwingUtilities.invokeLater(() -> {
                    startBroadcasting(authToken, autoDetectedPort);
                });

            }).start();
        });

        content.add(broadcast);
        content.add(status);
        content.add(help);
        content.add(new JLabel(PluginEntry.LOGO));

        AutoupdateUtil.trueLayout(content);
    }

    private void startBroadcasting(String authToken, String autoDetectedPort) {
        if (autoDetectedPort == null) {
            status.setText("<html>rusEFI ECU not detected.<br/>Please make sure that TunerStudio is currently not connected to ECU.</html>");
        } else {
            status.setText("rusEFI detected at " + autoDetectedPort);

            NetworkConnectorContext connectorContext = new NetworkConnectorContext();

            new Thread(() -> {
                NetworkConnector.NetworkConnectorResult networkConnectorResult = new NetworkConnector().runNetworkConnector(authToken, autoDetectedPort, connectorContext);

                SwingUtilities.invokeLater(() -> status.setText("One time password to connect to this ECU: " + networkConnectorResult.getOneTimeToken()));

            }).start();
        }
        AutoupdateUtil.trueLayout(content);
    }

    public JComponent getContent() {
        return content;
    }
}
