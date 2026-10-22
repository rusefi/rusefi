package com.rusefi.ui.console;

import com.devexperts.logging.FileLogger;
import com.opensr5.ini.IniFileModel;
import com.rusefi.binaryprotocol.BinaryProtocol;
import com.rusefi.core.preferences.storage.Node;
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
import java.nio.file.Files;
import java.nio.file.Path;
import java.time.LocalDate;
import java.util.ArrayList;
import java.util.List;
import java.util.Locale;
import java.util.concurrent.ExecutionException;
import java.util.function.BiConsumer;
import java.util.function.Consumer;

/** Owns the logging menu and recorder lifecycle. Menu/lifecycle methods run on the Swing event thread. */
final class BinaryLoggingMenu {
    private static final KeyStroke TOGGLE_LOGGING = KeyStroke.getKeyStroke(KeyEvent.VK_S, InputEvent.CTRL_DOWN_MASK);
    /** Path of the last recording, kept in the console config so View Last Log works after a restart. */
    static final String LAST_LOG = "last_log";
    /** Whether the status strips show the Start Data Logging / Stop Logging button; on unless turned off. */
    static final String SHOW_RECORD_BUTTON = "show_record_button";
    private final UIContext uiContext;
    private final Node settings;
    private final Component parent;
    private final JMenu menu;
    private final JMenuItem startItem;
    private final JMenuItem startAsItem;
    private final JMenuItem stopItem;
    private final JCheckBoxMenuItem saveTuneItem;
    private final JMenuItem viewLastItem;
    private final JCheckBoxMenuItem showRecordButtonItem;
    private SwingWorker<byte[], Void> startWorker;
    private File lastLog;
    private Consumer<File> viewLogAction;
    private final List<BiConsumer<File, Double>> recordingStoppedListeners = new ArrayList<>();
    private final List<Runnable> stateListeners = new ArrayList<>();
    /** When the current recording started, for the elapsed time shown while recording. */
    private long recordingSince;
    private File logsFolder = new File(FileLogger.DIR);

    /** @param settings where the last recording's path is kept; saved with the console config on exit */
    BinaryLoggingMenu(UIContext uiContext, Component parent, Icon startIcon, Icon stopIcon, Icon viewIcon,
                      Node settings) {
        this.uiContext = uiContext;
        this.parent = parent;
        this.settings = settings;
        String lastPath = settings.getProperty(LAST_LOG, null);
        lastLog = lastPath == null || lastPath.isEmpty() ? null : new File(lastPath);
        menu = new JMenu("Logging");
        menu.setMnemonic(KeyEvent.VK_B);

        startItem = new JMenuItem("Start");
        startItem.setIcon(startIcon);
        startItem.setToolTipText("Record a data log into the rusEFI logs folder (Ctrl+S)");
        startItem.addActionListener(e -> startNow());
        menu.add(startItem);

        startAsItem = new JMenuItem("Start As…");
        startAsItem.setToolTipText("Choose the data log's file, then record");
        startAsItem.addActionListener(e -> chooseAndStart());
        menu.add(startAsItem);

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
        saveTuneItem.setToolTipText("Embed the starting tune in each data log and save a dated MSQ beside it");
        menu.add(saveTuneItem);
        menu.addSeparator();
        viewLastItem = new JMenuItem("View Last Log");
        viewLastItem.setIcon(viewIcon);
        viewLastItem.setToolTipText("Open the log recorded last in the Log Viewer tab");
        viewLastItem.addActionListener(e -> {
            if (viewLogAction != null && lastLog != null) {
                viewLogAction.accept(lastLog);
            }
        });
        menu.add(viewLastItem);
        menu.addSeparator();
        showRecordButtonItem = new JCheckBoxMenuItem("Show Data Logging Button",
                settings.getBoolProperty(SHOW_RECORD_BUTTON, true));
        showRecordButtonItem.setToolTipText("Show the Start Data Logging / Stop Logging button in the status bar at the bottom");
        showRecordButtonItem.addActionListener(e -> {
            settings.setBoolProperty(SHOW_RECORD_BUTTON, showRecordButtonItem.isSelected());
            refresh();
        });
        menu.add(showRecordButtonItem);

        refresh();
    }

    /** Told the file and duration (seconds, NaN if unknown) of each recording that stops. */
    void addRecordingStoppedListener(BiConsumer<File, Double> listener) {
        recordingStoppedListeners.add(listener);
    }

    /** Called whenever starting, recording or the connection changes what the menu offers. */
    void addStateListener(Runnable listener) {
        stateListeners.add(listener);
    }

    /** What View Last Log does with the recorded file, e.g. open it in the Log Viewer tab. */
    void setViewLogAction(Consumer<File> viewLogAction) {
        this.viewLogAction = viewLogAction;
        refresh();
    }

    File lastLog() {
        return lastLog;
    }

    JMenuItem viewLastItem() {
        return viewLastItem;
    }

    JMenuItem startItem() {
        return startItem;
    }

    JMenuItem startAsItem() {
        return startAsItem;
    }

    JMenuItem stopItem() {
        return stopItem;
    }

    JCheckBoxMenuItem showRecordButtonItem() {
        return showRecordButtonItem;
    }

    boolean isRecordButtonShown() {
        return showRecordButtonItem.isSelected();
    }

    boolean isRecording() {
        return uiContext.sensorLogger.isLogging();
    }

    /** Saving the tune before the recording starts. */
    boolean isStarting() {
        return startWorker != null;
    }

    boolean canStart() {
        return startItem.isEnabled();
    }

    /** Milliseconds since the current recording started. */
    long recordingMillis() {
        return isRecording() ? System.currentTimeMillis() - recordingSince : 0;
    }

    /** Starts recording at once, or stops the recording or pending start: the logging button and Ctrl+S. */
    void toggle() {
        if (isRecording() || isStarting()) {
            stop();
            refresh();
        } else if (canStart()) {
            startNow();
        }
    }

    /** Opens a log in the Log Viewer, e.g. from the notification after a recording stops. */
    void view(File file) {
        if (viewLogAction != null) {
            viewLogAction.accept(file);
        }
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
        startAsItem.setEnabled(startItem.isEnabled());
        stopItem.setEnabled(isLogging || isStarting);
        // Only one item owns the accelerator at a time, including while saving the tune.
        startItem.setAccelerator(isLogging || isStarting ? null : TOGGLE_LOGGING);
        stopItem.setAccelerator(isLogging || isStarting ? TOGGLE_LOGGING : null);
        saveTuneItem.setEnabled(!isLogging && !isStarting);
        // a log still being written is not complete yet
        viewLastItem.setEnabled(viewLogAction != null && lastLog != null && lastLog.isFile()
                && !isLogging && !isStarting);
        for (Runnable listener : stateListeners) {
            listener.run();
        }
    }

    /** The file a quick start records into: the rusEFI logs folder, named by date and time. */
    File defaultFile() {
        return new File(logsFolder, "rusEFI_outputChannels_" + FileLogger.getDate() + ".mlg").getAbsoluteFile();
    }

    void setLogsFolderForTests(File folder) {
        logsFolder = folder;
    }

    JCheckBoxMenuItem saveTuneItem() {
        return saveTuneItem;
    }

    /** Records into {@link #defaultFile()} without asking. */
    void startNow() {
        logsFolder.mkdirs();
        startFile(defaultFile());
    }

    private void chooseAndStart() {
        logsFolder.mkdirs();
        JFileChooser chooser = new JFileChooser(logsFolder);
        chooser.setDialogTitle("Save data Log");
        chooser.setFileSelectionMode(JFileChooser.FILES_ONLY);
        chooser.setFileFilter(new FileNameExtensionFilter("Binary log files (.mlg)", "mlg"));
        chooser.setSelectedFile(defaultFile());
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
        startFile(file);
    }

    /** Saves the tune first when Save tune is checked, then records into {@code file}. */
    private void startFile(File file) {
        if (!saveTuneItem.isSelected()) {
            start(file, null);
            return;
        }

        BinaryProtocol protocol = uiContext.getBinaryProtocol();
        startWorker = new SwingWorker<byte[], Void>() {
            @Override
            protected byte[] doInBackground() throws Exception {
                IniFileModel ini = protocol == null ? null : protocol.getIniFileNullable();
                if (ini == null) {
                    throw new IllegalStateException("No ECU tune is available");
                }
                Msq tune = TuneSnapshot.read(uiContext.getLinkManager(), protocol, ini);
                if (!isCancelled()) {
                    Path saved = TuneSnapshot.save(file.getAbsoluteFile().getParentFile().toPath(), tune, LocalDate.now());
                    return Files.readAllBytes(saved);
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
                    byte[] tune = get();
                    if (uiContext.getBinaryProtocol() == protocol
                            && ConnectionStatusLogic.INSTANCE.getValue() == ConnectionStatusValue.CONNECTED) {
                        start(file, tune);
                    }
                } catch (InterruptedException failure) {
                    Thread.currentThread().interrupt();
                } catch (ExecutionException failure) {
                    JOptionPane.showMessageDialog(parent,
                            "Could not save the tune. Data logging was not started.\n" + failure.getCause().getMessage(),
                            "Logging", JOptionPane.ERROR_MESSAGE);
                } finally {
                    refresh();
                }
            }
        };
        startWorker.execute();
        refresh();
    }

    void stop() {
        SwingWorker<byte[], Void> pending = startWorker;
        startWorker = null;
        if (pending != null) {
            pending.cancel(false);
        }
        boolean wasLogging = uiContext.sensorLogger.isLogging();
        uiContext.sensorLogger.stop();
        if (wasLogging && lastLog != null) {
            double seconds = uiContext.sensorLogger.getLastRecordingSeconds();
            for (BiConsumer<File, Double> listener : recordingStoppedListeners) {
                listener.accept(lastLog, seconds);
            }
        }
    }

    void start(File file, byte[] tune) {
        if (uiContext.sensorLogger.start(file, tune)) {
            lastLog = file;
            recordingSince = System.currentTimeMillis();
            settings.setProperty(LAST_LOG, file.getAbsolutePath());
        } else {
            JOptionPane.showMessageDialog(parent,
                    "No supported output channels are available for binary logging.",
                    "Logging",
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
