package com.rusefi.ui.basic;

import com.devexperts.logging.Logging;
import com.rusefi.*;
import com.rusefi.core.FindFileHelper;
import com.rusefi.core.net.ConnectionAndMeta;
import com.rusefi.core.ui.AutoupdateUtil;
import com.rusefi.io.UpdateOperationCallbacks;
import com.rusefi.maintenance.ProgramSelector;
import com.rusefi.maintenance.jobs.*;
import com.rusefi.ui.LogoHelper;
import com.rusefi.ui.util.HorizontalLine;
import com.rusefi.ui.widgets.ToolButtons;
import org.putgemin.VerticalFlowLayout;

import javax.swing.*;
import java.awt.*;
import java.awt.event.*;
import java.util.List;
import java.util.Optional;
import java.util.Set;
import java.util.stream.Collectors;

import static com.devexperts.logging.Logging.getLogging;
import static com.rusefi.FileLog.isWindows;
import static com.rusefi.StartupFrame.newReleaseAnnounce;
import static com.rusefi.core.net.ConnectionAndMeta.getProperties;

public class BasicUpdaterPanel {
    private static final Logging log = getLogging(BasicUpdaterPanel.class);

    private final JPanel content = new JPanel(new VerticalFlowLayout());

    private final boolean isObfuscated = FindFileHelper.isObfuscated();

    private final String panamaUrl = getProperties().getProperty("panama_url");

    private final JLabel statusMessage = new JLabel();
    private final JCheckBox migrateSettings = new JCheckBox("Migrate Settings");

    private final JButton updateFirmwareButton = ProgramSelector.createUpdateFirmwareButton();
    private final JButton updateCalibrationsButton = new JButton(
        "Update Calibrations",
        AutoupdateUtil.loadIcon("writeconfig48.png")
    );

    private LogoLabelPopupMenu logoLabelPopupMenu = null;

    private final ConnectivityContext connectivityContext;
    private final SingleAsyncJobExecutor singleAsyncJobExecutor;
    private final UpdateOperationCallbacks updateOperationCallbacks;
    private final UpdateCalibrations updateCalibrations;
    private volatile Optional<AsyncJob> updateFirmwareJob = Optional.empty();
    private volatile Optional<PortResult> ecuPortToUse = Optional.empty();

    BasicUpdaterPanel(
        ConnectivityContext connectivityContext,
        final boolean showUrlLabel,
        final UpdateOperationCallbacks updateOperationCallbacks,
        final boolean doNotUseStatusWindow
    ) {
        this.connectivityContext = connectivityContext;
        singleAsyncJobExecutor = new SingleAsyncJobExecutor(
            updateOperationCallbacks,
            doNotUseStatusWindow,
            () -> SwingUtilities.invokeLater(this::refreshButtons)
        );
        this.updateOperationCallbacks = updateOperationCallbacks;
        updateCalibrations = new UpdateCalibrations(singleAsyncJobExecutor);

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
        updateMigrateSettingState();
        content.add(migrateSettings);
    }

    private void updateMigrateSettingState() {
        MigrateSettingsCheckboxState.isMigrationNeeded = migrateSettings.isSelected();
    }

    private void hideStatusMessage() {
        // we use .setText(" ") instead of .setVisible(false) to avoid layout contraction
        statusMessage.setText(" ");
    }

    public void updateStatus(final String niceStatus) {
        statusMessage.setText(niceStatus);
    }

    public void onHardwareUpdated(final AvailableHardware currentHardware) {
        hideStatusMessage();

        updateUpdateFirmwareJob(currentHardware);
        updateEcuPortToUse(currentHardware);
    }

    private void updateUpdateFirmwareJob(final AvailableHardware currentHardware) {
        if (currentHardware.isDfuFound()) {
            setUpdateFirmwareJob(new DfuManualJob());
        } else {
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
            final List<PortResult> portsToUpdateFirmware = currentHardware.getKnownPorts(
                portTypesToUpdateFirmware
            );

            switch (portsToUpdateFirmware.size()) {
                case 0: {
                    resetUpdateFirmwareJob("ECU not found");
                    break;
                }
                case 1: {
                    final PortResult portToUpdateFirmware = portsToUpdateFirmware.get(0);
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
                    setUpdateFirmwareJob(job);
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
    }

    private void setUpdateFirmwareJob(final AsyncJob updateFirmwareJob) {
        this.updateFirmwareJob = Optional.of(updateFirmwareJob);
        hideStatusMessage();
        refreshButtons();
        Optional<String> updateFirmwareButtonText = Optional.empty();
        if (updateFirmwareJob instanceof OpenBltAutoJob) {
            updateFirmwareButtonText = Optional.of("Auto Update Firmware");
        } else if (updateFirmwareJob instanceof OpenBltManualJob) {
            updateFirmwareButtonText = Optional.of("Blt Update Firmware");
        } else if (updateFirmwareJob instanceof DfuAutoJob) {
            updateFirmwareButtonText = Optional.of("Update Firmware");
        } else if (updateFirmwareJob instanceof DfuManualJob) {
            updateFirmwareButtonText = Optional.of("Update Firmware via DFU");
        } else {
            log.error(String.format("Unexpected job type: %s", updateFirmwareJob.getClass().getSimpleName()));
        }
        updateFirmwareButtonText.ifPresent(updateFirmwareButton::setText);
    }

    private void resetUpdateFirmwareJob(final String reason) {
        updateFirmwareJob = Optional.empty();
        updateFirmwareButton.setEnabled(false);
        statusMessage.setText(reason);
    }

    private void updateEcuPortToUse(final AvailableHardware currentHardware) {
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
        ecuPortToUse = Optional.of(port);

        SwingUtilities.invokeLater(() -> {
            refreshButtons();
            if (port.getFirmwareHash().isPresent()) {
                updateOperationCallbacks.logLine("Detected " + port.getFirmwareHash().get());
            }
        });
    }

    private void resetEcuPortToUse() {
        ecuPortToUse = Optional.empty();
        SwingUtilities.invokeLater(() -> {
            updateCalibrationsButton.setEnabled(false);
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

    private void refreshButtons() {
        updateFirmwareButton.setEnabled(updateFirmwareJob.isPresent() && singleAsyncJobExecutor.isNotInProgress());
        final Optional<PortResult> ecuPort = ecuPortToUse;
        final boolean isEcuPortJobPossible = ecuPort.isPresent() && singleAsyncJobExecutor.isNotInProgress();
        updateCalibrationsButton.setEnabled(isEcuPortJobPossible);
        if (logoLabelPopupMenu != null) {
            logoLabelPopupMenu.refreshUploadTuneAndPrintUnitLabelsMenuItems(
                isEcuPortJobPossible,
                true
            );
        }
    }

    private void disableButtons() {
        updateFirmwareButton.setEnabled(false);
        updateCalibrationsButton.setEnabled(false);
        if (logoLabelPopupMenu != null) {
            logoLabelPopupMenu.refreshUploadTuneAndPrintUnitLabelsMenuItems(false, false);
        }
    }

    private void uploadTune() {
        disableButtons();
        CompatibilityOptional.ifPresentOrElse(ecuPortToUse,
            port -> {
                singleAsyncJobExecutor.startJob(new UploadTuneJob(connectivityContext, port, panamaUrl), logoLabelPopupMenu);
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

    private void printUnitLabel() {
        disableButtons();
        CompatibilityOptional.ifPresentOrElse(ecuPortToUse,
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
