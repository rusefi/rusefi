package com.rusefi.maintenance;

import com.rusefi.core.OsUtil;

import com.devexperts.logging.Logging;
import com.rusefi.*;
import com.rusefi.core.FindFileHelper;
import com.rusefi.io.LinkManager;
import com.rusefi.core.ui.AutoupdateUtil;
import com.rusefi.maintenance.jobs.*;
import com.rusefi.ui.basic.SingleAsyncJobExecutor;
import com.rusefi.ui.util.URLLabel;
import com.rusefi.ui.widgets.JSplitButton;
import com.rusefi.ui.wizard.EmergencyWipePanel;
import org.jetbrains.annotations.NotNull;
import org.jetbrains.annotations.Nullable;

import javax.swing.*;
import javax.swing.filechooser.FileNameExtensionFilter;
import java.awt.*;
import java.awt.event.ItemEvent;
import java.io.File;
import java.io.IOException;
import java.util.ArrayList;
import java.util.List;
import java.util.Objects;
import java.util.function.BooleanSupplier;
import java.util.function.Consumer;
import java.util.function.Function;

import static com.devexperts.logging.Logging.getLogging;
import static com.rusefi.SerialPortType.OpenBlt;
import static com.rusefi.maintenance.UpdateMode.*;

public class ProgramSelector {
    private static final Logging log = getLogging(ProgramSelector.class);
    private final JPanel content = new JPanel(new BorderLayout());
    private final JLabel noHardware = new JLabel("Nothing detected");
    private final JPanel updateModeAndButton = new JPanel(new FlowLayout(FlowLayout.CENTER, 5, 5));
    private final JSplitButton splitButton = new JSplitButton("Update ECU Firmware", AutoupdateUtil.loadIcon("upload48.png"));
    private final List<JComponent> additionalFirmwareControls = new ArrayList<>();
    private final ConnectivityContext connectivityContext;
    private final JComboBox<PortResult> comboPorts;
    private final Consumer<JComponent> showFullScreenPanel;
    private final Runnable closeFullScreenPanel;
    @Nullable
    private SingleAsyncJobExecutor jobExecutor;
    @Nullable
    private LinkManager linkManager;
    private Function<JComponent, Boolean> firmwareUpdateInterceptor = source -> false;
    private BooleanSupplier externalBusy = () -> false;

    public ProgramSelector(ConnectivityContext connectivityContext, JComboBox<PortResult> comboPorts,
                           Consumer<JComponent> showFullScreenPanel, Runnable closeFullScreenPanel) {
        this.connectivityContext = connectivityContext;
        this.comboPorts = comboPorts;
        this.showFullScreenPanel = Objects.requireNonNull(showFullScreenPanel);
        this.closeFullScreenPanel = Objects.requireNonNull(closeFullScreenPanel);
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
            if (targetPort == null) {
                return;
            }
            executeJob(splitButton, mainButtonModeFor(targetPort), targetPort);
        });

        // Keep the main button label and enabled state in sync with whatever port is currently selected. The combo is
        // shared with the Connect tab and is repopulated/re-selected after apply() runs, so drive the
        // control off selection changes (same source the action reads) rather than off apply()'s snapshot.
        comboPorts.addItemListener(e -> {
            if (e.getStateChange() == ItemEvent.SELECTED) {
                refreshMainButtonText();
                apply(connectivityContext.getCurrentHardware());
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
            hasMatchingLiveConnection(
                selected,
                isLiveConnection(),
                linkManager == null ? null : linkManager.getLastTriedPort()),
            connectivityContext.getCurrentHardware().getKnownPorts(SerialPortType.Dfu),
            connectivityContext.getCurrentHardware().getKnownPorts(OpenBlt),
            UiProperties.isCanOpenBltEnabled()
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
        final List<PortResult> bltPorts,
        final boolean canOpenBltEnabled
    ) {
        if (isCanEcu(selected)) {
            return canOpenBltEnabled && liveConnection ? selected : null;
        }
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
        return isUnflashableEcu(selected) ? null : selected;
    }

    static boolean hasMatchingLiveConnection(
        @Nullable PortResult selected,
        boolean liveConnection,
        @Nullable String connectedPort
    ) {
        return liveConnection
            && (!isCanEcu(selected) || selected.port.equals(connectedPort));
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
                job = new DfuManualJob(connectivityContext.getConnectedEcuTarget(), null,
                    suggested -> DfuBoardPicker.pick(suggested, showFullScreenPanel, closeFullScreenPanel));
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
                job = new OpenBltSwitchJob(selectedPort, parent, linkManager,
                    connectivityContext.getPortScanner(), OpenbltRebooter.PRODUCTION_REBOOTER);
                break;
            case OPENBLT_CAN:
                job = new OpenBltCanJob(parent, connectivityContext.getPortScanner());
                break;
            case OPENBLT_MANUAL:
                job = OpenBltManualJobFactory.createProduction(selectedPort, parent, connectivityContext);
                break;
            case OPENBLT_AUTO:
                job = new OpenBltAutoJob(selectedPort, parent, connectivityContext, linkManager);
                break;
            case OPENBLT_EMERGENCY_WIPE:
                showEmergencyWipeConfirmation(parent, selectedPort);
                return;
            case DFU_ERASE:
                job = new DfuEraseJob();
                break;
            default:
                throw new IllegalArgumentException("How did you " + selectedMode);
        }

        runJob(job, parent);
    }

    private void showEmergencyWipeConfirmation(JComponent parent, PortResult selectedPort) {
        final OpenBltWipeArtifact artifact;
        try {
            artifact = OpenBltWipeArtifact.loadAndValidate(selectedPort);
        } catch (IOException e) {
            log.error("Emergency wipe validation failed", e);
            JOptionPane.showMessageDialog(parent, e.getMessage(), "Emergency wipe unavailable",
                JOptionPane.ERROR_MESSAGE);
            return;
        }

        showFullScreenPanel.accept(new EmergencyWipePanel(artifact.confirmationMessage(), () -> {
            closeFullScreenPanel.run();
            runJob(OpenBltManualJobFactory.createEmergencyWipe(
                selectedPort, parent, connectivityContext, artifact), parent);
        }, closeFullScreenPanel));
    }

    private void runJob(AsyncJob job, JComponent parent) {
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
        apply(currentHardware, DfuFlasher.isDfuProgrammingSupported(), FindFileHelper.isObfuscated(),
            com.rusefi.core.io.BundleUtil.getBundleTarget(),
            connectivityContext.getConnectedEcuTarget().effectiveTarget());
    }

    /** Snapshot inputs permit hardware-free testing of the actual buttons and menu. */
    JPopupMenu apply(AvailableHardware currentHardware, boolean supportsDfu, boolean obfuscated,
                     String bundleTarget, String effectiveTarget) {
        boolean isJobRunning = (jobExecutor != null && !jobExecutor.isNotInProgress()) || externalBusy.getAsBoolean();
        boolean additionalControlVisible = additionalFirmwareControls.stream().anyMatch(Component::isVisible);
        noHardware.setVisible(currentHardware.isEmpty());
        updateModeAndButton.setVisible(shouldShowFirmwareControls(
            currentHardware.isEmpty(), isJobRunning, additionalControlVisible));

        final List<PortResult> knownPorts = currentHardware.getKnownPorts();
        boolean hasSerialPorts = hasRealSerialPort(knownPorts);
        boolean hasDfuDevice = currentHardware.isDfuFound();

        JPopupMenu popupMenu = new JPopupMenu();

        boolean requireBlt = obfuscated || isForeignBoardOnUniversalBundle(bundleTarget, effectiveTarget);
        boolean canUseDfu = supportsDfu && !requireBlt;
        boolean canUseManualDfu = canUseManualDfu(supportsDfu, obfuscated, bundleTarget, effectiveTarget);

        if (canUseDfu) {
            if (hasSerialPorts) {
                addMenuItem(popupMenu, DFU_AUTO);
                addMenuItem(popupMenu, DFU_SWITCH);
            }
        }
        if (hasDfuDevice && canUseManualDfu) {
            addMenuItem(popupMenu, DFU_MANUAL);
        }
        if (canUseDfu) {
            if (hasDfuDevice) {
                addCustomFirmwareMenuItem(popupMenu);
                addMenuItem(popupMenu, DFU_ERASE);
                if (DfuFlasher.haveBootloaderBinFile()) {
                    addMenuItem(popupMenu, INSTALL_OPENBLT);
                }
            }
        }

        if (OsUtil.isWindows()) {
            if (!requireBlt && currentHardware.isStLinkConnected()) {
                addMenuItem(popupMenu, ST_LINK);
            }
            if (currentHardware.isPCANConnected()) {
                addMenuItem(popupMenu, OPENBLT_CAN);
            }
        }

        if (hasSerialPorts) {
            addMenuItem(popupMenu, OPENBLT_SWITCH);
            addMenuItem(popupMenu, OPENBLT_MANUAL);
            PortResult selected = resolveFlashPort();
            if (isEmergencyWipeAvailable(selected)) {
                addMenuItem(popupMenu, OPENBLT_EMERGENCY_WIPE);
            }
        }

        int menuItemCount = popupMenu.getComponentCount();
        PortResult flashPort = resolveFlashPort();
        boolean hasFirmwareTarget = hasFirmwareTarget(hasSerialPorts, flashPort);

        splitButton.setPopupMenu(menuItemCount > 0 ? popupMenu : null);
        splitButton.setMainButtonEnabled(shouldEnableMainButton(
            hasFirmwareTarget, hasDfuDevice, isJobRunning, mainButtonModeFor(flashPort), canUseManualDfu));
        splitButton.setArrowButtonEnabled(menuItemCount > 0 && !isJobRunning);

        // Keep the main-button mode/label in sync with the connection state too (not just combo changes):
        // once the board is a live ECU again the button must go back to AUTO. [tag:better_ux_for_flashing]
        refreshMainButtonText();

        AutoupdateUtil.trueLayoutAndRepaint(splitButton);
        AutoupdateUtil.trueLayoutAndRepaint(content);
        return popupMenu;
    }

    static boolean hasRealSerialPort(List<PortResult> ports) {
        return ports.stream().anyMatch(port -> port.type != SerialPortType.Dfu && !isUnflashableEcu(port));
    }

    static boolean canUseManualDfu(boolean platformSupported, boolean obfuscated,
                                   String bundleTarget, String effectiveTarget) {
        if (ManualDfuRecovery.isUniversalBundle(bundleTarget)) {
            return platformSupported;
        }
        boolean foreignBoard = bundleTarget != null && effectiveTarget != null
            && !bundleTarget.equalsIgnoreCase(effectiveTarget);
        return platformSupported && !obfuscated && !foreignBoard;
    }

    static boolean hasFirmwareTarget(boolean hasSerialPorts, @Nullable PortResult flashPort) {
        return hasSerialPorts || isCanEcu(flashPort);
    }

    static boolean isEmergencyWipeAvailable(@Nullable PortResult port) {
        return OpenBltWipeArtifact.isAvailableFor(port);
    }

    static boolean shouldEnableMainButton(
        boolean hasFirmwareTarget,
        boolean hasDfuDevice,
        boolean jobRunning,
        UpdateMode mode,
        boolean supportsDfu
    ) {
        boolean targetAvailable = mode == DFU_MANUAL ? hasDfuDevice : hasFirmwareTarget;
        boolean modeSupported = mode != DFU_MANUAL || supportsDfu;
        return targetAvailable && modeSupported && !jobRunning;
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
    private static boolean isForeignBoardOnUniversalBundle(String bundleTarget, String connected) {
        return bundleTarget != null && connected != null && !bundleTarget.equalsIgnoreCase(connected);
    }

    private void addMenuItem(JPopupMenu menu, UpdateMode mode) {
        JMenuItem item = new JMenuItem(mode.displayText);
        item.addActionListener(e -> {
            PortResult selected = mode == OPENBLT_EMERGENCY_WIPE
                ? resolveFlashPort()
                : (PortResult) comboPorts.getSelectedItem();
            if (isUnflashableEcu(selected)) {
                return;
            }
            executeJob(splitButton, mode, selected);
        });
        menu.add(item);
    }

    private void addCustomFirmwareMenuItem(JPopupMenu menu) {
        JMenuItem item = new JMenuItem("Pick Custom File [DFU]");
        item.addActionListener(e -> {
            JFileChooser chooser = new JFileChooser();
            chooser.setDialogTitle("Pick DFU firmware");
            chooser.setFileSelectionMode(JFileChooser.FILES_ONLY);
            chooser.setAcceptAllFileFilterUsed(false);
            chooser.setFileFilter(new FileNameExtensionFilter("Firmware binaries (.bin)", "bin"));
            if (chooser.showOpenDialog(splitButton) != JFileChooser.APPROVE_OPTION) {
                return;
            }

            File firmware = chooser.getSelectedFile();
            if (!firmware.isFile() || !chooser.getFileFilter().accept(firmware)) {
                JOptionPane.showMessageDialog(splitButton, "Please select a .bin firmware file.",
                    "Invalid firmware file", JOptionPane.ERROR_MESSAGE);
                return;
            }

            runJob(new DfuManualJob(
                connectivityContext.getConnectedEcuTarget(), firmware.getAbsolutePath()), splitButton);
        });
        menu.add(item);
    }

    private static boolean isUnflashableEcu(@Nullable PortResult port) {
        return port != null && (port.isUnsupportedEcu()
            || port.type == SerialPortType.EcuUnknown
            || LinkManager.isCanPort(port.port));
    }

    private static boolean isCanEcu(@Nullable PortResult port) {
        return port != null && isAutomaticCanPort(port.port) && port.isEcu();
    }

    /** CAN transports supported by the OpenBLT flasher. */
    private static boolean isAutomaticCanPort(String port) {
        return LinkManager.PCAN.equals(port) || LinkManager.SOCKET_CAN.equals(port);
    }

}
