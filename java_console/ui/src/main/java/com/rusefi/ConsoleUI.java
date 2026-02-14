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
import com.rusefi.core.ui.AutoupdateUtil;
import com.rusefi.util.LazyFile;
import com.rusefi.util.LazyFileImpl;


import javax.swing.*;
import javax.swing.event.ChangeEvent;
import javax.swing.event.ChangeListener;
import java.awt.*;
import java.awt.event.ActionListener;
import java.io.IOException;
import java.lang.reflect.InvocationTargetException;
import java.util.HashMap;
import java.util.Map;

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

    public final UIContext uiContext = new UIContext();

    /**
     * We can listen to tab activation event if we so desire
     */
    private final Map<Component, ActionListener> tabSelectedListeners = new HashMap<>();

    public ConsoleUI(String port) {
        LinkManager linkManager = uiContext.getLinkManager();

        CommandQueue.ERROR_HANDLER = e -> SwingUtilities.invokeLater(() -> {
            throw new IllegalStateException("Connectivity error", e);
        });

        ConnectionStatusIcon connectionStatus = new ConnectionStatusIcon(linkManager);

        tabbedPane = new TabbedPanel(uiContext);
        tabbedPane.setCornerComponent(connectionStatus);
        this.port = port;
        MainFrame mainFrame = new MainFrame(this, tabbedPane);
        JFrame frame = mainFrame.getFrame().getFrame();
        setFrameIcon(frame);
        log.info("Console " + UiVersion.CONSOLE_VERSION);

        log.info("Hardware: " + StLinkFlasher.getHardwareKind());

        getConfig().getRoot().setProperty(PORT_KEY, port);
        getConfig().getRoot().setProperty(SPEED_KEY, BaudRateHolder.INSTANCE.baudRate);

        // todo: this blocking IO operation should NOT be happening on the UI thread
        linkManager.start(port, mainFrame.listener);

        engineSnifferPanel = new EngineSnifferPanel(uiContext, getConfig().getRoot().getChild("digital_sniffer"));
        if (!LinkManager.isLogViewerMode(port))
            engineSnifferPanel.setOutpinListener(uiContext.getLinkManager().getEngineState());

        // what is LogViewer? is this all dead?
//        if (LinkManager.isLogViewerMode(port))
//            tabbedPane.addTab("Log Viewer", new LogViewer(uiContext, engineSnifferPanel));

        uiContext.DetachedRepositoryINSTANCE.init(getConfig().getRoot().getChild("detached"));
        uiContext.DetachedRepositoryINSTANCE.load();
        if (!linkManager.isLogViewer())
            tabbedPane.addTab("Gauges", new GaugesPanel(uiContext, getConfig().getRoot().getChild("gauges")).getContent());

        if (!linkManager.isLogViewer()) {
            MessagesPane messagesPane = new MessagesPane(uiContext, getConfig().getRoot().getChild("messages"));
            tabbedPaneAdd("Messages", messagesPane.getContent(), messagesPane.getTabSelectedListener());
        }
        if (!linkManager.isLogViewer()) {
            tabbedPane.addTab("Bench Test", new BenchTestPane(uiContext, getConfig()).getContent());
        }

        if (!linkManager.isLogViewer()) {
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

        mainFrame.getFrame().showFrame(tabbedPane.getContent());
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
        if (ConnectionAndMeta.saveReadmeHtmlToFile()) {
            new Thread(ConsoleUI::writeReadmeFile).start();
        }

        getConfig().load();
        AutotestLogging.suspendLogging = getConfig().getRoot().getBoolProperty(GaugesPanel.DISABLE_LOGS);
        commonUiStartup();
// not very useful?        VersionChecker.start();
        SwingUtilities.invokeAndWait(() -> awtCode(args));
    }

    /**
     * Adds a tab with activation listener
     */
    private void tabbedPaneAdd(String title, JComponent component, ActionListener tabSelectedListener) {
        tabSelectedListeners.put(component, tabSelectedListener);
        tabbedPane.addTab(title, component);
    }

    private static void awtCode(String[] args) {
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
                new ConsoleUI(port);
            } else {
                for (String p : LinkManager.getCommPorts())
                    MessagesCentral.getInstance().postMessage(Launcher.class, "Available port: " + p);
                new StartupFrame(ConnectivityContext.INSTANCE).showUi();
            }

        } catch (Throwable e) {
            throw new IllegalStateException(e);
        }
    }
}
