package com.rusefi.ui.basic;

import com.devexperts.logging.Logging;
import com.opensr5.ini.IniFileModel;
import com.rusefi.*;
import com.rusefi.core.FindFileHelper;
import com.rusefi.core.ui.AutoupdateUtil;
import com.rusefi.io.UpdateOperationCallbacks;
import com.rusefi.maintenance.CalibrationsInfo;
import com.rusefi.maintenance.ProgramSelector;
import com.rusefi.maintenance.jobs.*;
import com.rusefi.ui.LogoHelper;
import com.rusefi.ui.util.HorizontalLine;
import com.rusefi.ui.widgets.ToolButtons;
import com.rusefi.util.CompatibilityOptional;
import org.jetbrains.annotations.NotNull;
import org.putgemin.VerticalFlowLayout;

import javax.swing.*;
import java.awt.*;
import java.awt.event.*;
import java.util.List;
import java.util.Optional;
import java.util.Set;
import java.util.concurrent.atomic.AtomicReference;
import java.util.stream.Collectors;

import static com.devexperts.logging.Logging.getLogging;
import static com.rusefi.FileLog.isWindows;
import static com.rusefi.StartupFrame.newReleaseAnnounce;
import static com.rusefi.core.net.ConnectionAndMeta.getProperties;
import static com.rusefi.ui.basic.UnitLabelPrinter.UNIT_IDENTIFIER_FIELD_NAMES;

public class BasicUpdaterPanel implements BasicButtonCoordinator {
    private static final Logging log = getLogging(BasicUpdaterPanel.class);

    private final JPanel content = new JPanel(new VerticalFlowLayout());

    private final boolean isObfuscated = FindFileHelper.isObfuscated();

    private final String panamaUrl = getProperties().getProperty("panama_url");

    private final JLabel statusMessage = new JLabel();
    private final JCheckBox migrateSettings = new JCheckBox("Migrate Settings");
    private final JCheckBox verboseMessages = new JCheckBox("Verbose Status");

    private final JButton updateFirmwareButton = ProgramSelector.createUpdateFirmwareButton();
    // todo: this control lives on a different parent TODO fix this mess!
    private final ImportTuneControl importTuneButton;

    private final JButton updateCalibrationsButton = new JButton(
        "Update Calibrations",
        AutoupdateUtil.loadIcon("writeconfig48.png")
    );

    private LogoLabelPopupMenu logoLabelPopupMenu = null;

    private final ConnectivityContext connectivityContext;
    private final SingleAsyncJobExecutor singleAsyncJobExecutor;
    private final UpdateOperationCallbacks updateOperationCallbacks;
//    private final UpdateCalibrations updateCalibrations;
    private volatile Optional<AsyncJob> updateFirmwareJob = Optional.empty();
    private final AtomicReference<Optional<PortResult>> ecuPortToUse;

    private String latestReportedHash;

    BasicUpdaterPanel(
        ConnectivityContext connectivityContext, final boolean showUrlLabel,
        final UpdateOperationCallbacks updateOperationCallbacks, SingleAsyncJobExecutor singleAsyncJobExecutor, AtomicReference<Optional<PortResult>> ecuPortToUse
    ) {
        this.connectivityContext = connectivityContext;
        this.ecuPortToUse = ecuPortToUse;
        this.singleAsyncJobExecutor = singleAsyncJobExecutor;
        this.updateOperationCallbacks = updateOperationCallbacks;
        singleAsyncJobExecutor.addOnJobInProgressFinishedListener(() -> SwingUtilities.invokeLater(this::refreshButtons));
        importTuneButton = new ImportTuneControl(singleAsyncJobExecutor, this,
            connectivityContext,
            this.ecuPortToUse);
//        updateCalibrations = new UpdateCalibrations(singleAsyncJobExecutor);

        if (isWindows()) {
            final Optional<JPanel> newReleaseNotification = newReleaseAnnounce(
                "rusefi_updater.exe",
                "center",
                () -> 0
            );
            if (newReleaseNotification.isPresent()) {
                content.add(newReleaseNotification.get());
            }
            content.add(ToolButtons.createShowDeviceManagerButton());

            content.add(StartupFrame.binaryModificationControl());

            updateFirmwareButton.addActionListener(this::onUpdateFirmwareButtonClicked);
            updateFirmwareButton.setEnabled(false);

            statusMessage.setForeground(Color.red);
            content.add(statusMessage);
            content.add(updateFirmwareButton);

            importTuneButton.setEnabled(false);
        } else {
            content.add(new JLabel("Sorry only works on Windows"));
        }

        content.add(new HorizontalLine());
        JLabel logoLabel = LogoHelper.createLogoLabel();
        if (logoLabel != null) {
            logoLabelPopupMenu = new LogoLabelPopupMenu(this::uploadTune, this::printUnitLabel, panamaUrl != null);
            logoLabel.setComponentPopupMenu(logoLabelPopupMenu);
            content.add(logoLabel);
        }
        if (showUrlLabel)
            content.add(LogoHelper.createUrlLabel());

/*
never used?
        if (ConnectionAndMeta.showUpdateCalibrations()) {
            updateCalibrationsButton.addActionListener(this::onUpdateCalibrationsButtonClicked);
            updateCalibrationsButton.setEnabled(false);
            content.add(updateCalibrationsButton);
        }
  */
        migrateSettings.setSelected(true);
        migrateSettings.addActionListener(e -> updateMigrateSettingState());

        verboseMessages.setSelected(false);
        verboseMessages.addActionListener(e -> updateMigrateSettingState());

        updateMigrateSettingState();
        content.add(migrateSettings);
//        content.add(verboseMessages);
    }

    public ImportTuneControl getImportTuneButton() {
        return importTuneButton;
    }

    private void updateMigrateSettingState() {
        MigrateSettingsCheckboxState.isMigrationNeeded = migrateSettings.isSelected();
        MigrateSettingsCheckboxState.isVerboseMessages = verboseMessages.isSelected();
    }

    private void hideStatusMessage() {
        // we use .setText(" ") instead of .setVisible(false) to avoid layout contraction
        statusMessage.setText(" ");
    }

    public void updateStatus(final String niceStatus) {
        statusMessage.setText(niceStatus);
    }

    public void onHardwareUpdated() {
        hideStatusMessage();

        updateUpdateFirmwareJob();
        updateEcuPortToUse();
    }

    private void updateUpdateFirmwareJob() {
        final AvailableHardware currentHardware = connectivityContext.getCurrentHardware();
        log.info("updateUpdateFirmwareJob " + currentHardware);
        final List<PortResult> portsToUpdateFirmware = getPortResults(currentHardware);
        switch (portsToUpdateFirmware.size()) {
            case 1: {
                // OpenBlt first preference
                setUpdateFirmwareJob(getNonDfuUpdateFirmwareJobForPort(portsToUpdateFirmware.get(0)));
                break;
            }
            case 0: {
                // fallback to DFU which is more fragile
                setUpdateFirmwareJob(new DfuManualJob());
                break;
            }
            default: {
                resetUpdateFirmwareJob(String.format(
                    "Multiple ECUs found on: %s",
                    portsToUpdateFirmware.stream()
                        .map(portResult -> portResult.port)
                        .collect(Collectors.joining(", "))
                ));
                break;
            }
        }
    }

    private AsyncJob getNonDfuUpdateFirmwareJobForPort(final PortResult portToUpdateFirmware) {
        AsyncJob job = null;
        final SerialPortType portType = portToUpdateFirmware.type;
        switch (portType) {
            case Ecu: {
                job = new DfuAutoJob(portToUpdateFirmware, updateFirmwareButton, connectivityContext);
                break;
            }
            case EcuWithOpenblt: {
                job = new OpenBltAutoJob(portToUpdateFirmware, updateFirmwareButton, connectivityContext);
                break;
            }
            case OpenBlt: {
                job = new OpenBltManualJob(portToUpdateFirmware, updateFirmwareButton);
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
        hideStatusMessage();
        refreshButtons();
    }

    private void resetUpdateFirmwareJob(final String reason) {
        updateFirmwareJob = Optional.empty();
        updateFirmwareButton.setEnabled(false);
        statusMessage.setText(reason);
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
        SwingUtilities.invokeLater(() -> {
            importTuneButton.setEnabled(false);
//            updateCalibrationsButton.setEnabled(false);
            if (logoLabelPopupMenu != null) {
                logoLabelPopupMenu.refreshUploadTuneAndPrintUnitLabelsMenuItems(false, false);
            }
        });
    }

    private void onUpdateFirmwareButtonClicked(final ActionEvent actionEvent) {
        disableButtons();
        CompatibilityOptional.ifPresentOrElse(updateFirmwareJob,
            value -> {
                singleAsyncJobExecutor.startJob(value, updateFirmwareButton);
            },
            () -> log.error("Update firmware job is is not defined.")
        );
        refreshButtons();
    }

    /*
    private void onUpdateCalibrationsButtonClicked(final ActionEvent actionEvent) {
        disableButtons();
        CompatibilityOptional.ifPresentOrElse(ecuPortToUse,
            port -> {
                updateCalibrations.updateCalibrationsAction(port, updateCalibrationsButton, connectivityContext);
            }, () -> {
                JOptionPane.showMessageDialog(
                    updateCalibrationsButton,
                    "Device is not connected",
                    "Error",
                    JOptionPane.ERROR_MESSAGE
                );
            }
        );
        refreshButtons();
    }
    */

    public void refreshButtons() {
        refreshUpdateFirmwareButton();
        final Optional<PortResult> ecuPort = ecuPortToUse.get();
        final boolean isEcuPortJobPossible = ecuPort.isPresent() && singleAsyncJobExecutor.isNotInProgress();
        importTuneButton.setEnabled(isEcuPortJobPossible);
//        updateCalibrationsButton.setEnabled(isEcuPortJobPossible);
        if (logoLabelPopupMenu != null) {
            logoLabelPopupMenu.refreshUploadTuneAndPrintUnitLabelsMenuItems(
                isEcuPortJobPossible,
                ecuPort.map(port -> existsAnyOfUnitIdentifierFields(port.getCalibrations().getIniFile())).orElse(false)
            );
        }
    }

    private void refreshUpdateFirmwareButton() {
        final boolean isFirmwareUpdatePossible =
            updateFirmwareJob.isPresent() && singleAsyncJobExecutor.isNotInProgress();
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

    private boolean existsAnyOfUnitIdentifierFields(final IniFileModel iniFile) {
        for (final String fieldName : UNIT_IDENTIFIER_FIELD_NAMES) {
            if (iniFile.findIniField(fieldName).isPresent()) {
                return true;
            }
        }
        return false;
    }

    @Override
    public void disableButtons() {
        updateFirmwareButton.setEnabled(false);
        importTuneButton.setEnabled(false);
//        updateCalibrationsButton.setEnabled(false);
        if (logoLabelPopupMenu != null) {
            logoLabelPopupMenu.refreshUploadTuneAndPrintUnitLabelsMenuItems(false, false);
        }
    }

    private void uploadTune() {
        disableButtons();
        CompatibilityOptional.ifPresentOrElse(ecuPortToUse.get(),
            port -> {
                singleAsyncJobExecutor.startJob(new UploadTuneJob(connectivityContext, port, panamaUrl), logoLabelPopupMenu);
            }, () -> {
                JOptionPane.showMessageDialog(
                    content,
                    "Device is not connected",
                    "Error",
                    JOptionPane.ERROR_MESSAGE
                );
            }
        );
        refreshButtons();
    }

    private void printUnitLabel() {
        disableButtons();
        CompatibilityOptional.ifPresentOrElse(ecuPortToUse.get(),
            port -> {
                singleAsyncJobExecutor.startJob(new PrintUnitLabelJob(connectivityContext, port, logoLabelPopupMenu), logoLabelPopupMenu);
            }, () -> {
                JOptionPane.showMessageDialog(
                    updateCalibrationsButton,
                    "Device is not connected",
                    "Error",
                    JOptionPane.ERROR_MESSAGE
                );
            }
        );
        refreshButtons();
    }

    public Component getContent() {
        return content;
    }
}
