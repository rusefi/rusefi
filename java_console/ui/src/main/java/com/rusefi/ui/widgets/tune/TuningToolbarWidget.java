package com.rusefi.ui.widgets.tune;

import com.devexperts.logging.Logging;
import com.opensr5.ConfigurationImage;
import com.opensr5.ini.IniFileModel;
import com.rusefi.binaryprotocol.BinaryProtocol;
import com.rusefi.io.UpdateOperationCallbacks;
import com.rusefi.maintenance.OfflineTuneLoader;
import com.rusefi.maintenance.jobs.AsyncJob;
import com.rusefi.maintenance.jobs.AsyncJobExecutor;
import com.rusefi.maintenance.jobs.JobHelper;
import com.rusefi.tune.xml.Msq;
import com.rusefi.tune.xml.MsqFactory;
import com.rusefi.ui.UIContext;
import com.rusefi.ui.basic.LoadTuneHelper;
import org.jetbrains.annotations.NotNull;

import javax.swing.*;
import javax.swing.filechooser.FileNameExtensionFilter;
import java.awt.*;
import java.awt.event.ActionEvent;
import java.io.File;
import java.util.ArrayDeque;
import java.util.concurrent.CountDownLatch;
import java.util.concurrent.atomic.AtomicReference;
import java.util.function.Consumer;

/**
 * Toolbar for the Tune tab: Burn, Discard, Undo, and Redo buttons.
 * Also owns the undo/redo history stacks and the upload/debounce timers
 * so they are kept out of TuningPane.
 */
public class TuningToolbarWidget {
    private static final Logging log = Logging.getLogging(TuningToolbarWidget.class);
    private static final int MAX_UNDO = 15;
    private static final int IDLE_TIMEOUT_MS = 300;
    private static final int UPLOAD_DELAY_MS = 100;

    private final JPanel panel;

    /** [tag:offline_tune] Shows offline / unburned-edit state. Empty when connected and clean. */
    private final JLabel stateLabel = new JLabel();
    private final UIContext uiContext;
    private final AtomicReference<ConfigurationImage> sessionImage;

    private final ArrayDeque<ConfigurationImage> undoStack = new ArrayDeque<>();
    private final ArrayDeque<ConfigurationImage> redoStack = new ArrayDeque<>();
    private final AtomicReference<ConfigurationImage> undoBaseline = new AtomicReference<>();

    /** [tag:offline_tune] Baseline image for discard — set when a tune is loaded (from ECU or MSQ file). */
    private ConfigurationImage baselineImage;

    private final JButton undoButton = new JButton("Undo");
    private final JButton redoButton = new JButton("Redo");

    private AbstractAction loadTuneAction;
    private AbstractAction saveTuneAction;
    private final JFileChooser saveTuneChooser = createMsqFileChooser();

    private final Timer undoCommitTimer;
    private final Timer uploadTimer;
    private volatile boolean firmwareUpdateInProgress;

    /**
     * @param uiContext      live context (BinaryProtocol, LinkManager)
     * @param right          the dialog widget — needed for get/update working image
     * @param currentKey     single-element array holding the currently displayed menu key
     * @param sessionImage   single-element array holding the accumulated session image
     * @param baselineImage  baseline image for discard (from ECU or loaded MSQ); may be null initially
     */
    public TuningToolbarWidget(UIContext uiContext,
                                CalibrationDialogWidget right,
                                 AtomicReference<String> currentKey,
                                 AtomicReference<ConfigurationImage> sessionImage,
                                 ConfigurationImage baselineImage,
                                 UpdateOperationCallbacks loadCallbacks,
                                 Runnable onLoadStarted,
                                 Consumer<Boolean> onLoadFinished) {
        this.uiContext = uiContext;
        this.sessionImage = sessionImage;
        this.baselineImage = baselineImage;
        undoButton.setEnabled(false);
        redoButton.setEnabled(false);

        Runnable updateButtons = () -> {
            undoButton.setEnabled(!undoStack.isEmpty());
            redoButton.setEnabled(!redoStack.isEmpty());
        };

        Runnable flushUndoBaseline = () -> {
            ConfigurationImage baseline = undoBaseline.getAndSet(null);
            if (baseline != null) {
                undoStack.push(baseline);
                if (undoStack.size() > MAX_UNDO) {
                    undoStack.removeLast();
                }
                redoStack.clear();
                updateButtons.run();
            }
        };

        undoCommitTimer = new Timer(IDLE_TIMEOUT_MS, e -> flushUndoBaseline.run());
        undoCommitTimer.setRepeats(false);

        uploadTimer = new Timer(UPLOAD_DELAY_MS, e -> {
            if (firmwareUpdateInProgress) {
                return;
            }
            BinaryProtocol bp = uiContext.getBinaryProtocol();
            ConfigurationImage image = sessionImage.get();
            if (bp == null || image == null) {
                return;
            }
            final ConfigurationImage snapshot = image.clone();
            uiContext.getLinkManager().submit(() -> bp.uploadChangesWithoutBurn(snapshot));
        });
        uploadTimer.setRepeats(false);

        JButton burnButton = getBurnToEcuButton(uiContext, right, sessionImage);
        burnButton.setEnabled(uiContext.getBinaryProtocol() != null);

        JButton discardButton = getDiscardButton(uiContext, right, sessionImage, currentKey, updateButtons);
        discardButton.setEnabled(baselineImage != null);

        buildLoadTuneAction(uiContext, right, currentKey, sessionImage,
            loadCallbacks, onLoadStarted, onLoadFinished);
        buildSaveTuneAction(uiContext, right, sessionImage);

        undoButton.addActionListener(e -> {
            if (undoStack.isEmpty()) {
                return;
            }
            undoCommitTimer.stop();
            undoBaseline.set(null);
            ConfigurationImage image = sessionImage.get();
            if (image != null) {
                redoStack.push(image);
            }
            sessionImage.set(undoStack.pop());
            String key = currentKey.get();
            if (key != null) {
                right.update(key, uiContext.iniFileState.getIniFileModel(), sessionImage.get());
            }
            updateButtons.run();
        });

        redoButton.addActionListener(e -> {
            if (redoStack.isEmpty()) {
                return;
            }
            undoCommitTimer.stop();
            undoBaseline.set(null);
            ConfigurationImage image = sessionImage.get();
            if (image != null) {
                undoStack.push(image);
            }
            sessionImage.set(redoStack.pop());
            String key = currentKey.get();
            if (key != null) {
                right.update(key, uiContext.iniFileState.getIniFileModel(), sessionImage.get());
            }
            updateButtons.run();
        });

        panel = new JPanel(new FlowLayout(FlowLayout.LEFT));
        panel.add(burnButton);
        panel.add(discardButton);
        panel.add(undoButton);
        panel.add(redoButton);
        panel.add(stateLabel);

        // [tag:offline_tune] Refresh the state label whenever offline mode toggles (e.g. Load Tune
        // while disconnected). Edit/discard/burn/connect/disconnect paths call refreshState() directly.
        uiContext.addOfflineModeListener(offline -> SwingUtilities.invokeLater(this::refreshState));
        refreshState();
    }

    /**
     * [tag:offline_tune] Updates the toolbar state label from current offline mode + dirty state.
     * Empty (invisible) when connected with no pending edits.
     */
    public void refreshState() {
        boolean dirty = hasUnsavedChanges(sessionImage.get());
        stateLabel.setText(stateLabelText(uiContext.isOfflineMode(), dirty));
        stateLabel.setForeground(dirty ? new Color(0xB0, 0x60, 0x00) : Color.GRAY);
    }

    /**
     * Pure label-text logic for the offline/dirty toolbar indicator. Package-private and static so it
     * can be unit-tested without Swing (see {@code TuningToolbarStateLabelTest}).
     */
    static String stateLabelText(boolean offline, boolean dirty) {
        if (dirty) {
            return offline ? "Local changes not burned to ECU" : "Pending changes not burned";
        }
        return offline ? "Offline tune" : "";
    }

    private @NotNull JButton getBurnToEcuButton(UIContext uiContext,
                                                CalibrationDialogWidget right,
                                                AtomicReference<ConfigurationImage> sessionImage) {
        JButton burnButton = new JButton("Burn to ECU");
        burnButton.addActionListener(e -> burnToEcuAndThen(right, null));
        return burnButton;
    }

    public void burnToEcuAndThen(CalibrationDialogWidget right, Runnable onSuccess) {
        if (firmwareUpdateInProgress) {
            return;
        }
        BinaryProtocol bp = uiContext.getBinaryProtocol();
        ConfigurationImage toBurn = right.getWorkingImage();
        if (toBurn == null) {
            toBurn = sessionImage.get();
        }
        if (bp == null || toBurn == null) {
            return;
        }
        final ConfigurationImage image = toBurn;
        sessionImage.set(image);
        uiContext.getLinkManager().submit(() -> {
            bp.burn();
            bp.setConfigurationImage(image);
            SwingUtilities.invokeLater(() -> {
                setBaselineImage(image.clone());
                if (onSuccess != null) {
                    onSuccess.run();
                }
            });
        });
    }

    /** Call when ECU connects to enable the burn button. */
    public void onEcuConnected() {
        Component[] components = panel.getComponents();
        for (Component c : components) {
            if (c instanceof JButton && "Burn to ECU".equals(((JButton) c).getText())) {
                ((JButton) c).setEnabled(true);
                break;
            }
        }
        refreshState();
    }

    private @NotNull JButton getDiscardButton(UIContext uiContext,
                                              CalibrationDialogWidget right,
                                              AtomicReference<ConfigurationImage> sessionImage,
                                              AtomicReference<String> currentKey,
                                              Runnable updateButtons) {
        JButton discardButton = new JButton("Discard changes");

        discardButton.addActionListener(e -> {
            if (baselineImage == null) {
                return;
            }
            sessionImage.set(baselineImage.clone());
            String key = currentKey.get();
            if (key != null) {
                right.update(key, uiContext.iniFileState.getIniFileModel(), sessionImage.get());
            }
            undoCommitTimer.stop();
            undoStack.clear();
            redoStack.clear();
            undoBaseline.set(null);
            updateButtons.run();
            refreshState();
        });

        return discardButton;
    }

    private void buildLoadTuneAction(UIContext uiContext,
                                     CalibrationDialogWidget right,
                                     AtomicReference<String> currentKey,
                                     AtomicReference<ConfigurationImage> sessionImage,
                                     UpdateOperationCallbacks callbacks,
                                     Runnable onLoadStarted,
                                     Consumer<Boolean> onLoadFinished) {
        JFileChooser chooser = createMsqFileChooser();
        loadTuneAction = new AbstractAction(LoadTuneHelper.LOAD_TUNE_TEXT) {
            @Override
            public void actionPerformed(ActionEvent e) {
                if (firmwareUpdateInProgress) {
                    return;
                }
                if (chooser.showOpenDialog(null) != JFileChooser.APPROVE_OPTION) {
                    return;
                }
                final String path = chooser.getSelectedFile().getAbsolutePath();
                final String fileName = chooser.getSelectedFile().getName();

                callbacks.clear();
                onLoadStarted.run();

                AsyncJobExecutor.INSTANCE.executeJob(
                    new AsyncJob("Load Tune") {
                        @Override
                        public void doJob(UpdateOperationCallbacks cb, Runnable onJobFinished) {
                            JobHelper.doJob(() -> {
                                try {
                                    cb.logLine("Loading " + fileName + "...");
                                    OfflineTuneLoader.Result result = OfflineTuneLoader.loadTuneFromFile(
                                        path, null, cb::logLine);
                                    if (result == null) {
                                        cb.error();
                                        SwingUtilities.invokeLater(() -> onLoadFinished.accept(true));
                                        return;
                                    }
                                    if (firmwareUpdateInProgress) {
                                        cb.logLine("Firmware update in progress; tune load cancelled.");
                                        cb.error();
                                        SwingUtilities.invokeLater(() -> onLoadFinished.accept(true));
                                        return;
                                    }
                                    cb.logLine("Applying tune fields...");
                                    BinaryProtocol bp = uiContext.getBinaryProtocol();
                                    IniFileModel targetIni = bp == null
                                        ? result.ini
                                        : uiContext.iniFileState.getIniFileModel();
                                    ConfigurationImage currentImage = imageToSave(
                                        right.getWorkingImage(),
                                        sessionImage.get(),
                                        bp == null ? null : bp.getControllerConfiguration(),
                                        baselineImage
                                    );
                                    ConfigurationImage base = currentImage == null
                                        ? new ConfigurationImage(targetIni.getMetaInfo().getPageSize(0))
                                        : currentImage.clone();
                                    ConfigurationImage newImage = bp == null
                                        ? result.image
                                        : result.msq.applyOnto(base, targetIni);

                                    if (bp != null) {
                                        cb.logLine("Uploading and burning to ECU...");
                                        CountDownLatch latch = new CountDownLatch(1);
                                        uiContext.getLinkManager().submit(() -> {
                                            try {
                                                bp.uploadChanges(newImage);
                                            } finally {
                                                latch.countDown();
                                            }
                                        });
                                        latch.await();
                                    }

                                    final boolean loadedWhileDisconnected = (bp == null);
                                    SwingUtilities.invokeAndWait(() -> {
                                        sessionImage.set(newImage);
                                        // [tag:offline_tune] Loading a tune with no ECU attached is an offline session.
                                        if (loadedWhileDisconnected) {
                                            uiContext.setOfflineMode(true);
                                        }
                                        String key = currentKey.get();
                                        if (key != null) {
                                            right.update(key, targetIni, newImage);
                                        }
                                        // Adopt the loaded tune as baseline (enables discard + refreshes state label).
                                        setBaselineImage(newImage.clone());
                                        uiContext.fireConfigImageChanged(newImage);
                                    });
                                    cb.done();
                                    SwingUtilities.invokeLater(() -> onLoadFinished.accept(false));
                                } catch (Exception ex) {
                                    log.error("Failed to load tune " + path, ex);
                                    cb.logLine("Error: " + ex);
                                    cb.error();
                                    SwingUtilities.invokeLater(() -> onLoadFinished.accept(true));
                                }
                            }, onJobFinished);
                        }
                    },
                    callbacks,
                    () -> { }
                );
            }
        };
    }

    public void setFirmwareUpdateInProgress(boolean inProgress) {
        firmwareUpdateInProgress = inProgress;
        if (inProgress) {
            uploadTimer.stop();
        }
        loadTuneAction.setEnabled(!inProgress);
    }

    private void buildSaveTuneAction(UIContext uiContext,
                                     CalibrationDialogWidget right,
                                     AtomicReference<ConfigurationImage> sessionImage) {
        saveTuneAction = new AbstractAction(LoadTuneHelper.SAVE_TUNE_TEXT) {
            @Override
            public void actionPerformed(ActionEvent e) {
                saveTuneAndThen(right, null);
            }
        };
    }

    public void saveTuneAndThen(CalibrationDialogWidget right, Runnable onSuccess) {
        IniFileModel ini = uiContext.iniFileState.getIniFileModel();
        BinaryProtocol bp = uiContext.getBinaryProtocol();
        ConfigurationImage image = imageToSave(
            right.getWorkingImage(),
            sessionImage.get(),
            bp == null ? null : bp.getControllerConfiguration(),
            baselineImage
        );
        if (ini == null || image == null) {
            JOptionPane.showMessageDialog(null, "No configuration loaded", "Error", JOptionPane.ERROR_MESSAGE);
            return;
        }
        if (saveTuneChooser.showSaveDialog(null) != JFileChooser.APPROVE_OPTION) {
            return;
        }
        File selected = saveTuneChooser.getSelectedFile();
        String path = selected.getAbsolutePath();
        if (!path.toLowerCase().endsWith(".msq")) {
            path += ".msq";
        }
        final String finalPath = path;
        final ConfigurationImage finalImage = image;
        new Thread(() -> {
            try {
                MsqFactory.valueOf(finalImage, ini).writeXmlFile(finalPath);
                if (onSuccess != null) {
                    SwingUtilities.invokeLater(onSuccess);
                }
            } catch (Exception ex) {
                SwingUtilities.invokeLater(() -> JOptionPane.showMessageDialog(
                        null, "Failed to save tune: " + ex.getMessage(), "Error", JOptionPane.ERROR_MESSAGE));
            }
        }, "save-tune").start();
    }

    static ConfigurationImage imageToSave(ConfigurationImage workingImage,
                                          ConfigurationImage sessionImage,
                                          ConfigurationImage ecuImage,
                                          ConfigurationImage baselineImage) {
        if (workingImage != null) {
            return workingImage;
        }
        if (sessionImage != null) {
            return sessionImage;
        }
        return ecuImage != null ? ecuImage : baselineImage;
    }

    public AbstractAction getLoadTuneAction() {
        return loadTuneAction;
    }

    public AbstractAction getSaveTuneAction() {
        return saveTuneAction;
    }

    /**
     * @return true if {@code current} differs from the loaded baseline (i.e. there are pending edits).
     * If there is no baseline yet, any non-null image counts as a change.
     */
    public boolean hasUnsavedChanges(ConfigurationImage current) {
        if (current == null) {
            return false;
        }
        if (baselineImage == null) {
            return true;
        }
        return !java.util.Arrays.equals(current.getContent(), baselineImage.getContent());
    }

    /** @return the loaded baseline image (pre-edit), or null if none loaded. */
    public ConfigurationImage getBaselineImage() {
        return baselineImage;
    }

    /** Sets the baseline image for discard and enables the discard button. */
    public void setBaselineImage(ConfigurationImage image) {
        this.baselineImage = image;
        Component[] components = panel.getComponents();
        for (Component c : components) {
            if (c instanceof JButton && "Discard changes".equals(((JButton) c).getText())) {
                ((JButton) c).setEnabled(image != null);
                break;
            }
        }
        refreshState();
    }

    private static @NotNull JFileChooser createMsqFileChooser() {
        JFileChooser chooser = new JFileChooser();
        chooser.setFileSelectionMode(JFileChooser.FILES_ONLY);
        chooser.setFileFilter(new FileNameExtensionFilter("Tune files (.msq)", "msq"));
        return chooser;
    }

    public JPanel getPanel() {
        return panel;
    }

    /**
     * Must be called on every edit event (from {@code right.setOnConfigChange}).
     * Captures the pre-edit baseline for undo and restarts the debounce + upload timers.
     */
    public void onEdit(ConfigurationImage previousSessionImage) {
        undoBaseline.compareAndSet(null, previousSessionImage);
        undoCommitTimer.restart();
        uploadTimer.restart();
        refreshState();
    }

    /**
     * Returns true when a debounce window is open (i.e., the user is mid-edit).
     * Call {@link #flushBeforeNavigate()} before loading a new section.
     */
    public boolean isUndoTimerRunning() {
        return undoCommitTimer.isRunning();
    }

    /**
     * Flushes any pending debounce window to the undo stack.
     * Must be called before navigating to a different section so that the
     * in-progress edit becomes its own undo point.
     */
    public void flushBeforeNavigate() {
        if (undoCommitTimer.isRunning()) {
            undoCommitTimer.stop();
            ConfigurationImage baseline = undoBaseline.getAndSet(null);
            if (baseline != null) {
                undoStack.push(baseline);
                if (undoStack.size() > MAX_UNDO) undoStack.removeLast();
                redoStack.clear();
                undoButton.setEnabled(!undoStack.isEmpty());
                redoButton.setEnabled(!redoStack.isEmpty());
            }
        }
    }

    /**
     * Must be called when the ECU disconnects so stale state is dropped.
     * [tag:offline_tune] Preserves baselineImage so discard still works in offline mode.
     */
    public void onDisconnect() {
        undoCommitTimer.stop();
        uploadTimer.stop();
        undoStack.clear();
        redoStack.clear();
        undoBaseline.set(null);
        undoButton.setEnabled(false);
        redoButton.setEnabled(false);
        // Disable burn button when ECU disconnects
        Component[] components = panel.getComponents();
        for (Component c : components) {
            if (c instanceof JButton && "Burn to ECU".equals(((JButton) c).getText())) {
                ((JButton) c).setEnabled(false);
                break;
            }
        }
        refreshState();
    }
}
