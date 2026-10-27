package com.rusefi;

import com.rusefi.core.preferences.storage.PersistentConfiguration;
import com.rusefi.maintenance.DfuFlasher;
import com.rusefi.maintenance.ProgramSelector;
import com.rusefi.ui.UIContext;
import com.rusefi.ui.basic.FirmwareRollbackController;
import com.rusefi.ui.basic.MigrateSettingsCheckboxState;
import com.rusefi.ui.basic.SingleAsyncJobExecutor;
import com.rusefi.ui.basic.StatusPanelWithProgressBar;
import com.rusefi.ui.basic.UpdateFirmwareResult;
import com.rusefi.ui.widgets.CommandIcon;
import com.rusefi.ui.widgets.JoinedButtons;
import com.rusefi.ui.widgets.ToolbarStrip;

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
    static final String ACTIVITY = "Activity";
    static final String COPY_LOG = "Copy log";

    /**
     * Lays out two children at their preferred sizes, vertically centred: the first in the middle of the
     * whole width, the second at the right edge. When the window is too narrow for both, the first moves
     * left so it never covers the second.
     */
    static final class CenteredLayout implements LayoutManager {
        @Override
        public void layoutContainer(Container parent) {
            Insets insets = parent.getInsets();
            int height = parent.getHeight() - insets.top - insets.bottom;
            int right = parent.getWidth() - insets.right;
            if (parent.getComponentCount() > 1) {
                Component trailing = parent.getComponent(1);
                Dimension size = trailing.getPreferredSize();
                right -= size.width;
                trailing.setBounds(right, insets.top + (height - size.height) / 2, size.width, size.height);
                right -= JoinedButtons.ROW_GAP;
            }
            if (parent.getComponentCount() > 0) {
                Component middle = parent.getComponent(0);
                Dimension size = middle.getPreferredSize();
                int x = Math.max(insets.left, Math.min((parent.getWidth() - size.width) / 2, right - size.width));
                middle.setBounds(x, insets.top + (height - size.height) / 2, size.width, size.height);
            }
        }

        @Override
        public Dimension preferredLayoutSize(Container parent) {
            Insets insets = parent.getInsets();
            int width = 0;
            int height = 0;
            for (Component child : parent.getComponents()) {
                Dimension size = child.getPreferredSize();
                width += size.width + JoinedButtons.ROW_GAP;
                height = Math.max(height, size.height);
            }
            return new Dimension(width + insets.left + insets.right, height + insets.top + insets.bottom);
        }

        @Override
        public Dimension minimumLayoutSize(Container parent) {
            return preferredLayoutSize(parent);
        }

        @Override
        public void addLayoutComponent(String name, Component comp) {
        }

        @Override
        public void removeLayoutComponent(Component comp) {
        }
    }
    private final JPanel content = new JPanel();
    private final ConnectivityContext connectivityContext;
    private final DeviceSessionManager sessionManager;
    private final JTabbedPane tabbedPane;

    private final JComboBox<PortResult> comboPorts = new JComboBox<>();
    private final ProgramSelector selector;
    private final SingleAsyncJobExecutor jobExecutor;
    private final FirmwareRollbackController rollbackController;
    private final StatusPanelWithProgressBar statusPanel;
    private final JCheckBox autoUpdateBundle = new JCheckBox("Auto-update Software", AutoupdateProperty.get());
    private final JCheckBox migrateSettings = new JCheckBox("Try to keep my current tune", true);
    // Previous state rendered on the EDT — used to detect the *transition* into a bootloader state so we
    // pull focus to the Device tab only once, not on every scan cycle [tag:better_ux_for_flashing].
    private SessionState lastRenderedState;

    public DevicePane(final UIContext uiContext, final ConnectivityContext connectivityContext,
                      final DeviceSessionManager sessionManager, final JTabbedPane tabbedPane,
                      final SingleAsyncJobExecutor jobExecutor, final StatusPanelWithProgressBar statusPanel,
                      final Consumer<JComponent> showFullScreenPanel, final Runnable closeFullScreenPanel) {
        this.connectivityContext = connectivityContext;
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
        selector.setLinkManager(uiContext.getLinkManager());
        rollbackController = new FirmwareRollbackController(
            connectivityContext,
            statusPanel,
            jobExecutor,
            () -> Optional.ofNullable((PortResult) comboPorts.getSelectedItem()),
            () -> sessionManager.getState() == SessionState.CONNECTED,
            this::refreshFirmwareControls,
            showFullScreenPanel,
            closeFullScreenPanel);
        rollbackController.setLinkManager(uiContext.getLinkManager());
        selector.setTargetedFirmwareUpdateInterceptor(rollbackController::startLatestUpdate);
        selector.setExternalBusySupplier(rollbackController::isBusy);
        selector.addFirmwareControl(rollbackController.getRollbackButton());

        // Compact controls pinned to the top; the status log + progress bar fill the rest and the bar
        // sits at the bottom of the pane (as elsewhere in the console). No connect/disconnect here — the
        // auto-connect watchdog owns the connection; this tab is just firmware updates. comboPorts is
        // kept only as a populated *model* feeding ProgramSelector's flash target (no visible picker
        // needed for the single-board case). [tag:better_ux_for_flashing]
        content.setLayout(new BorderLayout());

        autoUpdateBundle.setToolTipText("Update this console software automatically when a newer version is available");
        autoUpdateBundle.setOpaque(false);
        autoUpdateBundle.addActionListener(e -> PersistentConfiguration.setBoolProperty(
            AutoupdateProperty.AUTO_UPDATE_BUNDLE_PROPERTY, autoUpdateBundle.isSelected()));

        migrateSettings.setToolTipText("Carry the current tune over to the new firmware when updating");
        migrateSettings.setOpaque(false);
        migrateSettings.addActionListener(e -> MigrateSettingsCheckboxState.isMigrationNeeded = migrateSettings.isSelected());
        MigrateSettingsCheckboxState.isMigrationNeeded = true;

        // toolbar in the Gauges/Lua style [tag:console_theme]: the update button in the middle, where
        // it always was, update options and the how-to link on the right
        selector.useToolbarStyle(CommandIcon.UPLOAD);
        JPanel actions = new JPanel(new FlowLayout(FlowLayout.LEFT, JoinedButtons.ROW_GAP, 0));
        actions.setOpaque(false);
        actions.add(selector.getControl());
        // GridBagLayout centres the options vertically in the toolbar
        JPanel options = new JPanel(new GridBagLayout());
        options.setOpaque(false);
        GridBagConstraints c = new GridBagConstraints();
        c.insets = new Insets(0, 0, 0, 16);
        options.add(autoUpdateBundle, c);
        options.add(migrateSettings, c);
        c.insets = new Insets(0, 0, 0, JoinedButtons.ROW_GAP);
        options.add(ProgramSelector.createHelpButton(), c);
        JPanel toolbar = new ToolbarStrip(new CenteredLayout());
        toolbar.setBorder(BorderFactory.createEmptyBorder(8, 0, 9, JoinedButtons.ROW_END_INSET - JoinedButtons.ROW_GAP));
        toolbar.add(actions);
        toolbar.add(options);
        content.add(toolbar, BorderLayout.NORTH);

        // Status log fills the middle under its own header; the StatusPanelWithProgressBar keeps its
        // progress bar at the very bottom of the pane.
        JButton copyLog = new JButton(COPY_LOG);
        copyLog.setToolTipText("Copy the whole log, e.g. to paste into a support request");
        copyLog.addActionListener(e -> statusPanel.copyLogToClipboard());
        JPanel logPanel = new JPanel(new BorderLayout());
        logPanel.add(ToolbarStrip.header(ACTIVITY, new JoinedButtons(copyLog)), BorderLayout.NORTH);
        logPanel.add(statusPanel.getContent(), BorderLayout.CENTER);
        content.add(logPanel, BorderLayout.CENTER);

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
            return DfuFlasher.isDfuProgrammingSupported()
                ? "Your ECU is in recovery mode. Use Recover ECU to choose its board and firmware."
                : "Your ECU is in recovery mode. Recovery is not supported on this platform.";
        }
        return "Bootloader detected — click Update ECU firmware to continue.";
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
}
