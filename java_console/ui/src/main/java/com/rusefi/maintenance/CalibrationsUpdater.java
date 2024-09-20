package com.rusefi.maintenance;

import com.devexperts.logging.Logging;
import com.opensr5.ConfigurationImage;
import com.rusefi.SerialPortScanner;
import com.rusefi.io.LinkManager;
import com.rusefi.io.UpdateOperationCallbacks;

import java.util.concurrent.CountDownLatch;
import java.util.concurrent.TimeUnit;

import static com.devexperts.logging.Logging.getLogging;

public enum CalibrationsUpdater {
    INSTANCE;

    private static final Logging log = getLogging(CalibrationsUpdater.class);

    private volatile ConfigurationImage calibrationsToUpload = null;

    public void setCalibrationsToUpload(final ConfigurationImage calibrations) {
        calibrationsToUpload = calibrations;
    }

    public synchronized void updateCalibrations(
        final String port,
        final UpdateOperationCallbacks callbacks
    ) {
        final ConfigurationImage calibrationsImage = calibrationsToUpload;
        if (calibrationsImage != null) {
            final int calibrationsImageSize = calibrationsImage.getSize();
            try {
                callbacks.logLine("Suspending port scanning...");
                try {
                    SerialPortScanner.INSTANCE.suspend().await(1, TimeUnit.MINUTES);
                    callbacks.logLine("Port scanning is suspended.");
                } catch (final InterruptedException e) {
                    callbacks.logLine("Failed to  suspend port scanning in a minute.");
                    callbacks.error();
                    return;
                }

                try (LinkManager linkManager = new LinkManager()
                    .setNeedPullText(false)
                    .setNeedPullLiveData(true)) {

                    callbacks.logLine(String.format("Connecting to port %s...", port));
                    try {
                        linkManager.connect(port).await(60, TimeUnit.SECONDS);
                    } catch (final InterruptedException e) {
                        final String errorMsg = String.format("Failed to connect to port %s", port);
                        log.error(errorMsg, e);
                        callbacks.logLine(errorMsg);
                        callbacks.error();
                        return;
                    }
                    callbacks.logLine(String.format(
                        "Updating configuration image (%d bytes) to port %s...",
                        calibrationsImageSize,
                        port
                    ));
                    final CountDownLatch latch = new CountDownLatch(1);
                    linkManager.execute(() -> {
                        linkManager.getBinaryProtocol().uploadChanges(calibrationsImage);
                        latch.countDown();
                    });
                    try {
                        if (!latch.await(1, TimeUnit.MINUTES)) {
                            callbacks.logLine(String.format(
                                "Failed to update configuration image (%d bytes) to port %s in a minute",
                                calibrationsImageSize,
                                port
                            ));
                            callbacks.error();
                        } else {
                            callbacks.logLine(String.format(
                                "Configuration image (%d bytes) has been uploaded to port %s",
                                calibrationsImageSize,
                                port
                            ));
                            callbacks.done();
                        }
                    } catch (final InterruptedException e) {
                        final String errorMsg = String.format(
                            "Updating calibrations to port %s was interrupted",
                            port
                        );
                        log.error(errorMsg, e);
                        callbacks.logLine(errorMsg);
                        callbacks.error();
                    }
                }
            } finally {
                callbacks.logLine("Resuming port scanning...");
                SerialPortScanner.INSTANCE.resume();
                callbacks.logLine("Port scanning is resumed.");
            }
        } else {
            callbacks.logLine("ERROR: Calibrations to update are undefined");
        }
    }
}
