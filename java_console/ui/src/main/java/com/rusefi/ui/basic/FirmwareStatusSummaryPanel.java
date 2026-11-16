package com.rusefi.ui.basic;

import com.rusefi.core.io.BundleInfo;
import com.rusefi.maintenance.ProgramSelector;

import javax.swing.*;
import java.awt.*;
import java.awt.event.HierarchyEvent;
import java.awt.geom.Rectangle2D;
import java.time.LocalDate;
import java.time.format.DateTimeFormatter;
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
    private final JTextArea status = valueArea();
    private final JTextArea ecuValue = valueArea();
    private final JLabel installedValue = new JLabel();
    private final JLabel latestValue = new JLabel();
    private final JTextArea branchValue = valueArea();
    private final JTextArea backupNote = valueArea();
    private final Component statusGap = Box.createVerticalStrut(10);
    private boolean wasDisplayable;

    public FirmwareStatusSummaryPanel(FirmwareRollbackController rollbackController, ProgramSelector selector,
                                      JButton branchSwitch, JCheckBox keepTune) {
        this.rollbackController = rollbackController;
        this.selector = selector;
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
        String ecu = firmware == null ? "Not connected" : firmware.getTarget();
        String port = rollbackController.getConnectedPortLabel();
        if (port != null) {
            ecu += " (" + port + ")";
        }
        ecuValue.setText(ecu);

        String branch = firmware == null ? "—" : FirmwareBranchSwitchController.label(firmware.getBranchName());
        boolean isLts = FirmwareRollbackController.isLtsBundle(firmware);
        branchValue.setText(branch + (isLts ? " (LTS)" : ""));
        branchValue.setToolTipText(firmware == null ? null : firmware.getBranchName());
        selector.setPrimaryActionBranch(firmware == null ? null : branch);
        selector.setPrimaryActionStatus("This ECU is up to date.".equals(release.status),
            "Update available".equals(release.status));

        status.setText(release.status);
        installedValue.setText(format(release.installedDate));
        latestValue.setText(format(release.latestDate));
        statusGap.setVisible(!release.status.isEmpty());
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
