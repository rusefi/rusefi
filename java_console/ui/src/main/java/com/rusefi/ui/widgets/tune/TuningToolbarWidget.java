package com.rusefi.ui.widgets.tune;

import com.devexperts.logging.Logging;
import com.formdev.flatlaf.FlatClientProperties;
import com.opensr5.ConfigurationImage;
import com.opensr5.ConfigurationImageWithMeta;
import com.opensr5.ini.IniFileModel;
import com.opensr5.ini.field.ArrayIniField;
import com.opensr5.ini.field.EnumIniField;
import com.opensr5.ini.field.IniField;
import com.opensr5.ini.field.ScalarIniField;
import com.rusefi.config.FieldType;
import com.rusefi.binaryprotocol.BinaryProtocol;
import com.rusefi.io.UpdateOperationCallbacks;
import com.rusefi.maintenance.CalibrationsHelper;
import com.rusefi.maintenance.CalibrationsInfo;
import com.rusefi.maintenance.OfflineTuneLoader;
import com.rusefi.maintenance.jobs.AsyncJob;
import com.rusefi.maintenance.jobs.AsyncJobExecutor;
import com.rusefi.maintenance.jobs.JobHelper;
import com.rusefi.maintenance.migration.IniFieldMigrationUtils;
import com.rusefi.tune.ConfigurationImageGetterSetter2;
import com.rusefi.tune.xml.Constant;
import com.rusefi.tune.xml.Msq;
import com.rusefi.tune.xml.MsqFactory;
import com.rusefi.tune.xml.Page;
import com.rusefi.util.RestorePointStore;
import com.rusefi.ui.UIContext;
import com.rusefi.ui.basic.LoadTuneHelper;
import com.rusefi.ui.widgets.JoinedButtons;
import org.jetbrains.annotations.NotNull;

import javax.swing.*;
import javax.swing.filechooser.FileNameExtensionFilter;
import java.awt.*;
import java.awt.event.ActionEvent;
import java.io.File;
import java.io.IOException;
import java.util.ArrayDeque;
import java.util.HashSet;
import java.util.Map;
import java.util.Objects;
import java.util.Set;
import java.util.concurrent.Future;
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
    static final String BURN_NOTE = "Burned. Previous tune kept as a restore point";
    /** Long enough to read the Burn note and click its link. */
    private static final int NOTE_MS = 8000;

    private final JPanel panel;
    private Window dialogOwner;
    private final JButton burnButton;
    private final JButton discardButton;

    /** [tag:offline_tune] Shows offline / unburned-edit state. Empty when connected and clean. */
    private final JLabel stateLabel = new JLabel();
    private final UIContext uiContext;
    private final Consumer<Runnable> submit;
    private final AtomicReference<ConfigurationImage> sessionImage;

    private final ArrayDeque<ConfigurationImage> undoStack = new ArrayDeque<>();
    private final ArrayDeque<ConfigurationImage> redoStack = new ArrayDeque<>();
    private final AtomicReference<ConfigurationImage> undoBaseline = new AtomicReference<>();

    /** [tag:offline_tune] Baseline image for discard — set when a tune is loaded (from ECU or MSQ file). */
    private ConfigurationImage baselineImage;

    private final JButton undoButton = new JButton("Undo");
    private final JButton redoButton = new JButton("Redo");

    private AbstractAction loadTuneAction;
    /** Where the tune is kept before each Burn and Load tune. */
    private volatile RestorePointStore restorePoints = RestorePointStore.shared();
    private AbstractAction saveTuneAction;
    private AbstractAction saveTuneAsAction;
    private AbstractAction createRestorePointAction;
    /** Writes restore points made by hand off the EDT. */
    private java.util.concurrent.Executor fileWork = work -> new Thread(work, "restore-point").start();
    /** Ends a {@link #showNote} message: the toolbar state comes back. */
    private final Timer noteTimer = new Timer(NOTE_MS, e -> refreshState());
    /** The link of the note shown, null for none. */
    private Runnable noteAction;
    private Runnable showRestorePoints = () -> { };
    private final java.awt.event.MouseAdapter noteClick = new java.awt.event.MouseAdapter() {
        @Override
        public void mouseClicked(java.awt.event.MouseEvent e) {
            Runnable action = noteAction;
            if (action != null && SwingUtilities.isLeftMouseButton(e)) {
                action.run();
            }
        }
    };
    private final JFileChooser saveTuneChooser = createMsqFileChooser();

    private final Timer undoCommitTimer;
    private final Timer uploadTimer;
    private volatile boolean firmwareUpdateInProgress;
    private boolean burnInProgress;
    private Runnable onBaselineChanged = () -> { };
    private long editGeneration;
    private volatile long connectionGeneration;
    private Consumer<String> errorHandler = message -> JOptionPane.showMessageDialog(null, message, "Error", JOptionPane.ERROR_MESSAGE);

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
        this(uiContext, right, currentKey, sessionImage, baselineImage, loadCallbacks,
            onLoadStarted, onLoadFinished, operation -> uiContext.getLinkManager().submit(operation));
    }

    TuningToolbarWidget(UIContext uiContext,
                        CalibrationDialogWidget right,
                        AtomicReference<String> currentKey,
                        AtomicReference<ConfigurationImage> sessionImage,
                        ConfigurationImage baselineImage,
                        UpdateOperationCallbacks loadCallbacks,
                        Runnable onLoadStarted,
                        Consumer<Boolean> onLoadFinished,
                        Consumer<Runnable> submit) {
        this.uiContext = uiContext;
        this.submit = submit;
        this.sessionImage = sessionImage;
        this.baselineImage = baselineImage;
        noteTimer.setRepeats(false);
        stateLabel.addMouseListener(noteClick);
        undoButton.setEnabled(false);
        redoButton.setEnabled(false);
        undoButton.setToolTipText("Undo the last tune change");
        redoButton.setToolTipText("Redo the last undone tune change");

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

        uploadTimer = new Timer(UPLOAD_DELAY_MS, e -> uploadPendingChanges());
        uploadTimer.setRepeats(false);

        burnButton = getBurnToEcuButton(uiContext, right, sessionImage);
        burnButton.setEnabled(uiContext.getBinaryProtocol() != null);

        discardButton = getDiscardButton(uiContext, right, sessionImage, currentKey, updateButtons);
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
            // [tag:detached_calibration_sync] Restored windows may be open before a primary page
            // is selected. Publish replacements even with a null key (also redo/discard/load below).
            right.update(key, uiContext.iniFileState.getIniFileModel(), sessionImage.get());
            publishReplacement();
            updateButtons.run();
            // the replaced tune can be clean again, or differ in another number of values
            refreshState();
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
            right.update(key, uiContext.iniFileState.getIniFileModel(), sessionImage.get());
            publishReplacement();
            updateButtons.run();
            refreshState();
        });

        // sits on the Tuning tab's toolbar strip (TuningPane), which supplies the colour
        panel = layOut(CONTROLS_TOP, stateLabel, burnButton, discardButton, undoButton, redoButton);

        // [tag:offline_tune] Refresh the state label whenever offline mode toggles (e.g. Load Tune
        // while disconnected). Edit/discard/burn/connect/disconnect paths call refreshState() directly.
        uiContext.addOfflineModeListener(offline -> SwingUtilities.invokeLater(this::refreshState));
        refreshState();
    }

    /**
     * Same controls as the Tuning toolbar [tag:console_theme]: 36 px tall, primary Burn, outlined
     * groups, Undo and Redo joined as one control, then the state label.
     * [tag:tune_controls_position] Burn sits {@link #CONTROLS_LEFT} px from the screen edge and
     * {@link #CONTROLS_TOP} px below the tab row on both Tuning and Gauges, so switching tabs never moves it.
     */
    private static JPanel layOut(int vgap, JLabel label, JButton burn, JButton discard, JButton undo, JButton redo) {
        JPanel row = new JPanel(new FlowLayout(FlowLayout.LEFT, JoinedButtons.ROW_GAP, vgap));
        row.setOpaque(false);
        burn.setPreferredSize(new Dimension(burn.getPreferredSize().width + 24, JoinedButtons.HEIGHT));
        row.add(burn);
        row.add(new JoinedButtons(discard));
        row.add(new JoinedButtons(undo, redo));
        label.setBorder(BorderFactory.createEmptyBorder(0, 6, 0, 0));
        row.add(label);
        return row;
    }

    /** [tag:tune_controls_position] Left inset of the Tuning strip (TuningPane) plus the row gap. */
    public static final int CONTROLS_LEFT = 4 + JoinedButtons.ROW_GAP;
    /** [tag:tune_controls_position] The Tuning toolbar row's vertical gap. */
    public static final int CONTROLS_TOP = 6;

    /**
     * Disabled Burn / Discard / Undo / Redo laid out exactly like {@link #createMirrorControls()}, holding
     * their place on another screen until the Tuning pane (built on first use) supplies the live ones.
     */
    public static JComponent createPlaceholderControls() {
        JButton burn = new JButton("Burn to ECU");
        burn.putClientProperty(FlatClientProperties.STYLE_CLASS, "primary");
        JButton[] buttons = {burn, new JButton("Discard changes"), new JButton("Undo"), new JButton("Redo")};
        for (JButton button : buttons) {
            button.setEnabled(false);
        }
        return layOut(0, new JLabel(), buttons[0], buttons[1], buttons[2], buttons[3]);
    }

    /**
     * Burn / Discard / Undo / Redo for another screen, e.g. the Gauges toolbar, where settings windows edit
     * the same tune. Each button runs this toolbar's own handler and follows its enabled state, so both
     * places always agree; the label repeats the unburned-changes text.
     */
    public JComponent createMirrorControls() {
        JLabel label = new JLabel();
        // a note's link works here too
        label.addMouseListener(noteClick);
        Runnable syncLabel = () -> {
            label.setCursor(stateLabel.getCursor());
            label.setToolTipText(stateLabel.getToolTipText());
            label.setText(stateLabel.getText());
            label.setForeground(stateLabel.getForeground());
        };
        stateLabel.addPropertyChangeListener("text", e -> syncLabel.run());
        stateLabel.addPropertyChangeListener("foreground", e -> syncLabel.run());
        syncLabel.run();
        return layOut(0, label, mirrorOf(burnButton), mirrorOf(discardButton), mirrorOf(undoButton),
            mirrorOf(redoButton));
    }

    private static JButton mirrorOf(JButton original) {
        JButton mirror = new JButton(original.getText());
        mirror.setToolTipText(original.getToolTipText());
        mirror.putClientProperty(FlatClientProperties.STYLE_CLASS,
            original.getClientProperty(FlatClientProperties.STYLE_CLASS));
        mirror.setEnabled(original.isEnabled());
        original.addPropertyChangeListener("enabled", e -> mirror.setEnabled(original.isEnabled()));
        mirror.addActionListener(e -> {
            if (original.isEnabled()) {
                for (java.awt.event.ActionListener listener : original.getActionListeners()) {
                    listener.actionPerformed(e);
                }
            }
        });
        return mirror;
    }

    /**
     * [tag:offline_tune] Updates the toolbar state label from current offline mode + dirty state.
     * Empty (invisible) when connected with no pending edits.
     */
    public void refreshState() {
        ConfigurationImage current = sessionImage.get();
        boolean dirty = hasUnsavedChanges(current);
        IniFileModel ini = uiContext.iniFileState == null ? null : uiContext.iniFileState.getIniFileModel();
        int changedValues = dirty ? changedValueCount(ini, current, baselineImage) : 0;
        // the state replaces any note, and its link
        noteAction = null;
        stateLabel.setCursor(Cursor.getDefaultCursor());
        stateLabel.setToolTipText(null);
        stateLabel.setText(stateLabelText(uiContext.isOfflineMode(), dirty, changedValues));
        stateLabel.setForeground(dirty ? TuningStyle.dirty() : TuningStyle.muted());
    }

    /**
     * [tag:unburned_marks] How many settings and table/curve cells of {@code current} differ from the
     * last burned tune; 0 without an .ini or a burned tune.
     */
    static int changedValueCount(IniFileModel ini, ConfigurationImage current, ConfigurationImage burned) {
        if (ini == null) {
            return 0;
        }
        return UnburnedChanges.of(current, burned).countChanged(ini.getAllIniFields().values());
    }

    static String stateLabelText(boolean offline, boolean dirty) {
        return stateLabelText(offline, dirty, 0);
    }

    /**
     * Pure label-text logic for the offline/dirty toolbar indicator. Package-private and static so it
     * can be unit-tested without Swing (see {@code TuningToolbarStateLabelTest}).
     *
     * @param changedValues settings and cells changed since the last burn; 0 shows no count, e.g. when
     *                      only bytes no setting covers differ
     */
    static String stateLabelText(boolean offline, boolean dirty, int changedValues) {
        if (dirty) {
            String text = offline ? "Local changes not burned to ECU" : "Pending changes not burned";
            if (changedValues > 0) {
                text += " (" + changedValues + (changedValues == 1 ? " value)" : " values)");
            }
            return text;
        }
        return offline ? "Offline tune" : "";
    }

    private @NotNull JButton getBurnToEcuButton(UIContext uiContext,
                                                CalibrationDialogWidget right,
                                                AtomicReference<ConfigurationImage> sessionImage) {
        JButton burnButton = new JButton("Burn to ECU");
        burnButton.setToolTipText("Save the current tune to persistent ECU memory");
        burnButton.putClientProperty(FlatClientProperties.STYLE_CLASS, "primary");
        burnButton.addActionListener(e -> burnToEcuAndThen(right, null));
        return burnButton;
    }

    public void burnToEcuAndThen(CalibrationDialogWidget right, Runnable onSuccess) {
        if (firmwareUpdateInProgress || burnInProgress) {
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
        uploadTimer.stop();
        final ConfigurationImage image = toBurn.clone();
        final IniFileModel ini = uiContext.iniFileState.getIniFileModel();
        final RestorePointStore store = restorePoints;
        final long savedEditGeneration = editGeneration;
        final long savedConnectionGeneration = connectionGeneration;
        burnInProgress = true;
        burnButton.setEnabled(false);
        discardButton.setEnabled(false);
        loadTuneAction.setEnabled(false);
        try {
            submit.accept(() -> {
                // the ECU's tune as it is before this Burn replaces it
                boolean previousKept = saveRestorePoint(store, bp.getControllerConfiguration(), ini,
                    RestorePointStore.Reason.BEFORE_BURN);
                RuntimeException failure = null;
                try {
                    bp.uploadChangesWithoutBurn(image);
                    bp.burn();
                    bp.setConfigurationImage(image);
                } catch (RuntimeException ex) {
                    failure = ex;
                }
                RuntimeException operationFailure = failure;
                SwingUtilities.invokeLater(() -> {
                    boolean sameConnection = savedConnectionGeneration == connectionGeneration
                        && uiContext.getBinaryProtocol() == bp;
                    ConfigurationImage currentImage = sessionImage.get();
                    boolean sameTune = currentImage != null
                        && java.util.Arrays.equals(currentImage.getContent(), image.getContent());
                    boolean noNewEdits = savedEditGeneration == editGeneration && sameTune;
                    burnInProgress = false;
                    burnButton.setEnabled(!firmwareUpdateInProgress && uiContext.getBinaryProtocol() != null);
                    discardButton.setEnabled(baselineImage != null);
                    loadTuneAction.setEnabled(!firmwareUpdateInProgress);
                    if (!sameConnection) {
                        return;
                    }
                    if (operationFailure != null) {
                        refreshState();
                        errorHandler.accept("Failed to burn tune: " + operationFailure.getMessage());
                        return;
                    }
                    setBaselineImage(image.clone());
                    refreshState();
                    if (previousKept) {
                        showNote(BURN_NOTE, "Restore points", showRestorePoints);
                    }
                    if (noNewEdits && onSuccess != null) {
                        onSuccess.run();
                    }
                });
            });
        } catch (RuntimeException ex) {
            burnInProgress = false;
            burnButton.setEnabled(uiContext.getBinaryProtocol() != null);
            discardButton.setEnabled(baselineImage != null);
            loadTuneAction.setEnabled(!firmwareUpdateInProgress);
            errorHandler.accept("Failed to burn tune: " + ex.getMessage());
        }
    }

    /** Call when ECU connects to enable the burn button. */
    public void onEcuConnected() {
        connectionGeneration++;
        burnButton.setEnabled(!burnInProgress);
        refreshState();
    }

    private @NotNull JButton getDiscardButton(UIContext uiContext,
                                              CalibrationDialogWidget right,
                                              AtomicReference<ConfigurationImage> sessionImage,
                                              AtomicReference<String> currentKey,
                                              Runnable updateButtons) {
        JButton discardButton = new JButton("Discard changes");
        discardButton.setToolTipText("Restore the tune from the last load or burn");

        discardButton.addActionListener(e -> {
            if (burnInProgress || baselineImage == null) {
                return;
            }
            uploadTimer.stop();
            ConfigurationImage restoration = baselineImage.clone();
            sessionImage.set(restoration);
            String key = currentKey.get();
            right.update(key, uiContext.iniFileState.getIniFileModel(), restoration);
            publishReplacement();
            undoCommitTimer.stop();
            undoStack.clear();
            redoStack.clear();
            undoBaseline.set(null);
            updateButtons.run();
            refreshState();

            BinaryProtocol bp = uiContext.getBinaryProtocol();
            if (bp == null || firmwareUpdateInProgress || uiContext.isOfflineMode()) {
                return;
            }
            ConfigurationImage snapshot = restoration.clone();
            long savedConnectionGeneration = connectionGeneration;
            try {
                submit.accept(() -> {
                    if (savedConnectionGeneration != connectionGeneration
                            || uiContext.getBinaryProtocol() != bp
                            || firmwareUpdateInProgress || uiContext.isOfflineMode()) {
                        return;
                    }
                    try {
                        bp.uploadChangesWithoutBurn(snapshot);
                    } catch (RuntimeException ex) {
                        SwingUtilities.invokeLater(() ->
                            errorHandler.accept("Failed to restore tune to ECU: " + ex.getMessage()));
                    }
                });
            } catch (RuntimeException ex) {
                SwingUtilities.invokeLater(() ->
                    errorHandler.accept("Failed to restore tune to ECU: " + ex.getMessage()));
            }
        });

        return discardButton;
    }

    /**
     * Tells views following the working tune ({@link UIContext#fireConfigImageChanged}: Pinout's Tune use,
     * the context's current image) that Undo, Redo or Discard replaced it. An edit notifies through the
     * dialog's change callback, but these replace the tune directly, so without this those views keep showing
     * the replaced tune (TuningUndoNotificationTest).
     */
    private void publishReplacement() {
        ConfigurationImage image = sessionImage.get();
        if (image != null) {
            uiContext.fireConfigImageChanged(image);
        }
    }

    private void uploadPendingChanges() {
        if (firmwareUpdateInProgress) {
            return;
        }
        BinaryProtocol bp = uiContext.getBinaryProtocol();
        ConfigurationImage image = sessionImage.get();
        if (bp == null || image == null) {
            return;
        }
        final ConfigurationImage snapshot = image.clone();
        try {
            submit.accept(() -> {
                try {
                    bp.uploadChangesWithoutBurn(snapshot);
                } catch (RuntimeException ex) {
                    SwingUtilities.invokeLater(() ->
                        errorHandler.accept("Failed to upload tune: " + ex.getMessage()));
                }
            });
        } catch (RuntimeException ex) {
            SwingUtilities.invokeLater(() ->
                errorHandler.accept("Failed to upload tune: " + ex.getMessage()));
        }
    }

    /** Fires the pending debounce upload deterministically in unit tests. */
    void flushUploadTimerForUnitTest() {
        uploadTimer.stop();
        uploadPendingChanges();
    }

    boolean isUploadTimerRunningForUnitTest() {
        return uploadTimer.isRunning();
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
                if (firmwareUpdateInProgress || burnInProgress) {
                    return;
                }
                TuneFileFolder.openIn(chooser);
                if (chooser.showOpenDialog(getDialogParent()) != JFileChooser.APPROVE_OPTION) {
                    return;
                }
                TuneFileFolder.remember(chooser.getSelectedFile());
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
                                    BinaryProtocol bp = uiContext.getBinaryProtocol();
                                    OfflineTuneLoader.Result result = bp == null
                                        ? OfflineTuneLoader.loadTuneFromFile(path, null, cb::logLine)
                                        : OfflineTuneLoader.loadConnectedTuneFromFile(
                                            path, uiContext.iniFileState.getIniFileModel(), cb::logLine);
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
                                    IniFileModel targetIni = bp == null
                                        ? result.ini
                                        : uiContext.iniFileState.getIniFileModel();
                                    ConfigurationImage currentImage = imageToSave(
                                        right.getWorkingImage(),
                                        sessionImage.get(),
                                        bp == null ? null : bp.getControllerConfiguration(),
                                        baselineImage
                                    );
                                    // the tune open now, unburned changes too: loading replaces it (and the ECU's)
                                    saveRestorePoint(restorePoints, currentImage,
                                        uiContext.iniFileState.getIniFileModel(), RestorePointStore.Reason.BEFORE_LOAD);
                                    ConfigurationImage base = currentImage == null
                                        ? new ConfigurationImage(targetIni.getMetaInfo().getPageSize(0), targetIni.getByteOrder())
                                        : currentImage.clone();
                                    ConfigurationImage newImage = bp == null
                                        ? result.image
                                        : applyLoadedTune(result.msq, result.ini, base, targetIni, cb);

                                    if (bp != null) {
                                        cb.logLine("Uploading and burning to ECU...");
                                        Future<?> upload = uiContext.getLinkManager().submit(
                                            () -> bp.uploadChanges(newImage));
                                        try {
                                            upload.get();
                                        } catch (InterruptedException ex) {
                                            Thread.currentThread().interrupt();
                                            throw ex;
                                        }
                                    }

                                    final boolean loadedWhileDisconnected = (bp == null);
                                    SwingUtilities.invokeAndWait(() -> {
                                        sessionImage.set(newImage);
                                        // [tag:offline_tune] Loading a tune with no ECU attached is an offline session.
                                        if (loadedWhileDisconnected) {
                                            uiContext.setOfflineMode(true);
                                        }
                                        uiContext.setTuneFilePath(path);
                                        String key = currentKey.get();
                                        right.update(key, targetIni, newImage);
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

    static ConfigurationImage applyLoadedTune(Msq tune, IniFileModel sourceIni,
                                               ConfigurationImage base, IniFileModel targetIni,
                                               UpdateOperationCallbacks callbacks) {
        if (sourceIni == null) {
            if (tune.getVersionInfo() == null
                    || !Objects.equals(tune.getVersionInfo().getSignature(), targetIni.getSignature())) {
                callbacks.logLine("Cannot import tune without its source INI because its signature differs "
                    + "from the destination ECU.");
                return base;
            }
            return applySameSignatureTune(tune, base, targetIni, callbacks);
        }
        Set<String> secondaryFields = new HashSet<>(sourceIni.getSecondaryIniFields().keySet());
        secondaryFields.addAll(targetIni.getSecondaryIniFields().keySet());

        CalibrationsInfo target = new CalibrationsInfo(
            targetIni, ConfigurationImageWithMeta.valueOf(targetIni, base));
        CalibrationsHelper.MergeResult result = CalibrationsHelper.mergeCalibrationsWithPartialFailure(
            sourceIni, tune, target, callbacks, secondaryFields);

        return result.mergedCalibrations
            .map(calibrations -> calibrations.getImage().getConfigurationImage())
            .orElse(base);
    }

    private static ConfigurationImage applySameSignatureTune(Msq tune, ConfigurationImage base,
                                                              IniFileModel targetIni,
                                                              UpdateOperationCallbacks callbacks) {
        ConfigurationImage result = base.clone();
        Page page = tune.findPage();
        if (page == null) {
            callbacks.logLine("Tune has no values to import.");
            return result;
        }
        Map<String, IniField> targetFields = targetIni.getAllIniFields();
        Set<String> secondaryFields = targetIni.getSecondaryIniFields().keySet();
        for (Constant constant : page.constant) {
            String name = constant.getName();
            if (name.startsWith("UNALLOCATED_SPACE")) {
                continue;
            }
            if (secondaryFields.contains(name)) {
                callbacks.logLine("Skipped " + name + ": secondary calibration.");
                continue;
            }
            IniField field = targetFields.get(name);
            if (field == null) {
                callbacks.logLine("Skipped " + name + ": not available.");
                continue;
            }
            String sourceUnits = constant.getUnits();
            String destinationUnits = field.getUnits();
            boolean sourceUnitsMissing = sourceUnits == null || sourceUnits.isEmpty();
            boolean destinationUnitsMissing = destinationUnits == null || destinationUnits.isEmpty();
            if (sourceUnitsMissing != destinationUnitsMissing
                    || !IniFieldMigrationUtils.checkIfUnitsCanBeMigrated(sourceUnits, destinationUnits)) {
                callbacks.logLine("Skipped " + name + ": units do not match.");
                continue;
            }
            if (field instanceof ArrayIniField) {
                ArrayIniField array = (ArrayIniField) field;
                if (!Objects.equals(constant.getRows(), Integer.toString(array.getRows()))
                        || !Objects.equals(constant.getCols(), Integer.toString(array.getCols()))) {
                    callbacks.logLine("Skipped " + name + ": table size does not match.");
                    continue;
                }
            } else if (constant.getRows() != null || constant.getCols() != null) {
                callbacks.logLine("Skipped " + name + ": table size does not match.");
                continue;
            }

            try {
                if (field instanceof EnumIniField) {
                    ConfigurationImage candidate = result.clone();
                    applyExactEnumValue((EnumIniField) field, constant, candidate);
                    result = candidate;
                } else {
                    validateNumericValues(field, constant);
                    ConfigurationImage candidate = result.clone();
                    ConfigurationImageGetterSetter2.setValue(field, candidate, constant);
                    result = candidate;
                }
            } catch (RuntimeException e) {
                String reason = e instanceof IllegalArgumentException ? e.getMessage() : null;
                if (!"unknown pin".equals(reason) && !"pin is not available".equals(reason)
                        && !"pin label is ambiguous".equals(reason) && !"value is out of range".equals(reason)
                        && !"table size does not match".equals(reason) && !"invalid value".equals(reason)) {
                    reason = "invalid value";
                }
                callbacks.logLine("Skipped " + name + ": " + reason + ".");
            }
        }
        return result;
    }

    private static void validateNumericValues(IniField field, Constant constant) {
        if (field instanceof ScalarIniField) {
            ScalarIniField scalar = (ScalarIniField) field;
            validateNumericValue(constant.getValue(), scalar.getType(), scalar.getMultiplier(),
                scalar.getSerializationOffset());
        } else if (field instanceof ArrayIniField) {
            ArrayIniField array = (ArrayIniField) field;
            String[][] values;
            try {
                values = array.getValues(constant.getValue());
            } catch (RuntimeException e) {
                throw new IllegalArgumentException("table size does not match");
            }
            for (String[] row : values) {
                for (String value : row) {
                    validateNumericValue(value, array.getType(), array.getMultiplier(), 0);
                }
            }
        }
    }

    private static void validateNumericValue(String value, FieldType type, double multiplier,
                                            double serializationOffset) {
        double parsed;
        double raw;
        try {
            parsed = Double.parseDouble(value);
            raw = (parsed - serializationOffset) / multiplier;
        } catch (RuntimeException e) {
            throw new IllegalArgumentException("invalid value");
        }
        if (!Double.isFinite(parsed) || !Double.isFinite(raw)) {
            throw new IllegalArgumentException("value is out of range");
        }

        if (type == FieldType.FLOAT) {
            if (!Float.isFinite((float) raw)) {
                throw new IllegalArgumentException("value is out of range");
            }
            return;
        }

        long rounded = Math.round(raw);
        long min;
        long max;
        switch (type) {
            case INT8:
                min = Byte.MIN_VALUE;
                max = Byte.MAX_VALUE;
                break;
            case UINT8:
                min = 0;
                max = 0xff;
                break;
            case INT16:
                min = Short.MIN_VALUE;
                max = Short.MAX_VALUE;
                break;
            case UINT16:
                min = 0;
                max = 0xffff;
                break;
            case INT:
                min = Integer.MIN_VALUE;
                max = Integer.MAX_VALUE;
                break;
            default:
                throw new IllegalArgumentException("invalid value");
        }
        if (rounded < min || rounded > max) {
            throw new IllegalArgumentException("value is out of range");
        }
    }

    private static void applyExactEnumValue(EnumIniField field, Constant constant, ConfigurationImage image) {
        String value = constant.getValue();
        if (value != null && !EnumIniField.isQuoted(value) && EnumIniField.isUnknownOrdinalValue(value)) {
            throw new IllegalArgumentException("unknown pin");
        }
        String label = value != null && EnumIniField.isQuoted(value)
            ? javax.management.ObjectName.unquote(value)
            : value;
        if (label == null) {
            throw new IllegalArgumentException("unknown pin");
        }
        if (field.isPinEnum() && "INVALID".equalsIgnoreCase(label)) {
            throw new IllegalArgumentException("unknown pin");
        }
        long matches = field.getEnums().values().stream().filter(label::equals).count();
        int ordinal = field.getEnums().indexOf(value);
        if (matches != 1 || ordinal < 0) {
            throw new IllegalArgumentException(matches > 1
                ? "pin label is ambiguous"
                : "pin is not available");
        }
        image.setBitValue(field, ordinal);
    }

    public void setFirmwareUpdateInProgress(boolean inProgress) {
        firmwareUpdateInProgress = inProgress;
        if (inProgress) {
            uploadTimer.stop();
        }
        loadTuneAction.setEnabled(!inProgress && !burnInProgress);
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
        saveTuneAsAction = new AbstractAction(LoadTuneHelper.SAVE_TUNE_AS_TEXT) {
            @Override
            public void actionPerformed(ActionEvent e) {
                saveTuneAsAndThen(right, null);
            }
        };
        createRestorePointAction = new AbstractAction(LoadTuneHelper.CREATE_RESTORE_POINT_TEXT) {
            @Override
            public void actionPerformed(ActionEvent e) {
                if (firmwareUpdateInProgress) {
                    return;
                }
                String note = askRestorePointNote();
                if (note != null) {
                    createRestorePoint(right, note);
                }
            }
        };
        createRestorePointAction.putValue(Action.SHORT_DESCRIPTION,
            "Keep the tune open now, unburned changes too, to come back to later");
    }

    /** The note for a restore point made by hand: "" for none, null when cancelled. */
    private String askRestorePointNote() {
        JTextField note = new JTextField(28);
        JLabel noteLabel = new JLabel("Note (optional)");
        noteLabel.setLabelFor(note);
        JLabel hint = new JLabel("Saves the tune open now, including unburned changes.");
        hint.setForeground(TuningStyle.muted());
        JPanel form = new JPanel(new GridLayout(0, 1, 0, 6));
        form.add(noteLabel);
        form.add(note);
        form.add(hint);
        int choice = JOptionPane.showOptionDialog(getDialogParent(), form, "Create restore point",
            JOptionPane.OK_CANCEL_OPTION, JOptionPane.PLAIN_MESSAGE, null, new Object[]{"Create", "Cancel"}, "Create");
        return choice == 0 ? note.getText() : null;
    }

    /**
     * [tag:restore_points] Keeps the tune open now, unburned changes included, as a restore point with a note. Made
     * by hand, it is saved even when the newest point holds the same tune. Says so in the toolbar when done.
     */
    void createRestorePoint(CalibrationDialogWidget right, String note) {
        IniFileModel ini = uiContext.iniFileState.getIniFileModel();
        BinaryProtocol bp = uiContext.getBinaryProtocol();
        ConfigurationImage image = imageToSave(
            right.getWorkingImage(),
            sessionImage.get(),
            bp == null ? null : bp.getControllerConfiguration(),
            baselineImage
        );
        if (ini == null || image == null) {
            errorHandler.accept("No tune open to keep as a restore point");
            return;
        }
        ConfigurationImage copy = image.clone();
        RestorePointStore store = restorePoints;
        fileWork.execute(() -> {
            try {
                RestorePointStore.RestorePoint point = store.saveAlways(MsqFactory.valueOf(copy, ini),
                    RestorePointStore.Reason.MANUAL, note);
                log.info("Restore point " + point.file);
                SwingUtilities.invokeLater(() -> showNote("Restore point created"));
            } catch (IOException | RuntimeException ex) {
                log.error("Failed to create restore point", ex);
                SwingUtilities.invokeLater(() -> errorHandler.accept("Failed to create restore point: " + ex.getMessage()));
            }
        });
    }

    /** A short message in place of the toolbar state for a few seconds, e.g. "Restore point created". */
    public void showNote(String text) {
        showNote(text, null, null);
    }

    /**
     * A note with a link after it, e.g. "Burned. Previous tune kept as a restore point · Restore points": a click
     * on the label (here or on its Gauges mirror) runs {@code action} while the note shows.
     */
    public void showNote(String text, String linkText, Runnable action) {
        noteAction = action;
        stateLabel.setCursor(action == null ? Cursor.getDefaultCursor() : Cursor.getPredefinedCursor(Cursor.HAND_CURSOR));
        stateLabel.setToolTipText(action == null ? null : "Open " + linkText);
        stateLabel.setForeground(TuningStyle.muted());
        stateLabel.setText(action == null ? text : noteHtml(text, linkText));
        noteTimer.restart();
    }

    static String noteHtml(String text, String linkText) {
        Color link = UIManager.getColor("Component.linkColor");
        String color = link == null ? "#1d4ed8" : String.format("#%06x", link.getRGB() & 0xffffff);
        return "<html>" + escapeHtml(text) + " · <u><font color=\"" + color + "\">" + escapeHtml(linkText)
            + "</font></u></html>";
    }

    private static String escapeHtml(String text) {
        return text.replace("&", "&amp;").replace("<", "&lt;").replace(">", "&gt;");
    }

    /** What the Burn note's link opens: the Restore Points window. */
    public void setShowRestorePoints(Runnable showRestorePoints) {
        this.showRestorePoints = showRestorePoints;
    }

    /** The note's link for tests: what a click on the label would run, or null. */
    Runnable getNoteActionForTests() {
        return noteAction;
    }

    public Action getCreateRestorePointAction() {
        return createRestorePointAction;
    }

    void setFileWorkForTests(java.util.concurrent.Executor fileWork) {
        this.fileWork = fileWork;
    }

    /** Saves to the current tune file without asking; asks for a file when there is none yet. */
    public void saveTuneAndThen(CalibrationDialogWidget right, Runnable onSuccess) {
        String current = uiContext.getTuneFilePath();
        if (current != null) {
            saveTuneToPathAndThen(right, current, onSuccess);
        } else {
            saveTuneAsAndThen(right, onSuccess);
        }
    }

    /** Asks for a file, starting at the current tune file, and makes the chosen file the current one. */
    public void saveTuneAsAndThen(CalibrationDialogWidget right, Runnable onSuccess) {
        IniFileModel ini = uiContext.iniFileState.getIniFileModel();
        BinaryProtocol bp = uiContext.getBinaryProtocol();
        ConfigurationImage image = imageToSave(
            right.getWorkingImage(),
            sessionImage.get(),
            bp == null ? null : bp.getControllerConfiguration(),
            baselineImage
        );
        if (ini == null || image == null) {
            errorHandler.accept("No configuration loaded");
            return;
        }
        String current = uiContext.getTuneFilePath();
        if (current != null) {
            saveTuneChooser.setSelectedFile(new File(current));
        } else {
            saveTuneChooser.setSelectedFile(null);
            TuneFileFolder.openIn(saveTuneChooser);
        }
        if (saveTuneChooser.showSaveDialog(getDialogParent()) != JFileChooser.APPROVE_OPTION) {
            return;
        }
        File selected = saveTuneChooser.getSelectedFile();
        TuneFileFolder.remember(selected);
        String path = selected.getAbsolutePath();
        if (!path.toLowerCase().endsWith(".msq")) {
            path += ".msq";
        }
        // JFileChooser does not ask before replacing a file; the current file is replaced by plain Save anyway
        if (new File(path).exists() && !new File(path).equals(current == null ? null : new File(current))
                && JOptionPane.showConfirmDialog(getDialogParent(), new File(path).getName() + " already exists. Replace it?",
                LoadTuneHelper.SAVE_TUNE_AS_TEXT, JOptionPane.YES_NO_OPTION, JOptionPane.WARNING_MESSAGE)
                != JOptionPane.YES_OPTION) {
            return;
        }
        saveTuneToPathAndThen(ini, image, path, onSuccess);
    }

    public void saveTuneToPathAndThen(CalibrationDialogWidget right, String path, Runnable onSuccess) {
        IniFileModel ini = uiContext.iniFileState.getIniFileModel();
        BinaryProtocol bp = uiContext.getBinaryProtocol();
        ConfigurationImage image = imageToSave(
            right.getWorkingImage(),
            sessionImage.get(),
            bp == null ? null : bp.getControllerConfiguration(),
            baselineImage
        );
        if (ini == null || image == null || path == null) {
            errorHandler.accept("No configuration loaded");
            return;
        }
        saveTuneToPathAndThen(ini, image, path, onSuccess);
    }

    private void saveTuneToPathAndThen(IniFileModel ini, ConfigurationImage image, String path, Runnable onSuccess) {
        final String finalPath = path;
        final ConfigurationImage finalImage = image;
        new Thread(() -> {
            try {
                MsqFactory.valueOf(finalImage, ini).writeXmlFile(finalPath);
                SwingUtilities.invokeLater(() -> {
                    uiContext.setTuneFilePath(finalPath);
                    if (onSuccess != null) {
                        onSuccess.run();
                    }
                });
            } catch (Exception ex) {
                SwingUtilities.invokeLater(() -> errorHandler.accept("Failed to save tune: " + ex.getMessage()));
            }
        }, "save-tune").start();
    }

    /**
     * [tag:restore_points] Keeps the tune about to be replaced as a restore point. Never throws: a point that cannot
     * be made is logged and the Burn or Load goes on.
     */
    static boolean saveRestorePoint(RestorePointStore store, ConfigurationImage image, IniFileModel ini,
                                    RestorePointStore.Reason reason) {
        if (store == null || image == null || ini == null) {
            return false;
        }
        try {
            // empty when the newest point already holds this tune: it is kept all the same
            store.save(MsqFactory.valueOf(image, ini), reason, null)
                .ifPresent(point -> log.info("Restore point " + point.file));
            return true;
        } catch (IOException | RuntimeException e) {
            log.error("Failed to make a restore point (" + reason.label + "): " + e, e);
            return false;
        }
    }

    public RestorePointStore getRestorePointStore() {
        return restorePoints;
    }

    public void setRestorePointStoreForTests(RestorePointStore store) {
        restorePoints = store;
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

    public void setErrorHandler(Consumer<String> errorHandler) {
        this.errorHandler = errorHandler;
    }

    public AbstractAction getSaveTuneAction() {
        return saveTuneAction;
    }

    public AbstractAction getSaveTuneAsAction() {
        return saveTuneAsAction;
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
        this.baselineImage = image == null ? null : image.clone();
        discardButton.setEnabled(!burnInProgress && image != null);
        refreshState();
        onBaselineChanged.run();
    }

    /** [tag:unburned_marks] Called after every burn, load or reconnect replaces the baseline. */
    public void setOnBaselineChanged(Runnable listener) {
        onBaselineChanged = listener == null ? () -> { } : listener;
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

    /** File menu actions can run before the lazy Tuning tab is attached to the console. */
    public void setDialogOwner(Window owner) {
        dialogOwner = owner;
    }

    private Component getDialogParent() {
        return dialogOwner != null ? dialogOwner : panel;
    }

    /**
     * Must be called on every edit event (from {@code right.setOnConfigChange}).
     * Captures the pre-edit baseline for undo and restarts the debounce + upload timers.
     */
    public void onEdit(ConfigurationImage previousSessionImage) {
        editGeneration++;
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
        connectionGeneration++;
        undoCommitTimer.stop();
        uploadTimer.stop();
        undoStack.clear();
        redoStack.clear();
        undoBaseline.set(null);
        undoButton.setEnabled(false);
        redoButton.setEnabled(false);
        // Disable burn button when ECU disconnects
        burnButton.setEnabled(false);
        discardButton.setEnabled(!burnInProgress && baselineImage != null);
        loadTuneAction.setEnabled(!firmwareUpdateInProgress && !burnInProgress);
        refreshState();
    }
}
