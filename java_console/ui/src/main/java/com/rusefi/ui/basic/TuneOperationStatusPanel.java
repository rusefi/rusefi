package com.rusefi.ui.basic;

import com.rusefi.ui.widgets.StatusPanel;

import javax.swing.*;
import java.awt.*;

public class TuneOperationStatusPanel {
    private final JPanel content = new JPanel(new BorderLayout(0, 12));
    private final JLabel result = new JLabel("", SwingConstants.CENTER);
    private final JProgressBar progress = new JProgressBar();
    private final JButton copyLog = new JButton("Copy Log");
    private final JButton continueButton = new JButton("Continue");

    public TuneOperationStatusPanel(StatusPanel statusPanel, Runnable onContinue) {
        result.setFont(result.getFont().deriveFont(Font.BOLD, 24f));
        progress.setIndeterminate(true);
        progress.setStringPainted(true);
        copyLog.addActionListener(e -> statusPanel.copyContentToClipboard());
        for (JButton button : new JButton[]{copyLog, continueButton}) {
            button.setFont(button.getFont().deriveFont(button.getFont().getSize() * 1.5f));
            button.setMargin(new Insets(10, 24, 10, 24));
        }
        continueButton.addActionListener(e -> onContinue.run());

        JPanel actions = new JPanel(new FlowLayout(FlowLayout.CENTER, 12, 8));
        actions.add(copyLog);
        actions.add(continueButton);
        Dimension copySize = copyLog.getPreferredSize();
        Dimension continueSize = continueButton.getPreferredSize();
        Dimension actionSize = new Dimension(
            Math.max(copySize.width, continueSize.width),
            Math.max(copySize.height, continueSize.height)
        );
        copyLog.setPreferredSize(actionSize);
        continueButton.setPreferredSize(actionSize);
        JPanel progressWrapper = new JPanel(new BorderLayout());
        progressWrapper.setBorder(BorderFactory.createEmptyBorder(10, 24, 10, 24));
        progressWrapper.add(progress, BorderLayout.CENTER);
        JPanel footer = new JPanel(new BorderLayout());
        footer.add(progressWrapper, BorderLayout.NORTH);
        footer.add(actions, BorderLayout.SOUTH);

        content.setBorder(BorderFactory.createEmptyBorder(24, 24, 24, 24));
        content.add(result, BorderLayout.NORTH);
        content.add(statusPanel, BorderLayout.CENTER);
        content.add(footer, BorderLayout.SOUTH);
    }

    public JPanel getContent() {
        return content;
    }

    public void showProgress(String message) {
        result.setText(message);
        result.setForeground(Color.DARK_GRAY);
        progress.setString(message);
        progress.setVisible(true);
        copyLog.setVisible(false);
        continueButton.setVisible(false);
    }

    public void showResult(String successMessage, String failureMessage, boolean failed) {
        result.setText(failed ? failureMessage : successMessage);
        result.setForeground(failed ? Color.RED.darker() : new Color(0, 128, 0));
        progress.setVisible(false);
        copyLog.setVisible(failed);
        continueButton.setVisible(true);
    }

    String getResultTextForUnitTest() {
        return result.getText();
    }

    boolean isProgressVisibleForUnitTest() {
        return progress.isVisible();
    }

    boolean isCopyLogVisibleForUnitTest() {
        return copyLog.isVisible();
    }

    boolean isContinueVisibleForUnitTest() {
        return continueButton.isVisible();
    }

    Dimension getCopyLogSizeForUnitTest() {
        return copyLog.getPreferredSize();
    }

    Dimension getContinueSizeForUnitTest() {
        return continueButton.getPreferredSize();
    }

    void continueForUnitTest() {
        continueButton.doClick();
    }
}
