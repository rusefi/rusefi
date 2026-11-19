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
    private static final String STARTUP_HOST_PROPERTY = ManualIniFilePicker.class.getName() + ".startupHost";

    interface IniImporter {
        Object importIni(String signature, java.io.File file) throws IOException;
    }

    static final class StartupHost {
        final JTabbedPane tabs;
        final Component connectContent;
        final Runnable revealControls;
        JPanel activePicker;
        int connectIndex = -1;
        int previousIndex = -1;

        StartupHost(JTabbedPane tabs, Component connectContent, Runnable revealControls) {
            this.tabs = tabs;
            this.connectContent = connectContent;
            this.revealControls = revealControls;
        }
    }

    public static void registerStartupHost(JRootPane rootPane, JTabbedPane tabs,
                                           Component connectContent, Runnable revealControls) {
        rootPane.putClientProperty(STARTUP_HOST_PROPERTY,
            new StartupHost(tabs, connectContent, revealControls));
    }

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

        showPicker(frame.getRootPane(), frame, signature);
    }

    static void showPicker(JRootPane rootPane, Component parent, String signature) {
        showPicker(rootPane, parent, signature, SignatureHelper::importIntoCache);
    }

    static void showPicker(JRootPane rootPane, Component parent, String signature, IniImporter importer) {
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

        StartupHost host = (StartupHost) rootPane.getClientProperty(STARTUP_HOST_PROPERTY);
        Component previousGlassPane = rootPane.getGlassPane();
        boolean previousVisible = previousGlassPane.isVisible();
        Runnable[] startupRestore = {null};
        if (host != null) {
            startupRestore[0] = showInStartup(host, content);
        }

        chooser.addActionListener(e -> {
            if (JFileChooser.APPROVE_SELECTION.equals(e.getActionCommand())) {
                try {
                    if (importer.importIni(signature, chooser.getSelectedFile()) == null) {
                        throw new IllegalArgumentException("Invalid ECU signature: " + signature);
                    }
                } catch (IOException | IllegalArgumentException ex) {
                    log.error("Failed to import picked .ini into cache", ex);
                    JOptionPane.showMessageDialog(parent, ex.getMessage(),
                        "Failed to Import INI", JOptionPane.ERROR_MESSAGE);
                    return;
                }
            } else if (!JFileChooser.CANCEL_SELECTION.equals(e.getActionCommand())) {
                return;
            }
            if (startupRestore[0] != null) {
                startupRestore[0].run();
            } else {
                rootPane.setGlassPane(previousGlassPane);
                previousGlassPane.setVisible(previousVisible);
            }
        });

        if (startupRestore[0] != null) {
            chooser.requestFocusInWindow();
            return;
        }
        JPanel overlay = new JPanel(new GridBagLayout());
        overlay.add(content);
        rootPane.setGlassPane(overlay);
        overlay.setVisible(true);
        chooser.requestFocusInWindow();
    }

    private static Runnable showInStartup(StartupHost host, JPanel content) {
        int connectIndex = host.activePicker == null
            ? host.tabs.indexOfComponent(host.connectContent)
            : host.tabs.indexOfComponent(host.activePicker);
        if (connectIndex < 0) {
            return null;
        }
        if (host.activePicker == null) {
            host.connectIndex = connectIndex;
            host.previousIndex = host.tabs.getSelectedIndex();
            host.revealControls.run();
        }
        JButton updateFirmware = new JButton("Go to Update ECU firmware");
        updateFirmware.addActionListener(e -> host.tabs.setSelectedIndex(0));
        content.add(Box.createVerticalStrut(12));
        content.add(updateFirmware);

        JPanel picker = new JPanel(new GridBagLayout());
        picker.add(content);
        host.tabs.setSelectedIndex(connectIndex);
        host.tabs.setComponentAt(connectIndex, picker);
        host.activePicker = picker;

        return () -> {
            if (host.activePicker != picker) {
                return;
            }
            host.tabs.setComponentAt(host.connectIndex, host.connectContent);
            host.activePicker = null;
            if (host.tabs.getSelectedIndex() == host.connectIndex && host.previousIndex >= 0) {
                host.tabs.setSelectedIndex(host.previousIndex);
            }
        };
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
