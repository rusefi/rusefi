package com.rusefi.maintenance;

import com.devexperts.logging.Logging;
import com.opensr5.ConfigurationImageMetaVersion0_0;
import com.opensr5.ConfigurationImageWithMeta;
import com.opensr5.ini.IniFileModel;
import com.rusefi.SerialPortScanner.PortResult;
import com.rusefi.binaryprotocol.BinaryProtocol;
import com.rusefi.io.UpdateOperationCallbacks;

import java.io.IOException;
import java.util.Optional;

import static com.devexperts.logging.Logging.getLogging;
import static com.rusefi.binaryprotocol.BinaryProtocol.iniFileProvider;

public class CalibrationsBackuper {
    private static final Logging log = getLogging(CalibrationsBackuper.class);
    private static final String PREVIOUS_CALIBRATIONS_BINARY = "prev_calibrations.zip";
    private static final String PREVIOUS_CALIBRATIONS_XML = "prev_calibrations.msq";

    private static Optional<ConfigurationImageMetaVersion0_0> readMeta(
        final BinaryProtocol binaryProtocol,
        final UpdateOperationCallbacks callbacks
    ) {
        try {
            final String signature = BinaryProtocol.getSignature(binaryProtocol.getStream());
            callbacks.logLine(String.format("Received a signature %s", signature));
            final IniFileModel iniFile = iniFileProvider.provide(signature);
            final int pageSize = iniFile.getMetaInfo().getTotalSize();
            callbacks.logLine(String.format("Page size is %d", pageSize));
            return Optional.of(new ConfigurationImageMetaVersion0_0(pageSize, signature));
        } catch (final IOException e) {
            log.error("Failed to read meta:", e);
            callbacks.logLine("Failed to read meta");
            return Optional.empty();
        }
    }

    public static boolean backUpCurrentCalibrations(
        final PortResult ecuPort,
        final UpdateOperationCallbacks callbacks
    ) {
        return BinaryProtocolExecutor.executeWithSuspendedPortScanner(
            ecuPort.port,
            callbacks,
            (binaryProtocol) -> {
                try {
                    final Optional<ConfigurationImageMetaVersion0_0> meta = readMeta(binaryProtocol, callbacks);
                    if (meta.isPresent()) {
                        callbacks.logLine("Reading current calibrations...");
                        final ConfigurationImageWithMeta image = binaryProtocol.readFullImageFromController(meta.get());
                        callbacks.logLine("Save current calibrations to files...");
                        binaryProtocol.saveConfigurationImageToFiles(
                            image,
                            PREVIOUS_CALIBRATIONS_BINARY,
                            PREVIOUS_CALIBRATIONS_XML
                        );
                        callbacks.logLine("Current calibrations are saved to files");
                        return true;
                    } else {
                        return false;
                    }
                } catch (final Exception e) {
                    log.error("Back up current calibrations failed:", e);
                    callbacks.logLine("Back up current calibrations failed");
                    return false;
                }
            }
        );
    }
}
