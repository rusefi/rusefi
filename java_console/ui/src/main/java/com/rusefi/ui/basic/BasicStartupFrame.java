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

    private final FrameHelper frame;

    private final JLabel noPortsMessage = new JLabel();
    private final StatusAnimation status = new StatusAnimation(noPortsMessage::setText, StartupFrame.SCANNING_PORTS);
    private final JButton update = ProgramSelector.createUpdateFirmwareButton();

    private volatile Optional<SerialPortScanner.PortResult> portToUpdateFirmware = Optional.empty();

    public static void main(String[] args) {
        runTool(null);
    }

    public static void runTool(String[] args) {
        DefaultExceptionHandler.install();
        new BasicStartupFrame().runTool();
    }

    public BasicStartupFrame() {
        String whiteLabel = ConnectionAndMeta.getWhiteLabel(ConnectionAndMeta.getProperties());
        String title = whiteLabel + " basic console " + Launcher.CONSOLE_VERSION;
        frame = FrameHelper.createFrame(title);
        JPanel panel = new JPanel(new VerticalFlowLayout());
        if (isWindows()) {
            panel.add(ToolButtons.createShowDeviceManagerButton());
            panel.add(StartupFrame.binaryModificationControl());

            boolean requireBlt = FindFileHelper.isObfuscated();
            if (requireBlt) {
                update.addActionListener(e-> onUpdateButtonClicked());
                update.setEnabled(false);

                noPortsMessage.setForeground(Color.red);
                panel.add(noPortsMessage);

                SerialPortScanner.INSTANCE.addListener(currentHardware -> SwingUtilities.invokeLater(() -> {
                    onHardwareUpdated(currentHardware);
                }));
            } else {
                update.addActionListener(e -> DfuFlasher.doAutoDfu(update, PortDetector.AUTO, new UpdateStatusWindow("Update")));
            }
            panel.add(update);
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
    }

    private void onHardwareUpdated(final AvailableHardware currentHardware) {
        status.stop();
        frame.getFrame().pack();

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
        noPortsMessage.setVisible(false);
        update.setEnabled(true);
        update.setText(updateButtonText);
    }

    private void resetPort(final String reason) {
        portToUpdateFirmware = Optional.empty();
        update.setEnabled(false);
        noPortsMessage.setText(reason);
        noPortsMessage.setVisible(true);
    }

    private void onUpdateButtonClicked() {
        portToUpdateFirmware.ifPresentOrElse(port -> {
                switch (port.type) {
                    case EcuWithOpenblt: {
                        ProgramSelector.executeJob(update, ProgramSelector.OPENBLT_AUTO, port);
                        break;
                    }
                    case OpenBlt: {
                        ProgramSelector.executeJob(update, ProgramSelector.OPENBLT_MANUAL, port);
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
