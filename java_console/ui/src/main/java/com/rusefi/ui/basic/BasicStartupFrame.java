package com.rusefi.ui.basic;

import com.devexperts.logging.Logging;
import com.rusefi.AvailableHardware;
import com.rusefi.Launcher;
import com.rusefi.SerialPortScanner;
import com.rusefi.StartupFrame;
import com.rusefi.autodetect.PortDetector;
import com.rusefi.core.FindFileHelper;
import com.rusefi.core.net.ConnectionAndMeta;
import com.rusefi.core.ui.FrameHelper;
import com.rusefi.maintenance.DfuFlasher;
import com.rusefi.maintenance.ProgramSelector;
import com.rusefi.maintenance.StatusAnimation;
import com.rusefi.maintenance.UpdateStatusWindow;
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
import java.util.stream.Stream;

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
    private final boolean requireBlt = FindFileHelper.isObfuscated();

    private final JLabel statusMessage = new JLabel();
    private final StatusAnimation status = new StatusAnimation(this::updateStatus, StartupFrame.SCANNING_PORTS);
    private final JButton updateFirmwareButton = ProgramSelector.createUpdateFirmwareButton();

    private volatile Optional<SerialPortScanner.PortResult> portToUpdateFirmwareWithBlt = Optional.empty();

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
            if (requireBlt) {
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

        updatePortToUpdateFirmwareWithBlt(currentHardware);
    }

    private void updatePortToUpdateFirmwareWithBlt(final AvailableHardware currentHardware) {
        final List<SerialPortScanner.PortResult> portsToUpdateFirmwareWithBlt = currentHardware.getKnownPorts(Set.of(
            SerialPortScanner.SerialPortType.EcuWithOpenblt,
            SerialPortScanner.SerialPortType.OpenBlt
        ));

        final int availablePortForFirmwareUpdateWithBltCount = portsToUpdateFirmwareWithBlt.size();
        switch (availablePortForFirmwareUpdateWithBltCount) {
            case 0: {
                resetPortToUpdateFirmwareWithBlt("ECU not found");
                break;
            }
            case 1: {
                setPortToUpdateFirmwareWithBlt(portsToUpdateFirmwareWithBlt.get(0));
                break;
            }
            default: {
                resetPortToUpdateFirmwareWithBlt(String.format(
                    "Multiple ECUs found on: %s",
                    portsToUpdateFirmwareWithBlt.stream()
                        .map(portResult -> portResult.port)
                        .collect(Collectors.joining(", "))
                ));
                break;
            }
        }
    }

    private void setPortToUpdateFirmwareWithBlt(final SerialPortScanner.PortResult port) {
        portToUpdateFirmwareWithBlt = Optional.of(port);
        hideStatusMessage();
        if (requireBlt) {
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
    }

    private void resetPortToUpdateFirmwareWithBlt(final String reason) {
        portToUpdateFirmwareWithBlt = Optional.empty();
        if (requireBlt) {
            updateFirmwareButton.setEnabled(false);
            statusMessage.setText(reason);
        }
    }

    private void onUpdateFirmwareButtonClicked(final ActionEvent actionEvent) {
        if (requireBlt) {
            portToUpdateFirmwareWithBlt.ifPresentOrElse(port -> {
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

    private void runTool() {
    }
}
