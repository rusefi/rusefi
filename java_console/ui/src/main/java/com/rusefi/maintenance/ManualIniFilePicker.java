package com.rusefi.maintenance;

import com.rusefi.binaryprotocol.RealIniFileProvider;

import javax.swing.*;
import javax.swing.filechooser.FileNameExtensionFilter;
import java.io.File;
import java.lang.reflect.InvocationTargetException;
import java.util.concurrent.atomic.AtomicReference;

/**
 * Swing .ini picker registered into the (Swing-free) io module via
 * {@link RealIniFileProvider#manualPicker}. Fires when a connect cannot download or locate the
 * matching .ini (internet down / custom board, see issue #9774); the returned file is cached by
 * {@link RealIniFileProvider} so subsequent connects are prompt-free.
 */
public class ManualIniFilePicker {
    public static void register() {
        RealIniFileProvider.manualPicker = ManualIniFilePicker::pick;
    }

    private static File pick(String signature) {
        // provide() runs on the connection thread; marshal the dialog onto the EDT
        AtomicReference<File> result = new AtomicReference<>();
        Runnable prompt = () -> result.set(promptOnEdt(signature));
        if (SwingUtilities.isEventDispatchThread()) {
            prompt.run();
        } else {
            try {
                SwingUtilities.invokeAndWait(prompt);
            } catch (InterruptedException | InvocationTargetException e) {
                return null;
            }
        }
        return result.get();
    }

    private static File promptOnEdt(String signature) {
        int choice = JOptionPane.showConfirmDialog(null,
                "Could not automatically find an INI file for signature:\n" + signature +
                        "\n\nWould you like to select an INI file manually?",
                "INI File Not Found", JOptionPane.YES_NO_OPTION, JOptionPane.WARNING_MESSAGE);
        if (choice != JOptionPane.YES_OPTION) {
            return null;
        }
        JFileChooser chooser = new JFileChooser();
        chooser.setFileSelectionMode(JFileChooser.FILES_ONLY);
        chooser.setFileFilter(new FileNameExtensionFilter("INI files", "ini"));
        if (chooser.showOpenDialog(null) != JFileChooser.APPROVE_OPTION) {
            return null;
        }
        return chooser.getSelectedFile();
    }
}
