package com.rusefi;

import com.rusefi.io.LinkManager;
import com.rusefi.io.serial.PortHolder;
import com.rusefi.io.tcp.TcpConnector;
import com.rusefi.maintenance.EraseChip;
import com.rusefi.maintenance.FirmwareFlasher;
import com.rusefi.maintenance.ProcessStatusWindow;
import com.rusefi.ui.util.HorizontalLine;
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
import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;

import static com.rusefi.ui.storage.PersistentConfiguration.getConfig;

/**
 * This frame is used on startup to select the port we would be using
 *
 * @author Andrey Belomutskiy
 *         <p/>
 *         2/14/14
 * @see SimulatorHelper
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
        ImageIcon icon = UiUtils.loadIcon(APPICON);
        if (icon != null)
            frame.setIconImage(icon.getImage());
    }

    public void chooseSerialPort() {
        List<String> ports = new ArrayList<>();
        ports.addAll(Arrays.asList(SerialPortList.getPortNames()));
        ports.addAll(TcpConnector.getAvailablePorts());

        JPanel leftPanel = new JPanel(new VerticalFlowLayout());
        leftPanel.setBorder(BorderFactory.createCompoundBorder(BorderFactory.createEmptyBorder(10, 10, 10, 10),
                BorderFactory.createLineBorder(Color.darkGray)));

        if (!ports.isEmpty()) {
            final JPanel connectPanel = new JPanel(new FlowLayout());
            addPortSelection(ports, connectPanel);
            leftPanel.add(connectPanel);
            leftPanel.add(new HorizontalLine());
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

        leftPanel.add(buttonLogViewer);
        leftPanel.add(new HorizontalLine());

        leftPanel.add(SimulatorHelper.createSimulatorComponent(this));

        if (ProcessStatusWindow.isWindows()) {
            leftPanel.add(new HorizontalLine());
            leftPanel.add(new FirmwareFlasher(FirmwareFlasher.IMAGE_DEBUG_FILE, "Program Firmware/Debug").getButton());
            leftPanel.add(new HorizontalLine());
            leftPanel.add(new FirmwareFlasher(FirmwareFlasher.IMAGE_RELEASE_FILE, "Program Firmware/Release").getButton());
            leftPanel.add(new HorizontalLine());
            leftPanel.add(new EraseChip().getButton());
        }

        JPanel rightPanel = new JPanel(new VerticalFlowLayout());

        ImageIcon logoIcon = UiUtils.loadIcon(LOGO);
        if (logoIcon != null) {
            JLabel logo = new JLabel(logoIcon);
            logo.setBorder(BorderFactory.createEmptyBorder(0, 0, 0, 10));
            URLLabel.addUrlAction(logo, URLLabel.createUri(URI));
            logo.setCursor(new Cursor(Cursor.HAND_CURSOR));
            rightPanel.add(logo);
        }
        rightPanel.add(new URLLabel(LINK_TEXT, URI));

        JPanel content = new JPanel(new BorderLayout());
        content.add(leftPanel, BorderLayout.WEST);
        content.add(rightPanel, BorderLayout.EAST);
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
        String defaultPort =  getConfig().getRoot().getProperty(Launcher.PORT_KEY);
        comboPorts.setSelectedItem(defaultPort);

        final JComboBox<String> comboSpeeds = createSpeedCombo();
//        panel.add(comboSpeeds);

        final JButton connect = new JButton("Connect");
        panel.add(connect);
        connect.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                disposeFrameAndProceed();
                PortHolder.BAUD_RATE = Integer.parseInt((String) comboSpeeds.getSelectedItem());
                new Launcher(comboPorts.getSelectedItem().toString());
            }
        });
    }

    private static JComboBox<String> createSpeedCombo() {
        JComboBox<String> combo = new JComboBox<>();
        int defaultSpeed = 115200;
        for (int speed : new int[]{9600, 14400, 38400, 115200, 460800, 921600})
            combo.addItem(Integer.toString(speed));
        combo.setSelectedItem(Integer.toString(defaultSpeed));
        return combo;
    }

}