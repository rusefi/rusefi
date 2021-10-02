package com.rusefi.ui.light;

import com.rusefi.*;
import com.rusefi.autodetect.PortDetector;
import com.rusefi.autoupdate.Autoupdate;
import com.rusefi.core.Sensor;
import com.rusefi.core.SensorCentral;
import com.rusefi.io.ConnectionStateListener;
import com.rusefi.io.ConnectionStatusLogic;
import com.rusefi.io.ConnectionWatchdog;
import com.rusefi.io.LinkManager;
import com.rusefi.ui.UIContext;
import com.rusefi.ui.util.FrameHelper;
import org.putgemin.VerticalFlowLayout;

import javax.swing.*;
import java.awt.*;

import static com.rusefi.StartupFrame.createLogoLabel;

public class LightweightGUI {

    private final UIContext uiContext;
    private FrameHelper frameHelper = new FrameHelper();
    private JPanel content = new JPanel(new BorderLayout());

    private JPanel connectedPanel = new JPanel();
    private JLabel connectedLabel = new JLabel();

    public LightweightGUI(UIContext uiContext) {
        this.uiContext = uiContext;
        frameHelper.getFrame().setTitle("rusEFI Lightweight " + rusEFIVersion.CONSOLE_VERSION);

        JPanel topPanel = new JPanel(new FlowLayout(FlowLayout.RIGHT));

        connectedPanel.add(connectedLabel);

        topPanel.add(connectedPanel);

        topPanel.add(new InternetStatus().getContent());


        JPanel leftPanel = new JPanel(new VerticalFlowLayout());
        leftPanel.add(new JLabel(Autoupdate.readBundleFullName()));


        JLabel firmwareVersion = new JLabel();
        SensorCentral.getInstance().addListener(Sensor.FIRMWARE_VERSION, value -> firmwareVersion.setText(Integer.toString((int) value)));
        leftPanel.add(firmwareVersion);

        content.add(topPanel, BorderLayout.NORTH);
        content.add(leftPanel, BorderLayout.WEST);

        content.add(createLogoUrlPanel(), BorderLayout.EAST);


        frameHelper.showFrame(content, true);
    }

    public static void start() {

        UIContext uiContext = new UIContext();

        LightweightGUI gui = new LightweightGUI(uiContext);

        gui.setConnectedUI(false);


        new Thread(() -> waitForDeviceAndStart(uiContext.getLinkManager())).start();

        ConnectionStatusLogic.INSTANCE.addListener(new ConnectionStatusLogic.Listener() {
            @Override
            public void onConnectionStatus(boolean isConnected) {
                uiContext.sensorLogger.init();
                SwingUtilities.invokeLater(new Runnable() {
                    @Override
                    public void run() {
                        gui.setConnectedUI(isConnected);
                    }
                });
            }
        });
    }

    private static JPanel createLogoUrlPanel() {
        JPanel panel = new JPanel(new BorderLayout());

        panel.add(new JLabel(StartupFrame.LINK_TEXT), BorderLayout.SOUTH);
        JLabel logo = createLogoLabel();
        if (logo != null) {
            panel.add(logo, BorderLayout.CENTER);
        }

        return panel;
    }

    private void setConnectedUI(boolean isConnected) {
        connectedLabel.setText(isConnected ? "Connected" : "NOT CONNECTED");
        connectedPanel.setBackground(isConnected ? Color.green : Color.red);
    }

    public static void waitForDeviceAndStart(LinkManager linkManager) {
        String autoDetectedPort = detectPortUntilDetected();
        System.out.println("First time port detected: " + autoDetectedPort);

        linkManager.startAndConnect(autoDetectedPort, ConnectionStateListener.VOID);

        new ConnectionWatchdog(Timeouts.CONNECTION_RESTART_DELAY, () -> {
            FileLog.MAIN.logLine("ConnectionWatchdog.reconnectTimer restarting: " + Timeouts.CONNECTION_RESTART_DELAY);
            linkManager.restart();
        }).start();
    }

    private static String detectPortUntilDetected() {
        while (true) {
            String port = PortDetector.autoDetectSerial(null).getSerialPort();
            System.out.println("Detected " + port);
            if (port != null)
                return port;
            IoUtil.sleepSeconds(1);
        }
    }
}
