package com.rusefi;

import com.rusefi.io.LinkManager;
import com.rusefi.io.tcp.TcpConnector;
import com.rusefi.maintenance.FirmwareFlasher;
import com.rusefi.ui.util.UiUtils;
import com.rusefi.ui.util.URLLabel;
import jssc.SerialPortList;
import org.putgemin.VerticalFlowLayout;

import javax.swing.*;
import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.WindowAdapter;
import java.awt.event.WindowEvent;
import java.net.URL;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;

/**
 * This frame is used on startup to select the port we would be using
 *
 * @author Andrey Belomutskiy
 *         <p/>
 *         2/14/14
 */
public class StartupFrame {
    // todo: figure out a better way to work with absolute path
    private static final String APPICON = "appicon.png";
    private static final String LOGO = "logo.gif";
    private static final String LINK_TEXT = "rusEfi (c) 2012-2015";
    private static final String URI = "http://rusefi.com/?java_console";

    private final JFrame frame;
    private boolean isProceeding;

    public StartupFrame() {
        frame = new JFrame(Launcher.CONSOLE_VERSION + ": Serial port selection");
        frame.setDefaultCloseOperation(JDialog.DISPOSE_ON_CLOSE);
        frame.addWindowListener(new WindowAdapter() {
            @Override
            public void windowClosed(WindowEvent ev) {
                if (!isProceeding)
                    System.exit(0);
            }
        });
        setAppIcon(frame);
    }

    public static void setAppIcon(JFrame frame) {
        ImageIcon icon = loadIcon(APPICON);
        if (icon != null)
            frame.setIconImage(icon.getImage());
    }

    public void chooseSerialPort() {
        List<String> ports = new ArrayList<>();
        ports.addAll(Arrays.asList(SerialPortList.getPortNames()));
        ports.addAll(TcpConnector.getAvailablePorts());

        JPanel startupOptions = new JPanel(new VerticalFlowLayout());
        startupOptions.setBorder(BorderFactory.createCompoundBorder(BorderFactory.createEmptyBorder(10, 10, 10, 10),
                BorderFactory.createLineBorder(Color.darkGray)));

        if (!ports.isEmpty()) {
            final JPanel connectPanel = new JPanel(new FlowLayout());
            addPortSelection(ports, connectPanel);
            startupOptions.add(connectPanel);
            startupOptions.add(new HorizontalLine());
        }

        final JButton buttonLogViewer = new JButton();
        buttonLogViewer.setText("Start " + LinkManager.LOG_VIEWER);
        buttonLogViewer.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                disposeFrameAndProceed();
                new Launcher(LinkManager.LOG_VIEWER);
            }
        });

        startupOptions.add(buttonLogViewer);
        startupOptions.add(new HorizontalLine());

        startupOptions.add(SimulatorHelper.createSimulatorComponent(this));

        if (FirmwareFlasher.isWindows()) {
            startupOptions.add(new HorizontalLine());
            startupOptions.add(FirmwareFlasher.getContent());
        }

        startupOptions.add(new HorizontalLine());
        startupOptions.add(new URLLabel(LINK_TEXT, URI));

        JPanel content = new JPanel(new BorderLayout());
        content.add(startupOptions, BorderLayout.WEST);

        ImageIcon logoIcon = loadIcon(LOGO);
        if (logoIcon != null) {
            JLabel logo = new JLabel(logoIcon);
            logo.setBorder(BorderFactory.createEmptyBorder(0, 0, 0, 10));
            content.add(logo, BorderLayout.EAST);
        }

        frame.add(content);
        frame.pack();
        frame.setVisible(true);
        UiUtils.centerWindow(frame);
    }

    public void disposeFrameAndProceed() {
        isProceeding = true;
        frame.dispose();
    }

    private void addPortSelection(List<String> ports, JPanel panel) {
        final JComboBox<String> comboPorts = new JComboBox<>();
        for (final String port : ports)
            comboPorts.addItem(port);
        panel.add(comboPorts);

        final JButton connect = new JButton("Connect");
        panel.add(connect);
        connect.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                disposeFrameAndProceed();
                new Launcher(comboPorts.getSelectedItem().toString());
            }
        });
    }

    public static ImageIcon loadIcon(String strPath) {
        URL imgURL = StartupFrame.class.getResource(strPath);
        if (imgURL != null)
            return new ImageIcon(imgURL);
        else
            return null;
    }
}