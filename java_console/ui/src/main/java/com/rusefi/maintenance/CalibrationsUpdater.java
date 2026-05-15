package com.rusefi.maintenance;

import com.opensr5.ConfigurationImage;
import com.rusefi.ConnectivityContext;
import com.rusefi.binaryprotocol.BinaryProtocol;
import com.rusefi.io.LinkManager;
import com.rusefi.io.UpdateOperationCallbacks;
import com.rusefi.maintenance.jobs.JobHelper;

import java.util.concurrent.ExecutionException;

public enum CalibrationsUpdater {
    INSTANCE;

    public synchronized void updateCalibrations(
        final String port,
        final ConfigurationImage calibrationsImage,
        final UpdateOperationCallbacks callbacks,
        final Runnable onJobFinished, ConnectivityContext connectivityContext
    ) {
        JobHelper.doJob(() -> {
            if (updateCalibrations(port, calibrationsImage, callbacks, connectivityContext)) {
                callbacks.done();
            } else {
                callbacks.error();
            }
        }, onJobFinished);
    }

    public synchronized boolean updateCalibrations(
        final String port,
        final ConfigurationImage calibrationsImage,
        final UpdateOperationCallbacks callbacks, ConnectivityContext connectivityContext
    ) {
        boolean result = false;
        if (calibrationsImage != null) {
            final int calibrationsImageSize = calibrationsImage.getSize();
            callbacks.logLine(String.format(
                "Updating configuration image (%d bytes) to port %s...",
                calibrationsImageSize,
                port
            ));
            result = BinaryProtocolExecutor.executeWithSuspendedPortScanner(port, callbacks, binaryProtocol -> {
                try {
                    binaryProtocol.uploadChanges(calibrationsImage);
                    return true;
                } catch (IllegalStateException e) {
                    callbacks.logLine("ERROR: " + e.getMessage());
                    return false;
                }
            }, false, connectivityContext, "uploadChanges");
            if (result) {
                callbacks.logLine(String.format(
                    "Configuration image (%d bytes) has been uploaded to port %s",
                    calibrationsImageSize,
                    port
                ));
            } else {
                callbacks.logLine(String.format(
                    "Failed to update configuration image (%d bytes) to port %s in a minute",
                    calibrationsImageSize,
                    port
                ));
            }
        } else {
            callbacks.logLine("ERROR: Calibrations to update are undefined");
        }
        return result;
    }

    /**
     * Uploads calibrations via a live {@link BinaryProtocol} without opening a fresh connection.
     * All IO is submitted through {@link LinkManager#submit} so it is serialized with the pull thread.
     * The caller must NOT be on the Swing EDT.
     */
    public synchronized boolean updateCalibrations(
        final BinaryProtocol bp,
        final LinkManager lm,
        final ConfigurationImage calibrationsImage,
        final UpdateOperationCallbacks callbacks
    ) {
        if (calibrationsImage == null) {
            callbacks.logLine("ERROR: Calibrations to update are undefined");
            return false;
        }
        final int size = calibrationsImage.getSize();
        callbacks.logLine(String.format("Updating configuration image (%d bytes) via live connection...", size));
        final boolean[] result = {false};
        try {
            lm.submit(() -> {
                try {
                    bp.uploadChanges(calibrationsImage);
                    result[0] = true;
                } catch (IllegalStateException e) {
                    callbacks.logLine("ERROR: " + e.getMessage());
                }
            }).get();
        } catch (ExecutionException | InterruptedException e) {
            if (e instanceof InterruptedException) Thread.currentThread().interrupt();
            callbacks.logLine("Failed to upload configuration image: " + e.getMessage());
            return false;
        }
        if (result[0]) {
            callbacks.logLine(String.format("Configuration image (%d bytes) uploaded", size));
        } else {
            callbacks.logLine(String.format("Failed to update configuration image (%d bytes) via live connection", size));
        }
        return result[0];
    }
}
