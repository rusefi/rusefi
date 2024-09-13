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
import java.util.List;
import java.util.Optional;
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

    private final JLabel statusMessage = new JLabel();
    private final StatusAnimation status = new StatusAnimation(this::updateStatus, StartupFrame.SCANNING_PORTS);
    private final JButton updateFirmwareButton = ProgramSelector.createUpdateFirmwareButton();

    private volatile Optional<SerialPortScanner.PortResult> portToUpdateFirmware = Optional.empty();

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

            boolean requireBlt = FindFileHelper.isObfuscated();
            if (requireBlt) {
                updateFirmwareButton.addActionListener(e-> onUpdateFirmwareButtonClicked());
                updateFirmwareButton.setEnabled(false);

                statusMessage.setForeground(Color.red);
                panel.add(statusMessage);

                SerialPortScanner.INSTANCE.addListener(currentHardware -> SwingUtilities.invokeLater(() -> {
                    onHardwareUpdated(currentHardware);
                }));
            } else {
                updateFirmwareButton.addActionListener(e -> DfuFlasher.doAutoDfu(updateFirmwareButton, PortDetector.AUTO, new UpdateStatusWindow("Update")));
            }
            panel.add(updateFirmwareButton);
        } else {
            panel.add(new JLabel("Sorry only works on Windows"));
        }

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
        // I'm not sure hat the following frame packing is really necessary, but I'm adding it just in case if frame was
        // not packed in updateStatus method
        packFrame();

        final List<SerialPortScanner.PortResult> ecuPorts = currentHardware.getKnownPorts(SerialPortScanner.SerialPortType.EcuWithOpenblt);
        final List<SerialPortScanner.PortResult> bootloaderPorts = currentHardware.getKnownPorts(SerialPortScanner.SerialPortType.OpenBlt);

        final int availablePortCount = ecuPorts.size() + bootloaderPorts.size();
        switch (availablePortCount) {
            case 0: {
                resetPort("ECU not found");
                break;
            }
            case 1: {
                if (!ecuPorts.isEmpty()) {
                    switchToPort(ecuPorts.get(0), "Auto Update Firmware");
                } else if (!bootloaderPorts.isEmpty()) {
                    switchToPort(bootloaderPorts.get(0), "Blt Update Firmware");
                } else {
                    log.error("Do nothing.");
                }
                break;
            }
            default: {
                resetPort(String.format(
                    "Multiple ECUs found on: %s",
                    Stream.concat(ecuPorts.stream(), bootloaderPorts.stream())
                        .map(portResult -> portResult.port)
                        .collect(Collectors.joining(", "))
                ));
                break;

            }
        }
    }

    private void switchToPort(final SerialPortScanner.PortResult port, final String updateButtonText) {
        portToUpdateFirmware = Optional.of(port);
        hideStatusMessage();
        updateFirmwareButton.setEnabled(true);
        updateFirmwareButton.setText(updateButtonText);
    }

    private void resetPort(final String reason) {
        portToUpdateFirmware = Optional.empty();
        updateFirmwareButton.setEnabled(false);
        statusMessage.setText(reason);
    }

    private void onUpdateFirmwareButtonClicked() {
        portToUpdateFirmware.ifPresentOrElse(port -> {
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
            }, ()-> {
                log.error("Port to update firmware is not defined.");
            }
        );
    }

    private void runTool() {
    }
}
