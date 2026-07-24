package com.rusefi.maintenance;

import com.devexperts.logging.Logging;
import com.opensr5.*;
import com.opensr5.ini.IniFileModel;
import com.opensr5.ini.field.*;
import com.rusefi.ConnectivityContext;
import com.rusefi.PortResult;
import com.rusefi.SerialPortType;
import com.rusefi.binaryprotocol.BinaryProtocol;
import com.rusefi.binaryprotocol.BinaryProtocolLocalCache;
import com.rusefi.binaryprotocol.IniNotFoundException;
import com.rusefi.config.generated.Integration;
import com.rusefi.core.RusEfiSignature;
import com.rusefi.core.SignatureHelper;
import com.rusefi.core.ui.AutoupdateUtil;
import com.rusefi.core.ui.CalibrationBackupFailureAction;
import com.rusefi.core.ui.CalibrationBackupFailureDialog;
import com.rusefi.io.LinkManager;
import com.rusefi.io.UpdateOperationCallbacks;
import org.jetbrains.annotations.Nullable;
import com.rusefi.maintenance.migration.migrators.ComposedTuneMigrator;
import com.rusefi.maintenance.migration.TuneMigrationContext;
import com.rusefi.tune.ConfigurationImageGetterSetter2;
import com.rusefi.tune.xml.Constant;
import com.rusefi.tune.xml.Msq;
import com.rusefi.ui.basic.MigrateSettingsCheckboxState;

import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.text.SimpleDateFormat;
import java.util.*;
import java.util.concurrent.ExecutionException;
import java.util.function.Predicate;
import java.util.function.Supplier;
import java.util.stream.Collectors;

import javax.swing.JComponent;
import javax.swing.SwingUtilities;

import static com.devexperts.logging.Logging.getLogging;
import static com.rusefi.maintenance.CallbacksWaitingUtil.waitForPredicate;
import static com.rusefi.util.TuneBackupUtil.saveConfigurationImageToFiles;
import static java.nio.file.StandardCopyOption.REPLACE_EXISTING;
import static java.util.Collections.emptySet;

public class CalibrationsHelper {
    private static final Logging log = getLogging(CalibrationsHelper.class);

    static final SimpleDateFormat DATE_FORMAT = new SimpleDateFormat("yyyy-MM-dd-HH.mm.ss");
    private static final String RUSEFI_FORCE_CALIBRATIONS_RESTORE = System.getenv("RUSEFI_FORCE_CALIBRATIONS_RESTORE");
    private static final int MAX_CALIBRATION_RETRIES = 3;

    /** Most recent tune successfully read+backed up off a live ECU this session. Used to restore the
     *  tune after a manual OpenBLT flash, where the board is already in the bootloader before flashing
     *  (no live ECU to read pre-flash like the auto path does). [tag:better_ux_for_flashing] */
    private static volatile Optional<CalibrationsInfo> lastEcuCalibrations = Optional.empty();

    public enum FirmwareUpdatePolicy {
        FORWARD_MIGRATION,
        ROLLBACK_RESTORE_OR_RESET
    }

    public static class MergeResult {
        public final Optional<CalibrationsInfo> mergedCalibrations;
        public final List<String> failedFields;

        public MergeResult(Optional<CalibrationsInfo> mergedCalibrations, List<String> failedFields) {
            this.mergedCalibrations = mergedCalibrations;
            this.failedFields = failedFields;
        }
    }

    enum PostMergeDecision {
        /** merged calibration is usable - write it to the ECU */
        WRITE_MERGED,
        /** leave the freshly-flashed ECU on its firmware defaults */
        RESET_TO_DEFAULTS,
        /** user aborted the firmware update */
        CANCEL,
        /** re-read the calibration from the ECU and merge again */
        RETRY
    }

    /**
     *
     * <ul>
     *     <li>no failed fields -> strict-equivalent success, write the merged tune (action ignored)</li>
     *     <li>CANCEL -> abort; RESET_ALL -> defaults; RETRY -> re-read and merge again</li>
     *     <li>SKIP_FAILED_FIELDS -> write the partial merge if usable, else fall back to defaults</li>
     * </ul>
     */
    static PostMergeDecision decidePostMerge(final MergeResult mergeResult, final CalibrationBackupFailureAction action) {
        if (mergeResult.failedFields.isEmpty()) {
            return PostMergeDecision.WRITE_MERGED;
        }
        if (action == CalibrationBackupFailureAction.CANCEL) {
            return PostMergeDecision.CANCEL;
        }
        if (action == CalibrationBackupFailureAction.RESET_ALL) {
            return PostMergeDecision.RESET_TO_DEFAULTS;
        }
        if (action == CalibrationBackupFailureAction.RETRY) {
            return PostMergeDecision.RETRY;
        }
        // SKIP_FAILED_FIELDS: accept the partial merge if usable, otherwise reset to defaults
        return mergeResult.mergedCalibrations.isPresent()
            ? PostMergeDecision.WRITE_MERGED
            : PostMergeDecision.RESET_TO_DEFAULTS;
    }

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
        return waitForPortAppeared(
            t -> t == desiredPortType,
            String.format("Waiting for %s port to appear...", desiredPortType),
            callbacks,
            connectivityContext
        );
    }

    private static List<PortResult> waitForPortAppeared(
        final Predicate<SerialPortType> portTypeMatches,
        final String description,
        final UpdateOperationCallbacks callbacks,
        final ConnectivityContext connectivityContext
    ) {
        final List<PortResult> foundPorts = new ArrayList<>();
        waitForPredicate(
            description,
            () -> {
                final List<PortResult> knownPorts = connectivityContext.getCurrentHardware().getKnownPorts();
                final Set<String> osPorts = LinkManager.getCommPorts();
                // Invalidate any cached port that the OS no longer reports — forces the scanner
                // to re-probe on the next cycle and prevents returning stale results (e.g. a
                // port that vanished after an ECU reboot and re-enumeration).
                for (PortResult p : knownPorts) {
                    if (!osPorts.contains(p.port)) {
                        connectivityContext.getPortScanner().invalidatePort(p.port);
                    }
                }
                final List<PortResult> matching = knownPorts.stream()
                    .filter(p -> portTypeMatches.test(p.type))
                    .filter(p -> osPorts.contains(p.port))
                    .collect(Collectors.toList());
                foundPorts.addAll(matching);
                if (!foundPorts.isEmpty()) {
                    return true;
                }
                // Post-flash the scanner can probe the port while the ECU firmware is still
                // booting and classify it as Unknown (or stale EcuWithOpenblt from before the
                // reboot). The cache is otherwise sticky until the OS port disappears — force
                // a re-probe by invalidating non-matching classifications so the scanner's
                // next cycle re-inspects.
                for (PortResult p : knownPorts) {
                    if (!portTypeMatches.test(p.type)) {
                        connectivityContext.getPortScanner().invalidatePort(p.port);
                    }
                }
                return false;
            },
            callbacks
        );
        return foundPorts;
    }

    public static boolean updateFirmwareAndRestorePreviousCalibrations(
        final JComponent parent,
        final PortResult originalEcuPort,
        @Nullable final BinaryProtocol bp,
        @Nullable final LinkManager lm,
        final UpdateOperationCallbacks callbacks,
        final Supplier<Boolean> updateFirmware,
        final ConnectivityContext connectivityContext
    ) {
        return updateFirmwareAndRestorePreviousCalibrations(
            parent, originalEcuPort, bp, lm, callbacks, updateFirmware, connectivityContext,
            FirmwareUpdatePolicy.FORWARD_MIGRATION);
    }

    public static boolean updateFirmwareAndRestorePreviousCalibrations(
        final JComponent parent,
        final PortResult originalEcuPort,
        @Nullable final BinaryProtocol bp,
        @Nullable final LinkManager lm,
        final UpdateOperationCallbacks callbacks,
        final Supplier<Boolean> updateFirmware,
        final ConnectivityContext connectivityContext,
        final FirmwareUpdatePolicy policy
    ) {
        AutoupdateUtil.assertNotAwtThread();

        final String timestampFileNameComponent = DATE_FORMAT.format(new Date());
        final String backupName = getFileNameWithoutExtension(timestampFileNameComponent, "backup_from_ecu");

        final Optional<CalibrationsInfo> prevCalibrations;
        boolean skipCalibrationRestore = false;

        int retryCount = 0;
        Optional<CalibrationsInfo> calibrations = Optional.empty();
        List<String> backupFailedFields = new ArrayList<>();

        while (retryCount <= MAX_CALIBRATION_RETRIES) {
            if (retryCount > 0) {
                callbacks.logLine("Retrying calibration backup (attempt " + retryCount + " of " + MAX_CALIBRATION_RETRIES + ")...");
                BinaryProtocol.sleep(5_000);
            }

            if (bp != null && lm != null) {
                final Optional<CalibrationsInfo>[] holder = new Optional[]{Optional.empty()};
                try {
                    lm.submit(() -> holder[0] = readAndBackupCurrentCalibrations(bp, callbacks, backupName)).get();
                } catch (ExecutionException | InterruptedException e) {
                    if (e instanceof InterruptedException) Thread.currentThread().interrupt();
                    callbacks.logLine("Failed to read current calibrations: " + e.getMessage());
                    holder[0] = Optional.empty();
                }
                calibrations = holder[0];
            } else {
                calibrations = readAndBackupCurrentCalibrationsWithSuspendedPortScanner(
                    originalEcuPort.port, callbacks, backupName, connectivityContext);
            }

            if (calibrations.isPresent()) {
                break;
            }

            retryCount++;
            if (retryCount > MAX_CALIBRATION_RETRIES) {
                if (isUiContext(callbacks)) {
                    CalibrationBackupFailureAction action = showCalibrationFailureDialog(
                        parent,
                        "Failed to back up current tune from ECU after " + MAX_CALIBRATION_RETRIES + " attempts.",
                        backupFailedFields,
                        false
                    );
                    if (action == CalibrationBackupFailureAction.RETRY) {
                        retryCount = 1;
                        continue;
                    } else if (action == CalibrationBackupFailureAction.CANCEL) {
                        callbacks.logLine("Firmware update cancelled by user.");
                        return false;
                    } else if (action == CalibrationBackupFailureAction.RESET_ALL) {
                        skipCalibrationRestore = true;
                    } else {
                        // SKIP_FAILED_FIELDS but no tune could be read at all -> nothing to merge, use defaults
                        skipCalibrationRestore = true;
                    }
                } else {
                    callbacks.logLine("Failed to back up current tune from ECU...");
                    return false;
                }
            }
        }

        prevCalibrations = calibrations;

        // Always disconnect before flashing - the port must be free for ECU reboot to OpenBLT
        if (bp != null && lm != null) {
            lm.disconnect();
        }

        if (!skipCalibrationRestore && !prevCalibrations.isPresent()) {
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

                if (skipCalibrationRestore) {
                    if (policy == FirmwareUpdatePolicy.ROLLBACK_RESTORE_OR_RESET) {
                        return resetRollbackDefaults(
                            newEcuPort,
                            timestampFileNameComponent,
                            callbacks,
                            connectivityContext);
                    }
                    callbacks.logLine("Skipping calibration restore - ECU will use default configuration.");
                    final Optional<CalibrationsInfo> freshCalibrations = readAndBackupCurrentCalibrationsWithSuspendedPortScanner(
                        newEcuPort.port,
                        callbacks,
                        getFileNameWithoutExtension(timestampFileNameComponent, "fresh_from_ecu"),
                        connectivityContext
                    );
                    if (!freshCalibrations.isPresent()) {
                        callbacks.logLine("WARNING: Failed to read fresh configuration from ECU after flash.");
                    } else {
                        callbacks.logLine("Fresh configuration read from ECU for console refresh.");
                    }
                    return true;
                }
                // The ECU's USB can briefly drop and re-enumerate right after the
                // scanner's probe. Retry a few times to ride out that transient window.
                Optional<CalibrationsInfo> updatedCalibrations = Optional.empty();
                for (int attempt = 1; attempt <= 3 && !updatedCalibrations.isPresent(); attempt++) {
                    if (attempt > 1) {
                        callbacks.logLine("Retrying calibration read after firmware update (attempt " + attempt + ")...");
                        BinaryProtocol.sleep(5_000);
                    }
                    updatedCalibrations = readAndBackupCurrentCalibrationsWithSuspendedPortScanner(
                        newEcuPort.port,
                        callbacks,
                        getFileNameWithoutExtension(timestampFileNameComponent, "after_firmware_update"), connectivityContext
                    );
                }
                if (!updatedCalibrations.isPresent()) {
                    callbacks.logLine("Failed to back up tune from ECU after firmware update...");

                    if (policy == FirmwareUpdatePolicy.ROLLBACK_RESTORE_OR_RESET) {
                        callbacks.logLine("Unable to verify the rolled-back calibration; resetting to firmware defaults.");
                        return resetRollbackDefaults(
                            newEcuPort,
                            timestampFileNameComponent,
                            callbacks,
                            connectivityContext);
                    }

                    if (isUiContext(callbacks)) {
                        CalibrationBackupFailureAction action = showCalibrationFailureDialog(
                            parent,
                            "Failed to read calibration from ECU after firmware update.",
                            new ArrayList<>(),
                            true
                        );
                        if (action == CalibrationBackupFailureAction.RESET_ALL) {
                            callbacks.logLine("Skipping calibration restore - ECU will use default configuration.");
                            return true;
                        } else if (action == CalibrationBackupFailureAction.CANCEL) {
                            callbacks.logLine("Firmware update cancelled by user.");
                            return false;
                        } else if (action == CalibrationBackupFailureAction.RETRY) {
                            updatedCalibrations = readAndBackupCurrentCalibrationsWithSuspendedPortScanner(
                                newEcuPort.port,
                                callbacks,
                                getFileNameWithoutExtension(timestampFileNameComponent, "after_firmware_update_retry"),
                                connectivityContext
                            );
                            if (!updatedCalibrations.isPresent()) {
                                return false;
                            }
                        } else {
                            // SKIP_FAILED_FIELDS: a partial merge needs the calibration read back from
                            // the ECU, but here the whole read failed - there is nothing to merge. Leave
                            // the freshly flashed ECU on its default configuration instead of crashing on
                            // an empty Optional.
                            callbacks.logLine("No calibration could be read from ECU - it will use default configuration.");
                            return true;
                        }
                    } else {
                        return false;
                    }
                }

                if (policy == FirmwareUpdatePolicy.ROLLBACK_RESTORE_OR_RESET) {
                    return restoreOrResetAfterRollback(
                        newEcuPort,
                        prevCalibrations.get(),
                        updatedCalibrations.get(),
                        timestampFileNameComponent,
                        callbacks,
                        connectivityContext);
                }

                // Try the strict-equivalent merge first: migrate every field, collecting any that fail.
                // mergeCalibrationsWithPartialFailure reports failures instead of throwing like the strict
                // mergeCalibrations, so an empty failedFields means a clean (strict) success - no prompt.
                MergeResult mergeResult = mergeCalibrationsWithPartialFailure(
                    prevCalibrations.get().getIniFile(),
                    prevCalibrations.get().generateMsq(),
                    updatedCalibrations.get(),
                    callbacks,
                    emptySet()
                );
                while (!mergeResult.failedFields.isEmpty()) {
                    callbacks.logLine("WARNING: " + mergeResult.failedFields.size()
                        + " field(s) could not be migrated to the new firmware.");
                    // Strict restore would fail on these fields - let the user pick (full reset or partial
                    // merge). Headless has no dialog, so default to accepting the partial merge.
                    final CalibrationBackupFailureAction action = isUiContext(callbacks)
                        ? showCalibrationFailureDialog(
                            parent,
                            mergeResult.failedFields.size() + " field(s) could not be migrated to the new firmware.",
                            mergeResult.failedFields,
                            true)
                        : CalibrationBackupFailureAction.SKIP_FAILED_FIELDS;
                    final PostMergeDecision decision = decidePostMerge(mergeResult, action);
                    if (decision == PostMergeDecision.CANCEL) {
                        callbacks.logLine("Firmware update cancelled by user.");
                        return false;
                    } else if (decision == PostMergeDecision.RESET_TO_DEFAULTS) {
                        callbacks.logLine("Calibration not restored - ECU will use default configuration.");
                        return true;
                    } else if (decision == PostMergeDecision.RETRY) {
                        final Optional<CalibrationsInfo> reread = readAndBackupCurrentCalibrationsWithSuspendedPortScanner(
                            newEcuPort.port,
                            callbacks,
                            getFileNameWithoutExtension(timestampFileNameComponent, "after_firmware_update_retry"),
                            connectivityContext
                        );
                        if (!reread.isPresent()) {
                            // can't read the config image anymore -> reset to defaults
                            callbacks.logLine("Could not re-read calibration from ECU - it will use default configuration.");
                            return true;
                        }
                        updatedCalibrations = reread;
                        mergeResult = mergeCalibrationsWithPartialFailure(
                            prevCalibrations.get().getIniFile(),
                            prevCalibrations.get().generateMsq(),
                            updatedCalibrations.get(),
                            callbacks,
                            emptySet()
                        );
                        continue;
                    }
                    // WRITE_MERGED: accept the partial merge (good fields applied, failed ones at defaults)
                    callbacks.logLine("Applying partial merge; " + mergeResult.failedFields.size()
                        + " field(s) left at firmware defaults.");
                    break;
                }

                if (mergeResult.mergedCalibrations.isPresent() && MigrateSettingsCheckboxState.isMigrationNeeded) {
                    if (!backUpCalibrationsInfo(
                        mergeResult.mergedCalibrations.get(),
                        getFileNameWithoutExtension(timestampFileNameComponent, "merged_to_write"),
                        callbacks
                    )) {
                        callbacks.logLine("Failed to back up merged tune before writing to ECU...");
                        return false;
                    }
                    if (!CalibrationsUpdater.INSTANCE.updateCalibrations(
                        newEcuPort.port,
                        mergeResult.mergedCalibrations.get().getImage().getConfigurationImage(),
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

    static boolean rollbackLayoutsMatch(final CalibrationsInfo before, final CalibrationsInfo after) {
        final String beforeSignature = before.getIniFile().getSignature();
        final String afterSignature = after.getIniFile().getSignature();
        final RusEfiSignature beforeParsed = SignatureHelper.parse(beforeSignature);
        final RusEfiSignature afterParsed = SignatureHelper.parse(afterSignature);
        if (beforeParsed == null || afterParsed == null
            || beforeParsed.getHash() == null || afterParsed.getHash() == null) {
            return beforeSignature.equals(afterSignature);
        }
        return beforeParsed.getHash().equals(afterParsed.getHash());
    }

    private static boolean restoreOrResetAfterRollback(
        final PortResult ecuPort,
        final CalibrationsInfo before,
        final CalibrationsInfo after,
        final String timestampFileNameComponent,
        final UpdateOperationCallbacks callbacks,
        final ConnectivityContext connectivityContext
    ) {
        if (rollbackLayoutsMatch(before, after)) {
            callbacks.logLine("Calibration layout is unchanged; restoring the pre-rollback tune.");
            if (!CalibrationsUpdater.INSTANCE.updateCalibrations(
                ecuPort.port,
                before.getImage().getConfigurationImage(),
                callbacks,
                connectivityContext)) {
                return false;
            }
            return readAndBackupCurrentCalibrationsWithSuspendedPortScanner(
                ecuPort.port,
                callbacks,
                getFileNameWithoutExtension(timestampFileNameComponent, "rollback_restored_from_ecu"),
                connectivityContext).isPresent();
        }

        callbacks.logLine("Calibration layout changed; reverse migration is unavailable. Resetting to firmware defaults.");
        return resetRollbackDefaults(ecuPort, timestampFileNameComponent, callbacks, connectivityContext);
    }

    private static boolean resetRollbackDefaults(
        final PortResult ecuPort,
        final String timestampFileNameComponent,
        final UpdateOperationCallbacks callbacks,
        final ConnectivityContext connectivityContext
    ) {
        final boolean reset = BinaryProtocolExecutor.executeWithSuspendedPortScanner(
            ecuPort.port,
            callbacks,
            binaryProtocol -> {
                final byte[] response = binaryProtocol.executeCommand(
                    Integration.TS_EXECUTE,
                    BinaryProtocol.getTextCommandBytesOnlyText("rewriteconfig"),
                    "rewriteconfig");
                return response != null
                    && response.length == 1
                    && response[0] == (byte) Integration.TS_RESPONSE_OK;
            },
            false,
            connectivityContext,
            "rewriteconfig");
        if (!reset) {
            callbacks.logLine("Failed to reset calibration after firmware rollback.");
            return false;
        }

        return readAndBackupCurrentCalibrationsWithSuspendedPortScanner(
            ecuPort.port,
            callbacks,
            getFileNameWithoutExtension(timestampFileNameComponent, "rollback_defaults_from_ecu"),
            connectivityContext).isPresent();
    }

    /**
     * Restore the tune after a <b>manual</b> OpenBLT flash.
     * <p>
     * The auto path reads the live ECU and restores it post-flash. The manual path starts with the
     * board already in the bootloader (no live ECU to read), so it restores {@link #lastEcuCalibrations}
     * - the last tune backed up off an ECU this session - onto the freshly flashed firmware.
     * <p>
     * Must NOT be called on the Swing EDT. [tag:better_ux_for_flashing]
     */
    public static void restorePreviousCalibrationsAfterManualFlash(
        final UpdateOperationCallbacks callbacks,
        final ConnectivityContext connectivityContext
    ) {
        AutoupdateUtil.assertNotAwtThread();

        final Optional<CalibrationsInfo> prev = lastEcuCalibrations;
        if (!prev.isPresent()) {
            callbacks.logLine("No previously backed-up tune available - ECU will keep its freshly flashed configuration.");
            return;
        }

        // A freshly flashed board comes up as either Ecu or EcuWithOpenblt depending on the port
        // probe timing; accept both.
        final List<PortResult> ecuPorts = waitForPortAppeared(
            t -> t == SerialPortType.Ecu || t == SerialPortType.EcuWithOpenblt,
            "Waiting for ECU to reappear after flash to restore tune...",
            callbacks,
            connectivityContext
        );
        if (ecuPorts.size() != 1) {
            callbacks.logLine(String.format(
                "Cannot restore tune: expected exactly one ECU after flash but found %d.", ecuPorts.size()));
            return;
        }
        final String newEcuPort = ecuPorts.get(0).port;

        final String timestamp = DATE_FORMAT.format(new Date());
        Optional<CalibrationsInfo> freshCalibrations = Optional.empty();
        for (int attempt = 1; attempt <= 3 && !freshCalibrations.isPresent(); attempt++) {
            if (attempt > 1) {
                callbacks.logLine("Retrying calibration read after flash (attempt " + attempt + ")...");
                BinaryProtocol.sleep(5_000);
            }
            freshCalibrations = readAndBackupCurrentCalibrationsWithSuspendedPortScanner(
                newEcuPort, callbacks,
                getFileNameWithoutExtension(timestamp, "after_manual_flash"), connectivityContext);
        }
        if (!freshCalibrations.isPresent()) {
            callbacks.logLine("Failed to read calibration from ECU after flash - tune not restored, ECU keeps defaults.");
            return;
        }

        final MergeResult mergeResult = mergeCalibrationsWithPartialFailure(
            prev.get().getIniFile(),
            prev.get().generateMsq(),
            freshCalibrations.get(),
            callbacks,
            emptySet()
        );
        if (!mergeResult.failedFields.isEmpty()) {
            callbacks.logLine("WARNING: " + mergeResult.failedFields.size()
                + " field(s) could not be migrated; leaving them at firmware defaults.");
        }
        if (!mergeResult.mergedCalibrations.isPresent()) {
            callbacks.logLine("Restored tune matches firmware defaults - nothing to write.");
            return;
        }
        if (!CalibrationsUpdater.INSTANCE.updateCalibrations(
            newEcuPort,
            mergeResult.mergedCalibrations.get().getImage().getConfigurationImage(),
            callbacks,
            connectivityContext
        )) {
            callbacks.logLine("Failed to write restored tune to ECU...");
            return;
        }
        callbacks.logLine("Previous tune restored after manual flash.");
    }

    /**
     * Imports a tune into the ECU using a live {@link BinaryProtocol} connection.
     * Reads, merges, and writes calibrations through the {@link LinkManager}'s executor queue so
     * the operation is safely interleaved with the pull thread without closing the connection.
     * Must NOT be called on the Swing EDT.
     */
    public static boolean importTune(
        final BinaryProtocol bp,
        final LinkManager lm,
        final Msq msqToImport,
        final UpdateOperationCallbacks callbacks,
        final ConnectivityContext connectivityContext
    ) {
        AutoupdateUtil.assertNotAwtThread();
        log.info("importTune via live BinaryProtocol connection");

        final String signature = msqToImport.versionInfo.getSignature();
        final IniFileModel iniFileToImport;
        try {
            iniFileToImport = BinaryProtocol.iniFileProvider.provide(signature);
        } catch (IniNotFoundException e) {
            callbacks.logLine(String.format("We failed to get .ini file for signature `%s`", signature));
            return false;
        }

        final String timestampFileNameComponent = DATE_FORMAT.format(new Date());

        if (!backupTune(iniFileToImport, msqToImport,
                getFileNameWithoutExtension(timestampFileNameComponent, "tune_to_import"), callbacks)) {
            callbacks.logLine("Failed to back up tune to import...");
            return false;
        }

        final String backupName = getFileNameWithoutExtension(timestampFileNameComponent, "backup_from_ecu");
        final Optional<CalibrationsInfo>[] prevTuneHolder = new Optional[]{Optional.empty()};
        try {
            lm.submit(() -> prevTuneHolder[0] = readAndBackupCurrentCalibrations(bp, callbacks, backupName)).get();
        } catch (ExecutionException | InterruptedException e) {
            if (e instanceof InterruptedException) Thread.currentThread().interrupt();
            callbacks.logLine("Failed to read current calibrations: " + e.getMessage());
            return false;
        }
        if (!prevTuneHolder[0].isPresent()) {
            callbacks.logLine("Failed to back up current tune from ECU...");
            return false;
        }

        final MergeResult mergeResult = mergeCalibrationsWithPartialFailure(
            iniFileToImport, msqToImport, prevTuneHolder[0].get(), callbacks,
            new HashSet<>(Collections.singletonList("vinNumber"))
        );
        if (!mergeResult.failedFields.isEmpty()) {
            callbacks.logLine("WARNING: Tune loaded without incompatible fields: "
                + String.join(", ", mergeResult.failedFields));
        }
        final Optional<CalibrationsInfo> mergedTune = mergeResult.mergedCalibrations;

        if (mergedTune.isPresent()) {
            if (!backUpCalibrationsInfo(mergedTune.get(),
                    getFileNameWithoutExtension(timestampFileNameComponent, "merged_to_write"), callbacks)) {
                callbacks.logLine("Failed to back up merged tune...");
                return false;
            }
            if (!CalibrationsUpdater.INSTANCE.updateCalibrations(
                    bp, lm, mergedTune.get().getImage().getConfigurationImage(), callbacks)) {
                callbacks.logLine("Failed to write merged tune to ECU...");
                return false;
            }
            final String mergedBackupName = getFileNameWithoutExtension(timestampFileNameComponent, "merged_from_ecu");
            final Optional<CalibrationsInfo>[] mergedFromEcuHolder = new Optional[]{Optional.empty()};
            try {
                lm.submit(() -> mergedFromEcuHolder[0] =
                    readAndBackupCurrentCalibrations(bp, callbacks, mergedBackupName)).get();
            } catch (ExecutionException | InterruptedException e) {
                if (e instanceof InterruptedException) Thread.currentThread().interrupt();
                callbacks.logLine("Failed to read merged calibrations back: " + e.getMessage());
                return false;
            }
            if (!mergedFromEcuHolder[0].isPresent()) {
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

    public static Optional<CalibrationsInfo> readCurrentCalibrations(
        final BinaryProtocol binaryProtocol,
        final UpdateOperationCallbacks callbacks
    ) {
        return readCalibrationsInfo(binaryProtocol, callbacks);
    }

    // right now we only load first page, one day soon LTFT would ask for other pages!
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
                    // Remember the last good tune read off a live ECU so a later manual OpenBLT flash
                    // (board already in the bootloader) can still restore it. [tag:better_ux_for_flashing]
                    lastEcuCalibrations = calibrationsInfo;
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
                        ConfigurationImageGetterSetter2.setValue(fieldToUpdate.get(), mergedImage, migratedValue);
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

    public static MergeResult mergeCalibrationsWithPartialFailure(
        final IniFileModel prevIniFile,
        final Msq prevMsq,
        final CalibrationsInfo newCalibrations,
        final UpdateOperationCallbacks callbacks,
        final Set<String> additionalIniFieldsToIgnore
    ) {
        final List<String> failedFields = new ArrayList<>();
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

        if (valuesToUpdate.isEmpty() && !"true".equalsIgnoreCase(RUSEFI_FORCE_CALIBRATIONS_RESTORE)) {
            callbacks.logLine("It looks like we do not need to update any fields to restore previous calibrations.");
            return new MergeResult(Optional.empty(), failedFields);
        }

        final ConfigurationImage mergedImage = newCalibrations.getImage().getConfigurationImage().clone();
        for (final Map.Entry<String, Constant> valueToUpdate : valuesToUpdate) {
            final String migratedFieldName = valueToUpdate.getKey();
            final Constant migratedValue = valueToUpdate.getValue();
            final Optional<IniField> fieldToUpdate = newIniFile.findIniField(migratedFieldName);
            if (fieldToUpdate.isPresent()) {
                try {
                    ConfigurationImageGetterSetter2.setValue(fieldToUpdate.get(), mergedImage, migratedValue);
                    callbacks.logLine(String.format(
                        "To restore previous calibrations we are going to update the field `%s` with a value `%s`",
                        migratedFieldName,
                        migratedValue.getValue()
                    ));
                } catch (Throwable e) {
                    log.error(
                        String.format("Failed to set value %s for ini-field %s", migratedValue, fieldToUpdate),
                        e
                    );
                    callbacks.logLine(String.format(
                        "WARNING: Failed to migrate field `%s`, skipping: %s",
                        migratedFieldName,
                        e.getMessage()
                    ));
                    failedFields.add(migratedFieldName);
                }
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
                failedFields.add(migratedFieldName);
            }
        }

        if (failedFields.isEmpty() && valuesToUpdate.isEmpty() && "true".equalsIgnoreCase(RUSEFI_FORCE_CALIBRATIONS_RESTORE)) {
            callbacks.logLine("It looks like we do not need to update previous calibrations, but for debugging we are going to rewrite to ECU new calibrations again.");
            return new MergeResult(Optional.of(newCalibrations), failedFields);
        }

        if (failedFields.isEmpty() && valuesToUpdate.isEmpty()) {
            return new MergeResult(Optional.empty(), failedFields);
        }

        return new MergeResult(Optional.of(new CalibrationsInfo(
            newIniFile,
            new ConfigurationImageWithMeta(newCalibrations.getImage().getMeta(), mergedImage.getContent())
        )), failedFields);
    }

    // package-private for CalibrationsHelperContextTest — reflection in unit tests is prohibited
    static boolean isUiContext(final UpdateOperationCallbacks callbacks) {
        return callbacks != UpdateOperationCallbacks.DUMMY &&
               callbacks != UpdateOperationCallbacks.LOGGER;
    }

    private static CalibrationBackupFailureAction showCalibrationFailureDialog(
        final JComponent parent,
        final String failureMessage,
        final List<String> failedFields,
        final boolean hasBackupData
    ) {
        final CalibrationBackupFailureAction[] result = new CalibrationBackupFailureAction[1];
        try {
            SwingUtilities.invokeAndWait(() -> {
                result[0] = CalibrationBackupFailureDialog.showDialog(
                    parent,
                    failureMessage,
                    failedFields,
                    hasBackupData
                );
            });
        } catch (Exception e) {
            log.error("Failed to show calibration failure dialog:", e);
            return CalibrationBackupFailureAction.RESET_ALL;
        }
        return result[0];
    }
}
