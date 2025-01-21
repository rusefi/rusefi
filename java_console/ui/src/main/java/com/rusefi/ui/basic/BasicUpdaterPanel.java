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
import java.awt.event.ActionEvent;
import java.util.List;
import java.util.Optional;
import java.util.Set;
import java.util.stream.Collectors;

import static com.devexperts.logging.Logging.getLogging;
import static com.rusefi.FileLog.isWindows;
import static com.rusefi.StartupFrame.newReleaseAnnounce;

public class BasicUpdaterPanel extends JPanel {
    private static final Logging log = getLogging(BasicUpdaterPanel.class);

    final UpdateOperationCallbacks updateOperationCallbacks;

    private final boolean isObfuscated = FindFileHelper.isObfuscated();

    private final JLabel statusMessage = new JLabel();
    private final JButton updateFirmwareButton = ProgramSelector.createUpdateFirmwareButton();
    private final JButton updateCalibrationsButton = new JButton(
        "Update Calibrations",
        AutoupdateUtil.loadIcon("writeconfig48.png")
    );
    private final UpdateCalibrations updateCalibrations = new UpdateCalibrations();
    private volatile Optional<AsyncJob> updateFirmwareJob = Optional.empty();
    private volatile Optional<SerialPortScanner.PortResult> portToUpdateCalibrations = Optional.empty();

    // Temporary feature flag for testing #7199
    // TODO: get rid of this feature flag after #7199 is completed
    private final boolean doNotUseStatusWindow;

    BasicUpdaterPanel(
        final boolean showUrlLabel,
        final UpdateOperationCallbacks updateOperationCallbacks,
        final boolean doNotUseStatusWindow
    ) {
        super(new VerticalFlowLayout());

        this.updateOperationCallbacks = updateOperationCallbacks;
        this.doNotUseStatusWindow = doNotUseStatusWindow;

        if (isWindows()) {
            final Optional<JPanel> newReleaseNotification = newReleaseAnnounce(
                "rusefi_updater.exe",
                "center",
                () -> 0
            );
            if (newReleaseNotification.isPresent()) {
                super.add(newReleaseNotification.get());
            }
            super.add(ToolButtons.createShowDeviceManagerButton());
            super.add(StartupFrame.binaryModificationControl());

            updateFirmwareButton.addActionListener(this::onUpdateFirmwareButtonClicked);
            updateFirmwareButton.setEnabled(false);

            statusMessage.setForeground(Color.red);
            super.add(statusMessage);
            super.add(updateFirmwareButton);
        } else {
            super.add(new JLabel("Sorry only works on Windows"));
        }

        super.add(new HorizontalLine());
        JLabel logoLabel = LogoHelper.createLogoLabel();
        if (logoLabel != null)
            super.add(logoLabel);
        if (showUrlLabel)
            super.add(LogoHelper.createUrlLabel());

        updateCalibrationsButton.addActionListener(this::onUpdateCalibrationsButtonClicked);
        updateCalibrationsButton.setEnabled(false);
        if (ConnectionAndMeta.showUpdateCalibrations()) {
            super.add(updateCalibrationsButton);
        }
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
        updatePortToUpdateCalibrations(currentHardware);
    }

    private void updateUpdateFirmwareJob(final AvailableHardware currentHardware) {
        if (currentHardware.isDfuFound()) {
            setUpdateFirmwareJob(new DfuManualJob());
        } else {
            final Set<SerialPortScanner.SerialPortType> portTypesToUpdateFirmware = (isObfuscated ?
                CompatibilitySet.of(
                    SerialPortScanner.SerialPortType.EcuWithOpenblt,
                    SerialPortScanner.SerialPortType.OpenBlt
                ) :
                CompatibilitySet.of(
                    SerialPortScanner.SerialPortType.Ecu,
                    SerialPortScanner.SerialPortType.EcuWithOpenblt
                )
            );
            final List<SerialPortScanner.PortResult> portsToUpdateFirmware = currentHardware.getKnownPorts(
                portTypesToUpdateFirmware
            );

            switch (portsToUpdateFirmware.size()) {
                case 0: {
                    resetUpdateFirmwareJob("ECU not found");
                    break;
                }
                case 1: {
                    final SerialPortScanner.PortResult portToUpdateFirmware = portsToUpdateFirmware.get(0);
                    AsyncJob job = null;
                    if (isObfuscated) {
                        final SerialPortScanner.SerialPortType portType = portToUpdateFirmware.type;
                        switch (portType) {
                            case EcuWithOpenblt: {
                                job = new OpenBltAutoJob(portToUpdateFirmware, updateFirmwareButton);
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
                    } else {
                        job = new DfuAutoJob(portToUpdateFirmware, updateFirmwareButton);
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
        updateFirmwareButton.setEnabled(true);
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

    private void updatePortToUpdateCalibrations(final AvailableHardware currentHardware) {
        final List<SerialPortScanner.PortResult> ecuPortsToUpdateCalibrations = currentHardware.getKnownPorts(CompatibilitySet.of(
            SerialPortScanner.SerialPortType.Ecu,
            SerialPortScanner.SerialPortType.EcuWithOpenblt
        ));

        switch (ecuPortsToUpdateCalibrations.size()) {
            case 0: {
                resetPortToUpdateCalibrations();
                log.info("No ECU ports to update calibrations found");
                break;
            }
            case 1: {
                setPortToUpdateCalibrations(ecuPortsToUpdateCalibrations.get(0));
                break;
            }
            default: {
                resetPortToUpdateCalibrations();
                log.info(String.format(
                    "Multiple ECU ports to update calibrations found on: %s",
                    ecuPortsToUpdateCalibrations.stream()
                        .map(portResult -> portResult.port)
                        .collect(Collectors.joining(", "))
                ));
                break;
            }
        }
    }

    private void setPortToUpdateCalibrations(final SerialPortScanner.PortResult port) {
        portToUpdateCalibrations = Optional.of(port);
        updateCalibrationsButton.setEnabled(true);
    }

    private void resetPortToUpdateCalibrations() {
        portToUpdateCalibrations = Optional.empty();
        updateCalibrationsButton.setEnabled(false);
    }

    private void onUpdateFirmwareButtonClicked(final ActionEvent actionEvent) {
        CompatibilityOptional.ifPresentOrElse(updateFirmwareJob,
            value -> {
                if (doNotUseStatusWindow) {
                    AsyncJobExecutor.INSTANCE.executeJob(value, updateOperationCallbacks);
                } else {
                    AsyncJobExecutor.INSTANCE.executeJobWithStatusWindow(value, updateOperationCallbacks);
                }
            },
            () -> log.error("Update firmware job is is not defined.")
        );
    }

    private void onUpdateCalibrationsButtonClicked(final ActionEvent actionEvent) {
        CompatibilityOptional.ifPresentOrElse(portToUpdateCalibrations,
            port -> {
                updateCalibrations.updateCalibrationsAction(port, updateCalibrationsButton);
            }, () -> {
                JOptionPane.showMessageDialog(
                    updateCalibrationsButton,
                    "Device is not connected",
                    "Error",
                    JOptionPane.ERROR_MESSAGE
                );
            }
        );
    }
}
