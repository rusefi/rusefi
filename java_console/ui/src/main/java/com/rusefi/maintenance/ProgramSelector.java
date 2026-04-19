package com.rusefi.maintenance;

import com.devexperts.logging.Logging;
import com.fazecast.jSerialComm.SerialPort;
import com.rusefi.*;
import com.rusefi.binaryprotocol.BinaryProtocol;
import com.rusefi.config.generated.Integration;
import com.rusefi.core.FindFileHelper;
import com.rusefi.autodetect.PortDetector;
import com.rusefi.io.BootloaderHelper;
import com.rusefi.io.UpdateOperationCallbacks;
import com.rusefi.core.ui.AutoupdateUtil;
import com.rusefi.io.IoStream;
import com.rusefi.io.serial.BufferedSerialIoStream;
import com.rusefi.maintenance.jobs.*;
import com.rusefi.ui.basic.SingleAsyncJobExecutor;
import com.rusefi.ui.util.URLLabel;
import com.rusefi.updater.OpenbltDetectorStrategy;
import org.jetbrains.annotations.NotNull;
import org.jetbrains.annotations.Nullable;

import javax.swing.*;
import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.util.ArrayList;
import java.util.List;
import java.util.Objects;
import java.io.EOFException;
import java.util.concurrent.TimeUnit;
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
    @Nullable
    private SingleAsyncJobExecutor jobExecutor;

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

        if (jobExecutor != null) {
            jobExecutor.startJob(job, parent);
        } else {
            AsyncJobExecutor.INSTANCE.executeJobWithStatusWindow(job);
        }
    }

    public void setJobExecutor(@Nullable SingleAsyncJobExecutor jobExecutor) {
        this.jobExecutor = jobExecutor;
    }

    public static void rebootToDfu(JComponent parent, String selectedPort, UpdateOperationCallbacks callbacks) {
        String port = selectedPort == null ? PortDetector.AUTO : selectedPort;
        DfuFlasher.rebootToDfu(parent, port, callbacks, Integration.CMD_REBOOT_DFU);
    }

    public static void rebootToOpenblt(JComponent parent, String selectedPort, UpdateOperationCallbacks callbacks) {
        String port = selectedPort == null ? PortDetector.AUTO : selectedPort;
        DfuFlasher.rebootToDfu(parent, port, callbacks, Integration.CMD_REBOOT_OPENBLT);
    }

    /**
     * Send reboot-to-OpenBLT via an already-open BinaryProtocol connection.
     * Use this from ConsoleUI context where the port is held by LinkManager.
     */
    public static void rebootToOpenblt(JComponent parent, BinaryProtocol binaryProtocol, UpdateOperationCallbacks callbacks) {
        BootloaderHelper.sendBootloaderRebootCommand(parent, binaryProtocol.signature, binaryProtocol.getStream(), callbacks, Integration.CMD_REBOOT_OPENBLT);
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
        final UpdateOperationCallbacks callbacks
    ) {
        // Scanner is already suspended by the caller (bltUpdateFirmware).
        return waitForPredicate(
            String.format("Waiting for ECU on port %s to reboot to OpenBlt for up to " + TOTAL_WAIT_SECONDS + " seconds...", ecuPort),
            () -> {
                // Directly probe the port rather than relying on the scanner snapshot.
                // The scanner cache would keep reporting EcuWithOpenblt even after the
                // ECU has already entered OpenBLT mode (the OS port does not disappear).
                try (IoStream stream = BufferedSerialIoStream.openPort(ecuPort.port)) {
                    if (stream == null) {
                        log.info("Port " + ecuPort.port + " is unavailable — ECU is rebooting");
                        return true;
                    }
                    if (OpenbltDetectorStrategy.isPortOpenblt(stream)) {
                        log.info("Port " + ecuPort.port + " is now in OpenBLT mode");
                        return true;
                    }
                } catch (EOFException e) {
                    // readByte timed out — keep waiting until a write error or
                    // null stream indicates the ECU has actually reset.
                    log.info("Port " + ecuPort.port + " still responding as ECU firmware (XCP ignored)");
                } catch (Exception e) {
                    log.info("Port " + ecuPort.port + " probe error (ECU transitioning): " + e.getMessage());
                    return true;
                }
                log.info("Port " + ecuPort.port + " still responding as ECU firmware");
                return false;
            },
            callbacks
        );
    }

    private static List<PortResult> waitForNewOpenBltPortAppeared(
        final List<PortResult> openBltPortsBefore,
        final UpdateOperationCallbacks callbacks
    ) {
        // Scanner is suspended by the caller (bltUpdateFirmware). Scan all system
        // COM ports ourselves so we can find OpenBLT even when it enumerates on a
        // different port number than the original ECU port (common with USB-CDC where
        // the bootloader has a different USB PID and gets a new COM assignment).
        final List<PortResult> newPorts = new ArrayList<>();
        waitForPredicate(
            "Waiting for new OpenBlt port to appear...",
            () -> {
                for (SerialPort sp : SerialPort.getCommPorts()) {
                    final String portName = sp.getSystemPortName();
                    if (openBltPortsBefore.stream().anyMatch(p -> p.port.equals(portName))) {
                        continue; // skip ports that were already in OpenBLT mode before
                    }
                    try (IoStream stream = BufferedSerialIoStream.openPort(portName)) {
                        if (OpenbltDetectorStrategy.isPortOpenblt(stream)) {
                            log.info("Direct probe: port " + portName + " is in OpenBLT mode");
                            newPorts.add(new PortResult(portName, OpenBlt));
                            return true;
                        }
                    } catch (Exception e) {
                        log.info("Probe of " + portName + " error: " + e.getMessage());
                    }
                }
                return false;
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
        // Suspend the scanner for the entire reboot → detect → flash sequence so it
        // never races with our direct port probes for exclusive COM port access on Windows.
        try {
            connectivityContext.getSerialPortScanner().suspend().await(30, TimeUnit.SECONDS);
        } catch (InterruptedException e) {
            Thread.currentThread().interrupt();
        }
        try {
            return bltUpdateFirmwareWithSuspendedScanner(parent, ecuPort, callbacks, connectivityContext);
        } finally {
            // Invalidate the cache so the scanner re-inspects the port (now running new
            // firmware) on its first post-resume scan cycle.
            connectivityContext.getSerialPortScanner().invalidatePort(ecuPort.port);
            connectivityContext.getSerialPortScanner().resume();
        }
    }

    private static boolean bltUpdateFirmwareWithSuspendedScanner(JComponent parent, PortResult ecuPort, UpdateOperationCallbacks callbacks, ConnectivityContext connectivityContext) {
        // Snapshot pre-existing OpenBLT ports so we can ignore them when searching
        // for the newly-appeared bootloader port after the reboot.
        final List<PortResult> openBltPortsBefore = connectivityContext.getCurrentHardware().getKnownPorts(OpenBlt);

        rebootToOpenblt(parent, ecuPort.port, callbacks);

        // invoking blocking method
        final boolean isEcuPortDisappeared = waitForEcuPortDisappeared(ecuPort, callbacks);

        if (!isEcuPortDisappeared) {
            callbacks.logLine("Looks like your ECU still haven't rebooted to OpenBLT");
            callbacks.logLine("");
            callbacks.logLine("Try closing and opening console again");
            callbacks.logLine("");
            return false;
        }

        final List<PortResult> newItems = waitForNewOpenBltPortAppeared(openBltPortsBefore, callbacks);

        // Check that exactly one thing appeared in the "after" list
        if (newItems.isEmpty()) {
            callbacks.logLine("Looks like your ECU disappeared during the update process. Please try again.");
            return false;
        }

        if (newItems.size() > 1) {
            // More than one port appeared? whattt?
            callbacks.logLine("Unable to find ECU after reboot as multiple serial ports appeared.");
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

        // When a DFU device is present, prefer DFU_MANUAL over DFU_SWITCH (which is the
        // default fallback shown before any device is detected). This handles both the first
        // detection and the case where hasDfuDevice briefly flipped to false then back to true,
        // leaving DFU_SWITCH restored instead of DFU_MANUAL. see #9157
        if (hasDfuDevice && updateModeComboBox.getSelectedItem() == DFU_SWITCH) {
            updateModeComboBox.setSelectedItem(DFU_MANUAL);
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
