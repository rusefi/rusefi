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
import com.rusefi.ui.todo_better_module.CurveTracerLayerUI;
import com.rusefi.ui.widgets.tune.CalibrationDialogWidget;
import javax.swing.*;
import java.io.InputStream;
import java.util.concurrent.Executors;
import java.util.concurrent.ScheduledExecutorService;
import java.util.concurrent.TimeUnit;

import static com.devexperts.logging.Logging.getLogging;

public class CurveTracerSandbox {
    private static final Logging log = getLogging(CurveTracerSandbox.class);
    private static final int BUFFER_SIZE = 65536;
    private static final String INI_PATH = "/january.ini";
    private static final int SIMULATION_DELAY_MS = 20;

    private final ScheduledExecutorService scheduler = Executors.newSingleThreadScheduledExecutor();

    public void start() {
        try {
            InputStream is = getClass().getResourceAsStream(INI_PATH);
            RawIniFile content = IniFileReaderUtil.read(is, INI_PATH);

            IniFileModel realModel;
            try {
                realModel = IniFileReaderUtil.readIniFile(content, INI_PATH, new IniFileMetaInfoImpl(content));
            } catch (IniParsingException exception) {
                log.error("Error parsing INI: " + exception.getMessage(), exception);
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

        // 1. Populate dwellVoltageCorrVoltBins (X-Axis: Battery Voltage)
        // From january.ini: U08, size 8, scale 0.1
        IniField xBinsField = model.getIniField("dwellVoltageCorrVoltBins");
        if (xBinsField != null) {
            int offset = xBinsField.getOffset();
            for (int i = 0; i < 8; i++) {
                // Values from 8.0V to 15.0V (encoded as 80 to 150)
                buffer[offset + i] = (byte) (80 + i * 10);
            }
        }

        // 2. Populate dwellVoltageCorrValues (Y-Axis: Multiplier)
        // From january.ini: U08, size 8, scale 0.02
        IniField yBinsField = model.getIniField("dwellVoltageCorrValues");
        if (yBinsField != null) {
            int offset = yBinsField.getOffset();
            for (int i = 0; i < 8; i++) {
                // Multipliers from 2.0 to 1.0 (encoded as 100 down to 50)
                buffer[offset + i] = (byte) (100 - i * 7);
            }
        }

        return buffer;
    }

    private void runAwt(UIContext uiContext) {
        CalibrationDialogWidget curveWidget = new CalibrationDialogWidget(uiContext);
        curveWidget.update("dwellVoltageCorrection");

        JPanel contentPanel = curveWidget.getContentPane();

        CurveTracerLayerUI tracerLayer = new CurveTracerLayerUI(uiContext, "dwellVoltageCorrection", Sensor.vvtPositionB1I);

        JLayer<JPanel> jLayer = new JLayer<>(contentPanel, tracerLayer);
        tracerLayer.setOwner(jLayer);

        JFrame frame = new JFrame("RusEFI - Curve Tracer");
        frame.add(jLayer);
        frame.setSize(1280, 720);
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
                double val = 12.0 + 3.0 * Math.sin(time);
                SensorCentral.getInstance().setValue(val, Sensor.vvtPositionB1I.getNativeName());
            } catch (Exception ignored) {}
        }, 0, SIMULATION_DELAY_MS, TimeUnit.MILLISECONDS);
    }

    public static void main(String[] args) {
        new CurveTracerSandbox().start();
    }
}
