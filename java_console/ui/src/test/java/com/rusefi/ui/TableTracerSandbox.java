package com.rusefi.ui;

import com.devexperts.logging.Logging;
import com.opensr5.ConfigurationImage;
import com.opensr5.ini.*;
import com.opensr5.io.DataListener;
import com.rusefi.binaryprotocol.BinaryProtocol;
import com.rusefi.binaryprotocol.IncomingDataBuffer;
import com.rusefi.config.Field;
import com.rusefi.config.FieldType;
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

    // ==========================================
    // GLOBAL CONFIGURATION
    // ==========================================
    private static final int ECU_MEMORY_SIZE = 65536;
    private static final String INI_FILE_PATH = "/january.ini";
    private static final int SIMULATION_REFRESH_RATE_MS = 20;

    private static final String TARGET_TABLE_ID = "veTableTbl";
    private static final String DEFAULT_FALLBACK_DIALOG = "veTableDialog";

    private static final double X_AXIS_MIN_VALUE = 400;
    private static final double X_AXIS_STEP_SIZE = 500;
    private static final int X_AXIS_BIN_COUNT = 16;

    private static final double Y_AXIS_MIN_VALUE = 20;
    private static final double Y_AXIS_STEP_SIZE = 5;
    private static final int Y_AXIS_BIN_COUNT = 16;
    // ==========================================

    private final ScheduledExecutorService simulationThreadExecutor = Executors.newSingleThreadScheduledExecutor();

    public void start() {
        try {
            IniFileModel iniFileModel = loadAndParseIniFile();

            TableModel targetTableMetadata = iniFileModel.getTable(TARGET_TABLE_ID);
            if (targetTableMetadata == null) {
                throw new RuntimeException(String.format("CRITICAL: Table ID '%s' not found.", TARGET_TABLE_ID));
            }

            String xAxisSensorChannel = resolveChannel(iniFileModel, targetTableMetadata, true);
            String yAxisSensorChannel = resolveChannel(iniFileModel, targetTableMetadata, false);

            double[] xAxisTableBins = generateLinearBinScale(X_AXIS_MIN_VALUE, X_AXIS_STEP_SIZE, X_AXIS_BIN_COUNT);
            double[] yAxisTableBins = generateLinearBinScale(Y_AXIS_MIN_VALUE, Y_AXIS_STEP_SIZE, Y_AXIS_BIN_COUNT);

            UIContext mockUiContext = initializeMockUiContext(iniFileModel);
            setupVirtualEcuController(mockUiContext, iniFileModel, targetTableMetadata, xAxisTableBins, yAxisTableBins);

            String discoveredDialogName = findDialogContainingTable(iniFileModel);

            SwingUtilities.invokeLater(() ->
                    launchTableTracerWindow(mockUiContext, xAxisSensorChannel, yAxisSensorChannel, xAxisTableBins, yAxisTableBins, discoveredDialogName)
            );

            runLiveSensorSimulation(xAxisSensorChannel, yAxisSensorChannel, xAxisTableBins, yAxisTableBins);

        } catch (Exception | IniParsingException e) {
            log.error("Sandbox encountered a fatal error during initialization", e);
        }
    }

    private IniFileModel loadAndParseIniFile() throws Exception, IniParsingException {
        InputStream iniInputStream = getClass().getResourceAsStream(INI_FILE_PATH);
        if (iniInputStream == null)
            throw new RuntimeException(String.format("Unable to locate INI resource: %s", INI_FILE_PATH));
        RawIniFile rawIniContent = IniFileReaderUtil.read(iniInputStream, INI_FILE_PATH);
        return IniFileReaderUtil.readIniFile(rawIniContent, INI_FILE_PATH, new IniFileMetaInfoImpl(rawIniContent));
    }

    private UIContext initializeMockUiContext(IniFileModel model) {
        UIContext context = new UIContext();
        context.iniFileState.setIniFileModelForTest(model);
        return context;
    }

    private void setupVirtualEcuController(UIContext context, IniFileModel model, TableModel table, double[] xAxisBins, double[] yAxisBins) {
        byte[] binaryMemoryBuffer = generateMockBinaryBuffer(model, table, xAxisBins, yAxisBins);
        ConfigurationImage configImage = new ConfigurationImage(binaryMemoryBuffer);

        AbstractIoStream mockStream = new AbstractIoStream() {
            @Override
            public void setInputListener(DataListener listener) {
            }

            @Override
            public IncomingDataBuffer getDataBuffer() {
                return createDataBuffer();
            }
        };

        context.getLinkManager().setBinaryProtocolForTests(new BinaryProtocol(context.getLinkManager(), mockStream) {
            @Override
            public ConfigurationImage getControllerConfiguration() {
                return configImage;
            }

            @Override
            public IniFileModel getIniFileNullable() {
                return model;
            }
        });
    }

    private String resolveChannel(IniFileModel model, TableModel table, boolean isXAxis) {
        String label = isXAxis ? table.getXLabel() : table.getYLabel();
        if (label == null || label.isEmpty()) {
            label = isXAxis ? table.getXBinsConstant() : table.getYBinsConstant();
        }

        GaugeModel gauge = model.getGauge(label);
        String resolvedChannel = (gauge != null && gauge.getChannel() != null) ? gauge.getChannel() : label;

        log.info(String.format("Channel Mapping (%s): '%s' -> '%s'", isXAxis ? "X" : "Y", label, resolvedChannel));

        return resolvedChannel;
    }

    private double[] generateLinearBinScale(double startValue, double stepSize, int binCount) {
        double[] bins = new double[binCount];
        for (int i = 0; i < binCount; i++) bins[i] = startValue + (i * stepSize);
        return bins;
    }

    private byte[] generateMockBinaryBuffer(IniFileModel model, TableModel table, double[] xAxisBins, double[] yAxisBins) {
        byte[] buffer = new byte[ECU_MEMORY_SIZE];
        ByteBuffer memoryWriter = ByteBuffer.wrap(buffer).order(ByteOrder.LITTLE_ENDIAN);

        populateFieldInMemory(model, table.getXBinsConstant(), memoryWriter, xAxisBins);
        populateFieldInMemory(model, table.getYBinsConstant(), memoryWriter, yAxisBins);

        String zConstantName = table.getZBinsConstant();
        model.findIniField(zConstantName).ifPresent(fieldMetadata -> {
            int rowCount = yAxisBins.length;
            int colCount = xAxisBins.length;
            int elementSizeInBytes = fieldMetadata.getSize() / (rowCount * colCount);

            // Detect FieldType based on element size
            FieldType type = (elementSizeInBytes == 1) ? FieldType.INT8 : FieldType.INT16;
            Field zField = Field.create(zConstantName, fieldMetadata.getOffset(), type);
            int absoluteOffset = model.getIniField(zField).getOffset();

            for (int rowIndex = 0; rowIndex < rowCount; rowIndex++) {
                for (int columnIndex = 0; columnIndex < colCount; columnIndex++) {
                    double cellValue = 10.0 + ((rowCount - 1 - rowIndex) * 1.5) + (columnIndex * 0.8);
                    int flatIndex = (rowIndex * colCount) + columnIndex;
                    int targetOffset = absoluteOffset + (flatIndex * elementSizeInBytes);

                    if (elementSizeInBytes == 1) {
                        memoryWriter.put(targetOffset, (byte) cellValue);
                    } else {
                        memoryWriter.putShort(targetOffset, (short) (cellValue * 10));
                    }
                }
            }
            log.info(String.format("Populated %d-bit memory for table '%s' (%d cells)", (elementSizeInBytes * 8), zConstantName, (rowCount * colCount)));
        });
        return buffer;
    }

    private void populateFieldInMemory(IniFileModel model, String constantName, ByteBuffer writer, double[] data) {
        model.findIniField(constantName).ifPresent(fieldMetadata -> {
            int elementSize = fieldMetadata.getSize() / data.length;
            FieldType type = (elementSize == 1) ? FieldType.INT8 : FieldType.INT16;
            Field axisField = Field.create(constantName, fieldMetadata.getOffset(), type);
            int absoluteOffset = model.getIniField(axisField).getOffset();

            for (int i = 0; i < data.length; i++) {
                int targetPos = absoluteOffset + (i * elementSize);
                if (elementSize == 1) {
                    writer.put(targetPos, (byte) data[i]);
                } else {
                    writer.putShort(targetPos, (short) data[i]);
                }
            }
        });
    }

    private String findDialogContainingTable(IniFileModel model) {
        String dialogName = model.getDialogs().entrySet().stream()
                .filter(entry -> entry.getValue().getPanels().stream()
                        .anyMatch(panel -> TARGET_TABLE_ID.equals(panel.getPanelName())))
                .map(java.util.Map.Entry::getKey)
                .findFirst()
                .orElse(DEFAULT_FALLBACK_DIALOG);

        log.info(String.format("UI Mapping: Table '%s' -> Dialog '%s'", TARGET_TABLE_ID, dialogName));
        return dialogName;
    }

    private void launchTableTracerWindow(UIContext context, String xCh, String yCh, double[] xBins, double[] yBins, String targetDialog) {
        CalibrationDialogWidget calibrationWidget = new CalibrationDialogWidget(context);
        calibrationWidget.update(targetDialog);

        TableTracerLayerUI tracerLayer = new TableTracerLayerUI(xCh, yCh, xBins, yBins);
        JLayer<JPanel> uiDecorator = new JLayer<>(calibrationWidget.getContentPane(), tracerLayer);
        tracerLayer.setOwner(uiDecorator);

        JFrame mainFrame = new JFrame("rusEFI Table Tracer Sandbox");
        mainFrame.addWindowListener(new java.awt.event.WindowAdapter() {
            @Override
            public void windowClosing(java.awt.event.WindowEvent e) {
                tracerLayer.unsubscribe();
                simulationThreadExecutor.shutdown();
                log.info("Sandbox cleanup completed. Simulation stopped.");
            }
        });

        mainFrame.add(uiDecorator);
        mainFrame.setSize(1280, 720);
        mainFrame.setLocationRelativeTo(null);
        mainFrame.setVisible(true);
        mainFrame.setDefaultCloseOperation(JFrame.DISPOSE_ON_CLOSE);
    }

    private void runLiveSensorSimulation(String xAxisChannel, String yAxisChannel, double[] xAxisBins, double[] yAxisBins) {
        double xAxisMin = xAxisBins[0], xAxisMax = xAxisBins[xAxisBins.length - 1];
        double yAxisMin = yAxisBins[yAxisBins.length - 1], yAxisMax = yAxisBins[0];

        double xAxisCenter = (xAxisMax + xAxisMin) / 2.0;
        double xAxisAmplitude = (xAxisMax - xAxisMin) / 2.0;
        double yAxisCenter = (yAxisMax + yAxisMin) / 2.0;
        double yAxisAmplitude = (yAxisMax - yAxisMin) / 2.0;

        simulationThreadExecutor.scheduleAtFixedRate(() -> {
            try {
                double currentTimeInSeconds = System.currentTimeMillis() / 1000.0;
                SensorCentral.getInstance().setValue(xAxisCenter + xAxisAmplitude * Math.sin(currentTimeInSeconds), xAxisChannel);
                SensorCentral.getInstance().setValue(yAxisCenter + yAxisAmplitude * Math.cos(currentTimeInSeconds), yAxisChannel);
            } catch (Exception ignored) {
            }
        }, 0, SIMULATION_REFRESH_RATE_MS, TimeUnit.MILLISECONDS);
    }

    public static void main(String[] args) {
        new TableTracerSandbox().start();
    }
}