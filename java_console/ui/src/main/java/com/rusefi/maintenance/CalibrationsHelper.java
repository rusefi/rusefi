package com.rusefi.maintenance;

import com.devexperts.logging.Logging;
import com.opensr5.ConfigurationImage;
import com.opensr5.ConfigurationImageGetterSetter;
import com.opensr5.ConfigurationImageMetaVersion0_0;
import com.opensr5.ConfigurationImageWithMeta;
import com.opensr5.ini.IniFileModel;
import com.opensr5.ini.field.*;
import com.rusefi.ConnectivityContext;
import com.rusefi.PortResult;
import com.rusefi.SerialPortType;
import com.rusefi.binaryprotocol.BinaryProtocol;
import com.rusefi.binaryprotocol.BinaryProtocolLocalCache;
import com.rusefi.binaryprotocol.IniNotFoundException;
import com.rusefi.core.ui.AutoupdateUtil;
import com.rusefi.io.UpdateOperationCallbacks;
import com.rusefi.maintenance.migration.migrators.ComposedTuneMigrator;
import com.rusefi.maintenance.migration.TuneMigrationContext;
import com.rusefi.tune.xml.Constant;
import com.rusefi.tune.xml.Msq;
import com.rusefi.ui.basic.MigrateSettingsCheckboxState;

import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.text.SimpleDateFormat;
import java.util.*;
import java.util.function.Supplier;
import java.util.stream.Collectors;

import static com.devexperts.logging.Logging.getLogging;
import static com.rusefi.maintenance.CallbacksWaitingUtil.waitForPredicate;
import static com.rusefi.util.TuneBackupUtil.saveConfigurationImageToFiles;
import static java.nio.file.StandardCopyOption.REPLACE_EXISTING;
import static java.util.Collections.emptySet;

public class CalibrationsHelper {
    private static final Logging log = getLogging(CalibrationsHelper.class);

    static final SimpleDateFormat DATE_FORMAT = new SimpleDateFormat("yyyy-MM-dd-HH.mm.ss");
    private static final String RUSEFI_FORCE_CALIBRATIONS_RESTORE = System.getenv("RUSEFI_FORCE_CALIBRATIONS_RESTORE");

    public static void main(String[] args) {
//        args = new String[] {"destinationFileName.xml", "COM34"};
        if (args.length != 2) {
            System.err.println("File name and port are expected as command line arguments!");
        } else {
            final String destinationFileName = args[0];
            final String port = args[1];
            Optional<CalibrationsInfo> calibrationsInfo = readAndBackupCurrentCalibrations(port, UpdateOperationCallbacks.CONSOLE, destinationFileName);
            if (!calibrationsInfo.isPresent()) {
                System.err.printf("Failed to read current calibrations from %s port%n", port);
            } else {
                backUpCalibrationsInfo(calibrationsInfo.get(), destinationFileName + "_", UpdateOperationCallbacks.CONSOLE);
            }
        }
    }

    private static List<PortResult> waitForPortAppeared(
        final SerialPortType desiredPortType,
        final UpdateOperationCallbacks callbacks,
        final ConnectivityContext connectivityContext
    ) {
        final List<PortResult> foundPorts = new ArrayList<>();
        waitForPredicate(
            String.format("Waiting for %s port to appear...", desiredPortType),
            () -> {
                foundPorts.addAll(connectivityContext.getCurrentHardware().getKnownPorts(desiredPortType));
                return !foundPorts.isEmpty();
            },
            callbacks
        );
        return foundPorts;
    }


    public static boolean updateFirmwareAndRestorePreviousCalibrations(
        final PortResult originalEcuPort,
        final UpdateOperationCallbacks callbacks,
        final Supplier<Boolean> updateFirmware,
        final ConnectivityContext connectivityContext
    ) {
        AutoupdateUtil.assertNotAwtThread();

        final String timestampFileNameComponent = DATE_FORMAT.format(new Date());

        final Optional<CalibrationsInfo> prevCalibrations = readAndBackupCurrentCalibrationsWithSuspendedPortScanner(
            originalEcuPort.port,
            callbacks,
            getFileNameWithoutExtension(timestampFileNameComponent, "backup_from_ecu"), connectivityContext
        );
        if (!prevCalibrations.isPresent()) {
            callbacks.logLine("Failed to back up current tune from ECU...");
            return false;
        }

        if (!updateFirmware.get()) {
            return false;
        }

        final List<PortResult> ecuPortsAfterFirmwareUpdate = waitForPortAppeared(
            originalEcuPort.type,
            callbacks,
            connectivityContext
        );
        switch (ecuPortsAfterFirmwareUpdate.size()) {
            case 0: {
                callbacks.logLine("No ECU found after firmware update...");
                return false;
            }
            case 1: {
                final PortResult newEcuPort = ecuPortsAfterFirmwareUpdate.get(0);
                callbacks.logLine(String.format(
                    "ECU is found on port %s after firmware update.",
                    newEcuPort.port
                ));

                final Optional<CalibrationsInfo> updatedCalibrations = readAndBackupCurrentCalibrationsWithSuspendedPortScanner(
                    newEcuPort.port,
                    callbacks,
                    getFileNameWithoutExtension(timestampFileNameComponent, "after_firmware_update"), connectivityContext
                );
                if (!updatedCalibrations.isPresent()) {
                    callbacks.logLine("Failed to back up tune from ECU after firmware update...");
                    return false;
                }
                final Optional<CalibrationsInfo> mergedCalibrations = mergeCalibrations(
                    prevCalibrations.get().getIniFile(),
                    prevCalibrations.get().generateMsq(),
                    updatedCalibrations.get(),
                    callbacks,
                    emptySet()
                );
                if (mergedCalibrations.isPresent() && MigrateSettingsCheckboxState.isMigrationNeeded) {
                    if (!backUpCalibrationsInfo(
                        mergedCalibrations.get(),
                        getFileNameWithoutExtension(timestampFileNameComponent, "merged_to_write"),
                        callbacks
                    )) {
                        callbacks.logLine("Failed to back up merged tune before writing to ECU...");
                        return false;
                    }
                    if (!CalibrationsUpdater.INSTANCE.updateCalibrations(
                        newEcuPort.port,
                        mergedCalibrations.get().getImage().getConfigurationImage(),
                        callbacks,
                        connectivityContext
                    )) {
                        callbacks.logLine("Failed to write merged tune to ECU...");
                        return false;
                    }
                    final Optional<CalibrationsInfo> mergedTuneFromEcu = readAndBackupCurrentCalibrationsWithSuspendedPortScanner(
                        newEcuPort.port,
                        callbacks,
                        getFileNameWithoutExtension(timestampFileNameComponent, "merged_from_ecu"),
                        connectivityContext
                    );
                    if (!mergedTuneFromEcu.isPresent()) {
                        callbacks.logLine("Failed to back up merged tune from ECU...");
                        return false;
                    }
                }
                return true;
            }
            default: {
                callbacks.logLine(String.format(
                    "Multiple ECU-s are found after firmware update on ports %s...",
                    ecuPortsAfterFirmwareUpdate.stream()
                        .map(portResult -> portResult.port)
                        .collect(Collectors.joining(", "))
                ));
                return false;
            }
        }
    }

    public static boolean importTune(
        final String ecuPort,
        final Msq msqToImport,
        final UpdateOperationCallbacks callbacks,
        final ConnectivityContext connectivityContext
    ) {
        AutoupdateUtil.assertNotAwtThread();

        final String signature = msqToImport.versionInfo.getSignature();
        final IniFileModel iniFileToImport;
        try {
            iniFileToImport = BinaryProtocol.iniFileProvider.provide(signature);
        } catch (IniNotFoundException e) {
            callbacks.logLine(String.format("We failed to get .ini file for signature `%s`", signature));
            return false;
        }

        final String timestampFileNameComponent = DATE_FORMAT.format(new Date());

        if (!backupTune(
            iniFileToImport,
            msqToImport,
            getFileNameWithoutExtension(timestampFileNameComponent, "tune_to_import"),
            callbacks
        )) {
            callbacks.logLine("Failed to back up tune to import...");
            return false;
        }

        final Optional<CalibrationsInfo> prevTune = readAndBackupCurrentCalibrationsWithSuspendedPortScanner(
            ecuPort,
            callbacks,
            getFileNameWithoutExtension(timestampFileNameComponent, "backup_from_ecu"), connectivityContext
        );
        if (!prevTune.isPresent()) {
            callbacks.logLine("Failed to back up current tune from ECU...");
            return false;
        }

        final Optional<CalibrationsInfo> mergedTune = mergeCalibrations(
            iniFileToImport,
            msqToImport,
            prevTune.get(),
            callbacks,
            // we do not want to import `vinNumber` ini-field
            new HashSet<>(Collections.singletonList("vinNumber"))
        );
        if (mergedTune.isPresent()) {
            if (!backUpCalibrationsInfo(
                mergedTune.get(),
                getFileNameWithoutExtension(timestampFileNameComponent, "merged_to_write"),
                callbacks
            )) {
                callbacks.logLine("Failed to back up merged tune...");
                return false;
            }
            if (!CalibrationsUpdater.INSTANCE.updateCalibrations(
                ecuPort,
                mergedTune.get().getImage().getConfigurationImage(),
                callbacks,
                connectivityContext
            )) {
                callbacks.logLine("Failed to write merged tune to ECU...");
                return false;
            }
            final Optional<CalibrationsInfo> mergedTuneFromEcu = readAndBackupCurrentCalibrationsWithSuspendedPortScanner(
                ecuPort,
                callbacks,
                getFileNameWithoutExtension(timestampFileNameComponent, "merged_from_ecu"),
                connectivityContext
            );
            if (!mergedTuneFromEcu.isPresent()) {
                callbacks.logLine("Failed to back up merged tune from ECU...");
                return false;
            }
        }
        return true;
    }

    static String getFileNameWithoutExtension(
        final String timestampNameComponent,
        final String fileNameComponent
    ) {
        return String.format("%s%s_%s",
            BinaryProtocolLocalCache.STATE_FOLDER,
            timestampNameComponent,
            fileNameComponent
        );
    }

    private static Optional<CalibrationsInfo> readCalibrationsInfo(
        final BinaryProtocol binaryProtocol,
        final UpdateOperationCallbacks callbacks) {
        try {
            final String signature = BinaryProtocol.getSignature(binaryProtocol.getStream());
            if (signature == null) {
                callbacks.logLine("No response from " + binaryProtocol.getStream());
                return Optional.empty();
            }
            callbacks.logLine(String.format("Received a signature %s", signature));
            final IniFileModel iniFile;
            try {
                iniFile = BinaryProtocol.iniFileProvider.provide(signature);
            } catch (IniNotFoundException e) {
                throw new RuntimeException(e);
            }
            Objects.requireNonNull(iniFile);
            final int pageSize = iniFile.getMetaInfo().getPageSize(0);
            callbacks.logLine(String.format("Page size is %d", pageSize));
            final ConfigurationImageMetaVersion0_0 meta = ConfigurationImageMetaVersion0_0.getMeta(iniFile);
            callbacks.logLine("Reading current calibrations...");
            final ConfigurationImageWithMeta image = binaryProtocol.readFullImageFromController(meta);
            return Optional.of(new CalibrationsInfo(iniFile, image));
        } catch (final IOException e) {
            log.error("Failed to read meta:", e);
            callbacks.logLine("Failed to read meta");
            return Optional.empty();
        }
    }

    private static boolean backupTune(
        final IniFileModel ini,
        final Msq msq,
        final String fileName,
        final UpdateOperationCallbacks callbacks
    )  {
        try {
            final String iniFileName = String.format("%s.ini", fileName);
            final Path iniFilePath = Paths.get(ini.getIniFilePath());
            callbacks.logLine(String.format("Backing up ini-file `%s`...", iniFilePath));
            Files.copy(
                iniFilePath,
                Paths.get(iniFileName),
                REPLACE_EXISTING
            );
            callbacks.logLine(String.format(
                "`%s` ini-file is backed up as `%s`",
                iniFilePath.getFileName(),
                iniFileName
            ));

            final String msqFileName = String.format("%s.msq", fileName);
            callbacks.logLine(String.format("Backing up msq-file to %s...", msqFileName));
            msq.writeXmlFile(msqFileName);
            callbacks.logLine(String.format("msq-file is backed up as `%s`", msqFileName));

            return true;
        } catch (final Exception e) {
            log.error("Backing up tune failed:", e);
            callbacks.logLine("Backing up tune failed: " + e);
            return false;
        }
    }

    public static boolean backUpCalibrationsInfo(
        final CalibrationsInfo calibrationsInfo,
        final String fileName,
        final UpdateOperationCallbacks callbacks
    ) {
        try {
            final String iniFileName = String.format("%s.ini", fileName);
            final IniFileModel ini = calibrationsInfo.getIniFile();
            final Path iniFilePath = Paths.get(ini.getIniFilePath());
            callbacks.logLine(String.format("Backing up current ini-file `%s`...", iniFilePath));
            Files.copy(
                iniFilePath,
                Paths.get(iniFileName),
                REPLACE_EXISTING
            );
            callbacks.logLine(String.format(
                "`%s` ini-file is backed up as `%s`",
                iniFilePath.getFileName(),
                iniFileName
            ));

            final String zipFileName = String.format("%s.zip", fileName);
            final String msqFileName = String.format("%s.msq", fileName);
            callbacks.logLine(String.format(
                "Backing up calibrations to files `%s` and `%s`...",
                zipFileName,
                msqFileName
            ));
            saveConfigurationImageToFiles(calibrationsInfo.getImage(), ini, zipFileName, msqFileName);
            callbacks.logLine(String.format(
                "Calibrations are backed up to files `%s` and `%s`",
                zipFileName,
                msqFileName
            ));
            return true;
        } catch (final Exception e) {
            log.error("Backing up calibrations failed:", e);
            callbacks.logLine("Backing up current calibrations failed: " + e);
            return false;
        }
    }

    public static Optional<CalibrationsInfo> readCurrentCalibrations(
        final String port,
        final UpdateOperationCallbacks callbacks, ConnectivityContext connectivityContext
    ) {
        return BinaryProtocolExecutor.executeWithSuspendedPortScanner(
            port,
            callbacks,
            (binaryProtocol) -> {
                try {
                    return readCalibrationsInfo(binaryProtocol, callbacks);
                } catch (final Exception e) {
                    log.error("Failed to read current calibrations:", e);
                    callbacks.logLine("Failed to read current calibrations");
                    return Optional.empty();
                }
            },
            Optional.empty(), connectivityContext,
            "readCalibrationsInfo");
    }

    public static Optional<CalibrationsInfo> readCurrentCalibrationsWithoutSuspendingPortScanner(
        final String port,
        final UpdateOperationCallbacks callbacks
    ) {
        return BinaryProtocolExecutor.execute(
            port,
            callbacks,
            (binaryProtocol) -> {
                try {
                    return readCalibrationsInfo(binaryProtocol, callbacks);
                } catch (final Exception e) {
                    log.error("Failed to read current calibrations:", e);
                    callbacks.logLine("Failed to read current calibrations");
                    return Optional.empty();
                }
            },
            Optional.empty(),
            true,
            "readCalibrationsInfo");
    }

    public static Optional<CalibrationsInfo> readAndBackupCurrentCalibrationsWithSuspendedPortScanner(
        final String ecuPort,
        final UpdateOperationCallbacks callbacks,
        final String backupFileName,
        final ConnectivityContext connectivityContext
    ) {
        return BinaryProtocolExecutor.executeWithSuspendedPortScanner(
            ecuPort,
            callbacks,
            binaryProtocol -> readAndBackupCurrentCalibrations(binaryProtocol, callbacks, backupFileName),
            Optional.empty(),
            connectivityContext,
            "readAndBackupCurrentCalibrations");
    }

    private static Optional<CalibrationsInfo> readAndBackupCurrentCalibrations(
        final String ecuPort,
        final UpdateOperationCallbacks callbacks,
        final String backupFileName
    ) {
        return BinaryProtocolExecutor.execute(
            ecuPort,
            callbacks,
            binaryProtocol -> readAndBackupCurrentCalibrations(binaryProtocol, callbacks, backupFileName),
            Optional.empty(),
            false,
            "readAndBackupCurrentCalibrations");
    }

    private static Optional<CalibrationsInfo> readAndBackupCurrentCalibrations(
        final BinaryProtocol binaryProtocol,
        final UpdateOperationCallbacks callbacks,
        final String backupFileName
    ) {
        try {
            final Optional<CalibrationsInfo> calibrationsInfo = readCalibrationsInfo(binaryProtocol, callbacks);
            if (calibrationsInfo.isPresent()) {
                final CalibrationsInfo receivedCalibrations = calibrationsInfo.get();
                if (backUpCalibrationsInfo(
                    receivedCalibrations,
                    backupFileName,
                    callbacks
                )) {
                    return calibrationsInfo;
                }
            }
            return Optional.empty();
        } catch (final Exception e) {
            log.error("Back up current calibrations failed:", e);
            callbacks.logLine("Back up current calibrations failed");
            return Optional.empty();
        }
    }

    public static Optional<CalibrationsInfo> mergeCalibrations(
        final IniFileModel prevIniFile,
        final Msq prevMsq,
        final CalibrationsInfo newCalibrations,
        final UpdateOperationCallbacks callbacks,
        final Set<String> additionalIniFieldsToIgnore
    ) {
        Optional<CalibrationsInfo> result = Optional.empty();
        final IniFileModel newIniFile = newCalibrations.getIniFile();
        final Msq newMsq = newCalibrations.generateMsq();

        final TuneMigrationContext context = new TuneMigrationContext(
            prevIniFile,
            prevMsq,
            newIniFile,
            newMsq,
            callbacks,
            additionalIniFieldsToIgnore
        );
        ComposedTuneMigrator.INSTANCE.migrateTune(context);
        final Set<Map.Entry<String, Constant>> valuesToUpdate = context.getMigratedConstants().entrySet();
        if (!valuesToUpdate.isEmpty()) {
            final ConfigurationImage mergedImage = newCalibrations.getImage().getConfigurationImage().clone();
            for (final Map.Entry<String, Constant> valueToUpdate : valuesToUpdate) {
                final String migratedFieldName = valueToUpdate.getKey();
                final Constant migratedValue = valueToUpdate.getValue();
                final Optional<IniField> fieldToUpdate = newIniFile.findIniField(migratedFieldName);
                if (fieldToUpdate.isPresent()) {
                    try {
                        ConfigurationImageGetterSetter.setValue(fieldToUpdate.get(), mergedImage, migratedValue);
                    } catch (Throwable e) {
                        log.error(
                            String.format("We failed to set value %s for ini-field %s", migratedValue, fieldToUpdate),
                            e
                        );
                        throw new IllegalStateException("Unexpected during " + migratedValue, e);
                    }
                    callbacks.logLine(String.format(
                        "To restore previous calibrations we are going to update the field `%s` with a value `%s`",
                        migratedFieldName,
                        migratedValue.getValue()
                    ));
                } else if (null == migratedValue) {
                    log.info(String.format(
                        "Disappeared `%s` field has been already migrated by one of migrators",
                        migratedFieldName
                    ));
                } else {
                    callbacks.logLine(String.format(
                        "WARNING!!! To restore previous calibrations we want to update the field `%s` with a value " +
                            "`%s`, but this field has disappeared in updated .ini file",
                        migratedFieldName,
                        migratedValue.getValue()
                    ));
                }
            }
            result = Optional.of(new CalibrationsInfo(
                newIniFile,
                new ConfigurationImageWithMeta(newCalibrations.getImage().getMeta(), mergedImage.getContent())
            ));
        } else if ("true".equalsIgnoreCase(RUSEFI_FORCE_CALIBRATIONS_RESTORE)) {
            callbacks.logLine("It looks like we do not need to update previous calibrations, but for debugging we are going to rewrite to ECU new calibrations again.");
            result = Optional.of(newCalibrations);
        } else {
            callbacks.logLine("It looks like we do not need to update any fields to restore previous calibrations.");
        }
        return result;
    }
}
