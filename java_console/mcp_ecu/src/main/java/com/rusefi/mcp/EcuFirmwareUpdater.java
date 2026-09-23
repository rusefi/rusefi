package com.rusefi.mcp;

import com.devexperts.logging.Logging;
import com.rusefi.ConnectivityContext;
import com.rusefi.EcuHardwareProbes;
import com.rusefi.PortResult;
import com.rusefi.SerialPortScanner;
import com.rusefi.SerialPortType;
import com.rusefi.binaryprotocol.BinaryProtocol;
import com.rusefi.core.FindFileHelper;
import com.rusefi.io.LinkManager;
import com.rusefi.io.UpdateOperationCallbacks;
import com.rusefi.libopenblt.file.SrecParser;
import com.rusefi.maintenance.FirmwareFlashEligibility;
import com.rusefi.maintenance.MaintenanceUtil;
import com.rusefi.maintenance.jobs.JobHelper;
import com.rusefi.maintenance.jobs.OpenBltAutoJob;
import org.json.simple.JSONArray;
import org.json.simple.JSONObject;

import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.util.Locale;
import java.util.concurrent.TimeUnit;

/** Headless adapter for the console's backup/OpenBLT/migration job. */
final class EcuFirmwareUpdater {
    private static final Logging log = Logging.getLogging(EcuFirmwareUpdater.class);

    @FunctionalInterface
    interface UpdateJob {
        void run(LinkManager lm, String firmware, UpdateOperationCallbacks callbacks) throws Exception;
    }

    private final UpdateJob job;

    EcuFirmwareUpdater() {
        this(EcuFirmwareUpdater::runProductionJob);
    }

    EcuFirmwareUpdater(UpdateJob job) {
        this.job = job;
    }

    static Path validateFirmwarePath(Object value) throws IOException {
        if (!(value instanceof String) || ((String) value).trim().isEmpty()) {
            throw new IllegalArgumentException("firmwarePath must be a non-empty string");
        }
        Path path = Paths.get((String) value).toAbsolutePath();
        if (!Files.isRegularFile(path) || !Files.isReadable(path)) {
            throw new IOException("Firmware path is not a readable regular file: " + path);
        }
        SrecParser parser = new SrecParser();
        parser.parse(path.toFile());
        if (parser.getSegments().isEmpty()) {
            throw new IOException("Firmware file contains no data records: " + path);
        }
        return path;
    }

    @SuppressWarnings("unchecked")
    JSONObject update(LinkManager lm, Path firmware, Runnable beforeHandoff) {
        Callbacks callbacks = new Callbacks(beforeHandoff);
        JSONObject result = new JSONObject();
        BinaryProtocol before = lm.getBinaryProtocol();
        result.put("previousSignature", before == null ? null : before.signature);
        try {
            job.run(lm, firmware == null ? null : firmware.toString(), callbacks);
        } catch (Exception | LinkageError e) {
            if (e instanceof InterruptedException) {
                Thread.currentThread().interrupt();
            }
            callbacks.logLine("Firmware update failed: " + e);
            callbacks.error();
        }
        result.put("success", callbacks.done && !callbacks.failed);
        result.put("handoffStarted", callbacks.handoffStarted);
        result.put("port", lm.getLastTriedPort());
        BinaryProtocol after = lm.getBinaryProtocol();
        result.put("connected", lm.isActive() && after != null && !after.isClosed());
        result.put("signature", after == null ? null : after.signature);
        result.put("messages", callbacks.messages);
        result.put("warnings", callbacks.warnings);
        if (!callbacks.done || callbacks.failed) {
            result.put("error", "OpenBLT update/configuration migration did not complete. See messages; " +
                "firmware may already have been flashed if handoffStarted is true.");
        }
        return result;
    }

    private static void runProductionJob(LinkManager lm, String firmware, UpdateOperationCallbacks callbacks)
        throws Exception {
        String port = lm.getLastTriedPort();
        boolean socketCan = LinkManager.SOCKET_CAN.equals(port);
        if (port == null || (!socketCan && LinkManager.isSpecialNotSerial(port))) {
            throw new IllegalArgumentException("OpenBLT update requires a serial or SocketCAN ECU connection");
        }
        BinaryProtocol bp = lm.getBinaryProtocol();
        if (bp == null || bp.isClosed()) {
            throw new IllegalStateException("No live ECU connection to back up");
        }
        if (!FirmwareFlashEligibility.isAllowed(bp.signature, firmware, callbacks)) {
            callbacks.error();
            return;
        }
        final String requestedFirmware = firmware;
        if (firmware == null) {
            if (!MaintenanceUtil.ensureFirmwareForConnectedTarget(callbacks, lm.getConnectedEcuTarget())) {
                callbacks.error();
                return;
            }
            firmware = FindFileHelper.findSrecFileForConnectedBoard(lm.getConnectedEcuTarget());
        }
        Path image = validateFirmwarePath(firmware);
        if (!MaintenanceUtil.confirmFirmwareMatchesBoard(image.toString(), callbacks, lm.getConnectedEcuTarget())) {
            callbacks.error();
            return;
        }
        // The UI flasher requires a 64-bit JVM. Reject before backup/disconnect, without a dialog.
        if (!"64".equals(System.getProperty("sun.arch.data.model"))) {
            throw new IllegalStateException("OpenBLT update requires 64-bit Java");
        }
        callbacks.logLine("Firmware image: " + image);
        SerialPortScanner scanner = new SerialPortScanner(new EcuHardwareProbes(lm.getConnectedEcuTarget()), true);
        ConnectivityContext context = new ConnectivityContext(scanner, lm.getConnectedEcuTarget());
        PortResult original = new PortResult(port, socketCan ? SerialPortType.Ecu : SerialPortType.EcuWithOpenblt);
        // Never let the scanner probe the port already owned by the MCP connection.
        scanner.cachePort(original);
        scanner.addListener(hardware -> { });
        try {
            new OpenBltAutoJob(original, null, context, lm, requestedFirmware).doJob(callbacks, () -> { });
            if (lm.isActive()) {
                JobHelper.awaitBinaryProtocol(lm, callbacks);
            }
        } finally {
            try {
                scanner.suspend().await(30, TimeUnit.SECONDS);
            } finally {
                scanner.stopTimer();
            }
        }
    }

    static final class Callbacks implements UpdateOperationCallbacks {
        private final Runnable beforeHandoff;
        private final JSONArray messages = new JSONArray();
        private final JSONArray warnings = new JSONArray();
        private boolean done;
        private boolean failed;
        private boolean handoffStarted;

        Callbacks(Runnable beforeHandoff) {
            this.beforeHandoff = beforeHandoff;
        }

        @Override
        public boolean isInteractive() {
            return false;
        }

        @Override
        public void firmwareHandoffStarted() {
            beforeHandoff.run();
            handoffStarted = true;
        }

        @Override
        @SuppressWarnings("unchecked")
        public void log(String message, boolean breakLineOnTextArea, boolean sendToLogger) {
            log.info("update_firmware: " + message);
            // Keep backup paths and migration diagnostics in the response, but omit per-field chatter.
            if (!message.startsWith("To restore previous calibrations we are going to update the field")) {
                messages.add(message);
            }
            if (message.toUpperCase(Locale.ROOT).contains("WARNING")) {
                warnings.add(message);
            }
        }

        @Override public void done() { done = true; }
        @Override public void error() { failed = true; }
        @Override public void warning() { }
        @Override public void clear() { }
    }
}
