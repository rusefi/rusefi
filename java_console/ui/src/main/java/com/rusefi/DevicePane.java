package com.rusefi;

import com.rusefi.core.preferences.storage.PersistentConfiguration;
import com.rusefi.autoupdate.FirmwareRollbackResolver;
import com.rusefi.maintenance.DfuFlasher;
import com.rusefi.maintenance.ProgramSelector;
import com.rusefi.ui.UIContext;
import com.rusefi.ui.basic.FirmwareRollbackController;
import com.rusefi.ui.basic.FirmwareBranchSwitchController;
import com.rusefi.ui.basic.FirmwareStatusSummaryPanel;
import com.rusefi.ui.basic.FirmwareUpdateCard;
import com.rusefi.ui.basic.StartupUpdateActions;
import com.rusefi.ui.basic.MigrateSettingsCheckboxState;
import com.rusefi.ui.basic.SingleAsyncJobExecutor;
import com.rusefi.ui.basic.StatusPanelWithProgressBar;
import com.rusefi.ui.basic.UpdateFirmwareResult;
import com.rusefi.ui.widgets.CommandIcon;
import com.rusefi.ui.widgets.JoinedButtons;
import com.formdev.flatlaf.util.UIScale;
import com.rusefi.ui.widgets.ToolbarStrip;
import com.rusefi.io.LinkManager;
import com.rusefi.binaryprotocol.BinaryProtocol;

import javax.swing.*;
import java.awt.*;
import java.util.Collections;
import java.util.List;
import java.util.Optional;
import java.util.function.Consumer;
import java.util.function.BiFunction;

/**
 * The live console's "Device" tab — a single-session device manager [tag:better_ux_for_flashing].
 * <p>
 * Instead of being frozen with the port/type it was opened with, this tab subscribes to
 * {@link DeviceSessionManager} and re-renders on every hardware/state change, so one running console
 * can hook / remove / re-connect an ECU and flash it via DFU or OpenBLT without a restart. Firmware
 * operations reuse {@link ProgramSelector} (which already builds the DFU/OpenBLT menu from the detected
 * {@link AvailableHardware}). The connection lifecycle itself is owned by the auto-connect watchdog —
 * this tab only offers firmware updates and locks the live tabs while the board isn't a usable ECU.
 */
public class DevicePane {
    static final String ACTIVITY = "Activity";
    static final String COPY_LOG = "Copy log";

    private final JPanel content = new JPanel();
    private final ConnectivityContext connectivityContext;
    private final LinkManager linkManager;
    private final DeviceSessionManager sessionManager;
    private final JTabbedPane tabbedPane;

    private final JComboBox<PortResult> comboPorts = new JComboBox<>();
    private final ProgramSelector selector;
    private final SingleAsyncJobExecutor jobExecutor;
    private final FirmwareRollbackController rollbackController;
    private final FirmwareBranchSwitchController branchSwitchController;
    private final FirmwareStatusSummaryPanel firmwareSummary;
    private final StatusPanelWithProgressBar statusPanel;
    private final JCheckBox autoUpdateBundle = new JCheckBox("Auto-update Software", AutoupdateProperty.get());
    private final JCheckBox migrateSettings = StartupUpdateActions.createMigrateSettingsCheckBox();
    // Previous state rendered on the EDT — used to detect the *transition* into a bootloader state so we
    // pull focus to the Device tab only once, not on every scan cycle [tag:better_ux_for_flashing].
    private SessionState lastRenderedState;

    public DevicePane(final UIContext uiContext, final ConnectivityContext connectivityContext,
                      final DeviceSessionManager sessionManager, final JTabbedPane tabbedPane,
                      final SingleAsyncJobExecutor jobExecutor, final StatusPanelWithProgressBar statusPanel,
                      final Consumer<JComponent> showFullScreenPanel, final Runnable closeFullScreenPanel) {
        this.connectivityContext = connectivityContext;
        this.linkManager = uiContext.getLinkManager();
        this.sessionManager = sessionManager;
        this.tabbedPane = tabbedPane;
        this.jobExecutor = jobExecutor;
        this.statusPanel = statusPanel;

        // Firmware and tune-import jobs share one executor, with output routed to their own status panels.
        // Tab-locking is driven by render() off the session state (which already includes FLASHING as
        // well as the DFU/OpenBLT bootloader states), so no separate job-executor listeners are needed.
        sessionManager.setJobExecutor(jobExecutor);

        this.selector = new ProgramSelector(
            connectivityContext, comboPorts, showFullScreenPanel, closeFullScreenPanel);
        selector.setJobExecutor(jobExecutor);
        selector.setLinkManager(linkManager);
        rollbackController = new FirmwareRollbackController(
            connectivityContext,
            statusPanel,
            jobExecutor,
            () -> Optional.ofNullable((PortResult) comboPorts.getSelectedItem()),
            () -> sessionManager.getState() == SessionState.CONNECTED,
            this::refreshFirmwareControls,
            showFullScreenPanel,
            closeFullScreenPanel);
        rollbackController.setLinkManager(linkManager);
        rollbackController.enableReleaseStatusDiscovery();
        branchSwitchController = new FirmwareBranchSwitchController(
            connectivityContext, statusPanel, jobExecutor,
            () -> Optional.ofNullable((PortResult) comboPorts.getSelectedItem()),
            () -> sessionManager.getState() == SessionState.CONNECTED,
            showFullScreenPanel, closeFullScreenPanel);
        branchSwitchController.setLinkManager(linkManager);
        selector.setTargetedFirmwareUpdateInterceptor(rollbackController::startLatestUpdate);
        selector.setExternalBusySupplier(rollbackController::isDownloadInProgress);
        selector.setExternalPrimaryActionBusySupplier(rollbackController::isPrimaryUpdateBlockedByCatalogDiscovery);
        selector.addFirmwareControl(rollbackController.getRollbackButton());
        firmwareSummary = new FirmwareStatusSummaryPanel(
            rollbackController, selector, branchSwitchController.getButton(), migrateSettings,
            connectivityContext, selector::getSelectedDetectedPort,
            () -> sessionManager.getState() == SessionState.FLASHING
                || !jobExecutor.isNotInProgress() || rollbackController.isDownloadInProgress());
        selector.setMissingIniStateChangedListener(this::refreshFirmwareStatusAfterMissingIniChange);
        rollbackController.addStateChangedListener(() -> SwingUtilities.invokeLater(this::refreshFirmwareControls));

        // No connect/disconnect here — the auto-connect watchdog owns the connection. comboPorts is
        // kept only as a populated model feeding ProgramSelector's flash target. [tag:better_ux_for_flashing]
        content.setLayout(new BorderLayout());

        autoUpdateBundle.setToolTipText("Update this console software automatically when a newer version is available");
        autoUpdateBundle.setOpaque(false);
        autoUpdateBundle.addActionListener(e -> PersistentConfiguration.setBoolProperty(
            AutoupdateProperty.AUTO_UPDATE_BUNDLE_PROPERTY, autoUpdateBundle.isSelected()));

        migrateSettings.setText("Keep my current tune");
        migrateSettings.setToolTipText("A tune backup is attempted before updating, but can fail. If it succeeds, this option controls restoring it afterwards. Check settings before using the ECU.");
        migrateSettings.setOpaque(false);
        migrateSettings.setSelected(true);
        migrateSettings.addActionListener(e -> MigrateSettingsCheckboxState.isMigrationNeeded = migrateSettings.isSelected());
        MigrateSettingsCheckboxState.isMigrationNeeded = migrateSettings.isSelected();

        // Keep ECU identity, update actions, computer-software settings and operation feedback together
        // in one responsive card; the progress/log area must not look like a separate full-width task.
        selector.useProminentCardStyle(CommandIcon.UPLOAD);
        FirmwareUpdateCard card = FirmwareUpdateCard.create("ECU firmware");
        JPanel softwareOptions = new JPanel() {
            @Override
            public void doLayout() {
                String text = autoUpdateBundle.getFontMetrics(autoUpdateBundle.getFont())
                    .stringWidth("Auto-update console software") + UIScale.scale(36) > getWidth()
                    ? "<html>Auto-update console<br>software</html>" : "Auto-update console software";
                if (!text.equals(autoUpdateBundle.getText())) {
                    autoUpdateBundle.setText(text);
                    revalidate();
                    if (getParent() != null) {
                        getParent().revalidate();
                    }
                }
                super.doLayout();
            }
        };
        softwareOptions.setLayout(new BoxLayout(softwareOptions, BoxLayout.Y_AXIS));
        softwareOptions.setOpaque(false);
        softwareOptions.setAlignmentX(Component.LEFT_ALIGNMENT);
        softwareOptions.setMaximumSize(new Dimension(Integer.MAX_VALUE, Integer.MAX_VALUE));
        autoUpdateBundle.setText("Auto-update console software");
        autoUpdateBundle.setAlignmentX(Component.LEFT_ALIGNMENT);
        migrateSettings.setAlignmentX(Component.LEFT_ALIGNMENT);
        softwareOptions.add(autoUpdateBundle);
        firmwareSummary.setAlignmentX(Component.LEFT_ALIGNMENT);
        card.add(firmwareSummary);
        card.add(Box.createVerticalStrut(24));
        card.add(softwareOptions);
        card.add(Box.createVerticalStrut(28));

        // Status log fills the middle under its own header; the StatusPanelWithProgressBar keeps its
        // progress bar at the very bottom of the pane.
        JButton copyLog = new JButton(COPY_LOG);
        copyLog.setToolTipText("Copy the whole log, e.g. to paste into a support request");
        copyLog.addActionListener(e -> statusPanel.copyLogToClipboard());
        JPanel logPanel = new JPanel(new BorderLayout(0, 10));
        logPanel.setOpaque(false);
        logPanel.add(ToolbarStrip.header(ACTIVITY, new JoinedButtons(copyLog)), BorderLayout.NORTH);
        logPanel.add(statusPanel.getContent(), BorderLayout.CENTER);
        logPanel.setAlignmentX(Component.LEFT_ALIGNMENT);
        statusPanel.setActivityVisibilityListener(visible -> {
            logPanel.setVisible(visible);
            card.revalidate();
        });
        logPanel.setVisible(statusPanel.isActivityVisible());
        card.add(logPanel);
        JPanel firmwareViews = FirmwareUpdateCard.createFirmwareTabContent(
            FirmwareUpdateCard.inResponsiveViewport(card), selector, tabbedPane, content);
        content.add(firmwareViews, BorderLayout.CENTER);

        sessionManager.addListener((state, hardware) -> SwingUtilities.invokeLater(() -> render(state, hardware)));
    }

    void render(final SessionState state, final AvailableHardware hardware) {
        if (shouldCancelNoIniConfirmation(lastRenderedState, state)) {
            selector.cancelNoIniUpdateConfirmation();
        }
        final AvailableHardware effectiveHardware = (hardware != null)
            ? hardware
            : connectivityContext.getCurrentHardware();

        // Repopulate the ports combo, preserving the current selection where possible.
        final PortResult previouslySelected = (PortResult) comboPorts.getSelectedItem();
        final List<PortResult> ports = (effectiveHardware != null)
            ? effectiveHardware.getKnownPorts()
            : Collections.emptyList();
        comboPorts.removeAllItems();
        for (final PortResult port : ports) {
            comboPorts.addItem(port);
        }
        reselectPort(ports, previouslySelected, state == SessionState.CONNECTED, connectedPort());

        // ProgramSelector builds the DFU / OpenBLT menu straight from the detected hardware.
        rollbackController.refresh((PortResult) comboPorts.getSelectedItem());
        branchSwitchController.refreshButton();
        selector.apply(effectiveHardware);
        firmwareSummary.refresh();
        final boolean flashing = state == SessionState.FLASHING;
        autoUpdateBundle.setEnabled(!flashing);
        migrateSettings.setEnabled(!flashing && firmwareSummary.isTunePreservationAvailable());
        comboPorts.setEnabled(!flashing);

        // The board is a usable live ECU only in CONNECTED. While it is being flashed or sits in a
        // DFU/OpenBLT bootloader, the live tabs show frozen/stale data — lock them and (on entry) pull
        // the user to the Device tab so the only offered action is a firmware update. [tag:better_ux_for_flashing]
        lockConsoleForState(state);

        lastRenderedState = state;
        content.revalidate();
        content.repaint();
    }

    private void refreshFirmwareControls() {
        selector.apply(connectivityContext.getCurrentHardware());
        rollbackController.refreshButton();
        branchSwitchController.refreshButton();
        firmwareSummary.refresh();
        content.revalidate();
        content.repaint();
    }

    private void refreshFirmwareStatusAfterMissingIniChange() {
        rollbackController.refresh((PortResult) comboPorts.getSelectedItem());
        refreshFirmwareControls();
    }

    private void lockConsoleForState(final SessionState state) {
        final boolean flashing = state == SessionState.FLASHING;
        final boolean bootloaderPresent = isBootloaderState(state);
        setTabsEnabledForState(state);
        if (tabbedPane != null) {
            // Frame blink (#9715) signals an update *in progress* — only while actually flashing, not
            // while merely waiting in a bootloader for the user to start the update.
            tabbedPane.putClientProperty("isUpdating", flashing);
            // Surface the bootloader state to the window title
            tabbedPane.putClientProperty("bootloaderMode",
                state == SessionState.DEVICE_IN_DFU ? "DFU"
                    : state == SessionState.DEVICE_IN_BLT ? "OpenBLT" : null);
            tabbedPane.repaint();
        }
        // Only on the transition into a bootloader state: pull focus to Device once
        // and drop a one-line hint into the status log, which replaces the old
        // status label. [tag:better_ux_for_flashing]
        if (bootloaderPresent && !isBootloaderState(lastRenderedState)) {
            selectDeviceTab();
            statusPanel.clear();
            statusPanel.revealActivity();
            statusPanel.log(bootloaderGuidance(state), true, false);
        } else if (state == SessionState.CONNECTED && lastRenderedState != SessionState.CONNECTED) {
            // Board is a live ECU again (e.g. it rebooted to firmware after a flash). Normally clear the
            // stale "Board is in the … bootloader" hint — but if a firmware update just finished, preserve
            // its (green/red) report so the success/failure survives the reconnect instead of being wiped
            // (issue #9832). consumeLastResult() shows it once; a later unrelated reconnect clears normally.
            if (jobExecutor.consumeLastResult() == UpdateFirmwareResult.NONE) {
                statusPanel.clear();
            }
        }
    }

    static String bootloaderGuidance(final SessionState state) {
        if (state == SessionState.DEVICE_IN_DFU) {
            return DfuFlasher.isDfuProgrammingSupported()
                ? "Your ECU is in recovery mode. Use Recover ECU to choose its board and firmware."
                : "Your ECU is in recovery mode. Recovery is not supported on this platform.";
        }
        return "OpenBLT bootloader detected — click Manual OpenBLT Update to continue.";
    }

    static boolean isBootloaderState(final SessionState state) {
        return state == SessionState.DEVICE_IN_DFU || state == SessionState.DEVICE_IN_BLT;
    }

    static boolean shouldCancelNoIniConfirmation(SessionState previous, SessionState current) {
        return previous != null && previous != current;
    }

    private void selectDeviceTab() {
        if (tabbedPane == null) {
            return;
        }
        for (int i = 0; i < tabbedPane.getTabCount(); i++) {
            if ("Device".equals(tabbedPane.getTitleAt(i))) {
                tabbedPane.setSelectedIndex(i);
                return;
            }
        }
    }

    private String connectedPort() {
        final BinaryProtocol protocol = linkManager.getBinaryProtocol();
        return protocol == null || protocol.isClosed() ? null : linkManager.getLastTriedPort();
    }

    private void reselectPort(final List<PortResult> ports, final PortResult previouslySelected,
                              final boolean liveSession, final String connectedPort) {
        if (liveSession) {
            for (final PortResult port : ports) {
                if (connectedPort != null && port.isEcu() && port.port.equals(connectedPort)) {
                    comboPorts.setSelectedItem(port);
                    return;
                }
            }
            // A live session with no matching detected port is not permission to target another ECU.
            comboPorts.setSelectedItem(null);
            return;
        }
        if (previouslySelected != null) {
            for (final PortResult port : ports) {
                if (port.port.equals(previouslySelected.port)) {
                    comboPorts.setSelectedItem(port);
                    return;
                }
            }
        }
        // No prior selection (or it disappeared): prefer the first connectable ECU.
        for (final PortResult port : ports) {
            if (port.isEcu()) {
                comboPorts.setSelectedItem(port);
                return;
            }
        }
        if (!ports.isEmpty()) {
            comboPorts.setSelectedIndex(0);
        }
    }

    /**
     * Programmatically start a firmware update (used by the "Update ECU" menu shortcut). No-op while a
     * job is already running or when there is no connectable port.
     */
    public void triggerAutoUpdate() {
        if (sessionManager.getState() == SessionState.FLASHING) {
            return;
        }
        selector.triggerUpdateFirmware();
    }

    // Tabs usable without a live ECU: the Device tab (hosts the update controls), offline tune editing
    // ("Tuning") and the pinout reference. Active flashing is stricter: Tuning can write if the old
    // BinaryProtocol is still reachable, so only Device progress and Pinout remain available.
    static boolean isOfflineCapableTab(final String title) {
        return "Device".equals(title) || "Tuning".equals(title) || "Pinout".equals(title)
            || "Manage Tunes".equals(title);
    }

    static boolean isTabEnabled(final String title, final SessionState state) {
        if (state == SessionState.FLASHING) {
            return "Device".equals(title) || "Pinout".equals(title);
        }
        if (isBootloaderState(state)) {
            return isOfflineCapableTab(title);
        }
        return true;
    }

    private void setTabsEnabledForState(final SessionState state) {
        if (tabbedPane == null) {
            return;
        }
        for (int i = 0; i < tabbedPane.getTabCount(); i++) {
            tabbedPane.setEnabledAt(i, isTabEnabled(tabbedPane.getTitleAt(i), state));
        }
    }

    public Component getContent() {
        return content;
    }

    void setDevMetadataProviderForTests(BiFunction<String, String, Long> provider) {
        rollbackController.setDevMetadataProviderForTests(provider);
    }

    void setLtsCatalogProviderForTests(
        BiFunction<String, String, List<FirmwareRollbackResolver.Build>> provider) {
        rollbackController.setCatalogProviderForTests(provider);
    }
}
