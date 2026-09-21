package com.rusefi.core.ui;

import org.jetbrains.annotations.NotNull;
import org.jetbrains.annotations.Nullable;

import javax.swing.*;
import java.awt.*;

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

    /**
     * @return the currently active window, searching owned windows recursively, or null if none is active
     */
    @Nullable
    public static Window getSelectedWindow(Window[] windows) {
        for (Window window : windows) {
            if (window.isActive()) {
                return window;
            }
            Window selected = getSelectedWindow(window.getOwnedWindows());
            if (selected != null) {
                return selected;
            }
        }
        return null;
    }

    public static void showErrorDialog(String message, String title) {
        // parenting to the active window gives Linux window managers a WM_TRANSIENT_FOR relation
        // so the modal dialog stacks above the application instead of a ghost frame, see #10144
        Window activeWindow = getSelectedWindow(Window.getWindows());
        if (activeWindow != null) {
            JOptionPane.showMessageDialog(activeWindow,
                message,
                title,
                JOptionPane.ERROR_MESSAGE
            );
            return;
        }
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
