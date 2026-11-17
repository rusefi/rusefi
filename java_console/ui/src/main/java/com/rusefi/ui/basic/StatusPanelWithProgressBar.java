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
    private final JPanel content = new JPanel(new BorderLayout(0, 8));
    private final StatusPanel statusPanelFirmwareTab = new StatusPanel(500);
    private final JProgressBar progressBar = new JProgressBar();
    private final JLabel operationStatus = new JLabel("Ready");
    private final JToggleButton detailsButton = new JToggleButton("Show details");
    private volatile boolean terminalStatus;
    private volatile UpdateOperationCallbacks.FirmwareUpdateStage firmwareStage;
    private volatile boolean activityVisible;
    private Consumer<Boolean> activityVisibilityListener = visible -> {};
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
        content.setOpaque(false);
        statusPanelFirmwareTab.setOpaque(false);
        progressBar.setIndeterminate(false);
        progressBar.setStringPainted(true);
        progressBar.setFont(progressBar.getFont().deriveFont(Font.BOLD, Math.max(16f,
            progressBar.getFont().getSize2D() + 4f)));
        progressBar.setPreferredSize(new Dimension(400, 42));
        // Keep the themed track tall without shifting its painted edges away from the status and details rows. [tag:console_theme]
        progressBar.setBorder(BorderFactory.createEmptyBorder());

        JPanel progress = new JPanel(new BorderLayout(0, 4));
        progress.setOpaque(false);
        progress.setBorder(BorderFactory.createEmptyBorder(8, 12, 4, 12));
        progress.add(operationStatus, BorderLayout.NORTH);
        progress.add(progressBar, BorderLayout.CENTER);
        detailsButton.addActionListener(event -> {
            boolean showDetails = detailsButton.isSelected();
            statusPanelFirmwareTab.setVisible(showDetails);
            detailsButton.setText(showDetails ? "Hide details" : "Show details");
        });
        JPanel details = new JPanel(new FlowLayout(FlowLayout.LEFT, 0, 0));
        details.setOpaque(false);
        details.setBorder(BorderFactory.createEmptyBorder(8, 0, 8, 0));
        details.add(detailsButton);
        progress.add(details, BorderLayout.SOUTH);
        statusPanelFirmwareTab.setVisible(false);
        content.add(progress, BorderLayout.NORTH);
        content.add(statusPanelFirmwareTab, BorderLayout.CENTER);
    }

    /** The log text, e.g. for {@link com.rusefi.ui.widgets.TextZoom}. */
    public JComponent getLogTextArea() {
        return statusPanelFirmwareTab.getLogTextArea();
    }

    public JPanel getContent() {
        return content;
    }

    public boolean isActivityVisible() {
        return activityVisible;
    }

    public void setActivityVisibilityListener(Consumer<Boolean> listener) {
        activityVisibilityListener = Objects.requireNonNull(listener);
        listener.accept(activityVisible);
    }

    /** Copies the status log, e.g. for a support request. */
    public void copyLogToClipboard() {
        statusPanelFirmwareTab.copyLogToClipboard();
    }

    @Override
    public void done() {
        showActivity();
        statusPanelFirmwareTab.done();
        terminalStatus = true;
        SwingUtilities.invokeLater(() -> {
            progressBar.setValue(100);
            operationStatus.setText("Operation finished");
        });
    }

    @Override
    public void error() {
        showActivity();
        statusPanelFirmwareTab.error();
        terminalStatus = true;
        SwingUtilities.invokeLater(() -> {
            showDetails();
            operationStatus.setText("Operation failed — see details");
        });
    }

    @Override
    public void warning() {
        showActivity();
        statusPanelFirmwareTab.warning();
        terminalStatus = true;
        SwingUtilities.invokeLater(() -> {
            showDetails();
            operationStatus.setText("Operation warning — see details");
        });
    }

    @Override
    public void clear() {
        statusPanelFirmwareTab.clear();
        terminalStatus = false;
        firmwareStage = null;
        activityVisible = false;
        SwingUtilities.invokeLater(() -> {
            progressBar.setValue(0);
            operationStatus.setText("Ready");
            detailsButton.setSelected(false);
            detailsButton.setText("Show details");
            statusPanelFirmwareTab.setVisible(false);
            activityVisibilityListener.accept(false);
        });
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
        showActivity();
        logLine(reason);
        SwingUtilities.invokeLater(() -> showBlockedUpdate.accept(reason));
    }

    @Override
    public void updateProgress(int percent) {
        showActivity();
        SwingUtilities.invokeLater(() -> {
            progressBar.setValue(percent);
            if (!terminalStatus) {
                operationStatus.setText(percent >= 100
                    ? "Current step complete — waiting for operation result"
                    : firmwareStage == null ? "Operation in progress"
                        : "Operation: " + firmwareStage.getDescription());
            }
        });
    }

    @Override
    public void updateFirmwarePhase(UpdateOperationCallbacks.FirmwareUpdateStage stage) {
        showActivity();
        firmwareStage = stage;
        SwingUtilities.invokeLater(() -> {
            if (!terminalStatus) {
                operationStatus.setText("Operation: " + stage.getDescription());
            }
        });
    }

    private void showDetails() {
        detailsButton.setSelected(true);
        detailsButton.setText("Hide details");
        statusPanelFirmwareTab.setVisible(true);
    }

    private void showActivity() {
        if (activityVisible) {
            return;
        }
        activityVisible = true;
        SwingUtilities.invokeLater(() -> activityVisibilityListener.accept(true));
    }

    /** Reveal non-progress guidance (for example, an ECU detected in its bootloader). */
    @Override
    public void revealActivity() {
        showActivity();
    }
}
