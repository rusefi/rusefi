package com.rusefi.maintenance;

import com.devexperts.logging.Logging;
import com.opensr5.ConfigurationImageMetaVersion0_0;
import com.opensr5.ConfigurationImageWithMeta;
import com.opensr5.ini.IniFileModel;
import com.rusefi.SerialPortScanner.PortResult;
import com.rusefi.binaryprotocol.BinaryProtocol;
import com.rusefi.io.UpdateOperationCallbacks;

import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.util.Optional;

import static com.devexperts.logging.Logging.getLogging;
import static com.rusefi.binaryprotocol.BinaryProtocol.iniFileProvider;
import static java.nio.file.StandardCopyOption.REPLACE_EXISTING;

public class CalibrationsBackuper {
    private static final Logging log = getLogging(CalibrationsBackuper.class);
    private static final String PREVIOUS_CALIBRATIONS_INI = "prev_calibrations.ini";
    private static final String PREVIOUS_CALIBRATIONS_BINARY = "prev_calibrations.zip";
    private static final String PREVIOUS_CALIBRATIONS_XML = "prev_calibrations.msq";

    private static class CalibrationsMeta {
        private final String iniFilePath;
        private final ConfigurationImageMetaVersion0_0 meta;

        CalibrationsMeta(final String iniFilePath, final ConfigurationImageMetaVersion0_0 meta) {
            this.iniFilePath = iniFilePath;
            this.meta = meta;
        }
    }

    private static Optional<CalibrationsMeta> readMeta(
        final BinaryProtocol binaryProtocol,
        final UpdateOperationCallbacks callbacks
    ) {
        try {
            final String signature = BinaryProtocol.getSignature(binaryProtocol.getStream());
            callbacks.logLine(String.format("Received a signature %s", signature));
            final IniFileModel iniFile = iniFileProvider.provide(signature);
            final int pageSize = iniFile.getMetaInfo().getTotalSize();
            callbacks.logLine(String.format("Page size is %d", pageSize));
            return Optional.of(new CalibrationsMeta(
                iniFile.getIniFilePath(),
                new ConfigurationImageMetaVersion0_0(pageSize, signature)
            ));
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
                    final Optional<CalibrationsMeta> meta = readMeta(binaryProtocol, callbacks);
                    if (meta.isPresent()) {
                        final CalibrationsMeta receivedMeta = meta.get();
                        callbacks.logLine("Reading current calibrations...");
                        final ConfigurationImageWithMeta image = binaryProtocol.readFullImageFromController(
                            receivedMeta.meta
                        );
                        final Path iniFilePath = Paths.get(receivedMeta.iniFilePath);
                        callbacks.logLine(String.format("Backing up current file %s...", iniFilePath));
                        Files.copy(
                            iniFilePath,
                            Paths.get(PREVIOUS_CALIBRATIONS_INI),
                            REPLACE_EXISTING
                        );
                        callbacks.logLine(String.format("%s file is backed up", iniFilePath.getFileName()));
                        callbacks.logLine("Save current calibrations to files...");
                        binaryProtocol.saveConfigurationImageToFiles(
                            image,
                            PREVIOUS_CALIBRATIONS_BINARY,
                            PREVIOUS_CALIBRATIONS_XML
                        );
                        callbacks.logLine("Current calibrations are backed up to files");
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
