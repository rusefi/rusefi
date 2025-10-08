package com.rusefi.ui.basic;

import com.rusefi.core.ui.ErrorMessageHelper;

import javax.swing.*;
import java.awt.event.ActionEvent;
import java.io.File;

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

        JMenuItem importTune = new JMenuItem("Load Tune");
        add(importTune);
        importTune.addActionListener(new AbstractAction() {
            @Override
            public void actionPerformed(ActionEvent e) {
                JFileChooser fileChooser = new JFileChooser();
                fileChooser.setDialogTitle("Select a File");
                int result = fileChooser.showOpenDialog(ErrorMessageHelper.createOnTopParent());
                if (result == JFileChooser.APPROVE_OPTION) {
                    File selectedFile = fileChooser.getSelectedFile();
                    System.out.println("Selected file: " + selectedFile.getAbsolutePath());
                }
            }
        });

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

    public void refreshUploadTuneAndPrintUnitLabelsMenuItems(
        final boolean isEcuPortDefined,
        final boolean existUnitIdentifierFields
    ) {
        final boolean enableUploadTuneMenuItem = (isEcuPortDefined && InstanceNameEditor.loadInstanceName() != null);
        uploadTuneMenuItem.setEnabled(enableUploadTuneMenuItem);
        printUnitLabelMenuItem.setEnabled(existUnitIdentifierFields);
    }
}
