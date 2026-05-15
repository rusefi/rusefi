package com.rusefi.ui.basic;

import com.rusefi.ConnectivityContext;
import com.rusefi.binaryprotocol.BinaryProtocol;
import com.rusefi.core.ui.AutoupdateUtil;
import com.rusefi.io.LinkManager;
import org.jetbrains.annotations.Nullable;

import javax.swing.*;
import java.awt.*;
import java.awt.event.ActionEvent;

public class ImportTuneControl implements EnableableControl {
    private final JButton importTuneButton = new JButton(
        LoadTuneHelper.LOAD_TUNE_TEXT,
        AutoupdateUtil.loadIcon("writeconfig48.png")
    );
    private final JButton exportTuneButton = new JButton(
        LoadTuneHelper.SAVE_TUNE_TEXT,
        AutoupdateUtil.loadIcon("download48.jpg")
    );

    private final ImportTuneFileChooser importTune;
    private final ExportTuneFileChooser exportTune;

    private final BasicButtonCoordinator basicButtonCoordinator;
    private final ConnectivityContext connectivityContext;
    @Nullable
    private volatile LinkManager splashLinkManager;

    public ImportTuneControl(SingleAsyncJobExecutor singleAsyncJobExecutor,
                             BasicButtonCoordinator basicButtonCoordinator,
                             ConnectivityContext connectivityContext) {
        this.basicButtonCoordinator = basicButtonCoordinator;
        this.connectivityContext = connectivityContext;
        importTuneButton.addActionListener(this::onImportTuneButtonClicked);
        exportTuneButton.addActionListener(this::onExportTuneButtonClicked);
        importTune = new ImportTuneFileChooser(singleAsyncJobExecutor);
        exportTune = new ExportTuneFileChooser(singleAsyncJobExecutor);
    }

    public void setLinkManager(@Nullable LinkManager lm) {
        this.splashLinkManager = lm;
    }

    @Override
    public void setEnabled(boolean b) {
        importTuneButton.setEnabled(b);
        exportTuneButton.setEnabled(b);
    }

    public Component getContent() {
        JPanel panel = new JPanel(new GridLayout(1, 2));
        panel.add(importTuneButton);
        panel.add(exportTuneButton);
        return panel;
    }

    private void onImportTuneButtonClicked(final ActionEvent actionEvent) {
        basicButtonCoordinator.disableButtons();
        final LinkManager lm = splashLinkManager;
        final BinaryProtocol liveBp = (lm != null) ? lm.getBinaryProtocol() : null;
        if (liveBp != null) {
            importTune.showFileChooserToImportTuneAction(liveBp, lm, importTuneButton, connectivityContext);
        } else if (lm != null) {
            importTune.showFileChooserToImportTuneAction(lm, importTuneButton, connectivityContext);
        } else {
            JOptionPane.showMessageDialog(importTuneButton, "Device is not connected", "Error", JOptionPane.ERROR_MESSAGE);
        }
        basicButtonCoordinator.refreshButtons();
    }

    private void onExportTuneButtonClicked(final ActionEvent actionEvent) {
        basicButtonCoordinator.disableButtons();
        final LinkManager lm = splashLinkManager;
        final BinaryProtocol liveBp = (lm != null) ? lm.getBinaryProtocol() : null;
        if (liveBp != null) {
            exportTune.showFileChooserToExportTuneAction(liveBp, lm, exportTuneButton, connectivityContext);
        } else {
            JOptionPane.showMessageDialog(exportTuneButton, "Device is not connected", "Error", JOptionPane.ERROR_MESSAGE);
        }
        basicButtonCoordinator.refreshButtons();
    }
}
