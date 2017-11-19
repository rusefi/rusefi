package com.rusefi;

import com.rusefi.io.LinkManager;
import com.rusefi.io.serial.PortHolder;
import com.rusefi.io.tcp.TcpConnector;
import com.rusefi.maintenance.EraseChip;
import com.rusefi.maintenance.FirmwareFlasher;
import com.rusefi.maintenance.ProcessStatusWindow;
import com.rusefi.ui.OlderDiscoveryChecbbox;
import com.rusefi.ui.util.HorizontalLine;
import com.rusefi.ui.util.URLLabel;
import com.rusefi.ui.util.UiUtils;
import jssc.SerialPortList;
import org.jetbrains.annotations.NotNull;
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
 * @see FirmwareFlasher
 */
public class StartupFrame {
    // todo: figure out a better way to work with absolute path
    private static final String APPICON = "appicon.png";
    private static final String LOGO = "logo.gif";
    private static final String LINK_TEXT = "rusEfi (c) 2012-2017";
    private static final String URI = "http://rusefi.com/?java_console";
    private static final String VCP_DRIVER_TEXT = "vcp driver";
    private static final String VCP_DRIVER_URI = "http://www.st.com/st-web-ui/static/active/en/st_prod_software_internet/resource/technical/software/driver/stsw-stm32102.zip";

    private final JFrame frame;
    private final Timer scanPortsTimes = new Timer(1000, new ActionListener() {
        @Override
        public void actionPerformed(ActionEvent e) {
            findAndApplyPorts();
        }
    });
    private final JPanel connectPanel = new JPanel(new FlowLayout());
    // todo: move this line to the connectPanel
    private final JComboBox<String> comboPorts = new JComboBox<>();
    @NotNull
    private List<String> currentlyDisplayedPorts = new ArrayList<>();
    private boolean isFirstTimeApplyingPorts = true;
    private JPanel leftPanel;

    /**
     * this flag tells us if we are closing the startup frame in order to proceed with console start or if we are
     * closing the application.
     */
    private boolean isProceeding;
    private JLabel noPortsMessage = new JLabel("No ports found!");

    public StartupFrame() {
        frame = new JFrame("rusEfi console v" + Launcher.CONSOLE_VERSION);
        frame.setDefaultCloseOperation(JDialog.DISPOSE_ON_CLOSE);
        frame.addWindowListener(new WindowAdapter() {
            @Override
            public void windowClosed(WindowEvent ev) {
                if (!isProceeding) {
                    getConfig().save();
                    System.exit(0);
                }
            }
        });
        setAppIcon(frame);
        scanPortsTimes.start();
    }

    public static void setAppIcon(JFrame frame) {
        ImageIcon icon = UiUtils.loadIcon(APPICON);
        if (icon != null)
            frame.setIconImage(icon.getImage());
    }

    public void chooseSerialPort() {

        leftPanel = new JPanel(new VerticalFlowLayout());
        leftPanel.setBorder(BorderFactory.createCompoundBorder(BorderFactory.createEmptyBorder(10, 10, 10, 10),
                BorderFactory.createLineBorder(Color.darkGray)));

        connectPanel.add(comboPorts);
        final JComboBox<String> comboSpeeds = createSpeedCombo();
        connectPanel.add(comboSpeeds);

        final JButton connect = new JButton("Connect");
        connectPanel.add(connect);
        connect.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                disposeFrameAndProceed();
                PortHolder.BAUD_RATE = Integer.parseInt((String) comboSpeeds.getSelectedItem());
                new Launcher(comboPorts.getSelectedItem().toString());
            }
        });

        leftPanel.add(connectPanel);
        leftPanel.add(noPortsMessage);
        installMessage(noPortsMessage, "Check you cables. Check your drivers. Do you want to start simulator maybe?");
        leftPanel.add(new URLLabel(VCP_DRIVER_TEXT, VCP_DRIVER_URI));
        leftPanel.add(new HorizontalLine());

        findAndApplyPorts();

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
            leftPanel.add(new OlderDiscoveryChecbbox().getButton());
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

    private void installMessage(JComponent component, String s) {
        component.setToolTipText(s);
    }

    private void findAndApplyPorts() {
        List<String> ports = findAllAvailablePorts();
        if (!currentlyDisplayedPorts.equals(ports) || isFirstTimeApplyingPorts) {
            FileLog.MAIN.logLine("Available ports " + ports);
            isFirstTimeApplyingPorts = false;
            connectPanel.setVisible(!ports.isEmpty());
            noPortsMessage.setVisible(ports.isEmpty());
//        panel.add(comboSpeeds); // todo: finish speed selector UI component
//            horizontalLine.setVisible(!ports.isEmpty());

            addPortSelection(ports);
            currentlyDisplayedPorts = ports;
            UiUtils.trueLayout(connectPanel);
            frame.pack();
        }
    }

    @NotNull
    private List<String> findAllAvailablePorts() {
        List<String> ports = new ArrayList<>();
        ports.addAll(Arrays.asList(SerialPortList.getPortNames()));
        ports.addAll(TcpConnector.getAvailablePorts());
        return ports;
    }

    public void disposeFrameAndProceed() {
        isProceeding = true;
        frame.dispose();
        scanPortsTimes.stop();
    }

    private void addPortSelection(List<String> ports) {
        comboPorts.removeAllItems();
        for (final String port : ports)
            comboPorts.addItem(port);
        String defaultPort = getConfig().getRoot().getProperty(Launcher.PORT_KEY);
        comboPorts.setSelectedItem(defaultPort);
    }

    private static JComboBox<String> createSpeedCombo() {
        JComboBox<String> combo = new JComboBox<>();
        String defaultSpeed = getConfig().getRoot().getProperty(Launcher.SPEED_KEY, "115200");
        for (int speed : new int[]{9600, 14400, 19200, 38400, 57600, 115200, 460800, 921600})
            combo.addItem(Integer.toString(speed));
        combo.setSelectedItem(defaultSpeed);
        return combo;
    }
}