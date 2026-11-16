package com.rusefi.ui.basic;

import javax.swing.JOptionPane;
import java.awt.Component;

/** Session-once application update prompt, independent of ECU firmware controls. */
public final class ApplicationUpdatePrompt {
    private ApplicationUpdatePrompt() {
    }

    public static void show(Component parent, SoftwareUpdateCoordinator coordinator) {
        if (coordinator.getState() != SoftwareUpdateCoordinator.State.AVAILABLE
            || !coordinator.claimAvailablePrompt()) {
            return;
        }
        Object[] options = {"Later", "Update now"};
        int selected = JOptionPane.showOptionDialog(
            parent,
            "<html><b>A new version of rusEFI tuner is available.</b><br><br>"
                + "This updates the desktop application, not your ECU firmware.<br>"
                + "Nothing will restart until you choose to restart.</html>",
            "Application update available",
            JOptionPane.DEFAULT_OPTION,
            JOptionPane.INFORMATION_MESSAGE,
            null,
            options,
            options[1]);
        switch (choice(selected)) {
            case UPDATE_NOW:
                coordinator.install();
                break;
            default:
                coordinator.later();
                break;
        }
    }

    enum Choice {
        LATER,
        UPDATE_NOW
    }

    static Choice choice(int selectedOption) {
        if (selectedOption == 1) {
            return Choice.UPDATE_NOW;
        }
        return Choice.LATER;
    }
}
