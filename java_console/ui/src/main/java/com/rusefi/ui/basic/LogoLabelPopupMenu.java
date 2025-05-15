package com.rusefi.ui.basic;

import javax.swing.*;

public class LogoLabelPopupMenu extends JPopupMenu {
    private final JMenuItem uploadTuneMenuItem;
    private final JMenuItem printUnitLabelMenuItem;

    public LogoLabelPopupMenu(
        final Runnable onUploadTuneMenuItemSelected,
        final Runnable onPrintUnitLabelMenuItemSelected,
        final boolean displayInstanceNameAndUploadTuneMenuItems
    ) {
        final JMenuItem instanceNameMenuItem = new JMenuItem("Instance name");
        instanceNameMenuItem.addActionListener(
            e -> InstanceNameEditor.INSTANCE.editInstanceName(instanceNameMenuItem)
        );

        uploadTuneMenuItem = new JMenuItem("Upload Tune");
        uploadTuneMenuItem.setEnabled(false);
        uploadTuneMenuItem.addActionListener(e -> onUploadTuneMenuItemSelected.run());

        printUnitLabelMenuItem = new JMenuItem("Print unit label");
        printUnitLabelMenuItem.setEnabled(false);
        printUnitLabelMenuItem.addActionListener(e -> onPrintUnitLabelMenuItemSelected.run());

        if (displayInstanceNameAndUploadTuneMenuItems) {
            add(uploadTuneMenuItem);
            add(instanceNameMenuItem);
        }
        add(printUnitLabelMenuItem);
    }

    public void refreshUploadTuneAndPrintUnitLabelsMenuItems(final boolean canBeEnabled) {
        final boolean enableUploadTuneMenuItem = (canBeEnabled ? InstanceNameEditor.loadInstanceName() != null : false);
        uploadTuneMenuItem.setEnabled(enableUploadTuneMenuItem);
        printUnitLabelMenuItem.setEnabled(canBeEnabled);
    }
}
