package com.rusefi.ui.basic;

import javax.swing.*;

public class LogoLabelPopupMenu extends JPopupMenu {
    private final JMenuItem uploadTuneMenuItem;

    public LogoLabelPopupMenu(final Runnable onUploadTuneMenuItemSelected) {
        JMenuItem instanceNameMenuItem = new JMenuItem("Instance name");
        instanceNameMenuItem.addActionListener(
            e -> InstanceNameEditor.INSTANCE.editInstanceName(instanceNameMenuItem)
        );

        uploadTuneMenuItem = new JMenuItem("Upload Tune");
        uploadTuneMenuItem.setEnabled(false);
        uploadTuneMenuItem.addActionListener(e -> onUploadTuneMenuItemSelected.run());

        add(uploadTuneMenuItem);
        add(instanceNameMenuItem);
    }

    public void refreshUploadTuneMenuItem(final boolean canBeEnabled) {
        final boolean enableUploadTuneMenuItem = (canBeEnabled ? InstanceNameEditor.loadInstanceName() != null : false);
        uploadTuneMenuItem.setEnabled(enableUploadTuneMenuItem);
    }
}
