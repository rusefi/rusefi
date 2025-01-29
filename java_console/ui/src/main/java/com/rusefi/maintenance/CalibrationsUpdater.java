package com.rusefi.maintenance;

import com.opensr5.ConfigurationImage;
import com.rusefi.io.UpdateOperationCallbacks;
import com.rusefi.maintenance.jobs.JobHelper;

public enum CalibrationsUpdater {
    INSTANCE;

    public synchronized void updateCalibrations(
        final String port,
        final ConfigurationImage calibrationsImage,
        final UpdateOperationCallbacks callbacks,
        final Runnable onJobFinished
    ) {
        JobHelper.doJob(() -> {
            updateCalibrations(port, calibrationsImage, callbacks);
        }, onJobFinished);
    }

    private synchronized void updateCalibrations(
        final String port,
        final ConfigurationImage calibrationsImage,
        final UpdateOperationCallbacks callbacks
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
                binaryProtocol.uploadChanges(calibrationsImage);
                return true;
            });
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
        if (result) {
            callbacks.done();
        } else {
            callbacks.error();
        }
    }
}
