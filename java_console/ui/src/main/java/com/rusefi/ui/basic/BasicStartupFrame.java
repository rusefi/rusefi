package com.rusefi.ui.basic;

import com.devexperts.logging.Logging;

import com.rusefi.AvailableHardware;
import com.rusefi.Launcher;
import com.rusefi.SerialPortScanner;
import com.rusefi.StartupFrame;
import com.rusefi.core.FindFileHelper;
import com.rusefi.core.net.ConnectionAndMeta;
import com.rusefi.core.ui.AutoupdateUtil;
import com.rusefi.core.ui.FrameHelper;
import com.rusefi.maintenance.*;
import com.rusefi.maintenance.jobs.*;
import com.rusefi.ui.BasicLogoHelper;
import com.rusefi.ui.LogoHelper;
import com.rusefi.ui.util.DefaultExceptionHandler;
import com.rusefi.ui.util.HorizontalLine;
import com.rusefi.ui.util.UiUtils;
import com.rusefi.ui.widgets.ToolButtons;
import org.putgemin.VerticalFlowLayout;

import javax.swing.*;

import java.awt.Color;
import java.awt.event.ActionEvent;
import java.util.List;
import java.util.Optional;
import java.util.Set;
import java.util.stream.Collectors;

import static com.devexperts.logging.Logging.getLogging;
import static com.rusefi.FileLog.isWindows;

/**
 * Focuses on firmware updater
 * Much simpler than {@link com.rusefi.StartupFrame}
 */
public class BasicStartupFrame {
    private static final Logging log = getLogging(BasicStartupFrame.class);


    private final String whiteLabel = ConnectionAndMeta.getWhiteLabel(ConnectionAndMeta.getProperties());
    private final FrameHelper frame = FrameHelper.createFrame(
        whiteLabel + " basic console " + Launcher.CONSOLE_VERSION
    );
    private final boolean isObfuscated = FindFileHelper.isObfuscated();

    private final JLabel statusMessage = new JLabel();
    private final StatusAnimation status = new StatusAnimation(this::updateStatus, StartupFrame.SCANNING_PORTS);
    private final JButton updateFirmwareButton = ProgramSelector.createUpdateFirmwareButton();
    private final JButton updateCalibrationsButton = new JButton(
        "Update Calibrations",
        AutoupdateUtil.loadIcon("writeconfig48.png")
    );
    private final UpdateCalibrations updateCalibrations = new UpdateCalibrations();

    private volatile Optional<AsyncJob> updateFirmwareJob = Optional.empty();
    private volatile Optional<SerialPortScanner.PortResult> portToUpdateCalibrations = Optional.empty();

    public static void main(String[] args) {
        runTool(null);
    }

    public static void runTool(String[] args) {
        DefaultExceptionHandler.install();
        new BasicStartupFrame();
    }

    public BasicStartupFrame() {
        JPanel panel = new JPanel(new VerticalFlowLayout());
        if (isWindows()) {
            panel.add(ToolButtons.createShowDeviceManagerButton());
            panel.add(StartupFrame.binaryModificationControl());

            updateFirmwareButton.addActionListener(this::onUpdateFirmwareButtonClicked);
            updateFirmwareButton.setEnabled(false);

            statusMessage.setForeground(Color.red);
            panel.add(statusMessage);
            panel.add(updateFirmwareButton);
        } else {
            panel.add(new JLabel("Sorry only works on Windows"));
        }
        SerialPortScanner.INSTANCE.addListener(currentHardware -> SwingUtilities.invokeLater(() -> {
            onHardwareUpdated(currentHardware);
        }));

        panel.add(new HorizontalLine());
        JLabel logoLabel = LogoHelper.createLogoLabel();
        if (logoLabel != null)
            panel.add(logoLabel);
        if (ConnectionAndMeta.isDefaultWhitelabel(whiteLabel))
            panel.add(LogoHelper.createUrlLabel());

        updateCalibrationsButton.addActionListener(this::onUpdateCalibrationsButtonClicked);
        updateCalibrationsButton.setEnabled(false);
        if (ConnectionAndMeta.showUpdateCalibrations()) {
            panel.add(updateCalibrationsButton);
        }

        BasicLogoHelper.setGenericFrameIcon(frame.getFrame());
        frame.showFrame(panel, false);
        UiUtils.centerWindow(frame.getFrame());
        packFrame();
    }

    private void packFrame() {
        frame.getFrame().pack();
    }

    private void hideStatusMessage() {
        // we use .setText(" ") instead of .setVisible(false) to avoid layout contraction
        statusMessage.setText(" ");
    }

    private void updateStatus(final String niceStatus) {
        statusMessage.setText(niceStatus);
        // I'm not sure why it works, but it looks like the following frame packing helps to avoid displaying of logo on
        // the right side of frame
        packFrame();
    }

    private void onHardwareUpdated(final AvailableHardware currentHardware) {
        status.stop();
        hideStatusMessage();
        // I'm not sure if the following frame packing is really necessary, but I'm adding it just in case if frame was
        // not packed in updateStatus method
        packFrame();

        updateUpdateFirmwareJob(currentHardware);
        updatePortToUpdateCalibrations(currentHardware);
    }

    private void updateUpdateFirmwareJob(final AvailableHardware currentHardware) {
        if (currentHardware.isDfuFound()) {
            setUpdateFirmwareJob(new DfuManualJob());
        } else {
            final Set<SerialPortScanner.SerialPortType> portTypesToUpdateFirmware = (isObfuscated ?
                Set.of(
                    SerialPortScanner.SerialPortType.EcuWithOpenblt,
                    SerialPortScanner.SerialPortType.OpenBlt
                ) :
                Set.of(
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
        final List<SerialPortScanner.PortResult> ecuPortsToUpdateCalibrations = currentHardware.getKnownPorts(Set.of(
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
        updateFirmwareJob.ifPresentOrElse(
            ProgramSelector::executeJob,
            () -> log.error("Update firmware job is is not defined.")
        );
    }

    private void onUpdateCalibrationsButtonClicked(final ActionEvent actionEvent) {
        portToUpdateCalibrations.ifPresentOrElse(
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
