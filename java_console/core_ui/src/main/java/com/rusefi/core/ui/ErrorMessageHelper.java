package com.rusefi.core.ui;

import org.jetbrains.annotations.NotNull;

import javax.swing.*;

public class ErrorMessageHelper {
    public static @NotNull JFrame createOnTopParent() {
        JFrame frame = new JFrame();
        // https://stackoverflow.com/questions/438463/joptionpane-wont-show-its-dialog-on-top-of-other-windows
        frame.setVisible(true);
// ?        frame.setLocation(100, 100);
        frame.setAlwaysOnTop(true);
        return frame;
    }

    public static void showErrorDialog(String message, String title) {
        JOptionPane.showMessageDialog(createOnTopParent(),
            message,
            title,
            JOptionPane.ERROR_MESSAGE
        );
    }
}
