package com.rusefi.ui.basic;

import com.devexperts.logging.Logging;
import com.rusefi.*;
import com.rusefi.autoupdate.Autoupdate;
import com.rusefi.core.FindFileHelper;
import com.rusefi.io.LinkManager;
import com.rusefi.io.UpdateOperationCallbacks;
import com.rusefi.maintenance.CalibrationsInfo;
import com.rusefi.maintenance.ProgramSelector;
import com.rusefi.maintenance.jobs.*;
import org.jetbrains.annotations.Nullable;
import com.rusefi.util.CompatibilityOptional;
import org.jetbrains.annotations.NotNull;

import javax.swing.*;
import java.awt.*;
import java.awt.event.ActionEvent;
import java.util.List;
import java.util.Optional;
import java.util.Set;
import java.util.concurrent.CompletableFuture;
import java.util.concurrent.atomic.AtomicReference;
import java.util.function.Consumer;
import java.util.stream.Collectors;

import static com.devexperts.logging.Logging.getLogging;

public class StartupUpdateActions implements BasicButtonCoordinator {
    private static final Logging log = getLogging(StartupUpdateActions.class);

    private final boolean isObfuscated = FindFileHelper.isObfuscated();

    private final JLabel updateSoftwareStatus = new JLabel();
    private final JCheckBox migrateSettings = new JCheckBox("Migrate Settings");
    private final JCheckBox verboseMessages = new JCheckBox("Verbose Status");

    private final JButton updateFirmwareButton = ProgramSelector.createUpdateFirmwareButton();
    private final JButton updateSoftwareButton = new JButton("Update Software");
    // todo: this control lives on a different parent TODO fix this mess!
    private final ImportTuneControl importTuneButton;

    private final ConnectivityContext connectivityContext;
    private final SingleAsyncJobExecutor singleAsyncJobExecutor;
    private final UpdateOperationCallbacks updateOperationCallbacks;
    private @Nullable LinkManager splashLinkManager;
//    private final UpdateCalibrations updateCalibrations;
    private volatile Optional<AsyncJob> updateFirmwareJob = Optional.empty();
    private boolean softwareUpdateAvailable;
    private boolean softwareUpdateInProgress;
    private final AtomicReference<Optional<PortResult>> ecuPortToUse;
    private final FirmwareRollbackController rollbackController;

    private String latestReportedHash;

    public StartupUpdateActions(
        ConnectivityContext connectivityContext,
        final UpdateOperationCallbacks updateOperationCallbacks, SingleAsyncJobExecutor singleAsyncJobExecutor,
        AtomicReference<Optional<PortResult>> ecuPortToUse,
        CompletableFuture<Autoupdate.UpdateOutcome> softwareUpdateOutcome,
        Consumer<JComponent> showRollbackPicker,
        Runnable closeRollbackPicker
    ) {
        this.connectivityContext = connectivityContext;
        this.ecuPortToUse = ecuPortToUse;
        this.singleAsyncJobExecutor = singleAsyncJobExecutor;
        this.updateOperationCallbacks = updateOperationCallbacks;
        rollbackController = new FirmwareRollbackController(
            connectivityContext, updateOperationCallbacks, singleAsyncJobExecutor, ecuPortToUse::get,
            () -> !softwareUpdateInProgress, this::refreshButtons,
            showRollbackPicker, closeRollbackPicker);
        singleAsyncJobExecutor.addOnJobAboutToStartListener(() -> SwingUtilities.invokeLater(this::refreshButtons));
        singleAsyncJobExecutor.addOnJobInProgressFinishedListener(() -> SwingUtilities.invokeLater(this::refreshButtons));
        importTuneButton = new ImportTuneControl(singleAsyncJobExecutor, this, connectivityContext);
//        updateCalibrations = new UpdateCalibrations(singleAsyncJobExecutor);

        updateFirmwareButton.addActionListener(this::onUpdateFirmwareButtonClicked);
        updateFirmwareButton.setEnabled(false);

        updateSoftwareButton.setPreferredSize(updateFirmwareButton.getPreferredSize());
        softwareUpdateAvailable = !Autoupdate.isAutoUpdateEnabled();
        updateSoftwareButton.setVisible(softwareUpdateAvailable);
        updateSoftwareButton.setEnabled(softwareUpdateAvailable);
        updateSoftwareButton.addActionListener(e -> onUpdateSoftwareClicked());
        updateSoftwareStatus.setForeground(Color.red);
        updateSoftwareStatus.setVisible(false);

        bindSoftwareUpdateOutcome(softwareUpdateOutcome, updateSoftwareButton, updateSoftwareStatus, () -> {
            softwareUpdateAvailable = true;
            refreshButtons();
        });

        importTuneButton.setEnabled(false);

        migrateSettings.setSelected(true);
        migrateSettings.setBorder(BorderFactory.createEmptyBorder(8, 8, 8, 8));
        migrateSettings.addActionListener(e -> updateMigrateSettingState());

        verboseMessages.setSelected(false);
        verboseMessages.addActionListener(e -> updateMigrateSettingState());

        updateMigrateSettingState();
    }

    public ImportTuneControl getImportTuneButton() {
        return importTuneButton;
    }

    /**
     * Registers the live splash auto-connect {@link LinkManager} so firmware-update jobs can
     * call {@link LinkManager#disconnect()} before flashing and {@link LinkManager#reconnect()}
     * after. Pass {@code null} on disconnect to clear the handoff.
     */
    public void setSplashLinkManager(@Nullable LinkManager lm) {
        this.splashLinkManager = lm;
        rollbackController.setLinkManager(lm);
        importTuneButton.setLinkManager(lm);
        updateUpdateFirmwareJob();
    }

    public JCheckBox getMigrateSettings() {
        return migrateSettings;
    }

    public JButton getUpdateFirmwareButton() {
        return updateFirmwareButton;
    }

    public JButton getRollbackFirmwareButton() {
        return rollbackController.getRollbackButton();
    }

    public void configureFirmwareSelector(ProgramSelector selector) {
        selector.setFirmwareUpdateInterceptor(rollbackController::startLatestUpdate);
        selector.setExternalBusySupplier(() -> rollbackController.isBusy() || softwareUpdateInProgress);
        rollbackController.addStateChangedListener(() -> selector.apply(connectivityContext.getCurrentHardware()));
    }

    public JButton getUpdateSoftwareButton() {
        return updateSoftwareButton;
    }

    public JLabel getUpdateSoftwareStatus() {
        return updateSoftwareStatus;
    }

    private void updateMigrateSettingState() {
        MigrateSettingsCheckboxState.isMigrationNeeded = migrateSettings.isSelected();
        MigrateSettingsCheckboxState.isVerboseMessages = verboseMessages.isSelected();
    }

    private void onUpdateSoftwareClicked() {
        softwareUpdateInProgress = true;
        refreshButtons();
        Thread updateThread = new Thread(() -> Autoupdate.runManualUpdate(message -> {
            if (message != null) {
                Autoupdate.relaunchConsole();
                return;
            }
            SwingUtilities.invokeLater(() -> {
                softwareUpdateInProgress = false;
                softwareUpdateAvailable = true;
                updateSoftwareStatus.setText("No software update was installed. You can retry.");
                updateSoftwareStatus.setVisible(true);
                refreshButtons();
            });
        }), "manual-update");
        updateThread.setDaemon(true);
        updateThread.start();
    }

    static void bindSoftwareUpdateOutcome(CompletableFuture<Autoupdate.UpdateOutcome> outcome,
                                          JButton button, JLabel status, Runnable onFailure) {
        outcome.thenAccept(value -> SwingUtilities.invokeLater(() -> {
            applySoftwareUpdateOutcome(value, button, status);
            if (value == Autoupdate.UpdateOutcome.FAILED) {
                onFailure.run();
            }
        }));
    }

    static void applySoftwareUpdateOutcome(Autoupdate.UpdateOutcome outcome, JButton button, JLabel status) {
        if (outcome == Autoupdate.UpdateOutcome.FAILED) {
            status.setText("Automatic software update failed. You can retry.");
            status.setVisible(true);
            button.setVisible(true);
            button.setEnabled(true);
        }
    }

    public void onHardwareUpdated() {
        updateUpdateFirmwareJob();
        updateEcuPortToUse();
    }

    private void updateUpdateFirmwareJob() {
        final AvailableHardware currentHardware = connectivityContext.getCurrentHardware();
        log.info("updateUpdateFirmwareJob " + currentHardware);
        final List<PortResult> portsToUpdateFirmware = getPortResults(currentHardware);
        int count = portsToUpdateFirmware.size();
        if (count == 1) {
            if (splashLinkManager == null) {
                log.info("updateUpdateFirmwareJob: skipping — splashLinkManager not set yet (auto-connect in progress)");
                resetUpdateFirmwareJob();
                return;
            }
            setUpdateFirmwareJob(getNonDfuUpdateFirmwareJobForPort(portsToUpdateFirmware.get(0)));
            return;
        }
        if (currentHardware.isDfuFound()) {
            setUpdateFirmwareJob(new DfuManualJob(connectivityContext.getConnectedEcuTarget()));
            return;
        }
        resetUpdateFirmwareJob();
    }

    private AsyncJob getNonDfuUpdateFirmwareJobForPort(final PortResult portToUpdateFirmware) {
        AsyncJob job = null;
        final SerialPortType portType = portToUpdateFirmware.type;
        switch (portType) {
            case Ecu: {
                job = new DfuAutoJob(portToUpdateFirmware, updateFirmwareButton, connectivityContext, splashLinkManager);
                break;
            }
            case EcuWithOpenblt: {
                job = new OpenBltAutoJob(portToUpdateFirmware, updateFirmwareButton, connectivityContext, splashLinkManager);
                break;
            }
            case OpenBlt: {
                job = OpenBltManualJobFactory.createProduction(portToUpdateFirmware, updateFirmwareButton, connectivityContext);
                break;
            }
            default: {
                log.error(String.format("Unexpected port type: %s", portType));
                break;
            }
        }
        return job;
    }

    private @NotNull List<PortResult> getPortResults(AvailableHardware currentHardware) {
        final Set<SerialPortType> portTypesToUpdateFirmware = (isObfuscated ?
            CompatibilitySet.of(
                SerialPortType.EcuWithOpenblt,
                SerialPortType.OpenBlt
            ) :
            CompatibilitySet.of(
                SerialPortType.Ecu,
                SerialPortType.EcuWithOpenblt
            )
        );
        return currentHardware.getKnownPorts(portTypesToUpdateFirmware);
    }

    private void setUpdateFirmwareJob(final AsyncJob updateFirmwareJob) {
        this.updateFirmwareJob = Optional.of(updateFirmwareJob);
        refreshButtons();
    }

    private void resetUpdateFirmwareJob() {
        updateFirmwareJob = Optional.empty();
        updateFirmwareButton.setEnabled(false);
    }

    private void updateEcuPortToUse() {
        final AvailableHardware currentHardware = connectivityContext.getCurrentHardware();
        final List<PortResult> ecuPortsToUse = currentHardware.getKnownPorts(CompatibilitySet.of(
            SerialPortType.Ecu,
            SerialPortType.EcuWithOpenblt
        ));

        switch (ecuPortsToUse.size()) {
            case 0: {
                resetEcuPortToUse();
                log.info("No ECU ports to use found");
                break;
            }
            case 1: {
                setEcuPortToUse(ecuPortsToUse.get(0));
                break;
            }
            default: {
                resetEcuPortToUse();
                log.info(String.format(
                    "Multiple ECU ports to use found on: %s",
                    ecuPortsToUse.stream()
                        .map(portResult -> portResult.port)
                        .collect(Collectors.joining(", "))
                ));
                break;
            }
        }
    }

    private void setEcuPortToUse(final PortResult port) {
        ecuPortToUse.set(Optional.of(port));
        rollbackController.refresh(port);

        SwingUtilities.invokeLater(() -> {
            refreshButtons();
            if (port.getFirmwareHash().isPresent()) {
                String hash = port.getFirmwareHash().get();
                if (hash.equals(latestReportedHash)) {
                    // we do not want to print same every second
                    return;
                }
                latestReportedHash = hash;
                updateOperationCallbacks.logLine("Detected " + hash);
                CalibrationsInfo calibrations = port.getCalibrations();
                if (calibrations != null) {
                    updateOperationCallbacks.logLine(calibrations.getIniFile().getSignature());
                    Usability.INSTANCE.onCalibrations(updateOperationCallbacks, calibrations);
                }
            }
        });
    }

    private void resetEcuPortToUse() {
        ecuPortToUse.set(Optional.empty());
        rollbackController.reset();
        SwingUtilities.invokeLater(() -> {
            importTuneButton.setEnabled(false);
        });
    }

    /** Outcome of the most recent firmware-update job, for the persistent post-reconnect banner (#9832). */
    public UpdateFirmwareResult getLastUpdateResult() {
        return singleAsyncJobExecutor.getLastResult();
    }

    private void onUpdateFirmwareButtonClicked(final ActionEvent actionEvent) {
        if (rollbackController.startLatestUpdate(updateFirmwareButton)) {
            return;
        }
        disableButtons();
        CompatibilityOptional.ifPresentOrElse(updateFirmwareJob,
            value -> {
                singleAsyncJobExecutor.startJob(value, updateFirmwareButton);
            },
            () -> log.error("Update firmware job is is not defined.")
        );
        refreshButtons();
    }

    public void refreshButtons() {
        refreshUpdateFirmwareButton();
        final Optional<PortResult> ecuPort = ecuPortToUse.get();
        final boolean noUpdateInProgress = singleAsyncJobExecutor.isNotInProgress()
            && !softwareUpdateInProgress
            && !rollbackController.isBusy();
        final boolean isEcuPortJobPossible = ecuPort.isPresent() && noUpdateInProgress;
        importTuneButton.setEnabled(isEcuPortJobPossible);
        updateSoftwareButton.setEnabled(
            updateSoftwareButton.isVisible() && softwareUpdateAvailable && noUpdateInProgress);
        rollbackController.refreshButton();
    }

    private void refreshUpdateFirmwareButton() {
        final boolean isFirmwareUpdatePossible =
            updateFirmwareJob.isPresent()
                && !rollbackController.isBusy()
                && singleAsyncJobExecutor.isNotInProgress()
                && !softwareUpdateInProgress;
        if (isFirmwareUpdatePossible) {
            final AsyncJob currentUpdateFirmwareJob = updateFirmwareJob.get();
            Optional<String> updateFirmwareButtonText = Optional.empty();
            if (currentUpdateFirmwareJob instanceof OpenBltAutoJob) {
                updateFirmwareButtonText = Optional.of("Auto Update Firmware");
            } else if (currentUpdateFirmwareJob instanceof OpenBltManualJob) {
                updateFirmwareButtonText = Optional.of("Blt Update Firmware");
            } else if (currentUpdateFirmwareJob instanceof DfuAutoJob) {
                updateFirmwareButtonText = Optional.of("Update Firmware");
            } else if (currentUpdateFirmwareJob instanceof DfuManualJob) {
                updateFirmwareButtonText = Optional.of("Update Firmware via DFU");
            } else {
                log.error(String.format(
                    "Unexpected job type: %s",
                    currentUpdateFirmwareJob.getClass().getSimpleName()
                ));
            }
            updateFirmwareButtonText.ifPresent(updateFirmwareButton::setText);
        }
        updateFirmwareButton.setEnabled(isFirmwareUpdatePossible);
    }

    @Override
    public void disableButtons() {
        updateFirmwareButton.setEnabled(false);
        rollbackController.getRollbackButton().setEnabled(false);
        updateSoftwareButton.setEnabled(false);
        importTuneButton.setEnabled(false);
    }
}
