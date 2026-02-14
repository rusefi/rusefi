package com.rusefi;

import com.devexperts.logging.Logging;
import com.opensr5.ini.PrimeTunerStudioCache;
import com.rusefi.core.net.ConnectionAndMeta;
import com.rusefi.core.preferences.storage.PersistentConfiguration;
import com.rusefi.core.ui.AutoupdateUtil;
import com.rusefi.core.ui.FrameHelper;
import com.rusefi.maintenance.*;
import com.rusefi.tools.TunerStudioHelper;
import com.rusefi.ui.BasicLogoHelper;
import com.rusefi.ui.LogoHelper;
import com.rusefi.ui.duplicates.ConsoleBundleUtil;
import com.rusefi.ui.util.HorizontalLine;
import com.rusefi.ui.util.URLLabel;
import com.rusefi.ui.util.UiUtils;
import com.rusefi.ui.widgets.ToolButtons;
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
import java.util.Optional;
import java.util.function.Predicate;
import java.util.function.Supplier;
import java.util.stream.Collectors;

import static com.devexperts.logging.Logging.getLogging;
import static com.rusefi.SerialPortType.EcuWithOpenblt;
import static com.rusefi.SerialPortType.OpenBlt;
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
 * @see ProgramSelector
 */
public class StartupFrame {
    private static final Logging log = getLogging(Launcher.class);

    public static final String ALWAYS_AUTO_PORT = "always_auto_port";
    public static final String AUTO_CLOSE_TS = "auto_close_ts";
    public static final String CHECK_TS_RUNNING = "check_ts_running";
    private static final String NO_PORTS_FOUND = "<html>No ports found!<br>Confirm blue LED is blinking</html>";
    public static final String SCANNING_PORTS = "Scanning ports";

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
    private final ConnectivityContext connectivityContext;
    /**
     * this flag tells us if we are closing the startup frame in order to proceed with console start or if we are
     * closing the application.
     */
    private boolean isProceeding;
    private final JLabel noPortsMessage = new JLabel();
    private final StatusAnimation status;
    private final JButton connectButton = new JButton("Connect", new ImageIcon(getClass().getResource("/com/rusefi/connect48.png")));
    private ProgramSelector selector;
    private boolean firstTimeHasEcuWithOpenBlt = true;
    private boolean firstTimeAutoConnect = true;

    public StartupFrame(ConnectivityContext connectivityContext) {
        this.connectivityContext = connectivityContext;
        String title = UiProperties.getWhiteLabel() + " console " + Launcher.CONSOLE_VERSION;
        log.info(title);
        noPortsMessage.setForeground(Color.red);
        status = new StatusAnimation(new StatusAnimation.StatusConsumer() {
            @Override
            public void onStatus(String niceStatus) {
                noPortsMessage.setText(niceStatus);
            }
        }, SCANNING_PORTS);

        frame = FrameHelper.createFrame(title).getFrame();
        frame.addWindowListener(new WindowAdapter() {
            @Override
            public void windowClosed(WindowEvent ev) {
                if (!isProceeding) {
                    getConfig().save();
                    log.info("Configuration saved.");
                }
            }
        });
        new NamedThreadFactory("ecuDef primer").newThread(PrimeTunerStudioCache::primeWithLocalFile).start();
    }

    public void showUi() {
        miscPanel.setBorder(new TitledBorder(BorderFactory.createLineBorder(Color.darkGray), "Miscellaneous"));

        connectPanel.add(portsComboBox.getComboPorts());
/*
    make baud rate selection much less visible #9103
        final JComboBox<String> comboSpeeds = createSpeedCombo();
        comboSpeeds.setToolTipText("For 'STMicroelectronics Virtual COM Port' device any speed setting would work the same");
        connectPanel.add(comboSpeeds);
*/
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

        portsComboBox.getComboPorts().addActionListener(e -> updateConnectButtonState());

        frame.getRootPane().setDefaultButton(connectButton);
        connectButton.addKeyListener(new KeyAdapter() {
            @Override
            public void keyPressed(KeyEvent e) {
                if (e.getKeyCode() == KeyEvent.VK_ENTER) {
                    connectButtonAction();
                }
            }
        });

        connectButton.addActionListener(e -> connectButtonAction());

        final Optional<JPanel> newReleaseNotification = newReleaseAnnounce(
            "rusefi_autoupdate.exe",
            "left",
            () -> realHardwarePanel.getPreferredSize().width
        );
        if (newReleaseNotification.isPresent()) {
            leftPanel.add(newReleaseNotification.get());
        }
        leftPanel.add(realHardwarePanel);
        if (UiProperties.useSimulator()) {
            leftPanel.add(miscPanel);
        }

        if (FileLog.isWindows()) {
            JPanel topButtons = new JPanel(new FlowLayout(FlowLayout.CENTER, 5, 0));
            topButtons.add(ToolButtons.createShowDeviceManagerButton());
            if (DriverInstall.isFolderExist())
                topButtons.add(DriverInstall.createButton());
            if (UiProperties.usePCAN()) {
                topButtons.add(ToolButtons.createPcanConnectorButton());
            }
            realHardwarePanel.add(topButtons, "right, wrap");
        }
        realHardwarePanel.add(connectPanel, "right, wrap");
        realHardwarePanel.add(noPortsMessage, "right, wrap");
        noPortsMessage.setToolTipText("Check you cables. Check your drivers. Do you want to start simulator maybe?");

        selector = new ProgramSelector(connectivityContext, portsComboBox.getComboPorts());

        realHardwarePanel.add(new HorizontalLine(), "right, wrap");
        realHardwarePanel.add(selector.getControl(), "right, wrap");

        if (FileLog.isWindows()) {

            // for F7 builds we just build one file at the moment
//            realHardwarePanel.add(new FirmwareFlasher(FirmwareFlasher.IMAGE_FILE, "ST-LINK Program Firmware", "Default firmware version for most users").getButton());
            JComponent updateHelp = ProgramSelector.createHelpButton();

            JLabel comp = binaryModificationControl();
            realHardwarePanel.add(comp, "right, wrap");
            realHardwarePanel.add(updateHelp, "right, wrap");

            // st-link is pretty advanced use-case, real humans do not have st-link as of 2021
            //realHardwarePanel.add(new EraseChip().getButton(), "right, wrap");
        }

        connectivityContext.getSerialPortScanner().addListener(currentHardware -> SwingUtilities.invokeLater(() -> {
            status.stop();
            selector.apply(currentHardware);
            applyKnownPorts(currentHardware);
            frame.pack();
        }));

        /*
        LOG_VIEWER is a bit dead, is not it?
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
*/
        miscPanel.add(SimulatorHelper.createSimulatorComponent(this));

        JPanel rightPanel = new JPanel(new VerticalFlowLayout());

        if (ConsoleBundleUtil.readBundleFullNameNotNull().getTarget().contains("proteus_f7")) {
            String text = "WARNING: Proteus F7";
            URLLabel urlLabel = new URLLabel(text, "https://wiki.rusefi.com/F7-requires-full-erase");
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
        if (ConnectionAndMeta.isDefaultWhitelabel(UiProperties.getWhiteLabel()))
            rightPanel.add(LogoHelper.createUrlLabel());
        rightPanel.add(new JLabel("Console " + Launcher.CONSOLE_VERSION));

        JPanel content = new JPanel(new BorderLayout());
        content.add(leftPanel, BorderLayout.WEST);
        content.add(rightPanel, BorderLayout.EAST);

        TunerStudioHelper.checkTunerStudio(frame.getContentPane(), () -> restoreContent(content));

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

    public static @NotNull Optional<JPanel> newReleaseAnnounce(
        final String upgradeExeFileName,
        final String textAlign,
        final Supplier<Integer> minWidthSupplier
    ) {
        final String nextBranchName = ConsoleBundleUtil.readBundleFullNameNotNull().getNextBranchName();
        if (nextBranchName != null && nextBranchName.trim().length() > 0) {
            final JLabel newReleaseAmmomceMessage = new JLabel(
                String.format(
                    "<html><p style=\"text-align: %s;font-weight: bold;color:red\">New release `%s` is available!<br/>To upgrade please restart `%s`.</p></html>",
                    textAlign,
                    nextBranchName,
                    upgradeExeFileName
                )
            );
            final JPanel newReleaseAnnouncePanel = new JPanel(new MigLayout()) {
                @Override
                public Dimension getPreferredSize() {
                    Dimension size = super.getPreferredSize();
                    return new Dimension(Math.max(size.width, minWidthSupplier.get()), size.height);
                }
            };
            newReleaseAnnouncePanel.setBorder(new TitledBorder(
                BorderFactory.createLineBorder(Color.darkGray),
                ""
            ));
            newReleaseAnnouncePanel.add(newReleaseAmmomceMessage);
            return Optional.of(newReleaseAnnouncePanel);
        }
        return Optional.empty();
    }

    public static @NotNull JLabel binaryModificationControl() {
        final long binaryModificationTimestamp = MaintenanceUtil.getBinaryModificationTimestamp();
        JLabel jLabel;
        if (binaryModificationTimestamp == 0) {
            jLabel = new JLabel("firmware file not found");
            jLabel.setForeground(Color.red);
        } else {
            final Date binaryModificationDate = new Date(binaryModificationTimestamp);
            final String branchNameToDisplay = ConsoleBundleUtil.readBundleFullNameNotNull().getBranchName();
            jLabel = new JLabel(String.format(
                "<html><center>%s files<br/>%s</center></html>",
                branchNameToDisplay,
                binaryModificationDate
            ));
            jLabel.setToolTipText("Click to copy");
            jLabel.addMouseListener(new MouseAdapter() {
                @Override
                public void mouseClicked(MouseEvent e) {
                    Toolkit.getDefaultToolkit().getSystemClipboard()
                        .setContents(
                            new StringSelection(String.format(
                                "%s files\r%s",
                                branchNameToDisplay,
                                binaryModificationDate
                            )),
                            null
                        );
                }
            });
        }

        return jLabel;
    }

    private void restoreContent(JPanel content) {
        frame.getContentPane().removeAll();
        frame.add(content);
        AutoupdateUtil.pack(frame);
    }

    private void updateConnectButtonState() {
        PortResult selectedItem = (PortResult) portsComboBox.getComboPorts().getSelectedItem();
        connectButton.setEnabled(selectedItem != null && selectedItem.type != OpenBlt);
    }

    private void applyKnownPorts(AvailableHardware currentHardware) {
        List<PortResult> ports = currentHardware.getKnownPorts();
/*
todo: enable auto-connect once we have 'Device' tab
        List<PortResult> ecuPorts = ports.stream().filter(portResult -> portResult.type == EcuWithOpenblt || portResult.type == SerialPortType.Ecu).collect(Collectors.toList());
        if (!ecuPorts.isEmpty() && firstTimeAutoConnect) {
            firstTimeAutoConnect = false;
            PortResult target = ecuPorts.get(0);
            log.info("Ecu detected, connecting automatically: " + target);
            // combo box selection is already updated by applyPortSelectionToUIcontrol
            connect(target);
            return;
        }
*/
        log.info("Rendering available ports: " + ports);
        connectPanel.setVisible(!ports.isEmpty());


        boolean hasEcuOrBootloader = applyPortSelectionToUIcontrol(portsComboBox.getComboPorts(), ports);
        if (ports.isEmpty()) {
            noPortsMessage.setText(NO_PORTS_FOUND);
        } else {
            noPortsMessage.setText("Make sure you are disconnected from TunerStudio");
        }

        updateConnectButtonState();

        noPortsMessage.setVisible(ports.isEmpty() || !hasEcuOrBootloader);

        boolean hasEcuWithOpenBlt = !ports.stream().filter(portResult -> portResult.type == EcuWithOpenblt).collect(Collectors.toList()).isEmpty();
        if (hasEcuWithOpenBlt && firstTimeHasEcuWithOpenBlt) {
            selector.setMode(UpdateMode.OPENBLT_AUTO);
            firstTimeHasEcuWithOpenBlt = false;
        }

        AutoupdateUtil.trueLayoutAndRepaint(connectPanel);
    }

    public static void setFrameIcon(Frame frame) {
        ImageIcon icon = LogoHelper.getBundleSpecificIcon();
        BasicLogoHelper.setFrameIcon(frame, icon);
    }

    private void connectButtonAction() {
/*
        make baud rate combo box much less visible #9103
        BaudRateHolder.INSTANCE.baudRate = Integer.parseInt((String) comboSpeeds.getSelectedItem());
*/
        PortResult selectedPort = ((PortResult)portsComboBox.getComboPorts().getSelectedItem());
        connect(selectedPort);
    }

    private void connect(PortResult selectedPort) {
        disposeFrameAndProceed();
        new ConsoleUI(selectedPort.port, selectedPort.type);
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
        status.stop();
        connectivityContext.getSerialPortScanner().stopTimer();
    }

    private static boolean applyPortSelectionToUIcontrol(JComboBox<PortResult> comboPorts, List<PortResult> ports) {
        comboPorts.removeAllItems();
        boolean hasEcuOrBootloader = false;
        for (final PortResult port : ports) {
            comboPorts.addItem(port);
            if (port.type == SerialPortType.Ecu ||
                port.type == SerialPortType.EcuWithOpenblt ||
                port.type == SerialPortType.OpenBlt) {
                hasEcuOrBootloader = true;
            }
        }
        String defaultPort = getConfig().getRoot().getProperty(ConsoleUI.PORT_KEY);
        if (!PersistentConfiguration.getBoolProperty(ALWAYS_AUTO_PORT)) {
            comboPorts.setSelectedItem(defaultPort);
        }

        AutoupdateUtil.trueLayoutAndRepaint(comboPorts);
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
