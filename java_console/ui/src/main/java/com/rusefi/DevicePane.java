package com.rusefi;

import com.rusefi.core.preferences.storage.PersistentConfiguration;
import com.rusefi.maintenance.ProgramSelector;
import com.rusefi.ui.UIContext;
import com.rusefi.ui.basic.FirmwareRollbackController;
import com.rusefi.ui.basic.MigrateSettingsCheckboxState;
import com.rusefi.ui.basic.SingleAsyncJobExecutor;
import com.rusefi.ui.basic.StatusPanelWithProgressBar;
import com.rusefi.ui.basic.UpdateFirmwareResult;

import javax.swing.*;
import java.awt.*;
import java.util.Collections;
import java.util.List;
import java.util.Optional;
import java.util.function.Consumer;

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
    private final JPanel content = new JPanel();
    private final ConnectivityContext connectivityContext;
    private final DeviceSessionManager sessionManager;
    private final JTabbedPane tabbedPane;

    private final JComboBox<PortResult> comboPorts = new JComboBox<>();
    private final ProgramSelector selector;
    private final SingleAsyncJobExecutor jobExecutor;
    private final FirmwareRollbackController rollbackController;
    private final StatusPanelWithProgressBar statusPanel = new StatusPanelWithProgressBar();
    private final JCheckBox autoUpdateBundle = new JCheckBox("Auto-update Software", AutoupdateProperty.get());
    private final JCheckBox migrateSettings = new JCheckBox("Migrate Settings", true);
    // Previous state rendered on the EDT — used to detect the *transition* into a bootloader state so we
    // pull focus to the Device tab only once, not on every scan cycle [tag:better_ux_for_flashing].
    private SessionState lastRenderedState;

    public DevicePane(final UIContext uiContext, final ConnectivityContext connectivityContext,
                      final DeviceSessionManager sessionManager, final JTabbedPane tabbedPane,
                      final Consumer<JComponent> showRollbackPicker, final Runnable closeRollbackPicker) {
        this.connectivityContext = connectivityContext;
        this.sessionManager = sessionManager;
        this.tabbedPane = tabbedPane;

        // Firmware jobs run on a per-tab single executor whose output is the status/progress panel.
        // Tab-locking is driven by render() off the session state (which already includes FLASHING as
        // well as the DFU/OpenBLT bootloader states), so no separate job-executor listeners are needed.
        this.jobExecutor = new SingleAsyncJobExecutor(statusPanel);
        sessionManager.setJobExecutor(jobExecutor);

        this.selector = new ProgramSelector(connectivityContext, comboPorts);
        selector.setJobExecutor(jobExecutor);
        selector.setLinkManager(uiContext.getLinkManager());
        rollbackController = new FirmwareRollbackController(
            connectivityContext,
            statusPanel,
            jobExecutor,
            () -> Optional.ofNullable((PortResult) comboPorts.getSelectedItem()),
            () -> sessionManager.getState() == SessionState.CONNECTED,
            this::refreshFirmwareControls,
            showRollbackPicker,
            closeRollbackPicker);
        rollbackController.setLinkManager(uiContext.getLinkManager());
        selector.setFirmwareUpdateInterceptor(rollbackController::startLatestUpdate);
        selector.setExternalBusySupplier(rollbackController::isBusy);
        selector.addFirmwareControl(rollbackController.getRollbackButton());

        // Compact controls pinned to the top; the status log + progress bar fill the rest and the bar
        // sits at the bottom of the pane (as elsewhere in the console). No connect/disconnect here — the
        // auto-connect watchdog owns the connection; this tab is just firmware updates. comboPorts is
        // kept only as a populated *model* feeding ProgramSelector's flash target (no visible picker
        // needed for the single-board case). [tag:better_ux_for_flashing]
        content.setLayout(new BorderLayout());
        JPanel column = new JPanel();
        column.setLayout(new BoxLayout(column, BoxLayout.Y_AXIS));
        content.add(column, BorderLayout.NORTH);

        autoUpdateBundle.setAlignmentX(Component.LEFT_ALIGNMENT);
        autoUpdateBundle.addActionListener(e -> PersistentConfiguration.setBoolProperty(
            AutoupdateProperty.AUTO_UPDATE_BUNDLE_PROPERTY, autoUpdateBundle.isSelected()));
        column.add(autoUpdateBundle);

        migrateSettings.setAlignmentX(Component.LEFT_ALIGNMENT);
        migrateSettings.addActionListener(e -> MigrateSettingsCheckboxState.isMigrationNeeded = migrateSettings.isSelected());
        MigrateSettingsCheckboxState.isMigrationNeeded = true;
        column.add(migrateSettings);

        JComponent selectorControl = selector.getControl();
        selectorControl.setAlignmentX(Component.LEFT_ALIGNMENT);
        column.add(selectorControl);

        // Status log fills the middle; the StatusPanelWithProgressBar keeps its progress bar at the
        // very bottom of the pane.
        content.add(statusPanel.getContent(), BorderLayout.CENTER);

        sessionManager.addListener((state, hardware) -> SwingUtilities.invokeLater(() -> render(state, hardware)));
    }

    private void render(final SessionState state, final AvailableHardware hardware) {
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
        reselectPort(ports, previouslySelected);

        // ProgramSelector builds the DFU / OpenBLT menu straight from the detected hardware.
        rollbackController.refresh((PortResult) comboPorts.getSelectedItem());
        selector.apply(effectiveHardware);
        final boolean flashing = state == SessionState.FLASHING;
        autoUpdateBundle.setEnabled(!flashing);
        migrateSettings.setEnabled(!flashing);
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
        content.revalidate();
        content.repaint();
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
            // DFU flashing is Windows-only in rusEFI (STM32_Programmer_CLI.exe); elsewhere it's a dead end.
            return FileLog.isWindows()
                ? "Board is in the DFU bootloader — click Update Firmware to flash."
                : "Board is in the DFU bootloader. DFU flashing requires Windows — power-cycle to exit, or use OpenBLT.";
        }
        return "Board is in the OpenBLT bootloader — click Update Firmware to flash.";
    }

    static boolean isBootloaderState(final SessionState state) {
        return state == SessionState.DEVICE_IN_DFU || state == SessionState.DEVICE_IN_BLT;
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

    private void reselectPort(final List<PortResult> ports, final PortResult previouslySelected) {
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
        return "Device".equals(title) || "Tuning".equals(title) || "Pinout".equals(title);
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
}
