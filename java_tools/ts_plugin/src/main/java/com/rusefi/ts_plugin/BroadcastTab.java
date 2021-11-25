package com.rusefi.ts_plugin;

import com.rusefi.auth.AuthTokenUtil;
import com.rusefi.autodetect.PortDetector;
import com.rusefi.autoupdate.AutoupdateUtil;
import com.rusefi.proxy.NetworkConnector;
import com.rusefi.proxy.NetworkConnectorContext;
import com.rusefi.tools.VehicleToken;
import com.rusefi.ui.AuthTokenPanel;
import com.rusefi.ui.util.URLLabel;
import org.putgemin.VerticalFlowLayout;

import javax.swing.*;
import java.awt.*;
import java.awt.datatransfer.Clipboard;
import java.awt.datatransfer.StringSelection;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

/**
 * @see PluginEntry
 */
public class BroadcastTab {
    private final JComponent content = new JPanel(new VerticalFlowLayout());

    private final JLabel status = new JLabel();
    private final JButton disconnect = new JButton("Stop Broadcasting");
    private NetworkConnector networkConnector;

    public BroadcastTab() {
        JButton broadcast = new JButton("Broadcast");
        disconnect.setEnabled(false);

        broadcast.addActionListener(e -> {
            String authToken = AuthTokenPanel.getAuthToken();
            if (!AuthTokenUtil.isToken(authToken)) {
                status.setText("Auth token is required to broadcast ECU");
                return;
            }

            new Thread(() -> {
                String autoDetectedPort = PortDetector.autoDetectSerial(null).getSerialPort();
                SwingUtilities.invokeLater(() -> {
                    startBroadcasting(authToken, autoDetectedPort);
                });

            }).start();
        });

        disconnect.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                networkConnector.close();
                disconnect.setEnabled(false);
            }
        });

        content.add(createVehicleTokenPanel());

        content.add(broadcast);
        content.add(status);
        content.add(disconnect);
        content.add(new URLLabel(RemoteTab.HOWTO_REMOTE_TUNING));
        content.add(new JLabel(PluginEntry.LOGO));

        AutoupdateUtil.trueLayout(content);
    }

    private Component createVehicleTokenPanel() {
        JLabel label = new JLabel();
        updateVehicleTokenLabel(label);

        JButton copy = new JButton("Copy to clipboard");
        copy.addActionListener(new AbstractAction() {
            @Override
            public void actionPerformed(ActionEvent e) {
                StringSelection stringSelection = new StringSelection("" + VehicleToken.getOrCreate());
                Clipboard clipboard = Toolkit.getDefaultToolkit().getSystemClipboard();
                clipboard.setContents(stringSelection, null);
            }
        });

        JButton reset = new JButton("Reset");
        reset.addActionListener(new AbstractAction() {
            @Override
            public void actionPerformed(ActionEvent e) {
                VehicleToken.refresh();
                updateVehicleTokenLabel(label);
            }
        });

        JPanel panel = new JPanel(new FlowLayout());
        panel.add(label);
        panel.add(copy);
        panel.add(reset);
        return panel;
    }

    private void updateVehicleTokenLabel(JLabel label) {
        label.setText("Vehicle access token: " + VehicleToken.getOrCreate());
    }

    private void startBroadcasting(String authToken, String autoDetectedPort) {
        if (autoDetectedPort == null) {
            status.setText("<html>rusEFI ECU not detected.<br/>Please make sure that TunerStudio is currently not connected to ECU.</html>");
        } else {
            status.setText("rusEFI detected at " + autoDetectedPort);
            disconnect.setEnabled(true);

            NetworkConnectorContext connectorContext = new NetworkConnectorContext();

            new Thread(() -> {
                networkConnector = new NetworkConnector();

                NetworkConnector.NetworkConnectorResult networkConnectorResult = networkConnector.start(NetworkConnector.Implementation.Plugin, authToken, autoDetectedPort, connectorContext);

                SwingUtilities.invokeLater(() -> status.setText("One time password to connect to this ECU: " + networkConnectorResult.getOneTimeToken()));

            }).start();
        }
        AutoupdateUtil.trueLayout(content);
    }

    public JComponent getContent() {
        return content;
    }
}
