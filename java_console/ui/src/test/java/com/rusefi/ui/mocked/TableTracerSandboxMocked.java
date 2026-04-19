package com.rusefi.ui.mocked;

import com.opensr5.ConfigurationImage;
import com.opensr5.ini.*;
import com.opensr5.ini.field.ArrayIniField;
import com.opensr5.ini.field.IniField;
import com.opensr5.io.DataListener;
import com.rusefi.binaryprotocol.BinaryProtocol;
import com.rusefi.binaryprotocol.IncomingDataBuffer;
import com.rusefi.config.FieldType;
import com.rusefi.core.Sensor;
import com.rusefi.core.SensorCentral;
import com.rusefi.core.ui.FrameHelper;
import com.rusefi.io.serial.AbstractIoStream;
import com.rusefi.ui.UIContext;
import com.rusefi.ui.todo_better_module.TracerLayerUI;
import com.rusefi.ui.widgets.tune.CalibrationDialogWidget;
import com.rusefi.ui.widgets.tune.MainMenuTreeWidget;

import javax.swing.*;
import java.awt.*;
import java.awt.event.WindowAdapter;
import java.awt.event.WindowEvent;
import java.io.IOException;
import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.util.List;
import java.util.Optional;
import java.util.concurrent.Executors;
import java.util.concurrent.ScheduledExecutorService;
import java.util.concurrent.TimeUnit;

import static java.util.Collections.singletonList;

public class TableTracerSandboxMocked {

    // --- UI & Simulation Constants ---
    private static final String WINDOW_TITLE = "RusEFI Table Tracer Sandbox";
    private static final Dimension WINDOW_SIZE = new Dimension(1280, 720);
    private static final int SIMULATION_DELAY_MS = 20;
    private static final int BUFFER_SIZE = 8192;

    // --- Table Metadata & IDs ---
    private static final String VE_TABLE_ID = "veTable";
    private static final String VE_TABLE_NAME = "Fuel Table";
    private static final String VE_TABLE_TITLE = "Stub VE Table";
    private static final String RPM_BINS_KEY = "rpmBins";
    private static final String LOAD_BINS_KEY = "loadBins";
    private static final String VE_ENTRIES_KEY = "veEntries";

    // --- Memory Layout (Offsets) ---
    private static final int RPM_BINS_OFFSET = 0;
    private static final int LOAD_BINS_OFFSET = 200;
    private static final int VE_TABLE_OFFSET = 500;
    private static final int TABLE_DIM = 16;

    // --- Simulation Formula Constants ---
    private static final double VE_BASE_VALUE = 45.0;
    private static final double LOAD_WEIGHT = 0.35;
    private static final double RPM_DIVISOR = 3000.0;
    private static final double SIN_AMPLITUDE = 15.0;

    // --- Bins defined directly as Double[] to avoid conversions ---
    private static final Double[] RPM_BINS = {650.0, 800.0, 1100.0, 1400.0, 1700.0, 2000.0, 2300.0, 2600.0, 2900.0, 3200.0, 3500.0, 3800.0, 4100.0, 4400.0, 4700.0, 7000.0};
    private static final Double[] LOAD_BINS = {10.0, 20.0, 30.0, 40.0, 50.0, 60.0, 70.0, 80.0, 90.0, 100.0, 110.0, 120.0, 130.0, 140.0, 150.0, 160.0};

    private final ScheduledExecutorService scheduler = Executors.newSingleThreadScheduledExecutor(
            new com.rusefi.NamedThreadFactory("TableTracerSandbox-Simulation", true)
    );

    public static void main(String[] args) {
        new TableTracerSandboxMocked().start();
    }

    public void start() {
        IniFileModel stubModel = new StubTableIniModel();
        ConfigurationImage stubImage = new ConfigurationImage(getData());

        AbstractIoStream tsStream = new AbstractIoStream() {
            @Override public void setInputListener(DataListener listener) {}
            @Override public void close() {}
            @Override public IncomingDataBuffer getDataBuffer() { return null; }
            @Override public void write(byte[] bytes) throws IOException {}
        };

        UIContext uiContext = new UIContext();
        uiContext.getLinkManager().setBinaryProtocolForTests(new BinaryProtocol(uiContext.getLinkManager(), tsStream) {
            @Override public ConfigurationImage getControllerConfiguration() { return stubImage; }
            @Override public IniFileModel getIniFileNullable() { return stubModel; }
        });

        launchSimulation(stubModel, stubImage);
        SwingUtilities.invokeLater(() -> runAwt(uiContext));
    }

    private void launchSimulation(IniFileModel stubModel, ConfigurationImage stubImage) {
        scheduler.scheduleAtFixedRate(() -> tick(stubModel, stubImage), 0, SIMULATION_DELAY_MS, TimeUnit.MILLISECONDS);
    }

    private void tick(IniFileModel stubModel, ConfigurationImage stubImage) {
        try {
            double time = System.currentTimeMillis() / 1000.0;

            // Oscillate values for the tracer
            double mockRpm = 2650 + 1850 * Math.sin(time * 0.5);
            double mockLoad = 80 + 60 * Math.cos(time * 0.7);

            SensorCentral sc = SensorCentral.getInstance();
            sc.setValue(mockRpm, Sensor.RPMGauge.getNativeName());
            sc.setValue(mockLoad, Sensor.MAPGauge.getNativeName());

            // Notify UI listeners
            sc.grabSensorValues(new byte[0], stubModel, stubImage);

        } catch (Exception exception) {
            System.err.println("Simulation error: " + exception.getMessage());
        }
    }

    private void runAwt(UIContext uiContext) {
        FrameHelper frameHelper = new FrameHelper(JFrame.EXIT_ON_CLOSE);
        JFrame frame = frameHelper.getFrame();

        MainMenuTreeWidget left = new MainMenuTreeWidget(uiContext);
        CalibrationDialogWidget right = new CalibrationDialogWidget(uiContext);

        TracerLayerUI tracerLayerUI = new TracerLayerUI(RPM_BINS, LOAD_BINS);
        JLayer<JPanel> tracerLayer = new JLayer<>(right.getContentPane(), tracerLayerUI);
        tracerLayerUI.setOwner(tracerLayer);

        frame.addWindowListener(new WindowAdapter() {
            @Override
            public void windowClosing(WindowEvent e) {
                scheduler.shutdownNow();
                tracerLayerUI.dispose();
            }
        });

        JSplitPane splitPane = new JSplitPane(JSplitPane.HORIZONTAL_SPLIT,
                left.getContentPane(),
                new JScrollPane(tracerLayer));

        left.setOnSelect(subMenu -> right.update(subMenu.getKey()));
        frame.setSize(WINDOW_SIZE);
        frame.setTitle(WINDOW_TITLE);
        frameHelper.showFrame(splitPane);

        left.selectSubMenu(VE_TABLE_ID);
    }

    private byte[] getData() {
        byte[] buffer = new byte[BUFFER_SIZE];
        ByteBuffer bb = ByteBuffer.wrap(buffer).order(ByteOrder.LITTLE_ENDIAN);

        // Write RPM Bins
        bb.position(RPM_BINS_OFFSET);
        for (Double bin : RPM_BINS) bb.putFloat(bin.floatValue());

        // Write Load Bins
        bb.position(LOAD_BINS_OFFSET);
        for (Double bin : LOAD_BINS) bb.putFloat(bin.floatValue());

        // Generate and write VE Table Data
        bb.position(VE_TABLE_OFFSET);
        for (int rowIdx = 0; rowIdx < TABLE_DIM; rowIdx++) {
            double load = LOAD_BINS[rowIdx];
            for (int colIdx = 0; colIdx < TABLE_DIM; colIdx++) {
                double rpm = RPM_BINS[colIdx];
                double ve = VE_BASE_VALUE + (load * LOAD_WEIGHT) + (Math.sin(rpm / RPM_DIVISOR) * SIN_AMPLITUDE);
                bb.putFloat((float) ve);
            }
        }
        return buffer;
    }

    static class StubTableIniModel extends StubIniFileModel {
        @Override
        public List<MenuModel> getMenus() {
            MenuModel menu = new MenuModel("Engine");
            menu.getItems().add(new SubMenuModel(VE_TABLE_ID, VE_TABLE_NAME));
            return singletonList(menu);
        }

        @Override
        public TableModel getTable(String name) {
            return new TableModel(
                    name, name + "_3d", VE_TABLE_TITLE, "Help",
                    "RPM", "Load", RPM_BINS_KEY, null, false,
                    LOAD_BINS_KEY, null, false, VE_ENTRIES_KEY,
                    "Up", "Down", 10.0, "normal"
            );
        }

        @Override
        public Optional<IniField> findIniField(String key) {
            // Memory types are FLOAT (4 bytes) even if stored as Double in UI
            if (RPM_BINS_KEY.equals(key)) {
                return Optional.of(new ArrayIniField(key, RPM_BINS_OFFSET, FieldType.FLOAT, TABLE_DIM, 1, "RPM", 1.0, "0", "8000", "0"));
            } else if (LOAD_BINS_KEY.equals(key)) {
                return Optional.of(new ArrayIniField(key, LOAD_BINS_OFFSET, FieldType.FLOAT, 1, TABLE_DIM, "kPa", 1.0, "0", "300", "0"));
            } else if (VE_ENTRIES_KEY.equals(key)) {
                return Optional.of(new ArrayIniField(key, VE_TABLE_OFFSET, FieldType.FLOAT, TABLE_DIM, TABLE_DIM, "%", 1.0, "0", "100", "1"));
            }
            return Optional.empty();
        }
    }
}
