package com.rusefi;

import com.irnems.FileLog;
import com.irnems.core.EngineState;
import com.irnems.core.MessagesCentral;
import com.rusefi.io.LinkManager;
import com.rusefi.ui.*;
import com.rusefi.ui.storage.Node;
import jssc.SerialPortList;

import javax.swing.*;

import static com.rusefi.ui.storage.PersistentConfiguration.getConfig;

/**
 * this is the main entry point of rusEfi ECU console
 * <p/>
 * <p/>
 * 12/25/12
 * (c) Andrey Belomutskiy 2013-2015
 *
 * @see StartupFrame
 * @see WavePanel
 */
public class Launcher extends FrameHelper {
    public static final int CONSOLE_VERSION = 20150216;
    public static final boolean SHOW_STIMULATOR = true;
    public static final String TAB_INDEX = "main_tab";
    private final String port;
    private final JTabbedPane tabbedPane = new JTabbedPane();

    public static int defaultFontSize;

    public Launcher(String port) {
        this.port = port;
        FileLog.MAIN.start();

        LinkManager.start(port);

        FileLog.MAIN.logLine("Console " + CONSOLE_VERSION);


        WavePanel wavePanel = new WavePanel(getConfig().getRoot().getChild("digital_sniffer"));
        if (LinkManager.isLogViewerMode(port))
            tabbedPane.add("Log Viewer", new LogViewer(wavePanel));

        RpmPanel mainGauges = new RpmPanel(getConfig().getRoot().getChild("main_gauges"));
        tabbedPane.addTab("Main", mainGauges.createRpmPanel());
        tabbedPane.addTab("Gauges", new GaugesPanel().getContent());
        tabbedPane.addTab("Engine Sniffer", wavePanel.getPanel());
        tabbedPane.addTab("Analog Sniffer", new AnalogChartPanel().getPanel());

        tabbedPane.addTab("LE controls", new FlexibleControls().getPanel());

//        tabbedPane.addTab("ADC", new AdcPanel(new BooleanInputsModel()).createAdcPanel());
        if (SHOW_STIMULATOR && !LinkManager.isStimulationMode && !LinkManager.isLogViewerMode(port)) {
            EcuStimulator stimulator = EcuStimulator.getInstance();
            tabbedPane.add("ECU stimulation", stimulator.getPanel());
        }
//        tabbedPane.addTab("live map adjustment", new Live3DReport().getControl());
        tabbedPane.add("Messages", new MessagesPane(getConfig().getRoot().getChild("messages")).getContent());
        tabbedPane.add("Wizards", new Wizard().createPane());


        if (!LinkManager.isLogViewerMode(port)) {
            int selectedIndex = getConfig().getRoot().getIntProperty("main_tab", 2);
            tabbedPane.setSelectedIndex(selectedIndex);
        }

        StartupFrame.setAppIcon(frame);
        showFrame(tabbedPane);
    }

    @Override
    protected void onWindowOpened() {
        super.onWindowOpened();
        setTitle("N/A");

        LinkManager.open();

        LinkManager.engineState.registerStringValueAction("rusEfiVersion", new EngineState.ValueCallback<String>() {
            @Override
            public void onUpdate(String firmwareVersion) {
                setTitle(firmwareVersion);
                VersionChecker.getInstance().onFirmwareVersion(firmwareVersion);
            }
        });
    }

    private void setTitle(String value) {
        frame.setTitle("Console " + CONSOLE_VERSION + "; firmware=" + value + "@" + port);
    }

    @Override
    protected void onWindowClosed() {
        super.onWindowClosed();
        /**
         * looks like reconnectTimer in {@link RpmPanel} keeps AWT alive. Simplest solution would be to 'exit'
         */
        SimulatorHelper.onWindowClosed();
        Node root = getConfig().getRoot();
        root.setProperty("version", CONSOLE_VERSION);
        root.setProperty(TAB_INDEX, tabbedPane.getSelectedIndex());
        getConfig().save();
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
}
