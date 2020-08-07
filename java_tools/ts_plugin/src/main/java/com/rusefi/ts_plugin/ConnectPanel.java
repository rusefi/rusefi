package com.rusefi.ts_plugin;

import com.rusefi.io.ConnectionStateListener;
import com.rusefi.io.LinkManager;

import javax.swing.*;
import java.awt.*;
import java.awt.event.ActionEvent;

/**
 * todo: move IO away from AWT thread
 */
public class ConnectPanel {
    private final JPanel content = new JPanel(new BorderLayout());
    private final JLabel status = new JLabel();

    private LinkManager controllerConnector;

    public ConnectPanel(final ConnectionStateListener connectionStateListener) {
        JPanel flow = new JPanel(new FlowLayout());

        JButton connect = new JButton("Connect");
        JButton disconnect = new JButton("Disconnect");

        disconnect.setEnabled(false);
        disconnect.addActionListener(new AbstractAction() {
            @Override
            public void actionPerformed(ActionEvent e) {
                controllerConnector.close();
                status.setText("Disconnected");
                disconnect.setEnabled(false);
                connect.setEnabled(true);
            }
        });

        connect.addActionListener(e -> {
            connect.setEnabled(false);

            controllerConnector = new LinkManager()
                    .setCompositeLogicEnabled(false)
                    .setNeedPullData(false);

            //controllerConnector.startAndConnect(":2390", ConnectionStateListener.VOID);
            controllerConnector.startAndConnect(":29001", new ConnectionStateListener() {
                public void onConnectionEstablished() {
                    status.setText("Connected to rusEFI");
                    disconnect.setEnabled(true);
                    connectionStateListener.onConnectionEstablished();
                }


                public void onConnectionFailed() {
                }
            });
        });

        flow.add(connect);
        flow.add(disconnect);

        content.add(flow, BorderLayout.NORTH);
        content.add(status, BorderLayout.SOUTH);
    }

    public LinkManager getControllerConnector() {
        return controllerConnector;
    }

    public static String getLastFour(String fileName) {
        int dotIndex = fileName.indexOf(".");
        fileName = fileName.substring(0, dotIndex);
        if (fileName.length() < 5)
            return fileName;
        return fileName.substring(fileName.length() - 4);
    }

    public JComponent getContent() {
        return content;
    }
}
