package com.rusefi.maintenance;

import com.devexperts.logging.Logging;
import com.rusefi.*;
import com.rusefi.config.generated.Integration;
import com.rusefi.core.FindFileHelper;
import com.rusefi.autodetect.PortDetector;
import com.rusefi.io.UpdateOperationCallbacks;
import com.rusefi.core.ui.AutoupdateUtil;
import com.rusefi.maintenance.jobs.*;
import com.rusefi.ui.util.URLLabel;
import org.jetbrains.annotations.NotNull;

import javax.swing.*;
import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.util.ArrayList;
import java.util.List;
import java.util.Objects;
import java.util.stream.Collectors;

import static com.devexperts.logging.Logging.getLogging;
import static com.rusefi.SerialPortType.OpenBlt;
import static com.rusefi.core.preferences.storage.PersistentConfiguration.getConfig;
import static com.rusefi.maintenance.CalibrationsHelper.*;
import static com.rusefi.maintenance.CallbacksWaitingUtil.TOTAL_WAIT_SECONDS;
import static com.rusefi.maintenance.CallbacksWaitingUtil.waitForPredicate;
import static com.rusefi.maintenance.UpdateMode.*;
import static java.lang.Boolean.parseBoolean;

public class ProgramSelector {
    private static final Logging log = getLogging(ProgramSelector.class);
    private final JPanel content = new JPanel(new BorderLayout());
    private final JLabel noHardware = new JLabel("Nothing detected");
    private final JPanel updateModeAndButton = new JPanel(new FlowLayout());
    private final JComboBox<UpdateMode> updateModeComboBox = new JComboBox<>();
    private final ConnectivityContext connectivityContext;

    public ProgramSelector(ConnectivityContext connectivityContext, JComboBox<PortResult> comboPorts) {
        this.connectivityContext = connectivityContext;
        content.add(updateModeAndButton, BorderLayout.NORTH);
        content.add(noHardware, BorderLayout.SOUTH);

        String persistedMode = getConfig().getRoot().getProperty(getClass().getSimpleName());

        parseDisplayText(persistedMode).ifPresent(updateModeComboBox::setSelectedItem);

        JButton updateFirmwareButton = createUpdateFirmwareButton();

        updateModeAndButton.setVisible(false);
        updateModeAndButton.add(updateModeComboBox);
        updateModeAndButton.add(updateFirmwareButton);

        updateFirmwareButton.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                final UpdateMode selectedMode = (UpdateMode) updateModeComboBox.getSelectedItem();
                final PortResult selectedPort = ((PortResult) comboPorts.getSelectedItem());

                final String persistedValue = (selectedMode != null ? selectedMode.displayText : null);
                getConfig().getRoot().setProperty(getClass().getSimpleName(), persistedValue);
                executeJob(comboPorts, selectedMode, selectedPort);
            }
        });
    }

    private void executeJob(JComponent parent, UpdateMode selectedMode, PortResult selectedPort) {
        log.info("ProgramSelector " + selectedMode + " " + selectedPort);
        Objects.requireNonNull(selectedMode);
        AsyncJob job;
        switch (selectedMode) {
            case DFU_AUTO:
                job = new DfuAutoJob(selectedPort, parent, connectivityContext);
                break;
            case DFU_MANUAL:
                job = new DfuManualJob();
                break;
            case INSTALL_OPENBLT:
                job = new InstallOpenBltJob();
                break;
            case ST_LINK:
                job = new StLinkJob(parent);
                break;
            case DFU_SWITCH:
                job = new DfuSwitchJob(selectedPort, parent);
                break;
            case OPENBLT_SWITCH:
                job = new OpenBltSwitchJob(selectedPort, parent);
                break;
            case OPENBLT_CAN:
                job = new OpenBltCanJob(parent);
                break;
            case OPENBLT_MANUAL:
                job = new OpenBltManualJob(selectedPort, parent);
                break;
            case OPENBLT_AUTO:
                job = new OpenBltAutoJob(selectedPort, parent, connectivityContext);
                break;
            case DFU_ERASE:
                job = new DfuEraseJob();
                break;
            default:
                throw new IllegalArgumentException("How did you " + selectedMode);
        }

        AsyncJobExecutor.INSTANCE.executeJobWithStatusWindow(job);
    }

    public static void rebootToDfu(JComponent parent, String selectedPort, UpdateOperationCallbacks callbacks) {
        String port = selectedPort == null ? PortDetector.AUTO : selectedPort;
        DfuFlasher.rebootToDfu(parent, port, callbacks, Integration.CMD_REBOOT_DFU);
    }

    public static void rebootToOpenblt(JComponent parent, String selectedPort, UpdateOperationCallbacks callbacks) {
        String port = selectedPort == null ? PortDetector.AUTO : selectedPort;
        DfuFlasher.rebootToDfu(parent, port, callbacks, Integration.CMD_REBOOT_OPENBLT);
    }

    public static void flashOpenBltCan(JComponent parent, UpdateOperationCallbacks callbacks) {
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
            callbacks.logLine("OpenbltJni Error: " + e);
            callbacks.error();
        } finally {
            OpenbltJni.stop(cb);
        }
    }

    private static boolean waitForEcuPortDisappeared(
        final PortResult ecuPort,
        final UpdateOperationCallbacks callbacks, ConnectivityContext connectivityContext
    ) {
        return waitForPredicate(
            String.format("Waiting for ECU on port %s to reboot to OpenBlt for up to " + TOTAL_WAIT_SECONDS + " seconds...", ecuPort),
            () -> {
                final AvailableHardware availableHardware = connectivityContext.getCurrentHardware();
                log.info(String.format(
                    "current ports: [%s]",
                    availableHardware.getKnownPorts().stream()
                        .map(PortResult::toString)
                        .collect(Collectors.joining(","))
                ));
                return !availableHardware.isPortAvailable(ecuPort);
            },
            callbacks
        );
    }

    private static List<PortResult> waitForNewOpenBltPortAppeared(
        final List<PortResult> openBltPortsBefore,
        final UpdateOperationCallbacks callbacks, ConnectivityContext connectivityContext
    ) {
        final List<PortResult> newPorts = new ArrayList<>();
        waitForPredicate(
            "Waiting for new OpenBlt port to appear...",
            () -> {
                final AvailableHardware availableHardwareAfter = connectivityContext.getCurrentHardware();
                log.info(String.format(
                    "ports after reboot to OpenBlt: [%s]",
                    availableHardwareAfter.getKnownPorts().stream()
                        .map(PortResult::toString)
                        .collect(Collectors.joining(","))
                ));
                for (final PortResult p : availableHardwareAfter.getKnownPorts(OpenBlt)) {
                    if (!openBltPortsBefore.contains(p)) {
                        // This item is in the after list but not before list
                        newPorts.add(p);
                    }
                }
                return !newPorts.isEmpty();
            },
            callbacks
        );
        return newPorts;
    }

    public static boolean flashOpenbltSerialAutomatic(
        JComponent parent,
        PortResult ecuPort,
        UpdateOperationCallbacks callbacks, ConnectivityContext connectivityContext
    ) {
        return updateFirmwareAndRestorePreviousCalibrations(
            ecuPort,
            callbacks,
            () -> bltUpdateFirmware(parent, ecuPort, callbacks, connectivityContext), connectivityContext
        );
    }

    private static boolean bltUpdateFirmware(JComponent parent, PortResult ecuPort, UpdateOperationCallbacks callbacks, ConnectivityContext connectivityContext) {
        final List<PortResult> openBltPortsBefore = connectivityContext.getCurrentHardware().getKnownPorts(OpenBlt);

        rebootToOpenblt(parent, ecuPort.port, callbacks);

        // invoking blocking method
        final boolean isEcuPortDisappeared = waitForEcuPortDisappeared(ecuPort, callbacks, connectivityContext);

        if (!isEcuPortDisappeared) {
            callbacks.logLine("Looks like your ECU still haven't rebooted to OpenBLT");
            callbacks.logLine("");
            callbacks.logLine("Try closing and opening console again");
            callbacks.logLine("");
            return false;
        }

        final List<PortResult> newItems = waitForNewOpenBltPortAppeared(openBltPortsBefore, callbacks, connectivityContext);

        // Check that exactly one thing appeared in the "after" list
        if (newItems.isEmpty()) {
            callbacks.logLine("Looks like your ECU disappeared during the update process. Please try again.");
            return false;
        }

        if (newItems.size() > 1) {
            // More than one port appeared? whattt?
            callbacks.logLine(
                "Unable to find ECU after reboot as multiple serial ports appeared. Before: "
                    + openBltPortsBefore.size()
            );
            return false;
        }

        final String openbltPort = newItems.get(0).port;

        callbacks.logLine("Serial port " + openbltPort + " appeared, programming firmware...");

        return flashOpenbltSerial(parent, openbltPort, callbacks);
    }

    private static OpenbltJni.OpenbltCallbacks makeOpenbltCallbacks(UpdateOperationCallbacks callbacks) {
        return new OpenbltJni.OpenbltCallbacks() {
            @Override
            public void log(String line) {
                callbacks.logLine(line);
            }

            @Override
            public void updateProgress(int percent) {
                callbacks.updateProgress(percent);
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

    public static boolean flashOpenbltSerial(JComponent parent, String port, UpdateOperationCallbacks callbacks) {
        if (FileLog.is32bitJava()) {
            showError32bitJava(parent);
            return false;
        }

        OpenbltJni.OpenbltCallbacks cb = makeOpenbltCallbacks(callbacks);

        String fileName = FindFileHelper.findSrecFile();
        if (fileName == null) {
            callbacks.logLine(".srec image file not found");
            return false;
        }
        try {
            callbacks.logLine("flashSerial " + fileName);
            OpenBltFlasher.flashSerial(fileName, port, cb);

            callbacks.logLine("Update completed successfully!");
            return true;
        } catch (Throwable e) {
            callbacks.logLine("flashOpenbltSerial Error: " + e);
            log.error("flashOpenbltSerial " + e, e);
            return false;
        }
    }

    @NotNull
    public static JComponent createHelpButton() {
        return new URLLabel("HOWTO Update Firmware", UiProperties.getUpdateHelpUrl());
    }

    public JPanel getControl() {
        return content;
    }

    public void apply(AvailableHardware currentHardware) {
        noHardware.setVisible(currentHardware.isEmpty());
        updateModeAndButton.setVisible(!currentHardware.isEmpty());

        boolean hasSerialPorts = !currentHardware.getKnownPorts().isEmpty();
        boolean hasDfuDevice = currentHardware.isDfuFound();

        Object updateModeToRestore = updateModeComboBox.getSelectedItem();
        updateModeComboBox.removeAllItems();
        if (FileLog.isWindows()) {
            boolean requireBlt = FindFileHelper.isObfuscated();

            if (!requireBlt) {
                if (hasSerialPorts) {
                    updateModeComboBox.addItem(DFU_AUTO);
                }
                if (hasDfuDevice) {
                    updateModeComboBox.addItem(DFU_MANUAL);
                    updateModeComboBox.addItem(DFU_ERASE);
                    if (DfuFlasher.haveBootloaderBinFile()) {
                        updateModeComboBox.addItem(INSTALL_OPENBLT);
                    }
                }
                updateModeComboBox.addItem(DFU_SWITCH);
                if (currentHardware.isStLinkConnected())
                    updateModeComboBox.addItem(ST_LINK);
            }
            if (currentHardware.isPCANConnected())
                updateModeComboBox.addItem(OPENBLT_CAN);
            // todo: detect PCAN mode.addItem(OPENBLT_CAN);
        }

        if (hasSerialPorts) {
            updateModeComboBox.addItem(OPENBLT_AUTO);
            updateModeComboBox.addItem(OPENBLT_SWITCH);
            updateModeComboBox.addItem(OPENBLT_MANUAL);

            List<PortResult> listOfBootloaders = currentHardware.getKnownPorts().stream().filter(portResult -> portResult.type == OpenBlt).collect(Collectors.toList());
            if (!listOfBootloaders.isEmpty()) {
                updateModeToRestore = OPENBLT_MANUAL;
            }
        }

        if (updateModeToRestore != null) {
            updateModeComboBox.setSelectedItem(updateModeToRestore);
        }

        AutoupdateUtil.trueLayoutAndRepaint(updateModeComboBox);
        AutoupdateUtil.trueLayoutAndRepaint(content);
    }

    @NotNull
    public static JButton createUpdateFirmwareButton() {
        return new JButton("Update Firmware", AutoupdateUtil.loadIcon("upload48.png"));
    }

    public void setMode(UpdateMode updateMode) {
        updateModeComboBox.setSelectedItem(updateMode);
    }
}
