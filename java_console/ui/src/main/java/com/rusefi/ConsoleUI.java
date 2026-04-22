package com.rusefi;

import com.devexperts.logging.FileLogger;
import com.devexperts.logging.Logging;
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
import java.awt.*;
import java.awt.event.ActionListener;
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
        this(new UIContext(), port, serialPortType, false);
    }

    public ConsoleUI(UIContext uiContext, String port, SerialPortType serialPortType, boolean alreadyConnected) {
        this.uiContext = uiContext;
        LinkManager linkManager = uiContext.getLinkManager();

        CommandQueue.ERROR_HANDLER = e -> {
            log.error("Connectivity error", e);
            linkManager.restart();
        };

        ConnectionStatusIcon connectionStatus = new ConnectionStatusIcon(linkManager);

        tabbedPane = new TabbedPanel(uiContext);
        this.port = port;

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

        MainFrame mainFrame = new MainFrame(this, tabbedPane);
        JFrame frame = mainFrame.getFrame().getFrame();
        setFrameIcon(frame);
        log.info("Console " + UiVersion.CONSOLE_VERSION);

        log.info("Hardware: " + StLinkFlasher.getHardwareKind());

        getConfig().getRoot().setProperty(PORT_KEY, port);
        getConfig().getRoot().setProperty(SPEED_KEY, BaudRateHolder.INSTANCE.baudRate);

        if (!alreadyConnected) {
            // todo: this blocking IO operation should NOT be happening on the UI thread
            linkManager.start(port, mainFrame.listener);
        }

        engineSnifferPanel = new EngineSnifferPanel(uiContext, getConfig().getRoot().getChild("digital_sniffer"));
        if (!LinkManager.isLogViewerMode(port))
            engineSnifferPanel.setOutpinListener(uiContext.getLinkManager().getEngineState());

        // what is LogViewer? is this all dead?
//        if (LinkManager.isLogViewerMode(port))
//            tabbedPane.addTab("Log Viewer", new LogViewer(uiContext, engineSnifferPanel));

        uiContext.DetachedRepositoryINSTANCE.init(getConfig().getRoot().getChild("detached"));
        uiContext.DetachedRepositoryINSTANCE.load();
        if (!linkManager.isLogViewer()) {
            tabbedPane.addTab("Gauges", new GaugesPanel(uiContext, getConfig().getRoot().getChild("gauges")).getContent());

            MessagesPane messagesPane = new MessagesPane(uiContext, getConfig().getRoot().getChild("messages"));
            tabbedPaneAdd("Messages", messagesPane.getContent(), messagesPane.getTabSelectedListener());

            LuaScriptPanel luaScriptPanel = new LuaScriptPanel(uiContext, getConfig().getRoot().getChild("lua"));
            tabbedPaneAdd("Lua Scripting", luaScriptPanel.getPanel(), luaScriptPanel.getTabSelectedListener());
        }

        tabbedPaneAdd("Engine Sniffer", engineSnifferPanel.getPanel(), engineSnifferPanel.getTabSelectedListener());



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
        if (!linkManager.isLogViewer()) {
/*
console live data tab is broken #8402

            tabbedPane.addTab("Live Data", LiveDataPane.createLazy(uiContext).getContent());
 */
            tabbedPane.addTab("Tuning", new TuningPane(uiContext).getContent());
            tabbedPane.addTab("Knock Analyzer", new KnockPane(uiContext).getContent());
            tabbedPane.addTab("Pinout", new PinoutPane(uiContext).getContent());
            tabbedPane.addTab("Device", new DevicePane(uiContext, port, serialPortType, tabbedPane.tabbedPane).getContent());
        }

        if (!linkManager.isLogViewer() && false) // todo: fix it & better name?
            tabbedPane.addTab("Logs Manager", tabbedPane.logsManager.getContent());


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

        if (!LinkManager.isLogViewerMode(port)) {
            int selectedIndex = getConfig().getRoot().getIntProperty(TAB_INDEX, DEFAULT_TAB_INDEX);
            if (selectedIndex < tabbedPane.tabbedPane.getTabCount())
                tabbedPane.tabbedPane.setSelectedIndex(selectedIndex);
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
        log.info("showFrame");

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
                StartupFrame startupFrame = new StartupFrame(ConnectivityContext.INSTANCE, new UIContext());
                if (bannerCallback != null)
                    bannerCallback.set(startupFrame::showUpdateBanner);
                startupFrame.showUi();
            }

        } catch (Throwable e) {
            throw new IllegalStateException(e);
        }
    }
}
