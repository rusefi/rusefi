package com.rusefi.maintenance;

import com.rusefi.binaryprotocol.RealIniFileProvider;

import javax.swing.*;
import javax.swing.filechooser.FileNameExtensionFilter;
import java.awt.*;
import java.io.File;
import java.util.concurrent.CountDownLatch;
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
        AtomicReference<File> result = new AtomicReference<>();
        CountDownLatch completed = new CountDownLatch(1);
        Runnable prompt = () -> showPicker(signature, result, completed);
        if (SwingUtilities.isEventDispatchThread()) {
            SecondaryLoop loop = Toolkit.getDefaultToolkit().getSystemEventQueue().createSecondaryLoop();
            prompt.run();
            Thread waiter = new Thread(() -> {
                await(completed);
                SwingUtilities.invokeLater(loop::exit);
            }, "INI picker wait");
            waiter.setDaemon(true);
            waiter.start();
            loop.enter();
        } else {
            SwingUtilities.invokeLater(prompt);
            if (!await(completed)) {
                return null;
            }
        }
        return result.get();
    }

    private static boolean await(CountDownLatch completed) {
        try {
            completed.await();
            return true;
        } catch (InterruptedException e) {
            Thread.currentThread().interrupt();
            return false;
        }
    }

    private static void showPicker(String signature, AtomicReference<File> result, CountDownLatch completed) {
        JFrame frame = findVisibleFrame();
        if (frame == null) {
            completed.countDown();
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
                result.set(chooser.getSelectedFile());
            } else if (!JFileChooser.CANCEL_SELECTION.equals(e.getActionCommand())) {
                return;
            }
            frame.setGlassPane(previousGlassPane);
            previousGlassPane.setVisible(previousVisible);
            completed.countDown();
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
