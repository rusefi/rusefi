package com.rusefi;

import com.devexperts.logging.Logging;
import com.rusefi.autodetect.PortDetector;
import com.rusefi.core.MessagesCentral;
import com.rusefi.io.CommandQueue;
import com.rusefi.io.LinkManager;
import com.rusefi.io.serial.BaudRateHolder;
import com.rusefi.ui.*;
import com.rusefi.ui.console.MainFrame;
import com.rusefi.ui.console.TabbedPanel;
import com.rusefi.ui.engine.EngineSnifferPanel;
import com.rusefi.ui.lua.LuaScriptPanel;
import com.rusefi.ui.util.DefaultExceptionHandler;
import com.rusefi.ui.util.JustOneInstance;
import com.rusefi.ui.util.UiUtils;

import javax.swing.*;
import javax.swing.event.ChangeEvent;
import javax.swing.event.ChangeListener;
import java.awt.*;
import java.awt.event.ActionListener;
import java.lang.reflect.InvocationTargetException;
import java.util.HashMap;
import java.util.Map;

import static com.devexperts.logging.Logging.getLogging;
import static com.rusefi.StartupFrame.setFrameIcon;
import static com.rusefi.core.preferences.storage.PersistentConfiguration.getConfig;
import static com.rusefi.core.rusEFIVersion.CONSOLE_VERSION;

/**
 * @see StartupFrame
 */
public class ConsoleUI {
    private static final Logging log = getLogging(ConsoleUI.class);
    private static final int DEFAULT_TAB_INDEX = 0;

    public static final String TAB_INDEX = "main_tab";
    protected static final String PORT_KEY = "port";
    protected static final String SPEED_KEY = "speed";
    public static EngineSnifferPanel engineSnifferPanel;

    private final TabbedPanel tabbedPane;
    private final String port;

    public final UIContext uiContext = new UIContext();

    /**
     * We can listen to tab activation event if we so desire
     */
    private final Map<Component, ActionListener> tabSelectedListeners = new HashMap<>();

    public ConsoleUI(String port) {
        CommandQueue.ERROR_HANDLER = e -> SwingUtilities.invokeLater(() -> {
            throw new IllegalStateException("Connectivity error", e);
        });

        log.info("init...");
        tabbedPane = new TabbedPanel(uiContext);
        this.port = port;
        MainFrame mainFrame = new MainFrame(this, tabbedPane);
        setFrameIcon(mainFrame.getFrame().getFrame());
        log.info("FOME EFI Console " + CONSOLE_VERSION);

        getConfig().getRoot().setProperty(PORT_KEY, port);
        getConfig().getRoot().setProperty(SPEED_KEY, BaudRateHolder.INSTANCE.baudRate);

        LinkManager linkManager = uiContext.getLinkManager();
        // todo: this blocking IO operation should NOT be happening on the UI thread
        linkManager.start(port, mainFrame.listener);

        engineSnifferPanel = new EngineSnifferPanel(uiContext, getConfig().getRoot().getChild("digital_sniffer"));

        engineSnifferPanel.setOutpinListener(uiContext.getLinkManager().getEngineState());

        MessagesPane messagesPane = new MessagesPane(uiContext, getConfig().getRoot().getChild("messages"));
        tabbedPaneAdd("Messages", messagesPane.getContent(), messagesPane.getTabSelectedListener());

        tabbedPane.addTab("Bench Test", new BenchTestPane(uiContext, getConfig()).getContent());

        LuaScriptPanel luaScriptPanel = new LuaScriptPanel(uiContext, getConfig().getRoot().getChild("lua"));
        tabbedPaneAdd("Lua Scripting", luaScriptPanel.getPanel(), luaScriptPanel.getTabSelectedListener());

        tabbedPaneAdd("Engine Sniffer", engineSnifferPanel.getPanel(), engineSnifferPanel.getTabSelectedListener());

        SensorSnifferPane sensorSniffer = new SensorSnifferPane(uiContext, getConfig().getRoot().getChild("sensor_sniffer"));
        tabbedPaneAdd("Sensor Sniffer", sensorSniffer.getPanel(), sensorSniffer.getTabSelectedListener());

//        if (tabbedPane.paneSettings.showStimulatorPane && !LinkManager.isSimulationMode && !LinkManager.isLogViewerMode(port)) {
//            // todo: rethink this UI? special command line key to enable it?
//            EcuStimulator stimulator = EcuStimulator.getInstance();
//            tabbedPane.addTab("ECU stimulation", stimulator.getPanel());
//        }

        if (tabbedPane.paneSettings.showTriggerShapePane)
            tabbedPane.addTab("Trigger Shape", new AverageAnglePanel(uiContext).getPanel());

        int selectedIndex = getConfig().getRoot().getIntProperty(TAB_INDEX, DEFAULT_TAB_INDEX);
        if (selectedIndex < tabbedPane.tabbedPane.getTabCount())
            tabbedPane.tabbedPane.setSelectedIndex(selectedIndex);

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

        UiUtils.setAppIcon(mainFrame.getFrame().getFrame());
        log.info("showFrame");
        mainFrame.getFrame().showFrame(tabbedPane.tabbedPane);
    }

    public String getPort() {
        return port;
    }

    static void startUi(String[] args) throws InterruptedException, InvocationTargetException {
        FileLog.MAIN.start();
        log.info("OS name: " + FileLog.getOsName());
        log.info("OS version: " + System.getProperty(FileLog.OS_VERSION));

        getConfig().load();
        Thread.setDefaultUncaughtExceptionHandler(new DefaultExceptionHandler());
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
            int result = JOptionPane.showConfirmDialog(null, "Looks like another instance is already running. Do you really want to start another instance?",
                    "rusEfi", JOptionPane.YES_NO_OPTION);
            if (result == JOptionPane.NO_OPTION)
                System.exit(-1);
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
                new StartupFrame().chooseSerialPort();
            }

        } catch (Throwable e) {
            throw new IllegalStateException(e);
        }
    }
}
