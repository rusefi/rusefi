package com.rusefi.mcp;

import com.opensr5.ini.IniFileModel;
import com.rusefi.core.OutputChannelSnapshot;
import com.rusefi.core.SensorCentral;
import com.rusefi.sensor_logs.BinarySensorLog;
import com.rusefi.sensor_logs.CustomBinaryLogEntry;
import com.rusefi.sensor_logs.SensorLogger;
import org.json.simple.JSONObject;

import java.io.IOException;
import java.io.OutputStream;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.StandardOpenOption;
import java.util.List;

/** One host-side recording, fed by complete ECU polls using the frontend's MLG encoder. */
final class EcuDataLogger {
    private OutputStream output;
    private BinarySensorLog<CustomBinaryLogEntry> writer;
    private SensorCentral.SnapshotListenerToken listener;
    private SensorCentral.FullOutputLease lease;
    private byte[] response;
    private Path path;
    private long samples;
    private int channelCount;
    private String error;

    synchronized JSONObject start(IniFileModel ini, Path requestedPath) throws IOException {
        if (writer != null) {
            throw new IllegalStateException("Data logging is already active; stop it before starting another file");
        }
        List<CustomBinaryLogEntry> entries = SensorLogger.getOutputChannels(ini);
        if (entries.isEmpty()) {
            throw new IllegalArgumentException("No recordable output channels in the ECU .ini");
        }
        Path target = requestedPath == null ? Files.createTempFile("rusefi_data_", ".mlg")
                : requestedPath.toAbsolutePath().normalize();
        OutputStream opened = requestedPath == null ? Files.newOutputStream(target)
                : Files.newOutputStream(target, StandardOpenOption.CREATE_NEW, StandardOpenOption.WRITE);
        try {
            writer = new BinarySensorLog<>(entry -> entry.getValue(response), entries, opened);
        } catch (IOException | RuntimeException failure) {
            try {
                opened.close();
            } catch (IOException closeFailure) {
                failure.addSuppressed(closeFailure);
            }
            throw failure;
        }
        output = opened;
        path = target;
        samples = 0;
        channelCount = entries.size();
        error = null;
        SensorCentral sensors = SensorCentral.getInstance();
        lease = sensors.acquireFullOutput();
        listener = sensors.addSnapshotListener(this::record);
        return status();
    }

    private synchronized void record(OutputChannelSnapshot snapshot) {
        if (writer == null || !snapshot.isFull() || snapshot.getGeneration() < lease.getGeneration()) {
            return;
        }
        try {
            response = snapshot.getResponse();
            writer.writeSensorLogLineChecked();
            samples++;
        } catch (IOException | RuntimeException failure) {
            error = failure.toString();
            stop();
        }
    }

    synchronized JSONObject stop() {
        if (listener != null) {
            listener.remove();
            listener = null;
        }
        writer = null;
        response = null;
        if (output != null) {
            try {
                output.close();
            } catch (IOException failure) {
                error = error == null ? failure.toString() : error + "; " + failure;
            }
            output = null;
        }
        if (lease != null) {
            lease.close();
            lease = null;
        }
        return status();
    }

    @SuppressWarnings("unchecked")
    synchronized JSONObject status() {
        JSONObject result = new JSONObject();
        result.put("success", error == null);
        result.put("logging", writer != null);
        result.put("path", path == null ? null : path.toString());
        result.put("format", "mlg");
        result.put("sampleCount", samples);
        result.put("channelCount", channelCount);
        if (error != null) {
            result.put("error", error);
        }
        return result;
    }
}
