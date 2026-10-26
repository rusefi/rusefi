package com.rusefi.sensor_logs;

import com.opensr5.ini.DatalogEntry;
import com.opensr5.ini.IniFileModel;
import com.opensr5.ini.field.EnumIniField;
import com.opensr5.ini.field.IniField;
import com.opensr5.ini.field.ScalarIniField;
import com.rusefi.binaryprotocol.BinaryProtocol;
import com.rusefi.config.FieldType;
import com.rusefi.core.OutputChannelSnapshot;
import com.rusefi.core.SensorCentral;

import java.io.File;
import java.util.ArrayList;
import java.util.Collections;
import java.util.HashSet;
import java.util.List;
import java.util.Map;
import java.util.Set;
import java.util.TreeMap;
import java.util.function.LongSupplier;
import java.util.function.Supplier;

import static com.rusefi.config.generated.VariableRegistryValues.GAUGE_NAME_MAP;
import static com.rusefi.core.SensorNames.MAPGauge;

/**
 * Andrey Belomutskiy, (c) 2013-2020
 * 4/15/2016.
 */
public class SensorLogger {
    private static List<CustomBinaryLogEntry> getOutputChannels(Supplier<BinaryProtocol> protocol,
                                                                LoggingProfile profile) {
        BinaryProtocol bp = protocol.get();
        if (bp == null) {
            return Collections.emptyList();
        }
        IniFileModel iniFileModel = bp.getIniFileNullable();
        if (iniFileModel == null) {
            return Collections.emptyList();
        }
        List<CustomBinaryLogEntry> all = getOutputChannels(iniFileModel);
        return profile == null ? all : profile.select(all, iniFileModel);
    }

    public static List<CustomBinaryLogEntry> getOutputChannels(IniFileModel iniFileModel) {
        List<CustomBinaryLogEntry> outputChannels = new ArrayList<>();
        Map<String, IniField> fieldsByName = new TreeMap<>(String.CASE_INSENSITIVE_ORDER);
        fieldsByName.putAll(iniFileModel.getAllOutputChannels());
        Set<IniField> labeledFields = new HashSet<>();
        for (DatalogEntry entry : iniFileModel.getDatalogEntries()) {
            IniField field = fieldsByName.get(entry.getChannel());
            // Computed and host-only channels (including TS 'time') have no raw field.
            if (isRecordable(field)) {
                outputChannels.add(new CustomBinaryLogEntry(entry.getLabel(), field));
                labeledFields.add(field);
            }
        }
        // Preserve coverage for INIs with missing or incomplete [Datalog] sections.
        for (IniField field : iniFileModel.getAllOutputChannels().values()) {
            if (isRecordable(field) && !labeledFields.contains(field)) {
                outputChannels.add(new CustomBinaryLogEntry(field.getName(), field));
                if (MAPGauge.equalsIgnoreCase(field.getName())) {
                    outputChannels.add(new CustomBinaryLogEntry(GAUGE_NAME_MAP, field));
                }
            }
        }
        return outputChannels;
    }

    private static boolean isRecordable(IniField field) {
        return field instanceof EnumIniField
                || (field instanceof ScalarIniField
                && ((ScalarIniField) field).getType() != FieldType.BIT
                && ((ScalarIniField) field).getType() != FieldType.STRING);
    }

    private final Supplier<BinaryProtocol> protocol;
    private final LongSupplier clock;
    private BinarySensorLog<CustomBinaryLogEntry> sensorLog;
    private long firstRowMillis = -1;
    private long lastRowMillis = -1;
    private double lastRecordingSeconds = Double.NaN;
    private SensorCentral.FullOutputLease fullOutputLease;
    private SensorCentral.SnapshotListenerToken snapshotListenerToken;
    private byte[] currentResponse;

    public SensorLogger(Supplier<BinaryProtocol> protocol) {
        this(protocol, System::currentTimeMillis);
    }

    /** @param clock milliseconds, for the rows' timestamps and the recording duration */
    SensorLogger(Supplier<BinaryProtocol> protocol, LongSupplier clock) {
        this.protocol = protocol;
        this.clock = clock;
    }

    public synchronized boolean start(File file) {
        return start(file, null);
    }

    public synchronized boolean start(File file, byte[] tune) {
        return start(file, tune, null);
    }

    /**
     * @param profile the signals to record, or null for every recordable channel. The full output block is
     *                read either way; the profile only decides what goes into the file.
     */
    public synchronized boolean start(File file, byte[] tune, LoggingProfile profile) {
        if (sensorLog != null) {
            return true;
        }

        List<CustomBinaryLogEntry> outputChannels = getOutputChannels(protocol, profile);
        if (outputChannels.isEmpty()) {
            return false;
        }

        firstRowMillis = lastRowMillis = -1;
        lastRecordingSeconds = Double.NaN;
        sensorLog = new BinarySensorLog<>(sensor -> {
            return currentResponse == null ? 0.0 : sensor.getValue(currentResponse);
        }, outputChannels, clock::getAsLong, file.getAbsolutePath(), tune);
        SensorCentral sensorCentral = SensorCentral.getInstance();
        fullOutputLease = sensorCentral.acquireFullOutput();
        snapshotListenerToken = sensorCentral.addSnapshotListener(this::writeSensorLogLine);
        return true;
    }

    private synchronized void writeSensorLogLine(OutputChannelSnapshot snapshot) {
        if (sensorLog != null && fullOutputLease != null && snapshot.isFull()
                && snapshot.getGeneration() >= fullOutputLease.getGeneration()) {
            currentResponse = snapshot.getResponse();
            long now = clock.getAsLong();
            sensorLog.writeSensorLogLine();
            if (firstRowMillis < 0) {
                firstRowMillis = now;
            }
            lastRowMillis = now;
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
            lastRecordingSeconds = firstRowMillis < 0 ? Double.NaN : (lastRowMillis - firstRowMillis) / 1000.0;
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

    /**
     * Seconds from the first to the last row of the recording stopped last, as a log viewer reads it from the
     * rows' timestamps; NaN when no recording has stopped or it has no rows.
     */
    public synchronized double getLastRecordingSeconds() {
        return lastRecordingSeconds;
    }
}
