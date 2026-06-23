package com.rusefi;

import com.devexperts.logging.Logging;
import com.opensr5.ConfigurationImage;
import com.opensr5.ini.IniFileModel;
import com.opensr5.ini.PrimeTunerStudioCache;
import com.rusefi.autoupdate.Autoupdate;
import com.rusefi.core.net.ConnectionAndMeta;
import com.rusefi.core.net.PropertiesHolder;
import com.rusefi.core.preferences.storage.PersistentConfiguration;
import com.rusefi.ts.TsProjectCreator;
import com.rusefi.core.ui.AutoupdateUtil;
import com.rusefi.core.ui.FrameHelper;
import com.rusefi.io.ConnectionStatusLogic;
import com.rusefi.io.LinkManager;
import com.rusefi.maintenance.*;
import com.rusefi.tools.TunerStudioHelper;
import com.rusefi.ui.BasicLogoHelper;
import com.rusefi.ui.LogoHelper;
import com.rusefi.ui.UIContext;
import com.rusefi.ui.wizard.WizardCatalog;
import com.rusefi.ui.wizard.WizardContainer;
import com.rusefi.ui.wizard.WizardStep;
import com.rusefi.ui.wizard.WizardStepDescriptor;
import com.rusefi.ui.duplicates.ConsoleBundleUtil;
import com.rusefi.ui.util.HorizontalLine;
import com.rusefi.ui.util.URLLabel;
import com.rusefi.ui.util.UiUtils;
import com.rusefi.io.DoubleCallbacks;
import com.rusefi.maintenance.jobs.AsyncJob;
import com.rusefi.maintenance.jobs.DfuAutoJob;
import com.rusefi.maintenance.jobs.ExportTuneJob;
import com.rusefi.maintenance.jobs.ImportTuneJob;
import com.rusefi.maintenance.jobs.OpenBltAutoJob;
import com.rusefi.maintenance.jobs.OpenBltSwitchJob;
import com.rusefi.ui.basic.FirmwareUpdateTab;
import com.rusefi.ui.basic.SingleAsyncJobExecutor;
import com.rusefi.ui.basic.StatusPanelWithProgressBar;
import com.rusefi.ui.basic.TuneManagementTab;
import com.rusefi.ui.widgets.StatusPanel;
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
import java.util.concurrent.atomic.AtomicReference;
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
    private static final String STARTUP_TAB_INDEX = "startup_tab_index";
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
    // TODO: we should rename this!; now we are showing more than the "no ports connected" message
    private final JLabel noPortsMessage = new JLabel();
    private final JLabel dfuErrorMessage = new JLabel(
        "Failed to check for DFU devices. Try 'Run as Administrator'");
    private boolean hasSeenEcuOrSimulator = false;
    private final StatusAnimation status;
    private final JButton connectButton = new JButton("Connect", new ImageIcon(getClass().getResource("/com/rusefi/connect48.png")));
    private ProgramSelector selector;
    private boolean firstTimeAutoConnect = true;

    private final StatusPanelWithProgressBar firmwareStatusPanel = new StatusPanelWithProgressBar();
    private final StatusPanel tuneStatusPanel = new StatusPanel(250);
    private final SingleAsyncJobExecutor asyncJobExecutor = new SingleAsyncJobExecutor(
        new DoubleCallbacks(firmwareStatusPanel, tuneStatusPanel));
    private final AtomicReference<Optional<PortResult>> ecuPortToUse = new AtomicReference<>(Optional.empty());
    private FirmwareUpdateTab firmwareUpdateTab;
    private StatusAnimation firmwareTabStatus;
    private JTabbedPane outerTabs;

    private final UIContext uiContext;
    private final JPanel rootContent = new JPanel(new CardLayout());
    private WizardContainer wizardContainer;
    private PortResult autoConnectedPort;
    private Thread autoConnectThread;
    // [tag:offline_tune] True once an offline-tune console has been opened on the shared uiContext. The
    // splash window is gone but the port scanner stays alive so a plugged-in ECU still auto-connects —
    // and the splash must NOT open a second console when that happens (the offline console transitions
    // itself online).
    private boolean offlineConsoleOpen = false;
    private ConnectionStatusLogic.Listener splashListener;
    // Registered in releaseSplashConnection() for firmware jobs; fires once on post-flash reconnect.
    private ConnectionStatusLogic.Listener postFlashReconnectListener;
    // Saved in releaseSplashConnection() for tune import/export jobs; restored in onLiveConnectionJobFinished().
    private PortResult postLiveConnectionJobPort;

    public StartupFrame(ConnectivityContext connectivityContext, UIContext uiContext) {
        this.connectivityContext = connectivityContext;
        this.uiContext = uiContext;
        // Firmware-update and tune operations open the serial port themselves via a fresh
        // LinkManager. If our splash auto-connect is holding the same port, they'd deadlock on
        // "Connecting...". Release the splash connection just before any job starts so the port
        // is free for the exclusive operation.
        asyncJobExecutor.addOnJobAboutToStartListener(this::releaseSplashConnection);
        asyncJobExecutor.addOnJobInProgressFinishedListener(this::onLiveConnectionJobFinished);
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
                    if (autoConnectedPort != null) {
                        // Splash owned the connection and we're not handing off — release it.
                        uiContext.getLinkManager().close();
                    }
                    saveTabIndex();
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

        portsComboBox.getComboPorts().addActionListener(e -> {
            updateConnectButtonState();
            final PortResult selectedPort = (PortResult) portsComboBox.getComboPorts().getSelectedItem();
            if (autoConnectedPort == null || selectedPort == null || selectedPort.port.equals(autoConnectedPort.port)) {
                return;
            }
            if (selectedPort.type != SerialPortType.Ecu && selectedPort.type != EcuWithOpenblt) {
                return;
            }
            if (!asyncJobExecutor.isNotInProgress()) {
                return;
            }
            onSplashDisconnected();
            autoConnect(selectedPort);
        });

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

        dfuErrorMessage.setForeground(Color.red);
        dfuErrorMessage.setVisible(false);
        realHardwarePanel.add(dfuErrorMessage, "right, wrap");

        // After a grace period, warn the user if DFU scanning failed and no ECU was ever seen.
        // This typically means the console needs to be run as Administrator on Windows.
        Timer dfuErrorTimer = new Timer(15_000, e -> {
            if (DfuFlasher.dfuDetectionCommandFailed && !hasSeenEcuOrSimulator) {
                dfuErrorMessage.setVisible(true);
                frame.pack();
            }
        });
        dfuErrorTimer.setRepeats(false);
        dfuErrorTimer.start();

        selector = new ProgramSelector(connectivityContext, portsComboBox.getComboPorts());
        selector.setJobExecutor(asyncJobExecutor);

        realHardwarePanel.add(new HorizontalLine(), "right, wrap");

        JButton openTunerStudio = new JButton("Open TunerStudio");
        setToolTip(openTunerStudio, "Launch TunerStudio and close this console so the serial port is released");
        openTunerStudio.addActionListener(e -> {
            if (TunerStudioHelper.launchTunerStudio(frame)) {
                frame.dispatchEvent(new WindowEvent(frame, WindowEvent.WINDOW_CLOSING));
            }
        });
        realHardwarePanel.add(openTunerStudio, "right, wrap");

        connectivityContext.getSerialPortScanner().addListener(currentHardware -> SwingUtilities.invokeLater(() -> {
            if (offlineConsoleOpen) {
                // [tag:offline_tune] Splash UI is disposed; keep only the auto-connect path alive so
                // plugging in an ECU transitions the already-open offline console online (see onSplashConnected).
                applyKnownPorts(currentHardware);
                return;
            }
            status.stop();
            // Hide the scanning indicator after the initial scan completes —
            // the scanner runs continuously in the background but the UI shouldn't
            // show "Scanning ports..." on every cycle. Only clear if the current
            // text is the scanning animation, preserving other messages like
            // "Connected to X" or auto-connect failures.
            String currentText = noPortsMessage.getText();
            if (currentText != null && currentText.startsWith(SCANNING_PORTS)) {
                noPortsMessage.setText("");
                noPortsMessage.setVisible(false);
            }
            selector.apply(currentHardware);
            applyKnownPorts(currentHardware);
            if (!hasSeenEcuOrSimulator) {
                boolean ecuOrSimSeen = currentHardware.getKnownPorts().stream()
                    .anyMatch(p -> p.type == SerialPortType.Ecu || p.type == SerialPortType.EcuWithOpenblt);
                if (ecuOrSimSeen) {
                    hasSeenEcuOrSimulator = true;
                }
            }
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

        JPanel connectTabWrapper = new JPanel(new BorderLayout());
        connectTabWrapper.add(content, BorderLayout.NORTH);

        outerTabs = new JTabbedPane() {
            @Override
            public Dimension getPreferredSize() {
                Dimension superPref = super.getPreferredSize();
                Component sel = getSelectedComponent();
                if (sel == null) return superPref;
                int connectTabHeight = Math.max(sel.getPreferredSize().height, leftPanel.getPreferredSize().height);
                return new Dimension(superPref.width, 100 + connectTabHeight);
            }
        };
        firmwareUpdateTab = new FirmwareUpdateTab(
            connectivityContext, UiProperties.getWhiteLabel(),
            firmwareStatusPanel, asyncJobExecutor, ecuPortToUse);

        JPanel firmwareTopPanel = new JPanel(new BorderLayout(0, 0));
        firmwareTopPanel.add(selector.getControl(), BorderLayout.NORTH);
        firmwareTopPanel.add(firmwareUpdateTab.getBasicUpdaterPanel().getMigrateSettings(), BorderLayout.SOUTH);

        JPanel firmwareTabPanel = new JPanel(new BorderLayout(0, 0));
        firmwareTabPanel.add(firmwareTopPanel, BorderLayout.NORTH);
        firmwareTabPanel.add(firmwareStatusPanel.getContent(), BorderLayout.CENTER);
        outerTabs.addTab("Update Firmware", firmwareTabPanel);
        outerTabs.addTab("Manage Tunes", new TuneManagementTab(
            connectivityContext,
            uiContext,
            firmwareUpdateTab.getBasicUpdaterPanel().getImportTuneButton().getContent(),
            asyncJobExecutor,
            tuneStatusPanel,
            this::openOfflineConsole
        ).getContent());
        outerTabs.addTab("Connect", connectTabWrapper);

        int savedTabIndex = getConfig().getRoot().getIntProperty(STARTUP_TAB_INDEX, 0);
        outerTabs.setSelectedIndex(Math.min(savedTabIndex, outerTabs.getTabCount() - 1));

        connectivityContext.getSerialPortScanner().addListener(currentHardware -> SwingUtilities.invokeLater(() -> {
            if (firmwareTabStatus != null)
                firmwareTabStatus.stop();
            firmwareUpdateTab.getBasicUpdaterPanel().onHardwareUpdated();
        }));

        firmwareTabStatus = new StatusAnimation(
            msg -> firmwareUpdateTab.getBasicUpdaterPanel().updateStatus(msg),
            SCANNING_PORTS);

        wizardContainer = new WizardContainer(uiContext, /*compact=*/true);
        wizardContainer.setOnWizardExit(() -> {
            showCard("startup");
        });
        rootContent.add(outerTabs, "startup");
        rootContent.add(wizardContainer, "wizard");

        TunerStudioHelper.checkTunerStudio(frame.getContentPane(), () -> restoreContent(rootContent));

        frame.add(rootContent);
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

    private void restoreContent(JComponent root) {
        frame.getContentPane().removeAll();
        frame.add(root);
        AutoupdateUtil.pack(frame);
    }

    private void updateConnectButtonState() {
        PortResult selectedItem = (PortResult) portsComboBox.getComboPorts().getSelectedItem();
        connectButton.setEnabled(selectedItem != null && selectedItem.type != OpenBlt);
    }

    private void applyKnownPorts(AvailableHardware currentHardware) {
        List<PortResult> ports = currentHardware.getKnownPorts();
        // Once auto-connect is in flight (or succeeded), the splash-connect flow owns the Connect
        // tab's UI state — skip re-rendering so we don't stomp on the "Connecting..."/"Connected to X"
        // label and re-enable a disabled combo.
        if (autoConnectedPort != null) {
            return;
        }
        log.info("Rendering available ports: " + ports);
        connectPanel.setVisible(!ports.isEmpty());


        boolean hasEcuOrBootloader = applyPortSelectionToUIcontrol(portsComboBox.getComboPorts(), ports);
        PortResult openBltPort = ports.stream()
            .filter(p -> p.type == OpenBlt)
            .findFirst()
            .orElse(null);
        boolean hasOpenBlt = openBltPort != null;
        if (ports.isEmpty()) {
            noPortsMessage.setForeground(Color.red);
            noPortsMessage.setText(NO_PORTS_FOUND);
        } else if (hasOpenBlt) {
            // A board sitting in the OpenBLT bootloader has no running firmware to auto-connect to —
            // mirror the auto-connect status line and point the user at the firmware-update flow.
            noPortsMessage.setForeground(Color.darkGray);
            noPortsMessage.setText("Board in OpenBLT bootloader on " + openBltPort.port
                + " — use the Update Firmware tab");
        } else {
            noPortsMessage.setForeground(Color.red);
            noPortsMessage.setText("Make sure you are disconnected from TunerStudio");
        }

        updateConnectButtonState();

        noPortsMessage.setVisible(ports.isEmpty() || !hasEcuOrBootloader || hasOpenBlt);

        AutoupdateUtil.trueLayoutAndRepaint(connectPanel);

        // Kick off auto-connect last — combo is already populated, so if this fails and we revert
        // to manual mode, the user has options to pick from without waiting for another scan event.
        List<PortResult> ecuPorts = ports.stream()
            .filter(p -> p.type == SerialPortType.Ecu || p.type == EcuWithOpenblt)
            .collect(Collectors.toList());
        if (ecuPorts.size() == 1 && firstTimeAutoConnect && shouldAutoConnect()) {
            firstTimeAutoConnect = false;
            PortResult target = ecuPorts.get(0);
            log.info("Single ECU detected, auto-connecting in background: " + target);
            autoConnect(target);
        }
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
        log.info("connect: port=" + selectedPort.port);
        boolean alreadyConnected = isAutoConnected(selectedPort);
        log.info("connect: alreadyConnected=" + alreadyConnected);
        // Splash started LinkManager but the live connection dropped (ECU reboot, cable yank)
        // before the user clicked Connect. Reset it so ConsoleUI's start() can set a fresh connector
        // without tripping the "Already started" guard.
        if (!alreadyConnected && autoConnectedPort != null) {
            log.info("connect: closing stale LinkManager");
            uiContext.getLinkManager().close();
        }
        log.info("connect: disposing splash and creating ConsoleUI");
        disposeFrameAndProceed();
        new ConsoleUI(uiContext, selectedPort.port, selectedPort.type, alreadyConnected);
    }

    /**
     * [tag:offline_tune]
     * Opens the full console in offline mode with a pre-loaded tune, reusing the shared {@link UIContext}.
     * Disposes the splash window but keeps the serial-port scanner running so that plugging in an ECU
     * later auto-connects and the offline console transitions itself online — without spawning a second
     * console (see the {@link #offlineConsoleOpen} guards).
     */
    public void openOfflineConsole(IniFileModel ini, ConfigurationImage image) {
        log.info("openOfflineConsole: launching offline console on shared uiContext");
        offlineConsoleOpen = true;
        isProceeding = true;
        // Drop the splash-scoped connection listener so it can't mutate the disposed splash widgets.
        if (splashListener != null) {
            ConnectionStatusLogic.INSTANCE.removeListener(splashListener);
            splashListener = null;
        }
        saveTabIndex();
        getConfig().save();
        frame.dispose();
        status.stop();
        if (firmwareTabStatus != null) {
            firmwareTabStatus.stop();
        }
        // NOTE: intentionally do NOT call serialPortScanner.stopTimer() — the offline console relies on
        // the scanner's auto-connect to go online.
        new ConsoleUI(uiContext, ini, image);
    }

    /**
     * True when the splash already established a connection to {@code target} via {@link #autoConnect}.
     * In that case, the handoff to {@link ConsoleUI} must not re-start the {@link LinkManager}.
     */
    private boolean isAutoConnected(PortResult target) {
        return autoConnectedPort != null
            && target != null
            && autoConnectedPort.port.equals(target.port)
            && ConnectionStatusLogic.INSTANCE.isConnected();
    }

    /**
     * Auto-connect is permitted as long as no firmware-update job is in flight. The Connect tab
     * may not be visible yet — the scanner only fires {@code onChange} on hardware-list changes,
     * so gating on the current tab would silently disable auto-connect for users who saved a
     * different startup tab last session.
     */
    private boolean shouldAutoConnect() {
        return asyncJobExecutor.isNotInProgress();
    }

    private void autoConnect(PortResult target) {
        autoConnectedPort = target;
        if (offlineConsoleOpen) {
            // [tag:offline_tune] Pre-cache the target so the scanner skips re-inspecting it during the
            // connect read window — otherwise a scan tick could reopen the port mid-read and hang in LOADING.
            connectivityContext.getSerialPortScanner().cachePort(new PortResult(target.port, target.type));
        }
        connectButton.setEnabled(false);
        connectButton.setText("Connecting...");
        portsComboBox.getComboPorts().setEnabled(false);
        noPortsMessage.setText("Connecting to " + target.port + "…");
        noPortsMessage.setForeground(Color.darkGray);
        noPortsMessage.setVisible(true);

        splashListener = new ConnectionStatusLogic.Listener() {
            @Override
            public void onConnectionStatus(boolean isConnected) {
                if (!isConnected) {
                    SwingUtilities.invokeLater(() -> onSplashDisconnected());
                    return;
                }
                SwingUtilities.invokeLater(() -> {
                    if (!ConnectionStatusLogic.INSTANCE.isConnected()) {
                        return;
                    }
                    if (uiContext.getBinaryProtocol() == null) {
                        return;
                    }
                    if (uiContext.getBinaryProtocol().getControllerConfiguration() == null) {
                        return;
                    }
                    onSplashConnected(target);
                });
            }

            @Override
            public void onConnectionFailed(String msg) {
                SwingUtilities.invokeLater(() -> onSplashConnectFailed(msg));
            }
        };
        ConnectionStatusLogic.INSTANCE.addListener(splashListener);

        autoConnectThread = new NamedThreadFactory("splash-connect").newThread(
            () -> uiContext.getLinkManager().startAndConnect(target.port, splashListener));
        autoConnectThread.start();
    }

    /**
     * Flip the root {@link CardLayout} and resize the frame to the active card's preferred size.
     * Without per-card packing, CardLayout reports the max of all children, leaving empty
     * horizontal margins around the narrower wizard content.
     */
    private void showCard(String name) {
        CardLayout cl = (CardLayout) rootContent.getLayout();
        cl.show(rootContent, name);
        Component active = null;
        for (Component c : rootContent.getComponents()) {
            if (c.isVisible()) {
                active = c;
                break;
            }
        }
        if (active != null) {
            rootContent.setPreferredSize(active.getPreferredSize());
        }
        frame.pack();
    }

    private void onSplashConnected(PortResult target) {
        if (autoConnectedPort == null || !autoConnectedPort.port.equals(target.port)) {
            // User cancelled or moved on — ignore the late event.
            return;
        }
        if (offlineConsoleOpen) {
            // [tag:offline_tune] The offline console (already open on the shared uiContext) is now connected
            // via autoConnect.
            // Cache the connected port so the still-running scanner does NOT re-probe it — re-opening the
            // connected port mid-read races the live read and hangs the UI in LOADING. The scanner stays
            // alive (and the splash listener stays registered) so a later disconnect/replug — possibly
            // under a different port name — auto-reconnects (handled via onSplashDisconnected re-arming).
            // Do NOT open a second console.
            log.info("onSplashConnected: offline console online on " + target.port + " — caching port, scanner kept alive");
            connectivityContext.getSerialPortScanner().cachePort(new PortResult(target.port, target.type));
            return;
        }
        connectButton.setText("Connect");
        connectButton.setEnabled(true);
        portsComboBox.getComboPorts().setEnabled(true);
        noPortsMessage.setForeground(Color.darkGray);
        noPortsMessage.setText("Connected to " + target.port + " — launching console…");
        noPortsMessage.setVisible(true);

        // Hand the live LinkManager to firmware-update jobs so they can disconnect/reconnect
        // cleanly instead of closing and re-opening the port from scratch.
        final LinkManager lm = uiContext.getLinkManager();
        firmwareUpdateTab.getBasicUpdaterPanel().setSplashLinkManager(lm);
        selector.setLinkManager(lm);

        maybeAutoCreateTsProject(target);

        // Check standalone wizard catalog for any step that needs attention on this ECU.
        for (WizardStepDescriptor d : WizardCatalog.standaloneAutoLaunch()) {
            if (!d.applicable.test(uiContext)) continue;
            if (d.needsAttention == null || !d.needsAttention.test(uiContext)) continue;
            WizardStep step = d.factory.apply(uiContext);
            wizardContainer.startSingleStep(step);
            showCard("wizard");
            return;
        }
        connect(target);
    }

    private void maybeAutoCreateTsProject(PortResult target) {
        if (!Boolean.parseBoolean(PropertiesHolder.getProperty("auto_create_project", "false"))) {
            return;
        }
        if (target.getCalibrations() == null) {
            return;
        }
        String projectName = PropertiesHolder.getProperty("default_project_name", "rusEFI");
        new Thread(() -> {
            try {
                boolean created = TsProjectCreator.createIfMissing(projectName, target.port, target.getCalibrations());
                log.info(created ? "Auto-created TS project " + projectName : "TS project " + projectName + " already exists, skipping");
            } catch (Exception e) {
                log.warn("Auto-create TS project failed", e);
            }
        }, "Auto-create TS project").start();
    }

    /**
     * Called just before an async job starts. Detaches the splash connection listener and decides
     * what to do with the live {@link LinkManager}:
     * <ul>
     *   <li>Firmware jobs ({@link DfuAutoJob}, {@link OpenBltAutoJob}): keep the LM alive —
     *       the job owns the disconnect/reconnect lifecycle.</li>
     *   <li>Tune import ({@link ImportTuneJob}): keep the LM alive — the job submits reads and
     *       writes through the executor queue while the pull thread runs.</li>
     *   <li>Any other job: close the LM so the port is released for the job's own connection.</li>
     * </ul>
     */
    private void releaseSplashConnection() {
        if (autoConnectedPort == null) return;
        log.info("Releasing splash auto-connection before async job");

        Optional<AsyncJob> job = asyncJobExecutor.getJobInProgress();
        boolean isFirmwareJob = job.map(j -> j instanceof DfuAutoJob || j instanceof OpenBltAutoJob).orElse(false);
        boolean isLiveConnectionJob = job.map(j -> j instanceof ImportTuneJob || j instanceof ExportTuneJob || j instanceof OpenBltSwitchJob).orElse(false);

        // Detach the original splash listener in all cases — the disconnect() inside
        // firmware jobs would fire NOT_CONNECTED and we don't want onSplashDisconnected()
        // to clobber state during an intentional firmware-update sequence.
        if (splashListener != null) {
            ConnectionStatusLogic.INSTANCE.removeListener(splashListener);
            splashListener = null;
        }
        // Clear the LM reference so it is rebuilt fresh after reconnect.
        firmwareUpdateTab.getBasicUpdaterPanel().setSplashLinkManager(null);

        if (isFirmwareJob) {
            // Firmware job owns disconnect/reconnect.  Register a one-shot listener so we
            // re-display "Connected" after the post-flash reconnect fires CONNECTED.
            PortResult savedPort = autoConnectedPort;
            postFlashReconnectListener = new ConnectionStatusLogic.Listener() {
                @Override
                public void onConnectionStatus(boolean isConnected) {
                    if (!isConnected) return;
                    ConnectionStatusLogic.INSTANCE.removeListener(this);
                    if (this == postFlashReconnectListener) postFlashReconnectListener = null;
                    SwingUtilities.invokeLater(() -> {
                        noPortsMessage.setForeground(Color.darkGray);
                        noPortsMessage.setText("Connected to " + savedPort.port + " — click Connect to open console");
                        noPortsMessage.setVisible(true);
                        connectPanel.setVisible(true);
                        final LinkManager reconnectedLm = uiContext.getLinkManager();
                        autoConnectedPort = savedPort;
                        firmwareUpdateTab.getBasicUpdaterPanel().setSplashLinkManager(reconnectedLm);
                        selector.setLinkManager(reconnectedLm);
                        // Restore the port in the combo so the user can switch if needed.
                        portsComboBox.getComboPorts().removeAllItems();
                        portsComboBox.getComboPorts().addItem(savedPort);
                        portsComboBox.getComboPorts().setSelectedItem(savedPort);
                        // Restore ecuPortToUse so the Upload Tune button is enabled.
                        ecuPortToUse.set(Optional.of(savedPort));
                        firmwareUpdateTab.getBasicUpdaterPanel().refreshButtons();
                    });
                }
            };
            ConnectionStatusLogic.INSTANCE.addListener(postFlashReconnectListener);
            autoConnectedPort = null;
            autoConnectThread = null;
            SwingUtilities.invokeLater(() -> {
                connectButton.setText("Connect");
                connectButton.setEnabled(true);
                portsComboBox.getComboPorts().setEnabled(true);
                noPortsMessage.setForeground(Color.darkGray);
                noPortsMessage.setText("Updating firmware...");
                noPortsMessage.setVisible(true);
            });
            return;
        }

        if (!isLiveConnectionJob) {
            // Non-handoff job: release the port so the job can open it.
            uiContext.getLinkManager().close();
        } else {
            // Tune import/export keeps the LM alive. Save the port so onLiveConnectionJobFinished()
            // can restore the selector LinkManager once the job completes.
            postLiveConnectionJobPort = autoConnectedPort;
        }

        autoConnectedPort = null;
        autoConnectThread = null;
        // Reset UI on the Connect tab so the user sees a normal state after the job finishes.
        SwingUtilities.invokeLater(() -> {
            connectButton.setText("Connect");
            connectButton.setEnabled(true);
            portsComboBox.getComboPorts().setEnabled(true);
            noPortsMessage.setForeground(Color.darkGray);
            noPortsMessage.setText("");
            noPortsMessage.setVisible(false);
        });
    }

    /**
     * Restores the selector LinkManager after a tune import/export job finishes.
     */
    private void onLiveConnectionJobFinished() {
        final PortResult savedPort = postLiveConnectionJobPort;
        if (savedPort == null) {
            return;
        }
        postLiveConnectionJobPort = null;
        SwingUtilities.invokeLater(() -> {
            final LinkManager lm = uiContext.getLinkManager();
            if (lm.getBinaryProtocol() == null) {
                return;
            }
            selector.setLinkManager(lm);
            autoConnectedPort = savedPort;
            firmwareUpdateTab.getBasicUpdaterPanel().setSplashLinkManager(lm);
            noPortsMessage.setForeground(Color.darkGray);
            noPortsMessage.setText("Connected to " + savedPort.port + " — click Connect to open console");
            noPortsMessage.setVisible(true);
        });
    }

    /**
     * Called on the EDT when the ECU drops the connection while still on the splash screen
     * lears splash-owned state so the port combo is repopulated
     * on the next scanner tick and the user can manually pick a port or wait for ECU B.
     */
    private void onSplashDisconnected() {
        if (splashListener != null) {
            ConnectionStatusLogic.INSTANCE.removeListener(splashListener);
            splashListener = null;
        }
        firmwareUpdateTab.getBasicUpdaterPanel().setSplashLinkManager(null);
        // Sets isStarted=false so the next connect() can create a new LinkManager connector.
        uiContext.getLinkManager().close();
        if (offlineConsoleOpen && autoConnectedPort != null) {
            // [tag:offline_tune] The offline console is still up. Re-arm scanner-driven auto-connect and
            // let the scanner re-inspect the (now stale) port so the console reconnects when the board
            // comes back — possibly under a different port name (USB re-enumeration).
            connectivityContext.getSerialPortScanner().invalidatePort(autoConnectedPort.port);
            firstTimeAutoConnect = true;
        }
        autoConnectedPort = null;
        autoConnectThread = null;
        connectButton.setText("Connect");
        connectButton.setEnabled(true);
        portsComboBox.getComboPorts().setEnabled(true);
    }

    private void onSplashConnectFailed(String msg) {
        log.warn("Splash auto-connect failed: " + msg);
        // Remove the listener before calling close() so the NOT_CONNECTED event it emits does
        // not re-trigger onSplashDisconnected() and overwrite the failure message below.
        if (splashListener != null) {
            ConnectionStatusLogic.INSTANCE.removeListener(splashListener);
            splashListener = null;
        }
        // Per design: keep auto-connect gate closed for the rest of the session.
        uiContext.getLinkManager().close();
        autoConnectedPort = null;
        autoConnectThread = null;
        connectButton.setEnabled(true);
        connectButton.setText("Connect");
        portsComboBox.getComboPorts().setEnabled(true);
        noPortsMessage.setForeground(Color.red);
        noPortsMessage.setText("Auto-connect failed: " + msg);
        noPortsMessage.setVisible(true);
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

    public void restartConsole() {
        // not much point requesting an extra click - if we have updated, we shall restart
        if (frame.isDisplayable())
            disposeFrameAndProceed();
        SimulatorHelper.onWindowClosed();
        Autoupdate.relaunchConsole();
    }

    private void saveTabIndex() {
        if (outerTabs != null)
            getConfig().getRoot().setProperty(STARTUP_TAB_INDEX, outerTabs.getSelectedIndex());
    }

    public void disposeFrameAndProceed() {
        isProceeding = true;
        // Detach the splash-scoped connection listener so it doesn't keep mutating this disposed
        // frame's widgets when ConsoleUI (or a reconnect) fires ConnectionStatusLogic events.
        if (splashListener != null) {
            ConnectionStatusLogic.INSTANCE.removeListener(splashListener);
            splashListener = null;
        }
        if (postFlashReconnectListener != null) {
            ConnectionStatusLogic.INSTANCE.removeListener(postFlashReconnectListener);
            postFlashReconnectListener = null;
        }
        saveTabIndex();
        getConfig().save();
        frame.dispose();
        status.stop();
        if (firmwareTabStatus != null)
            firmwareTabStatus.stop();
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

}
