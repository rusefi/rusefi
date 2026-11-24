package com.rusefi.ui.basic;

import com.devexperts.logging.Logging;
import com.rusefi.*;
import com.rusefi.autoupdate.Autoupdate;
import com.rusefi.io.LinkManager;
import com.rusefi.io.ConnectionStatusLogic;
import com.rusefi.io.ConnectionStatusValue;
import com.rusefi.io.UpdateOperationCallbacks;
import com.rusefi.maintenance.CalibrationsInfo;
import com.rusefi.maintenance.ProgramSelector;
import org.jetbrains.annotations.Nullable;

import javax.swing.*;
import java.awt.*;
import java.util.List;
import java.util.Optional;
import java.util.concurrent.atomic.AtomicReference;
import java.util.function.Consumer;
import java.util.function.BiFunction;
import java.util.function.Supplier;
import java.util.stream.Collectors;

import static com.devexperts.logging.Logging.getLogging;

public class StartupUpdateActions implements BasicButtonCoordinator {
    private static final Logging log = getLogging(StartupUpdateActions.class);

    private final JLabel updateSoftwareStatus = new JLabel();
    private final JCheckBox migrateSettings = createMigrateSettingsCheckBox();
    private final JCheckBox verboseMessages = new JCheckBox("Verbose Status");

    private final JButton updateSoftwareButton = new JButton("Update application");
    private final JButton updateLaterButton = new JButton("Later");
    private final JPanel softwareUpdateOffer = new JPanel(new BorderLayout(12, 0));
    private final SoftwareUpdateCoordinator softwareUpdateCoordinator;
    private final Supplier<Component> updateDialogParent;
    private final Runnable restartApplication;
    // todo: this control lives on a different parent TODO fix this mess!
    private final ImportTuneControl importTuneButton;

    private final ConnectivityContext connectivityContext;
    private final SingleAsyncJobExecutor singleAsyncJobExecutor;
    private final UpdateOperationCallbacks updateOperationCallbacks;
//    private final UpdateCalibrations updateCalibrations;
    private boolean softwareUpdateAvailable;
    private boolean softwareUpdateInProgress;
    private boolean softwareRestartRequired;
    private final AtomicReference<Optional<PortResult>> ecuPortToUse;
    private final FirmwareRollbackController rollbackController;
    private final FirmwareBranchSwitchController branchSwitchController;
    private FirmwareStatusSummaryPanel firmwareSummary;
    private volatile @Nullable LinkManager linkManager;

    private String latestReportedHash;

    public StartupUpdateActions(
        ConnectivityContext connectivityContext,
        final UpdateOperationCallbacks updateOperationCallbacks, SingleAsyncJobExecutor singleAsyncJobExecutor,
        AtomicReference<Optional<PortResult>> ecuPortToUse,
        SoftwareUpdateCoordinator softwareUpdateCoordinator,
        Consumer<JComponent> showRollbackPicker,
        Runnable closeRollbackPicker,
        Supplier<Component> updateDialogParent,
        Runnable restartApplication
    ) {
        this.connectivityContext = connectivityContext;
        this.ecuPortToUse = ecuPortToUse;
        this.singleAsyncJobExecutor = singleAsyncJobExecutor;
        this.updateOperationCallbacks = updateOperationCallbacks;
        this.softwareUpdateCoordinator = softwareUpdateCoordinator;
        this.updateDialogParent = updateDialogParent;
        this.restartApplication = restartApplication;
        rollbackController = new FirmwareRollbackController(
            connectivityContext, updateOperationCallbacks, singleAsyncJobExecutor, ecuPortToUse::get,
            () -> !softwareUpdateInProgress, this::refreshButtons,
            showRollbackPicker, closeRollbackPicker);
        // Release status is read-only; keep it available even when the rollback action is disabled.
        rollbackController.enableReleaseStatusDiscovery();
        branchSwitchController = new FirmwareBranchSwitchController(
            connectivityContext, updateOperationCallbacks, singleAsyncJobExecutor, ecuPortToUse::get,
            () -> !softwareUpdateInProgress && linkManager != null && linkManager.getBinaryProtocol() != null
                && ConnectionStatusLogic.INSTANCE.getValue() == ConnectionStatusValue.CONNECTED,
            showRollbackPicker, closeRollbackPicker);
        singleAsyncJobExecutor.addOnJobAboutToStartListener(() -> SwingUtilities.invokeLater(this::refreshButtons));
        singleAsyncJobExecutor.addOnJobReleasedListener(() -> SwingUtilities.invokeLater(this::refreshButtons));
        importTuneButton = new ImportTuneControl(singleAsyncJobExecutor, this, connectivityContext);
//        updateCalibrations = new UpdateCalibrations(singleAsyncJobExecutor);

        // Hidden until the launch-time check reports an update (applySoftwareUpdateState).
        softwareUpdateAvailable = false;
        updateSoftwareButton.setToolTipText("Update the application on this computer, not the ECU firmware.");
        updateSoftwareButton.setVisible(softwareUpdateAvailable);
        updateSoftwareButton.setEnabled(softwareUpdateAvailable);
        updateSoftwareButton.addActionListener(e -> {
            if (softwareRestartRequired) {
                restartApplication.run();
            } else {
                softwareUpdateCoordinator.install();
            }
        });
        updateLaterButton.setVisible(false);
        updateLaterButton.addActionListener(e -> {
            if (softwareRestartRequired) {
                showDeferredRestartReminder();
            } else {
                softwareUpdateCoordinator.later();
            }
        });
        Color error = UIManager.getColor("Console.ErrorText.foreground");
        updateSoftwareStatus.setForeground(error != null ? error : Color.red);
        updateSoftwareStatus.setVisible(false);
        softwareUpdateOffer.setOpaque(true);
        softwareUpdateOffer.setBackground(themed("Console.InstallerBanner.background", new Color(255, 243, 205)));
        softwareUpdateOffer.setBorder(BorderFactory.createCompoundBorder(
            BorderFactory.createMatteBorder(0, 0, 1, 0, themed("Console.InstallerBanner.borderColor", new Color(180, 140, 50))),
            BorderFactory.createEmptyBorder(6, 12, 6, 6)
        ));
        updateSoftwareStatus.setFont(updateSoftwareStatus.getFont().deriveFont(Font.BOLD));
        softwareUpdateOffer.add(updateSoftwareStatus, BorderLayout.CENTER);
        JPanel softwareUpdateActions = new JPanel(new FlowLayout(FlowLayout.RIGHT, 0, 0));
        softwareUpdateActions.setOpaque(false);
        softwareUpdateActions.add(updateLaterButton);
        softwareUpdateActions.add(Box.createHorizontalStrut(5));
        softwareUpdateActions.add(updateSoftwareButton);
        softwareUpdateOffer.add(softwareUpdateActions, BorderLayout.EAST);
        softwareUpdateOffer.setVisible(softwareUpdateAvailable);

        softwareUpdateCoordinator.addListener(this::applySoftwareUpdateState);

        importTuneButton.setEnabled(false);

        migrateSettings.setSelected(true);
        migrateSettings.setToolTipText("The console will try to restore your tune after the update, but this is not guaranteed. Check the settings before using the ECU.");
        migrateSettings.setBorder(BorderFactory.createEmptyBorder(8, 8, 8, 8));
        migrateSettings.addActionListener(e -> updateMigrateSettingState());

        verboseMessages.setSelected(false);
        verboseMessages.addActionListener(e -> updateMigrateSettingState());

        updateMigrateSettingState();

    }

    private static Color themed(String key, Color fallback) {
        Color color = UIManager.getColor(key);
        return color != null ? color : fallback;
    }

    public ImportTuneControl getImportTuneButton() {
        return importTuneButton;
    }

    /** Registers the live splash connection for tune import and rollback actions. */
    public void setSplashLinkManager(@Nullable LinkManager lm) {
        linkManager = lm;
        rollbackController.setLinkManager(lm);
        branchSwitchController.setLinkManager(lm);
        importTuneButton.setLinkManager(lm);
        rollbackController.refresh(ecuPortToUse.get().orElse(null));
        refreshFirmwareSummary();
    }

    public JCheckBox getMigrateSettings() {
        return migrateSettings;
    }

    public static JCheckBox createMigrateSettingsCheckBox() {
        return new JCheckBox("Keep my current tune");
    }

    public JButton getRollbackFirmwareButton() {
        return rollbackController.getRollbackButton();
    }

    public JButton getBranchSwitchButton() {
        return branchSwitchController.getButton();
    }

    public void configureFirmwareSelector(ProgramSelector selector) {
        firmwareSummary = new FirmwareStatusSummaryPanel(
            rollbackController, selector, branchSwitchController.getButton(), migrateSettings,
            connectivityContext, selector::getSelectedDetectedPort,
            () -> softwareUpdateInProgress || !singleAsyncJobExecutor.isNotInProgress()
                || rollbackController.isDownloadInProgress());
        selector.setMissingIniStateChangedListener(this::refreshFirmwareStatusAfterMissingIniChange);
        selector.setTargetedFirmwareUpdateInterceptor(rollbackController::startLatestUpdate);
        selector.setExternalBusySupplier(() -> rollbackController.isDownloadInProgress() || softwareUpdateInProgress);
        selector.setExternalPrimaryActionBusySupplier(rollbackController::isPrimaryUpdateBlockedByCatalogDiscovery);
        rollbackController.addStateChangedListener(() -> {
            selector.apply(connectivityContext.getCurrentHardware());
            refreshFirmwareSummary();
        });
        singleAsyncJobExecutor.addOnJobReleasedListener(() -> SwingUtilities.invokeLater(() -> {
            branchSwitchController.refreshButton();
            selector.apply(connectivityContext.getCurrentHardware());
        }));
    }

    public JButton getUpdateSoftwareButton() {
        return updateSoftwareButton;
    }

    public JButton getUpdateLaterButton() {
        return updateLaterButton;
    }

    public JComponent getFirmwareSummary() {
        return firmwareSummary;
    }

    /** Cross-package test seam for deterministic Dev status on the startup firmware tab. */
    public void setDevMetadataProviderForTests(BiFunction<String, String, Long> provider) {
        rollbackController.setDevMetadataProviderForTests(provider);
    }

    public JLabel getUpdateSoftwareStatus() {
        return updateSoftwareStatus;
    }

    public JPanel getSoftwareUpdateOffer() {
        return softwareUpdateOffer;
    }

    private void updateMigrateSettingState() {
        MigrateSettingsCheckboxState.isMigrationNeeded = migrateSettings.isSelected();
        MigrateSettingsCheckboxState.isVerboseMessages = verboseMessages.isSelected();
    }

    private void applySoftwareUpdateState(SoftwareUpdateCoordinator.State state) {
        switch (state) {
            case CHECKING:
                return;
            case AVAILABLE:
                softwareUpdateAvailable = true;
                softwareUpdateInProgress = false;
                softwareRestartRequired = false;
                applySoftwareUpdateOutcome(Autoupdate.UpdateOutcome.AVAILABLE, updateSoftwareButton,
                    updateSoftwareStatus, updateLaterButton);
                softwareUpdateOffer.setVisible(false);
                scheduleApplicationUpdateDialog();
                break;
            case AVAILABLE_DEFERRED:
                softwareUpdateAvailable = true;
                softwareUpdateInProgress = false;
                softwareRestartRequired = false;
                updateSoftwareStatus.setText("Application update available.");
                updateSoftwareStatus.setVisible(true);
                updateSoftwareButton.setText("Update now");
                updateSoftwareButton.setVisible(true);
                updateLaterButton.setVisible(false);
                softwareUpdateOffer.setVisible(true);
                break;
            case INSTALLING:
                softwareUpdateAvailable = false;
                softwareUpdateInProgress = true;
                softwareRestartRequired = false;
                updateSoftwareStatus.setText("Downloading application update…");
                updateSoftwareStatus.setVisible(true);
                updateSoftwareButton.setVisible(true);
                updateLaterButton.setVisible(false);
                softwareUpdateOffer.setVisible(true);
                break;
            case RESTART_REQUIRED:
                softwareUpdateAvailable = true;
                softwareUpdateInProgress = false;
                softwareRestartRequired = true;
                applySoftwareRestartRequired(updateSoftwareButton, updateSoftwareStatus,
                    updateLaterButton);
                softwareUpdateOffer.setVisible(true);
                break;
            case FAILED:
                softwareUpdateAvailable = true;
                softwareUpdateInProgress = false;
                softwareRestartRequired = false;
                applySoftwareUpdateOutcome(Autoupdate.UpdateOutcome.FAILED, updateSoftwareButton,
                    updateSoftwareStatus, updateLaterButton);
                softwareUpdateOffer.setVisible(true);
                break;
            default:
                softwareUpdateInProgress = false;
                softwareRestartRequired = false;
                // The check runs whatever the auto-update setting, so idle means nothing to offer.
                softwareUpdateAvailable = false;
                updateSoftwareButton.setText("Update application");
                updateSoftwareButton.setVisible(softwareUpdateAvailable);
                updateSoftwareStatus.setVisible(false);
                updateLaterButton.setVisible(false);
                softwareUpdateOffer.setVisible(softwareUpdateAvailable);
                break;
        }
        refreshButtons();
    }

    private void scheduleApplicationUpdateDialog() {
        SwingUtilities.invokeLater(() ->
            ApplicationUpdatePrompt.show(updateDialogParent.get(), softwareUpdateCoordinator));
    }

    private void showDeferredRestartReminder() {
        updateSoftwareStatus.setText("Update ready. Restart to finish updating.");
        updateSoftwareStatus.setVisible(true);
        updateSoftwareButton.setVisible(true);
        updateLaterButton.setVisible(false);
        softwareUpdateOffer.setVisible(true);
        refreshButtons();
    }

    static void applySoftwareUpdateOutcome(Autoupdate.UpdateOutcome outcome, JButton button, JLabel status,
                                           JButton laterButton) {
        if (outcome == Autoupdate.UpdateOutcome.UPDATED) {
            applySoftwareRestartRequired(button, status, laterButton);
            return;
        }
        if (outcome == Autoupdate.UpdateOutcome.AVAILABLE || outcome == Autoupdate.UpdateOutcome.FAILED) {
            boolean available = outcome == Autoupdate.UpdateOutcome.AVAILABLE;
            button.setText(available ? "Update now" : "Update application");
            Color statusColor = UIManager.getColor(available
                ? "Label.foreground"
                : "Console.ErrorText.foreground");
            if (statusColor != null) {
                status.setForeground(statusColor);
            }
            status.setText(outcome == Autoupdate.UpdateOutcome.AVAILABLE
                ? "Application update available."
                : "Automatic software update check failed. You can retry.");
            status.setVisible(true);
            button.setVisible(true);
            button.setEnabled(true);
            laterButton.setVisible(available);
        }
    }

    static void applySoftwareRestartRequired(JButton button, JLabel status,
                                             JButton laterButton) {
        Color normal = UIManager.getColor("Label.foreground");
        if (normal != null) {
            status.setForeground(normal);
        }
        button.setText("Restart now");
        button.setVisible(true);
        status.setText("Update ready. Restart the application to finish updating.");
        status.setVisible(true);
        laterButton.setText("Restart later");
        laterButton.setVisible(true);
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
        branchSwitchController.refreshButton();
        refreshFirmwareSummary();

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
        branchSwitchController.refreshButton();
        refreshFirmwareSummary();
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
            && !rollbackController.isDownloadInProgress();
        final boolean isEcuPortJobPossible = ecuPort.isPresent() && noUpdateInProgress;
        importTuneButton.setEnabled(isEcuPortJobPossible);
        updateSoftwareButton.setEnabled(
            updateSoftwareButton.isVisible() && softwareUpdateAvailable && noUpdateInProgress);
        updateLaterButton.setEnabled(updateLaterButton.isVisible() && noUpdateInProgress);
        rollbackController.refreshButton();
        branchSwitchController.refreshButton();
    }

    private void refreshFirmwareSummary() {
        Runnable update = () -> {
            if (firmwareSummary != null) {
                firmwareSummary.refresh();
            }
        };
        if (SwingUtilities.isEventDispatchThread()) {
            update.run();
        } else {
            SwingUtilities.invokeLater(update);
        }
    }

    private void refreshFirmwareStatusAfterMissingIniChange() {
        rollbackController.refresh(ecuPortToUse.get().orElse(null));
        refreshFirmwareSummary();
    }

    @Override
    public void disableButtons() {
        rollbackController.getRollbackButton().setEnabled(false);
        branchSwitchController.getButton().setEnabled(false);
        updateSoftwareButton.setEnabled(false);
        updateLaterButton.setEnabled(false);
        importTuneButton.setEnabled(false);
    }
}
