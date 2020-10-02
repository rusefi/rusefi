package com.rusefi.ts_plugin;

import com.efiAnalytics.plugin.ecu.ControllerAccess;
import com.rusefi.io.ConnectionStateListener;
import com.rusefi.io.IoStream;

import javax.swing.*;
import java.awt.*;
import java.util.function.Supplier;

public class LocalSdCardReader {
    private final JPanel content = new JPanel(new BorderLayout());

    private final SdCardReaderPanel sdCardReaderPanel;

    public LocalSdCardReader(Supplier<ControllerAccess> controllerAccessSupplier) {
        JPanel topPanel = new JPanel(new BorderLayout());

        ConnectPanel connectPanel = new ConnectPanel(new ConnectionStateListener() {
            public void onConnectionEstablished() {
                sdCardReaderPanel.onConnectionEstablished();
            }

            public void onConnectionFailed() {
            }
        });
        topPanel.add(connectPanel.getContent(), BorderLayout.NORTH);
        sdCardReaderPanel = new SdCardReaderPanel(controllerAccessSupplier, new Supplier<IoStream>() {
            @Override
            public IoStream get() {
                return connectPanel.getControllerConnector().getConnector().getBinaryProtocol().getStream();
            }
        }, content.getParent());

        content.add(topPanel, BorderLayout.NORTH);
        content.add(sdCardReaderPanel.getContent(), BorderLayout.CENTER);

        content.add(new JLabel("<html>This tab allows direct access to SD card<br/>Please be sure to disconnect Tuner Studio from ECU while downloading files using this tab"), BorderLayout.SOUTH);
    }

    public Component getContent() {
        return new JScrollPane(content, JScrollPane.VERTICAL_SCROLLBAR_ALWAYS, JScrollPane.HORIZONTAL_SCROLLBAR_AS_NEEDED);
    }
}
