package com.rusefi.ui.basic;

import com.rusefi.io.UpdateOperationCallbacks;
import com.rusefi.ui.widgets.StatusPanel;

import javax.swing.*;
import java.awt.*;
import java.util.Objects;
import java.util.function.Consumer;
import java.lang.reflect.InvocationTargetException;

/**
 * progress listener delegates to progress bar, everything else to status panel
 */
public class StatusPanelWithProgressBar implements UpdateOperationCallbacks {
    private final JPanel content = new JPanel();
    private final StatusPanel statusPanelFirmwareTab = new StatusPanel(500);
    private final JProgressBar progressBar = new JProgressBar();
    private final Consumer<String> showBlockedUpdate;
    private final Runnable prepareFirmwareHandoff;


    public StatusPanelWithProgressBar() {
        this(reason -> {});
    }

    public StatusPanelWithProgressBar(Consumer<String> showBlockedUpdate) {
        this(showBlockedUpdate, () -> {});
    }

    public StatusPanelWithProgressBar(Consumer<String> showBlockedUpdate, Runnable prepareFirmwareHandoff) {
        this.showBlockedUpdate = Objects.requireNonNull(showBlockedUpdate);
        this.prepareFirmwareHandoff = Objects.requireNonNull(prepareFirmwareHandoff);
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
    public void firmwareHandoffStarted() {
        // Detach splash listeners before the worker can disconnect or reboot the ECU.
        if (SwingUtilities.isEventDispatchThread()) {
            prepareFirmwareHandoff.run();
            return;
        }
        try {
            SwingUtilities.invokeAndWait(prepareFirmwareHandoff);
        } catch (InterruptedException e) {
            Thread.currentThread().interrupt();
            throw new IllegalStateException("Interrupted while preparing firmware handoff", e);
        } catch (InvocationTargetException e) {
            throw new IllegalStateException("Failed to prepare firmware handoff", e.getCause());
        }
    }

    @Override
    public void firmwareUpdateBlocked(String reason) {
        logLine(reason);
        SwingUtilities.invokeLater(() -> showBlockedUpdate.accept(reason));
    }

    @Override
    public void updateProgress(int percent) {
        SwingUtilities.invokeLater(() -> progressBar.setValue(percent));
    }
}
