package com.rusefi.maintenance;

import com.devexperts.logging.Logging;
import com.rusefi.binaryprotocol.RealIniFileProvider;
import com.rusefi.core.SignatureHelper;

import javax.swing.*;
import javax.swing.filechooser.FileNameExtensionFilter;
import java.awt.*;
import java.io.IOException;

import static com.devexperts.logging.Logging.getLogging;

/**
 * Swing .ini picker registered into the (Swing-free) io module via
 * {@link RealIniFileProvider#manualPicker}. Fires when a connect cannot download or locate the
 * matching .ini (internet down / custom board, see issue #9774); the selected file is cached so a
 * later port-scanner probe can connect without blocking on user input.
 */
public class ManualIniFilePicker {
    private static final Logging log = getLogging(ManualIniFilePicker.class);

    public static void register() {
        RealIniFileProvider.manualPicker = ManualIniFilePicker::pick;
    }

    private static void pick(String signature) {
        SwingUtilities.invokeLater(() -> showPicker(signature));
    }

    private static void showPicker(String signature) {
        JFrame frame = findVisibleFrame();
        if (frame == null) {
            RealIniFileProvider.allowManualPrompt(signature);
            return;
        }

        JFileChooser chooser = new JFileChooser();
        chooser.setFileSelectionMode(JFileChooser.FILES_ONLY);
        chooser.setFileFilter(new FileNameExtensionFilter("INI files", "ini"));

        JLabel title = new JLabel("INI File Not Found");
        title.setFont(title.getFont().deriveFont(Font.BOLD, 24f));
        title.setAlignmentX(Component.CENTER_ALIGNMENT);
        JLabel message = new JLabel("Could not automatically find an INI file for:");
        message.setAlignmentX(Component.CENTER_ALIGNMENT);
        JLabel signatureLabel = new JLabel(signature);
        signatureLabel.setFont(signatureLabel.getFont().deriveFont(Font.BOLD));
        signatureLabel.setAlignmentX(Component.CENTER_ALIGNMENT);
        JLabel instruction = new JLabel("Select the matching INI file to continue.");
        instruction.setAlignmentX(Component.CENTER_ALIGNMENT);

        JPanel content = new JPanel();
        content.setLayout(new BoxLayout(content, BoxLayout.Y_AXIS));
        content.add(title);
        content.add(Box.createVerticalStrut(12));
        content.add(message);
        content.add(Box.createVerticalStrut(4));
        content.add(signatureLabel);
        content.add(Box.createVerticalStrut(12));
        content.add(instruction);
        content.add(Box.createVerticalStrut(20));
        content.add(chooser);

        JPanel overlay = new JPanel(new GridBagLayout());
        overlay.add(content);
        Component previousGlassPane = frame.getGlassPane();
        boolean previousVisible = previousGlassPane.isVisible();

        chooser.addActionListener(e -> {
            if (JFileChooser.APPROVE_SELECTION.equals(e.getActionCommand())) {
                try {
                    if (SignatureHelper.importIntoCache(signature, chooser.getSelectedFile()) == null) {
                        throw new IllegalArgumentException("Invalid ECU signature: " + signature);
                    }
                } catch (IOException | IllegalArgumentException ex) {
                    log.error("Failed to import picked .ini into cache", ex);
                    JOptionPane.showMessageDialog(frame, ex.getMessage(),
                        "Failed to Import INI", JOptionPane.ERROR_MESSAGE);
                    return;
                }
            } else if (!JFileChooser.CANCEL_SELECTION.equals(e.getActionCommand())) {
                return;
            }
            frame.setGlassPane(previousGlassPane);
            previousGlassPane.setVisible(previousVisible);
        });

        frame.setGlassPane(overlay);
        overlay.setVisible(true);
        chooser.requestFocusInWindow();
    }

    private static JFrame findVisibleFrame() {
        Window active = KeyboardFocusManager.getCurrentKeyboardFocusManager().getActiveWindow();
        if (active instanceof JFrame && active.isVisible()) {
            return (JFrame) active;
        }
        for (Frame frame : JFrame.getFrames()) {
            if (frame instanceof JFrame && frame.isVisible()) {
                return (JFrame) frame;
            }
        }
        return null;
    }
}
