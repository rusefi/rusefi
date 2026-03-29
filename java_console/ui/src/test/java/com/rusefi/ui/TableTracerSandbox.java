package com.rusefi.ui;

import com.devexperts.logging.Logging;
import com.opensr5.ConfigurationImage;
import com.opensr5.ini.*;
import com.opensr5.ini.field.IniField;
import com.opensr5.io.DataListener;
import com.rusefi.binaryprotocol.BinaryProtocol;
import com.rusefi.binaryprotocol.IncomingDataBuffer;
import com.rusefi.core.Sensor;
import com.rusefi.core.SensorCentral;
import com.rusefi.ini.reader.IniFileReaderUtil;
import com.rusefi.ini.reader.IniParsingException;
import com.rusefi.io.serial.AbstractIoStream;
import com.rusefi.ui.todo_better_module.TableTracerLayerUI;
import com.rusefi.ui.widgets.tune.CalibrationDialogWidget;
import javax.swing.*;
import java.io.InputStream;
import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.util.concurrent.Executors;
import java.util.concurrent.ScheduledExecutorService;
import java.util.concurrent.TimeUnit;

import static com.devexperts.logging.Logging.getLogging;

public class TableTracerSandbox {
    private static final Logging log = getLogging(TableTracerSandbox.class);

     //Private constants for sandbox environment
    private static final int BUFFER_SIZE = 65536;
    private static final String INI_PATH = "/january.ini";
    private static final int SIMULATION_DELAY_MS = 20;

     //Private constants for INI field mapping
    private static final String RPM_BINS_FIELD = "veRpmBins";
    private static final String LOAD_BINS_FIELD = "veLoadBins";
    private static final String VE_TABLE_FIELD = "veTable";
    private static final String DIALOG_NAME = "veTableDialog";

     //Private constants for UI frame
    private static final int WIN_WIDTH = 1280;
    private static final int WIN_HEIGHT = 720;

     //Sensors to simulate (must match TableTracerLayerUI logic)
    private static final Sensor SIM_RPM_SENSOR = Sensor.RPMGauge;
    private static final Sensor SIM_LOAD_SENSOR = Sensor.MAPGauge;

    private final ScheduledExecutorService scheduler = Executors.newSingleThreadScheduledExecutor();

    public void start() {
        try {
            InputStream is = getClass().getResourceAsStream(INI_PATH);
            RawIniFile content = IniFileReaderUtil.read(is, INI_PATH);

            IniFileModel realModel;
            try {
                realModel = IniFileReaderUtil.readIniFile(content, INI_PATH, new IniFileMetaInfoImpl(content));
            } catch (IniParsingException exception) {
                log.error("FATAL: INI Error: " + exception.getMessage(), exception);
                return;
            }

            UIContext uiContext = new UIContext();
            uiContext.iniFileState.setIniFileModelForTest(realModel);

            byte[] initialData = createInitialBuffer(realModel);
            ConfigurationImage stubImage = new ConfigurationImage(initialData);

            AbstractIoStream dummyStream = new AbstractIoStream() {
                @Override public void setInputListener(DataListener listener) {}
                @Override public IncomingDataBuffer getDataBuffer() {
                    return createDataBuffer();
                }
            };

            uiContext.getLinkManager().setBinaryProtocolForTests(new BinaryProtocol(uiContext.getLinkManager(), dummyStream) {
                @Override public ConfigurationImage getControllerConfiguration() { return stubImage; }
                @Override public IniFileModel getIniFileNullable() { return realModel; }
            });

            SwingUtilities.invokeLater(() -> runAwt(uiContext));
            launchSimulation(realModel, stubImage);
        } catch (Exception exception) {
            log.error(exception.getMessage(), exception);
        }
    }

    private byte[] createInitialBuffer(IniFileModel model) {
        byte[] buffer = new byte[BUFFER_SIZE];
        ByteBuffer bb = ByteBuffer.wrap(buffer).order(ByteOrder.LITTLE_ENDIAN);

        IniField rpmBins = model.getIniField(RPM_BINS_FIELD);
        if (rpmBins != null) {
            int offset = rpmBins.getOffset();
            for (int i = 0; i < 16; i++) {
                bb.putShort(offset + (i * 2), (short) (400 + i * 500));
            }
        }

        IniField loadBins = model.getIniField(LOAD_BINS_FIELD);
        if (loadBins != null) {
            int offset = loadBins.getOffset();
            for (int i = 0; i < 16; i++) {
                bb.putShort(offset + (i * 2), (short) (20 + i * 15));
            }
        }

        IniField veTable = model.getIniField(VE_TABLE_FIELD);
        if (veTable != null) {
            int offset = veTable.getOffset();
            for (int i = 0; i < 256; i++) {
                short val = (short) (500 + (i * 2));
                bb.putShort(offset + (i * 2), val);
            }
        }
        return buffer;
    }

    private void runAwt(UIContext uiContext) {
        CalibrationDialogWidget tableWidget = new CalibrationDialogWidget(uiContext);
        tableWidget.update(DIALOG_NAME);

        TableTracerLayerUI tracerLayer = new TableTracerLayerUI();
        JPanel contentPanel = tableWidget.getContentPane();

        JLayer<JPanel> jLayer = new JLayer<>(contentPanel, tracerLayer);
        tracerLayer.setOwner(jLayer);

        JFrame frame = new JFrame("RusEFI - INI Table Tracer");
        frame.add(jLayer);
        frame.setSize(WIN_WIDTH, WIN_HEIGHT);
        frame.setLocationRelativeTo(null);
        frame.setVisible(true);
        frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
    }

    private void launchSimulation(IniFileModel model, ConfigurationImage image) {
        byte[] outputsBuffer = new byte[BUFFER_SIZE];
        scheduler.scheduleAtFixedRate(() -> {
            try {
                double time = System.currentTimeMillis() / 1000.0;
                SensorCentral.getInstance().grabSensorValues(outputsBuffer, model, image);

                 //Use the private sensors defined for simulation
                SensorCentral.getInstance().setValue(4000 + 3500 * Math.sin(time), SIM_RPM_SENSOR.getNativeName());
                SensorCentral.getInstance().setValue(120 + 80 * Math.cos(time), SIM_LOAD_SENSOR.getNativeName());
            } catch (Exception ignored) {}
        }, 0, SIMULATION_DELAY_MS, TimeUnit.MILLISECONDS);
    }

    public static void main(String[] args) {
        new TableTracerSandbox().start();
    }
}
