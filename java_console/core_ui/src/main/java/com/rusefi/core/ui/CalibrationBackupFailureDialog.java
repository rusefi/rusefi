package com.rusefi.core.ui;

import javax.swing.*;
import java.awt.*;
import java.util.List;

public class CalibrationBackupFailureDialog {

    private static final String TITLE = "Calibration Backup Failed";
    private static final String BUTTON_RETRY = "Retry Calibration";
    private static final String BUTTON_SKIP = "Skip Failed Fields";
    private static final String BUTTON_RESET = "Reset All Configuration";
    private static final String BUTTON_CANCEL = "Cancel Update";

    public static CalibrationBackupFailureAction showDialog(
        JComponent parent,
        String failureMessage,
        List<String> failedFields,
        boolean hasBackupData
    ) {
        StringBuilder message = new StringBuilder();
        message.append("<html><body style='width: 350px'>");
        message.append("<h3>Calibration Backup/Restore Failed</h3>");
        message.append(failureMessage);
        message.append("<br><br>");

        if (failedFields != null && !failedFields.isEmpty()) {
            message.append("<b>Failed fields:</b><br>");
            for (String field : failedFields) {
                message.append("&bull; ").append(field).append("<br>");
            }
            message.append("<br>");
        }

        if (!hasBackupData) {
            message.append("<i>No calibration backup available. You can retry or proceed with ECU defaults.</i>");
        }

        message.append("<br>What would you like to do?</body></html>");

        Object[] options;
        if (hasBackupData) {
            options = new Object[]{BUTTON_RETRY, BUTTON_SKIP, BUTTON_RESET};
        } else {
            options = new Object[]{BUTTON_RETRY, BUTTON_RESET, BUTTON_CANCEL};
        }

        Window owner = findOwnerWindow(parent);
        int choice = JOptionPane.showOptionDialog(
            owner,
            message.toString(),
            TITLE,
            JOptionPane.DEFAULT_OPTION,
            JOptionPane.WARNING_MESSAGE,
            null,
            options,
            options[0]
        );

        if (choice == 0) {
            return CalibrationBackupFailureAction.RETRY;
        } else if (choice == 1) {
            return hasBackupData ? CalibrationBackupFailureAction.SKIP_FAILED_FIELDS : CalibrationBackupFailureAction.RESET_ALL;
        } else if (choice == 2 && hasBackupData) {
            return CalibrationBackupFailureAction.RESET_ALL;
        } else {
            return CalibrationBackupFailureAction.CANCEL;
        }
    }

    private static Window findOwnerWindow(JComponent parent) {
        if (parent != null) {
            Window window = SwingUtilities.getWindowAncestor(parent);
            if (window != null && window.isShowing()) {
                return window;
            }
        }
        JFrame fallback = new JFrame();
        fallback.setAlwaysOnTop(true);
        fallback.setLocationRelativeTo(null);
        fallback.setSize(1, 1);
        fallback.setVisible(true);
        return fallback;
    }
}
