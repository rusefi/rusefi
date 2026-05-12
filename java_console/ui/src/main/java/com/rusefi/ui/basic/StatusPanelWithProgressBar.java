package com.rusefi.ui.basic;

import com.rusefi.io.UpdateOperationCallbacks;
import com.rusefi.ui.widgets.StatusPanel;

import javax.swing.*;
import java.awt.*;

/**
 * progress listener delegates to progress bar, everything else to status panel
 */
public class StatusPanelWithProgressBar implements UpdateOperationCallbacks {
    private final JPanel content = new JPanel();
    private final StatusPanel statusPanelFirmwareTab = new StatusPanel(500);
    private final JProgressBar progressBar = new JProgressBar();


    public StatusPanelWithProgressBar() {
        progressBar.setIndeterminate(false);
        progressBar.setStringPainted(true);
        progressBar.setBorder(BorderFactory.createLineBorder(Color.RED));

        content.setLayout(new BoxLayout(content, BoxLayout.Y_AXIS));
        content.add(statusPanelFirmwareTab);
        content.add(progressBar);
    }

    public JPanel getContent() {
        return content;
    }

    @Override
    public void done() {
        statusPanelFirmwareTab.done();
    }

    @Override
    public void error() {
        statusPanelFirmwareTab.error();
    }

    @Override
    public void warning() {
        statusPanelFirmwareTab.warning();
    }

    @Override
    public void clear() {
        statusPanelFirmwareTab.clear();
    }

    @Override
    public void log(String string, boolean breakLineOnTextArea, boolean sendToLogger) {
        statusPanelFirmwareTab.log(string, breakLineOnTextArea, sendToLogger);
    }

    @Override
    public void updateProgress(int percent) {
        SwingUtilities.invokeLater(() -> progressBar.setValue(percent));
    }
}
