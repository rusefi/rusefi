package com.rusefi.core.ui;

import org.jetbrains.annotations.NotNull;

import javax.swing.*;

public class ErrorMessageHelper {
    public static @NotNull JFrame createOnTopParent() {
        JFrame frame = new JFrame();
        // https://stackoverflow.com/questions/438463/joptionpane-wont-show-its-dialog-on-top-of-other-windows
        frame.pack();
        // https://stackoverflow.com/questions/65552872/how-do-i-center-a-jframe
        // we need some magic to center since JOptionPane is placed in relation to parent
        frame.setLocationRelativeTo(null);
        frame.setVisible(true);
        frame.setAlwaysOnTop(true);
        return frame;
    }

    public static void showErrorDialog(String message, String title) {
        JFrame parent = createOnTopParent();
        JOptionPane.showMessageDialog(parent,
            message,
            title,
            JOptionPane.ERROR_MESSAGE
        );
        parent.setVisible(false);
        parent.dispose();
    }
}
