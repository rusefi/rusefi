package com.rusefi;

import com.devexperts.logging.Logging;
import com.rusefi.core.io.BundleUtil;
import com.rusefi.core.preferences.storage.PersistentConfiguration;
import com.rusefi.core.ui.FrameHelper;
import com.rusefi.io.LinkManager;
import com.rusefi.io.serial.BaudRateHolder;
import com.rusefi.maintenance.DriverInstall;
import com.rusefi.maintenance.MaintenanceUtil;
import com.rusefi.maintenance.StLinkFlasher;
import com.rusefi.maintenance.ProgramSelector;
import com.rusefi.ui.LogoHelper;
import com.rusefi.ui.util.HorizontalLine;
import com.rusefi.ui.util.URLLabel;
import com.rusefi.ui.util.UiUtils;
import com.rusefi.ui.widgets.ToolButtons;
import com.rusefi.util.IoUtils;
import net.miginfocom.swing.MigLayout;
import org.jetbrains.annotations.NotNull;
import org.putgemin.VerticalFlowLayout;

import javax.swing.*;
import javax.swing.border.TitledBorder;
import java.awt.*;
import java.awt.datatransfer.StringSelection;
import java.awt.event.*;
import java.util.Date;
import java.util.List;

import static com.devexperts.logging.Logging.getLogging;
import static com.rusefi.core.preferences.storage.PersistentConfiguration.getConfig;
import static com.rusefi.ui.util.UiUtils.*;
import static javax.swing.JOptionPane.YES_NO_OPTION;

/**
 * This frame is used on startup to select the port we would be using
 *
 * @author Andrey Belomutskiy
 * <p/>
 * 2/14/14
 * @see SimulatorHelper
 * @see StLinkFlasher
 */
public class StartupFrame {
    private static final Logging log = getLogging(Launcher.class);
    public static final String ALWAYS_AUTO_PORT = "always_auto_port";
    private static final String NO_PORTS_FOUND = "<html>No ports found!<br>Confirm blue LED is blinking</html>";

    private final JFrame frame;
    private final JPanel connectPanel = new JPanel(new FlowLayout());
    // todo: move this line to the connectPanel
    private final PortsComboBox portsComboBox = new PortsComboBox();
    private final JPanel leftPanel = new JPanel(new VerticalFlowLayout());

    private final JPanel realHardwarePanel = new JPanel(new MigLayout());
    private final JPanel miscPanel = new JPanel(new MigLayout()) {
        @Override
        public Dimension getPreferredSize() {
            // want miscPanel and realHardwarePanel to be the same width
            Dimension size = super.getPreferredSize();
            return new Dimension(Math.max(size.width, realHardwarePanel.getPreferredSize().width), size.height);
        }
    };
    /**
     * this flag tells us if we are closing the startup frame in order to proceed with console start or if we are
     * closing the application.
     */
    private boolean isProceeding;
    private final JLabel noPortsMessage = new JLabel("Scanning ports...");

    public StartupFrame() {
        String title = "rusEFI console " + Launcher.CONSOLE_VERSION;
        log.info(title);
        noPortsMessage.setForeground(Color.red);
        frame = FrameHelper.createFrame(title).getFrame();
        frame.addWindowListener(new WindowAdapter() {
            @Override
            public void windowClosed(WindowEvent ev) {
                if (!isProceeding) {
                    getConfig().save();
                    IoUtils.exit("windowClosed", 0);
                }
            }
        });
    }

    public void showUi() {
        realHardwarePanel.setBorder(new TitledBorder(BorderFactory.createLineBorder(Color.darkGray), "Real stm32"));
        miscPanel.setBorder(new TitledBorder(BorderFactory.createLineBorder(Color.darkGray), "Miscellaneous"));

        connectPanel.add(portsComboBox.getComboPorts());
        final JComboBox<String> comboSpeeds = createSpeedCombo();
        comboSpeeds.setToolTipText("For 'STMicroelectronics Virtual COM Port' device any speed setting would work the same");
        connectPanel.add(comboSpeeds);

        final JButton connectButton = new JButton("Connect", new ImageIcon(getClass().getResource("/com/rusefi/connect48.png")));
        setToolTip(connectButton, "Connect to real hardware");

        JCheckBoxMenuItem menuItem = new JCheckBoxMenuItem("Always auto-connect port");
        menuItem.setState(PersistentConfiguration.getBoolProperty(ALWAYS_AUTO_PORT));
        menuItem.addActionListener(e -> PersistentConfiguration.setBoolProperty(ALWAYS_AUTO_PORT, menuItem.getState()));

        connectButton.addMouseListener(new MouseAdapter() {
            @Override
            public void mouseReleased(MouseEvent e) {
                if (!SwingUtilities.isRightMouseButton(e))
                    return;
                JPopupMenu menu = new JPopupMenu();
                menu.add(menuItem);
                menu.show(connectButton, e.getX(), e.getY());
            }
        });

        connectPanel.add(connectButton);
        connectPanel.setVisible(false);

        frame.getRootPane().setDefaultButton(connectButton);
        connectButton.addKeyListener(new KeyAdapter() {
            @Override
            public void keyPressed(KeyEvent e) {
                if (e.getKeyCode() == KeyEvent.VK_ENTER) {
                    connectButtonAction(comboSpeeds);
                }
            }
        });

        connectButton.addActionListener(e -> connectButtonAction(comboSpeeds));

        leftPanel.add(realHardwarePanel);
        leftPanel.add(miscPanel);

        if (FileLog.isWindows()) {
            JPanel topButtons = new JPanel(new FlowLayout(FlowLayout.CENTER, 5, 0));
            topButtons.add(ToolButtons.createShowDeviceManagerButton());
            topButtons.add(DriverInstall.createButton());
            topButtons.add(ToolButtons.createPcanConnectorButton());
            realHardwarePanel.add(topButtons, "right, wrap");
        }
        realHardwarePanel.add(connectPanel, "right, wrap");
        realHardwarePanel.add(noPortsMessage, "right, wrap");
        noPortsMessage.setToolTipText("Check you cables. Check your drivers. Do you want to start simulator maybe?");

        ProgramSelector selector = new ProgramSelector(portsComboBox.getComboPorts());

        if (FileLog.isWindows()) {
            realHardwarePanel.add(new HorizontalLine(), "right, wrap");

            realHardwarePanel.add(selector.getControl(), "right, wrap");

            // for F7 builds we just build one file at the moment
//            realHardwarePanel.add(new FirmwareFlasher(FirmwareFlasher.IMAGE_FILE, "ST-LINK Program Firmware", "Default firmware version for most users").getButton());
            JComponent updateHelp = ProgramSelector.createHelpButton();

            JLabel comp = binaryModificationControl();
            realHardwarePanel.add(comp, "right, wrap");
            realHardwarePanel.add(updateHelp, "right, wrap");

            // st-link is pretty advanced use-case, real humans do not have st-link as of 2021
            //realHardwarePanel.add(new EraseChip().getButton(), "right, wrap");
        }

        SerialPortScanner.INSTANCE.addListener(currentHardware -> SwingUtilities.invokeLater(() -> {
            selector.apply(currentHardware);
            applyKnownPorts(currentHardware);
            frame.pack();
        }));

        final JButton buttonLogViewer = new JButton();
        buttonLogViewer.setText("Start " + LinkManager.LOG_VIEWER);
        buttonLogViewer.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                disposeFrameAndProceed();
                new ConsoleUI(LinkManager.LOG_VIEWER);
            }
        });

        miscPanel.add(buttonLogViewer, "wrap");
        miscPanel.add(new HorizontalLine(), "wrap");

        miscPanel.add(SimulatorHelper.createSimulatorComponent(this));

        JPanel rightPanel = new JPanel(new VerticalFlowLayout());

        if (BundleUtil.readBundleFullNameNotNull().contains("proteus_f7")) {
            String text = "WARNING: Proteus F7";
            URLLabel urlLabel = new URLLabel(text, "https://github.com/rusefi/rusefi/wiki/F7-requires-full-erase");
            Color originalColor = urlLabel.getForeground();
            new Timer(500, new ActionListener() {
                int counter;
                @Override
                public void actionPerformed(ActionEvent e) {
                    // URL color is hard-coded, let's blink isUnderlined attribute as second best option
                    urlLabel.setText(text, counter++ % 2 == 0);
                }
            }).start();
            rightPanel.add(urlLabel);
        }

        JLabel logo = LogoHelper.createLogoLabel();
        if (logo != null)
            rightPanel.add(logo);
        rightPanel.add(LogoHelper.createUrlLabel());
        rightPanel.add(new JLabel("Console " + Launcher.CONSOLE_VERSION));

        JPanel content = new JPanel(new BorderLayout());
        content.add(leftPanel, BorderLayout.WEST);
        content.add(rightPanel, BorderLayout.EAST);
        frame.add(content);
        frame.pack();
        setFrameIcon(frame);
        log.info("setVisible");
        frame.setVisible(true);
        UiUtils.centerWindow(frame);

        KeyListener hwTestEasterEgg = functionalTestEasterEgg();

        for (Component component : getAllComponents(frame)) {
            component.addKeyListener(hwTestEasterEgg);
        }
    }

    private static @NotNull JLabel binaryModificationControl() {
        long binaryModificationTimestamp = MaintenanceUtil.getBinaryModificationTimestamp();
        String fileTimestampText = binaryModificationTimestamp == 0 ? "firmware file not found" : ("Files " + new Date(binaryModificationTimestamp).toString());
        JLabel jLabel = new JLabel(fileTimestampText);
        jLabel.setToolTipText("Click to copy");
        jLabel.addMouseListener(new MouseAdapter() {
            @Override
            public void mouseClicked(MouseEvent e) {
                Toolkit.getDefaultToolkit().getSystemClipboard()
                    .setContents(new StringSelection(fileTimestampText), null);
            }
        });
        return jLabel;
    }

    private void applyKnownPorts(SerialPortScanner.AvailableHardware currentHardware) {
        List<SerialPortScanner.PortResult> ports = currentHardware.getKnownPorts();
        log.info("Rendering available ports: " + ports);
        connectPanel.setVisible(!ports.isEmpty());


        boolean hasEcuOrBootloader = applyPortSelectionToUIcontrol(portsComboBox.getComboPorts(), ports);
        if (ports.isEmpty()) {
            noPortsMessage.setText(NO_PORTS_FOUND);
        } else {
            noPortsMessage.setText("Make sure you are disconnected from TunerStudio");
        }

        noPortsMessage.setVisible(ports.isEmpty() || !hasEcuOrBootloader);


        UiUtils.trueLayout(connectPanel);
    }

    public static void setFrameIcon(Frame frame) {
        ImageIcon icon = LogoHelper.getBundleIcon();
        if (icon != null)
            frame.setIconImage(icon.getImage());
    }

    private void connectButtonAction(JComboBox<String> comboSpeeds) {
        BaudRateHolder.INSTANCE.baudRate = Integer.parseInt((String) comboSpeeds.getSelectedItem());
        SerialPortScanner.PortResult selectedPort = ((SerialPortScanner.PortResult)portsComboBox.getComboPorts().getSelectedItem());
        disposeFrameAndProceed();
        new ConsoleUI(selectedPort.port);
    }

    /**
     * Here we listen to keystrokes while console start-up frame is being displayed and if magic "test" word is typed
     * we launch a functional test on real hardware, same as Jenkins runs within continuous integration
     */
    @NotNull
    private KeyListener functionalTestEasterEgg() {
        return new KeyAdapter() {
            private final static String TEST = "test";
            private String recentKeyStrokes = "";

            @Override
            public void keyTyped(KeyEvent e) {
                recentKeyStrokes = recentKeyStrokes + e.getKeyChar();
                if (recentKeyStrokes.toLowerCase().endsWith(TEST) && showTestConfirmation()) {
                    runFunctionalHardwareTest();
                }
            }

            private boolean showTestConfirmation() {
                return JOptionPane.showConfirmDialog(StartupFrame.this.frame, "Want to run functional test? This would freeze UI for the duration of the test",
                        "Better do not run while connected to vehicle!!!", YES_NO_OPTION) == JOptionPane.YES_OPTION;
            }

            private void runFunctionalHardwareTest() {
                boolean isSuccess = HwCiF4Discovery.runHardwareTest();
                JOptionPane.showMessageDialog(null, "Function test passed: " + isSuccess + "\nSee log folder for details.");
            }
        };
    }

    public void disposeFrameAndProceed() {
        isProceeding = true;
        frame.dispose();
        SerialPortScanner.INSTANCE.stopTimer();
    }

    private static boolean applyPortSelectionToUIcontrol(JComboBox<SerialPortScanner.PortResult> comboPorts, List<SerialPortScanner.PortResult> ports) {
        comboPorts.removeAllItems();
        boolean hasEcuOrBootloader = false;
        for (final SerialPortScanner.PortResult port : ports) {
            comboPorts.addItem(port);
            if (port.type == SerialPortScanner.SerialPortType.Ecu ||
                port.type == SerialPortScanner.SerialPortType.EcuWithOpenblt ||
                port.type == SerialPortScanner.SerialPortType.OpenBlt) {
                hasEcuOrBootloader = true;
            }
        }
        String defaultPort = getConfig().getRoot().getProperty(ConsoleUI.PORT_KEY);
        if (!PersistentConfiguration.getBoolProperty(ALWAYS_AUTO_PORT)) {
            comboPorts.setSelectedItem(defaultPort);
        }

        trueLayout(comboPorts);
        return hasEcuOrBootloader;
    }

    private static JComboBox<String> createSpeedCombo() {
        JComboBox<String> combo = new JComboBox<>();
        String defaultSpeed = getConfig().getRoot().getProperty(ConsoleUI.SPEED_KEY, "115200");
        for (int speed : new int[]{9600, 14400, 19200, 38400, 57600, 115200, 460800, 921600})
            combo.addItem(Integer.toString(speed));
        combo.setSelectedItem(defaultSpeed);
        return combo;
    }
}
