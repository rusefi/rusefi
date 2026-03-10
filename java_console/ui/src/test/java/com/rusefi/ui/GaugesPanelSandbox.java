package com.rusefi.ui;

import com.devexperts.logging.Logging;
import com.opensr5.ini.*;
import com.opensr5.ini.field.IniField;
import com.rusefi.NamedThreadFactory;
import com.rusefi.config.Field;
import com.rusefi.core.SensorCentral;
import com.rusefi.core.preferences.storage.Node;

import javax.swing.*;
import java.awt.*;
import java.awt.event.WindowAdapter;
import java.awt.event.WindowEvent;
import java.util.*;
import java.util.List;
import java.util.concurrent.Executors;
import java.util.concurrent.ScheduledExecutorService;
import java.util.concurrent.ThreadLocalRandom;
import java.util.concurrent.TimeUnit;

import static com.devexperts.logging.Logging.getLogging;

public class GaugesPanelSandbox {

    private static final String WINDOW_TITLE = "RusEFI Gauges Sandbox";
    private static final Dimension WINDOW_SIZE = new Dimension(1280, 720);
    private static final int SIMULATION_DELAY_IN_MILLISECONDS = 100;
    private static final int GAUGES_ROWS = 3;
    private static final int GAUGES_COLUMNS = 3;
    private static final Logging log = getLogging(GaugesPanelSandbox.class);

    private enum GaugeDefinition {
        // Positioned according to DEFAULT_LAYOUT in GaugesPanel.java
        RPM("RPMGauge", "RPMValue", "RPM", "", 0, 8000),
        MCU("internalMcuTemperatureGauge", "MCU_TEMP", "MCU", "C", 0, 100),
        CLT("CLTGauge", "CLT_CHANNEL", "CLT", "C", 0, 120),
        IAT("IATGauge", "IAT_CHANNEL", "IAT", "C", 0, 120),
        TPS("TPSGauge", "TPS_CHANNEL", "TPS", "%", 0, 100),
        MAP("MAPGauge", "MAP_CHANNEL", "MAP", "kPa", 0, 300),
        ERR("lastErrorCodeGauge", "ERROR_CODE", "Error", "ID", 0, 255),
        BATT("VBattGauge", "BATT_CHANNEL", "VBatt", "V", 0, 18),
        VSS("VSSGauge", "VSS_CHANNEL", "VSS", "km/h", 0, 200);

        final String gaugeId;
        final String sensorName;
        final String title;
        final String units;
        final double min;
        final double max;

        GaugeDefinition(String gaugeId, String sensorName, String title, String units, double min, double max) {
            this.gaugeId = gaugeId;
            this.sensorName = sensorName;
            this.title = title;
            this.units = units;
            this.min = min;
            this.max = max;
        }
    }

    private final ScheduledExecutorService scheduler = Executors.newSingleThreadScheduledExecutor(
            new NamedThreadFactory("GaugesSandbox-Data-Service", true)
    );

    public static void main(String[] args) {
        new GaugesPanelSandbox().start();
    }

    public void start() {
        SwingUtilities.invokeLater(this::initApplication);
    }

    private void initApplication() {
        UIContext uiContext = new UIContext();
        GaugesPanel.IS_PAUSED = false;

        setupMocking(uiContext);

        JFrame mainWindow = createMainWindow();
        Node layoutNode = createGridConfig();

        GaugesPanel panel = new GaugesPanel(uiContext, layoutNode);
        mainWindow.add(panel.getContent(), BorderLayout.CENTER);
        mainWindow.setVisible(true);

        launchSimulation();
    }

    private Node createGridConfig() {
        Node root = new Node();

        root.setProperty("gauges_rows", GAUGES_ROWS);
        root.setProperty("gauges_cols", GAUGES_COLUMNS);

        return root;
    }

    private void setupMocking(UIContext uiContext) {
        StubIniFileModel stub = new StubIniFileModel();

        for (GaugeDefinition gaugeDefinition : GaugeDefinition.values()) {
            stub.register(new GaugeModel(
                    gaugeDefinition.gaugeId,
                    gaugeDefinition.sensorName,
                    IniValue.ofString(gaugeDefinition.title),
                    IniValue.ofString(gaugeDefinition.units),
                    IniValue.ofNumeric(gaugeDefinition.min), // 5: Min Data
                    IniValue.ofNumeric(gaugeDefinition.max), // 6: Max Data
                    IniValue.ofNumeric(gaugeDefinition.min), // 7: Low Display (Here is where negatives values come in)
                    IniValue.ofNumeric(gaugeDefinition.min + 10), // 8: Low Warning (A little above min)
                    IniValue.ofNumeric(gaugeDefinition.max - 10), // 9: High Warning
                    IniValue.ofNumeric(gaugeDefinition.max), // 10: High Display
                    IniValue.ofNumeric(1), // 11: Decimals (Helps to see the "-" in the text)
                    IniValue.ofNumeric(0)  // 12: Offset/Row
            ));
        }

        uiContext.iniFileState.setIniFileModelForTest(stub);
    }

    private void launchSimulation() {
        scheduler.scheduleAtFixedRate(this::tick, 0, SIMULATION_DELAY_IN_MILLISECONDS, TimeUnit.MILLISECONDS);
    }

    private void tick() {
        try {
            if (GaugesPanel.IS_PAUSED) return;

            SensorCentral sensorCentral = SensorCentral.getInstance();
            ThreadLocalRandom safeRandom = ThreadLocalRandom.current();

            for (GaugeDefinition gaugeDefinition : GaugeDefinition.values()) {
                double value = safeRandom.nextDouble(gaugeDefinition.min, gaugeDefinition.max);
                sensorCentral.setValue(value, gaugeDefinition.sensorName);
            }

        } catch (Exception exception) {
            log.error("Simulation error: " + exception.getMessage());
        }
    }

    private JFrame createMainWindow() {
        JFrame mainWindow = new JFrame(WINDOW_TITLE);

        mainWindow.addWindowListener(new WindowAdapter() {
            @Override
            public void windowClosing(WindowEvent e) {
                log.info("Shutting down simulation service...");
                scheduler.shutdown();
            }
        });
        mainWindow.setSize(WINDOW_SIZE);
        mainWindow.setDefaultCloseOperation(WindowConstants.EXIT_ON_CLOSE);
        mainWindow.setLocationRelativeTo(null);
        return mainWindow;
    }

    private static class StubIniFileModel implements IniFileModel {
        private final Map<String, GaugeModel> gauges = new HashMap<>();
        public void register(GaugeModel g) { gauges.put(g.getName(), g); }
        @Override public Map<String, GaugeModel> getGauges() { return gauges; }
        @Override public GaugeModel getGauge(String name) { return gauges.get(name); }

        // --- Interface scaffolding ---
        @Override public String getSignature() { return "PRO_STUB"; }
        @Override public int getBlockingFactor() { return 128; }
        @Override public Map<String, List<String>> getDefines() { return Collections.emptyMap(); }
        @Override public Map<String, IniField> getAllIniFields() { return Collections.emptyMap(); }
        @Override public Map<String, IniField> getSecondaryIniFields() { return Collections.emptyMap(); }
        @Override public Optional<IniField> findIniField(String k) { return Optional.empty(); }
        @Override public IniField getIniField(Field f) { return null; }
        @Override public IniField getIniField(String k) { return null; }
        @Override public IniField getOutputChannel(String k) { return null; }
        @Override public String getExpressionOutputChannel(String k) { return null; }
        @Override public Map<String, String> getExpressionOutputChannels() { return Collections.emptyMap(); }
        @Override public Map<String, String> getProtocolMeta() { return Collections.emptyMap(); }
        @Override public com.opensr5.ini.IniFileMetaInfo getMetaInfo() { return null; }
        @Override public String getIniFilePath() { return ""; }
        @Override public Map<String, String> getTooltips() { return Collections.emptyMap(); }
        @Override public Map<String, DialogModel.Field> getFieldsInUiOrder() { return Collections.emptyMap(); }
        @Override public Map<String, DialogModel> getDialogs() { return Collections.emptyMap(); }
        @Override public String getDialogKeyByTitle(String t) { return null; }
        @Override public IniField findByOffset(int i) { return null; }
        @Override public Map<String, GaugeCategoryModel> getGaugeCategories() { return Collections.emptyMap(); }
        @Override public GaugeModel findGaugeByChannel(String c) { return null; }
        @Override public Map<String, String> getTopicHelp() { return Collections.emptyMap(); }
        @Override public Map<String, ContextHelpModel> getContextHelp() { return Collections.emptyMap(); }
        @Override public ContextHelpModel getContextHelp(String r) { return null; }
        @Override public Map<String, TableModel> getTables() { return Collections.emptyMap(); }
        @Override public Map<String, CurveModel> getCurves() { return Collections.emptyMap(); }
        @Override public TableModel getTable(String n) { return null; }
        @Override public FrontPageModel getFrontPage() { return null; }
        @Override public List<MenuModel> getMenus() { return Collections.emptyList(); }
    }
}
