package com.rusefi;

import com.rusefi.binaryprotocol.BinaryProtocol;
import com.rusefi.binaryprotocol.BinaryProtocolHolder;
import com.rusefi.config.Fields;
import com.rusefi.core.EngineState;
import com.rusefi.core.MessagesCentral;
import com.rusefi.core.Sensor;
import com.rusefi.core.SensorCentral;
import com.rusefi.io.*;
import com.rusefi.io.serial.PortHolder;
import com.rusefi.io.tcp.BinaryProtocolServer;
import com.rusefi.maintenance.VersionChecker;
import com.rusefi.ui.*;
import com.rusefi.ui.engine.EngineSnifferPanel;
import com.rusefi.ui.logview.LogViewer;
import com.rusefi.ui.storage.Node;
import com.rusefi.ui.util.DefaultExceptionHandler;
import com.rusefi.ui.util.FrameHelper;
import com.rusefi.ui.util.JustOneInstance;
import com.rusefi.ui.util.UiUtils;
import jssc.SerialPortList;

import javax.swing.*;
import javax.swing.event.ChangeEvent;
import javax.swing.event.ChangeListener;
import java.awt.*;
import java.awt.event.ActionListener;
import java.util.Arrays;
import java.util.HashMap;
import java.util.Map;
import java.util.TimeZone;
import java.util.concurrent.atomic.AtomicReference;

import static com.rusefi.ui.storage.PersistentConfiguration.getConfig;

/**
 * this is the main entry point of rusEfi ECU console
 * <p/>
 * <p/>
 * 12/25/12
 * (c) Andrey Belomutskiy 2013-2017
 *
 * @see StartupFrame
 * @see EngineSnifferPanel
 */
public class Launcher {
    public static final int CONSOLE_VERSION = 20180130;
    public static final boolean SHOW_STIMULATOR = false;
    private static final String TAB_INDEX = "main_tab";
    protected static final String PORT_KEY = "port";
    protected static final String SPEED_KEY = "speed";
    private final String port;
    // todo: the logic around 'fatalError' could be implemented nicer
    private String fatalError;
    public static EngineSnifferPanel engineSnifferPanel;
    private static SensorCentral.SensorListener wrongVersionListener;

    private final JTabbedPane tabbedPane = new JTabbedPane() {
        @Override
        public void paint(Graphics g) {
            super.paint(g);
            paintStatusText(g);
        }

        private void paintStatusText(Graphics g) {
            Font f = g.getFont();
            g.setFont(new Font(f.getName(), f.getStyle(), f.getSize() * 4));
            Dimension d = getSize();
            String text;
            switch (ConnectionStatus.INSTANCE.getValue()) {
                case NOT_CONNECTED:
                    text = "Not connected";
                    break;
                case LOADING:
                    text = "Loading";
                    break;
                default:
                    text = "";
            }
            if (fatalError != null) {
                text = fatalError;
                g.setColor(Color.red);
            }
            int labelWidth = g.getFontMetrics().stringWidth(text);
            g.drawString(text, (d.width - labelWidth) / 2, d.height / 2);
        }
    };
    public static AtomicReference<String> firmwareVersion = new AtomicReference<>("N/A");

    private static Frame staticFrame;
    private final TableEditorPane tableEditor = new TableEditorPane();
    private final SettingsTab settingsTab = new SettingsTab();
    private final LogDownloader logsManager = new LogDownloader();
    private final FuelTunePane fuelTunePane;

    /**
     * @see StartupFrame
     */
    private FrameHelper mainFrame = new FrameHelper() {
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
    private final Map<JComponent, ActionListener> tabSelectedListeners = new HashMap<JComponent, ActionListener>();

    public Launcher(String port) {
        this.port = port;
        staticFrame = mainFrame.getFrame();
        FileLog.MAIN.logLine("Console " + CONSOLE_VERSION);

        getConfig().getRoot().setProperty(PORT_KEY, port);
        getConfig().getRoot().setProperty(SPEED_KEY, PortHolder.BAUD_RATE);

        LinkManager.start(port);

        MessagesCentral.getInstance().addListener(new MessagesCentral.MessageListener() {
            @Override
            public void onMessage(Class clazz, String message) {
                if (message.startsWith(ConnectionStatus.FATAL_MESSAGE_PREFIX))
                    fatalError = message;
            }
        });

        engineSnifferPanel = new EngineSnifferPanel(getConfig().getRoot().getChild("digital_sniffer"));
        if (!LinkManager.isLogViewerMode(port))
            engineSnifferPanel.setOutpinListener(LinkManager.engineState);

        if (LinkManager.isLogViewerMode(port))
            tabbedPane.add("Log Viewer", new LogViewer(engineSnifferPanel));

        ConnectionWatchdog.start();


        GaugesPanel.DetachedRepository.INSTANCE.init(getConfig().getRoot().getChild("detached"));
        GaugesPanel.DetachedRepository.INSTANCE.load();
        if (!LinkManager.isLogViewer())
            tabbedPane.addTab("Gauges", new GaugesPanel(getConfig().getRoot().getChild("gauges")).getContent());

        if (!LinkManager.isLogViewer())
            tabbedPane.addTab("Formulas", new FormulasPane().getContent());

        tabbedPaneAdd("Engine Sniffer", engineSnifferPanel.getPanel(), engineSnifferPanel.getTabSelectedListener());

        if (!LinkManager.isLogViewer()) {
            SensorSnifferPane sensorSniffer = new SensorSnifferPane(getConfig().getRoot().getChild("sensor_sniffer"));
            tabbedPaneAdd("Sensor Sniffer", sensorSniffer.getPanel(), sensorSniffer.getTabSelectedListener());
        }

//        tabbedPane.addTab("LE controls", new FlexibleControls().getPanel());

//        tabbedPane.addTab("ADC", new AdcPanel(new BooleanInputsModel()).createAdcPanel());
        if (SHOW_STIMULATOR && !LinkManager.isStimulationMode && !LinkManager.isLogViewerMode(port)) {
            // todo: rethink this UI? special command line key to enable it?
            EcuStimulator stimulator = EcuStimulator.getInstance();
            tabbedPane.add("ECU stimulation", stimulator.getPanel());
        }
//        tabbedPane.addTab("live map adjustment", new Live3DReport().getControl());
        if (!LinkManager.isLogViewer()) {
            MessagesPane messagesPane = new MessagesPane(getConfig().getRoot().getChild("messages"));
            tabbedPaneAdd("Messages", messagesPane.getContent(), messagesPane.getTabSelectedListener());
        }
        if (!LinkManager.isLogViewer())
            tabbedPane.addTab("Table Editor", tableEditor);
//        tabbedPane.add("Wizards", new Wizard().createPane());

        if (!LinkManager.isLogViewer())
            tabbedPane.add("Settings", settingsTab.createPane());
        if (!LinkManager.isLogViewer())
            tabbedPane.add("Bench Test", new BenchTestPane().getContent());
        if (!LinkManager.isLogViewer() && false) // todo: fix it & better name?
            tabbedPane.add("Logs Manager", logsManager.getContent());
        fuelTunePane = new FuelTunePane(getConfig().getRoot().getChild("fueltune"));
        if (true)
            tabbedPane.add("Fuel Tune", fuelTunePane.getContent());


        if (!LinkManager.isLogViewer())
            tabbedPane.add("Trigger Shape", new AverageAnglePanel().getPanel());

        if (!LinkManager.isLogViewerMode(port)) {
            int selectedIndex = getConfig().getRoot().getIntProperty(TAB_INDEX, 2);
            if (selectedIndex < tabbedPane.getTabCount())
                tabbedPane.setSelectedIndex(selectedIndex);
        }

        tabbedPane.addChangeListener(new ChangeListener() {
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

        StartupFrame.setAppIcon(mainFrame.getFrame());
        mainFrame.showFrame(tabbedPane);
    }

    private void tabbedPaneAdd(String title, JComponent component, ActionListener tabSelectedListener) {
        tabSelectedListeners.put(component, tabSelectedListener);
        tabbedPane.add(title, component);
    }

    private void windowOpenedHandler() {
        setTitle();
        ConnectionStatus.INSTANCE.addListener(new ConnectionStatus.Listener() {
            @Override
            public void onConnectionStatus(boolean isConnected) {
                setTitle();
                UiUtils.trueRepaint(tabbedPane); // this would repaint status label
                if (ConnectionStatus.INSTANCE.getValue() == ConnectionStatus.Value.CONNECTED) {
                    long unixGmtTime = System.currentTimeMillis() / 1000L;
                    long withOffset = unixGmtTime + TimeZone.getDefault().getOffset(System.currentTimeMillis()) / 1000;
                    CommandQueue.getInstance().write("set date " + withOffset, CommandQueue.DEFAULT_TIMEOUT,
                            InvocationConfirmationListener.VOID, false);
                }
            }
        });

        LinkManager.open(new ConnectionStateListener() {
            @Override
            public void onConnectionFailed() {
            }

            @Override
            public void onConnectionEstablished() {
                tableEditor.showContent();
                settingsTab.showContent();
                logsManager.showContent();
                fuelTunePane.showContent();
                BinaryProtocolServer.start();
            }
        });

        LinkManager.engineState.registerStringValueAction(EngineState.RUS_EFI_VERSION_TAG, new EngineState.ValueCallback<String>() {
            @Override
            public void onUpdate(String firmwareVersion) {
                Launcher.firmwareVersion.set(firmwareVersion);
                SensorLogger.init();
                setTitle();
                VersionChecker.getInstance().onFirmwareVersion(firmwareVersion);
            }
        });
    }

    private void setTitle() {
        String disconnected = ConnectionStatus.INSTANCE.isConnected() ? "" : "DISCONNECTED ";
        mainFrame.getFrame().setTitle(disconnected + "Console " + CONSOLE_VERSION + "; firmware=" + Launcher.firmwareVersion.get() + "@" + port);
    }

    private void windowClosedHandler() {
        /**
         * looks like reconnectTimer in {@link com.rusefi.ui.RpmPanel} keeps AWT alive. Simplest solution would be to 'exit'
         */
        SimulatorHelper.onWindowClosed();
        Node root = getConfig().getRoot();
        root.setProperty("version", CONSOLE_VERSION);
        root.setProperty(TAB_INDEX, tabbedPane.getSelectedIndex());
        GaugesPanel.DetachedRepository.INSTANCE.saveConfig();
        getConfig().save();
        BinaryProtocol bp = BinaryProtocolHolder.getInstance().get();
        if (bp != null && !bp.isClosed)
            bp.close(); // it could be that serial driver wants to be closed explicitly
        System.exit(0);
    }

    public static void main(final String[] args) throws Exception {
        String toolName = args.length == 0 ? null : args[0];
        if ("compile_fsio_file".equalsIgnoreCase(toolName)) {
            /**
             * re-packaging array which contains input and output file names
             */
            int returnCode = CompileTool.run(Arrays.asList(args).subList(1, args.length));
            System.exit(returnCode);
            return;
        }

        if ("compile".equals(toolName)) {
            if (args.length != 2) {
                System.err.println("input expression parameter expected");
                System.exit(-1);
                return;
            }
            String input = args[1];
            System.out.println(CompileTool.handleOneFsioLine(input));
            System.exit(0);
        }


        FileLog.MAIN.start();
        getConfig().load();
        FileLog.suspendLogging = getConfig().getRoot().getBoolProperty(GaugesPanel.DISABLE_LOGS);
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
        wrongVersionListener = new SensorCentral.SensorListener() {
            @Override
            public void onSensorUpdate(double value) {
                if (value != Fields.TS_FILE_VERSION) {
                    String message = "This copy of rusEfi console is not compatible with this version of firmware\r\n" +
                            "Console compatible with " + Fields.TS_FILE_VERSION + " while firmware compatible with " +
                            (int) value;
                    JOptionPane.showMessageDialog(Launcher.getFrame(), message);
                    assert wrongVersionListener != null;
                    SensorCentral.getInstance().removeListener(Sensor.FIRMWARE_VERSION, wrongVersionListener);
                }
            }
        };
        SensorCentral.getInstance().addListener(Sensor.FIRMWARE_VERSION, wrongVersionListener);
        JustOneInstance.onStart();
        try {
            boolean isPortDefined = args.length > 0;
            boolean isBaudRateDefined = args.length > 1;
            if (isBaudRateDefined)
                PortHolder.BAUD_RATE = Integer.parseInt(args[1]);
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
