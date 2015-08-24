package com.rusefi;

import com.rusefi.binaryprotocol.BinaryProtocol;
import com.rusefi.core.EngineState;
import com.rusefi.core.MessagesCentral;
import com.rusefi.io.LinkManager;
import com.rusefi.maintenance.VersionChecker;
import com.rusefi.ui.*;
import com.rusefi.ui.engine.EngineSnifferPanel;
import com.rusefi.ui.logview.LogViewer;
import com.rusefi.ui.storage.Node;
import com.rusefi.ui.util.DefaultExceptionHandler;
import com.rusefi.ui.util.FrameHelper;
import com.rusefi.ui.util.JustOneInstance;
import jssc.SerialPortList;

import javax.swing.*;
import java.awt.*;
import java.util.concurrent.atomic.AtomicReference;

import static com.rusefi.ui.storage.PersistentConfiguration.getConfig;

/**
 * this is the main entry point of rusEfi ECU console
 * <p/>
 * <p/>
 * 12/25/12
 * (c) Andrey Belomutskiy 2013-2015
 *
 * @see StartupFrame
 * @see com.rusefi.ui.engine.EngineSnifferPanel
 * @see com.rusefi.StartupFrame
 */
public class Launcher {
    public static final int CONSOLE_VERSION = 20150823;
    public static final boolean SHOW_STIMULATOR = false;
    private static final String TAB_INDEX = "main_tab";
    protected static final String PORT_KEY = "port";
    private final String port;
    private final JTabbedPane tabbedPane = new JTabbedPane();
    private static AtomicReference<String> firmwareVersion = new AtomicReference<>("N/A");

    public static int defaultFontSize;

    private static Frame staticFrame;
    private final TableEditorPane tableEditor = new TableEditorPane();
    private final SettingsTab settingsTab = new SettingsTab();
    private final LogsManager logsManager = new LogsManager();

    FrameHelper frame = new FrameHelper() {
        @Override
        protected void onWindowOpened() {
            super.onWindowOpened();
            windowOpenedHandler();
        }

        @Override
        protected void onWindowClosed() {
            /**
             * here we would close the port and log a message about it
             */
            windowClosedHandler();
            /**
             * here we would close the log file
             */
            super.onWindowClosed();
        }
    };

    public Launcher(String port) {
        this.port = port;
        staticFrame = frame.getFrame();
        FileLog.MAIN.start();
        FileLog.MAIN.logLine("Console " + CONSOLE_VERSION);

        getConfig().getRoot().setProperty(PORT_KEY, port);

        LinkManager.start(port);

        EngineSnifferPanel engineSnifferPanel = new EngineSnifferPanel(getConfig().getRoot().getChild("digital_sniffer"));
        if (LinkManager.isLogViewerMode(port))
            tabbedPane.add("Log Viewer", new LogViewer(engineSnifferPanel));

        ConnectionWatchdog.start();

        tabbedPane.addTab("Gauges", new GaugesPanel(getConfig().getRoot().getChild("gauges")).getContent());
        tabbedPane.addTab("Engine Sniffer", engineSnifferPanel.getPanel());
        tabbedPane.addTab("Sensor Sniffer", new SensorSnifferPane().getPanel());


//        tabbedPane.addTab("LE controls", new FlexibleControls().getPanel());

//        tabbedPane.addTab("ADC", new AdcPanel(new BooleanInputsModel()).createAdcPanel());
        if (SHOW_STIMULATOR && !LinkManager.isStimulationMode && !LinkManager.isLogViewerMode(port)) {
            // todo: rethink this UI? special command line key to enable it?
            EcuStimulator stimulator = EcuStimulator.getInstance();
            tabbedPane.add("ECU stimulation", stimulator.getPanel());
        }
//        tabbedPane.addTab("live map adjustment", new Live3DReport().getControl());
        tabbedPane.add("Messages", new MessagesPane(getConfig().getRoot().getChild("messages")).getContent());
        tabbedPane.addTab("Table Editor", tableEditor);
//        tabbedPane.add("Wizards", new Wizard().createPane());

        tabbedPane.add("Settings", settingsTab.createPane());
        tabbedPane.add("Bench Test", new BenchTestPane().getContent());
        tabbedPane.add("Logs Manager", logsManager.getContent());

        if (!LinkManager.isLogViewerMode(port)) {
            int selectedIndex = getConfig().getRoot().getIntProperty(TAB_INDEX, 2);
            if (selectedIndex < tabbedPane.getTabCount())
                tabbedPane.setSelectedIndex(selectedIndex);
        }

        StartupFrame.setAppIcon(frame.getFrame());
        frame.showFrame(tabbedPane);
    }

    private void windowOpenedHandler() {
        setTitle();
        ConnectionStatus.INSTANCE.addListener(new ConnectionStatus.Listener() {
            @Override
            public void onConnectionStatus(boolean isConnected) {
                setTitle();
            }
        });

        LinkManager.open(new LinkManager.LinkStateListener() {
            @Override
            public void onConnectionFailed() {
            }

            @Override
            public void onConnectionEstablished() {
                tableEditor.showContent();
                settingsTab.showContent();
                logsManager.showContent();
            }
        });

        LinkManager.engineState.registerStringValueAction(EngineState.RUS_EFI_VERSION_TAG, new EngineState.ValueCallback<String>() {
            @Override
            public void onUpdate(String firmwareVersion) {
                Launcher.firmwareVersion.set(firmwareVersion);
                setTitle();
                VersionChecker.getInstance().onFirmwareVersion(firmwareVersion);
            }
        });
    }

    private void setTitle() {
        String disconnected = ConnectionStatus.INSTANCE.isConnected() ? "" : "DISCONNECTED ";
        frame.getFrame().setTitle(disconnected + "Console " + CONSOLE_VERSION + "; firmware=" + Launcher.firmwareVersion.get() + "@" + port);
    }

    private void windowClosedHandler() {
        /**
         * looks like reconnectTimer in {@link com.rusefi.ui.RpmPanel} keeps AWT alive. Simplest solution would be to 'exit'
         */
        SimulatorHelper.onWindowClosed();
        Node root = getConfig().getRoot();
        root.setProperty("version", CONSOLE_VERSION);
        root.setProperty(TAB_INDEX, tabbedPane.getSelectedIndex());
        getConfig().save();
        BinaryProtocol bp = BinaryProtocol.instance;
        if (bp != null && !bp.isClosed)
            bp.close(); // it could be that serial driver wants to be closed explicitly
        System.exit(0);
    }

    public static void main(final String[] args) throws Exception {
        getConfig().load();
        Thread.setDefaultUncaughtExceptionHandler(new DefaultExceptionHandler());
        VersionChecker.start();
        SwingUtilities.invokeAndWait(new Runnable() {
            public void run() {
                awtCode(args);
            }
        });
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
            if (isPortDefined) {
                new Launcher(args[0]);
            } else {
                for (String p : SerialPortList.getPortNames())
                    MessagesCentral.getInstance().postMessage(Launcher.class, "Available port: " + p);
                new StartupFrame().chooseSerialPort();
            }

        } catch (Throwable e) {
            throw new IllegalStateException(e);
        }
    }

    public static Frame getFrame() {
        return staticFrame;
    }
}
