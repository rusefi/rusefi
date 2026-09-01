package com.rusefi.core.ui;

import org.jetbrains.annotations.NotNull;

import java.awt.*;
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
        boolean createdNewFrame = false;
        Component parent = null;
        for (Window w : Window.getWindows()) {
            if (w.isShowing() && w instanceof JFrame) {
                parent = w;
                break;
            }
        }
        if (parent == null) {
            parent = createOnTopParent();
            createdNewFrame = true;
        }
        JOptionPane.showMessageDialog(
            parent,
            message,
            title,
            JOptionPane.ERROR_MESSAGE
        );
        if (createdNewFrame && parent instanceof JFrame) {
            ((JFrame) parent).setVisible(false);
            ((JFrame) parent).dispose();
        }
    }
}
