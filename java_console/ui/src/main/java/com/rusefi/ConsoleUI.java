package com.rusefi;

import com.devexperts.logging.FileLogger;
import com.devexperts.logging.Logging;
import com.opensr5.ConfigurationImage;
import com.opensr5.ini.IniFileModel;
import com.rusefi.autodetect.PortDetector;
import com.rusefi.binaryprotocol.BinaryProtocolLogger;
import com.rusefi.binaryprotocol.ShortcutsHelper;
import com.rusefi.core.MessagesCentral;
import com.rusefi.core.net.ConnectionAndMeta;
import com.rusefi.io.CommandQueue;
import com.rusefi.io.LinkManager;
import com.rusefi.io.serial.BaudRateHolder;
import com.rusefi.maintenance.StLinkFlasher;
import com.rusefi.ui.*;
import com.rusefi.ui.console.MainFrame;
import com.rusefi.ui.console.TabbedPanel;
import com.rusefi.ui.engine.EngineSnifferPanel;
import com.rusefi.ui.lua.LuaScriptPanel;
import com.rusefi.ui.util.JustOneInstance;
import com.rusefi.ui.widgets.ConnectionStatusIcon;
import com.rusefi.ui.wizard.WizardCatalog;
import com.rusefi.ui.wizard.WizardContainer;
import com.rusefi.ui.wizard.WizardStep;
import com.rusefi.ui.wizard.WizardStepDescriptor;
import com.rusefi.io.ConnectionStatusLogic;
import com.rusefi.io.ConnectionStatusValue;
import com.rusefi.core.ui.AutoupdateUtil;
import com.rusefi.util.LazyFile;
import com.rusefi.util.LazyFileImpl;
import org.jetbrains.annotations.NotNull;


import javax.swing.*;
import javax.swing.event.ChangeEvent;
import javax.swing.event.ChangeListener;
import com.rusefi.ui.InitOnFirstPaintPanel;
import com.rusefi.ui.basic.LoadTuneHelper;
import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.util.function.Supplier;
import java.io.IOException;
import java.lang.reflect.InvocationTargetException;
import java.util.HashMap;
import java.util.Map;
import java.util.concurrent.atomic.AtomicReference;
import java.util.function.Consumer;

import static com.devexperts.logging.Logging.getLogging;
import static com.rusefi.StartupFrame.setFrameIcon;
import static com.rusefi.core.preferences.storage.PersistentConfiguration.getConfig;
import static com.rusefi.ui.basic.UiHelper.commonUiStartup;
import static com.rusefi.ui.util.UiUtils.createOnTopParent;

/**
 * Main frame of rusEFI updater app
 *
 * @see StartupFrame
 */
public class ConsoleUI {
    private static final Logging log = getLogging(ConsoleUI.class);
    private static final int DEFAULT_TAB_INDEX = 0;
    private static final String WIKI_URL = "https://wiki.rusefi.com/rusEFI-logs-folder";

    public static final String TAB_INDEX = "main_tab";
    protected static final String PORT_KEY = "port";
    protected static final String SPEED_KEY = "speed";
    public static final String TITLE = "rusEFI";
    public static EngineSnifferPanel engineSnifferPanel;

    private final TabbedPanel tabbedPane;
    private final String port;

    public final UIContext uiContext;

    /**
     * We can listen to tab activation event if we so desire
     */
    private final Map<Component, ActionListener> tabSelectedListeners = new HashMap<>();

    public ConsoleUI(String port, SerialPortType serialPortType) {
        this(new UIContext(), port, serialPortType, false, null, null, null);
    }

    public ConsoleUI(UIContext uiContext, String port, SerialPortType serialPortType, boolean alreadyConnected) {
        this(uiContext, port, serialPortType, alreadyConnected, null, null, null);
    }

    /**
     * Reuses the already-visible, maximized {@code reuseFrame} handed off from {@link StartupFrame}
     * instead of opening a second window (#9715).
     */
    public ConsoleUI(UIContext uiContext, String port, SerialPortType serialPortType, boolean alreadyConnected, JFrame reuseFrame) {
        this(uiContext, port, serialPortType, alreadyConnected, null, null, reuseFrame);
    }

    /**
     * [tag:offline_tune]
     * Constructor for offline mode: opens the console with a pre-loaded tune and INI,
     * without requiring a live ECU connection.
     *
     * @param uiContext      shared UI context
     * @param ini            INI file model resolved from the MSQ signature
     * @param initialImage   ConfigurationImage loaded from the MSQ file
     */
    public ConsoleUI(UIContext uiContext, IniFileModel ini, ConfigurationImage initialImage) {
        this(uiContext, null, SerialPortType.Unknown, false, ini, initialImage, null);
    }

    /**
     * A menu action that builds the (lazily-constructed) Tuning pane on first use, then delegates to
     * its real action (#9715). After the build, {@code buildTuning} re-points the menu at the real
     * actions, so this wrapper only ever runs once.
     */
    private static Action lazyTuneAction(String text, Runnable buildTuning, Supplier<Action> realAction) {
        return new AbstractAction(text) {
            @Override
            public void actionPerformed(ActionEvent e) {
                buildTuning.run();
                realAction.get().actionPerformed(e);
            }
        };
    }

    private ConsoleUI(UIContext uiContext, String port, SerialPortType serialPortType,
                      boolean alreadyConnected, IniFileModel offlineIni, ConfigurationImage offlineImage,
                      JFrame reuseFrame) {
        this.uiContext = uiContext;
        LinkManager linkManager = uiContext.getLinkManager();

        boolean isOffline = (offlineIni != null && offlineImage != null);
        if (isOffline) {
            uiContext.setOfflineMode(true);
            uiContext.setIniFileModelForOffline(offlineIni);
        }

        CommandQueue.ERROR_HANDLER = e -> {
            log.error("Connectivity error", e);
            linkManager.restart();
        };

        tabbedPane = new TabbedPanel(uiContext);

        // Pass the tabbed pane so the icon can turn purple when a board is in a DFU/OpenBLT bootloader
        // (DevicePane publishes the "bootloaderMode" client property on it). [tag:better_ux_for_flashing]
        ConnectionStatusIcon connectionStatus = new ConnectionStatusIcon(linkManager, tabbedPane.tabbedPane);
        this.port = port;

        // Follow a renumbered ECU across a bootloader round-trip. After a reboot to DFU/OpenBLT *without*
        // flashing, the board can re-enumerate onto a different ttyACMx. ConnectionWatchdog.restart() only
        // retries the original port name (isPortAvailableAgain=false forever), so the live session would
        // never come back. When the scanner has classified exactly one ECU on a new port and the port we
        // were on has vanished, repoint the LinkManager there. [tag:better_ux_for_flashing]
        if (!isOffline) {
            ProductionConnectivity.CONTEXT.getPortScanner().addListener(currentHardware -> {
                if (linkManager.isDisconnectedByUser()) {
                    return;
                }
                // NB: intentionally NOT gated on ConnectionStatusLogic.isConnected(). After an OpenBLT
                // switch the status can stay stale "connected", which would block recovery forever.
                // if the port we were on is gone, the link is
                // dead regardless of the status flag; while genuinely connected that port is still present
                // so we skip and let the watchdog's same-name restart() own the reconnect.
                final String currentPort = linkManager.getLastTriedPort();
                if (currentPort == null || LinkManager.getCommPorts().contains(currentPort)) {
                    return;
                }
                final java.util.List<PortResult> ecuPorts = currentHardware.getKnownPorts(
                    CompatibilitySet.of(SerialPortType.Ecu, SerialPortType.EcuWithOpenblt));
                if (ecuPorts.size() != 1) {
                    return;
                }
                final String newPort = ecuPorts.get(0).port;
                if (newPort.equals(currentPort)) {
                    return;
                }
                log.info("ECU reappeared on " + newPort + " (was " + currentPort + ") — following renumbered port");
                linkManager.execute(() -> linkManager.reconnect(newPort));
            });
        }

        // Wizard container and CardLayout for switching between console and wizard modes
        JPanel rootPanel = new JPanel(new CardLayout());
        rootPanel.add(tabbedPane.getContent(), "console");

        WizardContainer wizardContainer = new WizardContainer(uiContext);
        rootPanel.add(wizardContainer, "wizard");

        CardLayout rootCardLayout = (CardLayout) rootPanel.getLayout();

        JButton launchWizardButton = getLaunchWizardButton(rootPanel, wizardContainer, rootCardLayout);

        wizardContainer.setOnWizardExit(() -> rootCardLayout.show(rootPanel, "console"));

        // On ECU connect, scan the wizard catalog for applicable standalone steps that need attention
        // (e.g. empty VIN) and auto-launch the first one. Fires on every reconnect; once the user
        // saves the value, subsequent connects skip this because needsAttention returns false.
        ConnectionStatusLogic.INSTANCE.addListener(isConnected -> {
            if (!isConnected) return;
            SwingUtilities.invokeLater(() -> {
                if (!ConnectionStatusLogic.INSTANCE.isConnected()) return;
                if (uiContext.getBinaryProtocol() == null) return;
                if (uiContext.getBinaryProtocol().getControllerConfiguration() == null) return;
                // Don't stomp on an already-visible wizard
                if (wizardContainer.isShowing()) return;

                for (WizardStepDescriptor d : WizardCatalog.standaloneAutoLaunch()) {
                    if (!d.applicable.test(uiContext)) continue;
                    if (d.needsAttention == null || !d.needsAttention.test(uiContext)) continue;
                    WizardStep step = d.factory.apply(uiContext);
                    wizardContainer.startSingleStep(step);
                    rootCardLayout.show(rootPanel, "wizard");
                    return;
                }
            });
        });

        JPanel cornerPanel = new JPanel(new FlowLayout(FlowLayout.RIGHT, 5, 0));
        cornerPanel.setOpaque(false);
        cornerPanel.add(connectionStatus);
        cornerPanel.add(launchWizardButton);
        tabbedPane.setCornerComponent(cornerPanel);

        // ---------------

        MainFrame mainFrame = new MainFrame(this, tabbedPane, reuseFrame);
        JFrame frame = mainFrame.getFrame().getFrame();
        setFrameIcon(frame);
        log.info("Console " + UiVersion.CONSOLE_VERSION);

        log.info("Hardware: " + StLinkFlasher.getHardwareKind());

        getConfig().getRoot().setProperty(PORT_KEY, port);
        getConfig().getRoot().setProperty(SPEED_KEY, BaudRateHolder.INSTANCE.baudRate);

        if (!alreadyConnected && !isOffline) {
            // todo: this blocking IO operation should NOT be happening on the UI thread
            linkManager.start(port, mainFrame.listener);
        }

        engineSnifferPanel = new EngineSnifferPanel(uiContext, getConfig().getRoot().getChild("digital_sniffer"));
        if (isOffline || !LinkManager.isLogViewerMode(port))
            engineSnifferPanel.setOutpinListener(uiContext.getLinkManager().getEngineState());

        // what is LogViewer? is this all dead?
//        if (LinkManager.isLogViewerMode(port))
//            tabbedPane.addTab("Log Viewer", new LogViewer(uiContext, engineSnifferPanel));

        uiContext.DetachedRepositoryINSTANCE.init(getConfig().getRoot().getChild("detached"));
        uiContext.DetachedRepositoryINSTANCE.load();
        // Heavy tabs (Lua, Tuning) are lazy-built unless they are the user's last-opened tab, which
        // will be shown immediately on startup — in that case build eagerly (#9715).
        int savedTabIndex = getConfig().getRoot().getIntProperty(TAB_INDEX, DEFAULT_TAB_INDEX);
        if (isOffline || !linkManager.isLogViewer()) {
            tabbedPane.addTab("Gauges", new GaugesPanel(uiContext, getConfig().getRoot().getChild("gauges")).getContent());

            MessagesPane messagesPane = new MessagesPane(uiContext, getConfig().getRoot().getChild("messages"));
            tabbedPaneAdd("Messages", messagesPane.getContent(), messagesPane.getTabSelectedListener());

            int luaIndex = tabbedPane.tabbedPane.getTabCount();
            if (luaIndex == savedTabIndex) {
                LuaScriptPanel luaScriptPanel = new LuaScriptPanel(uiContext, getConfig().getRoot().getChild("lua"));
                tabbedPaneAdd("Lua Scripting", luaScriptPanel.getPanel(), luaScriptPanel.getTabSelectedListener());
            } else {
                // Build the (heavy) Lua editor only when its tab is first shown (#9715).
                tabbedPane.addTab("Lua Scripting", new InitOnFirstPaintPanel() {
                    @Override
                    protected JPanel createContent() {
                        return new LuaScriptPanel(uiContext, getConfig().getRoot().getChild("lua")).getPanel();
                    }
                }.getContent());
            }
        }

        if (UiProperties.isEngineSnifferEnabled()) {
            tabbedPaneAdd("Engine Sniffer", engineSnifferPanel.getPanel(), engineSnifferPanel.getTabSelectedListener());
        }



//        tabbedPane.addTab("LE controls", new FlexibleControls().getPanel());

//        tabbedPane.addTab("ADC", new AdcPanel(new BooleanInputsModel()).createAdcPanel());
//        if (tabbedPane.paneSettings.showStimulatorPane && !LinkManager.isSimulationMode && !LinkManager.isLogViewerMode(port)) {
//            // todo: rethink this UI? special command line key to enable it?
//            EcuStimulator stimulator = EcuStimulator.getInstance();
//            tabbedPane.addTab("ECU stimulation", stimulator.getPanel());
//        }
//        tabbedPane.addTab("live map adjustment", new Live3DReport().getControl());
//        tabbedPane.add("Wizards", new Wizard().createPane());

//        if (!linkManager.isLogViewer())
//            tabbedPane.addTab("Settings", tabbedPane.settingsTab.createPane());
        if (isOffline || !linkManager.isLogViewer()) {
/*
console live data tab is broken #8402

            tabbedPane.addTab("Live Data", LiveDataPane.createLazy(uiContext).getContent());
 */
            PinoutPane pinoutPane = new PinoutPane(uiContext);

            // Tuning is heavy (~60ms) and not always the startup tab. Build it on first use — tab
            // shown, File>Load/Save Tune, or Pinout→Tune nav — unless it's the restored tab (#9715).
            // The holder wires the tune menu + navigation the first time the instance is needed.
            final TuningPane[] tuningHolder = {null};
            final Runnable buildTuning = () -> {
                if (tuningHolder[0] != null) {
                    return;
                }
                TuningPane tp = new TuningPane(uiContext, offlineImage, getConfig().getRoot().getChild("tuning"));
                tuningHolder[0] = tp;
                if (isOffline && offlineImage != null) {
                    tp.seedOfflineImage(offlineImage, null);
                }
                mainFrame.setTuneActions(tp.getLoadTuneAction(), tp.getSaveTuneAction());
                if (UiProperties.isPinoutEnabled()) {
                    tp.setNavigateToPinout(enumValue -> {
                        tabbedPane.selectTab("Pinout");
                        pinoutPane.highlightByEnumValue(enumValue);
                    });
                }
            };

            int tuningIndex = tabbedPane.tabbedPane.getTabCount();
            tabbedPane.addTab("Tuning", new InitOnFirstPaintPanel() {
                @Override
                protected JPanel createContent() {
                    buildTuning.run();
                    JPanel wrapper = new JPanel(new BorderLayout());
                    wrapper.add(tuningHolder[0].getContent(), BorderLayout.CENTER);
                    return wrapper;
                }
            }.getContent());
            // Until Tuning is built, the menu actions build it on first click then delegate; once
            // built, buildTuning re-points the menu at the real actions.
            mainFrame.setTuneActions(
                lazyTuneAction(LoadTuneHelper.LOAD_TUNE_TEXT, buildTuning, () -> tuningHolder[0].getLoadTuneAction()),
                lazyTuneAction(LoadTuneHelper.SAVE_TUNE_TEXT, buildTuning, () -> tuningHolder[0].getSaveTuneAction()));

            tabbedPane.addTab("Knock Analyzer", new KnockPane(uiContext).getContent());
            if (UiProperties.isPinoutEnabled()) {
                tabbedPane.addTab("Pinout", pinoutPane.getContent());
            }
            // Single-session device manager [tag:better_ux_for_flashing]: the scanner is kept alive for the whole console
            // lifetime so this one instance can hook / remove / re-connect / DFU / OpenBLT the board.
            PortResult initialPort = (port != null) ? new PortResult(port, serialPortType) : null;
            ConnectivityContext connectivityContext = ProductionConnectivity.CONTEXT;
            DeviceSessionManager deviceSessionManager = new DeviceSessionManager(connectivityContext, initialPort);
            DevicePane devicePane = new DevicePane(uiContext, connectivityContext, deviceSessionManager, tabbedPane.tabbedPane);
            tabbedPane.addTab("Device", devicePane.getContent());
            mainFrame.setUpdateEcuAction(() -> {
                tabbedPane.selectTab("Device");
                devicePane.triggerAutoUpdate();
            });

            // Pinout ↔ Tune bidirectional navigation
            pinoutPane.setNavigateToTune((dialogKey, fieldKey) -> {
                buildTuning.run();
                tabbedPane.selectTab("Tuning");
                tuningHolder[0].navigateToField(dialogKey, fieldKey);
            });

            // Build eagerly if Tuning was the user's last-opened tab (shown immediately on startup).
            if (tuningIndex == savedTabIndex) {
                buildTuning.run();
            }
        }

        if ((isOffline || !linkManager.isLogViewer()) && false) {
            // todo: fix it & better name?
            tabbedPane.addTab("Logs Manager", tabbedPane.logsManager.getContent());
        }


        MessagesCentral.getInstance().postMessage(ConsoleUI.class, "COMPOSITE_OFF_RPM=" + BinaryProtocolLogger.COMPOSITE_OFF_RPM);

        /*
        https://github.com/rusefi/rusefi/issues/5956
        tabbedPane.addTab("rusEFI Online", new OnlineTab(uiContext).getContent());
*/

        if (false) {
            // this feature is not totally happy safer to disable to reduce user confusion
            // https://github.com/rusefi/rusefi/issues/5292
            uiContext.sensorLogger.init();
        }

        if (isOffline || !LinkManager.isLogViewerMode(port)) {
            if (savedTabIndex < tabbedPane.tabbedPane.getTabCount())
                tabbedPane.tabbedPane.setSelectedIndex(savedTabIndex);
        }

        tabbedPane.tabbedPane.addChangeListener(new ChangeListener() {
            @Override
            public void stateChanged(ChangeEvent e) {
                if (e.getSource() instanceof JTabbedPane) {
                    JTabbedPane pane = (JTabbedPane) e.getSource();
                    int selectedIndex = pane.getSelectedIndex();
                    System.out.println("Selected paneNo: " + selectedIndex);
                    ActionListener actionListener = tabSelectedListeners.get(pane.getComponentAt(selectedIndex));
                    if (actionListener != null)
                        actionListener.actionPerformed(null);
                }
            }
        });

        ShortcutsHelper.installConnectAndDisconnect(uiContext, tabbedPane.tabbedPane);
        AutoupdateUtil.setAppIcon(mainFrame.getFrame().getFrame());

        mainFrame.getFrame().showFrame(rootPanel);
    }

    private @NotNull JButton getLaunchWizardButton(JPanel rootPanel, WizardContainer wizardContainer, CardLayout rootCardLayout) {
        JButton launchWizardButton = new JButton("Launch Wizard");
        launchWizardButton.addActionListener(e -> {
            if (ConnectionStatusLogic.INSTANCE.getValue() != ConnectionStatusValue.CONNECTED) {
                JOptionPane.showMessageDialog(rootPanel,
                    "Please connect to an ECU before launching the wizard.",
                    "Not Connected", JOptionPane.WARNING_MESSAGE);
                return;
            }
            wizardContainer.startWizard();
            rootCardLayout.show(rootPanel, "wizard");
        });
        return launchWizardButton;
    }

    public String getPort() {
        return port;
    }

    private static void writeReadmeFile() {
        LazyFile file = new LazyFileImpl(FileLogger.DIR + "README.html");
        file.write("<center>" + "<a href='" + WIKI_URL + "'>More info online<br/><img src=https://raw.githubusercontent.com/wiki/rusefi/rusefi/logo.gif></a>");
        try {
            file.close();
        } catch (IOException e) {
            // ignoring this one
        }
    }

    static void startUi(String[] args) throws InterruptedException, InvocationTargetException {
        startUi(args, null);
    }

    static void startUi(String[] args, AtomicReference<Consumer<String>> bannerCallback) throws InterruptedException, InvocationTargetException {
        if (ConnectionAndMeta.saveReadmeHtmlToFile()) {
            new Thread(ConsoleUI::writeReadmeFile).start();
        }

        getConfig().load();
        AutotestLogging.suspendLogging = getConfig().getRoot().getBoolProperty(GaugesPanel.DISABLE_LOGS);
        commonUiStartup();
// not very useful?        VersionChecker.start();
        SwingUtilities.invokeAndWait(() -> awtCode(args, bannerCallback));
    }

    /**
     * Adds a tab with activation listener
     */
    private void tabbedPaneAdd(String title, JComponent component, ActionListener tabSelectedListener) {
        tabSelectedListeners.put(component, tabSelectedListener);
        tabbedPane.addTab(title, component);
    }

    private static void awtCode(String[] args, AtomicReference<Consumer<String>> bannerCallback) {
        if (JustOneInstance.isAlreadyRunning()) {
            int result = JOptionPane.showConfirmDialog(createOnTopParent(), "Looks like another instance is already running. Do you really want to start another instance?",
                TITLE, JOptionPane.YES_NO_OPTION);
            if (result == JOptionPane.NO_OPTION) {
                System.exit(-1);
            }
        }
        JustOneInstance.onStart();

        try {
            boolean isPortDefined = args.length > 0;
            boolean isBaudRateDefined = args.length > 1;
            if (isBaudRateDefined)
                BaudRateHolder.INSTANCE.baudRate = Integer.parseInt(args[1]);

            String port = null;
            if (isPortDefined)
                port = args[0];

            if (isPortDefined) {
                port = PortDetector.autoDetectSerialIfNeeded(port);
                if (port == null) {
                    isPortDefined = false;
                }
            }

            if (isPortDefined) {
                new ConsoleUI(port, SerialPortType.Unknown);
            } else {
                for (String p : LinkManager.getCommPorts())
                    MessagesCentral.getInstance().postMessage(Launcher.class, "Available port: " + p);
                StartupFrame startupFrame = new StartupFrame(ProductionConnectivity.CONTEXT, new UIContext());
                if (bannerCallback != null)
                    bannerCallback.set(message -> startupFrame.restartConsole());
                startupFrame.showUi();
            }

        } catch (Throwable e) {
            throw new IllegalStateException(e);
        }
    }
}
