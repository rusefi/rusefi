package com.rusefi.ui.basic;

import com.devexperts.logging.Logging;

import com.opensr5.ConfigurationImage;
import com.opensr5.io.ConfigurationImageFile;
import com.rusefi.AvailableHardware;
import com.rusefi.Launcher;
import com.rusefi.SerialPortScanner;
import com.rusefi.StartupFrame;
import com.rusefi.autodetect.PortDetector;
import com.rusefi.core.FindFileHelper;
import com.rusefi.core.net.ConnectionAndMeta;
import com.rusefi.core.preferences.storage.PersistentConfiguration;
import com.rusefi.core.ui.AutoupdateUtil;
import com.rusefi.core.ui.FrameHelper;
import com.rusefi.maintenance.*;
import com.rusefi.ui.LogoHelper;
import com.rusefi.ui.util.DefaultExceptionHandler;
import com.rusefi.ui.util.HorizontalLine;
import com.rusefi.ui.util.UiUtils;
import com.rusefi.ui.widgets.ToolButtons;
import org.putgemin.VerticalFlowLayout;

import javax.swing.*;
import javax.swing.filechooser.FileNameExtensionFilter;

import java.awt.Color;
import java.awt.event.ActionEvent;
import java.io.File;
import java.io.IOException;
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

    private static final String BINARY_IMAGE_DEFAULT_DIRECTORY_PROPERTY_NAME = "binary_image_default_directory";

    private final String whiteLabel = ConnectionAndMeta.getWhiteLabel(ConnectionAndMeta.getProperties());
    private final FrameHelper frame = FrameHelper.createFrame(
        whiteLabel + " basic console " + Launcher.CONSOLE_VERSION
    );
    private final boolean isObfusacted = FindFileHelper.isObfuscated();

    private final JLabel statusMessage = new JLabel();
    private final StatusAnimation status = new StatusAnimation(this::updateStatus, StartupFrame.SCANNING_PORTS);
    private final JButton updateFirmwareButton = ProgramSelector.createUpdateFirmwareButton();
    private final JButton updateCalibrationsButton = new JButton(
        "Update Calibrations",
        AutoupdateUtil.loadIcon("writeconfig48.png")
    );
    private final JFileChooser calibrationsFileChooser = createConfigurationImageFileChooser();

    private volatile Optional<SerialPortScanner.PortResult> portToUpdateObfuscatedFirmware = Optional.empty();
    private volatile Optional<SerialPortScanner.PortResult> portToUpdateCalibrations = Optional.empty();

    public static void main(String[] args) {
        runTool(null);
    }

    public static void runTool(String[] args) {
        DefaultExceptionHandler.install();
        new BasicStartupFrame().runTool();
    }

    public BasicStartupFrame() {
        JPanel panel = new JPanel(new VerticalFlowLayout());
        if (isWindows()) {
            panel.add(ToolButtons.createShowDeviceManagerButton());
            panel.add(StartupFrame.binaryModificationControl());

            updateFirmwareButton.addActionListener(this::onUpdateFirmwareButtonClicked);
            if (isObfusacted) {
                updateFirmwareButton.setEnabled(false);

                statusMessage.setForeground(Color.red);
                panel.add(statusMessage);
            }
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
        panel.add(updateCalibrationsButton);

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

        if (isObfusacted) {
            updatePortToUpdateObfuscatedFirmware(currentHardware);
        }
        updatePortToUpdateCalibrations(currentHardware);
    }

    private void updatePortToUpdateObfuscatedFirmware(final AvailableHardware currentHardware) {
        final List<SerialPortScanner.PortResult> portsToUpdateObfuscatedFirmware = currentHardware.getKnownPorts(Set.of(
            SerialPortScanner.SerialPortType.EcuWithOpenblt,
            SerialPortScanner.SerialPortType.OpenBlt
        ));

        switch (portsToUpdateObfuscatedFirmware.size()) {
            case 0: {
                resetPortToUpdateObfuscatedFirmware("ECU not found");
                break;
            }
            case 1: {
                switchToPortToUpdateObfuscatedFirmware(portsToUpdateObfuscatedFirmware.get(0));
                break;
            }
            default: {
                resetPortToUpdateObfuscatedFirmware(String.format(
                    "Multiple ECUs found on: %s",
                    portsToUpdateObfuscatedFirmware.stream()
                        .map(portResult -> portResult.port)
                        .collect(Collectors.joining(", "))
                ));
                break;
            }
        }
    }

    private void switchToPortToUpdateObfuscatedFirmware(final SerialPortScanner.PortResult port) {
        portToUpdateObfuscatedFirmware = Optional.of(port);
        hideStatusMessage();
        updateFirmwareButton.setEnabled(true);
        switch (port.type) {
            case EcuWithOpenblt: {
                updateFirmwareButton.setText("Auto Update Firmware");
                break;
            }
            case OpenBlt: {
                updateFirmwareButton.setText("Blt Update Firmware");
                break;
            }
            default: {
                log.error(String.format("Unexpected port type: %s", port));
                break;
            }
        }
    }

    private void resetPortToUpdateObfuscatedFirmware(final String reason) {
        portToUpdateObfuscatedFirmware = Optional.empty();
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
        if (isObfusacted) {
            portToUpdateObfuscatedFirmware.ifPresentOrElse(port -> {
                    switch (port.type) {
                        case EcuWithOpenblt: {
                            ProgramSelector.executeJob(updateFirmwareButton, ProgramSelector.OPENBLT_AUTO, port);
                            break;
                        }
                        case OpenBlt: {
                            ProgramSelector.executeJob(updateFirmwareButton, ProgramSelector.OPENBLT_MANUAL, port);
                            break;
                        }
                        default: {
                            log.error(String.format("Unexpected port type: %s (%s)", port.type, port));
                            break;
                        }
                    }
                }, () -> {
                    log.error("Port to update firmware is not defined.");
                }
            );
        } else {
            DfuFlasher.doAutoDfu(updateFirmwareButton, PortDetector.AUTO, new UpdateStatusWindow("Update"));
        }
    }

    private void onUpdateCalibrationsButtonClicked(final ActionEvent actionEvent) {
        portToUpdateCalibrations.ifPresentOrElse(
            port -> {
                final int selectedOption = calibrationsFileChooser.showOpenDialog(updateCalibrationsButton);
                if (selectedOption == JFileChooser.APPROVE_OPTION) {
                    final File selectedFile = calibrationsFileChooser.getSelectedFile();
                    saveBinaryImageDefaultDirectory(selectedFile.getParent());
                    try {
                        final ConfigurationImage calibrationsImage = ConfigurationImageFile.readFromFile(
                            selectedFile.getAbsolutePath()
                        );
                        CalibrationsUpdater.INSTANCE.setCalibrationsToUpload(calibrationsImage);
                        ProgramSelector.executeJob(
                            updateCalibrationsButton,
                            ProgramSelector.UPDATE_CALIBRATIONS,
                            port
                        );
                    } catch (final IOException e) {
                        final String errorMsg = String.format(
                            "Failed to load calibrations from file %s",
                            selectedFile.getAbsolutePath()
                        );
                        log.error(errorMsg, e);
                        JOptionPane.showMessageDialog(
                            updateCalibrationsButton,
                            errorMsg,
                            "Error",
                            JOptionPane.ERROR_MESSAGE
                        );
                    }
                }
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

    private void saveBinaryImageDefaultDirectory(final String path) {
        PersistentConfiguration.getConfig().getRoot().setProperty(
            BINARY_IMAGE_DEFAULT_DIRECTORY_PROPERTY_NAME,
            path
        );
        PersistentConfiguration.getConfig().save();
    }

    private String loadBinaryImageDefaultDirectory() {
        return PersistentConfiguration.getConfig().getRoot().getProperty(
            BINARY_IMAGE_DEFAULT_DIRECTORY_PROPERTY_NAME,
            ""
        );
    }

    private void runTool() {
    }

    private JFileChooser createConfigurationImageFileChooser() {
        final JFileChooser fc = new JFileChooser();
        fc.setFileSelectionMode(JFileChooser.FILES_ONLY);
        fc.setFileFilter(new FileNameExtensionFilter("Binary image files (.binary_image)", "binary_image"));

        final String currentDirectory = loadBinaryImageDefaultDirectory();
        if (currentDirectory != null) {
            fc.setCurrentDirectory(new File(currentDirectory));
        }

        return fc;
    }
}
