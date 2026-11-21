package com.rusefi.ui.basic;

import com.rusefi.core.io.BundleInfo;
import com.rusefi.core.RusEfiSignature;
import com.rusefi.core.SignatureHelper;
import com.rusefi.ConnectivityContext;
import com.rusefi.PortResult;
import com.rusefi.SerialPortType;
import com.rusefi.io.LinkManager;
import com.rusefi.maintenance.ProgramSelector;
import org.jetbrains.annotations.Nullable;

import javax.swing.*;
import java.awt.*;
import java.awt.event.HierarchyEvent;
import java.awt.geom.Rectangle2D;
import java.time.LocalDate;
import java.time.format.DateTimeFormatter;
import java.util.List;
import java.util.Objects;
import java.util.function.BooleanSupplier;
import java.util.function.Supplier;
import javax.swing.text.BadLocationException;

/** Shared live ECU identity and release status used by startup and Device firmware controls. */
public final class FirmwareStatusSummaryPanel extends JPanel {
    private static final DateTimeFormatter DATE_FORMAT = DateTimeFormatter.ofPattern("yyyy.MM.dd");
    public static final String PRIMARY_ACTION_HINT = "The main action targets the ECU's current firmware branch. "
        + "The arrow opens programming and recovery options.";
    private static final String RELEASE_STATUS_HINT = "The update check reports whether a newer build can be confirmed. "
        + "If it can't, the firmware may still be valid; try again later.";
    private final FirmwareRollbackController rollbackController;
    private final ProgramSelector selector;
    private final ConnectivityContext connectivityContext;
    private final Supplier<PortResult> selectedPort;
    private final BooleanSupplier tunePreservationBusy;
    private final JCheckBox keepTune;
    private final JTextArea status = valueArea();
    private final JTextArea ecuValue = valueArea();
    private final JLabel installedValue = new JLabel();
    private final JLabel latestValue = new JLabel();
    private final JTextArea branchValue = valueArea();
    private final JTextArea backupNote = valueArea();
    private final Component statusGap = Box.createVerticalStrut(10);
    private boolean wasDisplayable;

    public FirmwareStatusSummaryPanel(FirmwareRollbackController rollbackController, ProgramSelector selector,
                                      JButton branchSwitch, JCheckBox keepTune,
                                      ConnectivityContext connectivityContext, Supplier<PortResult> selectedPort,
                                      BooleanSupplier tunePreservationBusy) {
        this.rollbackController = rollbackController;
        this.selector = selector;
        this.connectivityContext = connectivityContext;
        this.selectedPort = selectedPort;
        this.keepTune = keepTune;
        this.tunePreservationBusy = tunePreservationBusy;
        ecuValue.setColumns(1);
        branchValue.setColumns(1);
        setLayout(new BoxLayout(this, BoxLayout.Y_AXIS));
        setBorder(BorderFactory.createEmptyBorder());
        setOpaque(false);
        setAlignmentX(LEFT_ALIGNMENT);

        status.setAlignmentX(LEFT_ALIGNMENT);
        status.setToolTipText(RELEASE_STATUS_HINT);
        branchSwitch.setText("Switch firmware branch");
        branchSwitch.setToolTipText("Switch this connected ECU to another supported firmware branch");
        Font buttonFont = branchSwitch.getFont();
        branchSwitch.setFont(buttonFont);
        branchSwitch.setMargin(new Insets(com.formdev.flatlaf.util.UIScale.scale(6),
            com.formdev.flatlaf.util.UIScale.scale(10), com.formdev.flatlaf.util.UIScale.scale(6),
            com.formdev.flatlaf.util.UIScale.scale(10)));
        selector.addPrimaryFirmwareControl(branchSwitch);
        IdentityRows identityRows = new IdentityRows();
        identityRows.addRow("ECU", ecuValue);
        identityRows.addRow("Branch", branchValue);
        identityRows.addRow("Installed firmware", installedValue);
        identityRows.addRow("Latest firmware", latestValue);

        selector.getControl().setAlignmentX(LEFT_ALIGNMENT);
        selector.getControl().setToolTipText(PRIMARY_ACTION_HINT);
        keepTune.setOpaque(false);
        keepTune.setToolTipText("A tune backup is attempted before updating, but can fail. If it succeeds, this option controls restoring it afterwards. Check settings before using the ECU.");
        keepTune.setBorder(BorderFactory.createEmptyBorder(4, 0, 4, 0));
        keepTune.setAlignmentX(LEFT_ALIGNMENT);
        backupNote.setText("A tune backup is attempted first, but may fail. You’ll be told if it does.");
        backupNote.setForeground(field("Tune backup").getForeground());
        backupNote.setFont(backupNote.getFont().deriveFont(Math.max(11f, backupNote.getFont().getSize2D() - 1f)));
        backupNote.setAlignmentX(LEFT_ALIGNMENT);

        add(status);
        add(Box.createVerticalStrut(12));
        add(identityRows);
        add(statusGap);
        add(keepTune);
        add(backupNote);
        add(Box.createVerticalStrut(8));
        add(selector.getControl());

        addHierarchyListener(event -> {
            if ((event.getChangeFlags() & HierarchyEvent.SHOWING_CHANGED) != 0) {
                rollbackController.setDevStatusViewActive(isShowing());
            }
            if ((event.getChangeFlags() & HierarchyEvent.DISPLAYABILITY_CHANGED) != 0) {
                if (isDisplayable()) {
                    wasDisplayable = true;
                } else if (wasDisplayable) {
                    SwingUtilities.invokeLater(() -> {
                        if (!isDisplayable()) {
                            rollbackController.dispose();
                        }
                    });
                }
            }
        });
        refresh();
    }

    private static JLabel field(String label) {
        JLabel component = new JLabel(label);
        component.setForeground(UIManager.getColor("Label.disabledForeground") != null
            ? UIManager.getColor("Label.disabledForeground") : component.getForeground());
        return component;
    }

    private static JTextArea valueArea() {
        JTextArea area = new JTextArea("", 1, 28);
        area.setEditable(false);
        area.setFocusable(false);
        area.setLineWrap(true);
        area.setWrapStyleWord(true);
        area.setOpaque(false);
        area.setBorder(BorderFactory.createEmptyBorder());
        return area;
    }

    public void refresh() {
        BundleInfo firmware = rollbackController.getConnectedFirmware();
        FirmwareRollbackController.ReleaseDisplay release = rollbackController.getReleaseDisplay();
        MissingIniIdentity missingIni = firmware == null ? missingIniIdentity() : null;
        PortResult bootloader = selectedBootloader();
        status.setToolTipText(RELEASE_STATUS_HINT);
        String ecu;
        String branch;
        String statusText;
        String installedText;
        String latestText;
        if (bootloader != null) {
            com.rusefi.updater.OpenbltDetectorStrategy.OpenbltInfo info = bootloader.bootloaderInfo;
            String version = info == null || info.version == null || info.version.trim().isEmpty()
                ? "version unavailable" : "v" + info.version;
            String board = info == null || info.board == null || info.board.trim().isEmpty()
                ? "board unavailable" : info.board;
            ecu = "OpenBLT bootloader " + version + " — " + board + " (" + displayPort(bootloader.port) + ")";
            branch = "Unavailable in bootloader mode";
            statusText = "OpenBLT bootloader detected. Manual OpenBLT Update is available; application branch and "
                + "firmware dates are unavailable until the ECU is running application firmware.";
            status.setToolTipText("Application release status is unavailable while the ECU is in OpenBLT.");
            installedText = "Not available in bootloader mode";
            latestText = "Not available in bootloader mode";
            keepTune.setEnabled(false);
            keepTune.setToolTipText("Your preference is preserved for normal updates. A manual OpenBLT update may "
                + "restore a tune backed up earlier in this session, independently of this option. A fresh tune "
                + "backup is unavailable in bootloader mode.");
            backupNote.setText("A fresh tune backup is unavailable in bootloader mode. Manual updates may restore "
                + "a tune backed up earlier in this session, independently of this option.");
        } else if (missingIni != null) {
            RusEfiSignature signature = missingIni.signature;
            ecu = "Detected " + signature.getBundleTarget() + " (" + displayPort(missingIni.port.port)
                + ") — matching INI unavailable";
            branch = FirmwareBranchSwitchController.label(signature.getBranch());
            statusText = "ECU detected; matching INI unavailable, so tune backup and restore are disabled. "
                + release.status;
            installedText = format(FirmwareRollbackController.parseSignatureDate(signature));
            BundleInfo fallbackFirmware = signature.asBundleInfo();
            latestText = release.latestDate != null ? format(release.latestDate)
                : FirmwareRollbackController.isLtsBundle(fallbackFirmware)
                    || FirmwareRollbackController.isDevelopmentBundle(fallbackFirmware)
                    ? "Unknown" : "Not available for this firmware branch";
            keepTune.setEnabled(false);
            keepTune.setToolTipText("Tune backup and restore are unavailable without the matching INI. "
                + "Your preference is preserved for the next compatible ECU.");
            backupNote.setText("The tune cannot be backed up or restored without the matching INI. "
                + "The ECU will use firmware defaults after the update.");
        } else {
            ecu = firmware == null ? "Not connected" : firmware.getTarget();
            String port = rollbackController.getConnectedPortLabel();
            if (port != null) {
                ecu += " (" + port + ")";
            }
            branch = firmware == null ? "—" : FirmwareBranchSwitchController.label(firmware.getBranchName());
            statusText = release.status;
            installedText = format(release.installedDate);
            latestText = format(release.latestDate);
            keepTune.setEnabled(isTunePreservationAvailable());
            keepTune.setToolTipText("A tune backup is attempted before updating, but can fail. "
                + "If it succeeds, this option controls restoring it afterwards. Check settings before using the ECU.");
            backupNote.setText("A tune backup is attempted first, but may fail. You’ll be told if it does.");
        }
        ecuValue.setText(ecu);

        boolean isLts = bootloader == null && FirmwareRollbackController.isLtsBundle(
            firmware != null ? firmware : missingIni == null ? null : missingIni.signature.asBundleInfo());
        branchValue.setText(branch + (isLts ? " (LTS)" : ""));
        branchValue.setToolTipText(bootloader != null ? null : missingIni != null
            ? missingIni.signature.getBranch() : firmware == null ? null : firmware.getBranchName());
        selector.setPrimaryActionBranch(firmware == null ? null : branch);
        selector.setPrimaryActionStatus(isUpToDateReleaseStatus(release.status),
            isUpdateAvailableReleaseStatus(release.status));

        status.setText(statusText);
        installedValue.setText(installedText);
        latestValue.setText(latestText);
        statusGap.setVisible(!statusText.isEmpty());
    }

    public boolean isTunePreservationAvailable() {
        return !tunePreservationBusy.getAsBoolean()
            && selectedBootloader() == null
            && (rollbackController.getConnectedFirmware() != null || missingIniIdentity() == null);
    }

    static boolean isUpdateAvailableReleaseStatus(String releaseStatus) {
        return "Update available".equals(releaseStatus);
    }

    private static boolean isUpToDateReleaseStatus(String releaseStatus) {
        return "This ECU is up to date.".equals(releaseStatus);
    }

    private MissingIniIdentity missingIniIdentity() {
        PortResult selected = selectedPort.get();
        if (selected == null || (selected.type != SerialPortType.Ecu && selected.type != SerialPortType.EcuWithOpenblt)
            || LinkManager.isCanPort(selected.port)) {
            return null;
        }
        com.rusefi.AvailableHardware hardware = connectivityContext.getCurrentHardware();
        if (hardware == null) {
            return null;
        }
        List<PortResult> detected = hardware.getKnownPorts();
        boolean stillDetected = detected.stream().anyMatch(port ->
            port.type == selected.type && port.port.equals(selected.port));
        com.rusefi.MissingIniRecoveryState.Failure failure =
            connectivityContext.getMissingIniRecoveryState().get();
        if (!stillDetected || failure == null || !Objects.equals(failure.port, selected.port)) {
            return null;
        }
        RusEfiSignature signature = SignatureHelper.parse(failure.signature);
        return signature == null ? null : new MissingIniIdentity(selected, signature);
    }

    private @Nullable PortResult selectedBootloader() {
        PortResult selected = selectedPort.get();
        if (selected == null || selected.type != SerialPortType.OpenBlt) {
            return null;
        }
        com.rusefi.AvailableHardware hardware = connectivityContext.getCurrentHardware();
        if (hardware == null) {
            return null;
        }
        return hardware.getKnownPorts().stream().filter(port ->
            port.type == SerialPortType.OpenBlt && port.port.equals(selected.port)).findFirst().orElse(null);
    }

    private static String displayPort(String port) {
        return port.startsWith("/dev/") ? port.substring("/dev/".length()) : port;
    }

    private static final class MissingIniIdentity {
        private final PortResult port;
        private final RusEfiSignature signature;

        private MissingIniIdentity(PortResult port, RusEfiSignature signature) {
            this.port = port;
            this.signature = signature;
        }
    }

    private static String format(LocalDate date) {
        return date == null ? "Unknown" : DATE_FORMAT.format(date);
    }

    @Override
    public void doLayout() {
        if (getWidth() > 0) {
            updateRows(status, getWidth());
            updateRows(backupNote, getWidth());
        }
        super.doLayout();
        if (ecuValue.getWidth() > 0) {
            updateRows(ecuValue, ecuValue.getWidth());
        }
        if (branchValue.getWidth() > 0) {
            updateRows(branchValue, branchValue.getWidth());
        }
    }

    private void updateRows(JTextArea area, int width) {
        area.setSize(width, Math.max(1, area.getHeight()));
        try {
            Rectangle2D end = area.modelToView2D(area.getDocument().getLength());
            int lineHeight = area.getFontMetrics(area.getFont()).getHeight();
            int rows = Math.max(1, (int) Math.ceil(end.getMaxY() / lineHeight));
            if (area.getRows() != rows) {
                area.setRows(rows);
                invalidate();
                revalidate();
            }
        } catch (BadLocationException e) {
            throw new IllegalStateException(e);
        }
    }

    private static final class IdentityRows extends JPanel {
        private final GridBagLayout grid = new GridBagLayout();
        private final java.util.List<JLabel> labels = new java.util.ArrayList<>();
        private final java.util.List<JComponent> values = new java.util.ArrayList<>();
        private boolean stacked;

        private IdentityRows() {
            setLayout(grid);
            setOpaque(false);
            setAlignmentX(LEFT_ALIGNMENT);
        }

        private void addRow(String labelText, JComponent value) {
            JLabel label = field(labelText);
            labels.add(label);
            values.add(value);
            add(label);
            add(value);
            configureConstraints();
        }

        @Override
        public Dimension getPreferredSize() {
            return grid.preferredLayoutSize(this);
        }

        @Override
        public void doLayout() {
            boolean nextStacked = getWidth() < inlinePreferredWidth();
            if (stacked != nextStacked) {
                stacked = nextStacked;
                configureConstraints();
                if (getParent() != null) {
                    getParent().revalidate();
                }
            }
            grid.layoutContainer(this);
        }

        private void configureConstraints() {
            int gap = com.formdev.flatlaf.util.UIScale.scale(4);
            int columnGap = com.formdev.flatlaf.util.UIScale.scale(12);
            for (int i = 0; i < labels.size(); i++) {
                GridBagConstraints label = new GridBagConstraints();
                label.gridx = 0;
                label.gridy = stacked ? i * 2 : i;
                label.anchor = stacked ? GridBagConstraints.WEST : GridBagConstraints.BASELINE_LEADING;
                label.insets = new Insets(0, 0, stacked ? 0 : gap, stacked ? 0 : columnGap);
                label.gridwidth = stacked ? GridBagConstraints.REMAINDER : 1;
                grid.setConstraints(labels.get(i), label);

                GridBagConstraints value = new GridBagConstraints();
                value.gridx = stacked ? 0 : 1;
                value.gridy = stacked ? i * 2 + 1 : i;
                value.weightx = 1;
                value.fill = GridBagConstraints.HORIZONTAL;
                value.anchor = stacked ? GridBagConstraints.WEST : GridBagConstraints.BASELINE_LEADING;
                value.insets = new Insets(0, 0, gap, 0);
                value.gridwidth = GridBagConstraints.REMAINDER;
                grid.setConstraints(values.get(i), value);
            }
        }

        private int inlinePreferredWidth() {
            int labelWidth = 0;
            int valueWidth = 0;
            for (int i = 0; i < labels.size(); i++) {
                labelWidth = Math.max(labelWidth, labels.get(i).getPreferredSize().width);
                valueWidth = Math.max(valueWidth, textWidth(values.get(i)));
            }
            return labelWidth + com.formdev.flatlaf.util.UIScale.scale(12) + valueWidth;
        }

        private static int textWidth(JComponent value) {
            FontMetrics metrics = value.getFontMetrics(value.getFont());
            if (value instanceof JTextArea) {
                int width = 0;
                for (String line : ((JTextArea) value).getText().split("\\n", -1)) {
                    width = Math.max(width, metrics.stringWidth(line));
                }
                return width;
            }
            return value.getPreferredSize().width;
        }
    }

}
