package com.rusefi.ui.basic;

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

import static com.rusefi.FileLog.isWindows;

/**
 * Focuses on firmware updater
 * Much simpler than {@link com.rusefi.StartupFrame}
 */
public class BasicStartupFrame {
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

    private void onHardwareUpdated(final SerialPortScanner.AvailableHardware currentHardware) {
        status.stop();
        frame.getFrame().pack();

        final List<SerialPortScanner.PortResult> ecuPorts = currentHardware.getKnownPorts(SerialPortScanner.SerialPortType.EcuWithOpenblt);
        final List<SerialPortScanner.PortResult> bootloaderPorts = currentHardware.getKnownPorts(SerialPortScanner.SerialPortType.OpenBlt);


        if (!ecuPorts.isEmpty()) {
            portToUpdateFirmware = Optional.of(ecuPorts.get(0));
            noPortsMessage.setVisible(false);
            update.setEnabled(true);
            update.setText("Auto Update Firmware");
        } else if (!bootloaderPorts.isEmpty()) {
            portToUpdateFirmware = Optional.of(bootloaderPorts.get(0));
            noPortsMessage.setVisible(false);
            update.setEnabled(true);
            update.setText("Blt Update Firmware");
        } else {
            noPortsMessage.setText("ECU not found");
        }
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
                        // TODO: notify user about strange situation?
                        break;
                    }
                }
            }, ()-> {
                // TODO: notify user about strange situation?
            }
        );
    }

    private void runTool() {
    }
}
