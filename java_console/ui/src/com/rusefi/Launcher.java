package com.rusefi;

import com.fathzer.soft.javaluator.DoubleEvaluator;
import com.rusefi.autodetect.PortDetector;
import com.rusefi.binaryprotocol.BinaryProtocol;
import com.rusefi.config.generated.Fields;
import com.rusefi.core.MessagesCentral;
import com.rusefi.core.Sensor;
import com.rusefi.core.SensorCentral;
import com.rusefi.io.*;
import com.rusefi.io.serial.PortHolder;
import com.rusefi.maintenance.FirmwareFlasher;
import com.rusefi.maintenance.VersionChecker;
import com.rusefi.ui.*;
import com.rusefi.ui.console.MainFrame;
import com.rusefi.ui.console.TabbedPanel;
import com.rusefi.ui.engine.EngineSnifferPanel;
import com.rusefi.ui.logview.LogViewer;
import com.rusefi.ui.util.DefaultExceptionHandler;
import com.rusefi.ui.util.JustOneInstance;
import jssc.SerialPortList;

import javax.swing.*;
import javax.swing.event.ChangeEvent;
import javax.swing.event.ChangeListener;
import java.awt.*;
import java.awt.event.ActionListener;
import java.io.IOException;
import java.util.Arrays;
import java.util.HashMap;
import java.util.Map;
import java.util.concurrent.atomic.AtomicReference;

import static com.rusefi.ui.storage.PersistentConfiguration.getConfig;

/**
 * this is the main entry point of rusEfi ECU console
 * <p/>
 * <p/>
 * 12/25/12
 * (c) Andrey Belomutskiy 2013-2019
 *
 * @see StartupFrame
 * @see EngineSnifferPanel
 */
public class Launcher {
    public static final int CONSOLE_VERSION = 20200502;
    public static final String INI_FILE_PATH = System.getProperty("ini_file_path", "..");
    public static final String INPUT_FILES_PATH = System.getProperty("input_files_path", "..");
    public static final String TOOLS_PATH = System.getProperty("tools_path", ".");
    public static final String TAB_INDEX = "main_tab";
    protected static final String PORT_KEY = "port";
    protected static final String SPEED_KEY = "speed";

    private static final String TOOL_NAME_COMPILE_FSIO_FILE = "compile_fsio_file";
    private static final String TOOL_NAME_REBOOT_ECU = "reboot_ecu";
    private static final String TOOL_NAME_FIRING_ORDER = "firing_order";
    private static final String TOOL_NAME_FUNCTIONAL_TEST = "functional_test";
    private static final String TOOL_NAME_PERF_ENUMS = "ptrace_enums";
    // todo: rename to something more FSIO-specific? would need to update documentation somewhere
    private static final String TOOL_NAME_COMPILE = "compile";
    private static final int DEFAULT_TAB_INDEX = 0;

    public static String port;
    public static EngineSnifferPanel engineSnifferPanel;
    private static SensorCentral.SensorListener wrongVersionListener;

    public TabbedPanel tabbedPane = new TabbedPanel();

    public static AtomicReference<String> firmwareVersion = new AtomicReference<>("N/A");

    private static Frame staticFrame;

    MainFrame mainFrame = new MainFrame(tabbedPane);

    /**
     * We can listen to tab activation event if we so desire
     */
    private final Map<JComponent, ActionListener> tabSelectedListeners = new HashMap<>();

    public Launcher(String port) {
        Launcher.port = port;
        staticFrame = mainFrame.getFrame().getFrame();
        FileLog.MAIN.logLine("Console " + CONSOLE_VERSION);

        FileLog.MAIN.logLine("Hardware: " + FirmwareFlasher.getHardwareKind());

        getConfig().getRoot().setProperty(PORT_KEY, port);
        getConfig().getRoot().setProperty(SPEED_KEY, PortHolder.BAUD_RATE);

        LinkManager.start(port);

        engineSnifferPanel = new EngineSnifferPanel(getConfig().getRoot().getChild("digital_sniffer"));
        if (!LinkManager.isLogViewerMode(port))
            engineSnifferPanel.setOutpinListener(LinkManager.engineState);

        if (LinkManager.isLogViewerMode(port))
            tabbedPane.addTab("Log Viewer", new LogViewer(engineSnifferPanel));

        ConnectionWatchdog.start();


        GaugesPanel.DetachedRepository.INSTANCE.init(getConfig().getRoot().getChild("detached"));
        GaugesPanel.DetachedRepository.INSTANCE.load();
        if (!LinkManager.isLogViewer())
            tabbedPane.addTab("Gauges", new GaugesPanel(getConfig().getRoot().getChild("gauges"), tabbedPane.paneSettings).getContent());

        if (!LinkManager.isLogViewer()) {
            MessagesPane messagesPane = new MessagesPane(getConfig().getRoot().getChild("messages"));
            tabbedPaneAdd("Messages", messagesPane.getContent(), messagesPane.getTabSelectedListener());
        }
        if (!LinkManager.isLogViewer()) {
            tabbedPane.addTab("Bench Test", new BenchTestPane().getContent());
            if (tabbedPane.paneSettings.showEtbPane)
                tabbedPane.addTab("ETB", new ETBPane().getContent());
            tabbedPane.addTab("Presets", new PresetsPane().getContent());
        }

        tabbedPaneAdd("Engine Sniffer", engineSnifferPanel.getPanel(), engineSnifferPanel.getTabSelectedListener());

        if (!LinkManager.isLogViewer()) {
            SensorSnifferPane sensorSniffer = new SensorSnifferPane(getConfig().getRoot().getChild("sensor_sniffer"));
            tabbedPaneAdd("Sensor Sniffer", sensorSniffer.getPanel(), sensorSniffer.getTabSelectedListener());
        }

//        tabbedPane.addTab("LE controls", new FlexibleControls().getPanel());

//        tabbedPane.addTab("ADC", new AdcPanel(new BooleanInputsModel()).createAdcPanel());
        if (tabbedPane.paneSettings.showStimulatorPane && !LinkManager.isSimulationMode && !LinkManager.isLogViewerMode(port)) {
            // todo: rethink this UI? special command line key to enable it?
            EcuStimulator stimulator = EcuStimulator.getInstance();
            tabbedPane.addTab("ECU stimulation", stimulator.getPanel());
        }
//        tabbedPane.addTab("live map adjustment", new Live3DReport().getControl());
        if (!LinkManager.isLogViewer())
            tabbedPane.addTab("Table Editor", tabbedPane.tableEditor);
//        tabbedPane.add("Wizards", new Wizard().createPane());

        if (!LinkManager.isLogViewer())
            tabbedPane.addTab("Settings", tabbedPane.settingsTab.createPane());
        if (!LinkManager.isLogViewer()) {
            tabbedPane.addTab("Formulas/Live Data", new FormulasPane().getContent());
            tabbedPane.addTab("Sensors Live Data", new SensorsLiveDataPane().getContent());
        }

        if (!LinkManager.isLogViewer() && false) // todo: fix it & better name?
            tabbedPane.addTab("Logs Manager", tabbedPane.logsManager.getContent());
        if (tabbedPane.paneSettings.showFuelTunePane)
            tabbedPane.addTab("Fuel Tune", tabbedPane.fuelTunePane.getContent());


        if (!LinkManager.isLogViewer()) {
            if (tabbedPane.paneSettings.showTriggerShapePane)
                tabbedPane.addTab("Trigger Shape", new AverageAnglePanel().getPanel());
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

        StartupFrame.setAppIcon(mainFrame.getFrame().getFrame());
        mainFrame.getFrame().showFrame(tabbedPane.tabbedPane);
    }

    /**
     * Adds a tab with activation listener
     */
    private void tabbedPaneAdd(String title, JComponent component, ActionListener tabSelectedListener) {
        tabSelectedListeners.put(component, tabSelectedListener);
        tabbedPane.addTab(title, component);
    }

    /**
     * rusEfi console entry point
     * @see StartupFrame if no parameters specified
     */
    public static void main(final String[] args) throws Exception {
        String toolName = args.length == 0 ? null : args[0];

        if (TOOL_NAME_FUNCTIONAL_TEST.equals(toolName)) {
            // passing port argument if it was specified
            String[] toolArgs = args.length == 1 ? new String[0] : new String[]{args[1]};
            RealHwTest.main(toolArgs);
            return;
        }

        if (TOOL_NAME_COMPILE_FSIO_FILE.equalsIgnoreCase(toolName)) {
            int returnCode = invokeCompileFileTool(args);
            System.exit(returnCode);
        }

        if (TOOL_NAME_COMPILE.equals(toolName)) {
            invokeCompileExpressionTool(args);
            System.exit(0);
        }

        if (TOOL_NAME_FIRING_ORDER.equals(toolName)) {
            FiringOrderTSLogic.invoke(args[1]);
            System.exit(0);
        }

        if (TOOL_NAME_PERF_ENUMS.equals(toolName)) {
            PerfTraceTool.readPerfTrace(args[1], args[2], args[3], args[4]);
            System.exit(0);
        }

        System.out.println("Optional tools: " + Arrays.asList(TOOL_NAME_COMPILE_FSIO_FILE,
                TOOL_NAME_COMPILE,
                TOOL_NAME_REBOOT_ECU,
                TOOL_NAME_FIRING_ORDER));
        System.out.println("Starting rusEfi UI console " + CONSOLE_VERSION);

        FileLog.MAIN.start();

        if (TOOL_NAME_REBOOT_ECU.equalsIgnoreCase(toolName)) {
            invokeRebootTool();
            return;
        }


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

    private static int invokeCompileFileTool(String[] args) throws IOException {
        /**
         * re-packaging array which contains input and output file names
         */
        return CompileTool.run(Arrays.asList(args).subList(1, args.length));
    }

    private static void invokeRebootTool() throws IOException {
        String autoDetectedPort = PortDetector.autoDetectPort(null);
        if (autoDetectedPort == null) {
            System.err.println("rusEfi not detected");
            return;
        }
        PortHolder.EstablishConnection establishConnection = new PortHolder.EstablishConnection(autoDetectedPort).invoke();
        if (!establishConnection.isConnected())
            return;
        IoStream stream = establishConnection.getStream();
        byte[] commandBytes = BinaryProtocol.getTextCommandBytes(Fields.CMD_REBOOT);
        stream.sendPacket(commandBytes, FileLog.LOGGER);
    }

    private static void invokeCompileExpressionTool(String[] args) {
        if (args.length != 2) {
            System.err.println("input expression parameter expected");
            System.exit(-1);
        }
        String expression = args[1];
        System.out.println(DoubleEvaluator.process(expression).getPosftfixExpression());
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
                // todo: we need to migrate to TS_SIGNATURE validation!!!
                if (value != Fields.TS_FILE_VERSION) {
                    String message = "This copy of rusEfi console is not compatible with this version of firmware\r\n" +
                            "Console compatible with " + Fields.TS_FILE_VERSION + " while firmware compatible with " +
                            (int) value;
                    JOptionPane.showMessageDialog(Launcher.getFrame(), message);
                    assert wrongVersionListener != null;
                    SensorCentral.getInstance().removeListener(Sensor.TS_CONFIG_VERSION, wrongVersionListener);
                }
            }
        };
        SensorCentral.getInstance().addListener(Sensor.TS_CONFIG_VERSION, wrongVersionListener);
        JustOneInstance.onStart();
        try {
            boolean isPortDefined = args.length > 0;
            boolean isBaudRateDefined = args.length > 1;
            if (isBaudRateDefined)
                PortHolder.BAUD_RATE = Integer.parseInt(args[1]);

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
                new Launcher(port);
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
