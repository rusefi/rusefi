package com.rusefi.ui.light;

import com.rusefi.*;
import com.rusefi.autodetect.PortDetector;
import com.rusefi.io.ConnectionStateListener;
import com.rusefi.io.ConnectionStatusLogic;
import com.rusefi.io.ConnectionWatchdog;
import com.rusefi.io.LinkManager;
import com.rusefi.sensor_logs.SensorLogger;
import com.rusefi.ui.util.FrameHelper;

import javax.swing.*;
import java.awt.*;

import static com.rusefi.StartupFrame.createLogoLabel;

public class LightweightGUI {

    private FrameHelper frameHelper = new FrameHelper();
    private JPanel content = new JPanel(new BorderLayout());

    private JPanel connectedPanel = new JPanel();
    private JLabel connectedLabel = new JLabel();

    public LightweightGUI() {
        frameHelper.getFrame().setTitle("rusEFI Lightweight " + rusEFIVersion.CONSOLE_VERSION);

        JPanel topPanel = new JPanel(new FlowLayout(FlowLayout.RIGHT));

        connectedPanel.add(connectedLabel);

        topPanel.add(connectedPanel);

        topPanel.add(new InternetStatus().getContent());


        content.add(topPanel, BorderLayout.NORTH);
        content.add(new JLabel(StartupFrame.LINK_TEXT), BorderLayout.CENTER);

        JLabel logo = createLogoLabel();
        if (logo != null) {
            content.add(logo, BorderLayout.EAST);
        }
        frameHelper.showFrame(content, true);
    }

    public static void start() {
        LightweightGUI gui = new LightweightGUI();

        gui.setConnectedUI(false);


        new Thread(() -> waitForDeviceAndStart()).start();

        ConnectionStatusLogic.INSTANCE.addListener(new ConnectionStatusLogic.Listener() {
            @Override
            public void onConnectionStatus(boolean isConnected) {
                SensorLogger.init();
                SwingUtilities.invokeLater(new Runnable() {
                    @Override
                    public void run() {
                        gui.setConnectedUI(isConnected);
                    }
                });
            }
        });

    }

    private void setConnectedUI(boolean isConnected) {
        connectedLabel.setText(isConnected ? "Connected" : "NOT CONNECTED");
        connectedPanel.setBackground(isConnected ? Color.green : Color.red);
    }

    public static void waitForDeviceAndStart() {
        String autoDetectedPort = detectPortUntilDetected();
        System.out.println("First time port detected: " + autoDetectedPort);

        LinkManager.startAndConnect(autoDetectedPort, ConnectionStateListener.VOID);

        new ConnectionWatchdog(Timeouts.CONNECTION_RESTART_DELAY, () -> {
            FileLog.MAIN.logLine("ConnectionWatchdog.reconnectTimer restarting: " + Timeouts.CONNECTION_RESTART_DELAY);
            LinkManager.restart();
        }).start();
    }

    private static String detectPortUntilDetected() {
        while (true) {
            String port = PortDetector.autoDetectSerial(null);
            System.out.println("Detected " + port);
            if (port != null)
                return port;
            IoUtil.sleepSeconds(1);
        }
    }
}
