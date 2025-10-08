package com.rusefi.ui.basic;

import com.rusefi.CompatibilityOptional;
import com.rusefi.ConnectivityContext;
import com.rusefi.PortResult;
import com.rusefi.core.ui.AutoupdateUtil;

import javax.swing.*;
import java.awt.*;
import java.awt.event.ActionEvent;
import java.util.Optional;
import java.util.concurrent.atomic.AtomicReference;

public class ImportTuneControl implements EnableableControl {
    private final JButton importTuneButton = new JButton(
        "Load Tune From File...",
        AutoupdateUtil.loadIcon("writeconfig48.png")
    );

    private final ImportTuneFileChooser importTune;

    private final BasicButtonCoordinator basicButtonCoordinator;
    private final ConnectivityContext connectivityContext;
    private final AtomicReference<Optional<PortResult>> ecuPortToUse;

    public ImportTuneControl(SingleAsyncJobExecutor singleAsyncJobExecutor,
                             BasicButtonCoordinator basicButtonCoordinator,
                             ConnectivityContext connectivityContext,
                             AtomicReference<Optional<PortResult>> ecuPortToUse) {
        this.basicButtonCoordinator = basicButtonCoordinator;
        this.connectivityContext = connectivityContext;
        this.ecuPortToUse = ecuPortToUse;
        importTuneButton.addActionListener(this::onImportTuneButtonClicked);
        importTune = new ImportTuneFileChooser(singleAsyncJobExecutor);
    }

    @Override
    public void setEnabled(boolean b) {
        importTuneButton.setEnabled(b);
    }

    public Component getContent() {
        return importTuneButton;
    }

    private void onImportTuneButtonClicked(final ActionEvent actionEvent) {
        basicButtonCoordinator.disableButtons();
        CompatibilityOptional.ifPresentOrElse(ecuPortToUse.get(),
            port -> {
                importTune.showFileChooserToImportTuneAction(port, importTuneButton, connectivityContext);
            }, () -> {
                JOptionPane.showMessageDialog(
                    importTuneButton,
                    "Device is not connected",
                    "Error",
                    JOptionPane.ERROR_MESSAGE
                );
            }
        );
        basicButtonCoordinator.refreshButtons();
    }
}
