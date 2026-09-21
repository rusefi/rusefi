package com.rusefi.ui.console;

import com.devexperts.logging.FileLogger;
import com.opensr5.ini.IniFileModel;
import com.rusefi.binaryprotocol.BinaryProtocol;
import com.rusefi.io.ConnectionStatusLogic;
import com.rusefi.io.ConnectionStatusValue;
import com.rusefi.tune.xml.Msq;
import com.rusefi.ui.UIContext;
import com.rusefi.util.TuneSnapshot;

import javax.swing.*;
import javax.swing.filechooser.FileNameExtensionFilter;
import java.awt.Component;
import java.awt.event.InputEvent;
import java.awt.event.KeyEvent;
import java.io.File;
import java.time.LocalDate;
import java.util.Locale;
import java.util.concurrent.ExecutionException;

/** Owns the logging menu and recorder lifecycle. Menu/lifecycle methods run on the Swing event thread. */
final class BinaryLoggingMenu {
    private static final KeyStroke TOGGLE_LOGGING = KeyStroke.getKeyStroke(KeyEvent.VK_S, InputEvent.CTRL_DOWN_MASK);
    private final UIContext uiContext;
    private final Component parent;
    private final JMenu menu;
    private final JMenuItem startItem;
    private final JMenuItem stopItem;
    private final JCheckBoxMenuItem saveTuneItem;
    private SwingWorker<Void, Void> startWorker;

    BinaryLoggingMenu(UIContext uiContext, Component parent, Icon startIcon, Icon stopIcon) {
        this.uiContext = uiContext;
        this.parent = parent;
        menu = new JMenu("Binary Logging");
        menu.setMnemonic(KeyEvent.VK_B);

        startItem = new JMenuItem("Start");
        startItem.setIcon(startIcon);
        startItem.setToolTipText("Start a binary data log (Ctrl+S)");
        startItem.addActionListener(e -> chooseAndStart());
        menu.add(startItem);

        stopItem = new JMenuItem("Stop");
        stopItem.setIcon(stopIcon);
        stopItem.setToolTipText("Stop logging or cancel a pending start (Ctrl+S)");
        stopItem.addActionListener(e -> {
            stop();
            refresh();
        });
        menu.add(stopItem);
        menu.addSeparator();
        saveTuneItem = new JCheckBoxMenuItem("Save tune", true);
        saveTuneItem.setToolTipText("Save a dated tune snapshot beside each data log");
        menu.add(saveTuneItem);

        refresh();
    }

    JMenu getMenu() {
        return menu;
    }

    void refresh() {
        boolean isLogging = uiContext.sensorLogger.isLogging();
        boolean isConnected = ConnectionStatusLogic.INSTANCE.getValue() == ConnectionStatusValue.CONNECTED;
        boolean isStarting = startWorker != null;
        startItem.setText(isStarting ? "Starting..." : "Start");
        startItem.setEnabled(isConnected && !isLogging && !isStarting);
        stopItem.setEnabled(isLogging || isStarting);
        // Only one item owns the accelerator at a time, including while saving the tune.
        startItem.setAccelerator(isLogging || isStarting ? null : TOGGLE_LOGGING);
        stopItem.setAccelerator(isLogging || isStarting ? TOGGLE_LOGGING : null);
        saveTuneItem.setEnabled(!isLogging && !isStarting);
    }

    private void chooseAndStart() {
        FileLogger.createFolderIfNeeded();
        JFileChooser chooser = new JFileChooser(new File(FileLogger.DIR));
        chooser.setDialogTitle("Save data Log");
        chooser.setFileSelectionMode(JFileChooser.FILES_ONLY);
        chooser.setFileFilter(new FileNameExtensionFilter("Binary log files (.mlg)", "mlg"));
        chooser.setSelectedFile(new File(FileLogger.DIR,
                "rusEFI_outputChannels_" + FileLogger.getDate() + ".mlg").getAbsoluteFile());
        if (chooser.showSaveDialog(parent) != JFileChooser.APPROVE_OPTION) {
            return;
        }

        File file = ensureMlgExtension(chooser.getSelectedFile());
        if (file.exists() && JOptionPane.showConfirmDialog(parent,
                file.getName() + " already exists. Replace it?",
                "Replace Binary Log",
                JOptionPane.YES_NO_OPTION,
                JOptionPane.WARNING_MESSAGE) != JOptionPane.YES_OPTION) {
            return;
        }

        if (!saveTuneItem.isSelected()) {
            start(file);
            return;
        }

        BinaryProtocol protocol = uiContext.getBinaryProtocol();
        startWorker = new SwingWorker<Void, Void>() {
            @Override
            protected Void doInBackground() throws Exception {
                IniFileModel ini = protocol == null ? null : protocol.getIniFileNullable();
                if (ini == null) {
                    throw new IllegalStateException("No ECU tune is available");
                }
                Msq tune = TuneSnapshot.read(uiContext.getLinkManager(), protocol, ini);
                if (!isCancelled()) {
                    TuneSnapshot.save(file.getAbsoluteFile().getParentFile().toPath(), tune, LocalDate.now());
                }
                return null;
            }

            @Override
            protected void done() {
                if (startWorker != this) {
                    return;
                }
                startWorker = null;
                try {
                    get();
                    if (uiContext.getBinaryProtocol() == protocol
                            && ConnectionStatusLogic.INSTANCE.getValue() == ConnectionStatusValue.CONNECTED) {
                        start(file);
                    }
                } catch (InterruptedException failure) {
                    Thread.currentThread().interrupt();
                } catch (ExecutionException failure) {
                    JOptionPane.showMessageDialog(parent,
                            "Could not save the tune. Data logging was not started.\n" + failure.getCause().getMessage(),
                            "Binary Logging", JOptionPane.ERROR_MESSAGE);
                } finally {
                    refresh();
                }
            }
        };
        startWorker.execute();
        refresh();
    }

    void stop() {
        SwingWorker<Void, Void> pending = startWorker;
        startWorker = null;
        if (pending != null) {
            pending.cancel(false);
        }
        uiContext.sensorLogger.stop();
    }

    private void start(File file) {
        if (!uiContext.sensorLogger.start(file)) {
            JOptionPane.showMessageDialog(parent,
                    "No supported output channels are available for binary logging.",
                    "Binary Logging",
                    JOptionPane.WARNING_MESSAGE);
        }
        refresh();
    }

    static File ensureMlgExtension(File file) {
        return file.getName().toLowerCase(Locale.ROOT).endsWith(".mlg")
                ? file
                : new File(file.getPath() + ".mlg");
    }
}
