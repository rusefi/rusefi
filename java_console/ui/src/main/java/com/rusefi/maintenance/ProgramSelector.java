package com.rusefi.maintenance;

import com.devexperts.logging.Logging;
import com.rusefi.config.generated.Integration;
import com.rusefi.core.FindFileHelper;
import com.rusefi.FileLog;
import com.rusefi.Launcher;
import com.rusefi.SerialPortScanner;
import com.rusefi.autodetect.PortDetector;
import com.rusefi.binaryprotocol.BinaryProtocol;
import com.rusefi.core.Pair;
import com.rusefi.io.LinkManager;
import com.rusefi.io.UpdateOperationCallbacks;
import com.rusefi.core.ui.AutoupdateUtil;
import com.rusefi.ui.util.URLLabel;
import org.jetbrains.annotations.NotNull;

import javax.swing.*;
import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;
import java.util.Objects;
import java.util.function.Consumer;
import java.util.stream.Collectors;

import static com.devexperts.logging.Logging.getLogging;
import static com.rusefi.core.ui.FrameHelper.appendBundleName;
import static com.rusefi.core.preferences.storage.PersistentConfiguration.getConfig;
import static com.rusefi.ui.util.UiUtils.trueLayout;

public class ProgramSelector {
    private static final Logging log = getLogging(ProgramSelector.class);

    // todo: migrate to enum?
    private static final String DFU_AUTO = "Auto DFU Update";
    private static final String DFU_MANUAL = "Manual DFU Update";
    private static final String DFU_SWITCH = "Switch to DFU Mode";
    private static final String DFU_ERASE = "Full DFU Erase";

    private static final String ST_LINK = "ST-LINK Update";

    private static final String OPENBLT_SWITCH = "Switch to OpenBLT Mode";
    public static final String OPENBLT_MANUAL = "Manual OpenBLT Update";
    public static final String OPENBLT_AUTO = "Auto OpenBLT Update";
    private static final String INSTALL_OPENBLT = "Install OpenBLT";
    private static final String OPENBLT_CAN = "OpenBLT via CAN";

    private static final String HELP = "https://github.com/rusefi/rusefi/wiki/HOWTO-Update-Firmware";

    private final JPanel content = new JPanel(new BorderLayout());
    private final JLabel noHardware = new JLabel("Nothing detected");
    private final JPanel updateModeAndButton = new JPanel(new FlowLayout());
    private final JComboBox<String> updateModeComboBox = new JComboBox<>();

    public ProgramSelector(JComboBox<SerialPortScanner.PortResult> comboPorts) {
        content.add(updateModeAndButton, BorderLayout.NORTH);
        content.add(noHardware, BorderLayout.SOUTH);

        String persistedMode = getConfig().getRoot().getProperty(getClass().getSimpleName());
        if (Arrays.asList(DFU_AUTO, DFU_MANUAL, OPENBLT_CAN, OPENBLT_SWITCH, OPENBLT_MANUAL, OPENBLT_AUTO, DFU_ERASE, DFU_SWITCH).contains(persistedMode))
            updateModeComboBox.setSelectedItem(persistedMode);

        JButton updateFirmwareButton = createUpdateFirmwareButton();

        updateModeAndButton.setVisible(false);
        updateModeAndButton.add(updateModeComboBox);
        updateModeAndButton.add(updateFirmwareButton);

        updateFirmwareButton.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                final String selectedMode = (String) updateModeComboBox.getSelectedItem();
                final SerialPortScanner.PortResult selectedPort = ((SerialPortScanner.PortResult) comboPorts.getSelectedItem());

                getConfig().getRoot().setProperty(getClass().getSimpleName(), selectedMode);
                executeJob(comboPorts, selectedMode, selectedPort);
            }
        });
    }

    public static void executeJob(JComponent parent, String selectedMode, SerialPortScanner.PortResult selectedPort) {
        log.info("ProgramSelector " + selectedMode + " " + selectedPort);
                String jobName = null;
                Consumer<UpdateOperationCallbacks> job;

                Objects.requireNonNull(selectedMode);
                switch (selectedMode) {
                    case DFU_AUTO:
                        jobName = "DFU update";
                        job = (callbacks) -> DfuFlasher.doAutoDfu(parent, selectedPort.port, callbacks);
                        break;
                    case DFU_MANUAL:
                      jobName = "DFU update";
                      job = DfuFlasher::runDfuProgramming;
                        break;
                    case INSTALL_OPENBLT:
                        jobName = "OpenBLT Initial Programming";
                        job = DfuFlasher::runOpenBltInitialProgramming;
                        break;
                    case ST_LINK:
                        job = updateOperationCallbacks -> {
                            // todo: add ST-LINK no-assert mode? or not?
                            StLinkFlasher.doUpdateFirmware(FindFileHelper.FIRMWARE_BIN_FILE, parent);
                        };
                        break;
                    case DFU_SWITCH:
                        jobName = "DFU switch";
                        job = (callbacks) -> rebootToDfu(parent, selectedPort.port, callbacks);
                        break;
                    case OPENBLT_SWITCH:
                        jobName = "OpenBLT switch";
                        job = (callbacks) -> rebootToOpenblt(parent, selectedPort.port, callbacks);
                        break;
                    case OPENBLT_CAN:
                        jobName = "OpenBLT via CAN";
                        job = (callbacks) -> flashOpenBltCan(parent, callbacks);
                        break;
                    case OPENBLT_MANUAL:
                        jobName = "OpenBLT via Serial";
                        job = (callbacks) -> flashOpenbltSerialJni(parent, selectedPort.port, callbacks);
                        break;
                    case OPENBLT_AUTO:
                        jobName = "OpenBLT via Serial";
                        job = (callbacks) -> flashOpenbltSerialAutomatic(parent, selectedPort.port, callbacks);
                        break;
                    case DFU_ERASE:
                        jobName = "DFU erase";
                        job = DfuFlasher::runDfuEraseAsync;
                        break;
                    default:
                        throw new IllegalArgumentException("How did you " + selectedMode);
                }

                final UpdateOperationCallbacks callbacks = new UpdateStatusWindow(appendBundleName(jobName + " " + Launcher.CONSOLE_VERSION));
                final Consumer<UpdateOperationCallbacks> job2 = job;
                ExecHelper.submitAction(() -> job2.accept(callbacks), "mx");
    }

    private static void rebootToDfu(JComponent parent, String selectedPort, UpdateOperationCallbacks callbacks) {
        String port = selectedPort == null ? PortDetector.AUTO : selectedPort;
        DfuFlasher.rebootToDfu(parent, port, callbacks, Integration.CMD_REBOOT_DFU);
    }

    private static void rebootToOpenblt(JComponent parent, String selectedPort, UpdateOperationCallbacks callbacks) {
        String port = selectedPort == null ? PortDetector.AUTO : selectedPort;
        DfuFlasher.rebootToDfu(parent, port, callbacks, Integration.CMD_REBOOT_OPENBLT);
    }

    private static void flashOpenBltCan(JComponent parent, UpdateOperationCallbacks callbacks) {
        if (FileLog.is32bitJava()) {
            showError32bitJava(parent);
            return;
        }
        OpenbltJni.OpenbltCallbacks cb = makeOpenbltCallbacks(callbacks);

        try {
            OpenbltJni.flashCan(FindFileHelper.findSrecFile(), cb);

            callbacks.logLine("Update completed successfully!");
            callbacks.done();
        } catch (Throwable e) {
            callbacks.logLine("Error: " + e);
            callbacks.error();
        } finally {
            OpenbltJni.stop(cb);
        }
    }

    private static Pair<Boolean, String[]> waitForEcuPortDisappeared(
        final String ecuPort,
        JComponent parent,
        final UpdateOperationCallbacks callbacks
    ) {
        callbacks.log("Waiting for ECU to reboot to OpenBlt...", false, true);

        String[] currentPorts = null;
        for (int attemptsCount = 0; attemptsCount < 150; attemptsCount++) {
            // Give the bootloader sec to enumerate
            BinaryProtocol.sleep(200);

            currentPorts = LinkManager.getCommPorts();
            log.info("currentPorts: [" + String.join(",", currentPorts) + "]");

            // Check that the ECU disappeared from the "after" list
            final boolean ecuPortStillAlive = !PortDetector.AUTO.equals(ecuPort) && Arrays.stream(currentPorts).anyMatch(ecuPort::equals);
            if (!ecuPortStillAlive) {
                return new Pair<>(true, currentPorts);
            } else {
                callbacks.log(".", false, false);
            }
        }
        callbacks.log("", true, false);
        return new Pair<>(false, currentPorts);
    }

    private static void flashOpenbltSerialAutomatic(JComponent parent, String ecuPort, UpdateOperationCallbacks callbacks) {
        final String[] portsBefore = LinkManager.getCommPorts();
        rebootToOpenblt(parent, ecuPort, callbacks);

        final Pair<Boolean, String[]> rebootResult = waitForEcuPortDisappeared(ecuPort, parent, callbacks);
        final boolean ecuPrtDisappeared = rebootResult.first;
        final String[] portsAfter = rebootResult.second;

        if (!ecuPrtDisappeared) {
            callbacks.logLine("Looks like your ECU still haven't rebooted to OpenBLT");
            callbacks.logLine("");
            callbacks.logLine("Try closing and opening console again");
            callbacks.logLine("");
            callbacks.error();
            return;
        }

        // Check that exactly one thing appeared in the "after" list
        ArrayList<String> newItems = new ArrayList<>();
        for (String s : portsAfter) {
            if (!Arrays.stream(portsBefore).anyMatch(s::equals)) {
                // This item is in the after list but not before list
                newItems.add(s);
            }
        }

        if (newItems.isEmpty()) {
            callbacks.logLine("Looks like your ECU disappeared during the update process. Please try again.");
            callbacks.error();
            return;
        }

        if (newItems.size() > 1) {
            // More than one port appeared? whattt?
            callbacks.logLine("Unable to find ECU after reboot as multiple serial ports appeared. Before: " + portsBefore.length + " After: " + portsAfter.length);
            callbacks.error();
            return;
        }

        String openbltPort = newItems.get(0);

        callbacks.logLine("Serial port " + openbltPort + " appeared, programming firmware...");

        flashOpenbltSerialJni(parent, openbltPort, callbacks);
    }

    private static OpenbltJni.OpenbltCallbacks makeOpenbltCallbacks(UpdateOperationCallbacks callbacks) {
        return new OpenbltJni.OpenbltCallbacks() {
            @Override
            public void log(String line) {
                callbacks.logLine(line);
            }

            @Override
            public void updateProgress(int percent) {
                callbacks.logLine("Progress: " + percent + "%");
            }

            @Override
            public void error(String line) {
                throw new RuntimeException(line);
            }

            @Override
            public void setPhase(String title, boolean hasProgress) {
                callbacks.logLine("Phase: " + title);
            }
        };
    }

    private static void showError32bitJava(JComponent parent) {
        JOptionPane.showMessageDialog(parent, "64 bit java required. 32 bit java not supported!",
            "Error", JOptionPane.ERROR_MESSAGE);
    }

    private static void flashOpenbltSerialJni(JComponent parent, String port, UpdateOperationCallbacks callbacks) {
        if (FileLog.is32bitJava()) {
            showError32bitJava(parent);
            return;
        }

        OpenbltJni.OpenbltCallbacks cb = makeOpenbltCallbacks(callbacks);

        try {
            String fileName = FindFileHelper.findSrecFile();
            callbacks.logLine("flashSerial " + fileName);
            OpenbltJni.flashSerial(fileName, port, cb);

            callbacks.logLine("Update completed successfully!");
            callbacks.done();
        } catch (Throwable e) {
            callbacks.logLine("Error: " + e.toString());
            callbacks.error();
        } finally {
            OpenbltJni.stop(cb);
        }
    }

    @NotNull
    public static JComponent createHelpButton() {
        return new URLLabel("HOWTO Update Firmware", HELP);
    }

    public JPanel getControl() {
        return content;
    }

    public void apply(SerialPortScanner.AvailableHardware currentHardware) {
        noHardware.setVisible(currentHardware.isEmpty());
        updateModeAndButton.setVisible(!currentHardware.isEmpty());

        boolean hasSerialPorts = !currentHardware.getKnownPorts().isEmpty();
        boolean hasDfuDevice = currentHardware.isDfuFound();

        updateModeComboBox.removeAllItems();
        if (FileLog.isWindows()) {
            boolean requireBlt = FindFileHelper.isObfuscated();
            if (hasSerialPorts && !requireBlt) {
                updateModeComboBox.addItem(DFU_AUTO);
            }

            if (hasDfuDevice && !requireBlt) {
                updateModeComboBox.addItem(DFU_MANUAL);
                updateModeComboBox.addItem(DFU_ERASE);
                if (DfuFlasher.haveBootloaderBinFile()) {
                    updateModeComboBox.addItem(INSTALL_OPENBLT);
                }
            }
            if (!requireBlt) {
                updateModeComboBox.addItem(DFU_SWITCH);
            }
            if (currentHardware.isStLinkConnected())
                updateModeComboBox.addItem(ST_LINK);
            if (currentHardware.isPCANConnected())
                updateModeComboBox.addItem(OPENBLT_CAN);
            // todo: detect PCAN mode.addItem(OPENBLT_CAN);
        }

        if (hasSerialPorts) {
            updateModeComboBox.addItem(OPENBLT_AUTO);
            updateModeComboBox.addItem(OPENBLT_SWITCH);
            updateModeComboBox.addItem(OPENBLT_MANUAL);

            List<SerialPortScanner.PortResult> listOfBootloaders = currentHardware.getKnownPorts().stream().filter(portResult -> portResult.type == SerialPortScanner.SerialPortType.OpenBlt).collect(Collectors.toList());
            if (!listOfBootloaders.isEmpty()) {
                updateModeComboBox.setSelectedItem(OPENBLT_MANUAL);
            }
        }

        trueLayout(updateModeComboBox);
        trueLayout(content);
    }

  @NotNull
  public static JButton createUpdateFirmwareButton() {
    return new JButton("Update Firmware", AutoupdateUtil.loadIcon("upload48.png"));
  }
}
