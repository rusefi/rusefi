package com.rusefi.ui.basic;

import com.devexperts.logging.Logging;
import com.rusefi.*;
import com.rusefi.autoupdate.Autoupdate;
import com.rusefi.io.LinkManager;
import com.rusefi.io.UpdateOperationCallbacks;
import com.rusefi.maintenance.CalibrationsInfo;
import com.rusefi.maintenance.ProgramSelector;
import org.jetbrains.annotations.Nullable;

import javax.swing.*;
import java.awt.*;
import java.util.List;
import java.util.Optional;
import java.util.concurrent.CompletableFuture;
import java.util.concurrent.atomic.AtomicReference;
import java.util.function.Consumer;
import java.util.stream.Collectors;

import static com.devexperts.logging.Logging.getLogging;

public class StartupUpdateActions implements BasicButtonCoordinator {
    private static final Logging log = getLogging(StartupUpdateActions.class);

    private final JLabel updateSoftwareStatus = new JLabel();
    private final JCheckBox migrateSettings = new JCheckBox("Migrate Settings");
    private final JCheckBox verboseMessages = new JCheckBox("Verbose Status");

    private final JButton updateSoftwareButton = new JButton("Update Software");
    // todo: this control lives on a different parent TODO fix this mess!
    private final ImportTuneControl importTuneButton;

    private final ConnectivityContext connectivityContext;
    private final SingleAsyncJobExecutor singleAsyncJobExecutor;
    private final UpdateOperationCallbacks updateOperationCallbacks;
//    private final UpdateCalibrations updateCalibrations;
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

    /** Registers the live splash connection for tune import and rollback actions. */
    public void setSplashLinkManager(@Nullable LinkManager lm) {
        rollbackController.setLinkManager(lm);
        importTuneButton.setLinkManager(lm);
    }

    public JCheckBox getMigrateSettings() {
        return migrateSettings;
    }

    public JButton getRollbackFirmwareButton() {
        return rollbackController.getRollbackButton();
    }

    public void configureFirmwareSelector(ProgramSelector selector) {
        selector.addFirmwareControl(updateSoftwareButton);
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
        updateEcuPortToUse();
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

    public void refreshButtons() {
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

    @Override
    public void disableButtons() {
        rollbackController.getRollbackButton().setEnabled(false);
        updateSoftwareButton.setEnabled(false);
        importTuneButton.setEnabled(false);
    }
}
