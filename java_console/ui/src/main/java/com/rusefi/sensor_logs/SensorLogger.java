package com.rusefi.sensor_logs;

import com.opensr5.ini.IniFileModel;
import com.opensr5.ini.field.EnumIniField;
import com.opensr5.ini.field.IniField;
import com.opensr5.ini.field.ScalarIniField;
import com.rusefi.binaryprotocol.BinaryProtocol;
import com.rusefi.config.FieldType;
import com.rusefi.core.OutputChannelSnapshot;
import com.rusefi.core.SensorCentral;
import com.rusefi.ui.UIContext;

import java.io.File;
import java.util.ArrayList;
import java.util.Collections;
import java.util.List;
import java.util.Map;

import static com.rusefi.config.generated.VariableRegistryValues.GAUGE_NAME_MAP;
import static com.rusefi.core.SensorNames.MAPGauge;

/**
 * Andrey Belomutskiy, (c) 2013-2020
 * 4/15/2016.
 */
public class SensorLogger {
    private static List<CustomBinaryLogEntry> getOutputChannels(UIContext uiContext) {
        BinaryProtocol bp = uiContext.getBinaryProtocol();
        if (bp == null) {
            return Collections.emptyList();
        }
        IniFileModel iniFileModel = bp.getIniFileNullable();
        return iniFileModel == null ? Collections.emptyList() : getOutputChannels(iniFileModel);
    }

    private static List<CustomBinaryLogEntry> getOutputChannels(IniFileModel iniFileModel) {
        List<CustomBinaryLogEntry> outputChannels = new ArrayList<>();
        for (Map.Entry<String, IniField> entry : iniFileModel.getAllOutputChannels().entrySet()) {
            IniField field = entry.getValue();
            if (field instanceof EnumIniField
                    || (field instanceof ScalarIniField
                    && ((ScalarIniField) field).getType() != FieldType.BIT
                    && ((ScalarIniField) field).getType() != FieldType.STRING)) {
                outputChannels.add(new CustomBinaryLogEntry(entry.getKey(), field));
                if (MAPGauge.equals(entry.getKey())) {
                    outputChannels.add(new CustomBinaryLogEntry(GAUGE_NAME_MAP, field));
                }
            }
        }
        return outputChannels;
    }

    private final UIContext uiContext;
    private BinarySensorLog<CustomBinaryLogEntry> sensorLog;
    private SensorCentral.FullOutputLease fullOutputLease;
    private SensorCentral.SnapshotListenerToken snapshotListenerToken;
    private byte[] currentResponse;

    public SensorLogger(UIContext uiContext) {
        this.uiContext = uiContext;
    }

    public synchronized boolean start(File file) {
        if (sensorLog != null) {
            return true;
        }

        List<CustomBinaryLogEntry> outputChannels = getOutputChannels(uiContext);
        if (outputChannels.isEmpty()) {
            return false;
        }

        sensorLog = new BinarySensorLog<>(sensor -> {
            return currentResponse == null ? 0.0 : sensor.getValue(currentResponse);
        }, outputChannels, System::currentTimeMillis, file.getAbsolutePath());
        SensorCentral sensorCentral = SensorCentral.getInstance();
        fullOutputLease = sensorCentral.acquireFullOutput();
        snapshotListenerToken = sensorCentral.addSnapshotListener(this::writeSensorLogLine);
        return true;
    }

    private synchronized void writeSensorLogLine(OutputChannelSnapshot snapshot) {
        if (sensorLog != null && fullOutputLease != null && snapshot.isFull()
                && snapshot.getGeneration() >= fullOutputLease.getGeneration()) {
            currentResponse = snapshot.getResponse();
            sensorLog.writeSensorLogLine();
        }
    }

    public synchronized void stop() {
        if (snapshotListenerToken != null) {
            snapshotListenerToken.remove();
            snapshotListenerToken = null;
        }
        if (sensorLog != null) {
            sensorLog.close();
            sensorLog = null;
        }
        currentResponse = null;
        if (fullOutputLease != null) {
            fullOutputLease.close();
            fullOutputLease = null;
        }
    }

    public synchronized boolean isLogging() {
        return sensorLog != null;
    }
}
