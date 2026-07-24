package com.rusefi.maintenance;

import com.devexperts.logging.Logging;
import com.fazecast.jSerialComm.SerialPort;
import com.rusefi.*;
import com.rusefi.binaryprotocol.BinaryProtocol;
import com.rusefi.config.generated.Integration;
import com.rusefi.core.FindFileHelper;
import com.rusefi.autodetect.PortDetector;
import com.rusefi.io.LinkManager;
import com.rusefi.io.UpdateOperationCallbacks;
import com.rusefi.core.ui.AutoupdateUtil;
import com.rusefi.io.IoStream;
import com.rusefi.io.serial.BufferedSerialIoStream;
import com.rusefi.maintenance.jobs.*;
import com.rusefi.ui.basic.SingleAsyncJobExecutor;
import com.rusefi.ui.util.URLLabel;
import com.rusefi.ui.widgets.JSplitButton;
import com.rusefi.updater.OpenbltDetectorStrategy;
import org.jetbrains.annotations.NotNull;
import org.jetbrains.annotations.Nullable;

import javax.swing.*;
import java.awt.*;
import java.awt.event.ItemEvent;
import java.util.ArrayList;
import java.util.List;
import java.util.Objects;
import java.io.EOFException;
import java.util.concurrent.TimeUnit;
import java.util.function.BooleanSupplier;
import java.util.function.Function;

import static com.devexperts.logging.Logging.getLogging;
import static com.rusefi.SerialPortType.OpenBlt;
import static com.rusefi.maintenance.CalibrationsHelper.*;
import static com.rusefi.maintenance.CallbacksWaitingUtil.TOTAL_WAIT_SECONDS;
import static com.rusefi.maintenance.CallbacksWaitingUtil.waitForPredicate;
import static com.rusefi.maintenance.UpdateMode.*;

public class ProgramSelector {
    private static final Logging log = getLogging(ProgramSelector.class);
    private final JPanel content = new JPanel(new BorderLayout());
    private final JLabel noHardware = new JLabel("Nothing detected");
    private final JPanel updateModeAndButton = new JPanel(new FlowLayout(FlowLayout.CENTER, 32, 5));
    private final JSplitButton splitButton = new JSplitButton("Update ECU Firmware", AutoupdateUtil.loadIcon("upload48.png"));
    private final List<JComponent> additionalFirmwareControls = new ArrayList<>();
    private final ConnectivityContext connectivityContext;
    private final JComboBox<PortResult> comboPorts;
    @Nullable
    private SingleAsyncJobExecutor jobExecutor;
    @Nullable
    private LinkManager linkManager;
    private Function<JComponent, Boolean> firmwareUpdateInterceptor = source -> false;
    private BooleanSupplier externalBusy = () -> false;

    public ProgramSelector(ConnectivityContext connectivityContext, JComboBox<PortResult> comboPorts) {
        this.connectivityContext = connectivityContext;
        this.comboPorts = comboPorts;
        noHardware.setBorder(BorderFactory.createEmptyBorder(5, 8, 5, 8));
        noHardware.setFont(noHardware.getFont().deriveFont(noHardware.getFont().getSize2D() + 2));
        content.add(updateModeAndButton, BorderLayout.NORTH);
        content.add(noHardware, BorderLayout.SOUTH);

        updateModeAndButton.setVisible(false);
        updateModeAndButton.add(splitButton);

        splitButton.addActionListener(e -> {
            if (firmwareUpdateInterceptor.apply(splitButton)) {
                return;
            }
            final PortResult targetPort = resolveFlashPort();
            executeJob(splitButton, mainButtonModeFor(targetPort), targetPort);
        });

        // Keep the main button label in sync with whatever port is currently selected. The combo is
        // shared with the Connect tab and is repopulated/re-selected after apply() runs, so drive the
        // text off selection changes (same source the action reads) rather than off apply()'s snapshot.
        comboPorts.addItemListener(e -> {
            if (e.getStateChange() == ItemEvent.SELECTED) {
                refreshMainButtonText();
            }
        });
        refreshMainButtonText();
    }

    /**
     * A board already sitting in the OpenBLT bootloader has no running firmware: it never auto-connects,
     * so {@link #linkManager} is never set for it and {@link UpdateMode#OPENBLT_AUTO} (which reboots a
     * live ECU into the bootloader) cannot work. Flash it directly via {@link UpdateMode#OPENBLT_MANUAL}.
     */
    private UpdateMode mainButtonModeFor(@Nullable PortResult selectedPort) {
        return mainButtonModeFor(selectedPort, isLiveConnection());
    }

    /**
     * Pure decision logic behind the main Update-Firmware button mode, extracted so it can be unit tested
     * without Swing/hardware-detection singletons. [tag:better_ux_for_flashing]
     */
    static UpdateMode mainButtonModeFor(@Nullable PortResult selectedPort, boolean liveConnection) {
        if (selectedPort != null && selectedPort.type == OpenBlt) {
            return OPENBLT_MANUAL;
        }
        // A DFU device is a board already sitting in the STM32 built-in bootloader: it has no running
        // firmware to reboot and never auto-connects, so flash it directly via DFU_MANUAL rather than
        // mislabeling the button as a (dead) OpenBLT action. [tag:better_ux_for_flashing]
        if (selectedPort != null && selectedPort.type == SerialPortType.Dfu) {
            return DFU_MANUAL;
        }
        // OPENBLT_AUTO reboots a *live* ECU into the bootloader; with no live connection there is nothing
        // to reboot and awaitBinaryProtocol would just time out. So when offline — including a board
        // sitting in a bootloader whose detection is momentarily flickering to Unknown — flash manually
        // rather than flip-flopping into a dead AUTO job. [tag:better_ux_for_flashing]
        if (!liveConnection) {
            return OPENBLT_MANUAL;
        }
        return OPENBLT_AUTO;
    }

    private void refreshMainButtonText() {
        final UpdateMode mode = mainButtonModeFor(resolveFlashPort());
        if (mode == OPENBLT_MANUAL) {
            splitButton.setText(OPENBLT_MANUAL.displayText);
        } else if (mode == DFU_MANUAL) {
            splitButton.setText(DFU_MANUAL.displayText);
        } else {
            splitButton.setText("Update Firmware");
        }
        matchFirmwareControlSizes();
    }

    private boolean isLiveConnection() {
        return linkManager != null && linkManager.getBinaryProtocol() != null;
    }

    /**
     * Resolve which port the main Update-Firmware action targets. A board already in the OpenBLT
     * bootloader is flashed manually and needs no live connection, so — when the combo selection isn't
     * itself a bootloader and we're offline — prefer any detected OpenBLT port over the connection-
     * dependent AUTO path (which reboots a *running* ECU and just times out with no live BinaryProtocol).
     * This covers the offline "open a tune, plug an OpenBLT board" flow where the combo selection may be
     * null/stale right after the hot-plug. [tag:better_ux_for_flashing]
     */
    private PortResult resolveFlashPort() {
        final PortResult selected = (PortResult) comboPorts.getSelectedItem();
        return resolveFlashPort(
            selected,
            isLiveConnection(),
            connectivityContext.getCurrentHardware().getKnownPorts(SerialPortType.Dfu),
            connectivityContext.getCurrentHardware().getKnownPorts(OpenBlt)
        );
    }

    /**
     * Pure port-resolution logic behind the main Update-Firmware action, extracted so it can be unit
     * tested without Swing/hardware-detection singletons. [tag:better_ux_for_flashing]
     */
    static PortResult resolveFlashPort(
        @Nullable final PortResult selected,
        final boolean liveConnection,
        final List<PortResult> dfuPorts,
        final List<PortResult> bltPorts
    ) {
        if (selected != null && (selected.type == OpenBlt || selected.type == SerialPortType.Dfu)) {
            return selected;
        }
        if (!liveConnection) {
            // A board sitting in the STM32 built-in bootloader is flashed manually via DFU; prefer it over
            // the connection-dependent AUTO path (and over a null/stale combo selection right after launch)
            // so the button isn't mislabeled as a dead OpenBLT action. [tag:better_ux_for_flashing]
            if (!dfuPorts.isEmpty()) {
                return dfuPorts.get(0);
            }
            if (!bltPorts.isEmpty()) {
                return bltPorts.get(0);
            }
        }
        return selected;
    }

    private void executeJob(JComponent parent, UpdateMode selectedMode, PortResult selectedPort) {
        log.info("ProgramSelector " + selectedMode + " " + selectedPort);
        Objects.requireNonNull(selectedMode);
        AsyncJob job;
        switch (selectedMode) {
            case DFU_AUTO:
                job = new DfuAutoJob(selectedPort, parent, connectivityContext, linkManager);
                break;
            case DFU_MANUAL:
                job = new DfuManualJob(connectivityContext.getConnectedEcuTarget());
                break;
            case INSTALL_OPENBLT:
                job = new InstallOpenBltJob(connectivityContext.getConnectedEcuTarget());
                break;
            case ST_LINK:
                job = new StLinkJob(parent, connectivityContext.getConnectedEcuTarget());
                break;
            case DFU_SWITCH:
                job = new DfuSwitchJob(selectedPort, parent, linkManager);
                break;
            case OPENBLT_SWITCH:
                job = new OpenBltSwitchJob(selectedPort, parent, linkManager, OpenbltRebooter.PRODUCTION_REBOOTER);
                break;
            case OPENBLT_CAN:
                job = new OpenBltCanJob(parent);
                break;
            case OPENBLT_MANUAL:
                job = OpenBltManualJobFactory.createProduction(selectedPort, parent, connectivityContext);
                break;
            case OPENBLT_AUTO:
                job = new OpenBltAutoJob(selectedPort, parent, connectivityContext, linkManager);
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
        if (jobExecutor != null) {
            jobExecutor.addOnJobAboutToStartListener(() -> SwingUtilities.invokeLater(() -> apply(connectivityContext.getCurrentHardware())));
            jobExecutor.addOnJobInProgressFinishedListener(() -> SwingUtilities.invokeLater(() -> apply(connectivityContext.getCurrentHardware())));
        }
    }

    public void setLinkManager(@Nullable LinkManager linkManager) {
        this.linkManager = linkManager;
    }

    public void setFirmwareUpdateInterceptor(Function<JComponent, Boolean> firmwareUpdateInterceptor) {
        this.firmwareUpdateInterceptor = Objects.requireNonNull(firmwareUpdateInterceptor);
    }

    public void setExternalBusySupplier(BooleanSupplier externalBusy) {
        this.externalBusy = Objects.requireNonNull(externalBusy);
    }

    /**
     * Programmatically trigger the main "Update Firmware" action for the currently selected port —
     * same as clicking the split button. Used by the console's "Update ECU" menu shortcut [tag:better_ux_for_flashing].
     */
    public void triggerUpdateFirmware() {
        if (firmwareUpdateInterceptor.apply(splitButton)) {
            return;
        }
        final PortResult targetPort = resolveFlashPort();
        if (targetPort == null) {
            // Nothing detected/selected — mirrors the split button being disabled in apply(). [tag:better_ux_for_flashing]
            log.info("triggerUpdateFirmware: no port to flash, ignoring");
            return;
        }
        executeJob(splitButton, mainButtonModeFor(targetPort), targetPort);
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
        BinaryProtocol bp,
        LinkManager lm,
        UpdateOperationCallbacks callbacks,
        ConnectivityContext connectivityContext
    ) {
        return flashOpenbltSerialAutomatic(parent, ecuPort, bp, lm, callbacks, connectivityContext, null);
    }

    public static boolean flashOpenbltSerialAutomatic(
        JComponent parent,
        PortResult ecuPort,
        BinaryProtocol bp,
        LinkManager lm,
        UpdateOperationCallbacks callbacks,
        ConnectivityContext connectivityContext,
        @Nullable String firmwareSrecFile
    ) {
        return flashOpenbltSerialAutomatic(parent, ecuPort, bp, lm, callbacks, connectivityContext,
            firmwareSrecFile, CalibrationsHelper.FirmwareUpdatePolicy.FORWARD_MIGRATION);
    }

    public static boolean flashOpenbltSerialAutomatic(
        JComponent parent,
        PortResult ecuPort,
        BinaryProtocol bp,
        LinkManager lm,
        UpdateOperationCallbacks callbacks,
        ConnectivityContext connectivityContext,
        @Nullable String firmwareSrecFile,
        CalibrationsHelper.FirmwareUpdatePolicy policy
    ) {
        return updateFirmwareAndRestorePreviousCalibrations(
            parent, ecuPort, bp, lm, callbacks,
            () -> bltUpdateFirmware(parent, ecuPort, callbacks, connectivityContext, firmwareSrecFile),
            connectivityContext, policy);
    }

    private static boolean bltUpdateFirmware(JComponent parent, PortResult ecuPort, UpdateOperationCallbacks callbacks,
                                             ConnectivityContext connectivityContext, @Nullable String firmwareSrecFile) {
        // Suspend the scanner for the entire reboot → detect → flash sequence so it
        // never races with our direct port probes for exclusive COM port access on Windows.
        try {
            connectivityContext.getPortScanner().suspend().await(30, TimeUnit.SECONDS);
        } catch (InterruptedException e) {
            Thread.currentThread().interrupt();
        }
        try {
            return bltUpdateFirmwareWithSuspendedScanner(parent, ecuPort, callbacks, connectivityContext, firmwareSrecFile);
        } finally {
            // Invalidate the cache so the scanner re-inspects the port (now running new
            // firmware) on its first post-resume scan cycle.
            connectivityContext.getPortScanner().invalidatePort(ecuPort.port);
            connectivityContext.getPortScanner().resume();
        }
    }

    private static boolean bltUpdateFirmwareWithSuspendedScanner(JComponent parent, PortResult ecuPort,
                                                                 UpdateOperationCallbacks callbacks,
                                                                 ConnectivityContext connectivityContext,
                                                                 @Nullable String firmwareSrecFile) {
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

        return flashOpenbltSerial(
            parent, openbltPort, callbacks, connectivityContext.getConnectedEcuTarget(), firmwareSrecFile);
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

    public static boolean flashOpenbltSerial(JComponent parent, String port, UpdateOperationCallbacks callbacks,
                                              com.rusefi.core.io.ConnectedEcuTarget connectedEcuTarget) {
        return flashOpenbltSerial(parent, port, callbacks, connectedEcuTarget, null);
    }

    public static boolean flashOpenbltSerial(JComponent parent, String port, UpdateOperationCallbacks callbacks,
                                             com.rusefi.core.io.ConnectedEcuTarget connectedEcuTarget,
                                             @Nullable String firmwareSrecFile) {
        if (FileLog.is32bitJava()) {
            showError32bitJava(parent);
            return false;
        }

        OpenbltJni.OpenbltCallbacks cb = makeOpenbltCallbacks(callbacks);

        String fileName = firmwareSrecFile != null
            ? firmwareSrecFile
            : FindFileHelper.findSrecFileForConnectedBoard(connectedEcuTarget);
        if (fileName == null) {
            callbacks.logLine(".srec image file not found");
            return false;
        }
        // refuse to silently flash firmware built for a different board (e.g. a dev-build fallback or a naming quirk). [tag:better_ux_for_flashing]
        if (!MaintenanceUtil.confirmFirmwareMatchesBoard(fileName, callbacks, connectedEcuTarget)) {
            callbacks.logLine("Firmware update aborted — firmware/board mismatch.");
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

    public void addFirmwareControl(JComponent control) {
        additionalFirmwareControls.add(control);
        updateModeAndButton.add(control, 0);
        matchFirmwareControlSizes();
    }

    private void matchFirmwareControlSizes() {
        Dimension size = splitButton.getPreferredSize();
        for (JComponent control : additionalFirmwareControls) {
            control.setPreferredSize(size);
        }
    }

    public void apply(AvailableHardware currentHardware) {
        boolean isJobRunning = (jobExecutor != null && !jobExecutor.isNotInProgress()) || externalBusy.getAsBoolean();
        boolean additionalControlVisible = additionalFirmwareControls.stream().anyMatch(Component::isVisible);
        noHardware.setVisible(currentHardware.isEmpty());
        updateModeAndButton.setVisible(shouldShowFirmwareControls(
            currentHardware.isEmpty(), isJobRunning, additionalControlVisible));

        boolean hasSerialPorts = !currentHardware.getKnownPorts().isEmpty();
        boolean hasDfuDevice = currentHardware.isDfuFound();

        JPopupMenu popupMenu = new JPopupMenu();

        if (FileLog.isWindows()) {
            boolean requireBlt = FindFileHelper.isObfuscated()
                || isForeignBoardOnUniversalBundle(connectivityContext.getConnectedEcuTarget());

            if (!requireBlt) {
                if (hasSerialPorts) {
                    addMenuItem(popupMenu, DFU_AUTO);
                }
                if (hasDfuDevice) {
                    addMenuItem(popupMenu, DFU_MANUAL);
                    addMenuItem(popupMenu, DFU_ERASE);
                    if (DfuFlasher.haveBootloaderBinFile()) {
                        addMenuItem(popupMenu, INSTALL_OPENBLT);
                    }
                }
                addMenuItem(popupMenu, DFU_SWITCH);
                if (currentHardware.isStLinkConnected()) {
                    addMenuItem(popupMenu, ST_LINK);
                }
            }
            if (currentHardware.isPCANConnected()) {
                addMenuItem(popupMenu, OPENBLT_CAN);
            }
        }

        if (hasSerialPorts) {
            addMenuItem(popupMenu, OPENBLT_SWITCH);
            addMenuItem(popupMenu, OPENBLT_MANUAL);
        }

        int menuItemCount = popupMenu.getComponentCount();

        splitButton.setPopupMenu(menuItemCount > 0 ? popupMenu : null);
        splitButton.setMainButtonEnabled(hasSerialPorts && !isJobRunning);
        splitButton.setArrowButtonEnabled(menuItemCount > 0 && !isJobRunning);

        // Keep the main-button mode/label in sync with the connection state too (not just combo changes):
        // once the board is a live ECU again the button must go back to AUTO. [tag:better_ux_for_flashing]
        refreshMainButtonText();

        AutoupdateUtil.trueLayoutAndRepaint(splitButton);
        AutoupdateUtil.trueLayoutAndRepaint(content);
    }

    static boolean shouldShowFirmwareControls(
        boolean hardwareEmpty,
        boolean jobRunning,
        boolean additionalControlVisible
    ) {
        return !hardwareEmpty || jobRunning || additionalControlVisible;
    }

    /**
     * #9714: is the connected ECU a different board than this bundle? If so, a universal bundle will
     * download that board's firmware on demand and we cannot yet tell whether it is obfuscated, so the
     * caller forces OpenBLT (works for every board here; DFU would fail for obfuscated firmware).
     * With no live ECU, {@code effectiveTarget()} falls back to the persisted last-connected board (or the
     * bundle target if none) — so a board sitting in a bootloader after a restart is still treated as its
     * real (foreign) board here; the flash guard confirms that unverified target before programming.
     */
    private static boolean isForeignBoardOnUniversalBundle(com.rusefi.core.io.ConnectedEcuTarget connectedEcuTarget) {
        String bundleTarget = com.rusefi.core.io.BundleUtil.getBundleTarget();
        String connected = connectedEcuTarget.effectiveTarget();
        return bundleTarget != null && connected != null && !bundleTarget.equalsIgnoreCase(connected);
    }

    private void addMenuItem(JPopupMenu menu, UpdateMode mode) {
        JMenuItem item = new JMenuItem(mode.displayText);
        item.addActionListener(e -> executeJob(splitButton, mode, (PortResult) comboPorts.getSelectedItem()));
        menu.add(item);
    }

    @NotNull
    public static JButton createUpdateFirmwareButton() {
        return new JButton("Update Firmware", AutoupdateUtil.loadIcon("upload48.png"));
    }
}
