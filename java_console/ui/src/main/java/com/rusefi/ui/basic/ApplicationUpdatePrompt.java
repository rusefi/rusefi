package com.rusefi.ui.basic;

import javax.swing.JOptionPane;
import java.awt.Component;

/** Session-once application update prompts, independent of ECU firmware controls. */
public final class ApplicationUpdatePrompt {
    /** Asks whether to restart now; true for Restart now. */
    public interface RestartQuestion {
        boolean restartNow(Component parent);
    }

    private static volatile RestartQuestion restartQuestion = ApplicationUpdatePrompt::askToRestart;

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

    /**
     * [tag:update_restart_prompt] Once the update is downloaded, asks over the window in use whether to restart,
     * once per session. A banner alone can sit in a view the window is not showing, e.g. the startup window's
     * "Scanning for ECU…": the restart then went unseen until a tune was loaded. After Later the banners remain.
     *
     * @param restart closes this window and starts the updated application
     */
    public static void showRestart(Component parent, SoftwareUpdateCoordinator coordinator, Runnable restart) {
        if (coordinator.getState() != SoftwareUpdateCoordinator.State.RESTART_REQUIRED
            || !coordinator.claimRestartPrompt()) {
            return;
        }
        if (restartQuestion.restartNow(parent)) {
            restart.run();
        }
    }

    private static boolean askToRestart(Component parent) {
        Object[] options = {"Later", "Restart now"};
        int selected = JOptionPane.showOptionDialog(
            parent,
            "<html><b>The rusEFI tuner update is ready.</b><br><br>"
                + "Restart the application to finish updating.</html>",
            "Application update ready",
            JOptionPane.DEFAULT_OPTION,
            JOptionPane.INFORMATION_MESSAGE,
            null,
            options,
            options[1]);
        return selected == 1;
    }

    /** Replaces the restart dialog, for tests; null puts the dialog back. */
    public static void setRestartQuestionForTests(RestartQuestion question) {
        restartQuestion = question == null ? ApplicationUpdatePrompt::askToRestart : question;
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
