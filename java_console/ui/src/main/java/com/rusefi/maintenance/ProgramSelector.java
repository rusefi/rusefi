package com.rusefi.maintenance;

import com.rusefi.core.OsUtil;

import com.devexperts.logging.Logging;
import com.rusefi.*;
import com.rusefi.core.FindFileHelper;
import com.rusefi.core.io.BundleInfo;
import com.rusefi.core.io.BundleUtil;
import com.rusefi.core.io.ConnectedEcuTarget;
import com.rusefi.io.LinkManager;
import com.rusefi.core.ui.AutoupdateUtil;
import com.rusefi.maintenance.jobs.*;
import com.rusefi.ui.basic.SingleAsyncJobExecutor;
import com.rusefi.ui.util.URLLabel;
import com.rusefi.ui.widgets.JSplitButton;
import com.rusefi.ui.widgets.JoinedButtons;
import com.rusefi.ui.wizard.EmergencyWipePanel;
import com.rusefi.ui.wizard.DfuRecoveryConfirmationPanel;
import com.rusefi.ui.wizard.FirmwareProgrammingPanel;
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
import java.util.concurrent.atomic.AtomicBoolean;
import java.util.function.BooleanSupplier;
import java.util.function.BiFunction;
import java.util.function.Consumer;
import java.util.function.Function;
import java.util.function.Predicate;
import java.util.function.Supplier;

import static com.devexperts.logging.Logging.getLogging;
import static com.rusefi.SerialPortType.OpenBlt;
import static com.rusefi.maintenance.UpdateMode.*;

public class ProgramSelector {
    private static final Logging log = getLogging(ProgramSelector.class);
    private final JPanel content = new JPanel(new BorderLayout());
    private final JLabel noHardware = new JLabel("Nothing detected");
    private final JPanel updateModeAndButton = new JPanel(new FlowLayout(FlowLayout.CENTER, 5, 5));
    private final JSplitButton splitButton = new JSplitButton("Update ECU firmware", AutoupdateUtil.loadIcon("upload48.png"));
    private boolean recoveryDefault;
    private boolean directJobInProgress;
    private final List<JComponent> additionalFirmwareControls = new ArrayList<>();
    private final ConnectivityContext connectivityContext;
    private final JComboBox<PortResult> comboPorts;
    private final Consumer<JComponent> showFullScreenPanel;
    private final Runnable closeFullScreenPanel;
    @Nullable
    private SingleAsyncJobExecutor jobExecutor;
    @Nullable
    private LinkManager linkManager;
    private BiFunction<JComponent, PortResult, Boolean> firmwareUpdateInterceptor = (source, port) -> false;
    private BooleanSupplier externalBusy = () -> false;
    @Nullable
    private Predicate<String> confirmDfuRecoveryForTests;
    private long modalGeneration;
    @Nullable
    private Predicate<String> confirmNormalUpdateForTests;
    // Rendering is only a hint: every invocation reads fresh capability inputs, including after modal confirmation.
    private Supplier<ProgrammingCapabilities> capabilityProvider;

    static final class ProgrammingCapabilities {
        final boolean supportsDfu;
        final boolean obfuscated;
        final String bundleTarget;
        final String effectiveTarget;
        final boolean windows;

        ProgrammingCapabilities(boolean supportsDfu, boolean obfuscated, String bundleTarget,
                                String effectiveTarget, boolean windows) {
            this.supportsDfu = supportsDfu;
            this.obfuscated = obfuscated;
            this.bundleTarget = bundleTarget;
            this.effectiveTarget = effectiveTarget;
            this.windows = windows;
        }

        boolean requiresBlt() {
            return obfuscated || isForeignBoardOnUniversalBundle(bundleTarget, effectiveTarget);
        }

        boolean canUseDfu() {
            return supportsDfu && !requiresBlt();
        }
    }

    public ProgramSelector(ConnectivityContext connectivityContext, JComboBox<PortResult> comboPorts,
                           Consumer<JComponent> showFullScreenPanel, Runnable closeFullScreenPanel) {
        this.connectivityContext = connectivityContext;
        capabilityProvider = () -> new ProgrammingCapabilities(
            DfuFlasher.isDfuProgrammingSupported(), FindFileHelper.isObfuscated(), BundleUtil.getBundleTarget(),
            connectivityContext.getConnectedEcuTarget().effectiveTarget(), OsUtil.isWindows());
        this.comboPorts = comboPorts;
        this.showFullScreenPanel = Objects.requireNonNull(showFullScreenPanel);
        this.closeFullScreenPanel = Objects.requireNonNull(closeFullScreenPanel);
        noHardware.setBorder(BorderFactory.createEmptyBorder(5, 8, 5, 8));
        noHardware.setFont(noHardware.getFont().deriveFont(noHardware.getFont().getSize2D() + 2));
        content.add(updateModeAndButton, BorderLayout.NORTH);
        content.add(noHardware, BorderLayout.SOUTH);

        updateModeAndButton.setVisible(false);
        updateModeAndButton.add(splitButton);
        splitButton.addActionListener(e -> triggerPrimaryAction());

        // Keep the main button label and enabled state in sync with whatever port is currently selected. The combo is
        // shared with the Connect tab and is repopulated/re-selected after apply() runs, so drive the
        // control off selection changes (same source the action reads) rather than off apply()'s snapshot.
        comboPorts.addItemListener(e -> {
            if (e.getStateChange() == ItemEvent.SELECTED) {
                apply(connectivityContext.getCurrentHardware());
            }
        });
        refreshMainButtonText();
    }

    /**
     * Pure decision logic behind the normal update button mode, extracted so it can be unit tested
     * without Swing/hardware-detection singletons. [tag:better_ux_for_flashing]
     */
    @Nullable
    static UpdateMode mainButtonModeFor(@Nullable PortResult selectedPort, boolean liveConnection,
                                        boolean canUseDfu) {
        if (selectedPort != null && selectedPort.type == OpenBlt) {
            return OPENBLT_MANUAL;
        }
        // A DFU device is a board already sitting in the STM32 built-in bootloader: it has no running
        // firmware to reboot and never auto-connects. The adaptive primary button offers confirmed
        // recovery, but the normal-only update shortcut must not recover. [tag:better_ux_for_flashing]
        if (selectedPort != null && selectedPort.type == SerialPortType.Dfu) {
            return null;
        }
        if (selectedPort != null && selectedPort.type == SerialPortType.EcuWithOpenblt) {
            return liveConnection ? OPENBLT_AUTO : null;
        }
        if (isCanEcu(selectedPort)) {
            return liveConnection ? OPENBLT_AUTO : null;
        }
        if (selectedPort != null && selectedPort.type == SerialPortType.Ecu) {
            if (!liveConnection) {
                return null;
            }
            return canUseDfu ? DFU_AUTO : null;
        }
        return null;
    }

    private void refreshMainButtonText() {
        refreshMainButtonText(isDfuRecoveryDefault(connectivityContext.getCurrentHardware()));
    }

    private void refreshMainButtonText(boolean recoveryDefault) {
        this.recoveryDefault = recoveryDefault;
        splitButton.setText(recoveryDefault ? "Recover ECU" : "Update ECU firmware");
        matchFirmwareControlSizes();
    }

    private boolean isDfuRecoveryDefault(@Nullable AvailableHardware hardware) {
        if (hardware == null || !hardware.isDfuFound()) {
            return false;
        }
        final PortResult selected = (PortResult) comboPorts.getSelectedItem();
        final List<PortResult> knownPorts = hardware.getKnownPorts();
        if (selected != null) {
            final PortResult current = currentPort(knownPorts, selected);
            return current != null && current.type == SerialPortType.Dfu;
        }
        // With no selection, only one DFU-only detection can supply the default. Never replace a
        // stale/unknown selection or another ECU's update action with independently detected recovery.
        return knownPorts.size() == 1 && resolveDetectedDfuPort(hardware) != null;
    }

    private void triggerPrimaryAction() {
        final AvailableHardware hardware = connectivityContext.getCurrentHardware();
        if (recoveryDefault != isDfuRecoveryDefault(hardware)) {
            // A changed device must not turn a click on a stale Update label into recovery (or vice versa).
            apply(hardware);
            return;
        }
        if (recoveryDefault) {
            startDfuRecovery(null, true);
        } else {
            triggerUpdateFirmware();
        }
    }

    private boolean isLiveConnection() {
        return linkManager != null && linkManager.getBinaryProtocol() != null;
    }

    /** Resolve only the currently selected, still-detected port for expert operations; never guess a fallback. */
    private PortResult resolveFlashPort() {
        final PortResult selected = (PortResult) comboPorts.getSelectedItem();
        final AvailableHardware hardware = connectivityContext.getCurrentHardware();
        return currentPort(hardware.getKnownPorts(), selected);
    }

    /**
     * Resolve only a normal firmware-update target. DFU is intentionally excluded: it has its own
     * explicitly labelled and confirmed recovery action, never reached by the normal update shortcut.
     */
    private PortResult resolveNormalFlashPort() {
        final PortResult selected = (PortResult) comboPorts.getSelectedItem();
        final AvailableHardware hardware = connectivityContext.getCurrentHardware();
        return resolveNormalFlashPort(
            selected,
            isLiveConnection(),
            linkManager == null ? null : linkManager.getLastTriedPort(),
            hardware.getKnownPorts(),
            UiProperties.isCanOpenBltEnabled()
        );
    }

    static PortResult resolveNormalFlashPort(
        @Nullable PortResult selected,
        boolean liveConnection,
        @Nullable String connectedPort,
        List<PortResult> knownPorts,
        boolean canOpenBltEnabled
    ) {
        selected = currentPort(knownPorts, selected);
        if (selected == null) {
            return null;
        }
        if (isCanEcu(selected)) {
            return canOpenBltEnabled && hasMatchingLiveConnection(selected, liveConnection, connectedPort)
                ? selected : null;
        }
        if (isUnflashableEcu(selected)) {
            return null;
        }
        if (selected.type == SerialPortType.Dfu) {
            return null;
        }
        if (selected.type == OpenBlt) {
            return selected;
        }
        if (selected.type == SerialPortType.Ecu || selected.type == SerialPortType.EcuWithOpenblt) {
            return hasMatchingLiveConnection(selected, liveConnection, connectedPort) ? selected : null;
        }
        return null;
    }

    private PortResult resolveDetectedDfuPort(@Nullable AvailableHardware hardware) {
        if (hardware == null || !hardware.isDfuFound()) {
            return null;
        }
        final List<PortResult> detected = hardware.getKnownPorts(SerialPortType.Dfu);
        // The DFU backend is global, not bound to the selected physical device. A selection cannot disambiguate it.
        return detected.size() == 1 ? detected.get(0) : null;
    }

    private boolean canRecoverDfu(@Nullable AvailableHardware hardware, boolean customFirmware) {
        final ProgrammingCapabilities capabilities = capabilityProvider.get();
        return resolveDetectedDfuPort(hardware) != null
            && (customFirmware ? capabilities.canUseDfu() : canUseManualDfu(capabilities.supportsDfu,
                capabilities.obfuscated, capabilities.bundleTarget, capabilities.effectiveTarget))
            && !isJobRunning();
    }

    private boolean isJobRunning() {
        return directJobInProgress || (jobExecutor != null && !jobExecutor.isNotInProgress()) || externalBusy.getAsBoolean();
    }

    private void startDfuRecovery() {
        startDfuRecovery(null, false);
    }

    void startCustomDfuRecovery(File firmware) {
        if (!firmware.isFile() || !new FileNameExtensionFilter("Firmware binaries (.bin)", "bin").accept(firmware)) {
            showProgrammingError("Invalid firmware file", "Please select a .bin firmware file.");
            return;
        }
        startDfuRecovery(firmware.getAbsolutePath(), false);
    }

    private void startDfuRecovery(@Nullable String firmwareFile, boolean requireRecoveryDefault) {
        final AvailableHardware currentHardware = connectivityContext.getCurrentHardware();
        if (!canRecoverDfu(currentHardware, firmwareFile != null)
            || (requireRecoveryDefault && !isDfuRecoveryDefault(currentHardware))) {
            return;
        }
        final PortResult dfuPort = resolveDetectedDfuPort(currentHardware);
        if (dfuPort == null) {
            return;
        }
        final ProgrammingCapabilities confirmedCapabilities = capabilityProvider.get();
        final String warning = "Recovery may erase firmware and settings. Save a tune backup first, if possible.\n\n"
            + "Recovery cannot identify the board or choose between connected ECUs. Disconnect all other ECUs "
            + "and connect only the ECU you want to recover. Select its exact board and firmware.\n\n"
            + "Keep the engine stopped and maintain stable power throughout recovery.";
        showDfuRecoveryConfirmation(warning, () -> {
            final AvailableHardware latestHardware = connectivityContext.getCurrentHardware();
            final PortResult latestDfuPort = resolveDetectedDfuPort(latestHardware);
            if (!canRecoverDfu(latestHardware, firmwareFile != null) || latestDfuPort == null
                || !samePort(dfuPort, latestDfuPort)
                || (requireRecoveryDefault && !isDfuRecoveryDefault(latestHardware))
                || !sameFirmwareTarget(confirmedCapabilities, capabilityProvider.get())) {
                return;
            }
            if (firmwareFile == null) {
                executeJob(splitButton, DFU_MANUAL, latestDfuPort);
            } else {
                runJob(createDfuManualJob(firmwareFile), splitButton);
            }
        });
    }

    private void showDfuRecoveryConfirmation(String warning, Runnable confirmed) {
        if (confirmDfuRecoveryForTests != null) {
            // Existing deterministic capability tests can still supply immediate consent. Production never blocks EDT.
            if (confirmDfuRecoveryForTests.test(warning)) {
                confirmed.run();
            }
            return;
        }
        final long generation = ++modalGeneration;
        showFullScreenPanel.accept(new DfuRecoveryConfirmationPanel(warning,
            modalAction(generation, confirmed), modalAction(generation, () -> {})));
    }

    private void showNormalUpdateConfirmation(String message, Runnable confirmed) {
        if (confirmNormalUpdateForTests != null) {
            if (confirmNormalUpdateForTests.test(message)) {
                confirmed.run();
            }
            return;
        }
        final long generation = ++modalGeneration;
        showFullScreenPanel.accept(FirmwareProgrammingPanel.updateConfirmation(message,
            modalAction(generation, confirmed), modalAction(generation, () -> {})));
    }

    private void showProgrammingError(String title, String message) {
        final long generation = ++modalGeneration;
        showFullScreenPanel.accept(FirmwareProgrammingPanel.error(title, message,
            modalAction(generation, () -> {})));
    }

    /** Every selector modal shares ownership, including wipe and errors; stale actions cannot affect a newer view. */
    private Runnable modalAction(long generation, Runnable action) {
        return () -> {
            if (generation != modalGeneration) {
                return;
            }
            final long closedGeneration = ++modalGeneration;
            closeFullScreenPanel.run(); // Close before an interceptor or accepted job may open its next screen.
            if (modalGeneration == closedGeneration) {
                action.run();
            }
        };
    }

    private static boolean samePort(@Nullable PortResult left, @Nullable PortResult right) {
        return left != null && right != null && left.type == right.type && left.port.equals(right.port);
    }

    private String normalUpdateConfirmation(PortResult targetPort, ProgrammingCapabilities capabilities) {
        final ConnectedEcuTarget ecuTarget = connectivityContext.getConnectedEcuTarget();
        final String target = capabilities.effectiveTarget;
        final String targetDescription;
        if (targetPort.type == OpenBlt && isKnownTarget(bootloaderBoard(targetPort))) {
            targetDescription = bootloaderBoard(targetPort) + " (reported by selected bootloader)";
        } else if (targetPort.type == OpenBlt) {
            targetDescription = isKnownTarget(target)
                ? target + " (suggested only; selected bootloader did not identify the board)"
                : "Unknown (selected bootloader did not identify the board)";
        } else if (ecuTarget.isLiveTargetKnown()) {
            targetDescription = isKnownTarget(target)
                ? target + " (reported by connected ECU)"
                : "Unknown (connected ECU did not identify the board)";
        } else if (isKnownTarget(target)) {
            final String persisted = ConnectedEcuTarget.readPersisted();
            final String source = target.equalsIgnoreCase(persisted == null ? "" : persisted)
                ? "Saved board selection — not checked against the connected ECU"
                : "Board specified by this firmware package — not checked against the ECU";
            targetDescription = target + " (" + source + ")";
        } else {
            targetDescription = "Unknown — board not verified";
        }
        final BundleInfo bundle = BundleUtil.readBundleFullNameNotNull();
        final String channel = bundle.isMaster() ? "Snapshot (development build)" : bundle.getBranchName();
        return "ECU board: " + targetDescription + "\nFirmware channel: " + channel
            + "\n\nConnect only the ECU you want to update. Keep the engine stopped and maintain stable power."
            + "\n\nSelected device: " + targetPort.port;
    }

    private static boolean isKnownTarget(@Nullable String target) {
        return target != null && !target.trim().isEmpty() && !"unknown".equalsIgnoreCase(target.trim());
    }

    static boolean hasMatchingLiveConnection(
        @Nullable PortResult selected,
        boolean liveConnection,
        @Nullable String connectedPort
    ) {
        return selected != null && liveConnection && selected.port.equals(connectedPort);
    }

    private static PortResult currentPort(List<PortResult> ports, @Nullable PortResult selected) {
        return ports.stream().filter(port -> samePort(port, selected)).findFirst().orElse(null);
    }

    private boolean canUseDfuNow() {
        return capabilityProvider.get().canUseDfu();
    }

    private UpdateMode normalModeFor(PortResult targetPort, ProgrammingCapabilities capabilities) {
        return mainButtonModeFor(targetPort, isLiveConnection(), capabilities.canUseDfu());
    }

    private static String bootloaderBoard(PortResult port) {
        return port.bootloaderInfo == null ? null : port.bootloaderInfo.board;
    }

    private static boolean sameConfirmedPort(PortResult confirmed, @Nullable PortResult current) {
        return samePort(confirmed, current)
            && Objects.equals(confirmed.bootloaderInfo == null ? null : confirmed.bootloaderInfo.raw,
                current.bootloaderInfo == null ? null : current.bootloaderInfo.raw);
    }

    private static boolean sameFirmwareTarget(ProgrammingCapabilities confirmed, ProgrammingCapabilities current) {
        return Objects.equals(confirmed.bundleTarget, current.bundleTarget)
            && Objects.equals(confirmed.effectiveTarget, current.effectiveTarget);
    }

    private boolean isValidatedLiveEcuTarget(PortResult targetPort) {
        return isSerialEcu(targetPort)
            && hasMatchingLiveConnection(targetPort, isLiveConnection(),
                linkManager == null ? null : linkManager.getLastTriedPort());
    }

    void executeJob(JComponent parent, UpdateMode selectedMode, PortResult selectedPort) {
        log.info("ProgramSelector " + selectedMode + " " + selectedPort);
        Objects.requireNonNull(selectedMode);
        AsyncJob job;
        switch (selectedMode) {
            case DFU_AUTO:
                job = new DfuAutoJob(selectedPort, parent, connectivityContext, linkManager);
                break;
            case DFU_MANUAL:
                job = createDfuManualJob(null);
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
            showProgrammingError("Emergency wipe unavailable", e.getMessage() == null
                ? "The emergency wipe artifact could not be validated." : e.getMessage());
            return;
        }

        final long generation = ++modalGeneration;
        showFullScreenPanel.accept(new EmergencyWipePanel(artifact.confirmationMessage(), modalAction(generation, () -> {
            final PortResult latestSelected = resolveFlashPort();
            if (!isJobRunning() && latestSelected != null
                && sameConfirmedPort(selectedPort, latestSelected) && isEmergencyWipeAvailable(latestSelected)) {
                runJob(OpenBltManualJobFactory.createEmergencyWipe(
                    latestSelected, parent, connectivityContext, artifact), parent);
            }
        }), modalAction(generation, () -> {})));
    }

    private void runJob(AsyncJob job, JComponent parent) {
        if (jobExecutor != null) {
            jobExecutor.startJob(job, parent);
        } else {
            if (directJobInProgress) {
                return;
            }
            directJobInProgress = true;
            apply(connectivityContext.getCurrentHardware());
            AsyncJobExecutor.INSTANCE.executeJobWithStatusWindow(job, com.rusefi.io.UpdateOperationCallbacks.DUMMY,
                () -> SwingUtilities.invokeLater(() -> {
                    directJobInProgress = false;
                    apply(connectivityContext.getCurrentHardware());
                }));
        }
    }

    private DfuManualJob createDfuManualJob(@Nullable String firmwareFile) {
        final PortResult intendedDfu = resolveDetectedDfuPort(connectivityContext.getCurrentHardware());
        final PortResult intendedSelection = (PortResult) comboPorts.getSelectedItem();
        final ProgrammingCapabilities intendedCapabilities = capabilityProvider.get();
        return new DfuManualJob(connectivityContext.getConnectedEcuTarget(), firmwareFile,
            suggested -> DfuBoardPicker.pick(suggested, showFullScreenPanel, closeFullScreenPanel),
            () -> validateRecoveryOnEdt(() -> {
                final AvailableHardware hardware = connectivityContext.getCurrentHardware();
                final ProgrammingCapabilities current = capabilityProvider.get();
                final PortResult selected = (PortResult) comboPorts.getSelectedItem();
                final boolean selectionUnchanged = intendedSelection == null
                    ? selected == null || samePort(intendedDfu, currentPort(hardware.getKnownPorts(), selected))
                    : sameConfirmedPort(intendedSelection, currentPort(hardware.getKnownPorts(), selected));
                // The job owns the busy slot now. Recheck other busy sources, not its own reservation.
                return !externalBusy.getAsBoolean() && selectionUnchanged
                    && samePort(intendedDfu, resolveDetectedDfuPort(hardware))
                    && sameFirmwareTarget(intendedCapabilities, current)
                    && (firmwareFile == null ? canUseManualDfu(current.supportsDfu, current.obfuscated,
                        current.bundleTarget, current.effectiveTarget) : current.canUseDfu());
            }));
    }

    private static boolean validateRecoveryOnEdt(BooleanSupplier validation) {
        if (SwingUtilities.isEventDispatchThread()) {
            return validation.getAsBoolean();
        }
        AtomicBoolean valid = new AtomicBoolean();
        try {
            SwingUtilities.invokeAndWait(() -> valid.set(validation.getAsBoolean()));
            return valid.get();
        } catch (InterruptedException e) {
            Thread.currentThread().interrupt();
            return false;
        } catch (java.lang.reflect.InvocationTargetException e) {
            log.error("Recovery eligibility could not be rechecked", e.getCause());
            return false;
        }
    }

    public void setJobExecutor(@Nullable SingleAsyncJobExecutor jobExecutor) {
        this.jobExecutor = jobExecutor;
        if (jobExecutor != null) {
            jobExecutor.addOnJobAboutToStartListener(() -> SwingUtilities.invokeLater(() -> apply(connectivityContext.getCurrentHardware())));
            jobExecutor.addOnJobReleasedListener(() -> SwingUtilities.invokeLater(() -> apply(connectivityContext.getCurrentHardware())));
        }
    }

    public void setLinkManager(@Nullable LinkManager linkManager) {
        this.linkManager = linkManager;
    }

    public void setFirmwareUpdateInterceptor(Function<JComponent, Boolean> firmwareUpdateInterceptor) {
        Objects.requireNonNull(firmwareUpdateInterceptor);
        this.firmwareUpdateInterceptor = (source, port) -> firmwareUpdateInterceptor.apply(source);
    }

    public void setTargetedFirmwareUpdateInterceptor(BiFunction<JComponent, PortResult, Boolean> interceptor) {
        this.firmwareUpdateInterceptor = Objects.requireNonNull(interceptor);
    }

    void setCapabilityProviderForTests(Supplier<ProgrammingCapabilities> provider) {
        capabilityProvider = Objects.requireNonNull(provider);
    }

    public void setExternalBusySupplier(BooleanSupplier externalBusy) {
        this.externalBusy = Objects.requireNonNull(externalBusy);
    }

    void setDfuRecoveryConfirmationForTests(Predicate<String> confirmation) {
        confirmDfuRecoveryForTests = Objects.requireNonNull(confirmation);
    }

    void setNormalUpdateConfirmationForTests(@Nullable Predicate<String> confirmation) {
        confirmNormalUpdateForTests = confirmation;
    }

    /**
     * Programmatically trigger the main "Update ECU firmware" action for the currently selected port —
     * normal update only, even when the adaptive split button offers recovery. Used by the console's
     * "Update ECU" menu shortcut [tag:better_ux_for_flashing].
     */
    public void triggerUpdateFirmware() {
        if (isJobRunning()) {
            return;
        }
        final PortResult targetPort = resolveNormalFlashPort();
        if (targetPort == null) {
            log.info("triggerUpdateFirmware: no normal update target, ignoring");
            return;
        }
        final ProgrammingCapabilities capabilities = capabilityProvider.get();
        final UpdateMode mode = normalModeFor(targetPort, capabilities);
        if (mode == null) {
            return;
        }
        final String confirmation = normalUpdateConfirmation(targetPort, capabilities);
        showNormalUpdateConfirmation(confirmation, () -> {
            final PortResult currentPort = resolveNormalFlashPort();
            final ProgrammingCapabilities currentCapabilities = capabilityProvider.get();
            if (isJobRunning() || !sameConfirmedPort(targetPort, currentPort)
                || !sameFirmwareTarget(capabilities, currentCapabilities)
                || normalModeFor(currentPort, currentCapabilities) != mode) {
                return;
            }
            if (isValidatedLiveEcuTarget(currentPort) && firmwareUpdateInterceptor.apply(splitButton, currentPort)) {
                return;
            }
            executeJob(splitButton, mode, currentPort);
        });
    }

    @NotNull
    public static JComponent createHelpButton() {
        return new URLLabel("HOWTO Update ECU firmware", UiProperties.getUpdateHelpUrl());
    }

    public JPanel getControl() {
        return content;
    }

    /**
     * Fits the control into a toolbar row [tag:console_theme]: left-aligned without the outer gaps,
     * as tall as the other toolbar controls, with a small icon instead of the 48 px arrow.
     */
    public void useToolbarStyle(Icon icon) {
        updateModeAndButton.setLayout(new FlowLayout(FlowLayout.LEFT, JoinedButtons.ROW_GAP, 0));
        updateModeAndButton.setOpaque(false);
        content.setOpaque(false);
        splitButton.setIcon(icon);
        // height only: the label changes with the update mode, so the width must follow it
        splitButton.setFixedHeight(JoinedButtons.HEIGHT);
        matchFirmwareControlSizes();
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
        final ProgrammingCapabilities capabilities = capabilityProvider.get();
        apply(currentHardware, capabilities.supportsDfu, capabilities.obfuscated,
            capabilities.bundleTarget, capabilities.effectiveTarget);
    }

    /** Snapshot inputs permit hardware-free testing of the actual buttons and menu. */
    JPopupMenu apply(AvailableHardware currentHardware, boolean supportsDfu, boolean obfuscated,
                     String bundleTarget, String effectiveTarget) {
        boolean isJobRunning = isJobRunning();
        boolean additionalControlVisible = additionalFirmwareControls.stream().anyMatch(Component::isVisible);
        noHardware.setVisible(currentHardware.isEmpty());
        updateModeAndButton.setVisible(shouldShowFirmwareControls(
            currentHardware.isEmpty(), isJobRunning, additionalControlVisible));

        final List<PortResult> knownPorts = currentHardware.getKnownPorts();
        boolean hasSerialPorts = hasRealSerialPort(knownPorts);
        boolean hasDfuDevice = currentHardware.isDfuFound();

        JPopupMenu popupMenu = new JPopupMenu();
        JMenu advancedTools = new JMenu("Advanced programming tools");

        boolean requireBlt = obfuscated || isForeignBoardOnUniversalBundle(bundleTarget, effectiveTarget);
        boolean canUseDfu = supportsDfu && !requireBlt;
        boolean canUseManualDfu = canUseManualDfu(supportsDfu, obfuscated, bundleTarget, effectiveTarget);

        final PortResult selectedPort = currentPort(knownPorts, (PortResult) comboPorts.getSelectedItem());
        if (canUseDfu && isAdvancedActionAvailable(DFU_AUTO, currentHardware, selectedPort)) {
            addMenuItem(advancedTools, DFU_AUTO);
            addMenuItem(advancedTools, DFU_SWITCH);
        }
        if (hasDfuDevice && canUseManualDfu && resolveDetectedDfuPort(currentHardware) != null) {
            addMenuItem(advancedTools, DFU_MANUAL);
        }
        if (canUseDfu && hasDfuDevice && resolveDetectedDfuPort(currentHardware) != null) {
            addCustomFirmwareMenuItem(advancedTools);
            addMenuItem(advancedTools, DFU_ERASE);
            if (DfuFlasher.haveBootloaderBinFile()) {
                addMenuItem(advancedTools, INSTALL_OPENBLT);
            }
        }

        if (capabilityProvider.get().windows) {
            if (!requireBlt && currentHardware.isStLinkConnected()) {
                addMenuItem(advancedTools, ST_LINK);
            }
            if (currentHardware.isPCANConnected()) {
                addMenuItem(advancedTools, OPENBLT_CAN);
            }
        }

        if (hasSerialPorts) {
            addMenuItem(advancedTools, OPENBLT_SWITCH);
            addMenuItem(advancedTools, OPENBLT_MANUAL);
            PortResult selected = resolveFlashPort();
            if (isEmergencyWipeAvailable(selected)) {
                addMenuItem(advancedTools, OPENBLT_EMERGENCY_WIPE);
            }
        }

        if (advancedTools.getItemCount() > 0) {
            popupMenu.add(advancedTools);
        }
        PortResult flashPort = resolveNormalFlashPort();
        boolean hasFirmwareTarget = hasFirmwareTarget(hasSerialPorts, flashPort) && flashPort != null;
        boolean recoveryAvailable = hasDfuDevice && canUseManualDfu(supportsDfu, obfuscated, bundleTarget, effectiveTarget)
            && resolveDetectedDfuPort(currentHardware) != null;

        splitButton.setPopupMenu(popupMenu.getComponentCount() > 0 ? popupMenu : null);
        final boolean recoveryDefault = isDfuRecoveryDefault(currentHardware);
        final UpdateMode normalMode = flashPort == null ? null
            : mainButtonModeFor(flashPort, isLiveConnection(), canUseDfu);
        splitButton.setMainButtonEnabled(recoveryDefault ? recoveryAvailable && !isJobRunning
            : shouldEnableMainButton(hasFirmwareTarget, isJobRunning, normalMode));
        splitButton.setArrowButtonEnabled(popupMenu.getComponentCount() > 0 && !isJobRunning);

        // Keep the main-button mode/label in sync with the connection state too (not just combo changes):
        // once the board is a live ECU again the button must go back to AUTO. [tag:better_ux_for_flashing]
        refreshMainButtonText(recoveryDefault);

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

    static boolean shouldEnableMainButton(boolean hasFirmwareTarget, boolean jobRunning, @Nullable UpdateMode mode) {
        return mode != null && mode != DFU_MANUAL && hasFirmwareTarget && !jobRunning;
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

    private void addMenuItem(JMenu menu, UpdateMode mode) {
        JMenuItem item = new JMenuItem(mode.displayText);
        item.addActionListener(e -> {
            if (mode == DFU_MANUAL) {
                startDfuRecovery();
                return;
            }
            final AvailableHardware hardware = connectivityContext.getCurrentHardware();
            // PortResult equality omits board metadata; validate and dispatch the current scan object.
            final PortResult selected = currentPort(hardware.getKnownPorts(), (PortResult) comboPorts.getSelectedItem());
            if (!isAdvancedActionAvailable(mode, hardware, selected)) {
                return;
            }
            executeJob(splitButton, mode, selected);
        });
        menu.add(item);
    }

    private boolean isAdvancedActionAvailable(UpdateMode mode, AvailableHardware hardware, @Nullable PortResult selected) {
        if (isJobRunning()) {
            return false;
        }
        switch (mode) {
            case DFU_AUTO:
                return canUseDfuNow() && isSerialEcu(selected)
                    && hardware.isPortAvailable(selected)
                    && hasMatchingLiveConnection(selected, isLiveConnection(),
                        linkManager == null ? null : linkManager.getLastTriedPort());
            case DFU_SWITCH:
                return canUseDfuNow() && isSerialEcu(selected)
                    && hardware.isPortAvailable(selected)
                    && hasMatchingLiveConnection(selected, isLiveConnection(),
                        linkManager == null ? null : linkManager.getLastTriedPort());
            case DFU_ERASE:
            case INSTALL_OPENBLT:
                return canUseDfuNow() && resolveDetectedDfuPort(hardware) != null
                    && (mode != INSTALL_OPENBLT || DfuFlasher.haveBootloaderBinFile());
            case ST_LINK:
                final ProgrammingCapabilities capabilities = capabilityProvider.get();
                return capabilities.windows && !capabilities.requiresBlt() && hardware.isStLinkConnected();
            case OPENBLT_CAN:
                return capabilityProvider.get().windows && hardware.isPCANConnected();
            case OPENBLT_SWITCH:
            case OPENBLT_MANUAL:
                return selected != null && hardware.isPortAvailable(selected)
                    && hasRealSerialPort(java.util.Collections.singletonList(selected));
            case OPENBLT_EMERGENCY_WIPE:
                return selected != null && hardware.isPortAvailable(selected)
                    && isEmergencyWipeAvailable(selected);
            default:
                return false;
        }
    }

    private void addCustomFirmwareMenuItem(JMenu menu) {
        JMenuItem item = new JMenuItem("Pick Custom File [DFU]");
        item.addActionListener(e -> {
            if (!canUseDfuNow() || isJobRunning()
                || resolveDetectedDfuPort(connectivityContext.getCurrentHardware()) == null) {
                return;
            }
            JFileChooser chooser = new JFileChooser();
            chooser.setDialogTitle("Pick DFU firmware");
            chooser.setFileSelectionMode(JFileChooser.FILES_ONLY);
            chooser.setAcceptAllFileFilterUsed(false);
            chooser.setFileFilter(new FileNameExtensionFilter("Firmware binaries (.bin)", "bin"));
            if (chooser.showOpenDialog(splitButton) != JFileChooser.APPROVE_OPTION) {
                return;
            }

            File firmware = chooser.getSelectedFile();
            startCustomDfuRecovery(firmware);
        });
        menu.add(item);
    }

    private static boolean isUnflashableEcu(@Nullable PortResult port) {
        return port != null && (port.isUnsupportedEcu()
            || port.type == SerialPortType.EcuUnknown
            || LinkManager.isCanPort(port.port));
    }

    private static boolean isSerialEcu(@Nullable PortResult port) {
        return port != null && !LinkManager.isCanPort(port.port)
            && (port.type == SerialPortType.Ecu || port.type == SerialPortType.EcuWithOpenblt);
    }

    private static boolean isCanEcu(@Nullable PortResult port) {
        return port != null && isAutomaticCanPort(port.port) && port.isEcu();
    }

    /** CAN transports supported by the OpenBLT flasher. */
    private static boolean isAutomaticCanPort(String port) {
        return LinkManager.PCAN.equals(port) || LinkManager.SOCKET_CAN.equals(port);
    }

}
